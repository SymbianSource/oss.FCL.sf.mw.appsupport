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
#include "TSanimmif.h"

#include "sanimimageplugin.h"

#include "testwaiter.h"

#include <ecom/ecom.h>

#pragma tagging OFF // To disable CodeTest tagging

const TInt KSAnimMifPluginImplUid = 0x2000B11E;

_LIT( KUnknownFile, "C:\\temp\\unknown.mif" );
_LIT( KCorruptFile, "C:\\temp\\corrupt.mif" );
_LIT( KStillImageFile, "C:\\temp\\testimg_1.mif" );
_LIT( KScaledAnimationFile, "C:\\temp\\testanim_1.mif" );
_LIT( KUnscaledAnimationFile, "C:\\temp\\testanim_2.mif" );
_LIT( KStillImageNoMaskFile, "C:\\temp\\testimg_2.mif" );
_LIT( KNoFramesFile, "C:\\temp\\testimg_3.mif" );



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
// CTSanimmif::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTSanimmif::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTSanimmif::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTSanimmif::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CTSanimmif::ExampleL ),
        ENTRY( "SetupEmpty", CTSanimmif::SetupEmptyL ),
        ENTRY( "SetupObj", CTSanimmif::SetupObjL ),
        ENTRY( "SetupFile", CTSanimmif::SetupFileL ),        
        ENTRY( "SetupStillImageNoMask", CTSanimmif::SetupStillImageNoMaskL ),
        ENTRY( "SetupStillImage", CTSanimmif::SetupStillImageL ),
        ENTRY( "SetupScaledAnimation", CTSanimmif::SetupScaledAnimationL ),
        ENTRY( "SetupUnscaledAnimation", CTSanimmif::SetupUnscaledAnimationL ),
        ENTRY( "SetupFinished", CTSanimmif::SetupFinishedL ),
        ENTRY( "Teardown", CTSanimmif::Teardown ),        
        ENTRY( "TestConstructor", CTSanimmif::TestConstructorL ),
        ENTRY( "TestConstructorNoMemory", CTSanimmif::TestConstructorNoMemoryL ),
        ENTRY( "TestUnknownFile", CTSanimmif::TestUnknownFileL ),
        ENTRY( "TestCorruptFile", CTSanimmif::TestCorruptFileL ),
        ENTRY( "TestNoFramesFile", CTSanimmif::TestNoFramesFileL ),
        ENTRY( "TestLoadStillImage", CTSanimmif::TestLoadStillImageL ),             
        ENTRY( "TestLoadScaledAnimation", CTSanimmif::TestLoadScaledAnimationL ),
        ENTRY( "TestLoadUnscaledAnimation", CTSanimmif::TestLoadUnscaledAnimationL ),
        ENTRY( "TestLoadNoMemory", CTSanimmif::TestLoadNoMemoryL ),
        ENTRY( "TestCancelLoad", CTSanimmif::TestCancelLoadL ),
        ENTRY( "TestShowStillImage", CTSanimmif::TestShowStillImageL ),
        ENTRY( "TestShowAnimation", CTSanimmif::TestShowAnimationL ),        
        ENTRY( "TestCancelStillImage", CTSanimmif::TestCancelStillImageL ),
        ENTRY( "TestCancelAnimation", CTSanimmif::TestCancelAnimationL ),
        ENTRY( "TestCancelNotActive", CTSanimmif::TestCancelNotActiveL ),
        ENTRY( "TestShowBeforeLoad", CTSanimmif::TestShowBeforeLoadL ),
        ENTRY( "TestSetDisplayMode", CTSanimmif::TestSetDisplayModeL ),
        ENTRY( "TestSetSize", CTSanimmif::TestSetSizeL ),        
        ENTRY( "TestSetFrameDelay", CTSanimmif::TestSetFrameDelayL ),
        ENTRY( "TestBackroundColour", CTSanimmif::TestBackroundColourL ),       
     
         //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
    
    // ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// TMockMifAnimationObserver::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void TMockMifAnimationObserver::UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ )
    {
    RDebug::Print( _L( "TMockMifAnimationObserver::UpdateScreen" ) );

    iTestSuite.iFrameCounter++;
    }


// ---------------------------------------------------------------------------
// TMockMifAnimationObserver::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void TMockMifAnimationObserver::UpdateScreen( const CFbsBitmap& /*aFrame*/ )
    {
    iTestSuite.iFrameCounter++;
    }





// ---------------------------------------------------------------------------
// CTSanimmif::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupEmptyL()
    {
    iTestObj = NULL;
    iDtorKey = TUid::Uid( 0 );
    User::LeaveIfError( iFs.Connect() );
    User::LeaveIfError( iFs.ShareProtected() );
    iWaiter = CTestWaiter::NewL();
    iFrameCounter = 0;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupObjL()
    {
    SetupEmptyL();
    iTestObj = static_cast<CSAnimImagePlugin*>(
        REComSession::CreateImplementationL(
            TUid::Uid( KSAnimMifPluginImplUid ), iDtorKey, &iObserver ) );
    iTestObj->SetSize( TSize( 128, 128 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::SetupFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupFileL( const TDesC& aFileName )
    {
    SetupObjL();
    iTestObj->Load( iFs, aFileName, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::SetupStillImageNoMaskL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupStillImageNoMaskL()
    {
    SetupFileL( KStillImageNoMaskFile );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::SetupStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupStillImageL()
    {
    SetupFileL( KStillImageFile );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::SetupScaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupScaledAnimationL()
    {
    SetupFileL( KScaledAnimationFile );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::SetupUnscaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupUnscaledAnimationL()
    {
    SetupFileL( KUnscaledAnimationFile );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::SetupFinishedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::SetupFinishedL()
    {
    SetupUnscaledAnimationL();
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::Teardown()
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
// CSAnimMifPluginTestSuite::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestConstructorL()
    {
    TAny* ptr = NULL;
    ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimMifPluginImplUid ), iDtorKey, &iObserver );

    iTestObj = static_cast<CSAnimImagePlugin*>( ptr );
  //  EUNIT_ASSERT( iTestObj );
    delete iTestObj;
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestConstructorNoMemoryL()
    {
    TAny* ptr = NULL;
    ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimMifPluginImplUid ), iDtorKey, &iObserver );
    //  EUNIT_ASSERT( !ptr );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestUnknownFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestUnknownFileL()
    {
    iTestObj->Load( iFs, KUnknownFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNotFound );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::TestCorruptFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestCorruptFileL()
    {
    iTestObj->Load( iFs, KCorruptFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCorrupt );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestNoFramesFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestNoFramesFileL()
    {
    iTestObj->Load( iFs, KNoFramesFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
 //   EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCorrupt );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestLoadStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestLoadStillImageL()
    {
    iTestObj->Load( iFs, KStillImageFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestLoadScaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestLoadScaledAnimationL()
    {
    iTestObj->Load( iFs, KScaledAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestLoadUnscaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestLoadUnscaledAnimationL()
    {
    iTestObj->Load( iFs, KUnscaledAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
 //   EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestLoadNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestLoadNoMemoryL()
    {
    iTestObj->Load( iFs, KUnscaledAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNoMemory );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestCancelLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestCancelLoadL()
    {
    iTestObj->Load( iFs, KUnscaledAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone ); // Load is syncronous
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestShowStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestShowStillImageL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
 //   EUNIT_ASSERT_EQUALS( iFrameCounter, 1 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestShowAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestShowAnimationL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
   // EUNIT_ASSERT( iFrameCounter > 1 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestCancelStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestCancelStillImageL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::TestCancelAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestCancelAnimationL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::TestCancelNotActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestCancelNotActiveL()
    {
    iTestObj->Cancel();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestShowBeforeLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestShowBeforeLoadL()
    {
    //EUNIT_ASSERT_PANIC( iTestObj->Start( iWaiter->iStatus ), "STARTUPANIMATION", 2 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimmif::TestSetDisplayModeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestSetDisplayModeL()
    {
    iTestObj->SetDisplayMode( EGray2 );
    iTestObj->Load( iFs, KStillImageFile, iWaiter->iStatus );
    iTestObj->SetDisplayMode( ENone );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetDisplayMode( EColor16MA );
    iTestObj->SetDisplayMode( EColor16MU );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetDisplayMode( EColor256 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestSetSizeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestSetSizeL()
    {
    iTestObj->SetSize( TSize( 10, 10 ) );
    iTestObj->Load( iFs, KStillImageFile, iWaiter->iStatus );
    iTestObj->SetSize( TSize( 10000, 10000 ) );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetSize( TSize( 0, 0 ) );
    iTestObj->SetSize( TSize( 2, 1000 ) );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetSize( TSize( 1, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestSetFrameDelayL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestSetFrameDelayL()
    {
    iTestObj->SetFrameDelay( 100 );
    iTestObj->Load( iFs, KUnscaledAnimationFile, iWaiter->iStatus );
    iTestObj->SetFrameDelay( 0 );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetFrameDelay( 12000 );
    iTestObj->SetFrameDelay( 50 );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetFrameDelay( 3 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimMifPluginTestSuite::TestBackroundColourL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimmif::TestBackroundColourL()
    {
    TRgb colour = iTestObj->BackroundColour();
   // EUNIT_ASSERT_EQUALS( colour, TRgb() ); // Empty
    return KErrNone;
    }



// -----------------------------------------------------------------------------
// CTSanimmif::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTSanimmif::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTSanimmif, "TSanimmif" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTSanimmif, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTSanimmif, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }


// End of File
