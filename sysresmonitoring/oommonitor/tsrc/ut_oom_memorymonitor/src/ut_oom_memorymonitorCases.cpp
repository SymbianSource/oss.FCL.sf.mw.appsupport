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

#include <e32debug.h> // RDebug
#include <hal.h>
#include <u32hal.h>
#include <oommemorymonitor.h> 
#include <oomconfig.h>
#include <oomconstants.hrh>
#include "diclog.h"
#include "globaldata.h"
#include "oomfriendclass.h"

#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
	/**
	 * Example of variable common for some test cases
	 */

	 
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
	/** Example of use of STIF_SETUP - a variable common for some test cases is initialized
	 */

}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
	/** Example of use of STIF_TEARDOWN - a variable common for some test cases is destroyed
	 */

}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-001
 * @SYMTestCaseDesc            Test ConstruL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Check WindowGroupList pointer.
 *                             4. Check OomConfig pointer.
 *                             5. Check ClientRequestQueue pointer.
 *                             6. Check MemoryMonitorServer pointer.
 *                             7. Check ActionList pointer.
 *                             8. Check OomLogger pointer when debug.
 *                             9. Check OutOfMemoryWatcher pointer.
 *                             10. Check EventReceiver pointer.
 *                             11. Check the call of COomConfigParser::ParseL().
 *                             12. Check the call of COutOfMemoryWatcher::Start().
 *                             13. Check the call of COomWindowGroupList::Refresh().
 *                             14. Delete diclog file.
 *                             15. Delete COomFriendClass object.
 *                             16. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object sucessfully.
 *                             3. WindowGroupList pointer is not NULL.
 *                             4. OomConfig pointer is not NULL.
 *                             5. ClientRequestQueue pointer is not NULL.
 *                             6. MemoryMonitorServer pointer is not NULL.
 *                             7. ActionList pointer is not NULL.
 *                             8. OomLogger pointer when debug is not NULL.
 *                             9. OutOfMemoryWatcher pointer is not NULL.
 *                             10. EventReceiver pointer is not NULL.
 *                             11. COomConfigParser::ParseL() is called.
 *                             12. COutOfMemoryWatcher::Start() is called.
 *                             13. COomWindowGroupList::Refresh() is called.
 *                             14. Delete diclog file successfully.
 *                             15. Delete COomFriendClass object successfully.
 *                             16. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-001)
{
    iLog->Log( _L("OomMemoryMonitor: Start test ConstructL()!"));
    iLog->Log( _L("OomMemoryMonitor: Create CMemoryMonitor object!"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log( _L("OomMemoryMonitor: Delete diclog file."));
    
    iLog->Log( _L("OomMemoryMonitor: Create friend class!"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    STIF_ASSERT_NOT_NULL(oomFriendClass);
    
    COomWindowGroupList* oomWindowGroupList=oomFriendClass->GetWindowGroupList(oom);
    iLog->Log( _L("OomMemoryMonitor: oomWindowGroupList=%d"),oomWindowGroupList);
    STIF_ASSERT_NOT_NULL(oomWindowGroupList);
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    iLog->Log( _L("OomMemoryMonitor: oomConfig=%d"),oomConfig);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    COomClientRequestQueue* oomClientRequestQueue=oomFriendClass->GetClientRequestQueue(oom);
    iLog->Log( _L("OomMemoryMonitor: oomClientRequestQueue=%d"),oomClientRequestQueue);
    STIF_ASSERT_NOT_NULL(oomClientRequestQueue);
       
    CMemoryMonitorServer* oomMemoryMonitorServer=oomFriendClass->GetMemoryMonitorServer(oom);
    iLog->Log( _L("OomMemoryMonitor: oomMemoryMonitorServer=%d"),oomMemoryMonitorServer);
    STIF_ASSERT_NOT_NULL(oomMemoryMonitorServer);
    
    COomActionList* oomActionList=oomFriendClass->GetOomActionList(oom);
    iLog->Log( _L("OomMemoryMonitor: oomActionList=%d"),oomActionList);
    STIF_ASSERT_NOT_NULL(oomActionList);

#ifdef _DEBUG     
    COomLogger* oomLogger=oomFriendClass->GetOomLogger(oom);
    iLog->Log( _L("OomMemoryMonitor: oomLogger=%d"),oomLogger);
    STIF_ASSERT_NOT_NULL(oomLogger);
    
#endif    
        
    COutOfMemoryWatcher* oomOutOfMemoryWatcher=oomFriendClass->GetOutOfMemoryWatcher(oom);
    iLog->Log( _L("OomMemoryMonitor: oomOutOfMemoryWatcher=%d"),oomOutOfMemoryWatcher);
    STIF_ASSERT_NOT_NULL(oomOutOfMemoryWatcher);
    
    CWservEventReceiver* oomEventReceiver=oomFriendClass->GetEventReceiver(oom);
    iLog->Log( _L("OomMemoryMonitor: oomEventReceiver=%d"),oomEventReceiver);
    STIF_ASSERT_NOT_NULL(oomEventReceiver);
    
    iLog->Log( _L("OomMemoryMonitor: Check the call of COomConfigParser::ParseL()!"));
    TBuf<KIntValue8> buf(KConfigParserParser);
    TBuf<KIntValue8> buf1;    
    DicLog::ReadLog(TUid::Uid(0x0001), buf1);
    STIF_ASSERT_EQUALS(buf1, buf);
    
    iLog->Log( _L("OomMemoryMonitor: Check the call of COutOfMemoryWatcher::Start()!"));
    TBuf<KIntValue7> buf2(KOutOfMemoryWatcherStart);
    TBuf<KIntValue7> buf3;    
    DicLog::ReadLog(TUid::Uid(0x0002), buf3);
    STIF_ASSERT_EQUALS(buf2, buf3);
    
    iLog->Log( _L("OomMemoryMonitor: Check the call of COomWindowGroupList::Refresh()!"));
    TBuf<KIntValue9> buf4(KWingoupListRefresh);
    TBuf<KIntValue9> buf5;    
    DicLog::ReadLog(TUid::Uid(0x0003), buf5);
    STIF_ASSERT_EQUALS(buf4, buf5);
       
    DicLog::RemoveLogFile();
    iLog->Log( _L("OomMemoryMonitor: Delete diclog file."));
    CleanupStack::PopAndDestroy(oomFriendClass);
    iLog->Log( _L("OomMemoryMonitor: Delete oomFriendClass!"));
    CleanupStack::PopAndDestroy(oom);
    iLog->Log( _L("OomMemoryMonitor: test ConstructL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-002
 * @SYMTestCaseDesc            Test RefreshThresholds() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config.
 *                             4. Call RefreshThresholds().
 *                             5. Check windowgrouplist refresh function is called.
 *                             6. Check GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                value of CMemoryMonitor object.
 *                             7. Delete diclog file.
 *                             8. Delete COomFriendClass object.
 *                             9. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config is set.
 *                             4. RefreshThresholds() function is called.
 *                             5. Refresh function of windowgrouplist is called.
 *                             6. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of CMemoryMonitor object is set as expected.
 *                             7. Delete diclog file successfully.
 *                             8. Delete COomFriendClass object successfully.
 *                             9. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-002)
{
    iLog->Log(_L("OomMemoryMonitor: Start test RefreshThresholds()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log( _L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();

    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    const TInt goodRamThre=100;
    const TInt lowRamThre=200;
    const TInt goodSwapThre=300;
    const TInt lowSwapThre=400;
    
    oomConfig->SetDefaultGoodRamThreshold(goodRamThre);
    oomConfig->SetDefaultLowRamThreshold(lowRamThre);
    oomConfig->SetDefaultGoodSwapThreshold(goodSwapThre);
    oomConfig->SetDefaultLowSwapThreshold(lowSwapThre);
     
    oomFriendClass->CallRefreshThresholds(oom);
      
    iLog->Log( _L("OomMemoryMonitor: Call COomWindowGroupList::Refresh()!\n"));
    TBuf<KIntValue9> buf1(KWingoupListRefresh);
    TBuf<KIntValue9> buf2;    
    DicLog::ReadLog(TUid::Uid(0x0003), buf2);
    iLog->Log(_L("KWingoupListRefresh=%s"),buf2.Ptr());
    STIF_ASSERT_EQUALS(buf1, buf2);
    
    iLog->Log(_L("OomMemoryMonitor: Check return value!\n"));   
    TInt goodRamThreshold=oomFriendClass->GetGoodRamThreshold(oom);
    TInt lowRamThreshold=oomFriendClass->GetLowRamThreshold(oom);
    TInt goodSwapThreshold=oomFriendClass->GetGoodSwapThreshold(oom);
    TInt lowSwapThreshold=oomFriendClass->GetLowSwapThreshold(oom);
    iLog->Log(_L("valuse=%d,%d,%d,%d\n"),&goodRamThreshold, &lowRamThreshold, &goodSwapThreshold, &lowSwapThreshold);
    STIF_ASSERT_EQUALS(goodRamThre,goodRamThreshold);
    STIF_ASSERT_EQUALS(lowRamThre,lowRamThreshold);
    STIF_ASSERT_EQUALS(goodSwapThre,goodSwapThreshold);
    STIF_ASSERT_EQUALS(lowSwapThre,lowSwapThreshold);
        
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log( _L("OomMemoryMonitor: test RefreshThresholds() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-003
 * @SYMTestCaseDesc            Test StartFreeSomeRamL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Call StartFreeSomeRamL() function;
 *                             4. Check COomActionList->FreeMemory(aMaxPriority) is function is called.
 *                             5. Delete diclog file.
 *                             6. Delete COomFriendClass object.
 *                             7. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. StartFreeSomeRamL() function is called.
 *                             4. COomActionList->SwitchOffPlugins() is called.
 *                             5. Delete diclog file successfully.
 *                             6. Delete COomFriendClass object successfully.
 *                             7. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-003)
{
    iLog->Log(_L("OomMemoryMonitor: Start test StartFreeSomeRamL()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log( _L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();

    const TInt KPriority2=35;
    const TInt KTargeRamSize=500*1024*1024;
    oom->StartFreeSomeRamL(KTargeRamSize, KTargeRamSize, KPriority2);
    
    TInt32 maxPriority32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0015), maxPriority32);
    TInt maxPriority=STATIC_CAST(TInt, maxPriority32);
    iLog->Log(_L("OomMemoryMonitor: maxPriority32=%d"), maxPriority32);
    STIF_ASSERT_EQUALS(maxPriority, KPriority2);
        
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test StartFreeSomeRamL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-004
 * @SYMTestCaseDesc            Test HandleFocusedWgChangeL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set the value of iGoodRamThreshold, iLowRamThreshold, iGoodSwapThreshold
 *                                iLowSwapThreshold of CMemoryMonitor object.
 *                             4. Set GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config.
 *                             5. Call HandleFocusedWgChangeL function.
 *                             6. Check iGoodRamThreshold, iLowRamThreshold, iGoodSwapThreshold
 *                                iLowSwapThreshold of CMemoryMonitor object is set correct value.
 *                             7. Check iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             8. Check COomConfig->StartL() is called.
 *                             9. Check COomActionList->BuildActionListL() is called.
 *                             10. Check COomActionList->FreeMemory(aMaxPriority) is called.
 *                             11. Delete diclog file.
 *                             12. Delete COomFriendClass object.
 *                             13. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of iGoodRamThreshold, iLowRamThreshold, iGoodSwapThreshold
 *                                iLowSwapThreshold of CMemoryMonitor object is set.
 *                             4. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config is set.
 *                             5. HandleFocusedWgChangeL function is called.
 *                             6. iGoodRamThreshold, iLowRamThreshold, iGoodSwapThreshold
 *                                iLowSwapThreshold of CMemoryMonitor object is set correct value.
 *                             7. iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             8. COomConfig->StartL() is called.
 *                             9. COomActionList->BuildActionListL() is called.
 *                             10. COomActionList->FreeMemory(aMaxPriority) is called.
 *                             11. Delete diclog file successfully.
 *                             12. Delete COomFriendClass object successfully.
 *                             13. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-004)
{
    iLog->Log(_L("OomMemoryMonitor: Start test HandleFocusedWgChangeL()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log( _L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThreshold);
    oomConfig->SetDefaultLowRamThreshold(KLowRamThreshold);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThreshold);
    oomConfig->SetDefaultLowSwapThreshold(KLowSwapThreshold);
     
    oomFriendClass->CallRefreshThresholds(oom);
    
    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThreshold*KMSize);
    oomConfig->SetDefaultLowRamThreshold(KLowRamThreshold*KMSize);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThreshold*KMSize);
    oomConfig->SetDefaultLowSwapThreshold(KLowSwapThreshold*KMSize);

    oom->HandleFocusedWgChangeL();
    
    TInt32 lowRamThreshold32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0009), lowRamThreshold32);
    TInt lowRamThreshold=STATIC_CAST(TInt, lowRamThreshold32);
    STIF_ASSERT_EQUALS(lowRamThreshold, KLowRamThreshold*KMSize);
    
    TInt32 goodRamThreshold32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0010), goodRamThreshold32);
    TInt goodRamThreshold=STATIC_CAST(TInt, goodRamThreshold32);
    STIF_ASSERT_EQUALS(goodRamThreshold, KGoodRamThreshold*KMSize);
    
    TInt32 lowSwapThreshold32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0011), lowSwapThreshold32);
    TInt lowSwapThreshold=STATIC_CAST(TInt, lowSwapThreshold32);
    STIF_ASSERT_EQUALS((TInt)lowSwapThreshold, KLowSwapThreshold*KMSize);
    
    TInt32 goodSwapThreshold32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0012), goodSwapThreshold32);
    TInt goodSwapThreshold=STATIC_CAST(TInt, goodSwapThreshold32);
    STIF_ASSERT_EQUALS(goodSwapThreshold, KGoodSwapThreshold*KMSize);
    
    TInt ramTarget=oomFriendClass->GetCurrentRamTarget(oom);
    TInt swapTarget=oomFriendClass->GetCurrentSwapTarget(oom);
    
    STIF_ASSERT_EQUALS(ramTarget, KGoodRamThreshold*KMSize);
    STIF_ASSERT_EQUALS(swapTarget, KGoodSwapThreshold*KMSize);

#ifdef _DEBUG     
    TBuf<KIntValue8> buf1(KOomLoggerStartL);
    TBuf<KIntValue8> buf2;    
    DicLog::ReadLog(TUid::Uid(0x0013), buf2);
    iLog->Log(_L("KOomLoggerStartL=%s"),buf2.Ptr());
    STIF_ASSERT_EQUALS(buf1, buf2);
#endif
    
    TBuf<KIntValue18> buf3(KActionListBuildActionListL);
    TBuf<KIntValue18> buf4;    
    DicLog::ReadLog(TUid::Uid(0x0014), buf4);
    iLog->Log(_L("KActionListBuildActionListL=%s"),buf4.Ptr());
    STIF_ASSERT_EQUALS(buf3, buf4);
    
    TInt32 maxPriority32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0015), maxPriority32);
    TInt maxPriority=STATIC_CAST(TInt, maxPriority32);
    STIF_ASSERT_EQUALS(maxPriority, KOomPriorityInfinate - 1);
      
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test HandleFocusedWgChangeL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-005
 * @SYMTestCaseDesc            Test FreeMemThresholdCrossedL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config.
 *                             4. Call FreeMemThresholdCrossedL function.
 *                             5. Check iActionTrigger value of CMemoryMonitor object.
 *                             6. Check iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. Delete diclog file.
 *                             8. Delete COomFriendClass object.
 *                             9. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config is set.
 *                             4. FreeMemThresholdCrossedL function is called.
 *                             5. The value of iActionTrigger of CMemoryMonitor object is set as expected.
 *                             6. iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. Delete diclog file successfully.
 *                             8. Delete COomFriendClass object successfully.
 *                             9. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-005)
{
    iLog->Log(_L("OomMemoryMonitor: Start test FreeMemThresholdCrossedL()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log(_L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThreshold*KMSize);
    oomConfig->SetDefaultLowRamThreshold(KLowRamThreshold*KMSize);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThreshold*KMSize);
    oomConfig->SetDefaultLowSwapThreshold(KLowSwapThreshold*KMSize);
     
    oomFriendClass->CallRefreshThresholds(oom);

    oom->FreeMemThresholdCrossedL();
    
    TInt actionTriggerType1=STATIC_CAST(TInt, ERamRotation);
    TInt actionTriggerType2=oom->ActionTrigger();
    TInt actionTriggerType3=STATIC_CAST(TInt, actionTriggerType2);
    iLog->Log( _L("OomMemoryMonitor: ActionTrigger=%d\n"), actionTriggerType3);
    STIF_ASSERT_EQUALS(actionTriggerType1, actionTriggerType3);    

    TInt ramTarget=oomFriendClass->GetCurrentRamTarget(oom);
    TInt swapTarget=oomFriendClass->GetCurrentSwapTarget(oom);
    iLog->Log( _L("OomMemoryMonitor: ramTarget=%d, swapTarget=%d\n"), ramTarget, swapTarget);

    STIF_ASSERT_EQUALS(ramTarget, KGoodRamThreshold*KMSize);
    STIF_ASSERT_EQUALS(swapTarget, KGoodSwapThreshold*KMSize);
    
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test FreeMemThresholdCrossedL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-006
 * @SYMTestCaseDesc            Test RequestFreeMemoryPandSL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config.
 *                             4. Call RequestFreeMemoryPandSL function.
 *                             5. Check iActionTrigger value of CMemoryMonitor object.
 *                             6. Check iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. Delete diclog file.
 *                             8. Delete COomFriendClass object.
 *                             9. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config is set.
 *                             4. RequestFreeMemoryPandSL function is called.
 *                             5. The value of iActionTrigger of CMemoryMonitor object is set as expected.
 *                             6. iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. Delete diclog file successfully.
 *                             8. Delete COomFriendClass object successfully.
 *                             9. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-006)
{
    iLog->Log(_L("OomMemoryMonitor: Start test RequestFreeMemoryPandSL()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log(_L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThreshold*KMSize);
    oomConfig->SetDefaultLowRamThreshold(KLowRamThreshold*KMSize);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThreshold*KMSize);
    oomConfig->SetDefaultLowSwapThreshold(KLowSwapThreshold*KMSize);    
     
    oomFriendClass->CallRefreshThresholds(oom);

    TInt KByetRequest=1000;
    oom->RequestFreeMemoryPandSL(KByetRequest);
    
    TInt actionTriggerType1=STATIC_CAST(TInt, EPublishAndSubscribe);
    TInt actionTriggerType2=oom->ActionTrigger();
    TInt actionTriggerType3=STATIC_CAST(TInt, actionTriggerType2);
    iLog->Log(_L("OomMemoryMonitor: ActionTrigger=%d\n"), actionTriggerType3);
    STIF_ASSERT_EQUALS(actionTriggerType1, actionTriggerType3);    

    TInt ramTarget=oomFriendClass->GetCurrentRamTarget(oom);
    TInt swapTarget=oomFriendClass->GetCurrentSwapTarget(oom);
    iLog->Log(_L("OomMemoryMonitor: ramTarget=%d, swapTarget=%d\n"), ramTarget, swapTarget);
    STIF_ASSERT_EQUALS(ramTarget, KLowRamThreshold*KMSize+KByetRequest);
    STIF_ASSERT_EQUALS(swapTarget, KLowSwapThreshold*KMSize);
      
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test RequestFreeMemoryPandSL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-007
 * @SYMTestCaseDesc            Test RequestFreeMemoryL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config.
 *                             4. Call RequestFreeMemoryL function.
 *                             5. Check iActionTrigger value of CMemoryMonitor object.
 *                             6. Check iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. Check iDataPaged value.
 *                             8. Delete diclog file.
 *                             9. Delete COomFriendClass object.
 *                             10. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config is set.
 *                             4. RequestFreeMemoryL function is called.
 *                             5. The value of iActionTrigger of CMemoryMonitor object is set as expected.
 *                             6. iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. iDataPaged is assigned value as expected.
 *                             8. Delete diclog file successfully.
 *                             9. Delete COomFriendClass object successfully.
 *                             10. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-007)
{
    iLog->Log(_L("OomMemoryMonitor: Start test RequestFreeMemoryL()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log(_L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThreshold*KMSize);
    oomConfig->SetDefaultLowRamThreshold(KLowRamThreshold*KMSize);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThreshold*KMSize);
    oomConfig->SetDefaultLowSwapThreshold(KLowSwapThreshold*KMSize);     
     
    oomFriendClass->CallRefreshThresholds(oom);

    const TInt KByetRequestMem=2000;
    oom->RequestFreeMemoryL(KByetRequestMem, ETrue);
    
    TInt actionTriggerType1=STATIC_CAST(TInt, EClientServerRequestFreeMemory);
    TInt actionTriggerType2=oom->ActionTrigger();
    TInt actionTriggerType3=STATIC_CAST(TInt, actionTriggerType2);
    iLog->Log(_L("OomMemoryMonitor: ActionTrigger=%d\n"), actionTriggerType3);
    STIF_ASSERT_EQUALS(actionTriggerType1, actionTriggerType3);    

    TInt ramTarget=oomFriendClass->GetCurrentRamTarget(oom);
    TInt swapTarget=oomFriendClass->GetCurrentSwapTarget(oom);
    iLog->Log(_L("OomMemoryMonitor: ramTarget=%d, swapTarget=%d\n"), ramTarget, swapTarget);
    STIF_ASSERT_EQUALS(ramTarget, KLowRamThreshold*KMSize);
    STIF_ASSERT_EQUALS(swapTarget, KLowSwapThreshold*KMSize+KByetRequestMem);
    
    TBool dataPaged=oomFriendClass->GetDataPaged(oom);
    STIF_ASSERT_TRUE(dataPaged);
      
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test RequestFreeMemoryL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-008
 * @SYMTestCaseDesc            Test FreeOptionalRamL() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config.
 *                             4. Call FreeOptionalRamL function.
 *                             5. Check iActionTrigger value of CMemoryMonitor object.
 *                             6. Check iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. Check iDataPaged value.
 *                             8. Check COomActionList->FreeMemory(aMaxPriority) is called.
 *                             9. Delete diclog file.
 *                             10. Delete COomFriendClass object.
 *                             11. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of GoodRamThreshold, LowRamThreshold, GoodSwapThreshold and LowSwapThreshold
 *                                of global config is set.
 *                             4. FreeOptionalRamL function is called.
 *                             5. The value of iActionTrigger of CMemoryMonitor object is set as expected.
 *                             6. iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             7. iDataPaged is assigned value as expected.
 *                             8. COomActionList->FreeMemory(aMaxPriority) is called.
 *                             9. Delete diclog file successfully.
 *                             10. Delete COomFriendClass object successfully.
 *                             11. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-008)
{
    iLog->Log(_L("OomMemoryMonitor: Start test FreeOptionalRamL()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    iLog->Log(_L("OomMemoryMonitor: Create friend class!\n"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThreshold*KMSize);
    oomConfig->SetDefaultLowRamThreshold(KLowRamThreshold*KMSize);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThreshold*KMSize);
    oomConfig->SetDefaultLowSwapThreshold(KLowSwapThreshold*KMSize); 
     
    oomFriendClass->CallRefreshThresholds(oom);

    const TInt KByetRequestMemory=3000;
    const TInt KPluginID=99;
    oom->FreeOptionalRamL(KByetRequestMemory, KPluginID, EFalse);
    
    TInt actionTriggerType1=STATIC_CAST(TInt, EClientServerRequestOptionalRam);
    TInt actionTriggerType2=oom->ActionTrigger();
    TInt actionTriggerType3=STATIC_CAST(TInt, actionTriggerType2);
    iLog->Log(_L("OomMemoryMonitor: ActionTrigger=%d\n"), actionTriggerType3);
    STIF_ASSERT_EQUALS(actionTriggerType1, actionTriggerType3);    

    TInt ramTarget=oomFriendClass->GetCurrentRamTarget(oom);
    TInt swapTarget=oomFriendClass->GetCurrentSwapTarget(oom);
    iLog->Log(_L("OomMemoryMonitor: ramTarget=%d, swapTarget=%d\n"), ramTarget, swapTarget);
    STIF_ASSERT_EQUALS(ramTarget, KGoodRamThreshold*KMSize+KByetRequestMemory);
    STIF_ASSERT_EQUALS(swapTarget, KLowSwapThreshold*KMSize);
    
    TBool dataPaged=oomFriendClass->GetDataPaged(oom);
    iLog->Log( _L("OomMemoryMonitor: GetDataPaged=%d\n"), dataPaged);
    STIF_ASSERT_FALSE(dataPaged);
    
    TInt32 maxPriority32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0015), maxPriority32);
    TInt maxPriority=STATIC_CAST(TInt, maxPriority32);
    iLog->Log(_L("OomMemoryMonitor: maxPriority=%d\n"), maxPriority);
    STIF_ASSERT_EQUALS(maxPriority, KMaxPriority1-1);
      
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test FreeOptionalRamL() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-009
 * @SYMTestCaseDesc            Test GetFreeMemory() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Get free memory value.
 *                             3. Calculate free memory value.
 *                             4. Check free memory value.
 *                             5. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Get free memory successfully.
 *                             3. Calculate free memory successfully.
 *                             4. Two value are same.
 *                             5. Delete CMemoryMonitor object.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-009)
{
    iLog->Log(_L("OomMemoryMonitor: Start test GetFreeMemory()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    
    TInt freeMemory;
    oom->GetFreeMemory(freeMemory);
    TInt freeMemory1;
    User::CompressAllHeaps();
    HAL::Get( HALData::EMemoryRAMFree, freeMemory1);
    iLog->Log(_L("OomMemoryMonitor: test freeMemory=%d, freeMemory1=%d!"), freeMemory, freeMemory1);
    STIF_ASSERT_EQUALS(freeMemory, freeMemory1);
    
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test GetFreeMemory() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-010
 * @SYMTestCaseDesc            Test AppNotExiting(), ActionsCompleted() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Call AppNotExiting function.
 *                             3. Check COomActionList->AppNotExiting() is called.
 *                             4. Call ActionsCompleted function.
 *                             5. Check ActionsCompleted() function is called.
 *                             6. Delete diclog file.
 *                             7. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. AppNotExiting function is called.
 *                             3. COomActionList->AppNotExiting() is called.
 *                             4. Call ActionsCompleted function.
 *                             5. COomClientRequestQueue->ActionsCompleted() function is called.
 *                             6. Diclog file is deleted.
 *                             7. Delete CMemoryMonitor object.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-010)
{
    iLog->Log(_L("OomMemoryMonitor: Start test AppNotExiting(), ActionsCompleted()!\n"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    
    const TInt KWgId=99;
    oom->AppNotExiting(KWgId);
    
    TInt32 wgID32=0;
    DicLog::ReadIntLog(TUid::Uid(0x0016), wgID32);
    TInt wgID=STATIC_CAST(TInt, wgID32);
    STIF_ASSERT_EQUALS(wgID, KWgId);
    
    oom->ActionsCompleted(KWgId,KWgId);
    
    iLog->Log(_L("OomMemoryMonitor: Call COomClientRequestQueue::ActionsCompleted()!\n"));
    TBuf<18> buf3(KClientRequestQueue);
    TBuf<18> buf4;    
    DicLog::ReadLog(TUid::Uid(0x0008), buf4);
    iLog->Log(_L("COomClientRequestQueue::ActionsCompleted()()=%s"),buf4.Ptr());
    STIF_ASSERT_EQUALS(buf3, buf4);
    
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: test AppNotExiting(), ActionsCompleted() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-011
 * @SYMTestCaseDesc            Test ResetTargets() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.
 *                             3. Set GoodRamThreshold, GoodSwapThreshold of global config.                             
 *                             4. Call ResetTargets function.
 *                             5. Check iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             6. Delete diclog file.
 *                             7. Delete COomFriendClass object.
 *                             8. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. The value of GoodRamThreshold, GoodSwapThreshold of global config is set.                             
 *                             4. ResetTargets function is called.
 *                             5. iCurrentRamTarget and iCurrentSwapTarget of CMemoryMonitor object
 *                                is set correct value.
 *                             6. Delete diclog file successfully.
 *                             7. Delete COomFriendClass object successfully.
 *                             8. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-011)
{
    iLog->Log(_L("OomMemoryMonitor: Test ResetTargets()"));
    
    iLog->Log(_L("OomMemoryMonitor: Create CMemoryMonitor object!"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
       
    iLog->Log(_L("OomMemoryMonitor: Create friend class!"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
    
    const TInt KGoodRamThre=500;
    const TInt KGoodSwapThre=600;

    oomConfig->SetDefaultGoodRamThreshold(KGoodRamThre);
    oomConfig->SetDefaultGoodSwapThreshold(KGoodSwapThre);
    oomFriendClass->CallRefreshThresholds(oom);
    
    oom->ResetTargets();
    TInt ramTarget=oomFriendClass->GetCurrentRamTarget(oom);
    TInt swapTarget=oomFriendClass->GetCurrentSwapTarget(oom);
    iLog->Log(_L("OomMemoryMonitor: ramTarget=%d, swapTarget=%d!"), ramTarget, swapTarget);
    STIF_ASSERT_EQUALS(ramTarget, KGoodRamThre);
    STIF_ASSERT_EQUALS(swapTarget, KGoodSwapThre);
           
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    iLog->Log(_L("OomMemoryMonitor: Delete oomFriendClass!"));
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: Delete  OomMemoryMonitor object!"));
    iLog->Log(_L("OomMemoryMonitor: Test ResetTargets() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-012
 * @SYMTestCaseDesc            Test ResetTargets() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.                          
 *                             3. Call SetMemoryMonitorStatusProperty function.
 *                             4. Check iLastMemoryMonitorStatusProperty value.
 *                             5. Delete diclog file.
 *                             6. Delete COomFriendClass object.
 *                             7. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. SetMemoryMonitorStatusProperty function is called.
 *                             4. iLastMemoryMonitorStatusProperty is set as expected value.
 *                             5. Delete diclog file successfully.
 *                             6. Delete COomFriendClass object successfully.
 *                             7. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-012)
{
    iLog->Log(_L("OomMemoryMonitor: Test SetMemoryMonitorStatusProperty()"));
    
    iLog->Log(_L("OomMemoryMonitor: Create CMemoryMonitor object!"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
       
    iLog->Log(_L("OomMemoryMonitor: Create friend class!"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    COomConfig* oomConfig=oomFriendClass->GetOomConfig(oom);
    STIF_ASSERT_NOT_NULL(oomConfig);
       
    oom->SetMemoryMonitorStatusProperty(EBelowTreshHold);
    TInt lastMemoryMonitorStatusProperty=oomFriendClass->GetLastMemoryMonitorStatusProperty(oom);
       
    iLog->Log(_L("OomMemoryMonitor: lastMemoryMonitorStatusProperty=%d"),lastMemoryMonitorStatusProperty);

    TInt statusPropertyValues= STATIC_CAST(TInt,EBelowTreshHold);
    STIF_ASSERT_EQUALS(lastMemoryMonitorStatusProperty, statusPropertyValues);
            
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    iLog->Log(_L("OomMemoryMonitor: Delete oomFriendClass!"));
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: Delete  OomMemoryMonitor object!"));
    iLog->Log(_L("OomMemoryMonitor: Test SetMemoryMonitorStatusProperty() finished!"));
}

/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-013
 * @SYMTestCaseDesc            Test GoodRamThreshold() LowRamThreshold() and ActionTrigger() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.                          
 *                             3. Call GoodRamThreshold function.
 *                             4. Check iGoodRamThreshold value.
 *                             5. Call LowRamThreshold function.
 *                             6. Check iLowRamThreshold value.
 *                             7. Call ActionTrigger function.
 *                             8. Check iActionTrigger value.
 *                             9. Delete COomFriendClass object.
 *                             10. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object.
 *                             3. GoodRamThreshold function is called.
 *                             4. iGoodRamThreshold is set as expected value.
 *                             5. LowRamThreshold function is called.
 *                             6. iLowRamThreshold is set as expected value.
 *                             7. ActionTrigger function is called.
 *                             8. iActionTrigger is set as expected value.
 *                             9. Delete COomFriendClass object successfully.
 *                             10. Delete CMemoryMonitor object successfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-013)
{
    iLog->Log(_L("OomMemoryMonitor: Test GoodRamThreshold() LowRamThreshold() and ActionTrigger()"));
    
    iLog->Log(_L("OomMemoryMonitor: Create CMemoryMonitor object!"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    
    iLog->Log(_L("OomMemoryMonitor: Create friend class!"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
    
    TInt goodRamThreshold2=oom->GoodRamThreshold();
    TInt goodRamThreshold1=oomFriendClass->GetGoodRamThreshold(oom);    
    STIF_ASSERT_EQUALS(goodRamThreshold1, goodRamThreshold2);
    iLog->Log(_L("OomMemoryMonitor: GoodRamThreshold() passed!"));
       
    TInt lowRamThreshold2=oom->LowRamThreshold();
    TInt lowRamThreshold1=oomFriendClass->GetLowRamThreshold(oom);
    STIF_ASSERT_EQUALS(lowRamThreshold1, lowRamThreshold2);
    iLog->Log(_L("OomMemoryMonitor: GetLowRamThreshold() passed!"));
       
    TInt actionTriggerType2=STATIC_CAST(TInt,oom->ActionTrigger());
    TInt actionTriggerType1=oomFriendClass->GetActionTrigger(oom);
    STIF_ASSERT_EQUALS(actionTriggerType1, actionTriggerType2);
    iLog->Log(_L("OomMemoryMonitor: ActionTrigger() passed!"));
    
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    iLog->Log(_L("OomMemoryMonitor: Delete oomFriendClass!"));
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: Delete  OomMemoryMonitor object!"));
    iLog->Log(_L("OomMemoryMonitor: Test GoodRamThreshold() LowRamThreshold() and ActionTrigger() finished!"));
}


/**
 * @SYMTestCaseID              GAPS-OOM-MEMORYMONITOR-014
 * @SYMTestCaseDesc            Test SetPriorityBusy(), SetPriorityNormal() and SetPriorityHigh() function of CMemoryMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CMemoryMonitor object.
 *                             2. Create COomFriendClass object.  
 *                             3. Get wgID of forground window.                        
 *                             4. Call SetPriorityBusy function.
 *                             5. Check COomWindowGroupList->SetPriorityBusy() is called.
 *                             6. Call SetPriorityHigh function.
 *                             7. Check COomWindowGroupList->SetPriorityHigh() is called.
 *                             8. Call SetPriorityNormal function.
 *                             9. Check COomWindowGroupList->SetPriorityNormal() is called.
 *                             10. Delete diclog file.
 *                             11. Delete COomFriendClass object.
 *                             12. Delete CMemoryMonitor object.
 * @SYMTestExpectedResults     1. Create CMemoryMonitor object successfully.
 *                             2. Create COomFriendClass object successfully.  
 *                             3. The wgID of forground window is get.                        
 *                             4. Call SetPriorityBusy function.
 *                             5. COomWindowGroupList->SetPriorityBusy() is called.
 *                             6. Call SetPriorityHigh function.
 *                             7. COomWindowGroupList->SetPriorityHigh() is called.
 *                             8. Call SetPriorityNormal function.
 *                             9. COomWindowGroupList->SetPriorityNormal() is called.
 *                             10. Delete diclog file successfully.
 *                             11. Delete COomFriendClass object successfully.
 *                             12. Delete CMemoryMonitor object sucessfully.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            15-08-2010
 */

STIF_TESTDEFINE(GAPS-OOM-MEMORYMONITOR-014)
{
    iLog->Log(_L("OomMemoryMonitor: Test SetPriorityBusy(), SetPriorityNormal() and SetPriorityHigh()"));
    
    iLog->Log(_L("OomMemoryMonitor: Create CMemoryMonitor object!"));
    CMemoryMonitor* oom = NULL;
    oom = CMemoryMonitor::NewL();
    STIF_ASSERT_NOT_NULL(oom);
    
    CleanupStack::PushL(oom);
    
    iLog->Log(_L("OomMemoryMonitor: Create friend class!"));
    COomFriendClass* oomFriendClass=COomFriendClass::NewLC();
   
    COomWindowGroupList* oomWindowGroupList=oomFriendClass->GetWindowGroupList(oom);
    STIF_ASSERT_NOT_NULL(oomWindowGroupList);
    
    RWsSession::TWindowGroupChainInfo wgChainInfo=oomWindowGroupList->WgId(0);
    TInt wgId=wgChainInfo.iId;
    
    iLog->Log(_L("OomMemoryMonitor: wgId=%d"), wgId);
    
    oom->SetPriorityBusy(wgId);
    TInt32 return1=0;
    DicLog::ReadIntLog(TUid::Uid(0x0006), return1);
    iLog->Log(_L("OomMemoryMonitor: wgId=%d"), return1);
    STIF_ASSERT_EQUALS((TInt)return1, wgId);
          
    oom->SetPriorityHigh(wgId);
    TInt32 return2=0;
    DicLog::ReadIntLog(TUid::Uid(0x0004), return2);
    iLog->Log(_L("OomMemoryMonitor: wgId=%d"), return2);
    STIF_ASSERT_EQUALS((TInt)return2, wgId);
    
    oom->SetPriorityNormal(wgId);
    TInt32 return3=0;
    DicLog::ReadIntLog(TUid::Uid(0x0005), return3);
    iLog->Log(_L("OomMemoryMonitor: wgId=%d"), return3);
    STIF_ASSERT_EQUALS((TInt)return3, wgId);
    
    DicLog::RemoveLogFile();
    CleanupStack::PopAndDestroy(oomFriendClass);
    iLog->Log(_L("OomMemoryMonitor: Delete oomFriendClass!"));
    CleanupStack::PopAndDestroy(oom);
    iLog->Log(_L("OomMemoryMonitor: Delete  OomMemoryMonitor object!"));
    iLog->Log(_L("OomMemoryMonitor: Test SetPriorityBusy(), SetPriorityNormal() and SetPriorityHigh() finished."));
}

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
