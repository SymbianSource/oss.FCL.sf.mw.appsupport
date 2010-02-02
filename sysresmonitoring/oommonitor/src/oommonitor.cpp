/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Definitions of OOM Monitor.
*
*/


#include <hal.h>
#include <s32file.h>
#include <w32std.h>
#include <akneiksrvc.h>
#include <LafMemoryWatcher.rsg>
#include <bautils.h>
#include <apgwgnam.h>
#include <apgtask.h>
#include <barsc.h>
#include <barsread2.h>

#include <e32property.h>
#include <UikonInternalPSKeys.h>
#include <UiklafInternalCRKeys.h>

#include <data_caging_path_literals.hrh>

#include "oommonitor.h"
#include "oommonitorplugin.h"
#include "oommonitorplugin.hrh"
#include "oommonitorclientserver.h"

#define OOM_WATCHDOG_STATUS_IDLE -1

const TInt KPreallocatedSpaceForAppList = 50;
const TInt KPluginFreeMemoryTime = 100000;       // 100 milliseconds for plugins to free memory, overridden by LafMemoryWatcher.rss

_LIT(KDriveZ, "z:");
_LIT(KOOMWatcherResourceFileName, "lafmemorywatcher.rsc");
_LIT(KOOMWatcherThreadName, "OOM FW");
_LIT(KDummyWgName, "20");


// Implements just Error() to avoid panic
class CSimpleScheduler : public CActiveScheduler
    {
    void Error( TInt ) const{} // From CActiveScheduler
    };

// thread function for OOM watcher
GLDEF_C TInt WatcherThreadFunction( TAny* )
    {
    TInt err( KErrNone );

    CTrapCleanup* cleanup = CTrapCleanup::New();
    CActiveScheduler* scheduler = new CSimpleScheduler();
    CMemoryMonitor* oom = NULL;

    if ( !cleanup || !scheduler)
        {
        err = KErrNoMemory;
        }
    else
        {
        CActiveScheduler::Install( scheduler );
        TRAPD( err,
            {
            RThread me;
            me.SetPriority(EPriorityAbsoluteHigh);
            oom = CMemoryMonitor::NewL();
            } )

        if ( err == KErrNone )
            {
            // start the watchers
            CActiveScheduler::Start();
            }
        }

    delete cleanup;
    delete scheduler;
    delete oom;

    return err;
    }

// Creates thread for OOM watchers
EXPORT_C void CreateOOMWatcherThreadL()
    {
    RThread thread;
    TInt ret = thread.Create( KOOMWatcherThreadName,
                              WatcherThreadFunction,
                              0x2000, // stack size
                              NULL, // uses caller thread's heap
                              (TAny*)NULL );

    if ( ret == KErrNone )
        {
        thread.Resume();
        thread.Close();
        }

    User::LeaveIfError( ret );
    }


// ======================================================================
// class CMemoryMonitor
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor* CMemoryMonitor::NewL()
    { // static
    CMemoryMonitor* self = new(ELeave) CMemoryMonitor();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitor::CMemoryMonitor()
: iRamPluginRunTime(KPluginFreeMemoryTime), iCurrentTask(iWs), 
  iMemoryAboveThreshold(ETrue), iPluginMemoryGood(ETrue)
    {
    SetMemoryMonitorTls(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor::~CMemoryMonitor()
    {
    if (iWatchdogStatusSubscriber)
    {
        iWatchdogStatusSubscriber->StopSubscribe();
    }
    iWatchdogStatusProperty.Close();
    delete iWatchdogStatusSubscriber;

    delete iServer;
    delete iWservEventReceiver;
    delete iAppCloseTimer;
    delete iAppCloseWatcher;
    delete iOOMWatcher;
    delete iPlugins;
    iAppCloseOrderMap.Close();
    iWgIds.Close();
    delete iWgName;
    iFs.Close();
    iWs.Close();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::ConstructL()
    {
    User::LeaveIfError(iWs.Connect());

    // Reserve enough space to build an app list later.
    iWgIds.ReserveL(KPreallocatedSpaceForAppList);
    // Reserve enough space for CApaWindowGroupName.
    iWgName = CApaWindowGroupName::NewL(iWs);
    iWgNameBuf = HBufC::NewL(CApaWindowGroupName::EMaxLength);
    (*iWgNameBuf) = KDummyWgName;
    iWgName->SetWindowGroupName(iWgNameBuf);    // iWgName takes ownership of iWgNameBuf

   // Load up threshold & OOM app lists from resource.
    User::LeaveIfError(iFs.Connect());

    RResourceFile resFile;
    TFileName filename(KDriveZ);
    filename.Append(KDC_RESOURCE_FILES_DIR);
    filename.Append(KOOMWatcherResourceFileName);
    BaflUtils::NearestLanguageFile(iFs, filename);
    resFile.OpenL(iFs, filename);
    CleanupClosePushL(resFile);

    HBufC8* thresholds = resFile.AllocReadLC(R_APP_OOM_THRESHOLDS);
    RResourceReader theReader;
    theReader.OpenLC(*thresholds);

    iLowThreshold = theReader.ReadInt32L();
    iGoodThreshold = theReader.ReadInt32L();
    iMaxExitTime = theReader.ReadInt32L();
    TRAP_IGNORE(iRamPluginRunTime = theReader.ReadInt32L());    // trapped in case lafmemorywatcher.rss does not define this value

    CleanupStack::PopAndDestroy(&theReader);
    CleanupStack::PopAndDestroy(thresholds);

    // apps to close first, first app in list gets closed first
    ReadAppResourceArrayL(resFile, R_APP_OOM_EXIT_CANDIDATES, ECloseFirst, +1);	// +1 means apps later in the list are closed later
    // apps to never close, all apps get order ENeverClose
    ReadAppResourceArrayL(resFile, R_APP_OOM_EXIT_NEVER, ENeverClose, 0);	// 0 means that all apps get ENeverClose
    // apps to close last, first app in list gets closed last
    ReadAppResourceArrayL(resFile, R_APP_OOM_EXIT_LAST, ECloseLast, -1);	// -1 means apps later in the list are closed earlier
    
    CleanupStack::PopAndDestroy(); // resFile.Close();

    iPlugins = new(ELeave) COomPlugins;
    iPlugins->ConstructL();

    TInt err = iWatchdogStatusProperty.Attach(KPSUidUikon, KUikOOMWatchdogStatus);
#ifdef _DEBUG
    RDebug::Print(_L("xxxx KUikOOMWatchdogStatus err=%d"), err);
#endif
    err = iWatchdogStatusProperty.Set(OOM_WATCHDOG_STATUS_IDLE);

    iWatchdogStatusSubscriber = new (ELeave) CSubscribeHelper(TCallBack(WatchdogStatusStatusChanged, this), iWatchdogStatusProperty);
    iWatchdogStatusSubscriber->SubscribeL();

    iOOMWatcher = COutOfMemoryWatcher::NewL(*this, iLowThreshold, iGoodThreshold);
    iOOMWatcher->Start();

    iAppCloseTimer = CAppCloseTimer::NewL(*this);
    iAppCloseWatcher = new(ELeave) CAppCloseWatcher(*this);
    
    iWservEventReceiver = new(ELeave) CWservEventReceiver(*this, iWs);
    iWservEventReceiver->ConstructL();

    iServer = CMemoryMonitorServer::NewL(*this);
    }

void CMemoryMonitor::ReadAppResourceArrayL(RResourceFile& aResFile, TInt aResId, TInt aOrderBase, TInt aOrderInc)
    {
    // apps in this list will be ordered starting from aOrderBase
    TInt order = aOrderBase;
    // get the resource and set up the resource reader
    HBufC8* apps = aResFile.AllocReadLC(aResId);
    TResourceReader theReader;
    theReader.SetBuffer(apps);
    // go through all apps in the list
    TInt appsCount = theReader.ReadUint16();
    for (TInt ii = 0; ii < appsCount; ii++)
        {
        TInt appUid = theReader.ReadInt32();
        // insert the app UID with the appropriate order
        iAppCloseOrderMap.Insert(appUid, order);
        // change the order number as appropriate for this list
        order += aOrderInc;
        }
    CleanupStack::PopAndDestroy(apps);
    }

void CMemoryMonitor::CancelAppCloseWatchers()
    {
    iAppCloserRunning = EFalse;
    iCurrentTask.SetWgId(0);
    if (iAppCloseTimer)
        iAppCloseTimer->Cancel();
    if (iAppCloseWatcher)
        iAppCloseWatcher->Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::FreeMemThresholdCrossedL()
    {
    StartFreeSomeRamL(iGoodThreshold);
    }

void CMemoryMonitor::HandleFocusedWgChangeL()
    {
    // The focused window group has changed.
    if (iAppCloserRunning)
        {
        // if the app closer is currently running, restart it
        RestartAppCloser();
        }
    else if (!iMemoryAboveThreshold)
    	{
	    // If memory is low, rescan for free memory
    	StartFreeSomeRamL(iGoodThreshold);
    	}
    }

void CMemoryMonitor::RestartAppCloser()
	{
    CancelAppCloseWatchers();
    // StartFreeSomeRamL is trapped so that clients waiting
    // for completion will definitely receive it.
	TRAPD(err, StartFreeSomeRamL(iCurrentTarget))
	if (err != KErrNone)
        CloseAppsFinished(FreeMemoryAboveThreshold());
	}

// ---------------------------------------------------------
// This function attempts to free enough RAM to leave the target amount of space free.
// This function could take a substantial time to return as it may have to close a number
// of applications, and each will be given a timeout of KAPPEXITTIMEOUT.
// ---------------------------------------------------------
//
void CMemoryMonitor::StartFreeSomeRamL(TInt aTargetFree)
    {
    // update the target if new target is higher
    if (aTargetFree > iCurrentTarget)
        iCurrentTarget = aTargetFree;

    // do nothing more if app closer is already running
    if (iAppCloserRunning)
        return;

       iCurrentTarget = aTargetFree;

    // check if there is enough free memory already.
    if (FreeMemoryAboveThreshold())
        {
        CloseAppsFinished(ETrue);
        return;
        }

    // Tell plugins to free memory
    bool pluginsToldToFreeMemory = iPluginMemoryGood;
    SetPluginMemoryGood(EFalse);

    // get the list of apps to free
    GetWgsToCloseL();

    if (pluginsToldToFreeMemory)
        {
        // Give the plugins a short time to free memory.
        // App close timer will kick off the app closer.
        iAppCloseTimer->After(iRamPluginRunTime);
        }
    else
        {
        // start closing apps
        CloseNextApp();
        }
    }

void CMemoryMonitor::CloseNextApp()
    {
    if(iNextAppToClose >= 0)
        {
        // close an app, if there's an app to be closed
#ifdef _DEBUG
        RDebug::Print(_L("OOM WATCHER: Target not achieved; continuing... Target:%d"),iCurrentTarget);
#endif
		// CloseNextApp() may have been called by one of the event
		// watchers, cancel them all to prevent more events from the
		// last app before restarting the watchers for the new app to close
        CancelAppCloseWatchers();
        iAppCloserRunning = ETrue;
        // Set the TApaTask to the app
        iCurrentTask.SetWgId(iWgIds[iNextAppToClose].iId);
        // Start a timer and the thread watcher
        iAppCloseTimer->After(iMaxExitTime);
        iAppCloseWatcher->Start(iCurrentTask);
        // Tell the app to close
        iCurrentTask.EndTask();
        iNextAppToClose--;
        }
    else
        {
        // stop if we have no more apps
        CloseAppsFinished(EFalse);
        }
    }

// handle an app closed event
void CMemoryMonitor::CloseAppEvent()
    {
    if (FreeMemoryAboveThreshold())
        {
        // stop if we have enough memory
        CloseAppsFinished(ETrue);
        }
    else
        {
        // otherwise try to close another app
        CloseNextApp();
        }
    }

// The app closer is finished
void CMemoryMonitor::CloseAppsFinished(TBool aMemoryGood)
    {
    CancelAppCloseWatchers();
#ifdef _DEBUG
    RDebug::Print(_L("OOM WATCHER: Final result: Target:%d Good?:%d"),iCurrentTarget,aMemoryGood);
#endif
    iServer->CloseAppsFinished(aMemoryGood);
    // plugins can start using memory if result is good
    SetPluginMemoryGood(aMemoryGood);
    }

TBool CMemoryMonitor::FreeMemoryAboveThreshold()
    {
    // may cause some extra load but allows more precise action
    User::CompressAllHeaps();

    TInt current = 0;
    HAL::Get( HALData::EMemoryRAMFree, current );

#ifdef _DEBUG
    RDebug::Print(_L("OOM WATCHER: Free RAM now:%d "),current);
#endif

	iMemoryAboveThreshold = (current >= iCurrentTarget);
    return iMemoryAboveThreshold;
    }

void CMemoryMonitor::GetWgsToCloseL()
    {
    // get all window groups, with info about parents
    TInt numGroups = iWs.NumWindowGroups(0);
    iWgIds.ReserveL(numGroups);
    User::LeaveIfError(iWs.WindowGroupList(0, &iWgIds));

    // Remove all child window groups, promote parents to foremost child position
    ColapseWindowGroupTree();
    
    // now rearange the list so that it only contains apps to close
    // first remove the foreground window group
    iWgIds.Remove(0);

    // go through the list from start to end.
    // this divides the list into two sections. Apps that never
    // close move towards the end of the list, apps that will close
    // go towards the start.
    // apps to close first will be placed further back in the close list.
    TInt numAppsToClose = 0;		// numAppsToClose marks the boundary between closing and non-closing apps
    TInt count = iWgIds.Count();
    for (TInt ii=0; ii<count; ii++)
        {
        ASSERT(ii >= numAppsToClose);	// we are always looking at apps after the sorted section
        // get the next window group
        RWsSession::TWindowGroupChainInfo info = iWgIds[ii];
        // get the close order for the window group
        TInt closeOrder = AppCloseOrder(ii, info.iId);
        if (closeOrder == ENeverClose)
            {
            // leave apps which should not be closed in place,
            // after the last app to close.
            continue;
            }
        else
            {
            // We no longer need the parent id. Use it to store the close order, to avoid the need to allocate more array space
            info.iParentId = closeOrder;
               // remove the app from it's current position
            iWgIds.Remove(ii);
            // find the right place to insert the app (lower orders are put further back)
            TInt insertPos;
            for (insertPos = 0; insertPos < numAppsToClose; insertPos++)
                {
                // compare this close order with window groups already in the list,
                // whose close order is stored in iParentId
                if (closeOrder > iWgIds[insertPos].iParentId)
                    break;
                }
            // Insert the app in the correct place
            ASSERT(insertPos <= ii && insertPos <= numAppsToClose);	// apps to close are always moved to front, before the close boundary
            iWgIds.Insert(info, insertPos);
            numAppsToClose++;
            }
        }
    
    // start closing apps from the end of the list of apps to close
    iNextAppToClose = numAppsToClose - 1;
    }

// Calculate the order number in which this app should be closed
// This is based on the app's UID and its window group Z-order
// Apps given a lower order will be closed before those with a
// higher order.
TInt CMemoryMonitor::AppCloseOrder(TInt aWgIndex, TInt aWgId)
    {
    // get the app's details
    TPtr wgPtr(iWgNameBuf->Des());
    if (iWs.GetWindowGroupNameFromIdentifier(aWgId, wgPtr) != KErrNone)
        return ENeverClose;
    iWgName->SetWindowGroupName(iWgNameBuf);
    TUid uid = iWgName->AppUid(); // This UID comes from the app, not the mmp!
    TInt* order = iAppCloseOrderMap.Find(uid.iUid);

    // The default app close order is normal with further 
    // back apps getting a lower order
    TInt closeOrder = ECloseNormal - aWgIndex;
    if (order)
        {
        // Apps with a defined close order get that order.
        closeOrder = *order;
        }
    else if (uid.iUid == 0 || iWgName->IsSystem() || iWgName->Hidden() || iWgName->IsBusy())
        {
        // Apps that should never close get the ENeverClose rank
        closeOrder = ENeverClose;
        }

    return closeOrder;
    }

void CMemoryMonitor::SetPluginMemoryGood(TBool aSetToGood)
    {
    if (aSetToGood && !iPluginMemoryGood)
        iPlugins->MemoryGood();
    else if (!aSetToGood && iPluginMemoryGood)
        iPlugins->FreeRam();
    iPluginMemoryGood = aSetToGood;
    }


// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt CMemoryMonitor::WatchdogStatusStatusChanged(TAny* aPtr)
    {
    CMemoryMonitor* self = STATIC_CAST(CMemoryMonitor*,aPtr);
    if (self)
        self->HandleWatchdogStatusCallBackL();
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::HandleWatchdogStatusCallBackL()
    {
    // Someone has set the key to request some free memory.
    TInt target = 0;
    iWatchdogStatusProperty.Get(target);

    // Try to free the RAM.
    if (target >= 1)
        {
        iOOMWatcher->Cancel();              // Pause memory notifys.
        TRAP_IGNORE(StartFreeSomeRamL(target)); // This call could take a few seconds to do its stuff.
        iOOMWatcher->Start();               // Restarts memory monitoring.
        }
    // Set the key back to OOM_WATCHDOG_STATUS_IDLE to indicate we're done.
    iWatchdogStatusProperty.Set(OOM_WATCHDOG_STATUS_IDLE);
    }

// ---------------------------------------------------------
// Remove child window groups. Promote parent window groups forward to child position
// ---------------------------------------------------------
//
void CMemoryMonitor::ColapseWindowGroupTree()
    {
    // start from the front, wg count can reduce as loop runs
    for (TInt ii=0; ii<iWgIds.Count();)
        {
        RWsSession::TWindowGroupChainInfo& info = iWgIds[ii];
        if (info.iParentId > 0)        // wg has a parent
            {
            // Look for the parent position
            TInt parentPos = ii;        // use child pos as not-found signal
            TInt count = iWgIds.Count();
            for (TInt jj=0; jj<count; jj++)
                {
                if (iWgIds[jj].iId == info.iParentId)
                    {
                    parentPos = jj;
                    break;
                    }
                }

            if (parentPos > ii)  // parent should be moved forward
                {
                iWgIds[ii] = iWgIds[parentPos];
                iWgIds.Remove(parentPos);
                }
            else if (parentPos < ii)  // parent is already ahead of child, remove child
                iWgIds.Remove(ii);
            else                    // parent not found, skip
                ii++;
            }
        else    // wg does not have a parent, skip
            ii++;
        }
    }

void CMemoryMonitor::AppNotExiting(TInt aWgId)
    {
    if (aWgId == iCurrentTask.WgId())
        CloseAppEvent();
    }


// ======================================================================
// class COutOfMemoryWatcher
// - notifies when free memory crosses preset thresholds
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher* COutOfMemoryWatcher::NewL(CMemoryMonitor& aMonitor, TInt aLowThreshold, TInt aGoodThreshold)
    {
    COutOfMemoryWatcher* self = new (ELeave) COutOfMemoryWatcher(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL(aLowThreshold, aGoodThreshold);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher::~COutOfMemoryWatcher()
    {
    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher::COutOfMemoryWatcher(CMemoryMonitor& aMonitor)
:   CActive(CActive::EPriorityStandard),
    iLafShutdown(aMonitor)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::ConstructL(TInt aLowThreshold, TInt aGoodThreshold)
    {
    UserSvr::SetMemoryThresholds(aLowThreshold,aGoodThreshold);
    User::LeaveIfError(iChangeNotifier.Create());
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::Start()
    {
    if (!IsActive())
        {
        iChangeNotifier.Logon(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::DoCancel()
    {
    iChangeNotifier.LogonCancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::RunL()
    {
    TInt status = iStatus.Int();

    if (status < 0)
        {
        User::Leave(status);
        }

    // Check for memory status change.
    if (status & EChangesFreeMemory)
        {
        iLafShutdown.FreeMemThresholdCrossedL();
        }

    // We are not active until FreeMemThresholdCrossedL returns.
    Start();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CSubscribeHelper::CSubscribeHelper(TCallBack aCallBack, RProperty& aProperty)
    : CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CSubscribeHelper::~CSubscribeHelper()
    {
    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::SubscribeL()
    {
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::StopSubscribe()
    {
    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::RunL()
    {
    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        SubscribeL();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::DoCancel()
    {
    iProperty.Cancel();
    }


COomPlugins::COomPlugins()
    {
    }

COomPlugins::~COomPlugins()
    {
    TInt count = iPlugins.Count();
    for (TInt ii=0; ii<count; ii++)
        {
        TPlugin& plugin = iPlugins[ii];
        if (plugin.iImpl)    // only if successfully added
            REComSession::DestroyedImplementation(plugin.iDtorUid);
        }
    iPlugins.Close();
    }

void COomPlugins::ConstructL()
    {
    RImplInfoPtrArray implArray;
    CleanupClosePushL(implArray);
    REComSession::ListImplementationsL(KOomPluginInterfaceUid, implArray);

    TInt count = implArray.Count();
    iPlugins.ReserveL(count);

    for (TInt ii=0; ii<count; ii++)
        {
        iPlugins.AppendL(TPlugin());
        TPlugin& plugin = iPlugins[ii];
        TUid uid(implArray[ii]->ImplementationUid());
        plugin.iImpl = static_cast<COomMonitorPlugin*>(REComSession::CreateImplementationL(uid, plugin.iDtorUid, NULL));
        }
    
    CleanupStack::PopAndDestroy(&implArray);
    }

void COomPlugins::FreeRam()
    {
    TInt count = iPlugins.Count();
    for (TInt ii=0; ii<count; ii++)
        {
        TPlugin& plugin = iPlugins[ii];
        plugin.iImpl->FreeRam();
        }
    }

void COomPlugins::MemoryGood()
    {
    TInt count = iPlugins.Count();
    for (TInt ii=0; ii<count; ii++)
        {
        TPlugin& plugin = iPlugins[ii];
        plugin.iImpl->MemoryGood();
        }
    }

COomPlugins::TPlugin::TPlugin()
: iImpl(0)
    {
    }


CAppCloseTimer* CAppCloseTimer::NewL(CMemoryMonitor& aMonitor)
    {
    CAppCloseTimer* self = new(ELeave)CAppCloseTimer(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CAppCloseTimer::CAppCloseTimer(CMemoryMonitor& aMonitor)
: CTimer(CActive::EPriorityStandard), iMonitor(aMonitor)
    {
    CActiveScheduler::Add(this);
    }

void CAppCloseTimer::RunL()
    {
    iMonitor.CloseAppEvent();
    }



CAppCloseWatcher::CAppCloseWatcher(CMemoryMonitor& aMonitor)
: CActive(CActive::EPriorityStandard), iMonitor(aMonitor)
    {
    CActiveScheduler::Add(this);
    }

CAppCloseWatcher::~CAppCloseWatcher()
    {
    Cancel();
    }

void CAppCloseWatcher::Start(const TApaTask& aTask)
    {
    TInt err = iThread.Open(aTask.ThreadId());
    if (err == KErrNone)
        {
        iOriginalProcessPriority = iThread.ProcessPriority();
        iThread.SetProcessPriority(EPriorityForeground);
        iThread.Logon(iStatus);
        SetActive();
        }
    else
        {
        iStatus = KRequestPending;
        TRequestStatus* s = &iStatus;
        User::RequestComplete(s, err);
        SetActive();
        }
    }

void CAppCloseWatcher::DoCancel()
    {
    iThread.LogonCancel(iStatus);
    iThread.SetProcessPriority(iOriginalProcessPriority);
    iThread.Close();
    }

void CAppCloseWatcher::RunL()
    {
    if (iThread.Handle())
        iThread.SetProcessPriority(iOriginalProcessPriority);
    iThread.Close();
    // Experimentation shows that memory may take up to 40ms
    // to be released back to the system after app thread close.
    // Using this delay should minimise the number of apps that
    // need to be closed to recover the necessary memory.
    const TInt KAppTidyUpDelay = 40000;
    User::After(KAppTidyUpDelay);
    iMonitor.CloseAppEvent();
    }



CWservEventReceiver::CWservEventReceiver(CMemoryMonitor& aMonitor, RWsSession& aWs)
: CActive(CActive::EPriorityStandard), iMonitor(aMonitor), iWs(aWs), iWg(aWs)
    {
    CActiveScheduler::Add(this);
    }

CWservEventReceiver::~CWservEventReceiver()
    {
    Cancel();
    iWg.Close();
    }

void CWservEventReceiver::ConstructL()
    {
    User::LeaveIfError(iWg.Construct((TUint32)this, EFalse));
    iWg.SetOrdinalPosition(0, ECoeWinPriorityNeverAtFront);
    iWg.EnableFocusChangeEvents();
    Queue();
    }

void CWservEventReceiver::Queue()
    {
    iWs.EventReady(&iStatus);
    SetActive();
    }

void CWservEventReceiver::DoCancel()
    {
    iWs.EventReadyCancel();
    }

void CWservEventReceiver::RunL()
    {
    TWsEvent event;
    iWs.GetEvent(event);
    if (event.Type() == EEventFocusGroupChanged)
        iMonitor.HandleFocusedWgChangeL();
    Queue();
    }



CMemoryMonitorServer* CMemoryMonitorServer::NewL(CMemoryMonitor& aMonitor)
    {
    CMemoryMonitorServer* self=new(ELeave) CMemoryMonitorServer(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitorServer::~CMemoryMonitorServer()
    {
    }

CMemoryMonitorServer::CMemoryMonitorServer(CMemoryMonitor& aMonitor)
:CServer2(CActive::EPriorityStandard), iMonitor(aMonitor)
    {
    }

void CMemoryMonitorServer::ConstructL()
    {
    StartL(KMemoryMonitorServerName);
    }

CSession2* CMemoryMonitorServer::NewSessionL(const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/) const
    {
    return new(ELeave) CMemoryMonitorSession();
    }

TInt CMemoryMonitorServer::RunError(TInt aError)
    {
    Message().Complete(aError);
    //
    // The leave will result in an early return from CServer::RunL(), skipping
    // the call to request another message. So do that now in order to keep the
    // server running.
    ReStart();
    return KErrNone;    // handled the error fully
    }

CMemoryMonitor& CMemoryMonitorServer::Monitor()
    {
    return iMonitor;
    }

void CMemoryMonitorServer::CloseAppsFinished(TBool aMemoryGood)
    {
    iSessionIter.SetToFirst();
    CSession2* s;
    while ((s = iSessionIter++) != 0)
        static_cast<CMemoryMonitorSession*>(s)->CloseAppsFinished(aMemoryGood);
    }


CMemoryMonitorSession::CMemoryMonitorSession()
    {
    }

void CMemoryMonitorSession::CreateL()
    {
    }

CMemoryMonitorSession::~CMemoryMonitorSession()
    {
    }

CMemoryMonitorServer& CMemoryMonitorSession::Server()
    {
    return *static_cast<CMemoryMonitorServer*>(const_cast<CServer2*>(CSession2::Server()));
    }

CMemoryMonitor& CMemoryMonitorSession::Monitor()
    {
    return Server().Monitor();
    }

void CMemoryMonitorSession::ServiceL(const RMessage2& aMessage)
    {
    aMessage.HasCapabilityL(ECapabilityWriteDeviceData); //Leaves if client has not correct capability
    switch (aMessage.Function())
        {
        case EOomMonitorRequestFreeMemory:
            if (!iRequestFreeRam.IsNull())
                PanicClient(aMessage, EPanicRequestActive);
            // message will be completed when CloseAppsFinished() is called.
            iRequestFreeRam = aMessage;
            Monitor().StartFreeSomeRamL(aMessage.Int0());
            break;

        case EOomMonitorCancelRequestFreeMemory:
            if (!iRequestFreeRam.IsNull())
                iRequestFreeRam.Complete(KErrCancel);
            aMessage.Complete(KErrNone);
            break;

        case EOomMonitorThisAppIsNotExiting:
            Monitor().AppNotExiting(aMessage.Int0());
            aMessage.Complete(KErrNone);
            break;

        default:
            PanicClient(aMessage, EPanicIllegalFunction);
            break;
        }
    }

void CMemoryMonitorSession::CloseAppsFinished(TBool aMemoryGood)
    {
    if (!iRequestFreeRam.IsNull())
        iRequestFreeRam.Complete(aMemoryGood ? KErrNone : KErrNoMemory);
    }


void PanicClient(const RMessagePtr2& aMessage,TOomMonitorClientPanic aPanic)
    {
    aMessage.Panic(KMemoryMonitorServerName, aPanic);
    }

// End of file.
