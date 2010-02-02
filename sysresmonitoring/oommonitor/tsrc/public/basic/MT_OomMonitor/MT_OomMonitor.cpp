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
* Description:
*
*/


//  CLASS HEADER
#include "MT_OomMonitor.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <oommonitorsession.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
MT_OomMonitor* MT_OomMonitor::NewL()
    {
    MT_OomMonitor* self = MT_OomMonitor::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_OomMonitor* MT_OomMonitor::NewLC()
    {
    MT_OomMonitor* self = new( ELeave ) MT_OomMonitor();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_OomMonitor::~MT_OomMonitor()
    {
    }

// Default constructor
MT_OomMonitor::MT_OomMonitor()
    {
    }

// Second phase construct
void MT_OomMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void MT_OomMonitor::EmptySetupL(  )
    {
    }

void MT_OomMonitor::EmptyTeardown(  )
    {
    }

void MT_OomMonitor::SetupL(  )
    {
    iROomMonitorSession.Connect();
    }
    

void MT_OomMonitor::Teardown(  )
    {
    iROomMonitorSession.Close(); 
    }
    

void MT_OomMonitor::T_ROomMonitorSession_ConnectL(  )
    {
    EUNIT_ASSERT_DESC( iROomMonitorSession.Connect( ) == KErrNone, "Session should have been connected");
    iROomMonitorSession.Close();
    }
    
void MT_OomMonitor::T_ROomMonitorSession_RequestFreeMemoryL(  )
    {
    TInt rtn = iROomMonitorSession.RequestFreeMemory( 1024 );
    EUNIT_ASSERT_DESC( rtn == KErrNone || rtn == KErrNoMemory, "OOM monitor should free 1024 bytes");
    }
    
void MT_OomMonitor::T_ROomMonitorSession_RequestFreeMemory2L(  )
    {
    TRequestStatus status;    
    iROomMonitorSession.RequestFreeMemory( 1024, status );
    User::WaitForRequest( status );
    EUNIT_ASSERT_DESC( status == KErrNone || status == KErrNoMemory, "OOM monitor should free 1024 bytes");
    }
    
void MT_OomMonitor::T_ROomMonitorSession_CancelRequestFreeMemoryL(  )
    {
    TRequestStatus status;    
    iROomMonitorSession.RequestFreeMemory( 1024, status );
    iROomMonitorSession.CancelRequestFreeMemory( );
    User::WaitForRequest( status );
    }
    
void MT_OomMonitor::T_ROomMonitorSession_ThisAppIsNotExitingL(  )
    {
    iROomMonitorSession.ThisAppIsNotExiting( 1 );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_OomMonitor,
    "ROomMonitorSession test suite",
    "MODULE" )

EUNIT_TEST(
    "Connect - test",
    "ROomMonitorSession",
    "Connect",
    "FUNCTIONALITY",
    EmptySetupL, T_ROomMonitorSession_ConnectL, EmptyTeardown)
    
EUNIT_TEST(
    "RequestFreeMemory -synchronous test",
    "ROomMonitorSession",
    "RequestFreeMemory - synchronous version",
    "FUNCTIONALITY",
    SetupL, T_ROomMonitorSession_RequestFreeMemoryL, Teardown)
    
EUNIT_TEST(
    "RequestFreeMemory - asynchronous test",
    "ROomMonitorSession",
    "RequestFreeMemory - asynchronous version",
    "FUNCTIONALITY",
    SetupL, T_ROomMonitorSession_RequestFreeMemory2L, Teardown)
    
EUNIT_TEST(
    "CancelRequestFreeMemory - test",
    "ROomMonitorSession",
    "CancelRequestFreeMemory",
    "FUNCTIONALITY",
    SetupL, T_ROomMonitorSession_CancelRequestFreeMemoryL, Teardown)
    
EUNIT_TEST(
    "ThisAppIsNotExiting -test",
    "ROomMonitorSession",
    "ThisAppIsNotExiting",
    "FUNCTIONALITY",
    SetupL, T_ROomMonitorSession_ThisAppIsNotExitingL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
