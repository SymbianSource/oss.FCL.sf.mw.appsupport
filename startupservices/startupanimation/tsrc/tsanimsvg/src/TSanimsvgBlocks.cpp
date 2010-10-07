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
#include "TSanimsvg.h"


#include "sanimimageplugin.h"
#include "testwaiter.h"

#include <ecom/ecom.h>


#pragma tagging OFF // To disable CodeTest tagging

const TInt KSAnimSvgPluginImplUid = 0x2000B122;
const TInt K1Second = 1000000;

_LIT( KUnknownFile, "C:\\temp\\unknown.svg" );
_LIT( KCorruptFile, "C:\\temp\\corrupt.svg" );
_LIT( KStillImageFile, "C:\\temp\\testimg_1.svg" );
_LIT( KScaledAnimationFile, "C:\\temp\\testanim_1.svg" );
_LIT( KUnscaledAnimationFile, "C:\\temp\\testanim_2.svg" );
_LIT( KAnimationWithTextFile, "C:\\temp\\testanim_3.svg" );
_LIT( KStillImageBinaryFile, "C:\\temp\\testimg_1.svgb" );
_LIT( KScaledAnimationBinaryFile, "C:\\temp\\testanim_1.svgb" );



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
// CTSanimsvg::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTSanimsvg::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTSanimsvg::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTSanimsvg::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CTSanimsvg::ExampleL ),
        ENTRY( "SetupEmpty", CTSanimsvg::SetupEmptyL ),
        ENTRY( "SetupObj", CTSanimsvg::SetupObjL ),
        ENTRY( "SetupStillImage", CTSanimsvg::SetupStillImageL ),
        ENTRY( "SetupScaledAnimation", CTSanimsvg::SetupScaledAnimationL ),
        ENTRY( "SetupUnscaledAnimation", CTSanimsvg::SetupUnscaledAnimationL ),
        ENTRY( "SetupAnimationWithText", CTSanimsvg::SetupAnimationWithTextL ),                                            
        ENTRY( "SetupStillImageBinary", CTSanimsvg::SetupStillImageBinaryL ),
        ENTRY( "SetupScaledAnimationBinary", CTSanimsvg::SetupScaledAnimationBinaryL ),
        ENTRY( "SetupFinished", CTSanimsvg::SetupFinishedL ),
        ENTRY( "Teardown", CTSanimsvg::Teardown ),
        ENTRY( "TestConstructor", CTSanimsvg::TestConstructorL ),
        ENTRY( "TestConstructorNoMemory", CTSanimsvg::TestConstructorNoMemoryL ),                                  
        ENTRY( "TestUnknownFile", CTSanimsvg::TestUnknownFileL ),
        ENTRY( "TestCorruptFile", CTSanimsvg::TestCorruptFileL ),
        ENTRY( "TestLoadStillImage", CTSanimsvg::TestLoadStillImageL ),
        ENTRY( "TestLoadScaledAnimation", CTSanimsvg::TestLoadScaledAnimationL ),
        ENTRY( "TestLoadUnscaledAnimation", CTSanimsvg::TestLoadUnscaledAnimationL ),
        ENTRY( "TestLoadAnimationWithText", CTSanimsvg::TestLoadAnimationWithTextL ),
        ENTRY( "TestLoadStillImageBinary", CTSanimsvg::TestLoadStillImageBinaryL ),
        ENTRY( "TestLoadScaledAnimationBinary", CTSanimsvg::TestLoadScaledAnimationBinaryL ),
        ENTRY( "TestLoadNoMemory", CTSanimsvg::TestLoadNoMemoryL ),
        ENTRY( "TestCancelLoad", CTSanimsvg::TestCancelLoadL ),
        ENTRY( "TestShowStillImage", CTSanimsvg::TestShowStillImageL ),
        ENTRY( "TestShowAnimation", CTSanimsvg::TestShowAnimationL ),
        ENTRY( "TestShowNoMemory", CTSanimsvg::TestShowNoMemoryL ),
        ENTRY( "TestCancelStillImage", CTSanimsvg::TestCancelStillImageL ),
        ENTRY( "TestCancelAnimation", CTSanimsvg::TestCancelAnimationL ),
        ENTRY( "TestCancelNotActive", CTSanimsvg::TestCancelNotActiveL ),
        ENTRY( "TestShowBeforeLoad", CTSanimsvg::TestShowBeforeLoadL ),
        ENTRY( "TestSetDisplayMode", CTSanimsvg::TestSetDisplayModeL ),
        ENTRY( "TestSetSize", CTSanimsvg::TestSetSizeL ),
        ENTRY( "TestSetFrameDelay", CTSanimsvg::TestSetFrameDelayL ),
        ENTRY( "TestBackroundColour", CTSanimsvg::TestBackroundColourL ),
                   
        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not rove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// TMockSvgAnimationObserver::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void TMockSvgAnimationObserver::UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ )
    {
    RDebug::Print( _L( "TMockSvgAnimationObserver::UpdateScreen" ) );

    iTestSuite.iFrameCounter++;
    }


// ---------------------------------------------------------------------------
// TMockSvgAnimationObserver::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void TMockSvgAnimationObserver::UpdateScreen( const CFbsBitmap& /*aFrame*/ )
    {
    iTestSuite.iFrameCounter++;
    }



// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupEmptyL()
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
// CSAnimSvgPluginTestSuite::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupObjL()
    {
    SetupEmptyL();
    iTestObj = static_cast<CSAnimImagePlugin*>(
        REComSession::CreateImplementationL(
            TUid::Uid( KSAnimSvgPluginImplUid ), iDtorKey, &iObserver ) );
    iTestObj->SetSize( TSize( 360 , 61 ) );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupFileL
//
// ---------------------------------------------------------------------------
//
void CTSanimsvg::SetupFileL( const TDesC& aFileName )
    {
    SetupObjL();
    iTestObj->Load( iFs, aFileName, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    	
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupStillImageL()
    {
    SetupFileL( KStillImageFile );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupScaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupScaledAnimationL()
    {
    SetupFileL( KScaledAnimationFile );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupUnscaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupUnscaledAnimationL()
    {
    SetupFileL( KUnscaledAnimationFile );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupAnimationWithTextL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupAnimationWithTextL()
    {
    SetupFileL( KAnimationWithTextFile );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupStillImageBinaryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupStillImageBinaryL()
    {
    SetupFileL( KStillImageBinaryFile );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupScaledAnimationBinaryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupScaledAnimationBinaryL()
    {
    SetupFileL( KScaledAnimationBinaryFile );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::SetupFinishedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::SetupFinishedL()
    {
    SetupScaledAnimationL();
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    	 return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::Teardown()
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
// CSAnimSvgPluginTestSuite::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestConstructorL()
    {
    TAny* ptr = NULL;
    ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimSvgPluginImplUid ),iDtorKey, &iObserver);

    iTestObj = static_cast<CSAnimImagePlugin*>( ptr );
   // EUNIT_ASSERT( iTestObj );
    delete iTestObj;
    iTestObj = NULL;
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestConstructorNoMemoryL()
    {
    TAny* ptr = NULL;
     ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimSvgPluginImplUid ), iDtorKey, &iObserver );
     	
    //EUNIT_ASSERT( !ptr );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestUnknownFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestUnknownFileL()
    {
    iTestObj->Load( iFs, KUnknownFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   if( iWaiter->iStatus.Int()== KErrGeneral ) // SVG engine returns "unknown error"
       {
       
       }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestCorruptFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestCorruptFileL()
    {
    iTestObj->Load( iFs, KCorruptFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrGeneral )// SVG engine returns "unknown error"
        {
        
        }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadStillImageL()
    {
    iTestObj->Load( iFs, KStillImageFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNone )
        {
        
        }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadScaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadScaledAnimationL()
    {
    iTestObj->Load( iFs, KScaledAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadUnscaledAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadUnscaledAnimationL()
    {
    iTestObj->Load( iFs, KUnscaledAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   if ( iWaiter->iStatus.Int()== KErrNone )
       {
       
       }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadAnimationWithTextL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadAnimationWithTextL()
    {
    iTestObj->Load( iFs, KAnimationWithTextFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNone )
        {
        
        }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadStillImageBinaryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadStillImageBinaryL()
    {
    iTestObj->Load( iFs, KStillImageBinaryFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   if ( iWaiter->iStatus.Int()== KErrNone )
       {
       
       }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadScaledAnimationBinaryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadScaledAnimationBinaryL()
    {
    iTestObj->Load( iFs, KScaledAnimationBinaryFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestLoadNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestLoadNoMemoryL()
    {
    iTestObj->Load( iFs, KAnimationWithTextFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if (iWaiter->iStatus.Int()== KErrNoMemory )
        {
        
        }
   return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestCancelLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestCancelLoadL()
    {
    iTestObj->Load( iFs, KAnimationWithTextFile, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   if ( iWaiter->iStatus.Int()== KErrNone )
       {
       }
       
        // Load is syncronous
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestShowStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestShowStillImageL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if (iWaiter->iStatus.Int() == KErrNone )
        {
        
        }
   // EUNIT_ASSERT_EQUALS( iFrameCounter, 1 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestShowAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestShowAnimationL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    //EUNIT_ASSERT( iFrameCounter > 1 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestShowNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestShowNoMemoryL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNoMemory )
        {
        
        }
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestCancelStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestCancelStillImageL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if (iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestCancelAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestCancelAnimationL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrCancel )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestCancelNotActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestCancelNotActiveL()
    {
    iTestObj->Cancel();
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestShowBeforeLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestShowBeforeLoadL()
    {
    /*
     [Test]
    title TestShowBeforeLoad
    create TSanimsvg AnimSvgPlugin
    AnimSvgPlugin SetupObj
    AnimSvgPlugin TestShowBeforeLoad
    AnimSvgPlugin Teardown
    delete AnimSvgPlugin
    [Endtest] 
       */
    
   // STIF_ASSERT_PANIC( iTestObj->Start( iWaiter->iStatus ), "STARTUPANIMATION", 2 );
   // iTestObj->Start( iWaiter->iStatus );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestSetDisplayModeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestSetDisplayModeL()
    {
    iTestObj->SetDisplayMode( EGray2 );
    iTestObj->Load( iFs, KStillImageFile, iWaiter->iStatus );
    iTestObj->SetDisplayMode( ENone );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        }
    
    iTestObj->SetDisplayMode( EColor16MA );
    iTestObj->SetDisplayMode( EColor16MU );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    iTestObj->SetDisplayMode( EColor256 );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestSetSizeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestSetSizeL()
    {
    iTestObj->SetSize( TSize( 10, 10 ) );
    iTestObj->Load( iFs, KStillImageFile, iWaiter->iStatus );
    iTestObj->SetSize( TSize( 10000, 10000 ) );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetSize( TSize( 0, 0 ) );
    iTestObj->SetSize( TSize( 2, 1000 ) );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    //iTestObj->SetSize( TSize( 1, 1 ) );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestSetFrameDelayL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimsvg::TestSetFrameDelayL()
    {
    iTestObj->SetFrameDelay( 100 );
    iTestObj->Load( iFs, KScaledAnimationFile, iWaiter->iStatus );
    iTestObj->SetFrameDelay( 0 );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    iTestObj->SetFrameDelay( 12000 );
    iTestObj->SetFrameDelay( 50 );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    iTestObj->SetFrameDelay( 3 );
     return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginTestSuite::TestBackroundColourL
//
// ---------------------------------------------------------------------------
//

TInt CTSanimsvg::TestBackroundColourL()
    {
    TRgb colour = iTestObj->BackroundColour();
   // EUNIT_ASSERT_EQUALS( colour, TRgb() ); // Empty
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CTSanimsvg::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTSanimsvg::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTSanimsvg, "TSanimsvg" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTSanimsvg, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTSanimsvg, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CTSanimsvg::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CTSanimsvg::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove

// End of File
