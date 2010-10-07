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
* Description: This file contains testclass implementation.
*
*/

// INCLUDE FILES
#include <StifTestInterface.h>
#include "TSanimctrl.h"
#include <SettingServerClient.h>

const TInt KTestRepository( 0x0AA0151E ); // Repository to use for testing
#include "sanimmockview.h"
#include "sanimctrlrootsuite.h"


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTSanimctrl::CTSanimctrl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTSanimctrl::CTSanimctrl( 
    CTestModuleIf& aTestModuleIf,CSAnimMockView& view ):
        CScriptBase( aTestModuleIf ),
        iView(view)
    {
    
    }

// -----------------------------------------------------------------------------
// CTSanimctrl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTSanimctrl::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
   // iView = CSAnimMockView::NewL();
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KTSanimctrlLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KTSanimctrlLogFile);
        }

    iLog = CStifLogger::NewL( KTSanimctrlLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    iParams.iRepositoryUid = TUid::Uid( KTestRepository );
          iParams.iAnimationPathKey = 1;
          iParams.iFrameDelayKey = 2;
          iParams.iTonePathKey = 3;
          iParams.iVolumeKey = 4;
       
    }

// -----------------------------------------------------------------------------
// CTSanimctrl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTSanimctrl* CTSanimctrl::NewL( 
    CTestModuleIf& aTestModuleIf,CSAnimMockView& aview )
    {
    CTSanimctrl* self = new (ELeave) CTSanimctrl( aTestModuleIf,aview );
   // CSAnimMockView *iView = CSAnimMockView::NewL();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CTSanimctrl::~CTSanimctrl()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// CTSanimctrl::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CTSanimctrl::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("TSanimctrl.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {
    CSAnimCtrlRootSuite* rootSuite = CSAnimCtrlRootSuite::NewL();
    return ( CScriptBase* ) CTSanimctrl::NewL( aTestModuleIf ,rootSuite->MainView());

    }

// End of File
