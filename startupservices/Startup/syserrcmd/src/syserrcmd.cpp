/*
* Copyright (c) 2009-10 Nokia Corporation and/or its subsidiary(-ies).
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
* Implementation of CSysErrCmd class.
*
*/

#include "syserrcmd.h"
#include "trace.h"

#include <AknGlobalNote.h>
#include <aknSDData.h>
#include <data_caging_path_literals.hrh>
#include <featmgr.h>
#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>
#include <StringLoader.h>
#include <startup.rsg>
#include <stringresourcereader.h>


_LIT( KResourceFileName, "Z:startup.rsc" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysErrorPlugin::GetResourceFileNameLC
//
// ---------------------------------------------------------------------------
//
static TFileName* GetResourceFileNameLC()
    {
    FUNC_LOG;

    // TParse uses a lot of stack space, so allocate it from heap.
    TParse* parse = new ( ELeave ) TParse; 
    CleanupDeletePushL( parse );
    TInt errorCode = parse->Set( KResourceFileName, 
                                 &KDC_APP_RESOURCE_DIR, 
                                 NULL );
    ERROR( errorCode, "parse::Set() failed with error code %d" );
    User::LeaveIfError( errorCode );

    TFileName* filename = new ( ELeave ) TFileName( parse->FullName() );

    CleanupStack::PopAndDestroy( parse );
    CleanupDeletePushL( filename );

    INFO_1( "Resource file name: %S", filename );

    return filename;
    }

// ---------------------------------------------------------------------------
// CSysErrorPlugin::GetFatalErrorStringLC
//
// ---------------------------------------------------------------------------
//
static TBool IsCoverUiSupported()
    {
    FUNC_LOG;

    // If this fails, default to false.
    TRAPD( errorCode, FeatureManager::InitializeLibL() ); 
    ERROR( errorCode, "Failed to initialize FeatureManager" );

    TBool retVal = EFalse;
    if ( errorCode == KErrNone &&
         FeatureManager::FeatureSupported( KFeatureIdCoverDisplay ) )
        {
        retVal = ETrue;
        }

    FeatureManager::UnInitializeLib();

    INFO_1( "CoverUiSupported = %d", retVal );
    return retVal;
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysErrCmd::NewL
// ---------------------------------------------------------------------------
//
CSysErrCmd* CSysErrCmd::NewL()
	{
    FUNC_LOG;
	return new ( ELeave ) CSysErrCmd();
	}


// ---------------------------------------------------------------------------
// CSysErrCmd::~CSysErrCmd
// ---------------------------------------------------------------------------
//
CSysErrCmd::~CSysErrCmd()
    {
    FUNC_LOG;
    
    delete iNote;
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Initialize
// ---------------------------------------------------------------------------
//
TInt CSysErrCmd::Initialize( CSsmCustomCommandEnv* aCmdEnv )
    {
    FUNC_LOG;
    ASSERT_TRACE( aCmdEnv );
    iEnv = aCmdEnv;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Execute
// ---------------------------------------------------------------------------
//
void CSysErrCmd::Execute( const TDesC8& /*aParams*/, TRequestStatus& aRequest )
    {
    FUNC_LOG;
    
    INFO_1( "[0x%08x] Execute", this );
    TInt err( KErrNone );
    TRAP( err, DoExecuteL( aRequest ) );
    ERROR( err, "DoExecuteL failed." );
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::ExecuteCancel
// ---------------------------------------------------------------------------
//
void CSysErrCmd::ExecuteCancel()
    {
    FUNC_LOG;

    if ( iNote )
        {
        TInt errorCode( KErrNone );
        TRAP( errorCode, iNote->CancelNoteL( iNoteId ) );
        ERROR( errorCode, "Failed to cancel global note" );
        }

    delete iNote; // Note must be deleted here! Otherwise it doesn't complete
    iNote = NULL; // request with KErrCancel and Cancel() gets stuck.
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Close
// ---------------------------------------------------------------------------
//
void CSysErrCmd::Close()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Release
// ---------------------------------------------------------------------------
//
void CSysErrCmd::Release()
    {
    FUNC_LOG;

	delete this;
    }

// ---------------------------------------------------------------------------
// CSysErrCmd::DoExecuteL
// ---------------------------------------------------------------------------
//
void CSysErrCmd::DoExecuteL( TRequestStatus& aRequest )
    {
    delete iNote;
    iNote = NULL;
    iNote = CAknGlobalNote::NewL();

    if ( IsCoverUiSupported() )
        {
        CAknSDData* sdData = CAknSDData::NewL( 
                        SecondaryDisplay::KCatStartup,
                        SecondaryDisplay::ECmdShowErrorNote,
                        TPckgBuf<TInt>( SecondaryDisplay::EContactService ) );
        
        // ownership to notifier client
        iNote->SetSecondaryDisplayData( sdData ); 
        }

    TFileName* filename = GetResourceFileNameLC();
    
    RFs& fs = const_cast<RFs&>( iEnv->Rfs() );
    
    CStringResourceReader* resReader = CStringResourceReader::NewLC( *filename,
                                                                     fs );
    
    TPtrC errorStr( resReader->ReadResourceString( 
                                            R_SU_SELFTEST_FAILED_NOTE_TEXT ) );

    iNoteId = iNote->ShowNoteL( aRequest, EAknGlobalPermanentNote, errorStr );
    
    CleanupStack::PopAndDestroy( resReader );
    CleanupStack::PopAndDestroy( filename );
    
    }
