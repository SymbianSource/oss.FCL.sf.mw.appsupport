/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "remconeventtable.h"
#include "mockremcontspcontroller.h"
#include "tsptriggerevents.h"
#include "ut_atsptspeventsstub.h"

#include <W32STD.H>

#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
	 CRemConEventTable* table;
	 CTspTriggerEventsWatcher* watcher;
	 CUt_AtspTspEventsStub* tspstub;
	 CActiveScheduler* scheduler;
	 static const TInt KDefaultWaitTimeInMicroSeconds = 5000000;
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
 * STIF_TESTDEFINE defines a test case
 *
 */

/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-001
 * @SYMTestCaseDesc            Make sure TableNum is fix after construction.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConEventTable object with table number as 5.
 *                             2. TableNum() should return 5.
 * @SYMTestExpectedResults     1. CRemConEventTable object was successfully constructed.
 *                             2. TableNum() returns 5.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-001)
{
    TInt tableNum = 5;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
	
	STIF_ASSERT_EQUALS(table->TableNum(), tableNum);
	STIF_LOG("TableNum() returned correct table number.");
	
	CleanupStack::PopAndDestroy();
}

/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-002
 * @SYMTestCaseDesc            Add different kinds of remote control events into event table. 
 *                             Check whether those events are really in event table. Check whether unadded events are not in event table.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConEventTable object with table number as 1.
 *                             2. Add ERemConEvent into event table.
 *                             3. Add ERemConExtEventCall into event table.
 *                             4. Add ERemConExtEventPoc into event table.
 *                             5. Add ERemConExtEventSync into event table.
 *                             6. Add another ERemConEvent into event table.
 *                             7. Add another ERemConExtEventCall into event table.
 *                             8. Add another ERemConExtEventPoc into event table.
 *                             9. Add another ERemConExtEventSync into event table.
 *                             10. Check ERemConEvent in event table.
 *                             11. Check ERemConExtEventCall in event table.
 *                             12. Check ERemConExtEventPoc in event table.
 *                             13. Check ERemConExtEventSync in event table.
 *                             14. Check another ERemConEvent in event table.
 *                             15. Check another ERemConExtEventCall in event table.
 *                             16. Check another ERemConExtEventPoc in event table.
 *                             17. Check another ERemConExtEventSync in event table.
 *                             18. Check unadded events are not in event table.
 * @SYMTestExpectedResults     1. CRemConEventTable object was successfully constructed.
 *                             2. An ERemConEvent was added into event table.
 *                             3. An ERemConExtEventCall was added into event table.
 *                             4. An ERemConExtEventPoc was added into event table.
 *                             5. An ERemConExtEventSync was added into event table.
 *                             6. Another ERemConEvent was added into event table.
 *                             7. Another ERemConExtEventCall was added into event table.
 *                             8. Another ERemConExtEventPoc was added into event table.
 *                             9. Another ERemConExtEventSync was added into event table.
 *                             10. ERemConEvent was in event table.
 *                             11. ERemConExtEventCall was in event table.
 *                             12. ERemConExtEventPoc was in event table.
 *                             13. ERemConExtEventSync was in event table.
 *                             14. Another ERemConEvent was in event table.
 *                             15. Another ERemConExtEventCall was in event table.
 *                             16. Another ERemConExtEventPoc was in event table.
 *                             17. Another ERemConExtEventSync was in event table.
 *                             18. Unadded events are not in event table.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-002)
{
    // 1. Construct a CRemConEventTable object with table number as 1.
    TInt tableNum = 1;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
    
    // 2. Add ERemConEvent into event table.
    // 3. Add ERemConExtEventCall into event table.
    // 4. Add ERemConExtEventPoc into event table.
    // 5. Add ERemConExtEventSync into event table.
    TInt eventValue = 41;
    table->AddRemConEventToTable(eventValue, CRemConEventTable::ERemConEvent);
    table->AddRemConEventToTable(eventValue, CRemConEventTable::ERemConExtEventCall);
    table->AddRemConEventToTable(eventValue, CRemConEventTable::ERemConExtEventPoc);
    table->AddRemConEventToTable(eventValue, CRemConEventTable::ERemConExtEventSync);
    
    // 6. Add another ERemConEvent into event table.
    // 7. Add another ERemConExtEventCall into event table.
    // 8. Add another ERemConExtEventPoc into event table.
    // 9. Add another ERemConExtEventSync into event table.
    TInt eventValue2 = 40;
    table->AddRemConEventToTable(eventValue2, CRemConEventTable::ERemConEvent);
    table->AddRemConEventToTable(eventValue2, CRemConEventTable::ERemConExtEventCall);
    table->AddRemConEventToTable(eventValue2, CRemConEventTable::ERemConExtEventPoc);
    table->AddRemConEventToTable(eventValue2, CRemConEventTable::ERemConExtEventSync);
    
    // 10. Check ERemConEvent in event table.
    // 11. Check ERemConExtEventCall in event table.
    // 12. Check ERemConExtEventPoc in event table.
    // 13. Check ERemConExtEventSync in event table.
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue, CRemConEventTable::ERemConEvent), static_cast<TBool>(ETrue));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue, CRemConEventTable::ERemConExtEventCall), static_cast<TBool>(ETrue));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue, CRemConEventTable::ERemConExtEventPoc), static_cast<TBool>(ETrue));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue, CRemConEventTable::ERemConExtEventSync), static_cast<TBool>(ETrue));
    
    // 14. Check another ERemConEvent in event table.
    // 15. Check another ERemConExtEventCall in event table.
    // 16. Check another ERemConExtEventPoc in event table.
    // 17. Check another ERemConExtEventSync in event table.
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue2, CRemConEventTable::ERemConEvent), static_cast<TBool>(ETrue));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue2, CRemConEventTable::ERemConExtEventCall), static_cast<TBool>(ETrue));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue2, CRemConEventTable::ERemConExtEventPoc), static_cast<TBool>(ETrue));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue2, CRemConEventTable::ERemConExtEventSync), static_cast<TBool>(ETrue));
    
    // 18. Check unadded events are not in event table.
    TInt unaddedEventValue = 39;
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(unaddedEventValue, CRemConEventTable::ERemConEvent), static_cast<TBool>(EFalse));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(unaddedEventValue, CRemConEventTable::ERemConExtEventCall), static_cast<TBool>(EFalse));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(unaddedEventValue, CRemConEventTable::ERemConExtEventPoc), static_cast<TBool>(EFalse));
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(unaddedEventValue, CRemConEventTable::ERemConExtEventSync), static_cast<TBool>(EFalse));
    
    CleanupStack::PopAndDestroy();
}

/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-003
 * @SYMTestCaseDesc            Add invalid remote control events into event table. Check whether invalid events exists in event table.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Add invalid remote control events into event table.
 *                             2. Check whether invalid events exist in event table.
 * @SYMTestExpectedResults     1. There isn't any crash and leave.
 *                             2. Invalid events are not in event table.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-003)
{
    // Construct a CRemConEventTable object with table number as 1.
    TInt tableNum = 1;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
    
    // 1. Add invalid remote control events into event table.
    TInt eventValue = 41;
    CRemConEventTable::RemConEventType invalidEventType = static_cast<CRemConEventTable::RemConEventType>(10);
    table->AddRemConEventToTable(eventValue, invalidEventType);
    
    // 2. Check whether invalid events exist in event table.
    STIF_ASSERT_EQUALS(table->IsRemConEventInTable(eventValue, invalidEventType), static_cast<TBool>(EFalse));

    CleanupStack::PopAndDestroy();
}

/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-004
 * @SYMTestCaseDesc            Try to add valid rules, get rules.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConEventTable object with table number as 1.
 *                             2. Get rules list from event table.
 *                             3. Check EActiveAudio not in the rules list.
 *                             4. Check EForegroundApplication not in the rules list.
 *                             5. Check EDefaultApplicationGroup not in the rules list.
 *                             6. Check ELastPlayedMediaApplication not in the rules list.
 *                             7. Check ELaunchDefaultApp not in the rules list.
 *                             8. Add EActiveAudio into event table.
 *                             9. Add EForegroundApplication into event table.
 *                             10. Add EDefaultApplicationGroup into event table.
 *                             11. Add ELastPlayedMediaApplication into event table.
 *                             12. Add ELaunchDefaultApp event table.
 *                             13. Get rules list from event table.
 *                             14. Check EActiveAudio in the rules list.
 *                             15. Check EForegroundApplication in the rules list.
 *                             16. Check EDefaultApplicationGroup in the rules list.
 *                             17. Check ELastPlayedMediaApplication in the rules list.
 *                             18. Check ELaunchDefaultApp in the rules list.
 * @SYMTestExpectedResults     1. CRemConEventTable object was successfully constructed.
 *                             2. Rules list was gotten.
 *                             3. EActiveAudio was not in the rules list.
 *                             4. EForegroundApplication was not in the rules list.
 *                             5. EDefaultApplicationGroup was not in the rules list.
 *                             6. ELastPlayedMediaApplication was not in the rules list.
 *                             7. ELaunchDefaultApp was not in the rules list.
 *                             8. An EActiveAudio was added into event table.
 *                             9. An EForegroundApplication was added into event table.
 *                             10. An EDefaultApplicationGroup was added into event table.
 *                             11. An ELastPlayedMediaApplication was added into event table.
 *                             12. An ELaunchDefaultApp was added into event table.
 *                             13. Rules list was gotten.
 *                             14. EActiveAudio was in the rules list.
 *                             15. EForegroundApplication was in the rules list.
 *                             16. EDefaultApplicationGroup was in the rules list.
 *                             17. ELastPlayedMediaApplication was in the rules list.
 *                             18. ELaunchDefaultApp was in the rules list.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-004)
{
    // 1. Construct a CRemConEventTable object with table number as 1.
    TInt tableNum = 1;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
    
    // 2. Get rules list from event table.
    RArray<TInt> rulesForThisTable;
    table->GetRulesForThisTable(rulesForThisTable);
    
    // 3. Check EActiveAudio not in the rules list.
    // 4. Check EForegroundApplication not in the rules list.
    // 5. Check EDefaultApplicationGroup not in the rules list.
    // 6. Check ELastPlayedMediaApplication not in the rules list.
    // 7. Check ELaunchDefaultApp not in the rules list.
    STIF_ASSERT_EQUALS(rulesForThisTable.Find(CRemConEventTable::EActiveAudio), KErrNotFound);
    STIF_ASSERT_EQUALS(rulesForThisTable.Find(CRemConEventTable::EForegroundApplication), KErrNotFound);
    STIF_ASSERT_EQUALS(rulesForThisTable.Find(CRemConEventTable::EDefaultApplicationGroup), KErrNotFound);
    STIF_ASSERT_EQUALS(rulesForThisTable.Find(CRemConEventTable::ELastPlayedMediaApplication), KErrNotFound);
    STIF_ASSERT_EQUALS(rulesForThisTable.Find(CRemConEventTable::ELaunchDefaultApp), KErrNotFound);
    
    // 8. Add EActiveAudio into event table.
    // 9. Add EForegroundApplication into event table.
    // 10. Add EDefaultApplicationGroup into event table.
    // 11. Add ELastPlayedMediaApplication into event table.
    // 12. Add ELaunchDefaultApp event table.
    table->AddRuleForThisTable(CRemConEventTable::EActiveAudio);
    table->AddRuleForThisTable(CRemConEventTable::EForegroundApplication);
    table->AddRuleForThisTable(CRemConEventTable::EDefaultApplicationGroup);
    table->AddRuleForThisTable(CRemConEventTable::ELastPlayedMediaApplication);
    table->AddRuleForThisTable(CRemConEventTable::ELaunchDefaultApp);
    
    // 13. Get rules list from event table.
    rulesForThisTable.Reset();
    table->GetRulesForThisTable(rulesForThisTable);
    
    // 14. Check EActiveAudio in the rules list.
    // 15. Check EForegroundApplication in the rules list.
    // 16. Check EDefaultApplicationGroup in the rules list.
    // 17. Check ELastPlayedMediaApplication in the rules list.
    // 18. Check ELaunchDefaultApp in the rules list.
    STIF_ASSERT_NOT_EQUALS(rulesForThisTable.Find(CRemConEventTable::EActiveAudio), KErrNotFound);
    STIF_ASSERT_NOT_EQUALS(rulesForThisTable.Find(CRemConEventTable::EForegroundApplication), KErrNotFound);
    STIF_ASSERT_NOT_EQUALS(rulesForThisTable.Find(CRemConEventTable::EDefaultApplicationGroup), KErrNotFound);
    STIF_ASSERT_NOT_EQUALS(rulesForThisTable.Find(CRemConEventTable::ELastPlayedMediaApplication), KErrNotFound);
    STIF_ASSERT_NOT_EQUALS(rulesForThisTable.Find(CRemConEventTable::ELaunchDefaultApp), KErrNotFound);
    
    CleanupStack::PopAndDestroy();
}

/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-005
 * @SYMTestCaseDesc            Try to add default application, get default application.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConEventTable object with table number as 1.
 *                             2. Get default application list from event table.
 *                             3. Check the default application info not in the rules list.
 *                             4. Add a default application into event table.
 *                             5. Get default application list from event table.
 *                             6. Check default application info is right in the rules list.
 * @SYMTestExpectedResults     1. CRemConEventTable object was successfully constructed.
 *                             2. Default application list was gotten.
 *                             3. The default application info was not in the rules list.
 *                             4. A default application was added into event table.
 *                             5. Default application list was gotten.
 *                             6. The default application info in the list is right.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-005)
{
    // 1. Construct a CRemConEventTable object with table number as 1.
    TInt tableNum = 1;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
    
    TInt defaultApplication = 0x100058B3;
    
    // 2. Get default application list from event table.
    RArray<TInt> defaultApplicationsForThisTable;
    table->GetDefaultApplicationsForTable(defaultApplicationsForThisTable);
    
    // 3. Check the default application info not in the rules list.
    STIF_ASSERT_EQUALS(defaultApplicationsForThisTable.Find(defaultApplication), KErrNotFound);
    
    // 4. Add a default application into event table.
    table->AddDefaultApplicationForThisTable(defaultApplication);
    
    // 5. Get default application list from event table.
    defaultApplicationsForThisTable.Reset();
    table->GetDefaultApplicationsForTable(defaultApplicationsForThisTable);
    
    // 6. Check default application info is right in the rules list.
    STIF_ASSERT_NOT_EQUALS(defaultApplicationsForThisTable.Find(defaultApplication), KErrNotFound);
    
    CleanupStack::PopAndDestroy();
}


/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-006
 * @SYMTestCaseDesc            Try to add default launch application, get the default launch application.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConEventTable object with table number as 1.
 *                             2. Add a default launch application into event table.
 *                             3. Get default launch application from event table.
 * @SYMTestExpectedResults     1. CRemConEventTable object was successfully constructed.
 *                             2. The default launch application was set into event table.
 *                             3. The default launch application uid is right.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-006)
{
    // 1. Construct a CRemConEventTable object with table number as 1.
    TInt tableNum = 1;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
    
    TInt32 defaultLaunchApplication = 0x100058B3;
    // 2. Add a default launch application into event table.
    table->AddDefaultLaunchAppForThisTable(defaultLaunchApplication);
    
    // 3. Get default launch application from event table.
    TUid retUid;
    table->GetDefaultLaunchAppForTable(retUid);
    STIF_ASSERT_EQUALS(retUid.iUid, defaultLaunchApplication);
    
    CleanupStack::PopAndDestroy();
}

/**
 * @SYMTestCaseID              GAPS-ATSP-EVENTTABLE-007
 * @SYMTestCaseDesc            Try to add transparent application, get the transparent application.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConEventTable object with table number as 1.
 *                             2. Add a transparent application into event table.
 *                             3. Get transparent application list from event table.
 *                             4. Check transparent application info is right in the list.
 *                             5. Check unadded transparent application info is not in the list.
 * @SYMTestExpectedResults     1. CRemConEventTable object was successfully constructed.
 *                             2. A transparent application was added into event table.
 *                             3. Transparent application list was gotten.
 *                             4. The transparent application info in the list is right.
 *                             5. Unadded transparent application info is not in the list.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-EVENTTABLE-007)
{
    // 1. Construct a CRemConEventTable object with table number as 1.
    TInt tableNum = 1;
    table = CRemConEventTable::NewL(tableNum);
    CleanupStack::PushL(table);
    
    TInt transparentApplication = 0x100058B3;
    TInt unaddedApplication = 0x100058B4;
    // 2. Add a transparent application into event table.
    table->AddTransparentApplicationForThisTable(transparentApplication);
    
    // 3. Get transparent application list from event table.
    RArray<TInt> transparentApplicationsForThisTable;
    table->GetTransparentApplicationsForTable(transparentApplicationsForThisTable);
    
    // 4. Check transparent application info is right in the list.
    STIF_ASSERT_NOT_EQUALS(transparentApplicationsForThisTable.Find(transparentApplication), KErrNotFound);
    
    // 5. Check unadded transparent application info is not in the list.
    STIF_ASSERT_EQUALS(transparentApplicationsForThisTable.Find(unaddedApplication), KErrNotFound);
    
    CleanupStack::PopAndDestroy();
}

/**
 * @SYMTestCaseID              GAPS-ATSP-IDLELISTENER-001
 * @SYMTestCaseDesc            Create CRemConIdleListener with mock CRemConTspController. CreateAfterIdleL no error.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConIdleListener object with mock CRemConTspController.
 *                             2. CRemConIdleListener::CreateAfterIdleL returned with no error.
 *                             3. Destruct CRemConIdleListener object.
 * @SYMTestExpectedResults     1. A CRemConIdleListener object was constructed with mock CRemConTspController.
 *                             2. CRemConIdleListener::CreateAfterIdleL returned with no error.
 *                             3. The CRemConIdleListener object was destructed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-IDLELISTENER-001)
{
    CRemConTspController* tspController = CRemConTspController::NewL(ECorrect);
    
    delete tspController;
}

/**
 * @SYMTestCaseID              GAPS-ATSP-IDLELISTENER-002
 * @SYMTestCaseDesc            Create CRemConIdleListener with mock CRemConTspController. CreateAfterIdleL leaves with deleteInd not changed.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConIdleListener object with mock CRemConTspController.
 *                             2. CRemConIdleListener::CreateAfterIdleL leaved with deleteInd not changed.
 *                             3. Destruct CRemConIdleListener object.
 * @SYMTestExpectedResults     1. A CRemConIdleListener object was constructed with mock CRemConTspController.
 *                             2. CRemConIdleListener::CreateAfterIdleL leaved with deleteInd not changed.
 *                             3. The CRemConIdleListener object was destructed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-IDLELISTENER-002)
{
    CRemConTspController* tspController = CRemConTspController::NewL(ELeaveNoChange);
    
    delete tspController;
}

/**
 * @SYMTestCaseID              GAPS-ATSP-IDLELISTENER-003
 * @SYMTestCaseDesc            Create CRemConIdleListener with mock CRemConTspController. CreateAfterIdleL leaves with deleteInd changed to ETrue.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a CRemConIdleListener object with mock CRemConTspController.
 *                             2. CRemConIdleListener::CreateAfterIdleL leaved with deleteInd changed to ETrue.
 *                             3. Destruct CRemConIdleListener object.
 * @SYMTestExpectedResults     1. A CRemConIdleListener object was constructed with mock CRemConTspController.
 *                             2. CRemConIdleListener::CreateAfterIdleL leaved with deleteInd changed to ETrue.
 *                             3. The CRemConIdleListener object was destructed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-IDLELISTENER-003)
{
    CRemConTspController* tspController = CRemConTspController::NewL(ELeaveChange);
    
    delete tspController;
}

/**
 * @SYMTestCaseID              GAPS-ATSP-TSPEVENTS-001
 * @SYMTestCaseDesc            Create CTspTriggerEventsWatcher and all observers
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Call CTspTriggerEventsWatcher::NewL() function 
 *                             2. Delete the created object
 * @SYMTestExpectedResults     1. Function returns a CTspTriggerEventsWatcher* pointer. All its members 
 *                                (iAudioClientObserver; iWsEventsObserver; iClientMapperObserver;)are also created.
 *                             2. The object is deleted, and no memory leak
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-TSPEVENTS-001)
{
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
    tspstub = CUt_AtspTspEventsStub::NewL();
    CleanupStack::PushL(tspstub);
    watcher = CTspTriggerEventsWatcher::NewL(*tspstub);
    CleanupStack::PushL(watcher);
        
    STIF_ASSERT_NOT_NULL(tspstub);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CTspTriggerEventsWatcher created successfully");

    CleanupStack::PopAndDestroy(2);
}

/**
 * @SYMTestCaseID              GAPS-ATSP-TSPEVENTS-002
 * @SYMTestCaseDesc            Event which CTspClientMapperObserver watch is triggered
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CTspClientMapperObserver object with parameter a pointer to a MTspRulesTriggerObserver type interface
 *                             2. Trigger a Property change which has property category equals to 0x10200C70
 *                             3. Delete the created object
 * @SYMTestExpectedResults     1. Object created
 *                             2. MTspRulesTriggerObserver::MtrtoEvaluateRoutingRules() is called by the object
 *                             3. Object deleted
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-TSPEVENTS-002)
{
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
    tspstub = CUt_AtspTspEventsStub::NewL();
    CleanupStack::PushL(tspstub);
    watcher = CTspTriggerEventsWatcher::NewL(*tspstub);
    CleanupStack::PushL(watcher);
        
    STIF_ASSERT_NOT_NULL(tspstub);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CTspTriggerEventsWatcher created successfully");
    
    tspstub->After(TTimeIntervalMicroSeconds32(KDefaultWaitTimeInMicroSeconds));
    tspstub->IssueTspPropertyL();
    CActiveScheduler::Start();

    STIF_ASSERT_TRUE(tspstub->iIsRunLCalled);
    STIF_ASSERT_FALSE(tspstub->iIsTimerElapsed);
    STIF_LOG("Trigger of tsp client mapper is as expected");

    CleanupStack::PopAndDestroy(2);
}

/**
 * @SYMTestCaseID              GAPS-ATSP-TSPEVENTS-003
 * @SYMTestCaseDesc            Event which CWsEventsObserver watch is triggered
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CWsEventsObserver object with parameter a pointer to a MTspRulesTriggerObserver type interface
 *                             2. Trigger a window server event with type of EEventWindowGroupListChanged
 *                             3. Delete the created object
 * @SYMTestExpectedResults     1. Object created
 *                             2. MTspRulesTriggerObserver::MtrtoEvaluateRoutingRules() is called by the object
 *                             3. Object deleted
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-TSPEVENTS-003)
{
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
    tspstub = CUt_AtspTspEventsStub::NewL();
    CleanupStack::PushL(tspstub);
    watcher = CTspTriggerEventsWatcher::NewL(*tspstub);
    CleanupStack::PushL(watcher);
        
    STIF_ASSERT_NOT_NULL(tspstub);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CTspTriggerEventsWatcher created successfully");
    
    tspstub->After(TTimeIntervalMicroSeconds32(KDefaultWaitTimeInMicroSeconds));
    tspstub->IssueWsEventL(EEventWindowGroupListChanged);
    CActiveScheduler::Start();

    STIF_ASSERT_TRUE(tspstub->iIsRunLCalled);
    STIF_ASSERT_FALSE(tspstub->iIsTimerElapsed);
    STIF_LOG("Trigger of ws event is as expected");

    CleanupStack::PopAndDestroy(2);
}

/**
 * @SYMTestCaseID              GAPS-ATSP-TSPEVENTS-004
 * @SYMTestCaseDesc            Event which CWsEventsObserver watch is triggered, but not interested type
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CWsEventsObserver object with parameter a pointer to a MTspRulesTriggerObserver type interface
 *                             2. Trigger a window server event which type is not EEventWindowGroupListChanged
 *                             3. Delete the created object
 * @SYMTestExpectedResults     1. Object created
 *                             2. RunL of CWsEventsObserver is called, but MTspRulesTriggerObserver::MtrtoEvaluateRoutingRules() is not called by the object
 *                             3. Object deleted
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-TSPEVENTS-004)
{
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
    tspstub = CUt_AtspTspEventsStub::NewL();
    CleanupStack::PushL(tspstub);
    watcher = CTspTriggerEventsWatcher::NewL(*tspstub);
    CleanupStack::PushL(watcher);
        
    STIF_ASSERT_NOT_NULL(tspstub);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CTspTriggerEventsWatcher created successfully");
    
    tspstub->After(TTimeIntervalMicroSeconds32(KDefaultWaitTimeInMicroSeconds));
    tspstub->IssueWsEventL(EEventWindowVisibilityChanged);
    CActiveScheduler::Start();

    STIF_ASSERT_FALSE(tspstub->iIsRunLCalled);
    STIF_ASSERT_TRUE(tspstub->iIsTimerElapsed);
    STIF_LOG("Trigger of not cared ws event is as expected");

    CleanupStack::PopAndDestroy(2);
}

/**
 * @SYMTestCaseID              GAPS-ATSP-TSPEVENTS-005
 * @SYMTestCaseDesc            Event which CActiveAudioClientObserver watch is triggered
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create CActiveAudioClientObserver object with parameter a pointer to a MTspRulesTriggerObserver type interface
 *                             2. Trigger a Property change which has property category equals to 0x101F457f
 *                             3. Delete the created object
 * @SYMTestExpectedResults     1. Object created
 *                             2. MTspRulesTriggerObserver::MtrtoEvaluateRoutingRules() is called by the object
 *                             3. Object deleted
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-ATSP-TSPEVENTS-005)
{
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
    tspstub = CUt_AtspTspEventsStub::NewL();
    CleanupStack::PushL(tspstub);
    CActiveAudioClientObserver* audioObserver = CActiveAudioClientObserver::NewL(*tspstub);
    CleanupStack::PushL(audioObserver);
        
    STIF_ASSERT_NOT_NULL(tspstub);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CTspTriggerEventsWatcher created successfully");
    
    tspstub->After(TTimeIntervalMicroSeconds32(KDefaultWaitTimeInMicroSeconds));
    tspstub->IssueAudioPropertyL();
    CActiveScheduler::Start();

    STIF_ASSERT_TRUE(tspstub->iIsRunLCalled);
    STIF_ASSERT_FALSE(tspstub->iIsTimerElapsed);
    STIF_LOG("Trigger of audio client mapper is as expected");

    CleanupStack::PopAndDestroy(2);
}

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
