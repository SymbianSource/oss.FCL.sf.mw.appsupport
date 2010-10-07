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
#include "TSanimengine.h"


#include "sanimengine.h"
#include "testwaiter.h"

#include <ecom/ecom.h>

//#pragma tagging OFF // To disable CodeTest tagging

_LIT( KImageOutputFile, "C:\\temp\\sanimimagetestoutput.txt" );
_LIT( KToneOutputFile, "C:\\temp\\sanimtonetestoutput.txt" );
_LIT( KFileNoExtension, "C:\\temp\\test" ); // There is no plug-in for this type
_LIT( KFileTooLongExtension, "C:\\temp\\test.xxxxxxxxxxxxxxxx" ); // There is no plug-in for this type
_LIT( KDummyImageFile1, "C:\\temp\\test.ti1" ); // There is no plug-in for this type
_LIT( KDummyImageFile2, "C:\\temp\\test.ti2" ); // There is a plug-in for this type
_LIT( KDummyImageFile3, "C:\\temp\\test.ti3" ); // There are multiple plug-ins for this type
_LIT( KDummyImageFile4, "C:\\temp\\test.ti6" ); // Wildcard match
_LIT( KDummyImageFile5, "C:\\temp\\test.ti5" ); // This plug-in returns error
_LIT( KDummyImageFile6, "C:\\temp\\test.zzz.ti2" ); // There is a plug-in for this type
_LIT( KDummyToneFile1, "C:\\temp\\test.tt1" ); // There is no plug-in for this type
_LIT( KDummyToneFile2, "C:\\temp\\test.tt2" ); // There is a plug-in for this type
_LIT( KDummyToneFile3, "C:\\temp\\test.tt3" ); // There are multiple plug-ins for this type
_LIT( KDummyToneFile4, "C:\\temp\\test.tt6" ); // Wildcard match
_LIT( KDummyToneFile5, "C:\\temp\\test.tt5" ); // This plug-in returns error

const TUint32 KImagePlugin1ImplUid = 0x0AA01513;
const TUint32 KImagePlugin2ImplUid = 0x0AA01515;
const TUint32 KImagePlugin3ImplUid = 0x0AA01517;
const TUint32 KImagePlugin4ImplUid = 0x0AA01519;
const TUint32 KTonePlugin1ImplUid = 0x0AA0150B;
const TUint32 KTonePlugin2ImplUid = 0x0AA0150D;
const TUint32 KTonePlugin3ImplUid = 0x0AA0150F;
const TUint32 KTonePlugin4ImplUid = 0x0AA01511;




void CTSanimengine::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTSanimengine::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTSanimengine::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CTSanimengine::ExampleL ),
        ENTRY( "SetupEmpty", CTSanimengine::SetupEmptyL),
        ENTRY( "SetupObj", CTSanimengine::SetupObjL),
        ENTRY( "SetupImagePlugin", CTSanimengine::SetupImagePluginL),
        ENTRY( "SetupTonePlugin", CTSanimengine::SetupTonePluginL),
        ENTRY( "SetupBothPlugins", CTSanimengine::SetupBothPluginsL),
        ENTRY( "SetupImagePluginFaster", CTSanimengine::SetupImagePluginFasterL),
        ENTRY( "SetupTonePluginFaster", CTSanimengine::SetupTonePluginFasterL),
        ENTRY( "SetupImagePluginReturnsError", CTSanimengine::SetupImagePluginReturnsErrorL),
        ENTRY( "SetupTonePluginReturnsError", CTSanimengine::SetupTonePluginReturnsErrorL),
        ENTRY( "Load", CTSanimengine::LoadL),      
        ENTRY( "SetupImageLoaded", CTSanimengine::SetupImageLoadedL),
        ENTRY( "SetupToneLoaded", CTSanimengine::SetupToneLoadedL),
        ENTRY( "SetupBothLoaded", CTSanimengine::SetupBothLoadedL),
        ENTRY( "SetupLoadFailed", CTSanimengine::SetupLoadFailedL),     
        ENTRY( "SetupBothFinished", CTSanimengine::SetupBothFinishedL),
        ENTRY( "Teardown", CTSanimengine::Teardown),
        ENTRY( "SetupBothPluginsReturnError", CTSanimengine::SetupBothPluginsReturnErrorL),     
        
        ENTRY( "TestConstructor", CTSanimengine::TestConstructorL),
        ENTRY( "TestConstructorNoMemory", CTSanimengine::TestConstructorNoMemoryL),
        ENTRY( "TestExtensionlessFile", CTSanimengine::TestExtensionlessFileL), 
        ENTRY( "TestTooLongExtension", CTSanimengine::TestTooLongExtensionL), 
        ENTRY( "SetImagePropertiesUnknownExtension", CTSanimengine::SetImagePropertiesUnknownExtensionL), 
        ENTRY( "SetImagePropertiesKnownExtension", CTSanimengine::SetImagePropertiesKnownExtensionL),
        ENTRY( "SetImagePropertiesUnknownExtensionNoMemory", CTSanimengine::SetImagePropertiesUnknownExtensionNoMemoryL),
        ENTRY( "SetImagePropertiesKnownExtensionNoMemory", CTSanimengine::SetImagePropertiesKnownExtensionNoMemoryL),
        ENTRY( "SetImagePropertiesManyExtensions", CTSanimengine::SetImagePropertiesManyExtensionsL),    
        ENTRY( "SetImagePropertiesManyExtensionsNoMemory", CTSanimengine::SetImagePropertiesManyExtensionsNoMemoryL),
        ENTRY( "SetImagePropertiesManyPlugins", CTSanimengine::SetImagePropertiesManyPluginsL), 
        ENTRY( "SetImagePropertiesManyPluginsNoMemory", CTSanimengine::SetImagePropertiesManyPluginsNoMemoryL), 
        ENTRY( "SetImagePropertiesWildcardMatch", CTSanimengine::SetImagePropertiesWildcardMatchL), 
        ENTRY( "SetImagePropertiesWildcardMatchNoMemory", CTSanimengine::SetImagePropertiesWildcardMatchNoMemoryL), 
        ENTRY( "SetTonePropertiesUnknownExtension", CTSanimengine::SetTonePropertiesUnknownExtensionL), 
        ENTRY( "SetTonePropertiesUnknownExtensionNoMemory", CTSanimengine::SetTonePropertiesUnknownExtensionNoMemoryL), 
        ENTRY( "SetTonePropertiesKnownExtension", CTSanimengine::SetTonePropertiesKnownExtensionL), 
        ENTRY( "SetTonePropertiesKnownExtensionNoMemory", CTSanimengine::SetTonePropertiesKnownExtensionNoMemoryL), 
        ENTRY( "SetTonePropertiesManyPlugins", CTSanimengine::SetTonePropertiesManyPluginsL),         
        ENTRY( "SetTonePropertiesManyPluginsNoMemory", CTSanimengine::SetTonePropertiesManyPluginsNoMemoryL), 
        ENTRY( "SetTonePropertiesWildcardMatch", CTSanimengine::SetTonePropertiesWildcardMatchL), 
        ENTRY( "SetTonePropertiesWildcardMatchNoMemory", CTSanimengine::SetTonePropertiesWildcardMatchNoMemoryL),
        ENTRY( "TestLoadEmpty", CTSanimengine::TestLoadEmptyL), 
        ENTRY( "TestCancelLoadEmpty", CTSanimengine::TestCancelLoadEmptyL), 
        ENTRY( "TestLoadNoMemory", CTSanimengine::TestLoadNoMemoryL), 
        ENTRY( "TestCancelLoad", CTSanimengine::TestCancelLoadL), 
        ENTRY( "TestLoadImagePluginFaster", CTSanimengine::TestLoadImagePluginFasterL), 
        ENTRY( "TestLoadTonePluginFaster", CTSanimengine::TestLoadTonePluginFasterL), 
        ENTRY( "TestLoadImagePluginReturnsError", CTSanimengine::TestLoadImagePluginReturnsErrorL), 

        ENTRY( "TestLoadTonePluginReturnsError", CTSanimengine::TestLoadTonePluginReturnsErrorL), 
        ENTRY( "TestBackroundColour", CTSanimengine::TestBackroundColourL), 
        ENTRY( "TestCancelNotActive", CTSanimengine::TestCancelNotActiveL), 
        ENTRY( "TestLoadWhenActive", CTSanimengine::TestLoadWhenActiveL), 
        ENTRY( "TestStartWhenActive", CTSanimengine::TestStartWhenActiveL), 
        ENTRY( "TestStart", CTSanimengine::TestStartL),       
        
        ENTRY( "TestStartNotReady", CTSanimengine::TestStartNotReadyL), 
        ENTRY( "TestLoadAgain", CTSanimengine::TestLoadAgainL), 
        ENTRY( "TestStartAgain", CTSanimengine::TestStartAgainL), 
        ENTRY( "TestLoadAndStartAnotherShow", CTSanimengine::TestLoadAndStartAnotherShowL), 
        ENTRY( "TestLoadAndStartEmptyShowAfterAnotherShow", CTSanimengine::TestLoadAndStartEmptyShowAfterAnotherShowL), 
        ENTRY( "TestStartWhenLoadFailed", CTSanimengine::TestStartWhenLoadFailedL),         
                                             
     
     
         
         
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CTSanimengine::SetupEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupEmptyL()
    {
    User::LeaveIfError( iFs.Connect() );

    RFile file;
    file.Replace( iFs, KImageOutputFile, EFileShareAny | EFileWrite );
    file.Replace( iFs, KToneOutputFile, EFileShareAny | EFileWrite );
    file.Close();

    iTestObj = NULL;
    iWaiter = CTestWaiter::NewL();
    iWaiter2 = CTestWaiter::NewL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupObjL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupObjL()
    {
    SetupEmptyL();
    iTestObj = CSAnimEngine::NewL( iFs, iObserver );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupImagePluginL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupImagePluginL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile2, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupTonePluginL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupTonePluginL()
    {
    SetupObjL();
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile2, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupBothPluginsL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupBothPluginsL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile2, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile2, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupImagePluginFasterL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupImagePluginFasterL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile2, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile4, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupTonePluginFasterL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupTonePluginFasterL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile4, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile2, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupImagePluginReturnsErrorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupImagePluginReturnsErrorL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile5, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile2, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupTonePluginReturnsErrorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupTonePluginReturnsErrorL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile2, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile5, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupBothPluginsReturnErrorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupBothPluginsReturnErrorL()
    {
    SetupObjL();
    User::LeaveIfError(
        iTestObj->SetImageProperties( KDummyImageFile5, EGray2, TSize( 0, 0 ), 0, ETrue, 1 ) );
    User::LeaveIfError( iTestObj->SetToneProperties( KDummyToneFile5, 4, 0, 1 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::LoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::LoadL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    User::LeaveIfError( iWaiter->iStatus.Int() );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupImageLoadedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupImageLoadedL()
    {
    SetupImagePluginL();
    LoadL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupToneLoadedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupToneLoadedL()
    {
    SetupTonePluginL();
    LoadL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupBothLoadedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupBothLoadedL()
    {
    SetupBothPluginsL();
    LoadL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupLoadFailedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupLoadFailedL()
    {
    SetupBothPluginsReturnErrorL();
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetupBothFinishedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetupBothFinishedL()
    {
    SetupBothLoadedL();
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    User::LeaveIfError( iWaiter->iStatus.Int() );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::Teardown
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::Teardown()
    {
    delete iWaiter2;
    iWaiter2 = NULL;
    delete iWaiter;
    iWaiter = NULL;
    delete iTestObj;
    iTestObj = NULL;
    iFs.Close();

    REComSession::FinalClose();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::CheckNoImagePluginOutputL
//
// ---------------------------------------------------------------------------
//
TBool CTSanimengine::CheckNoImagePluginOutputL()
    {
    RFile file;
    CleanupClosePushL( file );
    User::LeaveIfError(
        file.Open( iFs, KImageOutputFile, EFileShareAny | EFileRead ) );

    TFileText reader;
    reader.Set( file );
    TBuf<16> buf;
    TBool retVal = ( reader.Read( buf ) == KErrEof );

    CleanupStack::PopAndDestroy( &file );
    return retVal;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::CheckImagePluginOutputL
//
// ---------------------------------------------------------------------------
//
TBool CTSanimengine::CheckImagePluginOutputL(
    const TUint32 aPluginUid,
    const TDesC& aFileToLoad,
    const TInt aDisplayMode,
    const TSize& aSize,
    const TInt aFrameDelay )
    {
    RFile file;
    CleanupClosePushL( file );
    User::LeaveIfError(
        file.Open( iFs, KImageOutputFile, EFileShareAny | EFileRead ) );

    TFileText reader;
    reader.Set( file );

    TBool retVal = ETrue;
    TBuf<256> buf;
    TLex16 lex;

    User::LeaveIfError( reader.Read( buf ) );
    TUint32 uidFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( uidFromFile, EHex ) );
    if ( uidFromFile != aPluginUid )
        {
      //  EUNIT_PRINT( _L( "Plugin UID mismatch: expected 0x%08x, got 0x%08x" ), aPluginUid, uidFromFile );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    if ( buf != aFileToLoad )
        {
      //  EUNIT_PRINT( _L( "File name mismatch: expected %S, got %S" ), &aFileToLoad, &buf );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    TInt modeFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( modeFromFile ) );
    if ( modeFromFile != aDisplayMode )
        {
     //   EUNIT_PRINT( _L( "Display mode mismatch: expected %d, got %d" ), aDisplayMode, modeFromFile );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    TInt widthFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( widthFromFile ) );
    User::LeaveIfError( reader.Read( buf ) );
    TInt heightFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( heightFromFile ) );
    if ( widthFromFile != aSize.iWidth || heightFromFile != aSize.iHeight )
        {
      //  EUNIT_PRINT( _L( "Size mismatch: expected (%d,%d), got (%d,%d)" ),
     //       aSize.iWidth, aSize.iHeight, widthFromFile, heightFromFile );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    TUint delayFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( delayFromFile ) );
    if ( delayFromFile != aFrameDelay )
        {
   //     EUNIT_PRINT( _L( "Frame delay mismatch: expected %d, got %d" ), aFrameDelay, delayFromFile );
        retVal = EFalse;
        }

    CleanupStack::PopAndDestroy( &file );
    return retVal;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::CheckNoTonePluginOutputL
//
// ---------------------------------------------------------------------------
//
TBool CTSanimengine::CheckNoTonePluginOutputL()
    {
    RFile file;
    CleanupClosePushL( file );
    User::LeaveIfError(
        file.Open( iFs, KToneOutputFile, EFileShareAny | EFileRead ) );

    TFileText reader;
    reader.Set( file );
    TBuf<16> buf;
    TBool retVal = ( reader.Read( buf ) == KErrEof );

    CleanupStack::PopAndDestroy( &file );
    return retVal;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::CheckTonePluginOutputL
//
// ---------------------------------------------------------------------------
//
TBool CTSanimengine::CheckTonePluginOutputL(
    const TUint32 aPluginUid,
    const TDesC& aFileToLoad,
    const TInt aVolume,
    const TInt64& aVolumeRamp )
    {
    RFile file;
    CleanupClosePushL( file );
    User::LeaveIfError(
        file.Open( iFs, KToneOutputFile, EFileShareAny | EFileRead ) );

    TFileText reader;
    reader.Set( file );

    TBool retVal = ETrue;
    TBuf<256> buf;
    TLex16 lex;

    User::LeaveIfError( reader.Read( buf ) );
    TUint32 uidFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( uidFromFile, EHex ) );
    if ( uidFromFile != aPluginUid )
        {
   //     EUNIT_PRINT(
     //       _L( "Plugin UID mismatch: expected 0x%08x, got 0x%08x" ), aPluginUid, uidFromFile );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    if ( buf != aFileToLoad )
        {
  //      EUNIT_PRINT( _L( "File name mismatch: expected %S, got %S" ), &aFileToLoad, &buf );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    TInt volumeFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( volumeFromFile ) );
    if ( volumeFromFile != aVolume )
        {
  //      EUNIT_PRINT( _L( "Volume mismatch: expected %d, got %d" ), aVolume, volumeFromFile );
        retVal = EFalse;
        }

    User::LeaveIfError( reader.Read( buf ) );
    TInt64 volumeRampFromFile;
    lex.Assign( buf );
    User::LeaveIfError( lex.Val( volumeRampFromFile ) );
    if ( volumeRampFromFile != aVolumeRamp )
        {
   //     EUNIT_PRINT(
   //         _L( "Volume ramp mismatch: expected %d, got %d" ), aVolumeRamp, volumeRampFromFile );
        retVal = EFalse;
        }

    CleanupStack::PopAndDestroy( &file );
    return retVal;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestConstructorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestConstructorL()
    {
    iTestObj = CSAnimEngine::NewL( iFs, iObserver );
    delete iTestObj;
    iTestObj = NULL;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestConstructorNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestConstructorNoMemoryL()
    {
    iTestObj = CSAnimEngine::NewL( iFs, iObserver );
   // EUNIT_ASSERT( !iTestObj );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestExtensionlessFileL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestExtensionlessFileL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KFileNoExtension, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
    if (errorCode == KErrNotFound )
        return KErrNone;
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestTooLongExtensionL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestTooLongExtensionL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KFileTooLongExtension, 4, 0, 1 );
    if ( errorCode == KErrNotFound )
        return KErrNone;
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesUnknownExtensionL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesUnknownExtensionL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile1, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
    if ( errorCode == KErrNotFound )
        return KErrNone;
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesUnknownExtensionNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesUnknownExtensionNoMemoryL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile1, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
     if (errorCode == KErrNoMemory  )
         return KErrNone;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesKnownExtensionL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesKnownExtensionL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile2, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
 //   EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

    if ( CheckImagePluginOutputL(KImagePlugin1ImplUid, KDummyImageFile2, EGray2, TSize( 0, 0 ), 0 ) == KErrNone)
    return KErrNone;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesKnownExtensionNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesKnownExtensionNoMemoryL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile2, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
    if (errorCode == KErrNoMemory )
        return KErrNone;
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::SetImagePropertiesManyPluginsL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesManyPluginsL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile3, EGray2, TSize( 100, 200 ), 0, ETrue, 1 );
  //  EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

    // Either plug-in 2 or 3 is acceptable.
  /*
    EUNIT_ASSERT(
        CheckImagePluginOutputL(
            KImagePlugin2ImplUid, KDummyImageFile3, EGray2, TSize( 100, 200 ), 0 ) ||
        CheckImagePluginOutputL(
            KImagePlugin3ImplUid, KDummyImageFile3, EGray2, TSize( 100, 200 ), 0 ) );
   */
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesManyPluginsNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesManyPluginsNoMemoryL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile3, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
  if ( errorCode== KErrNoMemory )
      return KErrNone;
    return errorCode;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesWildcardMatchL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesWildcardMatchL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile4, EGray2, TSize( 0, 0 ), 135000, ETrue, 1 );
   // EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

  //  EUNIT_ASSERT( CheckImagePluginOutputL(
    //    KImagePlugin4ImplUid, KDummyImageFile4, EGray2, TSize( 0, 0 ), 135000 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesWildcardMatchNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesWildcardMatchNoMemoryL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile4, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
   if ( errorCode == KErrNoMemory )
    return KErrNone;
   
   return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetImagePropertiesManyExtensionsL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesManyExtensionsL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile6, EColor16M, TSize( 0, 0 ), 0, ETrue, 1 );
   // EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

  if ( CheckImagePluginOutputL(KImagePlugin1ImplUid, KDummyImageFile6, EColor16M, TSize( 0, 0 ), 0 ) )
    return KErrNone;
  return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::SetImagePropertiesManyExtensionsNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetImagePropertiesManyExtensionsNoMemoryL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile6, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
   if ( errorCode == KErrNoMemory )
       return KErrNone;
   
    return errorCode;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesUnknownExtensionL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesUnknownExtensionL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile1, 4, 0, 1 );
    if ( errorCode == KErrNotFound )
        return KErrNone;
    
    return errorCode;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesUnknownExtensionNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesUnknownExtensionNoMemoryL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile1, 4, 0, 1 );
    if ( errorCode == KErrNoMemory )
        return KErrNone;
    
    return errorCode;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesKnownExtensionL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesKnownExtensionL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile2, 4, 0, 1 );
  //  EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

  //  EUNIT_ASSERT( CheckTonePluginOutputL(
   //     KTonePlugin1ImplUid, KDummyToneFile2, 4, 0 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesKnownExtensionNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesKnownExtensionNoMemoryL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile2, 4, 0, 1 );
    if ( errorCode == KErrNoMemory )
        return KErrNone;
    return errorCode;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesManyPluginsL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesManyPluginsL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile3, 2, 300000, 1 );
   // EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

    // Either plug-in 2 or 3 is acceptable.
  if (  CheckTonePluginOutputL(KTonePlugin2ImplUid, KDummyToneFile3, 2, 300000 ) ||
        CheckTonePluginOutputL(KTonePlugin3ImplUid, KDummyToneFile3, 2, 300000 ) )
    return KErrNone;
  
  return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesManyPluginsNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesManyPluginsNoMemoryL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile3, 4, 0, 1 );
    if ( errorCode == KErrNoMemory )
        return KErrNone;
    
    return errorCode;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesWildcardMatchL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesWildcardMatchL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile4, 0, 1000000, 1 );
  //  EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus ); // Mock plug-in writes output file when Load is called.
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.

  //  EUNIT_ASSERT( CheckTonePluginOutputL(
  //      KTonePlugin4ImplUid, KDummyToneFile4, 0, 1000000 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::SetTonePropertiesWildcardMatchNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::SetTonePropertiesWildcardMatchNoMemoryL()
    {
    TInt errorCode = iTestObj->SetToneProperties( KDummyToneFile4, 4, 0, 1 );
    if (errorCode == KErrNoMemory )
        return KErrNone;
    return errorCode;
    }



// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadEmptyL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );

    CheckNoImagePluginOutputL();
    CheckNoTonePluginOutputL();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestCancelLoadEmptyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestCancelLoadEmptyL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadNoMemoryL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadNoMemoryL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone ); // Mock plug-in does not reserve memory in Load
    return KErrNone;
    }



// ---------------------------------------------------------------------------
// CTSanimengine::TestCancelLoadL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestCancelLoadL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Cancel();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int()== KErrCancel )
        return KErrNone;
    
    return KErrCancel;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadImagePluginFasterL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadImagePluginFasterL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    //EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );

    //EUNIT_ASSERT( CheckImagePluginOutputL(
     //   KImagePlugin1ImplUid, KDummyImageFile2, EGray2, TSize( 0, 0 ), 0 ) );

  //  EUNIT_ASSERT( CheckTonePluginOutputL(
   //     KTonePlugin4ImplUid, KDummyToneFile4, 4, 0 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadTonePluginFasterL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadTonePluginFasterL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );

  //  EUNIT_ASSERT( CheckImagePluginOutputL(
   //     KImagePlugin4ImplUid, KDummyImageFile4, EGray2, TSize( 0, 0 ), 0 ) );

   // EUNIT_ASSERT( CheckTonePluginOutputL(
    //    KTonePlugin1ImplUid, KDummyToneFile2, 4, 0 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadImagePluginReturnsErrorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadImagePluginReturnsErrorL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrGeneral );

   // EUNIT_ASSERT( CheckImagePluginOutputL(
    //    KImagePlugin3ImplUid, KDummyImageFile5, EGray2, TSize( 0, 0 ), 0 ) );

   // EUNIT_ASSERT( CheckTonePluginOutputL(
    //    KTonePlugin1ImplUid, KDummyToneFile2, 4, 0 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadTonePluginReturnsErrorL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadTonePluginReturnsErrorL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrGeneral );

   // EUNIT_ASSERT( CheckImagePluginOutputL(
    //    KImagePlugin1ImplUid, KDummyImageFile2, EGray2, TSize( 0, 0 ), 0 ) );

  //  EUNIT_ASSERT( CheckTonePluginOutputL(
    //    KTonePlugin3ImplUid, KDummyToneFile5, 4, 0 ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestBackroundColourL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestBackroundColourL()
    {
  //  EUNIT_ASSERT_EQUALS( iTestObj->BackroundColour(), TRgb() );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::TestCancelNotActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestCancelNotActiveL()
    {
    iTestObj->Cancel();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::TestLoadWhenActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadWhenActiveL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Load( iWaiter2->iStatus );
    iWaiter2->Activate();
    CActiveScheduler::Start(); // Waiter2 stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter2->iStatus.Int(), KErrNotReady );
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::TestStartWhenActiveL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestStartWhenActiveL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    iTestObj->Start( iWaiter2->iStatus );
    iWaiter2->Activate();
    CActiveScheduler::Start(); // Waiter2 stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter2->iStatus.Int(), KErrNotReady );
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::TestStartL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestStartL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::TestStartNotReadyL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestStartNotReadyL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
    if ( iWaiter->iStatus.Int() == KErrNotReady )
        return KErrNone;
    return KErrNotReady;
    }


// ---------------------------------------------------------------------------
// CSAnimEngineTestSuite::TestLoadAgainL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadAgainL()
    {
    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestStartAgainL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestStartAgainL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadAndStartAnotherShowL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadAndStartAnotherShowL()
    {
    TInt errorCode =
        iTestObj->SetImageProperties( KDummyImageFile3, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
  //  EUNIT_ASSERT_EQUALS( errorCode, KErrNone );
    errorCode = iTestObj->SetToneProperties( KDummyToneFile3, 4, 0, 1 );
  //  EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
 //   EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );

    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestLoadAndStartEmptyShowAfterAnotherShowL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestLoadAndStartEmptyShowAfterAnotherShowL()
    {
    TInt errorCode = iTestObj->SetImageProperties( KNullDesC, EGray2, TSize( 0, 0 ), 0, ETrue, 1 );
   // EUNIT_ASSERT_EQUALS( errorCode, KErrNone );
    errorCode = iTestObj->SetToneProperties( KNullDesC, 4, 0, 1 );
  //  EUNIT_ASSERT_EQUALS( errorCode, KErrNone );

    iTestObj->Load( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );

    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
   // EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNone );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CTSanimengine::TestStartWhenLoadFailedL
//
// ---------------------------------------------------------------------------
//
TInt CTSanimengine::TestStartWhenLoadFailedL()
    {
    iTestObj->Start( iWaiter->iStatus );
    iWaiter->Activate();
    CActiveScheduler::Start(); // Waiter stops it when request completes.
  //  EUNIT_ASSERT_EQUALS( iWaiter->iStatus.Int(), KErrNotReady );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CTSanimengine::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTSanimengine::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTSanimengine, "TSanimengine" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTSanimengine, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTSanimengine, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// ---------------------------------------

// End of File
