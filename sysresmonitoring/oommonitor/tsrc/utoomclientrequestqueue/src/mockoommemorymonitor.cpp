/*
 * mockoommemorymonitor.cpp
 *
 *  Created on: Aug 5, 2010
 *      Author: t4sun
 */
#include <e32base.h>
#include <hal.h>
#include <u32hal.h>

#include "oomconstants.hrh"

#include "oommemorymonitor.h"
#include "oomclientrequestqueue.h"
#include "oomconfig.h"
#include "oomconfigparser.h"
#include "oomconstants.hrh"
#include "oomapplicationconfig.h"
#include "oompanic.h"
#include "oommemorymonitorserver.h"
#include "oommonitorclientserver.h"

#include "utoommockclientserver.h"



CMemoryMonitor* CMemoryMonitor::NewL()
    { 

    CMemoryMonitor* self = new(ELeave) CMemoryMonitor();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitor::CMemoryMonitor()
    {
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor::~CMemoryMonitor()
    {

#ifndef CLIENT_REQUEST_QUEUE
    if (iWatchdogStatusSubscriber)
        {
        iWatchdogStatusSubscriber->StopSubscribe();
        }
    iWatchdogStatusProperty.Close();
    delete iWatchdogStatusSubscriber;
#endif
    
    delete iServer;
#ifdef CLIENT_REQUEST_QUEUE
    delete iQueue;
#endif
    
#ifdef _DEBUG    
    delete iLogger;
#endif
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::ConstructL()
    {
    
#ifdef CLIENT_REQUEST_QUEUE
    iQueue = COomClientRequestQueue::NewL(*this);
    
    iServer = CMemoryMonitorServer::NewL(*iQueue);
#else
    iServer = CMemoryMonitorServer::NewL(*this);    
#endif
    
    
    
    }

const COomGlobalConfig& CMemoryMonitor::GlobalConfig()
    {
    TPtrC* par;
    CMemoryMonitor* globalMemoryMonitor = (CMemoryMonitor*)(par);
    return globalMemoryMonitor->iConfig->GlobalConfig();
    
    }



/***
 * FreeOptionalRamL
 * RequestFreeMemoryL
 * RequestFreeMemoryPandSL
 * GoodRamThreshold
 */

void CMemoryMonitor::FreeOptionalRamL(TInt aBytesRequested, TInt aPluginId, TBool aDataPaged) // The ID of the plugin that will clear up the allocation, used to determine the priority of the allocation
    {
    // make mark here
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, Stub_CMemoryMonitor_FreeOptionalRamL );
    
    iActionTrigger = EClientServerRequestOptionalRam;

    iDataPaged = aDataPaged;
       
    StartFreeSomeRamL(aBytesRequested + iGoodRamThreshold, iLowSwapThreshold, 0);
    }

void CMemoryMonitor::RequestFreeMemoryL(TInt aBytesRequested, TBool aDataPaged)
    {
    // make mark here
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, Stub_CMemoryMonitor_RequestFreeMemoryL );
    
    iActionTrigger = EClientServerRequestFreeMemory;
    iDataPaged = aDataPaged;
    StartFreeSomeRamL(iLowRamThreshold, aBytesRequested + iLowSwapThreshold);
    }

void CMemoryMonitor::RequestFreeMemoryPandSL(TInt aBytesRequested)
    {
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, Stub_CMemoryMonitor_RequestFreeMemoryPandSL );
    
    iActionTrigger = EPublishAndSubscribe;
    StartFreeSomeRamL(aBytesRequested + iLowRamThreshold, iLowSwapThreshold);
    }

TInt CMemoryMonitor::GoodRamThreshold() const
    {
    return 0;
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt aFreeRamTarget, TInt aFreeSwapTarget)
    {
    StartFreeSomeRamL(aFreeRamTarget, aFreeSwapTarget, KOomPriorityInfinate - 1);
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt aFreeRamTarget, TInt aFreeSwapTarget, TInt aMaxPriority) // The maximum priority of action to run
    {
    TInt freeMemory = 0;
    
    // put it here for ut test
    // get free memory from property for test
    TInt memoryGood(0);
    RProperty::Get(KUTOomPropertyCategory, KUTOomFreeMemorySetKey, freeMemory );
    RProperty::Get(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, memoryGood );
    RProperty::Set(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, memoryGood+1 );
    if( memoryGood >= 1)
        {
        iQueue->ActionsCompleted(freeMemory, ETrue);
        }
    else
        {
        iQueue->ActionsCompleted(freeMemory, EFalse);
        }
    
    }

void CMemoryMonitor::GetFreeMemory(TInt& aCurrentFreeMemory)
    {
    }
TActionTriggerType CMemoryMonitor::ActionTrigger() const
    {
    return iActionTrigger;
    }

void CMemoryMonitor::RefreshThresholds()
    {
    }

void CMemoryMonitor::SetPriorityBusy(TInt aWgId)
    {
    //FUNC_LOG;

    //iOomWindowGroupList->SetPriorityBusy(aWgId);
    }
void CMemoryMonitor::SetPriorityNormal(TInt aWgId)
    {
    //FUNC_LOG;

    //iOomWindowGroupList->SetPriorityNormal(aWgId);
    }

void CMemoryMonitor::SetPriorityHigh(TInt aWgId)
    {
    //iOomWindowGroupList->SetPriorityHigh(aWgId);
    }

void CMemoryMonitor::AppNotExiting(TInt aWgId)
    {
    //FUNC_LOG;

    //iOomActionList->AppNotExiting(aWgId);
    }

void PanicClient(const RMessagePtr2& aMessage,TOomMonitorClientPanic aPanic)
    {
    //FUNC_LOG;

    aMessage.Panic(KMemoryMonitorServerName, aPanic);
    }

//**********************************
//Global functions
//**********************************

LOCAL_C void MainL()
    {
    //
    // create and install the active scheduler we need
    CActiveScheduler* s=new(ELeave) CActiveScheduler;
    CleanupStack::PushL(s);
    CActiveScheduler::Install(s);
    //
    // create the server (leave it on the cleanup stack)
    CMemoryMonitor* mmPtr = CMemoryMonitor::NewL();
    if(mmPtr!=NULL)
        {
        CleanupStack::PushL(mmPtr);

        // Initialisation complete, now signal the client
        RProcess::Rendezvous(KErrNone);
        //
        // Ready to run
        CActiveScheduler::Start();
        //
        // Cleanup the server
        CleanupStack::PopAndDestroy(1);
        }
    // Cleanup the scheduler
    CleanupStack::PopAndDestroy(1);
    }
/**
    Returns standard error code on exit
*/
GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    //
    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt r=KErrNoMemory;
    if (cleanup)
        {
        TRAP(r,MainL());
        delete cleanup;
        }
    //
    __UHEAP_MARKEND;
    return r;
    }


