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
* Description: This file contains testclass implementation.
*
*/

// INCLUDE FILES
#include <StifTestInterface.h>
#include "AccFwUiNotifierTestModule.h"
#include <SettingServerClient.h>
#include "AccFwUiNoteNotifier.h"

#include <accfwnoteuinotifier.rsg>
//#include <e32des8.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
// CONSTANTS


// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS


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
// CAccFwUiNotifierTestModule::CAccFwUiNotifierTestModule
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAccFwUiNotifierTestModule::CAccFwUiNotifierTestModule( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CAccFwUiNotifierTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAccFwUiNotifierTestModule::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
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
        logFileName.Format(KAccFwUiNotifierTestModuleLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KAccFwUiNotifierTestModuleLogFile);
        }

    iLog = CStifLogger::NewL( KAccFwUiNotifierTestModuleLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// CAccFwUiNotifierTestModule::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAccFwUiNotifierTestModule* CAccFwUiNotifierTestModule::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CAccFwUiNotifierTestModule* self = new (ELeave) CAccFwUiNotifierTestModule( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// -----------------------------------------------------------------------------
// CAccFwUiNotifierTestModule::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CAccFwUiNotifierTestModule::Delete() 
    {

    }


// Destructor
CAccFwUiNotifierTestModule::~CAccFwUiNotifierTestModule()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// CAccFwUiNotifierTestModule::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CAccFwUiNotifierTestModule::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("AccFwUiNotifierTestModule.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	}

// ------------------------------------------------------------------------------
// PanicClient
// ------------------------------------------------------------------------------
//
void CAccFwUiNotifierTestModule::PanicClient(TInt aPanic)
    {
    _LIT(KAccFwUiNotifierTestModulePanic, "AccFwUiNotifierTestModule");
    User::Panic(KAccFwUiNotifierTestModulePanic, aPanic);
    }
// -----------------------------------------------------------------------------
// CAccFwUiNotifierTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CAccFwUiNotifierTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CreateNotifierPlugin", CAccFwUiNotifierTestModule::CreateNotifierPlugin ),
        ENTRY( "TestSupportedAccessoryUiNoteL", CAccFwUiNotifierTestModule::TestSupportedAccessoryUiNoteL ),
        ENTRY( "TestUnsupportedAccessoryUiNoteL", CAccFwUiNotifierTestModule::TestUnsupportedAccessoryUiNoteL ),
        ENTRY( "TestUpdateNotifierL", CAccFwUiNotifierTestModule::TestUpdateNotifierL ),
        ENTRY( "TestStartNotifierL", CAccFwUiNotifierTestModule::TestStartNotifierL ),
    
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


TInt CAccFwUiNotifierTestModule::CreateNotifierPlugin(CStifItemParser& /*aItem */)
    {
    RDebug::Print(_L("CAccFwUiNotifierTestModule::CreatePlugin"));
    TUid accfwuinotifierUID = {0x10205060};
    iNotifierPlugin = reinterpret_cast < CAccFwUiNoteNotifier* >(REComSession::CreateImplementationL (accfwuinotifierUID, idtorID ) );

    STIF_ASSERT_NOT_NULL( iNotifierPlugin );
    RDebug::Print(_L("iNotifierPlugin = %x"),iNotifierPlugin);
        
    return KErrNone;
    
    }

TInt CAccFwUiNotifierTestModule::TestSupportedAccessoryUiNoteL(CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("CAccFwUiNotifierTestModule::AccessoryUiNote"));
    RNotifier notifier;
    notifier.Connect();
    TInt selectionListBitmask = 255;

    TInt reply = 0;
    
    
    TPckg<TInt> replyPck( reply);
    
    TPckg<TUint32> selectionBitmaskPackage( selectionListBitmask);
    //activate active object
    TRequestStatus status;
    status = KRequestPending;
    
    //open ui
    
    notifier.StartNotifierAndGetResponse( status, KAccFwUiNoteNotifierUid, selectionBitmaskPackage, replyPck);
    User::WaitForRequest( status);
    
    notifier.Close();

    return KErrNone;
    }

TInt CAccFwUiNotifierTestModule::TestUnsupportedAccessoryUiNoteL(CStifItemParser& /*aItem*/ )
    {
     RDebug::Print(_L("CAccFwUiNotifierTestModule::AccessoryUiNote"));
     RNotifier notifier;
     notifier.Connect();
     //TInt selectionListBitmask = 255;

     TInt reply = 0;
     
     
     TPckg<TInt> replyPck( reply);

     //activate active object
     TRequestStatus status;
     status = KRequestPending;
     
     //open ui
     
     notifier.StartNotifierAndGetResponse( status, KAccFwUiNoteNotifierUid, KNullDesC8, replyPck);
     User::WaitForRequest( status);

     notifier.Close();

    return KErrNone;
    }

TInt CAccFwUiNotifierTestModule::TestUpdateNotifierL(CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("CAccFwUiNotifierTestModule::TestUpdateNotifierL"));
    RNotifier notifier;
    notifier.Connect();
    //TInt selectionListBitmask = 255;
    
    TInt reply = 0;
    
    
    TPckg<TInt> replyPck( reply);
    
    //activate active object
    TRequestStatus status;
    status = KRequestPending;
    
    //open ui
    
    notifier.StartNotifierAndGetResponse( status, KAccFwUiNoteNotifierUid, KNullDesC8, replyPck);
    User::WaitForRequest( status);
    
    TBuf8<1> answer;
    TInt err = notifier.UpdateNotifier(KAccFwUiNoteNotifierUid, KNullDesC8, answer);
    if (err)
        {
        TBuf<4> error;
        error.Zero();
        error.AppendNum(err);
        //CEikonEnv::InfoWinL(_L("Error:"), error);
        }

    User::WaitForRequest( status);
    
    notifier.Close();

    return KErrNone;
    }

TInt CAccFwUiNotifierTestModule::TestStartNotifierL(CStifItemParser& /*aItem*/ )
    {
    RDebug::Print(_L("CAccFwUiNotifierTestModule::TestStart2L-enter"));
    const TInt KGeneralBufferLength = 32;
    
    TBuf8<KGeneralBufferLength> responseBuffer;
     
     RNotifier notifier;
     User::LeaveIfError( notifier.Connect() );
     
     //Call Info() function.
     notifier.StartNotifier( KAccFwUiNoteNotifierUid,KNullDesC8, responseBuffer);
     
     notifier.Close();

    
    return KErrNone;
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

    return ( CScriptBase* ) CAccFwUiNotifierTestModule::NewL( aTestModuleIf );

    }

// End of File
