/*
* Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*		Implements CRfsHandles class.
*
*/


// INCLUDE FILES

#include <e32std.h>
#include <bautils.h>                // BaflUtils
#include <eikenv.h>                 // CEikonEnv
#include <etelmm.h>
#include <mmtsy_names.h>

#include <featmgr.h>
#include <e32property.h>
#include <PSVariables.h>
#include <ctsydomainpskeys.h>
#include <ir_sock.h>
#include <bt_subscribe.h>
#include <wlaninternalpskeys.h>

#include <starterclient.h>
#include <syslangutil.h>            // default language

#include <secui.h>                  // security UI
#include <secuisecurityhandler.h>
#include <StringLoader.h>
#include <hal.h>
#include <centralrepository.h>
#include <CommonEngineDomainCRKeys.h>

#include <data_caging_path_literals.hrh>

#include "CleanupResetPointer.h"
#include <pdpcontextmanagerpskeys.h>
#include <pdpcontextmanagerinternalcrkeys.h>
#include "rfsConnectionObserver.h"
#include "rfsHandler.h"
#include "rfsClient.h"
#include "RfsTraces.h"
#include <hbdevicemessageboxsymbian.h>
#include <hbtextresolversymbian.h>


_LIT(Kcommonerrors,"common_errors_");
_LIT(Kcontrolpanel,"control_panel_");
_LIT(KtsfilePath,"z:/resource/qt/translations/");


// CONSTANTS
const TInt KPhoneIndex = 0;
const TInt KRfsHandlerActivated             = 0x0001;
const TInt KRfsHandlerPhoneModuleLoaded     = 0x0002;
const TInt KRfsHandlerSecUiInitialized      = 0x0004;
const TInt KRfsHandlerInitDone              = 0x0008;


EXPORT_C CRfsHandler::CRfsHandler()
    {
    }

EXPORT_C CRfsHandler::~CRfsHandler()
    {
    if ( iDestroyedPtr )
        {
        *iDestroyedPtr = ETrue;
        iDestroyedPtr = NULL;
        }
    Cancel();
    }

// -----------------------------------------------------------------------------
// CRfsHandler::ActivateRfsL()
// -----------------------------------------------------------------------------
EXPORT_C void CRfsHandler::ActivateRfsL( TRfsType aType, TBool aAskSecurityCodeFirst )
    {
    TRACES("CRfsHandler::ActivateRfsL()");
    
    // if already activated, do nothing
    if ( iFlags & KRfsHandlerActivated )
        {
        TRACES("CRfsHandler::ActivateRfsL(): return");
        return;
        }

    
    // Create a generic connection observer for closing all the active connections
    // if they exist. Currently there are only 2 types i.e. SIP and PDP
    CRfsConnectionObserver* connectionObserver = CRfsConnectionObserver::NewLC();
 
    // This wil return a boolean:
    //  ETrue:  if all the active connections are closed  i.e. both SIP and PDP
    //  EFalse: if the user pressed 'Cancel' button in between any of the connection closing process
    iAlwaysOnlineConnectionClosed = connectionObserver->CloseAlwaysOnConnectionL();
    
	// Ensure there aren't any active calls or data sessions
    if ( CheckConnectionsL() )
        {
        // If there are some connections active, then we publish that the RFS has failed/stoped
        connectionObserver->ReOpenPDPConnection();
        connectionObserver->ReportRfsFailureToSip();
        CleanupStack::PopAndDestroy(connectionObserver);
        return;
        }

    // Puts 'this' in the cleanup stack so that Cancel() is called on leave
    CleanupStack::PushL( TCleanupItem( DoCleanup, this ) );

    // Do other initialisations if not already done
    if ( !(iFlags & KRfsHandlerInitDone) )
        {
        // create or open the needed resources
        User::LeaveIfError( iTelServer.Connect() );
        iTelServer.LoadPhoneModule( KMmTsyModuleName );
        iFlags |= KRfsHandlerPhoneModuleLoaded;
        iTelServer.SetExtendedErrorGranularity( RTelServer::EErrorExtended );
        
        RTelServer::TPhoneInfo info;
        // get info about default phone (KPhoneIndex)
        User::LeaveIfError( iTelServer.GetPhoneInfo( KPhoneIndex, info ) );
        User::LeaveIfError( iPhone.Open( iTelServer, info.iName ) );
        TSecUi::InitializeLibL();
        iFlags |= KRfsHandlerSecUiInitialized;
        iSecurityHandler = new( ELeave ) CSecurityHandler( iPhone );
        }

    iFlags |= ( KRfsHandlerActivated | KRfsHandlerInitDone );
    TBool proceed( ETrue );

    
    // Ask first query.
    if ( aAskSecurityCodeFirst )
        {
        proceed = iSecurityHandler->AskSecCodeL();
        }
    else
        {
        proceed = AskConfirmationL( aType );
        }

    // If OK, ask second query.
    if ( proceed )
        {
        if ( aAskSecurityCodeFirst )
            {
            proceed = AskConfirmationL( aType );
            }
        else
            {
            proceed = iSecurityHandler->AskSecCodeL();
            }
        }

    // If all OK, then perform RFS.
    if ( proceed && !CheckConnectionsL() )
        {
        // In case of deep level RFS, set the default language code here, before RFS reboot.
        if ( aType == ERfsDeep )
            {
            SetDefaultLanguage();
            }
        RStarterSession startersession;
        if( startersession.Connect() == KErrNone )
            {
			// Displays information note to the user telling that the device will restart
            TBool result = HbTextResolverSymbian::Init(Kcommonerrors, KtsfilePath);
            _LIT(Krestarting,"txt_error_info_restarting");
            HBufC* prompt =HbTextResolverSymbian::LoadL(Krestarting);
                 
            CHbDeviceMessageBoxSymbian* note = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EInformation);
                CleanupStack::PushL(note);
                note->SetTextL(*prompt);
                //could have used show() but it is aynchronous and execution proceeds before note is seen so have used synchronous API exec()
                note->ExecL();
                CleanupStack::PopAndDestroy(note);
                
                	
                	              
               
                
            if (aType == ERfsNormal ) startersession.Reset( RStarterSession::ENormalRFSReset );
            else if (aType == ERfsDeep ) startersession.Reset( RStarterSession::EDeepRFSReset );
            else startersession.Reset( RStarterSession::EUnknownReset );
            startersession.Close();
            }
        }
    // Inform SIP and PDP that the RFS has completed its operation.
    // The method is used for two purposes
    //  1. When we reopen the connection due to RFS failure.
    //  2. when we have successfully completed the RFS operation.
    // In both of the above cases the RFS has completed either successfully or not 
    connectionObserver->ReOpenPDPConnection();
    
    if(!proceed)
        {
        // Inform SIP that user has either selected not to perform the RFS
        // or has failed to provide the right security code due to which
        // RFS operation is not performed. So in this case the SIP should reopen 
        // its connection.
        connectionObserver->ReportRfsFailureToSip();
        }
    else
        {
        connectionObserver->ReportRfsCompletionToSip();
        }
       
    CleanupStack::PopAndDestroy( this ); // this
     
    CleanupStack::PopAndDestroy(connectionObserver);
    }

// -----------------------------------------------------------------------------
// CRfsHandler::Cancel()
// -----------------------------------------------------------------------------
EXPORT_C void CRfsHandler::Cancel()
    {
    TRACES("CRfsHandler::Cancel()");
    
     
     // delete security handler
    if ( iSecurityHandler )
        {
        iSecurityHandler->CancelSecCodeQuery();

        delete iSecurityHandler;
        iSecurityHandler = NULL;
        }

    // uninitialize security UI
    if ( iFlags & KRfsHandlerSecUiInitialized )
        {
        TSecUi::UnInitializeLib();
        }

    // close ETel connection
    iPhone.Close();

    if ( iFlags & KRfsHandlerPhoneModuleLoaded )
        {
        iTelServer.UnloadPhoneModule( KMmTsyModuleName );
        }

    iTelServer.Close();

    // finally, reset the flags
    iFlags = 0;
    }


// -----------------------------------------------------------------------------
// CRfsHandler::SetDefaultLanguage()
// -----------------------------------------------------------------------------
void CRfsHandler::SetDefaultLanguage() const
    {
    TRACES("CRfsHandler::SetDefaultLanguage()");

    CRepository* repository = NULL;
    TRAPD( err, repository = CRepository::NewL( KCRUidCommonEngineKeys ) );
    if ( err == KErrNone )
        {
        err = repository->Reset( KGSDisplayTxtLang );
        TRACE_IF_ERROR_1("CRfsHandler::SetDefaultLanguage(): Failed to write language code: %d",err)
        delete repository;
        }
    else
        {
        TRACES1("CRfsHandler::SetDefaultLanguage(): Failed to open repository: %d", err);
        }
    }

// -----------------------------------------------------------------------------
// CRfsHandler::AskConfirmationL()
// -----------------------------------------------------------------------------
TBool CRfsHandler::AskConfirmationL( TRfsType aType )
    {
    TRACES("CRfsHandler::AskConfirmationL()");
   
     TBool result = HbTextResolverSymbian::Init(Kcontrolpanel, KtsfilePath);
     _LIT(Knormalrfs,"txt_cp_info_restore_original_settings_no_data_wil");
     _LIT(KDeeprfs,"txt_cp_info_delete_all_data_and_restore_original_s");
    
        
     HBufC* query = NULL;
    if( aType == ERfsNormal ) 
      query = HbTextResolverSymbian::LoadL(Knormalrfs);
    else
      query = HbTextResolverSymbian::LoadL(KDeeprfs); 
   
    _LIT(Kcommonlocalisationfile, "common_");
    TBool result1 = HbTextResolverSymbian::Init(Kcommonlocalisationfile, KtsfilePath);
    _LIT(Kyes,"txt_common_button_ok");
    _LIT(Kno,"txt_common_button_cancel");
   
    HBufC* yes = HbTextResolverSymbian::LoadL(Kyes);
    HBufC* no = HbTextResolverSymbian::LoadL(Kno);
    // Show the confirmation query.   
   CHbDeviceMessageBoxSymbian::TButtonId selection = CHbDeviceMessageBoxSymbian::QuestionL(*query,*yes,*no);
    TBool ret;    
    if (selection == CHbDeviceMessageBoxSymbian::EAcceptButton) // user pressed yes
        {
        ret=ETrue;
        }
    else
        {
        ret=EFalse;
        }  
    return ret;  
    
    }

// -----------------------------------------------------------------------------
// CRfsHandler::DoCleanup()
// -----------------------------------------------------------------------------
void CRfsHandler::DoCleanup( TAny* aAny )
    {
    TRACES("CRfsHandler::DoCleanup()");
    REINTERPRET_CAST( CRfsHandler*, aAny )->Cancel();
    }

// -----------------------------------------------------------------------------
// CRfsHandler::CheckConnectionsL()
// -----------------------------------------------------------------------------
TBool CRfsHandler::CheckConnectionsL()
	{
    TInt callState( 0 );
    TInt gprsState( 0 );
    TInt wcdmaState( 0 );
    TInt btState( 0 );
    TInt irdaState( 0 );
    TInt wlanState( 0 );
    
    RProperty property;

    property.Attach( KPSUidCtsyCallInformation, KCTsyCallState );
    property.Get( callState );

    property.Attach( KPropertyUidBluetoothCategory, KPropertyKeyBluetoothGetPHYCount );
    property.Get( btState );

    property.Attach( KIrdaPropertyCategory, KIrdaStatus );
    property.Get( irdaState );

    TUid categoryUid = { KUidSystemCategoryValue };
    property.Attach( categoryUid, KPSUidGprsStatusValue );
    property.Get( gprsState );

    property.Attach( categoryUid, KPSUidWcdmaStatusValue );
    property.Get( wcdmaState );

    property.Attach( KPSUidWlan, KPSWlanIndicator );
    property.Get( wlanState );
    
    property.Close();

    if (IsGprs())
        {
        wcdmaState = EPSWcdmaUnattached;
        }
    else
        {
        gprsState = EPSGprsUnattached;
        }

    TRACES1("CRfsHandler::ActivateRfsL(): Call state:     %d", callState);
    TRACES1("CRfsHandler::ActivateRfsL(): Gprs state:     %d", gprsState);
    TRACES1("CRfsHandler::ActivateRfsL(): Wcdma state:    %d", wcdmaState);
    TRACES1("CRfsHandler::ActivateRfsL(): BT state:       %d", btState);
    TRACES1("CRfsHandler::ActivateRfsL(): IrDa State:     %d", irdaState);
    TRACES1("CRfsHandler::ActivateRfsL(): WLan State:     %d", wlanState);

    if( FeatureManager::FeatureSupported( KFeatureIdAlwaysOnlinePDPContext2 ) )
        {
        if (iAlwaysOnlineConnectionClosed)
            {
            gprsState = KErrUnknown;
            wcdmaState = KErrUnknown;
            }
        }    

#ifndef __WINS__ //Do not allow RFS in emulator
    if (( callState != KErrUnknown &&
          callState != EPSCTsyCallStateNone ) ||
        ( gprsState != KErrUnknown &&
          gprsState != EPSGprsUnattached &&
          gprsState != EPSGprsAttach &&
          gprsState != EPSGprsSuspend) ||
        ( wcdmaState != KErrUnknown &&
          wcdmaState != EPSWcdmaUnattached &&
          wcdmaState != EPSWcdmaAttach &&
          wcdmaState != EPSWcdmaSuspend) ||
        ( btState ) ||
        ( irdaState == TIrdaStatusCodes::EIrConnected ) ||
        ( wlanState == EPSWlanIndicatorActive || wlanState == EPSWlanIndicatorActiveSecure ))
#endif //__WINS__
        {
        TRACES("CRfsHandler::checkconnectionsL():show active connections note ");
        TBool result = HbTextResolverSymbian::Init(Kcontrolpanel, KtsfilePath);
        _LIT(Kactivecalls,"txt_cp_info_active_calls_and_connections_must_be_d");
        HBufC* prompt = HbTextResolverSymbian::LoadL(Kactivecalls);

        CHbDeviceMessageBoxSymbian* note = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EInformation);
        CleanupStack::PushL(note);
        note->SetTextL(*prompt);
        note->ShowL();
        CleanupStack::PopAndDestroy( note );
                      
        return ETrue;
        }

    return EFalse;
	}

// -----------------------------------------------------------------------------
// CRfsHandler::IsGprs()
// -----------------------------------------------------------------------------
TBool CRfsHandler::IsGprs() const
    {
    TRACES("CRfsHandler::IsGprs()");
    RTelServer telServer;
    RMobilePhone mobilePhone;
    TInt error = telServer.Connect();
    if ( error != KErrNone )
        {
        TRACES("CRfsHandler::IsGprs(): Connect to RTelServer failed");
        return ETrue;
        }
    error = telServer.LoadPhoneModule( KMmTsyModuleName );
    if ( error != KErrNone )
        {
        TRACES("CRfsHandler::IsGprs(): Loading phone module failed");
        telServer.Close();
        return ETrue;
        }
    RTelServer::TPhoneInfo phoneInfo;
    error = telServer.GetPhoneInfo( KPhoneIndex, phoneInfo );
    if ( error != KErrNone )
        {
        TRACES("CRfsHandler::IsGprs(): Getting phone info failed");
        telServer.Close();
        return ETrue;
        }
    error = mobilePhone.Open( telServer, phoneInfo.iName );
    if ( error != KErrNone )
        {
        TRACES("CRfsHandler::IsGprs(): Opening RMobilePhone failed");
        telServer.Close();
        return ETrue;
        }
    RMobilePhone::TMobilePhoneNetworkMode networkMode = RMobilePhone::ENetworkModeGsm;
    error = mobilePhone.GetCurrentMode( networkMode );

    mobilePhone.Close();
    telServer.Close();

    if ( error == KErrNone && networkMode == RMobilePhone::ENetworkModeWcdma )
        {
        TRACES("CRfsHandler::IsGprs(): Currently in 3G network");
        return EFalse;
        }
    else
        {
        TRACES("CRfsHandler::IsGprs(): Currently in 2G network");
        return ETrue;
        }
    }

//  End of File
