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
#include "ut_oomwatchercommon.h"
#include "oomoutofmemorywatcher.h"
#include "oommemorymonitor.h"
#include <oommonitorsession.h>
#include "oompanic.h"

#include <u32hal.h>
#include <hal.h>
#include <e32property.h>

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
 * @SYMTestCaseID              GAPS-OOM-WATCHER-001
 * @SYMTestCaseDesc            Watch when memory is lower then threshold
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COutOfMemoryWatcher object, set memory threshold in constructor
 *                             2) Occupy memory so that free memory is below memory threshold
 * @SYMTestExpectedResults     1) COutOfMemoryWatcher object created
 *                             2) FreeMemThresholdCrossedL() is called       
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-WATCHER-001)
{
    const TInt KMemThresholdLow = 26214400;
    const TInt KMemThresholdGood = 31457280; // low=25M good=30M
    CMemoryMonitor* monitor = CMemoryMonitor::NewL();
    CleanupStack::PushL(monitor);
    COutOfMemoryWatcher* watcher = COutOfMemoryWatcher::NewL(*monitor, KMemThresholdLow, KMemThresholdGood, 
                                                              ETrue, KMemThresholdLow, KMemThresholdGood);// low=25M good=30M
    CleanupStack::PushL(watcher);
    // Verify objects created
    STIF_ASSERT_NOT_NULL(monitor);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CMemoryMonitor and COutOfMemoryWatcher has been created successfully");
    
    // Start the watcher
    watcher->Start();
    
    // Create property for getting whether watcher has called correct method
    RProperty testWatcherProperty;
    TInt error;
    error = RProperty::Define(KOomWatcherTestProperty, KOomWatcherTestKey, RProperty::EInt, 
                              KAllowAllPolicy, KAllowAllPolicy);
    STIF_ASSERT_EQUALS(error, KErrNone);
    error = testWatcherProperty.Attach(KOomWatcherTestProperty, KOomWatcherTestKey, EOwnerThread);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(testWatcherProperty);
    STIF_LOG("Communication property created");
    
    // Occupy memory so that low mem is triggered
    UserSvr::HalFunction(EHalGroupKernel, EKernelHalSupervisorBarrier, 0, 0);
    TMemoryInfoV1Buf meminfo;
    UserHal::MemoryInfo(meminfo);
    TInt free = meminfo().iFreeRamInBytes;
    TInt totalRam;
    STIF_ASSERT_EQUALS(HAL::Get(HAL::EMemoryRAM, totalRam), KErrNone);
    TChunkCreateInfo createInfo;
    const TInt KChunkMaximumBelowFree = 20480*1024; // Chunk maximum = free - 20M
    createInfo.SetNormal(0, free - KChunkMaximumBelowFree);
    createInfo.SetPaging(TChunkCreateInfo::EUnpaged);
    RChunk c;
    error = c.Create(createInfo);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(c);
    const TInt KBufferSpace = 22528*1024; // 22M buffer
    STIF_LOG("Leave 22M RAM");
    error = c.Adjust(free - KBufferSpace);  // leave 22M
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_LOG("Memory occupied");
    
    CActiveScheduler::Start();
    TInt propValue;
    error = testWatcherProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestWatcherPropertyValue); // Same with set in ut_mockoommonitor.cpp
    STIF_LOG("Memory low handling function has been called correctly");

    // Cleanup
    CleanupStack::PopAndDestroy(4);
    error = RProperty::Delete(KOomWatcherTestProperty, KOomWatcherTestKey);
    STIF_ASSERT_EQUALS(error, KErrNone);
}

/**
 * @SYMTestCaseID              GAPS-OOM-WATCHER-002
 * @SYMTestCaseDesc            Watch when swap memory is lower then threshold
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COutOfMemoryWatcher object
 *                             2) Set swap memory threshold with UpdateThresholds, 
 *                                the value is larger than maximum swap memory space
 * @SYMTestExpectedResults     1) COutOfMemoryWatcher object created
 *                             2) FreeMemThresholdCrossedL() is called                      
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-WATCHER-002)
{
    const TInt KMemThresholdLow = 26214400;
    const TInt KMemThresholdGood = 31457280; // low=25M good=30M
    CMemoryMonitor* monitor = CMemoryMonitor::NewL();
    CleanupStack::PushL(monitor);
    COutOfMemoryWatcher* watcher = COutOfMemoryWatcher::NewL(*monitor, KMemThresholdLow, KMemThresholdGood, ETrue, KMemThresholdLow, KMemThresholdGood);// low=25M good=30M
    CleanupStack::PushL(watcher);
    // Verify objects created
    STIF_ASSERT_NOT_NULL(monitor);
    STIF_ASSERT_NOT_NULL(watcher);
    STIF_LOG("CMemoryMonitor and COutOfMemoryWatcher has been created successfully");
    
    // Start the watcher
    watcher->Start();
    
    // Create property for getting whether watcher has called correct method
    RProperty testWatcherProperty;
    TInt error;
    error = RProperty::Define(KOomWatcherTestProperty, KOomWatcherTestKey, RProperty::EInt, 
                              KAllowAllPolicy, KAllowAllPolicy);
    STIF_ASSERT_EQUALS(error, KErrNone);
    error = testWatcherProperty.Attach(KOomWatcherTestProperty, KOomWatcherTestKey, EOwnerThread);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(testWatcherProperty);
    STIF_LOG("Communication property created");
    
    // Change swap memory target to lower then swap memory, so that low mem is triggered
    // Get swap info
    SVMSwapInfo swapInfo;
    error = UserSvr::HalFunction(EHalGroupVM, EVMHalGetSwapInfo, &swapInfo, 0);
    STIF_ASSERT_EQUALS(error, KErrNone);
    // Set swap threshold
    watcher->UpdateThresholds(KMemThresholdLow, KMemThresholdGood, swapInfo.iSwapSize, swapInfo.iSwapSize);
    STIF_LOG("Swap memory threshold changed");
    
    CActiveScheduler::Start();
    TInt propValue;
    error = testWatcherProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestWatcherPropertyValue); // Same with set in ut_mockoommonitor.cpp
    STIF_LOG("Swap memory low handling function has been called correctly");

    // Cleanup
    CleanupStack::PopAndDestroy(3);
    error = RProperty::Delete(KOomWatcherTestProperty, KOomWatcherTestKey);
    STIF_ASSERT_EQUALS(error, KErrNone);
}

/**
 * @SYMTestCaseID              GAPS-OOM-SERVER-001
 * @SYMTestCaseDesc            Test calling functions via client-server
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Start server in another process
 *                             2) Create and connect to ROomMonitorSession
 *                             3) Call ROomMonitorSession::RequestFreeMemory() function
 *                             4) Call ROomMonitorSession::RequestOptionalRam() function
 *                             5) Call ROomMonitorSession::CancelRequestFreeMemory() function
 *                             6) Call ROomMonitorSession::ThisAppIsNotExiting() function
 * @SYMTestExpectedResults     1) Server started
 *                             2) ROomMonitorSession connects to server successfully
 *                             3) RequestFreeMemoryL() function at server side is called
 *                             4) RequestOptionalRamL() function at server side is called
 *                             5) The cancel request is handled by ServiceL() function at server side
 *                             6) AppNotExiting() function at server side is called
 * @SYMTestExpectedResults     
 *                             
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-SERVER-001)
{
    // Start the mock server
    _LIT(KServerName, "ut_mockoomserver.exe");
    RProcess process;
    TInt error = process.Create(KServerName, KNullDesC);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(process);
    TRequestStatus status;
    process.Rendezvous(status);
    process.Resume();
    User::WaitForRequest(status);
    STIF_ASSERT_EQUALS(status.Int(), KErrNone);
    STIF_LOG("Mock test server has been started");

    // Create property for getting whether server has called correct method
    RProperty testServerProperty;
    error = RProperty::Define(KOomServerTestProperty, KOomServerTestKey, RProperty::EInt, 
                              KAllowAllPolicy, KAllowAllPolicy);
    STIF_ASSERT_EQUALS(error, KErrNone);
    error = testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(testServerProperty);
    STIF_LOG("Communication property created");

    // Connect to ROomMonitorSession
    ROomMonitorSession monitorSession;
    error = monitorSession.Connect();
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(monitorSession);
    STIF_LOG("ROomMonitorSession has connected to server successfully");
    
    // Test ROomMonitorSession::RequestFreeMemory() function (2 overloaded versions)
    monitorSession.RequestFreeMemory(0);
    // The function shall already be called at this time, and the property shall already get set
    TInt propValue = KOomTestInitialValue;
    error = testServerProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestClientQueueFreeMemoryCalled); // Same with set in ut_mockoomclientrequestqueue.cpp
    STIF_LOG("RequestFreeMemory() sync version has been called correctly");
    // Set the property to initial value before next call
    error = testServerProperty.Set(KOomTestInitialValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    propValue = KOomTestInitialValue;
    
    monitorSession.RequestFreeMemory(0, status);
    User::WaitForRequest(status);
    // The function shall already be called at this time, and the property shall already get set
    error = testServerProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestClientQueueFreeMemoryCalled); // Same with set in ut_mockoomclientrequestqueue.cpp
    STIF_LOG("RequestFreeMemory() async version has been called correctly"); 
    // Set the property to initial value before next call
    error = testServerProperty.Set(KOomTestInitialValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    propValue = KOomTestInitialValue;

    // Test ROomMonitorSession::RequestOptionalRam() function (2 overloaded versions)
    TInt bytesAvailable;
    monitorSession.RequestOptionalRam(0, 0, 0, bytesAvailable);
    // The function shall already be called at this time, and the property shall already get set
    error = testServerProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestClientQueueOptionalRamCalled); // Same with set in ut_mockoomclientrequestqueue.cpp
    STIF_LOG("RequestOptionalRam() sync version has been called correctly"); 
    // Set the property to initial value before next call
    error = testServerProperty.Set(KOomTestInitialValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    propValue = KOomTestInitialValue;
    
    monitorSession.RequestOptionalRam(0, 0, 0, status);
    User::WaitForRequest(status);
    // The function shall already be called at this time, and the property shall already get set
    error = testServerProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestClientQueueOptionalRamCalled); // Same with set in ut_mockoomclientrequestqueue.cpp
    STIF_LOG("RequestOptionalRam() async version has been called correctly"); 
    // Set the property to initial value before next call
    error = testServerProperty.Set(KOomTestInitialValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    propValue = KOomTestInitialValue;

    // Test ROomMonitorSession::CancelRequestFreeMemory()
    monitorSession.CancelRequestFreeMemory();
    // No verification is needed as this function only returns success
    STIF_LOG("CancelRequestFreeMemory() has been called correctly"); 
    
    // Test ROomMonitorSession::ThisAppIsNotExiting() function 
    monitorSession.ThisAppIsNotExiting(0);
    // The function shall already be called at this time, and the property shall already get set
    error = testServerProperty.Get(propValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    STIF_ASSERT_EQUALS(propValue, KOomTestMonitorAppNotExitingCalled); // Same with set in ut_mockoommonitor.cpp
    STIF_LOG("ThisAppIsNotExiting() has been called correctly"); 
    // Set the property to initial value before next call
    error = testServerProperty.Set(KOomTestInitialValue);
    STIF_ASSERT_EQUALS(error, KErrNone);
    propValue = KOomTestInitialValue;

    // The test is done, close the app
    process.Kill(KErrNone);
    // Cleanup
    CleanupStack::PopAndDestroy(3);
    error = RProperty::Delete(KOomServerTestProperty, KOomServerTestKey);
    STIF_ASSERT_EQUALS(error, KErrNone);
}

/**
 * @SYMTestCaseID              GAPS-OOM-SERVER-002
 * @SYMTestCaseDesc            Test calling SetOomPriority functions via client-server
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Start server in another process
 *                             2) Create and connect to ROomMonitorSession
 *                             3) Call ROomMonitorSession::SetOomPriority() function
 * @SYMTestExpectedResults     1) Server started
 *                             2) ROomMonitorSession connects to server successfully
 *                             3) The request is handled by ServiceL() function at server side
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-SERVER-002)
{
    // Start the mock server
    _LIT(KServerName, "ut_mockoomserver.exe");
    RProcess process;
    TInt error = process.Create(KServerName, KNullDesC);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(process);
    TRequestStatus status;
    process.Rendezvous(status);
    process.Resume();
    User::WaitForRequest(status);
    STIF_ASSERT_EQUALS(status.Int(), KErrNone);
    STIF_LOG("Mock test server has been started");

    // Connect to ROomMonitorSession
    ROomMonitorSession monitorSession;
    error = monitorSession.Connect();
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(monitorSession);
    STIF_LOG("ROomMonitorSession has connected to server successfully");

#ifdef _DEBUG
    TestModuleIf().SetExitReason(CTestModuleIf::EPanic, KNoCoeEnvFound);
#endif
    // Test ROomMonitorSession::SetOomPriority() function 
    monitorSession.SetOomPriority(ROomMonitorSession::EOomPriorityNormal);
    // No need to verify
    STIF_LOG("SetOomPriority() has been called correctly"); 

    // The test is done, close the app
    process.Kill(KErrNone);
    // Cleanup
    CleanupStack::PopAndDestroy(2);
    error = RProperty::Delete(KOomServerTestProperty, KOomServerTestKey);
    STIF_ASSERT_EQUALS(error, KErrNone);
}

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
