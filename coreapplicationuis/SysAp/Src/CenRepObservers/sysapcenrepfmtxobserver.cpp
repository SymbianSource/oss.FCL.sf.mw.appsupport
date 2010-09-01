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
* Description:  CSysApCenRepFmTxObserver implementation.
 *
*/


// INCLUDE FILES
#include <centralrepository.h>
#include <hwrmfmtx.h>
#include <hwrmfmtxdomaincrkeys.h>
#include <hwrmfmtxdomainpskeys.h>
#include "sysapcenrepfmtxobserver.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver* CSysApCenRepFmTxObserver::NewL()
// ----------------------------------------------------------------------------

CSysApCenRepFmTxObserver* CSysApCenRepFmTxObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::NewL" ) ) );
    CSysApCenRepFmTxObserver* self = new ( ELeave ) CSysApCenRepFmTxObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver::CSysApCenRepFmTxObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApCenRepFmTxObserver::CSysApCenRepFmTxObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi ),
      iSession( NULL ),
      iFmTxPowerStateHandler( NULL ),
      iFrequency( 0 )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::CSysApCenRepFmTxObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver::~CSysApCenRepFmTxObserver()
// ----------------------------------------------------------------------------

CSysApCenRepFmTxObserver::~CSysApCenRepFmTxObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepFmTxObserver") ) );
    delete iFmTxPowerStateHandler;
    delete iSession;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApCenRepFmTxObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::ConstructL: trying CRepository::NewL( KCRUidBluetoothPowerState )") ) );
    iSession = CRepository::NewL( KCRUidFmTxCenRes );
    iFmTxPowerStateHandler = 
        CCenRepNotifyHandler::NewL( *this, 
                                    *iSession, 
                                    CCenRepNotifyHandler::EIntKey, 
                                    KFmTxCenResKeyFrequency );
    iFmTxPowerStateHandler->StartListeningL();
    TInt err = iSession->Get( KFmTxCenResKeyFrequency, iFrequency );
    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::ConstructL: err=%d, iFrequency=%d"), err, iFrequency ) );
    User::LeaveIfError( err );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver::HandleNotifyInt()
// ----------------------------------------------------------------------------

void CSysApCenRepFmTxObserver::HandleNotifyInt( TUint32 aId, TInt aNewValue )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::HandleNotifyInt(): aId=%d, aNewValue=%d" ), aId, aNewValue ) );
    if ( aId == KFmTxCenResKeyFrequency )
        {
        iFrequency = aNewValue;
        TFmTxState state = static_cast<TFmTxState>(iSysApAppUi.StateOfProperty( KPSUidHWRMFmTx, KHWRMFmTxStatus ));
        switch ( state )
            {
            case EFmTxStateActive:
            case EFmTxStateInactive:
            case EFmTxStatePowerSaveInactivity:
            case EFmTxStatePowerSaveAccessory:
            case EFmTxStateScanning:
                // show "Tune radio to xx.xx MHz" if FM TX is on
                TRAPD( err, iSysApAppUi.ShowUiNoteL( EFmTxOnNote ) );
                if ( err != KErrNone )
                    {
                    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::HandleNotifyInt(): err=%d" ), err ) );
                    }
                break;
            
            default:
                // do nothing
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver::HandleNotifyError()
// ----------------------------------------------------------------------------

void CSysApCenRepFmTxObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepFmTxObserver::HandleNotifyError()" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepFmTxObserver::Frequency()  
// ----------------------------------------------------------------------------

TInt CSysApCenRepFmTxObserver::Frequency() const
    {
    return iFrequency;
    }
    

// End of File

