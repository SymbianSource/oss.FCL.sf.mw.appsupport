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
#include "TSanimctrl.h"

#include <centralrepository.h>

#include <profile.hrh>
#include <profileenginesdkcrkeys.h>

//#include "sanimstartupctrltestsuite.h"
#include "sanimmockview.h"
#include "testwaiter.h"


#include "sanimctrl.h"
//#include "sanimctrltestsuite.h"


//#pragma tagging OFF // To disable CodeTest tagging

_LIT( KDummyImageFile, "C:\\temp\\test.ti2" ); // There is a plug-in for this type
_LIT( KDummyToneFile, "C:\\temp\\test.tt2" ); // There is a plug-in for this type
//#pragma tagging OFF // To disable CodeTest tagging

const TInt KTestRepository( 0x0AA0151E ); // Repository to use for testing

_LIT( KUnknownFile, "C:\\temp\\unknown.unk" );
_LIT( KProtectedFile, "Z:\\sys\\bin\\sysap.exe" );
//_LIT( KDummyImageFile, "C:\\temp\\test.ti2" ); // There is a plug-in for this type
//_LIT( KDummyToneFile, "C:\\temp\\test.tt2" ); // There is a plug-in for this type

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
// CTSanimctrl::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTSanimctrl::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTSanimctrl::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTSanimctrl::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static  TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY("Example", CTSanimctrl::ExampleL),
        ENTRY("InitObj", CTSanimctrl::InitObjL),
        ENTRY("SetupEmpty", CTSanimctrl::SetupEmptyL),
        ENTRY("SetupEmptyValues", CTSanimctrl::SetupEmptyValuesL),
        ENTRY("SetupUnknownFile", CTSanimctrl::SetupUnknownFileL),
        ENTRY("SetupProtectedFile", CTSanimctrl::SetupProtectedFileL),
        ENTRY("SetupExistingImageFile", CTSanimctrl::SetupExistingImageFileL),
        ENTRY("SetupExistingToneFileSilent", CTSanimctrl::SetupExistingToneFileSilentL),
        ENTRY("SetupExistingToneFileVeryLowVolume", CTSanimctrl::SetupExistingToneFileVeryLowVolumeL),
        ENTRY("SetupExistingToneFileLowVolume", CTSanimctrl::SetupExistingToneFileLowVolumeL),
        ENTRY("SetupExistingToneFileNormalVolume", CTSanimctrl::SetupExistingToneFileNormalVolumeL),
        ENTRY("SetupExistingToneFileTooLowVolume", CTSanimctrl::SetupExistingToneFileTooLowVolumeL),
        ENTRY("SetupExistingToneFileTooHighVolume", CTSanimctrl::SetupExistingToneFileTooHighVolumeL),
        ENTRY("SetupUnknownToneFileNormalVolume", CTSanimctrl::SetupUnknownToneFileNormalVolumeL),
        ENTRY("SetupObj", CTSanimctrl::SetupObjL),
        ENTRY("SetupEmptyShow", CTSanimctrl::SetupEmptyShowL),
        ENTRY("SetupShow", CTSanimctrl::SetupShowL),
        ENTRY("Teardown", CTSanimctrl::Teardown),
        ENTRY("TestConstructor", CTSanimctrl::TestConstructorL),
        ENTRY("TestConstructorNoMemory", CTSanimctrl::TestConstructorNoMemoryL),
        ENTRY( "TestLoadNoContent", CTSanimctrl::TestLoadNoContentL),
        ENTRY("TestLoadHasContent", CTSanimctrl::TestLoadHasContentL),
        ENTRY("TestLoadNoMemory", CTSanimctrl::TestLoadNoMemoryL),
        ENTRY("TestCancelLoad", CTSanimctrl::TestCancelLoadL),
        ENTRY("TestLoadWhenActive", CTSanimctrl::TestLoadWhenActiveL),
        ENTRY("TestStart", CTSanimctrl::TestStartL),
        ENTRY("TestCancelStart", CTSanimctrl::TestCancelStartL),
        ENTRY("TestStartWhenActive", CTSanimctrl::TestStartWhenActiveL),
       
        ENTRY("SetupEmptyCtr", CTSanimctrl::SetupEmptyCtrL),
        ENTRY("SetupObjCtr", CTSanimctrl::SetupObjCtrL),
        ENTRY("SetupObjLoadedCtr", CTSanimctrl::SetupObjLoadedCtrL),
        ENTRY("TeardownCtr", CTSanimctrl::TeardownCtr),
        ENTRY("TestConstructorCtr", CTSanimctrl::TestConstructorCtrL),
        ENTRY("TestConstructorNoMemoryCtr", CTSanimctrl::TestConstructorNoMemoryCtrL),
        ENTRY("TestLoadEmptyCtr", CTSanimctrl::TestLoadEmptyCtrL),
        ENTRY("TestLoadImageCtr", CTSanimctrl::TestLoadImageCtrL),
        ENTRY("TestLoadToneCtr", CTSanimctrl::TestLoadToneCtrL),
        ENTRY("TestLoadBothCtr", CTSanimctrl::TestLoadBothCtrL),
        ENTRY("TestLoadWhenActiveCtr", CTSanimctrl::TestLoadWhenActiveCtrL),
        ENTRY("TestCancelLoadCtr", CTSanimctrl::TestCancelLoadCtrL),
        ENTRY("TestStartCtr", CTSanimctrl::TestStartCtrL),
        ENTRY("TestCancelStartCtr", CTSanimctrl::TestCancelStartCtrL),
        ENTRY("TestStartNoMemoryCtr",CTSanimctrl::TestStartNoMemoryCtrL),
        ENTRY("TestStartWhenActiveCtr",CTSanimctrl::TestStartWhenActiveCtrL)
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// Praveen-2

// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupEmptyCtrL()
    {
    iTestObj2 = NULL;
    iWaiter = CTestWaiter::NewL();
    iWaiter2 = CTestWaiter::NewL();
    iView.SetComponent( *iTestObj2 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupObjCtrL()
    {
    SetupEmptyCtrL();
    //CSAnimMockView& view = *iView;
    iTestObj2 = CSAnimCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj2 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::SetupObjLoadedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupObjLoadedCtrL()
    {
    SetupObjCtrL();
    iTestObj2->Load(
        KDummyImageFile, 1000, ETrue, 1,
        KDummyToneFile, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // User::LeaveIfError( iWaiter->iStatus.Int() );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TeardownCtr()
    {
    delete iWaiter;
    iWaiter = NULL;
    delete iWaiter2;
    iWaiter2 = NULL;
    delete iTestObj;
    iTestObj2 = NULL;

    REComSession::FinalClose();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestConstructorCtrL()
    {
    //CSAnimMockView& view = *iView;
    iTestObj2 = CSAnimCtrl::NewL( iView.Rect(), iView );
    //STIF_ASSERT_EQUALS( iTestObj,KErrNone );
    delete iTestObj;
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestConstructorNoMemoryCtrL()
    {
    //CSAnimMockView& view = *iView;
    iTestObj2 = CSAnimCtrl::NewL( iView.Rect(), iView );
    //STIF_ASSERT_EQUALS( !iTestObj , KErrNone);
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestLoadEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadEmptyCtrL()
    {
    /*
     * Returns panic.. valid condition
    [Test]
    title TestLoadEmptyCtr
    create TSanimctrl CSAnimStartupCtrl
    CSAnimStartupCtrl SetupEmptyCtr
    CSAnimStartupCtrl TestLoadEmptyCtr
    CSAnimStartupCtrl TeardownCtr
    delete CSAnimStartupCtrl
    [Endtest] 

    iTestObj2->Load(
        KNullDesC, 1000, ETrue, 1,
        KNullDesC, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
 //   STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
  * 
  */
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestLoadImageL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadImageCtrL()
    {
    iTestObj2->Load(
        KDummyImageFile, 1000, ETrue, 1,
        KNullDesC, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
//    STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestLoadToneL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadToneCtrL()
    {
    iTestObj2->Load(
        KNullDesC, 1000, ETrue, 1,
        KDummyToneFile, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestLoadBothL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadBothCtrL()
    {
    iTestObj2->Load(
        KDummyImageFile, 1000, ETrue, 1,
        KDummyToneFile, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
 //   STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestLoadWhenActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadWhenActiveCtrL()
    {
    iTestObj2->Load(
        KDummyImageFile, 1000, ETrue, 1,
        KNullDesC, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj2->Load(
        KNullDesC, 1000, ETrue, 1,
        KDummyToneFile, 4, 0, 1,
        iWaiter2->iStatus );
    iWaiter2->Activate();
    CActiveScheduler::Start(); // Waiter2 stops it when request completes.
//    STIF_ASSERT_EQUALS( iWaiter2->iStatus.Int(), KErrNotReady );
    CActiveScheduler::Start(); // Waiter stops it when request completes.
 //   STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestCancelLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestCancelLoadCtrL()
    {
    iTestObj2->Load(
        KDummyImageFile, 1000, ETrue, 1,
        KDummyToneFile, 4, 0, 1,
        iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj2->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestStartCtrL()
    {
    iTestObj2->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestCancelStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestCancelStartCtrL()
    {
    /*
    [Test]
    title TestCancelStartCtr
    create TSanimctrl CSAnimStartupCtrl
    CSAnimStartupCtrl SetupObjLoadedCtr
    CSAnimStartupCtrl TestCancelStartCtr
    CSAnimStartupCtrl TeardownCtr
    delete CSAnimStartupCtrl
    [Endtest] 
     
    iTestObj2->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj2->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // if ( iWaiter->iStatus.Int() == KErrCancel )
    * 
    */
      return KErrNone;
      
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestStartNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestStartNoMemoryCtrL()
    {
    iTestObj2->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone ); // Mock plug-in does not reserve memory in Load
    return KErrNone;
    }



// ---------------------------------------------------------------------------
// CSAnimCtrlTestSuite::TestStartWhenActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestStartWhenActiveCtrL()
    {
    iTestObj2->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj2->Start( iWaiter2->iStatus );
    iWaiter2->Activate();
    CActiveScheduler::Start(); // Waiter2 stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter2->iStatus.Int(), KErrNotReady );
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


//Praveen-2

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupParamsL
//
// ---------------------------------------------------------------------------
//
void CTSanimctrl::SetupParamsL(
    const TDesC& aImageFileName,
    const TDesC& aToneFileName,
    const TInt aFrameDelay,
    const TInt aVolume,
    const TInt /*aVolumeRamp*/ )
    {
    CRepository* repository = CRepository::NewLC( TUid::Uid( KTestRepository ) );
    User::LeaveIfError( repository->Set( iParams.iAnimationPathKey, aImageFileName ) );
    User::LeaveIfError( repository->Set( iParams.iTonePathKey, aToneFileName ) );
    User::LeaveIfError( repository->Set( iParams.iFrameDelayKey, aFrameDelay ) );
    User::LeaveIfError( repository->Set( iParams.iVolumeKey, aVolume ) );
    CleanupStack::PopAndDestroy( repository );
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupProfileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupProfileL( const TInt aRingingType, const TInt aRingingVolume )
    {
    CRepository* repository = CRepository::NewLC( KCRUidProfileEngine );
    User::LeaveIfError( repository->Set( KProEngActiveRingingType, aRingingType ) );
    User::LeaveIfError( repository->Set( KProEngActiveRingingVolume, aRingingVolume ) );
    CleanupStack::PopAndDestroy( repository );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::InitObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::InitObjL()
    {
  //  iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    iTestObj->Load (iParams,ETrue,0,iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }



// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupEmptyL()
    {
    SetupParamsL( KNullDesC, KNullDesC, 0, 0, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
    iTestObj = NULL;
    iWaiter = CTestWaiter::NewL();
    iWaiter2 = CTestWaiter::NewL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupEmptyValuesL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupEmptyValuesL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KNullDesC, 0, 0, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
    
   // iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupUnknownFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupUnknownFileL()
    {
    SetupEmptyL();
    SetupParamsL( KUnknownFile, KNullDesC, 0, 0, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
    //iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupProtectedFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupProtectedFileL()
    {
    SetupEmptyL();
    SetupParamsL( KProtectedFile, KNullDesC, 0, 0, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
  //  iView = CSAnimMockView::NewL();
   // CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingImageFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingImageFileL()
    {
    SetupEmptyL();
    SetupParamsL( KDummyImageFile, KNullDesC, 0, 0, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
   // iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingToneFileSilentL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingToneFileSilentL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KDummyToneFile, 0, 2, 0 );
    SetupProfileL( EProfileRingingTypeSilent, 4 );
   // iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingToneFileVeryLowVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingToneFileVeryLowVolumeL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KDummyToneFile, 0, 2, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 0 );
 //   iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingToneFileLowVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingToneFileLowVolumeL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KDummyToneFile, 0, 2, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 1 );
  //  iView = CSAnimMockView::NewL();
  //  CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingToneFileNormalVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingToneFileNormalVolumeL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KDummyToneFile, 0, 2, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
  //  iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingToneFileTooLowVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingToneFileTooLowVolumeL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KDummyToneFile, 0, -1, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
   // iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupExistingToneFileTooHighVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupExistingToneFileTooHighVolumeL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KDummyToneFile, 0, 11, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
   // iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupUnknownToneFileNormalVolumeL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupUnknownToneFileNormalVolumeL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KUnknownFile, 0, 2, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
   // iView = CSAnimMockView::NewL();
   // CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupObjL()
    {
    SetupEmptyL();
    SetupParamsL( KDummyImageFile, KDummyToneFile, 0, 4, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
    //iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    iView.SetComponent( *iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupEmptyShowL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupEmptyShowL()
    {
    SetupEmptyL();
    SetupParamsL( KNullDesC, KNullDesC, 0, 0, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
    InitObjL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::SetupShowL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::SetupShowL()
    {
    SetupEmptyL();
    SetupParamsL( KDummyImageFile, KDummyToneFile, 0, 4, 0 );
    SetupProfileL( EProfileRingingTypeRinging, 4 );
    InitObjL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::Teardown()
    {
    delete iWaiter;
    iWaiter = NULL;
    delete iWaiter2;
    iWaiter2 = NULL;
    delete iTestObj;
    iTestObj = NULL;

    REComSession::FinalClose();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestConstructorL()
    {
     iView.SetComponent( *iTestObj );
   // iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj =CSAnimStartupCtrl::NewL( iView.Rect(), iView );
    //STIF_ASSERT_EQUALS( iTestObj,1 );
    delete iTestObj;
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestConstructorNoMemoryL()
    {
    //iView = CSAnimMockView::NewL();
    //CSAnimMockView& view = *iView;
    iTestObj = CSAnimStartupCtrl::NewL( iView.Rect(), iView );
   // STIF_ASSERT_EQUALS( !iTestObj,0 );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestLoadNoContentL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadNoContentL()
    {
    iTestObj->Load( iParams, EFalse, 0, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
   // STIF_ASSERT_EQUALS( iTestObj->HasContent(), EFalse );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestLoadHasContentL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadHasContentL()
    {
    iTestObj->Load( iParams, ETrue, 1, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
   // STIF_ASSERT_EQUALS( iTestObj->HasContent(), ETrue );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestLoadNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadNoMemoryL()
    {
    iTestObj->Load( iParams, EFalse, 1, iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNoMemory );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestCancelLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestCancelLoadL()
    {
    iTestObj->Load( iParams, ETrue, 0, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestLoadWhenActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestLoadWhenActiveL()
    {
    iTestObj->Load( iParams, ETrue, 0, iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Load( iParams, ETrue, 0, iWaiter2->iStatus );
    iWaiter2->Activate();
    CActiveScheduler::Start(); // Waiter2 stops it when request completes.
  //  STIF_ASSERT_EQUALS( iWaiter2->iStatus.Int(), KErrNotReady );
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestStartL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestCancelStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestCancelStartL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrCancel );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimStartupCtrlTestSuite::TestStartWhenActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimctrl::TestStartWhenActiveL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Start( iWaiter2->iStatus );
    iWaiter2->Activate();
    CActiveScheduler::Start(); // Waiter2 stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter2->iStatus.Int(), KErrNotReady );
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // STIF_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }




// -----------------------------------------------------------------------------
// CTSanimctrl::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTSanimctrl::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTSanimctrl, "TSanimctrl" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTSanimctrl, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTSanimctrl, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CTSanimctrl::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CTSanimctrl::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove

// End of File
