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
#include <e32def.h> 
#include <e32std.h>
#include <e32property.h> 
#include <UikonInternalPSKeys.h>


#include "oommonitorsession.h"
#include "utoommockclientserver.h"
#include "oommonitorclientserver.h"

#endif //STIF_UNIT_INCLUDE_SECTION_END

    #define KMaxServerNameLength 256
    #define KRequestMemoryValue 100
    #define KMinMemoryNeedValue 50
    #define KMemoryGoodSetKeyETrueValue 10    // >=1 for memorygood = ETrue else EFalse
    #define KMemoryGoodSetKeyEFalseValue 0    // >=1 for memorygood = ETrue else EFalse

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
	ROomMonitorSession ss;
	const TInt KCreateSessionRetryCount = 2;                    //CreateSession retry count
    const TInt KServerDefaultMessageSlots = 2;                  //server async Message Slots
    RProcess serverProcess;
    TRequestStatus waitStatus;
    TInt stubValue;
    TInt value2;
    const TInt KOomWatchDogStatusIdle = -1;

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
    TInt retry=KCreateSessionRetryCount; //number of CreateSession retries
    FOREVER
        {
        // try to create a session with the server which has KServerDefaultMessageSlots async message slots.
        TInt err;
        err = ss.Connect();
        if (err!=KErrNotFound && err!=KErrServerTerminated)
            break;
        STIF_ASSERT_NOT_EQUALS(retry, 0);
        //if (--retry==0)
        //  User::Leave(err);    
        err = serverProcess.Create(KMemoryMonitorServerName, KNullDesC);
        if (err!=KErrNone)
            User::Leave(err);   
        
        TRequestStatus stat;
        serverProcess.Rendezvous(stat);
    
        if (stat!=KRequestPending)
            serverProcess.Kill(KErrCouldNotConnect);       // abort startup
        else
            serverProcess.Resume();    // logon OK - start the server
        
        User::WaitForRequest(stat);     // wait for start or death
    
        // The server exit type may be a panic value and a panic value can be zero, which is the same 
        // value as KErrNone. So, the exit type is tested for a panic before being returned.
        if (serverProcess.ExitType()==EExitPanic)
            {
            err = KErrServerTerminated;
            }
        else
            {
            err = stat.Int();
            }
        iLog->Log(_L("start server return with %d"), err);
        if (err!=KErrNone && err!=KErrAlreadyExists)
            User::Leave(err);
        }

    _LIT_SECURITY_POLICY_PASS(KOomMemoryMonitorPolicyRead);
    
    RProperty::Define(KUTOomPropertyCategory, KUTOomStubPropertyKey, RProperty::EInt, KOomMemoryMonitorPolicyRead, KOomMemoryMonitorPolicyRead);
    RProperty::Define(KUTOomPropertyCategory, KUTOomFreeMemorySetKey, RProperty::EInt, KOomMemoryMonitorPolicyRead, KOomMemoryMonitorPolicyRead);
    RProperty::Define(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, RProperty::EInt, KOomMemoryMonitorPolicyRead, KOomMemoryMonitorPolicyRead);

    }

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
    {
    RProperty::Delete(KUTOomPropertyCategory, KUTOomStubPropertyKey);
    RProperty::Delete(KUTOomPropertyCategory, KUTOomFreeMemorySetKey);
    RProperty::Delete(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey);

    // close the session
    ss.Close();
    
    // kill server process
    serverProcess.Kill(KErrNone);
    iLog->Log(_L("server process killed"));
    
    }

/**
 * STIF_TESTDEFINE defines a test case
 *
 *  following test case are make call to client resession - ROomMonitorSession
 *  through client/server framework, control transfer to oomclientrequestqueue
 *  and then to oommemorymonitor (implementation is mockoommemorymonitor.cpp),
 *  in which 
 *  1. will change corresponding property's value to show an API had been called, and 
 *  2. will check corresponding property's value, and use these values to call 
 *     oomclientrequestqueue::ActionsCompleted(...), then complete RMessage2,
 *  then control transfer back to test code, so that we check return value and 
 *  corresponsing property's value, make verification, decide if test pass or not. 
 */
STIF_TESTDEFINE(RequestFreeMemory_KErrNone)
    {
    // 1. check call to RequestFreeMemory, 
    //  memory request can be fullfilled,
    //  return value should be KErrNone.
    //  make a call and check stub
    iLog->Log(_L("start RequestFreeMemory_KErrNone"));
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, 0 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, KMemoryGoodSetKeyETrueValue );
    ss.RequestFreeMemory(KRequestMemoryValue, waitStatus);
    User::WaitForRequest(waitStatus);
    iLog->Log(_L("wait status is %d"), waitStatus);
    RProperty::Get(KUTOomPropertyCategory, KUTOomStubPropertyKey, stubValue );
    RProperty::Get(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, value2 );
    iLog->Log(_L("call return and property value now is %d, %d"), stubValue, value2);
    STIF_ASSERT_EQUALS(stubValue, Stub_CMemoryMonitor_RequestFreeMemoryL);
    STIF_ASSERT_EQUALS(waitStatus.Int(), KErrNone);
    iLog->Log(_L("RequestFreeMemory_KErrNone passed"));
    }

STIF_TESTDEFINE(RequestFreeMemory_KErrNoMemory)
    {    
    // 2. check call to RequestFreeMemory, 
    //  memory request can not be fullfilled,
    //  return value should be KErrNoMemory.
    //  make a call and check stub
    iLog->Log(_L("start RequestFreeMemory_KErrNoMemory"));
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, 0 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, KMemoryGoodSetKeyEFalseValue );
    ss.RequestFreeMemory(KRequestMemoryValue, waitStatus);
    User::WaitForRequest(waitStatus);
    iLog->Log(_L("wait status is %d"), waitStatus);
    RProperty::Get(KUTOomPropertyCategory, KUTOomStubPropertyKey, stubValue );
    RProperty::Get(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, value2 );
    STIF_ASSERT_EQUALS(stubValue, Stub_CMemoryMonitor_RequestFreeMemoryL);
    STIF_ASSERT_EQUALS(waitStatus.Int(), KErrNoMemory);
    iLog->Log(_L("RequestFreeMemory_KErrNoMemory passed"));

    }

STIF_TESTDEFINE(Twice_RequestFreeMemory_KErrNoMemory)
    {    
    // 2. check call to RequestFreeMemory, 
    //  memory request can not be fullfilled,
    //  return value should be KErrNoMemory.
    //  make a call and check stub
    iLog->Log(_L("start RequestFreeMemory_KErrNoMemory"));
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, 0 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, KMemoryGoodSetKeyEFalseValue );
    TRequestStatus waitStatus_second;
    ss.RequestFreeMemory(KRequestMemoryValue, waitStatus);
    ss.RequestFreeMemory(KRequestMemoryValue, waitStatus_second);
    User::WaitForRequest(waitStatus);
    User::WaitForRequest(waitStatus_second);
    iLog->Log(_L("wait status is %d"), waitStatus);
    iLog->Log(_L("second wait status is %d"), waitStatus_second);
    RProperty::Get(KUTOomPropertyCategory, KUTOomStubPropertyKey, stubValue );
    RProperty::Get(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, value2 );
    STIF_ASSERT_EQUALS(stubValue, Stub_CMemoryMonitor_RequestFreeMemoryL);
    STIF_ASSERT_EQUALS(waitStatus.Int(), KErrNoMemory);
    iLog->Log(_L("RequestFreeMemory_KErrNoMemory passed"));

    }

STIF_TESTDEFINE(RequestOptionalRam_MemoryAvailable)
    {

    // 3. check call to RequestOptionalRam, 
    //  memory request can be fullfilled,
    //  return value should be memory available.
    //  make a call and check stub
    iLog->Log(_L("start RequestOptionalRam_MemoryAvailable"));
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, 0 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomFreeMemorySetKey, KMinMemoryNeedValue + 10 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, KMemoryGoodSetKeyETrueValue );
    ss.RequestOptionalRam(KRequestMemoryValue, KMinMemoryNeedValue, 1, waitStatus);
    User::WaitForRequest(waitStatus);
    iLog->Log(_L("wait status is %d"), waitStatus);
    RProperty::Get(KUTOomPropertyCategory, KUTOomStubPropertyKey, stubValue );
    RProperty::Get(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, value2 );
    iLog->Log(_L("call return and property value now is %d, %d"), stubValue, value2);
    STIF_ASSERT_EQUALS(stubValue, Stub_CMemoryMonitor_FreeOptionalRamL);
    STIF_ASSERT_EQUALS(waitStatus.Int(), 60);

    iLog->Log(_L("RequestOptionalRam_MemoryAvailable passed"));
    
    }

STIF_TESTDEFINE(RequestOptionalRam_KErrNoMemory)
    {

    // 3. check call to RequestOptionalRam, 
    //  memory request can be fullfilled,
    //  return value should be memory available.
    //  make a call and check stub
    iLog->Log(_L("start RequestOptionalRam_KErrNoMemory"));
    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, 0 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomFreeMemorySetKey, KMinMemoryNeedValue - 10 );
    RProperty::Set(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, KMemoryGoodSetKeyETrueValue );
    ss.RequestOptionalRam(KRequestMemoryValue, KMinMemoryNeedValue, 1, waitStatus);
    User::WaitForRequest(waitStatus);
    iLog->Log(_L("wait status is %d"), waitStatus);
    RProperty::Get(KUTOomPropertyCategory, KUTOomStubPropertyKey, stubValue );
    RProperty::Get(KUTOomPropertyCategory, KUTOomMemoryGoodSetKey, value2 );
    iLog->Log(_L("call return and property value now is %d, %d"), stubValue, value2);
    STIF_ASSERT_EQUALS(stubValue, Stub_CMemoryMonitor_FreeOptionalRamL);
    STIF_ASSERT_EQUALS(waitStatus.Int(), KErrNoMemory);

    iLog->Log(_L("RequestOptionalRam_KErrNoMemory passed"));
    
    }

/**
 * in this test case, 
 * 1. set property's value, 
 * 2. check that a stub property's value is changed while mockoommemorymonitor's API 
 *    being called.
 */
STIF_TESTDEFINE(KUikOOMWatchdogStatus_Changed)
    {
    // 4. check WatchdogStatusStatusChanged should be triggered,  
    //  memory request can be fullfilled,
    //  check that CMemoryMonitor_RequestFreeMemoryPandSL should be called
    

    RProperty::Set(KUTOomPropertyCategory, KUTOomStubPropertyKey, 0 );

    // set a value to trigger handle function in oomclientrequestqueue
    TInt err = RProperty::Set(KPSUidUikon, KUikOOMWatchdogStatus, KRequestMemoryValue );
    STIF_ASSERT_EQUALS(err, KErrNone);
        
    //wait aleast that last item in queue had been process and idel enough time
    User::After(1000000);    //microseconds
    
    // check stub property's value
    RProperty::Get(KUTOomPropertyCategory, KUTOomStubPropertyKey, stubValue );
    STIF_ASSERT_EQUALS(stubValue, Stub_CMemoryMonitor_RequestFreeMemoryPandSL);
    
    CleanupStack::PopAndDestroy();  // watchdogStatusProperty
    }

#endif
/**
 * END OF TEST CASES SECTION
 */


// End of File
