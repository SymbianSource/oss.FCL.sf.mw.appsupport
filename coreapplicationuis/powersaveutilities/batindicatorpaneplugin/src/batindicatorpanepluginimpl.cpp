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
* Description:  BatIndicatorPanePlugin plugin implementation
*
*/


// INCLUDE FILES
#include <e32property.h> // RPropert
#include <batindpaneplugin.rsg> // BatIndicatorPanePlugin resources
#include <StringLoader.h> // Stringloader
#include <psmclient.h>
#include <psmsettings.h>
#include <hwrmpowerstatesdkpskeys.h>
#include <data_caging_path_literals.hrh> // KDC_ECOM_RESOURCE_DIR
#include <centralrepository.h>
#include "batindicatorpanepluginimpl.h"
#include "batindpanepluginprivatecrkeys.h"
#include "bsutil.h"
#include "trace.h"

// CONSTANTS
_LIT( KIndicatorResourceFile,"z:batindpaneplugin.rsc" );


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CBatIndicatorPanePluginImpl* CBatIndicatorPanePluginImpl::NewL( )
    {
    FUNC_LOG
    
    CBatIndicatorPanePluginImpl* self = new( ELeave ) CBatIndicatorPanePluginImpl( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBatIndicatorPanePluginImpl::~CBatIndicatorPanePluginImpl()
    {
    FUNC_LOG
    
    delete iContentText;
    delete iPsmClient; 
    delete iBatteryStateUtil;
    delete iPopupControl;
    delete iLinkText;   
    iResourceLoader.Close();
    }

// ---------------------------------------------------------------------------
// From MEikCommandObserver
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::ProcessCommandL( TInt aCommandId )
    {    	
    FUNC_LOG
    
    if( aCommandId == ELinkFirst )
        {
        SetPowerSaveModeEnabled();        	
        }                  
    }
        
// ---------------------------------------------------------------------------
// CBatIndicatorPanePluginImpl::CBatIndicatorPanePluginImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CBatIndicatorPanePluginImpl::CBatIndicatorPanePluginImpl( ):
                                                iResourceLoader( *CEikonEnv::Static() ),
                                                iBatIndPanePluginFeatures( 0 )
    {
    FUNC_LOG                                          	
    }

// ---------------------------------------------------------------------------
// CBatIndicatorPanePluginImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::ConstructL( )
    {
    FUNC_LOG
    // Create PSM client       
    iPsmClient = CPsmClient::NewL( *this );
    
    GetFeatures();
        
    // Create Battery State Utilities
    if( !Supported( KBatIndPanePluginLinkOnly ) )
        {
        iBatteryStateUtil = CBSUtil::NewL();
        }
            
    // Read resource strings
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    fp->Set( KIndicatorResourceFile, &KDC_ECOM_RESOURCE_DIR , NULL );
    TFileName filename = fp->FullName();
    iResourceLoader.OpenL( filename );
    CleanupStack::PopAndDestroy( fp );
    }

// ---------------------------------------------------------------------------
// Shows soft notification.
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::ShowPreviewPopupL( )
    {
    FUNC_LOG
    
    delete iPopupControl;
    iPopupControl = NULL;        
    
    if( iContentText )
        {
        iPopupControl = CBatteryPopupControl::NewL( *iContentText,
                                                    *iLinkText );
        }
    else
        {
        iPopupControl = CBatteryPopupControl::NewL( KNullDesC,
                                                    *iLinkText );              
        }

    iPopupControl->SetCommandObserver( *this );
    // Launch
    iPopupControl->ShowPopUpL();
    
    delete iContentText;
    iContentText = NULL;

    delete iLinkText;
    iLinkText = NULL;
	}
	  
// ---------------------------------------------------------------------------
// Enables or disables power save mode.
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::SetPowerSaveModeEnabled( )
    {
    FUNC_LOG
    // Get PSM state
    TInt mode;
    TInt err = iPsmClient->PsmSettings().GetCurrentMode( mode );
        
    if ( err == KErrNone  )
        {
        if( mode == EPsmsrvModeNormal ||
            mode == EPsmsrvPartialMode )
            {
            iPsmClient->ChangePowerSaveMode( EPsmsrvModePowerSave );
            }
        else
            {
            iPsmClient->ChangePowerSaveMode( EPsmsrvModeNormal );
            }
        }
    }	  

// ---------------------------------------------------------------------------
// From CAknIndicatorPlugin
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::HandleIndicatorTapL( const TInt /*aUid*/ )
    {
    FUNC_LOG
    
    delete iContentText;
    iContentText = NULL;
    
    delete iLinkText;
    iLinkText = NULL;
    
    TInt err = KErrNone;
    TInt error = KErrNone;
    
    if ( iBatteryStateUtil )
        {
        TInt batteryStatus,chargingStatus;
        err = RProperty::Get( KPSUidHWRMPowerState, KHWRMBatteryStatus, batteryStatus );
        error = RProperty::Get( KPSUidHWRMPowerState, KHWRMChargingStatus,chargingStatus  );
        if(!error && ( chargingStatus==EChargingStatusChargingContinued || chargingStatus==EChargingStatusCharging))
            {
            iContentText = StringLoader::LoadL( R_QTN_BATTERY_CHARGING_POPUP ); 
            }
        
        else if( !err && batteryStatus == EBatteryStatusLow )
            {
            iContentText = StringLoader::LoadL( R_QTN_BATTERY_LOW_PREVIEW_POPUP ); 
            }
        else
            {
            TUint8 batteryCapacity = iBatteryStateUtil->GetBatteryCapacity();	
                   
            iContentText = StringLoader::LoadL( R_QTN_BATTERY_STATUS_POPUP, 
                                                batteryCapacity );                
            }
        }
    
    // Get PSM state
    TInt mode;
    TBool psmActive = EFalse;
    err = iPsmClient->PsmSettings().GetCurrentMode( mode );
      
    if ( err == KErrNone  )
        {
        psmActive = ( mode == EPsmsrvModePowerSave );
        }
                        
    if ( psmActive )
        {
        iLinkText = StringLoader::LoadL( R_QTN_DEACTIVATE_POWER_SAVING );
        }
    else
        {      
        iLinkText = StringLoader::LoadL( R_QTN_ACTIVATE_POWER_SAVING );           
        }        
    ShowPreviewPopupL();  
    }

// ----------------------------------------------------------------------------
// CBatIndicatorPanePluginImpl::GetFeatures() 
// ----------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::GetFeatures( )
    {
    FUNC_LOG
    
    CRepository* repository = NULL;
    
    TRAPD( err, repository = CRepository::NewL( KCRUidBatIndPanePluginConf ) );
    
    if ( err == KErrNone )
        {
        err = repository->Get( KBatIndPanePlugin, iBatIndPanePluginFeatures );

        if ( err != KErrNone || iBatIndPanePluginFeatures < 0 ) // ERROR
            {
            INFO_1( "CBatIndicatorPanePluginImpl::GetFeatures - Reading features err %d", err )
            iBatIndPanePluginFeatures = 0;
            }
        }
    else
        {
        INFO_1( "CBatIndicatorPanePluginImpl::GetFeatures: open KCRUidBatIndPanePluginConf failed, err=%d", err ) 
        }
    delete repository;
    }

// ----------------------------------------------------------------------------
// CBatIndicatorPanePluginImpl::Supported() 
// ----------------------------------------------------------------------------
//
TInt CBatIndicatorPanePluginImpl::Supported( TInt aFeatureId )
    {
    FUNC_LOG
    return aFeatureId & iBatIndPanePluginFeatures;
    }
// ---------------------------------------------------------------------------
// From MPsmClientObserver
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::PowerSaveModeChanged( const TPsmsrvMode /*aMode*/ )
    {
    FUNC_LOG
    }

// ---------------------------------------------------------------------------
// From MPsmClientObserver
// ---------------------------------------------------------------------------
//
void CBatIndicatorPanePluginImpl::PowerSaveModeChangeError( const TInt /*aError*/ )
    {
    FUNC_LOG      	
    }	 
//  End of File
