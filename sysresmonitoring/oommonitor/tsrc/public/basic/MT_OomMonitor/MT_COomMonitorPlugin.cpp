/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "MT_COomMonitorPlugin.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <oommonitorplugin.h>
#include <oommonitor.h>



MT_COomMonitorPlugin* MT_COomMonitorPlugin::NewL()
    {
    MT_COomMonitorPlugin* self = MT_COomMonitorPlugin::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_COomMonitorPlugin* MT_COomMonitorPlugin::NewLC()
    {
    MT_COomMonitorPlugin* self = new( ELeave ) MT_COomMonitorPlugin();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_COomMonitorPlugin::~MT_COomMonitorPlugin()
    {
    }

// Default constructor
MT_COomMonitorPlugin::MT_COomMonitorPlugin()
    {
    }

// Second phase construct
void MT_COomMonitorPlugin::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void MT_COomMonitorPlugin::BasicSetupL(  )
    {
    iMonitor = CMemoryMonitor::NewL();
    }
    

void MT_COomMonitorPlugin::BasicTeardown(  )
    {
    delete iMonitor;
    iMonitor = NULL; 
    }
    
void MT_COomMonitorPlugin::SetupL(  )
    {
    BasicSetupL();
    
    const TUid KMyAppUid = { 0x01234567};
    iCOomMonitorPlugin = CAppOomMonitorPlugin::NewL( KMyAppUid );
    }
    

void MT_COomMonitorPlugin::Teardown(  )
    {
    BasicTeardown();
    
    delete iCOomMonitorPlugin; 
    iCOomMonitorPlugin = NULL;
    }
    
    
void MT_COomMonitorPlugin::T_CAppOomMonitorPlugin_NewLL(  )
    {
    const TUid KMyAppUid = { 0x01234567};
    CAppOomMonitorPlugin* plugin = CAppOomMonitorPlugin::NewL( KMyAppUid );
    CleanupStack::PushL( plugin );
    
    EUNIT_ASSERT_DESC( plugin, "CAppOomMonitorPlugin instance not created!" );
    CleanupStack::PopAndDestroy( plugin );
    }
    
void MT_COomMonitorPlugin::T_COomMonitorPlugin_FreeRamL(  )
    {
    // private function in CAppOomMonitorPlugin, couldn't be tested
    //iCOomMonitorPlugin->FreeRam( );
    }
    
void MT_COomMonitorPlugin::T_COomMonitorPlugin_MemoryGoodL(  )
    {
    // private function in CAppOomMonitorPlugin, couldn't be tested
    //iCOomMonitorPlugin->MemoryGood( );
    }
    
void MT_COomMonitorPlugin::T_COomMonitorPlugin_FsSessionL(  )
    {
    iCOomMonitorPlugin->FsSession( );
    }
    
void MT_COomMonitorPlugin::T_COomMonitorPlugin_WsSessionL(  )
    {
    iCOomMonitorPlugin->WsSession( );
    }

void MT_COomMonitorPlugin::T_COomMonitorPlugin_ExtensionInterfaceL(  )
    {
	CTestOomMonitorPlugin* plugin = new(ELeave) CTestOomMonitorPlugin;
	CleanupStack::PushL(plugin);
	plugin->ConstructL();
	
	const TUid KTestUid = { 0x01234567 };      // a nonsense UID for testing
	TAny* any = NULL;
	plugin->ExtensionInterface( KTestUid, any );
	
	CleanupStack::PopAndDestroy(plugin);
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_COomMonitorPlugin,
    "COomMonitorPlugin test suite",
    "MODULE" )

EUNIT_TEST(
    "NewL - test",
    "COomMonitorPlugin",
    "CAppOomMonitorPlugin NewL and COomMonitorPlugin Constructor, Destructor",
    "FUNCTIONALITY",
    BasicSetupL, T_CAppOomMonitorPlugin_NewLL, BasicTeardown)
    
EUNIT_TEST(
    "FsSession - test",
    "COomMonitorPlugin",
    "FsSession",
    "FUNCTIONALITY",
    SetupL, T_COomMonitorPlugin_FsSessionL, Teardown)
    
EUNIT_TEST(
    "WsSession - test",
    "COomMonitorPlugin",
    "WsSession",
    "FUNCTIONALITY",
    SetupL, T_COomMonitorPlugin_WsSessionL, Teardown)
    
EUNIT_TEST(
    "ExtensionInterface - test",
    "COomMonitorPlugin",
    "ExtensionInterface",
    "FUNCTIONALITY",
    SetupL, T_COomMonitorPlugin_ExtensionInterfaceL, Teardown)    

EUNIT_END_TEST_TABLE

//  END OF FILE
