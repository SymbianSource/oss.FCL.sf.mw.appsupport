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
#include "Tsanimihl.h"


#include "sanimimageplugin.h"

#include "testwaiter.h"
#include <ecom/ecom.h>



// Praveen

const TInt KSAnimIhlPluginImplUid = 0x2000B11C;
const TInt K1Second = 1000000;

_LIT( KUnknownFile, "C:\\temp\\unknown.gif" );
_LIT( KCorruptFile, "C:\\temp\\corrupt.gif" );
_LIT( KStillImageGray2File, "C:\\temp\\still_image_gray2.gif" );
_LIT( KStillImageColourFile, "C:\\temp\\gif_2kb.gif" );
_LIT( KStillImageBgColourFile, "C:\\temp\\testimg_1.gif" );
_LIT( KMbmFile, "C:\\temp\\startupnokia_qvga.mbm" );
_LIT( KAnimationFile, "C:\\temp\\testanim_7.gif" );

// ---------------------------------------------------------------------------
// TMockIhlAnimationObserver::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void TMockIhlAnimationObserver::UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ )
    {
    iTestSuite.iFrameCounter++;
    }


// ---------------------------------------------------------------------------
// TMockIhlAnimationObserver::UpdateScreen
//
// ---------------------------------------------------------------------------
//
void TMockIhlAnimationObserver::UpdateScreen( const CFbsBitmap& /*aFrame*/ )
    {
    iTestSuite.iFrameCounter++;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::NewL
//
// ---------------------------------------------------------------------------
//
/*
CTsanimihl* CTsanimihl::NewL()
    {
    CTsanimihl* self = CTsanimihl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::NewLC
//
// ---------------------------------------------------------------------------
//
CTsanimihl* CTsanimihl::NewLC()
    {
    CTsanimihl* self = new( ELeave ) CTsanimihl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

*/

// -----------------------------------------------------------------------------
// CTsanimihl::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTsanimihl::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY("Example", CTsanimihl::ExampleL ),
        ENTRY("SetupEmpty",CTsanimihl::SetupEmptyL),
        ENTRY("SetupObj",CTsanimihl::SetupObjL),
        ENTRY("SetupStillImage",CTsanimihl::SetupStillImageL),
        ENTRY("SetupAnimation",CTsanimihl::SetupAnimationL),
        ENTRY("SetupMultiFrame",CTsanimihl::SetupMultiFrameL),
        ENTRY("SetupStillImageWithBgColour",CTsanimihl::SetupStillImageWithBgColourL),
        ENTRY("Teardown",CTsanimihl::Teardown),
        ENTRY("TestConstructor",CTsanimihl::TestConstructorL),
        ENTRY("TestConstructorNoMemory",CTsanimihl::TestConstructorNoMemoryL),
        ENTRY("TestUnknownFile",CTsanimihl::TestUnknownFileL),
        ENTRY("TestCorruptFile",CTsanimihl::TestCorruptFileL),
        ENTRY("TestLoadStillImageGray2",CTsanimihl::TestLoadStillImageGray2L),
        ENTRY("TestLoadStillImageColour",CTsanimihl::TestLoadStillImageColourL),
        ENTRY("TestLoadAnimation",CTsanimihl::TestLoadAnimationL),
        ENTRY("TestLoadMultiFrame",CTsanimihl::TestLoadMultiFrameL),
        ENTRY("TestLoadStillImageNoMemory",CTsanimihl::TestLoadStillImageNoMemoryL),
        ENTRY("TestLoadAnimationNoMemory",CTsanimihl::TestLoadAnimationNoMemoryL),
        ENTRY("TestCancelLoadStillImage",CTsanimihl::TestCancelLoadStillImageL),
        ENTRY("TestCancelLoadAnimation",CTsanimihl::TestCancelLoadAnimationL),
        ENTRY("TestCancelLoadMultiFrame",CTsanimihl::TestCancelLoadMultiFrameL),
        ENTRY("TestShowStillImage",CTsanimihl::TestShowStillImageL),
        ENTRY("TestShowStillImageWithFrameDelay",CTsanimihl::TestShowStillImageWithFrameDelayL),
        ENTRY("TestShowAnimation",CTsanimihl::TestShowAnimationL),
        ENTRY("TestShowMultiFrame",CTsanimihl::TestShowMultiFrameL),
        ENTRY("TestCancelPlay",CTsanimihl::TestCancelPlayL),
        ENTRY("TestBgColour",CTsanimihl::TestBgColourL),
        ENTRY("TestBgColourBeforeLoad",CTsanimihl::TestBgColourBeforeLoadL),
        ENTRY("TestScaleStillImage",CTsanimihl::TestScaleStillImageL)
                //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::CSAnimIhlPluginTestSuite
//
// ---------------------------------------------------------------------------
//
/*
CTsanimihl::CTsanimihl() : iObserver( *this )
    {
    }
*/

// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::ConstructL
//
// ---------------------------------------------------------------------------
//
/*
void CTsanimihl::ConstructL()
    {
   // REComSession::FinalClose();
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
   
    }
*/

// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt  CTsanimihl::SetupEmptyL()
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
// CSAnimIhlPluginTestSuite::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::SetupObjL()
    {
    SetupEmptyL();
    iTestObj = static_cast<CSAnimImagePlugin*>(
            REComSession::CreateImplementationL(
            TUid::Uid( KSAnimIhlPluginImplUid ), iDtorKey , &iObserver) );
    return KErrNone;
    
       
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::SetupStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::SetupStillImageL()
    {
    SetupObjL();
    iTestObj->Load( iFs, KStillImageGray2File, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::SetupAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::SetupAnimationL()
    {
    SetupObjL();
    iTestObj->Load( iFs, KAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::SetupMultiFrameL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::SetupMultiFrameL()
    {
    SetupObjL();
    iTestObj->Load( iFs, KMbmFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::SetupStillImageWithBgColourL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::SetupStillImageWithBgColourL()
    {
    SetupObjL();
    iTestObj->Load( iFs, KStillImageBgColourFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::Teardown()
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
// CSAnimIhlPluginTestSuite::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestConstructorL()
    {
    TAny* ptr = NULL;
     ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimIhlPluginImplUid ), iDtorKey, &iObserver );

    iTestObj = static_cast<CSAnimImagePlugin*>( ptr );
   // STIF_ASSERT( iTestObj );
    delete iTestObj;
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestConstructorNoMemoryL()
    {
    TAny* ptr = NULL;
    ptr = REComSession::CreateImplementationL(TUid::Uid( KSAnimIhlPluginImplUid ), iDtorKey, &iObserver );
    //STIF_ASSERT( !ptr );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestUnknownFileL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestUnknownFileL()
    {
    iTestObj->Load( iFs, KUnknownFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if (iWaiter->iStatus.Int()== KErrNotFound )
    {
    
    }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestCorruptFileL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestCorruptFileL()
    {
    iTestObj->Load( iFs, KCorruptFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if( iWaiter->iStatus.Int()== KErrNotFound ) // For some reason, IHL returns KErrNotFound...
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestLoadStillImageGray2L
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestLoadStillImageGray2L()
    {
    iTestObj->Load( iFs, KStillImageGray2File, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
                {
        
                }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestLoadStillImageColourL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestLoadStillImageColourL()
    {
    iTestObj->Load( iFs, KStillImageColourFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestLoadAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestLoadAnimationL()
    {
    iTestObj->Load( iFs, KAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestLoadMultiFrameL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestLoadMultiFrameL()
    {
    iTestObj->Load( iFs, KMbmFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestLoadStillImageNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestLoadStillImageNoMemoryL()
    {
    iTestObj->Load( iFs, KStillImageColourFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNoMemory )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestLoadAnimationNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestLoadAnimationNoMemoryL()
    {
    iTestObj->Load( iFs, KAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNoMemory )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestCancelLoadStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestCancelLoadStillImageL()
    {
    iTestObj->Load( iFs, KStillImageGray2File, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrCancel )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestCancelLoadAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestCancelLoadAnimationL()
    {
    iTestObj->Load( iFs, KAnimationFile, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrCancel )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestCancelLoadMultiFrameL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestCancelLoadMultiFrameL()
    {
    iTestObj->Load( iFs, KMbmFile, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrCancel )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestShowStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestShowStillImageL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }
    //STIF_ASSERT_EQUALS( iFrameCounter, 1 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestShowStillImageWithFrameDelayL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestShowStillImageWithFrameDelayL()
    {
    iTestObj->SetFrameDelay( K1Second );
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()==  KErrNone )
        {
        
        }
    //STIF_ASSERT_EQUALS( iFrameCounter, 1 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestShowAnimationL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestShowAnimationL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNone )
        {
        
        }
    //STIF_ASSERT_EQUALS( iFrameCounter, 43 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestShowMultiFrameL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestShowMultiFrameL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNone )
        {
        
        }
    //STIF_ASSERT_EQUALS( iFrameCounter, 34 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestCancelPlayL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestCancelPlayL()
    {
    iTestObj->SetFrameDelay( K1Second );
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
// CSAnimIhlPluginTestSuite::TestBgColourL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestBgColourL()
    {
    TRgb bgColour = iTestObj->BackroundColour();
    RDebug::Print( _L( "BG color: 0x%08x" ), bgColour.Value() );
    if ( bgColour.Value()== 0x00bf8040 )
        {
        
        }
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestBgColourBeforeLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestBgColourBeforeLoadL()
    {
    TRgb bgColour = iTestObj->BackroundColour();
   // STIF_ASSERT_EQUALS( bgColour.Value(), 0x00FFFFFF );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimIhlPluginTestSuite::TestScaleStillImageL
//
// ---------------------------------------------------------------------------
//
TInt CTsanimihl::TestScaleStillImageL()
    {
    iTestObj->SetSize( TSize( 126, 126 ) );

    iTestObj->Load( iFs, KStillImageGray2File, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrNone )
        {
        
        }

    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNone )
        {
        
        }
    //STIF_ASSERT_EQUALS( iFrameCounter, 1 );
    return KErrNone;
    }


// Praveen
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
// CTsanimihl::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTsanimihl::Delete() 
    {

    }




// -----------------------------------------------------------------------------
// CTsanimihl::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTsanimihl::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTsanimihl, "Tsanimihl" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTsanimihl, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTsanimihl, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }



// -----------------------------------------------------------------------------
// CTsanimihl::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CTsanimihl::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove

// End of File
