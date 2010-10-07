/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains STIFUnit implementation.
*
*/

/**
 * STIF_UNIT_INCLUDE SECTION - put all #includes between STIF_UNIT_INCLUDE_SECTION
 *                             and STIF_UNIT_INCLUDE_SECTION_END
 */
#ifdef STIF_UNIT_INCLUDE_SECTION
#include "oomconstants.hrh"
#include "oomcloseapp.h"
#include "oomappclosetimer.h"
#include "oomappclosewatcher.h"
#include "moc_oomactionlist.h"
#include "oomactionref.h"
#include "oommemorymonitor.h"
#include <apgwgnam.h> 
#include <oomwindowgrouplist.h>
#include <oomwserveventreceiver.h>
#include <mytimer.h>
#include <e32debug.h>
#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
    CActiveScheduler* scheduler;
#endif
/**
 * END OF GLOBAL VARIABLES SECTION
 */


/**
 * TEST CASES SECTION
 */
#ifdef TEST_CASES
/**
 * STIF_SETUP defines activities needed before every test case.
 */
STIF_SETUP
{
    // Install active scheduler at start up
    scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
    // Delete active scheduler at last
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
}

/**
 * @SYMTestCaseID              GAPS-OOM-WINDOWGROUPLIST-001
 * @SYMTestCaseDesc            Verify the behaviors of oomwindowgrouplist.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Get the list of window group of current system.
 *                             2. Create appfortest process.
 *                             3. Get the list of window group of current system.
 *                             4. Get the wgID of appfortest process.
 *                             5. Create COomWindowGroupList object.
 *                             6. Get window group name.
 *                             7. Get index from app ID.
 *                             8. Get app ID from index.
 *                             9. Get Idle time 1 of appfortest.
 *                             10. Wait 1 second.
 *                             11. Get Idle time 2 of appfortest.
 *                             12. Compare Idle times.
 *                             13. Set appfortest priority busy.
 *                             14. Check appfortest priority.
 *                             15. Set appfortest priority high.
 *                             16. Check appfortest priority.
 *                             17. Set appfortest priority normal.
 *                             18. Check appfortest priority.
 * @SYMTestExpectedResults     1. Get the window group list of current system successfully.
 *                             2. Create appfortest process successfully.
 *                             3. Get the window group list of current system successfully.
 *                             4. Get the wgID of appfortest process successfully.
 *                             5. Create COomWindowGroupList object successfully.
 *                             6. Get window group name successfully.
 *                             7. Get index from app ID successfully.
 *                             8. Get app ID from index successfully.
 *                             9. Get Idle time 1 of appfortest successfully.
 *                             10. Wait 1 second.
 *                             11. Get Idle time of appfortest successfully.
 *                             12. Idle time 2 greater than or equal to idle time 1.
 *                             13. Set appfortest priority busy successfully.
 *                             14. Appfortest priority is set to busy.
 *                             15. Set appfortest priority high successfully.
 *                             16. Appfortest priority is set to high.
 *                             17. Set appfortest priority normal successfully.
 *                             18. Appfortest priority is set to normal.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */

STIF_TESTDEFINE(GAPS-OOM-WINDOWGROUPLIST-001)
{
    TInt retrunValue=KErrNone;
        
    RWsSession wsSession;
    CleanupClosePushL(wsSession);
    User::LeaveIfError(wsSession.Connect());
    TInt numGroups = wsSession.NumWindowGroups(0);
    
    //Get the wgID list before appfortest.exe start
    RArray<RWsSession::TWindowGroupChainInfo> wgIds;
    wgIds.ReserveL(numGroups);
    User::LeaveIfError(wsSession.WindowGroupList(0, &wgIds));
    
    //Create appfortest.exe process
    RProcess process;
    CleanupClosePushL(process);
    User::LeaveIfError(process.Create(KExeName, KNullDesC) );      
    process.Resume();   
    User::After(5000000);   // wait process start...
    
    // Get the app id of appfortest.exe
    TSecureId AppSecuID=process.SecureId();
    
    // Check wgNumber
    TInt numGroups1 = wsSession.NumWindowGroups(0);    
    // STIF_ASSERT_EQUALS(numGroups+1, numGroups1);
    
    //Get the wgID list after appfortest.exe started
    RArray<RWsSession::TWindowGroupChainInfo> wgIds1;
    wgIds1.ReserveL(numGroups1);
    User::LeaveIfError(wsSession.WindowGroupList(0, &wgIds1));  
    
    // Get teh wgID of appfortest.exe
    for(TInt count=0; count<numGroups; count++)
        {
        TInt position=wgIds1.Find(wgIds[count]);
        wgIds1.Remove(position);
        }

    TInt wgID=-1;
    if(1 == wgIds1.Count())
        {
        wgID=wgIds1[0].iId;
        }
    else
        {
        iLog->Log(_L("Failed to get wgID of appfortest.exe!\n"));
        retrunValue = KErrNotFound;
        }
    
    if(KErrNone == retrunValue)
        {
        // Create wsSession object
        COomWindowGroupList* oomWindowGroupList = COomWindowGroupList::NewL(wsSession);
        CleanupStack::PushL(oomWindowGroupList);
        
        // Test WgName
        CApaWindowGroupName* wgName=oomWindowGroupList->WgName();
        TPtrC potint=wgName->WindowGroupName();      
        if(wgName->WindowGroupName()==KDummyWgName)
            {
            iLog->Log(_L("Get WgName successfully!\n"));
                        }
        else
            {
            iLog->Log(_L("Failed to get windowgroup name!\n"));
            retrunValue = KErrGeneral;
            }
        
        // Test GetIndexFromAppId and AppID
        TInt index=oomWindowGroupList->GetIndexFromAppId(AppSecuID.iId);
        
        TUint appID=oomWindowGroupList->AppId(index, ETrue);
        if(AppSecuID.iId==appID)
            {
            iLog->Log(_L("Get AppID successfully!\n"));
            }
        else
            {
            iLog->Log(_L("Failed to get AppID!\n"));
            retrunValue = KErrGeneral;
            }
        
        // Test IdleTime
        TTimeIntervalSeconds time1=oomWindowGroupList->IdleTime(index);
        User::After(1000000);
        TTimeIntervalSeconds time2=oomWindowGroupList->IdleTime(index);
        if(time2>=time1)
            {
            iLog->Log(_L("IdleTime test passed!\n"));
            }
        else
            {
            iLog->Log(_L("IdleTime test failed!\n"));
            retrunValue = KErrGeneral;
            }
        
        // Test priority
        oomWindowGroupList->SetPriorityBusy(wgID);
        if(!oomWindowGroupList->IsDynamicHighPriority(index) && oomWindowGroupList->IsBusy(index))
            {
            iLog->Log(_L("Test priority passed 1!\n"));
            }
        else
            {
            iLog->Log(_L("Test priority failed 1!\n"));
            retrunValue = KErrGeneral;
            }
        
        oomWindowGroupList->SetPriorityHigh(wgID);
        if(!oomWindowGroupList->IsBusy(index) && oomWindowGroupList->IsDynamicHighPriority(index))
            {
            iLog->Log(_L("Test priority passed 2!\n"));
            }
        else
            {
            iLog->Log(_L("Test priority failed 2!\n"));
            retrunValue = KErrGeneral;
            }
       
        oomWindowGroupList->SetPriorityNormal(wgID);
        if(!oomWindowGroupList->IsBusy(index) && !oomWindowGroupList->IsDynamicHighPriority(index))
            {
            iLog->Log(_L("Test priority passed 3!\n"));
            }
        else
            {
            iLog->Log(_L("Test priority failed 3!\n"));
            retrunValue = KErrGeneral;
            }
        CleanupStack::PopAndDestroy(oomWindowGroupList);
        }

    wgIds.Close();
    wgIds1.Close();
    process.Kill(KErrNone);
    
    User::After(5000000);   // waiting to kill process ...
    CleanupStack::PopAndDestroy(&process);
    CleanupStack::PopAndDestroy(&wsSession);

    STIF_ASSERT_EQUALS(retrunValue, KErrNone);
}

/**
 * @SYMTestCaseID              GAPS-OOM-WINDOWGROUPLIST-001
 * @SYMTestCaseDesc            Verify the behaviors of oomwserveventreceiver.cpp.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create and install CActiveScheduler object.
 *                             2. Create RWsSession object.
 *                             3. Create CWservEventReceiver object and add it into ActiveScheduler.
 *                             4. Create CMyTimer object.
 *                             5. Start ActiveScheduler.
 *                             6. Create appfortest.exe process.
 *                             7. Close appfortest.exe process. Call the cancel method of CTimer object.
 *                             8. Create appfortest.exe process.
 *                             9. Close appfortest.exe process.
 *                             10. Stop CActiveScheduler.
 * @SYMTestExpectedResults     1. Create and install CActiveScheduler object successfully.
 *                             2. Create RWsSession object successfully.
 *                             3. Create CWservEventReceiver object and add it into ActiveScheduler successfully.
 *                             4. Create CMyTimer object successfully.
 *                             5. Start ActiveScheduler successfully.
 *                             6. Create appfortest.exe process successfully. WservEventReceiver received focus change event.
 *                             7. Close appfortest.exe process successfully. cancel method is called.
 *                             8. Create CMyTimer object successfully. WservEventReceiver will not receive focus change event.
 *                             9. Close appfortest.exe process successfully.
 *                             10. Stop CActiveScheduler successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-WSERVEVENTRECEIVER-001)
{   
    iLog->Log(_L("Start oomwserveventreceiver.cpp test!\n"));
    TInt Count=0;
    Dll::SetTls(&Count);
        
    RWsSession wsSession;
    CleanupClosePushL(wsSession);
    User::LeaveIfError(wsSession.Connect());
    
    CMemoryMonitor* oom=NULL;
    CWservEventReceiver* winServEventReceiver = new(ELeave) CWservEventReceiver(*oom, wsSession);
    CleanupStack::PushL(winServEventReceiver);
    winServEventReceiver->ConstructL();
    
    CMyTimer* timer=CMyTimer::NewLC(winServEventReceiver);
    iLog->Log(_L("Start CActiveScheduler!\n"));
    CActiveScheduler::Start();

    iLog->Log(_L("CActiveScheduler stopped!\n"));
    
    CleanupStack::PopAndDestroy(3, &wsSession);
        
    TInt* pointer=(int*)(Dll::Tls());
    TInt* Count1=STATIC_CAST(TInt*, pointer);   
    iLog->Log(_L("count=%d\n"),*Count1);
    
    STIF_ASSERT_EQUALS(*Count1, 2);
    iLog->Log(_L("End of oomwserveventreceiver.cpp test!\n"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-CLOSEAPP-001
 * @SYMTestCaseDesc            Unit test for COomCloseApp, COomAppCloseTimer, and COomAppCloseWatcher. Given process is data caged.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Connect window session.
 *                             2. Get the wgID list before appfortest.exe start.
 *                             3. Create appfortest.exe process.
 *                             4. Get the app id of eshell.
 *                             5. Get the wgID list after appfortest.exe started.
 *                             6. Get teh wgID of appfortest.exe.
 *                             7. Construct a COomCloseApp object.
 *                             8. Reconfigure the reference to an action.
 *                             9. Close the application in order to free memory.
 *                             10.Close windows group list
 * @SYMTestExpectedResults     1. Windows session connected.
 *                             2. wgID list was got.
 *                             3. appfortest was created.
 *                             4. App id was got.
 *                             5. wgID list was got again.
 *                             6. wgID of appfortest.exe was got.
 *                             7. COomCloseApp object was constructed.
 *                             8. The reference to an action was reconfigured.
 *                             9. appfortest was closed.
 *                             10. Windows group list was closed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-CLOSEAPP-001)
{    
    STIF_LOG("1. Connect window session.");
    RWsSession wsSession;
    CleanupClosePushL(wsSession);
    User::LeaveIfError(wsSession.Connect());
    TInt numGroups = wsSession.NumWindowGroups(0);

    STIF_LOG("2. Get the wgID list before appfortest.exe start.");
    RArray<RWsSession::TWindowGroupChainInfo> wgIds;
    CleanupClosePushL(wgIds);
    wgIds.ReserveL(numGroups);
    User::LeaveIfError(wsSession.WindowGroupList(0, &wgIds));

    STIF_LOG("3. Create appfortest.exe process.");
    RProcess process;
    CleanupClosePushL(process);
    User::LeaveIfError(process.Create(KExeName, KArguments) );      
    process.Resume();   
    User::After(5000000);   // wait process start...
    
    STIF_LOG("4. Get the app id of eshell.");
    TSecureId AppSecuID=process.SecureId();
    
    CleanupStack::PopAndDestroy(&process);

    TInt numGroups1 = wsSession.NumWindowGroups(0);    
    
    STIF_LOG("5. Get the wgID list after appfortest.exe started.");
    RArray<RWsSession::TWindowGroupChainInfo> wgIds1;
    CleanupClosePushL(wgIds1);
    wgIds1.ReserveL(numGroups1);
    User::LeaveIfError(wsSession.WindowGroupList(0, &wgIds1));  

    STIF_LOG("6. Get teh wgID of appfortest.exe.");
    for(TInt count=0; count<numGroups; count++)
        {
        TInt position=wgIds1.Find(wgIds[count]);
        wgIds1.Remove(position);
        }

    TInt wgID=-1;
    if(1 == wgIds1.Count())
        {
        wgID=wgIds1[0].iId;
        }
    else
        {
        STIF_LOG("Failed to get wgID of appfortest.exe.");
        User::Leave(KErrNotFound);
        }
    
    CMemoryMonitor* memoryMonitor = CMemoryMonitor::NewL();
    CleanupStack::PushL(memoryMonitor);
    
    CMoc_COomActionList* oomActList = new (ELeave) CMoc_COomActionList();
    CleanupStack::PushL(oomActList);

    STIF_LOG("7. Construct a COomCloseApp object.");
    COomCloseApp* oomCloseApp = COomCloseApp::NewL(*oomActList, wsSession);
    CleanupStack::PushL(oomCloseApp);
    
    // 
    STIF_LOG("8. Reconfigure the reference to an action.");
    const TInt KBytesInMegabyte = 1024;
    TActionRef actionRef(TActionRef::EAppClose, KOomPriorityInfinate, EContinue, KBytesInMegabyte, wgID, 1);
    
    oomCloseApp->Reconfigure(actionRef);
    
    STIF_LOG("9. Close the application in order to free memory.");
    oomCloseApp->FreeMemory(0, ETrue);

    CleanupStack::PopAndDestroy(oomCloseApp);
    CleanupStack::PopAndDestroy(oomActList);
    CleanupStack::PopAndDestroy(memoryMonitor);
    
    STIF_LOG("10. Close windows group list.");

    CleanupStack::PopAndDestroy(&wgIds1);
    CleanupStack::PopAndDestroy(&wgIds);
    
    User::After(5000000);   // wait resource cleaning...
    
    CleanupStack::PopAndDestroy(&wsSession);
}

/**
 * @SYMTestCaseID              GAPS-OOM-CLOSEAPP-002
 * @SYMTestCaseDesc            Unit test for COomCloseApp, COomAppCloseTimer, and COomAppCloseWatcher. Given process is not data caged.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Connect window session.
 *                             2. Get the wgID list before appfortest.exe start.
 *                             3. Create appfortest.exe process.
 *                             4. Get the app id of eshell.
 *                             5. Get the wgID list after appfortest.exe started.
 *                             6. Get teh wgID of appfortest.exe.
 *                             7. Construct a COomCloseApp object.
 *                             8. Reconfigure the reference to an action.
 *                             9. Close the application in order to free memory.
 *                             10.Close windows group list
 * @SYMTestExpectedResults     1. Windows session connected.
 *                             2. wgID list was got.
 *                             3. appfortest was created.
 *                             4. App id was got.
 *                             5. wgID list was got again.
 *                             6. wgID of appfortest.exe was got.
 *                             7. COomCloseApp object was constructed.
 *                             8. The reference to an action was reconfigured.
 *                             9. appfortest was closed.
 *                             10. Windows group list was closed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-CLOSEAPP-002)
{    
    STIF_LOG("1. Connect window session.");
    RWsSession wsSession;
    CleanupClosePushL(wsSession);
    User::LeaveIfError(wsSession.Connect());
    TInt numGroups = wsSession.NumWindowGroups(0);

    STIF_LOG("2. Get the wgID list before appfortest.exe start.");
    RArray<RWsSession::TWindowGroupChainInfo> wgIds;
    CleanupClosePushL(wgIds);
    wgIds.ReserveL(numGroups);
    User::LeaveIfError(wsSession.WindowGroupList(0, &wgIds));

    STIF_LOG("3. Create appfortest.exe process.");
    RProcess process;
    CleanupClosePushL(process);
    User::LeaveIfError(process.Create(KExeName, KArguments) );      
    process.Resume();   
    User::After(5000000);   // wait process start...
    
    STIF_LOG("4. Get the app id of eshell.");
    TSecureId AppSecuID=process.SecureId();
    
    CleanupStack::PopAndDestroy(&process);

    TInt numGroups1 = wsSession.NumWindowGroups(0);    
    
    STIF_LOG("5. Get the wgID list after appfortest.exe started.");
    RArray<RWsSession::TWindowGroupChainInfo> wgIds1;
    CleanupClosePushL(wgIds1);
    wgIds1.ReserveL(numGroups1);
    User::LeaveIfError(wsSession.WindowGroupList(0, &wgIds1));  

    STIF_LOG("6. Get teh wgID of appfortest.exe.");
    for(TInt count=0; count<numGroups; count++)
        {
        TInt position=wgIds1.Find(wgIds[count]);
        wgIds1.Remove(position);
        }

    TInt wgID=-1;
    if(1 == wgIds1.Count())
        {
        wgID=wgIds1[0].iId;
        }
    else
        {
        STIF_LOG("Failed to get wgID of appfortest.exe.");
        User::Leave(KErrNotFound);
        }
    
    CMemoryMonitor* memoryMonitor = CMemoryMonitor::NewL();
    CleanupStack::PushL(memoryMonitor);
    
    CMoc_COomActionList* oomActList = new (ELeave) CMoc_COomActionList();
    CleanupStack::PushL(oomActList);

    STIF_LOG("7. Construct a COomCloseApp object.");
    COomCloseApp* oomCloseApp = COomCloseApp::NewL(*oomActList, wsSession);
    CleanupStack::PushL(oomCloseApp);
    
    // 
    STIF_LOG("8. Reconfigure the reference to an action.");
    const TInt KBytesInMegabyte = 1024;
    TActionRef actionRef(TActionRef::EAppClose, KOomPriorityInfinate, EContinue, KBytesInMegabyte, wgID, 1);
    
    oomCloseApp->Reconfigure(actionRef);
    
    STIF_LOG("9. Close the application in order to free memory.");
    oomCloseApp->FreeMemory(0, EFalse);

    CleanupStack::PopAndDestroy(oomCloseApp);
    CleanupStack::PopAndDestroy(oomActList);
    CleanupStack::PopAndDestroy(memoryMonitor);
    
    STIF_LOG("10. Close windows group list.");

    CleanupStack::PopAndDestroy(&wgIds1);
    CleanupStack::PopAndDestroy(&wgIds);
    
    User::After(5000000);   // wait resource cleaning...
    
    CleanupStack::PopAndDestroy(&wsSession);
}

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
