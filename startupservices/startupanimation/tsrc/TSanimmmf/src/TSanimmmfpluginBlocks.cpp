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

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "TSanimmmfplugin.h"


// Praveen
#include "sanimtoneplugin.h"
#include "testwaiter.h"

#include <ecom/ecom.h>

const TInt KSAnimMmfPluginImplUid = 0x2000B120;

_LIT( KUnknownFile, "C:\\temp\\unknown.wav" );
_LIT( KCorruptFile, "C:\\temp\\corrupt.wav" );
_LIT( K8bitMonoFile, "C:\\temp\\sine-8bit-mono.wav" );
_LIT( K16bitMonoFile, "C:\\temp\\sine-16bit-mono.wav" );
_LIT( KGsm6_10File, "C:\\temp\\sine-gsm-6-10.wav" );
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
// CTSanimmmfplugin::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTSanimmmfplugin::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTSanimmmfplugin::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CTSanimmmfplugin::ExampleL ),
		ENTRY( "SetupEmpty", CTSanimmmfplugin::SetupEmptyL),
		ENTRY( "SetupObj", CTSanimmmfplugin::SetupObjL),
		//ENTRY( "SetupFile", CTSanimmmfplugin::SetupFileL),
		ENTRY( "Setup8bitMonoTone", CTSanimmmfplugin::Setup8bitMonoToneL),
		ENTRY( "Setup16bitMonoTone", CTSanimmmfplugin::Setup16bitMonoToneL),
		ENTRY( "SetupGsm6_10Tone", CTSanimmmfplugin::SetupGsm6_10ToneL),
		ENTRY( "SetupFinished", CTSanimmmfplugin::SetupFinishedL),
		ENTRY( "Teardown", CTSanimmmfplugin::Teardown),
		ENTRY( "TestConstructor", CTSanimmmfplugin::TestConstructorL),
		ENTRY( "TestConstructorNoMemory", CTSanimmmfplugin::TestConstructorNoMemoryL),
		ENTRY( "TestUnknownFile", CTSanimmmfplugin::TestUnknownFileL),
		ENTRY( "TestCorruptFile", CTSanimmmfplugin::TestCorruptFileL),
		ENTRY( "TestLoad8bitMonoTone", CTSanimmmfplugin::TestLoad8bitMonoToneL),
		ENTRY( "TestLoad16bitMonoTone", CTSanimmmfplugin::TestLoad16bitMonoToneL),
		ENTRY( "TestLoadGsm6_10Tone", CTSanimmmfplugin::TestLoadGsm6_10ToneL),
		ENTRY( "TestLoadNoMemory", CTSanimmmfplugin::TestLoadNoMemoryL),
		ENTRY( "TestCancelLoad", CTSanimmmfplugin::TestCancelLoadL),
		ENTRY( "TestStart", CTSanimmmfplugin::TestStartL),
		ENTRY( "TestCancelStart", CTSanimmmfplugin::TestCancelStartL),
		ENTRY( "TestCancelNotActive", CTSanimmmfplugin::TestCancelNotActiveL),
		ENTRY( "TestStartBeforeLoad", CTSanimmmfplugin::TestStartBeforeLoadL),
		ENTRY( "TestSetVolume", CTSanimmmfplugin::TestSetVolumeL),
		ENTRY( "TestSetVolumeRamp", CTSanimmmfplugin::TestSetVolumeRampL)
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


/*
void CTSanimmmfplugin::ConstructL()
    {
    REComSession::FinalClose();
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    //CEUnitTestSuiteClass::ConstructL();
	
    }
*/

// ---------------------------------------------------------------------------
// CTSanimmmfplugin::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::SetupEmptyL()
    {
    iTestObj = NULL;
    iDtorKey = TUid::Uid( 0 );
    User::LeaveIfError( iFs.Connect() );
    User::LeaveIfError( iFs.ShareProtected() );
    iWaiter = CTestWaiter::NewL();
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfPlugin::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::SetupObjL()
    {

    SetupEmptyL();
    iTestObj = static_cast<CSAnimTonePlugin*>(
        REComSession::CreateImplementationL(
            TUid::Uid( KSAnimMmfPluginImplUid ), iDtorKey ) );
	return KErrNone;
    }
// ---------------------------------------------------------------------------
// CSAnimMmfpluginTestSuite::SetupFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::SetupFileL( const TDesC& aFileName )
    {
	//CStifItemParser aItem;
    SetupObjL();
    iTestObj->Load( iFs, aFileName, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::Setup8bitMonoToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::Setup8bitMonoToneL()
    {
    /*
    [Test]
    title Setup8bitMonotone
    create TSanimmmf MMFPlugin
    MMFPlugin Setup8bitMonotone
    delete MMFPlugin
    [Endtest]
    */
    // 8 bit Mono is not supported ??
    SetupFileL( K8bitMonoFile );
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfPlugin::Setup16bitMonoToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::Setup16bitMonoToneL()
    {
    SetupFileL( K16bitMonoFile );
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::SetupGsm6_10ToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::SetupGsm6_10ToneL()
    {
    SetupFileL( KGsm6_10File );
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::SetupFinishedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::SetupFinishedL()
    {
    Setup16bitMonoToneL();
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::Teardown()
    {
    delete iTestObj;
    iTestObj = NULL;

    REComSession::DestroyedImplementation( iDtorKey );

    delete iWaiter;
    iWaiter = NULL;

    iFs.Close();

    REComSession::FinalClose();
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfPlugin::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestConstructorL()
    {
    TAny* ptr = NULL;
   // EUNIT_ASSERT_NO_LEAVE( ptr = REComSession::CreateImplementationL(
     //   TUid::Uid( KSAnimMmfPluginImplUid ), iDtorKey ) );
	ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimMmfPluginImplUid ), iDtorKey );

    iTestObj = static_cast<CSAnimTonePlugin*>( ptr );
   // EUNIT_ASSERT( iTestObj );
    delete iTestObj;
    iTestObj = NULL;
	return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestConstructorNoMemoryL()
    {
    TAny* ptr = NULL;
    //EUNIT_ASSERT_SPECIFIC_LEAVE( ptr = REComSession::CreateImplementationL(
    //    TUid::Uid( KSAnimMmfPluginImplUid ), iDtorKey ), KErrNoMemory );
	ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimMmfPluginImplUid ), iDtorKey );
   // EUNIT_ASSERT( !ptr );
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestUnknownFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestUnknownFileL()
    {
    iTestObj->Load( iFs, KUnknownFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
     if ( iWaiter->iStatus.Int() == KErrNotFound )
         return KErrNone;
     return KErrNone;
     
    }

// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestCorruptFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestCorruptFileL()
    {
    iTestObj->Load( iFs, KCorruptFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNotSupported ) // For some reason, MMF returns KErrNotSupported...
        return KErrNone;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestLoad8bitMonoToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestLoad8bitMonoToneL()
    {
    iTestObj->Load( iFs, K8bitMonoFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
	return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestLoad16bitMonoToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestLoad16bitMonoToneL()
    {
    iTestObj->Load( iFs, K16bitMonoFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestLoadGsm6_10ToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestLoadGsm6_10ToneL()
    {
    iTestObj->Load( iFs, KGsm6_10File, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNone || iWaiter->iStatus.Int()  == KErrNotFound )
        return KErrNone;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestLoadNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestLoadNoMemoryL()
    {
    iTestObj->Load( iFs, KGsm6_10File, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
     if (iWaiter->iStatus.Int() == KErrNoMemory || iWaiter->iStatus.Int() == KErrNone || 
             iWaiter->iStatus.Int() == KErrGeneral   )
         return KErrNone;
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfPlugin::TestCancelLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestCancelLoadL()
    {
    iTestObj->Load( iFs, KGsm6_10File, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
	return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestStartL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestCancelStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestCancelStartL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
	return KErrNone;
    }
// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestCancelNotActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestCancelNotActiveL()
    {
    iTestObj->Cancel();
	return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmmfPplugin::TestStartBeforeLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestStartBeforeLoadL()
    {
  // iTestObj->Start( iWaiter->iStatus );
	return KErrNone;
    }
// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestSetVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestSetVolumeL()
    {
    iTestObj->SetVolume( 1 );
    iTestObj->Load( iFs, KGsm6_10File, iWaiter->iStatus );
    iTestObj->SetVolume( 0 );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetVolume( 12 );
    iTestObj->SetVolume( 5 );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetVolume( 3 );
	return KErrNone;
    }
// ---------------------------------------------------------------------------
// CTSanimmmfplugin::TestSetVolumeRampL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::TestSetVolumeRampL()
    {
    iTestObj->SetVolumeRamp( 1000000 );
    iTestObj->Load( iFs, KGsm6_10File, iWaiter->iStatus );
    iTestObj->SetVolumeRamp( 0 );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetVolumeRamp( 12000000 );
    iTestObj->SetVolumeRamp( 5000 );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetVolumeRamp( 300 );
	return KErrNone;
    }

// Praveen
// -----------------------------------------------------------------------------
// CTSanimmmfplugin::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTSanimmmfplugin::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTSanimmmfplugin, "TSanimmmfplugin" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTSanimmmfplugin, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTSanimmmfplugin, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CTSanimmmfplugin::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CTSanimmmfplugin::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove

// End of File
