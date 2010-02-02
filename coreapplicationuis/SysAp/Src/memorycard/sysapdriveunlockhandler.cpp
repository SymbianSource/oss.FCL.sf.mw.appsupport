/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Drive unlock handler implementation
 *
*/



// INCLUDE FILES
#include <e32std.h>
#include <f32file.h>
#include <e32property.h>
#include <UikonInternalPSKeys.h>
#include "sysapdriveunlockhandler.h"
#include "sysapdrivelist.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::CSysApDriveUnlockHandler
// ---------------------------------------------------------------------------
//
CSysApDriveUnlockHandler* CSysApDriveUnlockHandler::NewL(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported )
    {
    return new ( ELeave ) CSysApDriveUnlockHandler(
        aSysApDriveList,
        aSysApAppUi,
        aMemoryCardLockSupported );
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::CSysApDriveUnlockHandler
// ---------------------------------------------------------------------------
//
CSysApDriveUnlockHandler::CSysApDriveUnlockHandler(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported ) :
    iSysApDriveList( aSysApDriveList ),
    iSysApAppUi( aSysApAppUi ),
    iMemoryCardLockSupported( aMemoryCardLockSupported )
    {
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::~CSysApDriveUnlockHandler
// ---------------------------------------------------------------------------
//
CSysApDriveUnlockHandler::~CSysApDriveUnlockHandler()
    {
    TRACES( RDebug::Print(
        _L( "CSysApDriveUnlockHandler::~CSysApDriveUnlockHandler" ) ) );

    iIgnoreQueryResult = ETrue;
    delete iQueryShowCB;
    delete iMemoryCardDialog;
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::StartUnlock
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::StartUnlock()
    {
    TBool isOngoing( IsQueryOngoing() );

    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::StartUnlock: ongoing: %d" ),
        isOngoing ) );

    if ( isOngoing )
        {
        return;
        }
    TRAPD( err, DoStartQueryIfNeededL() );
    if ( err != KErrNone )
        {
        DoStopUnlock( err );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::StopUnlock
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::StopUnlock( TInt aDrive )
    {
    TBool isOngoing( IsQueryOngoing() );

    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::StopUnlock: ongoing: %d, drive: %d" ),
        isOngoing, iDriveToUnlock ) );

    if ( !isOngoing || iDriveToUnlock != aDrive )
        {
        return;
        }
    // Stop current query and continue with other locked memory cards
    iSysApDriveList.MarkDriveUnlockQueryShown( iDriveToUnlock );
    DoStopUnlock( KErrNone );
    TRAPD( err, DoStartQueryIfNeededL() );
    if ( err != KErrNone )
        {
        DoStopUnlock( err );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::UnlockComplete
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::UnlockComplete( TInt aResult )
    {
    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::UnlockComplete: ignore: %d, drive: %d, result: %d" ),
        iIgnoreQueryResult, iDriveToUnlock, aResult ) );

    if ( iIgnoreQueryResult || iDriveToUnlock == KErrNotFound )
        {
        return;
        }

    // Handle default memory card functionality
    if ( iDriveToUnlock == iSysApDriveList.DefaultMemoryCard() )
        {
        if ( aResult == KErrNone )
            {
            RProperty::Set( KPSUidUikon, KUikMMCInserted, 1 );
            }
        else
            {
            RProperty::Set( KPSUidUikon, KUikMMCInserted, 0 );
            }
        }

    // Mark handled and start next query
    iSysApDriveList.MarkDriveUnlockQueryShown( iDriveToUnlock );
    DoStartQueryAsyncIfNeeded();
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::QueryShowCB
// ---------------------------------------------------------------------------
//
TInt CSysApDriveUnlockHandler::QueryShowCB( TAny* aPtr )
    {
    CSysApDriveUnlockHandler* self =
        static_cast< CSysApDriveUnlockHandler* >( aPtr );
    TRAPD( err, self->ShowUnlockQueryL() );
    if ( err != KErrNone )
        {
        self->DoStopUnlock( err );
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::ShowUnlockQueryL
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::ShowUnlockQueryL()
    {
    TRACES( RDebug::Print(
        _L( "CSysApMCSysApMMCUnlockObserver::ShowUnlockQueryL: drive: %d" ),
        iDriveToUnlock ) );

    if ( !iMemoryCardDialog )
        {
        CAknMemoryCardDialog* mmcDialog = CAknMemoryCardDialog::NewLC( this );
        iMemoryCardDialog = mmcDialog; // temporary variable used for hiding codescanner error 
        iMemoryCardDialog->SetSelfPointer( &iMemoryCardDialog );
        TDriveNumber drive( static_cast< TDriveNumber >( iDriveToUnlock ) );
        iMemoryCardDialog->UnlockCardLD( drive, ETrue );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::IsQueryOngoing
// ---------------------------------------------------------------------------
//
TBool CSysApDriveUnlockHandler::IsQueryOngoing() const
    {
    return ( iMemoryCardDialog || iQueryShowCB );
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::DoStartQueryIfNeededL
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::DoStartQueryIfNeededL()
    {
    iDriveToUnlock = iSysApDriveList.DriveToUnlock();
    if ( iDriveToUnlock == KErrNotFound )
        {
        return;
        }
    if ( iMemoryCardLockSupported )
        {
        ShowUnlockQueryL();
        }
    else
        {
        iSysApAppUi.ShowQueryL( ESysApMemoryCardLockedNote );
        iSysApDriveList.MarkDriveUnlockQueryShown( iDriveToUnlock );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::DoStartQueryAsyncIfNeeded
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::DoStartQueryAsyncIfNeeded()
    {
    iDriveToUnlock = iSysApDriveList.DriveToUnlock();
    if ( iDriveToUnlock == KErrNotFound )
        {
        DoStopUnlock( KErrNone );
        return;
        }
    delete iQueryShowCB;
    iQueryShowCB = NULL;
    iQueryShowCB = new CAsyncCallBack(
        TCallBack( QueryShowCB, this ), CActive::EPriorityStandard );
    if ( iQueryShowCB )
        {
        iQueryShowCB->CallBack();
        }
    else
        {
        DoStopUnlock( KErrNoMemory );
        }
    }

// ---------------------------------------------------------------------------
// CSysApDriveUnlockHandler::DoStopUnlock
// ---------------------------------------------------------------------------
//
void CSysApDriveUnlockHandler::DoStopUnlock( TInt aError )
    {
    if ( aError != KErrNone )
        {
        TRACES( RDebug::Print(
            _L( "CSysApMCSysApMMCUnlockObserver::DoStopUnlock: error: %d" ),
            aError ) );
        }
    iIgnoreQueryResult = ETrue;
    delete iMemoryCardDialog;
    iMemoryCardDialog = NULL;
    delete iQueryShowCB;
    iQueryShowCB = NULL;
    iIgnoreQueryResult = EFalse;
    }

// End of File
