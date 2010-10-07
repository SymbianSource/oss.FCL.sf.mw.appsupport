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
#include <e32property.h> 
#include "utoodsubscriberconst.h"
#include "outofdiskmonitor.h"
#include "outofdiskpsobserver.h"
#include "outofdisksubscriber.h"
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
RProperty::Define(KUTOODPropertyCategory, KUTOODStubKey, 
        RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
RProperty::Delete(KUTOODPropertyCategory, KUTOODStubKey); 
}

/**
 * @SYMTestCaseID              GAPS-OODMONITOR-SUBSCRIBER-001
 * @SYMTestCaseDesc            Unit test for COutOfDiskSubscriber.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a mock COutOfDiskMonitor object.
 *                             2. Construct a mock COutOfDiskPSObserver object.
 *                             3. Construct a COutOfDiskSubscriber object.
 *                             4. Subscribe
 *                             5. Set property value.
 *                             6. Start activescheduler
 *                             7. Directly call RunError
 * @SYMTestExpectedResults     1. Mock COutOfDiskMonitor object was constructed.
 *                             2. Mock COutOfDiskPSObserver object was constructed.
 *                             3. COutOfDiskSubscriber object was constructed.
 *                             4. COutOfDiskSubscriber object was set active.
 *                             5. Property was set.
 *                             6. ActiveScheduler was started.
 *                             7. RunError returns error code.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            23-08-2010
 */
STIF_TESTDEFINE(GAPS-OODMONITOR-SUBSCRIBER-001)
{    
    // instance and install, but not started
    scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
        
    STIF_LOG("1. Construct a mock COutOfDiskMonitor object.");
    MShutdownEventObserver* shutdownEventObserver = NULL;
    COutOfDiskMonitor* outOfDiskMonitor = COutOfDiskMonitor::NewL(*shutdownEventObserver);
    CleanupStack::PushL(outOfDiskMonitor);
    
    STIF_LOG("2. Construct a mock COutOfDiskPSObserver object.");
    COutOfDiskPSObserver* outOfDiskPSObserver = COutOfDiskPSObserver::NewL(outOfDiskMonitor);
    CleanupStack::PushL(outOfDiskPSObserver);
    
    STIF_LOG("3. Construct a COutOfDiskSubscriber object.");
    COutOfDiskSubscriber* outOfDiskSubscriber = COutOfDiskSubscriber::NewL(*outOfDiskPSObserver, KUTOODPropertyCategory, KUTOODStubKey);
    CleanupStack::PushL(outOfDiskSubscriber);
    
    STIF_LOG("4. Subscribe.");
    outOfDiskSubscriber->Subscribe();
    
    STIF_LOG("5. Set property value.");
    RProperty::Set(KUTOODPropertyCategory, KUTOODStubKey, KNotTouched);
    
    STIF_LOG("6. Start activescheduler");
    CActiveScheduler::Start();
    
    STIF_LOG("7. Directly call RunError.");
    STIF_ASSERT_EQUALS(KErrNoMemory, outOfDiskSubscriber->RunError(KErrNoMemory));
    
    CleanupStack::PopAndDestroy(outOfDiskSubscriber);
    CleanupStack::PopAndDestroy(outOfDiskPSObserver);
    CleanupStack::PopAndDestroy(outOfDiskMonitor);
    
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
}

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
