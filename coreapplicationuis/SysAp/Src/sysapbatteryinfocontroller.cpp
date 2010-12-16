/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApBatteryInfoController implementation
*
*/


#include <e32debug.h>
#include <e32property.h>
#include <StringLoader.h>
#include <SysAp.rsg>
#include "bsutil.h"
#include <hwrmpowerstatesdkpskeys.h>
#include <aknappui.h>
#include <AknCapServerClient.h>
#include <AknSgcc.h>
#include "batterypopupcontrol.h"
#include "sysapbatteryinfocontroller.h"
#include "SysAp.hrh"

#include "startupdomainpskeys.h"

static const TInt KDefaultThreshold = 30;
static const TInt KNotInitialized = -1;
static const TInt KCapacityDeltaPercentage = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::CSysApBatteryInfoController
// ---------------------------------------------------------------------------
//
CSysApBatteryInfoController::CSysApBatteryInfoController( TInt aThreshold) 
                                                        : iBsUtil( NULL ),
                                                          iCurrentCapacity( KNotInitialized ),
                                                          iThresholdCapacity( aThreshold  ),
                                                          iBatteryInfoState( EBatteryInfoAboveThreshold ),
                                                          iBatteryPopup( NULL )
    {
    // sanity check, fall back to hard coded default value
    const TInt min(0);
    const TInt max(100);
    
    if ( iThresholdCapacity < min || iThresholdCapacity > max )
        {
        iThresholdCapacity = KDefaultThreshold;
        }
    
    }

// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::ConstructL
// ---------------------------------------------------------------------------
//
void CSysApBatteryInfoController::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::ConstructL") ) );
    
    iBsUtil = CBSUtil::NewL();
    }


// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::NewL
// ---------------------------------------------------------------------------
//
CSysApBatteryInfoController* CSysApBatteryInfoController::NewL( TInt aThreshold )
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::NewL: aThreshold=%d"), aThreshold ) );
    CSysApBatteryInfoController* self = new( ELeave ) CSysApBatteryInfoController( aThreshold );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::~CSysApBatteryInfoController
// ---------------------------------------------------------------------------
//
CSysApBatteryInfoController::~CSysApBatteryInfoController()
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::~CSysApBatteryInfoController") ) );
    delete iBsUtil;
    delete iBatteryPopup;
    }

// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::PowerMenuShownL
// ---------------------------------------------------------------------------
//
void CSysApBatteryInfoController::PowerMenuShownL()
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::PowerMenuShownL") ) );
    ShowBatteryPreviewPopupL( iBsUtil->GetBatteryCapacity() );
    }
     
// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::BatteryLevelUpdatedL
// ---------------------------------------------------------------------------
//
void CSysApBatteryInfoController::BatteryLevelUpdatedL()
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::BatteryLevelUpdatedL: iCurrentCapacity=%d, iBatteryInfoState=%d"),
            iCurrentCapacity, iBatteryInfoState ) );
    
    TInt prevCapacity = iCurrentCapacity;
    iCurrentCapacity = iBsUtil->GetBatteryCapacity();
    
    TBool showPopup( EFalse );
    
    switch ( iBatteryInfoState )
        {
        case EBatteryInfoAboveThreshold:
            // popup is displayed if the battery capacity is below defined threshold
            showPopup = ( iCurrentCapacity <= iThresholdCapacity );
            break;
        
        case EBatteryInfoBelowThreshold:
            // popup is displayed if the battery capacity has decreased enough
            if ( ( prevCapacity > iCurrentCapacity )
                && ( prevCapacity - iCurrentCapacity >= KCapacityDeltaPercentage ) )
                {
                showPopup = ETrue;
                }
            break;
        
        case EBatteryInfoStatusLow: // do not show the popup
        default:
            break;
        }
    
    if ( iBatteryInfoState != EBatteryInfoStatusLow ) // state change allowed here only if battery is not low
        {
        // never switch to EBatteryInfoStatusLow here, it is done only in BatteryStatusUpdated()
        iBatteryInfoState = ( iCurrentCapacity <= iThresholdCapacity ) ?  EBatteryInfoBelowThreshold : EBatteryInfoAboveThreshold;    
        }
    
    
    if ( showPopup )
        {
        ShowBatteryPreviewPopupL( iCurrentCapacity );
        }
    }

// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::BatteryStatusUpdated
// ---------------------------------------------------------------------------
//
void CSysApBatteryInfoController::BatteryStatusUpdated( const TInt aValue )
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::BatteryStatusUpdated: aValue=%d, iCurrentCapacity=%d, iBatteryInfoState=%d"),
            aValue, iCurrentCapacity, iBatteryInfoState ) );
            
    switch ( aValue )
        {
        case EBatteryStatusLow:
        case EBatteryStatusEmpty:
            // battery is low, discontinue showing battery popup
            iBatteryInfoState = EBatteryInfoStatusLow;
            break;
        
        default:
            // back to normal operation
            iBatteryInfoState = ( iCurrentCapacity <= iThresholdCapacity )
                                ? EBatteryInfoBelowThreshold
                                : EBatteryInfoAboveThreshold;
            break;
        }
    }

// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::ShowBatteryPreviewPopupL
// ---------------------------------------------------------------------------
//
void CSysApBatteryInfoController::ShowBatteryPreviewPopupL( TInt aCapacity )
    {
    TRACES( RDebug::Print( _L("CSysApBatteryInfoController::ShowBatteryPreviewPopupL: aCapacity=%d"), aCapacity ) );
    
	// If the charger is connected in the device powered off state,
    // The NOKIA logo screen is replaced with dark screen to save the power
    // SysAp is not showing the Battery % level.
	TInt state( 0 );
    TInt error = RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state );
    if ( error == KErrNone && state == ESwStateCharging ) 
        {
        return;
        }
    
    delete iBatteryPopup;
    iBatteryPopup = NULL;
    HBufC* popupTextBuf = StringLoader::LoadLC( R_QTN_BATTERY_STATUS_POPUP, aCapacity ); 
    iBatteryPopup = CBatteryPopupControl::NewL( *popupTextBuf, KNullDesC );
    CleanupStack::PopAndDestroy( popupTextBuf );
        
    // switch layout for showing the battery popup
    CAknAppUi* appUi = static_cast<CAknAppUi*>(CCoeEnv::Static()->AppUi());
    CEikStatusPane* statusPane = appUi->StatusPane();
    
    TInt currLayout = statusPane->CurrentLayoutResId();
        
    RAknUiServer* aknSrv = CAknSgcClient::AknSrv();
    if ( aknSrv )
        {
        statusPane->SwitchLayoutL( aknSrv->StatusPaneResourceId()  );	
        }    
    // Trap for switching layout back to current.
    TRAPD( err, iBatteryPopup->ShowPopUpL() );
    
    if( err )
        {
        TRACES( RDebug::Print( _L("CSysApBatteryInfoController::ShowBatteryPreviewPopupL ShowPopUpL() leaved: %d"), err ) );
        }
        
    // restore layout
    statusPane->SwitchLayoutL( currLayout );
    
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// CSysApBatteryInfoController::IsBatteryInfoAboveThreshold
// ---------------------------------------------------------------------------
//
TBool CSysApBatteryInfoController::IsBatteryInfoAboveThreshold() const
        {
        TInt capacity = iBsUtil->GetBatteryCapacity();
        return ((capacity <= iThresholdCapacity ? EBatteryInfoBelowThreshold : EBatteryInfoAboveThreshold) == EBatteryInfoAboveThreshold);
        }
