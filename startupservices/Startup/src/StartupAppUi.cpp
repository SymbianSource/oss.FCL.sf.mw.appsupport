/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  AppUi class of the application.
*
*/


// SYSTEM INCLUDES
#include <StringLoader.h>
#include <AknGlobalNote.h>          //used for Selftest failed note
#include <aknPopup.h>
#include <aknlists.h>
#include <aknSDData.h>
#include <badesca.h>
#include <tzlocalizationdatatypes.h>
#include <tzlocalizer.h>
#include <tz.h>
#include <featmgr.h>                // Feature Manager
#include <centralrepository.h>
#include <startup.rsg>
#include <starterdomaincrkeys.h>
#include "startupappprivatepskeys.h"
#include <startupdomainpskeys.h>
#include <startupdomaincrkeys.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <starterclient.h>

#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
// Transition effects
#include <gfxtranseffect/gfxtranseffect.h>
#include <akntranseffect.h>
#endif

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
  #include "sanimstartupctrl.h"
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
  #include <akndef.h>                 // For layout change event definitions
  #include <AknSoundSystem.h>
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// USER INCLUDES
#include "StartupAppUi.h"
#include "StartupApplication.h"
#include "StartupUserWelcomeNote.h"
#include "StartupQueryDialog.h"     //used for Startup own Time and Date queries
#include "StartupPopupList.h"       //used for Startup own City and Country queries
#include "StartupMediatorObserver.h"
#include "StartupPubSubObserver.h"

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
  #include "startupanimationwrapper.h"
  #include "startupview.h"
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
  #include "StartupDocument.h"
  #include "StartupOperatorAnimation.h"
  #include "StartupTone.h"
  #include "StartupWelcomeAnimation.h"
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


// CONSTANTS
const TInt KUserWelcomeNoteShowPeriodTime = 3000000; // 3 sec

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
  const TInt KOneMilliSecondInMicroSeconds = 1000;
  const TInt KMaxToneInitWait = 200; // 200 ms
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

static const TInt KMaxCityLength(120);
static const TInt KMaxCountryLength(120);

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
static const CSAnimStartupCtrl::TAnimationParams KStartupAnimationParams =
    {
    KCRUidStartupConf,
    KStartupAnimationPath,
    KStartupAnimationFrameDelay,
    KStartupAnimationScaling,
    KStartupTonePath,
    KStartupToneVolume
    };
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

_LIT(KEmpty, " ");

// Default date and time value is used if cenrep string is not valid
// Default date is 01.01.2007 and default time is 09:00 AM
_LIT( KDefaultDateTimeValue, "20070000:090000" ); // YYYYMMDD:HHMMSS

static const TInt KTimeFormatLength(16); // "20070000:090000."

_LIT_SECURITY_POLICY_C1(KReadDeviceDataPolicy, ECapabilityReadDeviceData);
_LIT_SECURITY_POLICY_C1(KWriteDeviceDataPolicy, ECapabilityWriteDeviceData);
_LIT_SECURITY_POLICY_PASS(KAlwaysPassPolicy);

// ======== LOCAL FUNCTIONS ==================================================

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
namespace
    {
    TInt AnimationFinishedFunc( TAny* aPtr )
        {
        static_cast<CStartupAppUi*>( aPtr )->AnimationFinished();
        return KErrNone;
        }
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


// ================= MEMBER FUNCTIONS ========================================
//
// ---------------------------------------------------------------------------
// CStartupAppUi::CStartupAppUi()
// ---------------------------------------------------------------------------
CStartupAppUi::CStartupAppUi() :
    iUserWelcomeNote( NULL ),
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iStartupTone( NULL ),
    iOpStartupTone( NULL ),
    iToneInitWaitTime( 0 ),
    iAnimation( EFalse ),
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iInternalState( EStartupStartingUp ),
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iSplashScreenShouldBeRemoved( EFalse ),
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iStartupFirstBootAndRTCCheckAlreadyCalled( EFalse ),
    iChargingOrAlarmBoot( EFalse ),
    iFirstBoot( ETrue ),
    iCleanBoot( EStartupNormalBoot ),
    iOfflineModeSupported( EFalse ),
    iOfflineModeQueryShown( EFalse ),
    iCriticalBlockEnded( EFalse ),
    iSwStateFatalStartupError( EFalse ),
    iStartupWaitingShowStartupAnimation( EFalse ),
    iSimSupported( ETrue ),
    iStartupMediatorObserver( NULL ),
    iCoverUISupported( EFalse ),
    iCounryListIndex( 0 )
    , iTime( 0 )
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    , iTouchScreenCalibSupport( EFalse )
    , iTouchScreenCalibrationDone( EFalse )
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    {
    TRACES("CStartupAppUi::CStartupAppUi");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ConstructL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ConstructL()
    {
    TRACES("CStartupAppUi::ConstructL()");
    TInt flags = EStandardApp|EAknEnableSkin|EAknEnableMSK ;

    BaseConstructL(flags);

    iAvkonAppUi->SetKeyEventFlags( CAknAppUiBase::EDisableSendKeyShort |
                                   CAknAppUiBase::EDisableSendKeyLong );

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iMainView = CStartupView::NewL( ApplicationRect() );
    iAnimation = CStartupAnimationWrapper::NewL( *iMainView );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TInt err = RProperty::Define( KPSUidStartupApp,
                                  KPSStartupAppState,
                                  RProperty::EInt,
                                  KReadDeviceDataPolicy,
                                  KWriteDeviceDataPolicy );

    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KPSStartupAppState define err %d", err);
        }

    err = RProperty::Set( KPSUidStartupApp,
                          KPSStartupAppState,
                          EStartupAppStateWait );

    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KPSStartupAppState set err %d", err);
        }

    iStartupPubSubObserver = CStartupPubSubObserver::NewL( this );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    err = RProperty::Define( KPSUidStartup,
                             KStartupBootIntoOffline,
                             RProperty::EInt,
                             KReadDeviceDataPolicy,
                             KWriteDeviceDataPolicy );

    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KStartupBootIntoOffline define err %d", err);
        }

    err = RProperty::Define( KPSUidStartup,
                             KStartupSecurityCodeQueryStatus,
                             RProperty::EInt,
                             KReadDeviceDataPolicy,
                             KWriteDeviceDataPolicy );
    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KStartupSecurityCodeQueryStatus define err %d", err);
        }
    err = RProperty::Define( KPSUidStartup,
                             KStartupCleanBoot,
                             RProperty::EInt,
                             KReadDeviceDataPolicy,
                             KWriteDeviceDataPolicy );
    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KStartupCleanBoot define err %d", err);
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    err = RProperty::Define( KPSUidStartup,
                             KPSStartupUiPhase,
                             RProperty::EInt, 
                             KAlwaysPassPolicy,
                             KWriteDeviceDataPolicy );
                             
    if( KErrNone != err &&
        KErrAlreadyExists != err )
        {
        TRACES1("CStartupAppUi::ConstructL(): KPSStartupUiPhase define err %d", err);       
        }
    
    UpdateStartupUiPhase( EStartupUiPhaseUninitialized );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) )
        {
        iOfflineModeSupported = ETrue;
        }
    if ( !FeatureManager::FeatureSupported( KFeatureIdSimCard ) )
        {
        iSimSupported = EFalse;
        }
    if ( FeatureManager::FeatureSupported( KFeatureIdCoverDisplay ) )
        {
        iCoverUISupported = ETrue;
        }

    TRACES1("CStartupAppUi::ConstructL(): Offline mode supported: %d", iOfflineModeSupported );
    TRACES1("CStartupAppUi::ConstructL(): SIM card supported:     %d", iSimSupported );
    TRACES1("CStartupAppUi::ConstructL(): CoverUI supported:      %d", iCoverUISupported );

#if defined (RD_SCALABLE_UI_V2) && !defined(RD_STARTUP_ANIMATION_CUSTOMIZATION)
    if ( FeatureManager::FeatureSupported(KFeatureIdPenSupport) &&
         FeatureManager::FeatureSupported(KFeatureIdPenSupportCalibration) )
        {
        iTouchScreenCalibSupport = ETrue;
        }
#endif // RD_SCALABLE_UI_V2 && !RD_STARTUP_ANIMATION_CUSTOMIZATION

    if (iCoverUISupported)
        {
        iStartupMediatorObserver = CStartupMediatorObserver::NewL( this );
        }

    iFirstBoot = FirstBoot();
    TRACES1("CStartupAppUi::ConstructL(): First boot:             %d", iFirstBoot );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iWelcomeAnimation = CStartupWelcomeAnimation::NewL( this, ClientRect());
    AddToStackL( iWelcomeAnimation );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    // Set Startup application to be system application
    iEikonEnv->SetSystem( ETrue );

    iEikonEnv->RootWin().SetOrdinalPosition(0,0);

    // Disable priority changes of window server
    iEikonEnv->WsSession().ComputeMode(
        RWsSession::EPriorityControlDisabled );

    iNoteTimer = CPeriodic::NewL( EPriorityNormal );
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iAnimTimer = CPeriodic::NewL( EPriorityNormal );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iExitTimer = CPeriodic::NewL( EPriorityNormal );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    TRACES("CStartupAppUi::ConstructL(): StartupTone: Initialising");
    iStartupTone = CStartupTone::NewL( this, EStartupTone );
    TRACES("CStartupAppUi::ConstructL(): StartupTone: Initialised");

    TRACES("CStartupAppUi::ConstructL(): Operator StartupTone: Initialising");
    iOpStartupTone = CStartupTone::NewL( this, EStartupOpTone );
    TRACES("CStartupAppUi::ConstructL(): Operator StartupTone: Initialised");
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    if ( iSwStateFatalStartupError )
        {
        TRACES("CStartupAppUi::ConstructL(): ESWStateFatalStartupError");
        DoNextStartupPhaseL( EStartupSystemFatalError );
        TRACES("CStartupAppUi::ConstructL(): End, Boot failed");
        return;//Execution stops here.
        }

    DoStartupStartPartL();
    TRACES("CStartupAppUi::ConstructL(): End");
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::~CStartupAppUi()
// ---------------------------------------------------------------------------
CStartupAppUi::~CStartupAppUi()
    {
    TRACES("CStartupAppUi::~CStartupAppUi()");

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if (iWelcomeAnimation)
        {
        RemoveFromStack( iWelcomeAnimation );
        delete iWelcomeAnimation;
        }

    if (iOperatorAnimation)
        {
        RemoveFromStack( iOperatorAnimation);
        delete iOperatorAnimation;
        }

    if (iUserWelcomeNote)
        {
        RemoveFromStack( iUserWelcomeNote );
        delete iUserWelcomeNote;
        iUserWelcomeNote = NULL;
        }

    if (iStartupPubSubObserver)
        {
        delete iStartupPubSubObserver;
        }
    if (iStartupMediatorObserver)
        {
        delete iStartupMediatorObserver;
        }
    if (iStartupTone)
        {
        delete iStartupTone;
        }
    if (iOpStartupTone)
        {
        delete iOpStartupTone;
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    if( iExitTimer )
        {
        iExitTimer->Cancel();
        delete iExitTimer;
        }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if( iAnimTimer )
        {
        iAnimTimer->Cancel();
        delete iAnimTimer;
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    if( iNoteTimer )
        {
        iNoteTimer->Cancel();
        delete iNoteTimer;
        }

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    delete iUserWelcomeNote;
    delete iAnimation;
    delete iStartupPubSubObserver;
    delete iStartupMediatorObserver;
    delete iMainView;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    FeatureManager::UnInitializeLib();

    TRACES("CStartupAppUi::~CStartupAppUi(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::PrepareToExit()
// ---------------------------------------------------------------------------
void CStartupAppUi::PrepareToExit()
    {
    TRACES("CStartupAppUi::PrepareToExit()");

#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    // Start the custom exit effect at boot time.
    // Note: Not allowed to call GfxTransEffect::EndFullScreen() as AVKON takes care of that when
    // EApplicationExit context is used!

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if ( !( iAnimation->WasCancelled() ) )
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    if( !iWelcomeAnimation->IsAnimationCancelled() )
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        {
        TRACES("CStartupAppUi::PrepareToExit(): Starting transition effect");

        GfxTransEffect::BeginFullScreen( AknTransEffect::EApplicationExit, TRect(),
            AknTransEffect::EParameterType,
            AknTransEffect::GfxTransParam( KUidStartUp, AknTransEffect::TParameter::EAllowAtBoot ) );
        }
#endif

    CEikAppUi::PrepareToExit();
#ifndef RD_BOOT_CUSTOMIZABLE_AI
    if( !iChargingOrAlarmBoot )
        {
        TRACES("CStartupAppUi::PrepareToExit(): NOT Charger/alarm boot");

        TRACES("CStartupAppUi::PrepareToExit(): Activate Idle app");
        TVwsViewId viewId = TVwsViewId(TUid::Uid(0x101FD64C), TUid::Uid(0x101FD64C));

        TUid customMessageUid = KNullUid;

        TRAPD(ignore, CreateActivateViewEventL(viewId, customMessageUid, KNullDesC8()));
        if (ignore != KErrNone)
            {
            TRACES1("CStartupAppUi::PrepareToExit(): CreateActivateViewEventL() leaves, err = %d", ignore );
            }
        }
#endif //RD_BOOT_CUSTOMIZABLE_AI
    TRACES("CStartupAppUi::PrepareToExit(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStopTimingL(TAny* aObject)
// ---------------------------------------------------------------------------
TInt CStartupAppUi::DoStopTimingL(TAny* aObject)
    {
    TRACES("CStartupAppUi::DoStopTimingL() +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ");
    TRACES1("CStartupAppUi::DoStopTimingL(): Object: %d", aObject);
    STATIC_CAST( CStartupAppUi*, aObject )->StopTimingL(); // cast, and call non-static function
    TRACES("CStartupAppUi::DoStopTimingL(): End ------------------------------------------------------ ");
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoExitApplication(TAny* aObject)
// ---------------------------------------------------------------------------
TInt CStartupAppUi::DoExitApplication(TAny* aObject)
    {
    TRACES("CStartupAppUi::DoExitApplication()");
    STATIC_CAST( CStartupAppUi*, aObject )->ExitApplication(); // cast, and call non-static function
    TRACES("CStartupAppUi::DoExitApplication(): End");
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::HandleKeyEventL
// ---------------------------------------------------------------------------
TKeyResponse CStartupAppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    TRACES3("CStartupAppUi::HandleKeyEventL(): code = %d, type = %d, repeats = %d",
        aKeyEvent.iCode, aType, aKeyEvent.iRepeats);

    TKeyResponse response( EKeyWasNotConsumed );
    if ( aType != EEventNull &&
         aKeyEvent.iCode == EKeyApplication0 && // Apps key
         ( iInternalState == EStartupShowingWelcomeAnimation ||
           iInternalState == EStartupShowingOperatorAnimation ||
           iInternalState == EStartupShowingUserWelcomeNote ) )
        {
        if ( aKeyEvent.iRepeats && iCleanBoot != EStartupCleanBoot)
            {
            // Long apps key press detected. Set clean boot.
            SetCleanBoot();
            }

        response = EKeyWasConsumed;
        }
    else if ( aType != EEventNull &&
              aKeyEvent.iCode != EStdKeyNull &&
              aType == EEventKeyDown )
        {
        TRACES("CStartupAppUi::HandleKeyEventL(): Event code is EEventKeyDown");

        if ( iOfflineModeQueryShown )
            {
            TRACES("CStartupAppUi::HandleKeyEventL(): First key event comes from Offline Mode Query");
            // first key event comes from Offline Mode Query
            iOfflineModeQueryShown = EFalse;
            response = EKeyWasConsumed;
            }
        else if ( iUserWelcomeNote )
            {
            TRACES("CStartupAppUi::HandleKeyEventL(): This key event is used to stop UserWelcomeAnimation");
            //this is used to stop User Welcome note showing
            StopTimingL();
            response = EKeyWasConsumed;
            }
        else if ( ( iInternalState == EStartupShowingWelcomeAnimation ||
                    iInternalState == EStartupShowingOperatorAnimation ) &&
                   !( iAnimation->WasCancelled() ) )
            {
            TRACES("CStartupAppUi::HandleKeyEventL(): This key event is used to cancel animations");

            CancelAnimation();
            response = EKeyWasConsumed;
            }
        }

    TRACES1("CStartupAppUi::HandleKeyEventL(): End, return %d", response);
    return response;
    }

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::HandleResourceChangeL
//
// ---------------------------------------------------------------------------
//
void CStartupAppUi::HandleResourceChangeL( TInt aType )
    {
    TRACES("CStartupAppUi::HandleResourceChangeL()");
    TRACES1("CStartupAppUi::HandleResourceChangeL Type: %d", aType);

    CAknAppUi::HandleResourceChangeL( aType );

    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        iMainView->SetRect( ApplicationRect() );
        }

    TRACES("CStartupAppUi::HandleResourceChangeL(): End");
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


// ---------------------------------------------------------------------------
// CStartupAppUi::HandleCommandL(TInt aCommand)
// ---------------------------------------------------------------------------
void CStartupAppUi::HandleCommandL(TInt aCommand)
    {
    TRACES("CStartupAppUi::HandleCommandL()");
    switch (aCommand)
        {
        case EEikCmdExit:
            {
            TRACES("CStartupAppUi::HandleCommandL(): EEikCmdExit");
            Exit();
            }
            break;
        default:
            break;
        }//End of switch-clause
    TRACES("CStartupAppUi::HandleCommandL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupStartPartL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupStartPartL()
    {
    TRACES("CStartupAppUi::DoStartupStartPartL()");
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    TryPreLoadAnimation();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    DoNextStartupPhaseL( EStartupWaitingCriticalBlock );
    TRACES("CStartupAppUi::DoStartupStartPartL(): End");
    }

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::TryPreLoadAnimation()
// ---------------------------------------------------------------------------
void CStartupAppUi::TryPreLoadAnimation()
    {
    TRACES("CStartupAppUi::TryPreLoadAnimation()");

    TInt state( ESwStateStartingCriticalApps );
    TInt err;
    err = RProperty::Get( KPSUidStartup, KPSGlobalSystemState, state ); // Use default if this fails
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::TryPreLoadAnimation(): KPSGlobalSystemState get err %d", err);
        }
    TInt mode( EStartupModeNormal );
    err = RProperty::Get( KPSUidStartup, KPSGlobalStartupMode, mode ); // Use default if this fails
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::TryPreLoadAnimation(): KPSGlobalStartupMode get err %d", err);
        }

    TRACES2("CStartupAppUi::TryPreLoadAnimation(): State: %d, mode %d", state, mode);

    if ( ( !HiddenReset() || iFirstBoot ) && // First boot or not-a-reset
         ( ( ( state == ESwStateStartingCriticalApps || state == ESwStateSelfTestOK ) &&
           mode == EStartupModeNormal ) || // Not going to charging or alarm mode
         state == ESwStateSecurityCheck ) )
        {
        iAnimation->PreLoad(
            ClientRect(),
            *iMainView,
            KStartupAnimationParams,
            ETrue,
            SecondaryDisplay::EStartWelcomeAnimation );
        }

    TRACES("CStartupAppUi::TryPreLoadAnimation(): End");
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupShowWelcomeAnimationL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupShowWelcomeAnimationL()
    {
    TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL()");
    if ( HiddenReset() )
        {
        //skip the welcome animation and user welcome note and continue with
        //the same way like in the end of ShowUserWelcomeNoteL()
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): Skip the animation and UWN because it's hidden reset");
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): and change internal state directly to EStartupFirstBootAndRTCCheck");
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        iWelcomeAnimation->RemoveSplashScreen();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        return;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    else
        {
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): Show animation");

        iAnimation->Play(
            ClientRect(),
            *iMainView,
            KStartupAnimationParams,
            ETrue,
            SecondaryDisplay::EStartWelcomeAnimation,
            TCallBack( AnimationFinishedFunc, this ) );
        }
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): Play startup tone.");

    // Play startup tone
    if (iStartupTone->Play() != KErrNone)
        {
        // Play startup beep.
        TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): Play startup beep");
        static_cast<CAknAppUi*>(iEikonEnv->
                                EikAppUi())->
                                KeySounds()->
                                PlaySound( EAvkonSIDPowerOnTone );
        }
    iWelcomeAnimation->SetAnimationShowing(ETrue);
    ShowWelcomeAnimationL();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupAppUi::DoStartupShowWelcomeAnimationL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupShowOperatorAnimationL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupShowOperatorAnimationL()
    {
    TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL()");
    if ( HiddenReset() )
        {
        //skip the operator animation and user welcome note and continue with
        //the same way like in the end of ShowUserWelcomeNoteL()
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): Skip the animation and UWN because it's hidden reset");
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): and change internal state directly to EStartupFirstBootAndRTCCheck");
        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        return;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    else
        {
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): Show animation");

        CSAnimStartupCtrl::TAnimationParams params;
        params.iRepositoryUid = KCRUidStartupConf;
        params.iAnimationPathKey = KStartupOperatorAnimationPath;
        params.iFrameDelayKey = KStartupOperatorAnimationFrameDelay;
        params.iEnableScalingKey = KStartupOperatorAnimationScaling;
        params.iTonePathKey = KStartupOperatorTonePath;
        params.iVolumeKey = KStartupOperatorToneVolume;
        iAnimation->Play(
            ClientRect(),
            *iMainView,
            params,
            EFalse,
            SecondaryDisplay::EStartOperatorAnimation,
            TCallBack( AnimationFinishedFunc, this ) );
        }
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    if ( iOperatorAnimation->ShowingTime() )
        {
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): Operator animation time defined properly");
        iOperatorAnimation->SetAnimationShowing(ETrue);
        iOpStartupTone->Play();
        ShowOperatorAnimationL();
        }
    else
        {
        TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): Operator animation not defined. Skip it.");
        iOperatorAnimation->SetAnimationShowing(EFalse);
        DoNextStartupPhaseL( EStartupShowingUserWelcomeNote );
        }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupAppUi::DoStartupShowOperatorAnimationL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupShowUserWelcomeNoteL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupShowUserWelcomeNoteL()
    {
    TRACES("CStartupAppUi::DoStartupShowUserWelcomeNoteL()");
    ShowUserWelcomeNoteL();
    TRACES("CStartupAppUi::DoStartupShowUserWelcomeNoteL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::StartupQueriesEnabled()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::StartupQueriesEnabled()
    {
    TRACES("CStartupAppUi::StartupQueriesEnabled()");

    TInt value( EStartupQueriesEnabled );
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KStartupQueries, value );
        }
    delete repository;

    TRACES1("CStartupAppUi::StartupQueriesEnabled(): returns %d", value);
    return value;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::PredictiveTimeEnabled()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::PredictiveTimeEnabled()
    {
    TRACES("CStartupAppUi::PredictiveTimeEnabled()");

    TInt value( EPredictiveTimeEnabled );
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KPredictiveTimeAndCountrySelection, value );
        }
    delete repository;

    TRACES1("CStartupAppUi::PredictiveTimeEnabled(): returns %d", value);
    return value;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupFirstBootAndRTCCheckL()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupFirstBootAndRTCCheckL()
    {
    TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL()");

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    RProperty::Set( KPSUidStartup, KStartupCleanBoot, iCleanBoot );
    RProperty::Set( KPSUidStartup, KPSSplashShutdown, ESplashShutdown );

    delete iUserWelcomeNote;
    iUserWelcomeNote = NULL;
    iMainView->DrawDeferred();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    // When Predictive Time and Country Selection is enabled, no queries are
    // shown to user during first boot. Instead, Clock application gets the
    // time and location from the network and marks the first boot as done.
    if( !PredictiveTimeEnabled() )
        {
        if( iFirstBoot && !HiddenReset() && StartupQueriesEnabled() )
            {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): First boot. Show city, time and date queries.");
    
    
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
            iWelcomeAnimation->UpdateDrawInfo( EStartupDICityTimeDateQueries );
            iWelcomeAnimation->DrawNow();
    
            if (iOperatorAnimation)
                {
                RemoveFromStack( iOperatorAnimation );
                delete iOperatorAnimation;
                iOperatorAnimation = NULL;
                }
            if (iUserWelcomeNote)
                {
                RemoveFromStack( iUserWelcomeNote );
                delete iUserWelcomeNote;
                iUserWelcomeNote = NULL;
                }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
            
            ShowStartupQueriesL();
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): Mark first boot");
            MarkFirstBoot();    
                
            }
        else if( !RTCStatus() && !HiddenReset() && StartupQueriesEnabled())
            {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): No first boot but RTCStatus is corrupted. Ask time and date");
    #ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
            iWelcomeAnimation->UpdateDrawInfo( EStartupDICityTimeDateQueries );
            if (iUserWelcomeNote)
                {
                RemoveFromStack( iUserWelcomeNote );
                delete iUserWelcomeNote;
                iUserWelcomeNote = NULL;
                }
    #endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    
            ShowStartupQueriesL(); // Not first boot, so skips Country/City query
            }
        if( iFirstBoot && !StartupQueriesEnabled() )
            {
            TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): First boot ongoing and queries are disabled.");
            MarkFirstBoot();
            }
        }

    TRACES("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): Setting KPSStartupAppState = EStartupAppStateFinished");
    TInt err = RProperty::Set( KPSUidStartupApp, KPSStartupAppState, EStartupAppStateFinished );
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::DoStartupFirstBootAndRTCCheckL(): KPSStartupAppState set err %d"
                , err);
        }

    DoNextStartupPhaseL( EStartupWaitingCUIStartupReady );
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowStartupQueriesL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowStartupQueriesL()
    {
    TRACES("CStartupAppUi::ShowStartupQueriesL()");

    TBool citysaved(EFalse);
    TBool timesaved(EFalse);
    TBool datesaved(EFalse);

    // Get default time ( to be used only in date query )
    GetDefaultTimeAndDate( iTime );

    // Show Country, Date and Time queries ( with possibility to go back ).
    // Country query is shown only in the first boot.

    while (!timesaved)
        {
        while (!datesaved)
            {
            while (!citysaved && iFirstBoot)
                {
                // 1. Select time zone
                ShowCountryAndCityListsL();
                citysaved = ETrue;
                TRACES1("CStartupAppUi::ShowStartupQueriesL(): citysaved = %d", citysaved );
                }
            // 2. Set date
            datesaved = ShowDateQueryL();
            TRACES1("CStartupAppUi::ShowStartupQueriesL(): datesaved = %d", datesaved );
            if (!datesaved)
                {
                citysaved = EFalse;
                }
            }
        // 3. Set time
        timesaved = ShowTimeQueryL();
        TRACES1("CStartupAppUi::ShowStartupQueriesL(): timesaved = %d", timesaved );
        if (!timesaved)
            {
            datesaved = EFalse;
            }
        }

    // All the queries completed.

    TRACES("CStartupAppUi::ShowStartupQueriesL() - END");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoStartupEndPart()
// ---------------------------------------------------------------------------
void CStartupAppUi::DoStartupEndPart()
    {
    TRACES("CStartupAppUi::DoStartupEndPart()");
    TRACES("CStartupAppUi::DoStartupEndPart(): STARTUP OK");

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    RProperty::Set( KPSUidStartup, KStartupCleanBoot, iCleanBoot );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION    
    UpdateStartupUiPhase( EStartupUiPhaseAllDone );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupAppUi::DoStartupEndPart(): Exit application.");
    iExitTimer->Start( 100000, 100000, TCallBack( DoExitApplication, this ) );
    TRACES("CStartupAppUi::DoStartupEndPart(): DoExitApplication-timer called.");
    TRACES("CStartupAppUi::DoStartupEndPart(): End");
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::ContinueStartupAfterToneL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ContinueStartupAfterToneL(TToneType aToneType)
    {
    TRACES("CStartupAppUi::ContinueStartupAfterToneL()");

    if (aToneType == EStartupTone)
        {
        TRACES("CStartupAppUi::ContinueStartupAfterToneL(): Tone type EStartupTone");
        DoNextStartupPhaseL( EStartupWaitingCUIOperatorAnim );
        }
    else if (aToneType == EStartupOpTone)
        {
        TRACES("CStartupAppUi::ContinueStartupAfterToneL(): Tone type EStartupOpTone");
        DoNextStartupPhaseL( EStartupShowingUserWelcomeNote );
        }
    else
        {
        TRACES("CStartupAppUi::ContinueStartupAfterToneL(): Tone interrupted");
        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
        }
    TRACES("CStartupAppUi::ContinueStartupAfterToneL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::BringToForeground()
// ---------------------------------------------------------------------------
void CStartupAppUi::BringToForeground()
    {
    TRACES("CStartupAppUi::BringToForeground()");
    if ((iInternalState != EStartupWaitingTouchScreenCalib) ||
        (iTouchScreenCalibrationDone))
        {
        TRACES("CStartupAppUi::BringToForeground(): Bring to foreground");
        TApaTask self(iCoeEnv->WsSession());
        self.SetWgId(iCoeEnv->RootWin().Identifier());
        self.BringToForeground();
        }
    TRACES("CStartupAppUi::BringToForeground(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SendToBackground()
// ---------------------------------------------------------------------------
void CStartupAppUi::SendToBackground()
    {
    TRACES("CStartupAppUi::SendToBackground()");
    TApaTask self(iCoeEnv->WsSession());
    self.SetWgId(iCoeEnv->RootWin().Identifier());
    self.SendToBackground();
    TRACES("CStartupAppUi::SendToBackground(): End");
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupAppUi::StopTimingL()
// ---------------------------------------------------------------------------
void CStartupAppUi::StopTimingL()
    {
    TRACES("CStartupAppUi::StopTimingL()");
    if( !iStartupFirstBootAndRTCCheckAlreadyCalled )
        {
        if (iInternalState == EStartupShowingUserWelcomeNote)
            {
            //telling that timer has expired in appui so that
            //key pressed are not handled any more...
            TRACES("CStartupAppUi::StopTimingL(): Stopping UWN");
            iStartupFirstBootAndRTCCheckAlreadyCalled = ETrue;
            iNoteTimer->Cancel();
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
            UpdateStartupUiPhase( EStartupUiPhaseUserWelcomeDone );
            DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
       iUserWelcomeNote->CancelNoteCancelTimer();
            iWelcomeAnimation->UpdateDrawInfo( EStartupDIWelcomeNoteEnd );
            iWelcomeAnimation->DrawNow();
            iUserWelcomeNote->SetUserWelcomeNoteShowing(EFalse);
            TRACES("CStartupAppUi::StopTimingL(): UWN stopped");

            if (iStartupTone->Playing())
                {
                TRACES("CStartupAppUi::StopTimingL(): Startup tone playing. Cannot continue to next phase");
                iStartupTone->StartupWaiting(ETrue);
                }
            else
                {
                DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
                }
            }
        else if (iInternalState == EStartupShowingOperatorAnimation)  // EStartupShowingOperatorAnimation
            {
            TRACES("CStartupAppUi::StopTimingL(): Stopping animation");
            iAnimTimer->Cancel();
            iWelcomeAnimation->CancelAnimCancelTimer();
            iOperatorAnimation->UpdateDrawInfo( EStartupDIOperatorAnimEnd );
            iOperatorAnimation->SetAnimationShowing(EFalse);
            TRACES("CStartupAppUi::StopTimingL(): operator animation showing stopped");
            if ( iOperatorAnimation->IsAnimationCancelled())
                {
                TRACES("CStartupAppUi::StopTimingL(): Animation is cancelled by user and therefore UWN is not shown");
                StopOperatorTone();
                iStartupFirstBootAndRTCCheckAlreadyCalled = ETrue;
                DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
                }
            else
                {
                // If tone is still playing wait until it completes.
                if (iOpStartupTone->Playing())
                    {
                    TRACES("CStartupAppUi::StopTimingL(): Operator startup tone is still playing. Wait until it completes.");
                    iOpStartupTone->StartupWaiting(ETrue);
                    }
                else
                    {
                    TRACES("CStartupAppUi::StopTimingL(): Lets display UWN");
                    DoNextStartupPhaseL( EStartupShowingUserWelcomeNote );
                    }
                }
            TRACES("CStartupAppUi::StopTimingL(): Operator Animation stopped");
            }
        else // EStartupShowingWelcomeAnimation
            {
            TRACES("CStartupAppUi::StopTimingL(): Stopping animation");
            iAnimTimer->Cancel();
            iWelcomeAnimation->CancelAnimCancelTimer();
            iWelcomeAnimation->UpdateDrawInfo( EStartupDIWelcomeAnimEnd );
            iWelcomeAnimation->SetAnimationShowing(EFalse);
            TRACES("CStartupAppUi::StopTimingL(): Welcome animation showing stopped");

            if ( iWelcomeAnimation->IsAnimationCancelled())
                {
                TRACES("CStartupAppUi::StopTimingL(): Animation is cancelled by user and therefore operator animation and UWN is not shown");
                StopStartupTone();
                iStartupFirstBootAndRTCCheckAlreadyCalled = ETrue;
                DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
                }
            else
                {
                if (iStartupTone->Playing())
                    {
                    // If tone is still playing wait until it completes.
                    iStartupTone->StartupWaiting(ETrue);
                    }
                else
                    {
                    DoNextStartupPhaseL( EStartupWaitingCUIOperatorAnim );
                    }
                }
            TRACES("CStartupAppUi::StopTimingL(): Animation stopped");
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
            }
        }

    TRACES("CStartupAppUi::StopTimingL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ExitApplication()
// ---------------------------------------------------------------------------
void CStartupAppUi::ExitApplication()
    {
    TRACES("CStartupAppUi::ExitApplication()");
    iExitTimer->Cancel();
    Exit();
    TRACES("CStartupAppUi::ExitApplication(): End");
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::ShowWelcomeAnimationL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowWelcomeAnimationL()
    {
    TRACES("CStartupAppUi::ShowWelcomeAnimationL()");
    __ASSERT_DEBUG( iWelcomeAnimation , PANIC( EStartupPanicClassMemberVariableIsNull ) );
    TInt showtime = iWelcomeAnimation->ShowingTime();
    iAnimation = ETrue;
    TRACES("CStartupAppUi::ShowWelcomeAnimationL(): Animation timer started");
    iAnimTimer->Start(
            showtime*KOneMilliSecondInMicroSeconds,
            showtime*KOneMilliSecondInMicroSeconds,
            TCallBack( DoStopTimingL, this ) );
    iWelcomeAnimation->StartL();
    TRACES("CStartupAppUi::ShowWelcomeAnimationL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowOperatorAnimationL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowOperatorAnimationL()
    {
    TRACES("CStartupAppUi::ShowOperatorAnimationL()");
    __ASSERT_DEBUG( iOperatorAnimation , PANIC( EStartupPanicClassMemberVariableIsNull ) );
    TInt showtime = iOperatorAnimation->ShowingTime();
    iAnimation = ETrue;
    TRACES("CStartupAppUi::ShowWelcomeAnimationL(): Operator Animation timer started");
    iAnimTimer->Start(
        showtime*KOneMilliSecondInMicroSeconds,
        showtime*KOneMilliSecondInMicroSeconds,
        TCallBack( DoStopTimingL, this ) );
    iOperatorAnimation->StartL();
    TRACES("CStartupAppUi::ShowOperatorAnimationL(): End");
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowUserWelcomeNoteL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowUserWelcomeNoteL()
    {
    TRACES("CStartupAppUi::ShowUserWelcomeNoteL()");
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iUserWelcomeNote = CStartupUserWelcomeNote::NewL( *this, ClientRect(), *iMainView );
    TStartupNoteTypeInformation type = iUserWelcomeNote->NoteTypeInformation();
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    __ASSERT_DEBUG( iUserWelcomeNote , PANIC( EStartupPanicClassMemberVariableIsNull ) );
    TStartupNoteTypeInformation type;
    type = iUserWelcomeNote->NoteTypeInformation();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    if( type == EStartupImage )
        {
        // UserWelcomeNote type is EStartupImage
        // This type of note is shown fixed (KUserWelcomeNoteShowPeriodTime) time
        TRACES("CStartupAppUi::ShowUserWelcomeNoteL(): UWNTimer started (graphic)");
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        iAnimation = EFalse;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        iNoteTimer->Start(
            KUserWelcomeNoteShowPeriodTime,
            KUserWelcomeNoteShowPeriodTime,
            TCallBack( DoStopTimingL, this ) );
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        iUserWelcomeNote->StartL();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    else if ( type == EStartupText )
        {
        TRACES("CStartupAppUi::ShowUserWelcomeNoteL(): Text UWN");

        iUserWelcomeNote->StartL();
        
        UpdateStartupUiPhase( EStartupUiPhaseUserWelcomeDone );
                
        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
        }
    else
        {
        TRACES("CStartupAppUi::ShowUserWelcomeNoteL(): No UWN");

        DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck );
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    else
        {
        //if User Welcome Note type is ETextWelcomeNote nothing to do here,
        //because it is implemented with Avkon globalnote
        //or if type is EDefaultWelcomeNote no User Welcome Note is shown.
        TRACES("CStartupAppUi::ShowUserWelcomeNoteL(): No UWN to show or UWN is text");
        }
    //invoke welcome note container to show note
    iUserWelcomeNote->StartL();

    if( type == EStartupText || type == EStartupNoNote)
        {
        //this is called already here because timer not activated in text uwn case
        //and so DoStopTimingL() is never called and should be called here.
        StopTimingL();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }

    TRACES("CStartupAppUi::ShowUserWelcomeNoteL(): End");
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingTouchScreenCalibL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingTouchScreenCalibL()
    {
    TRACES("CStartupAppUi::WaitingTouchScreenCalibL()");
#ifdef RD_SCALABLE_UI_V2

    if( iFirstBoot && iTouchScreenCalibSupport )
        {
        if (iTouchScreenCalibrationDone)
            {
            TRACES("CStartupAppUi::WaitingTouchScreenCalibL(): Calibration already done. Continue boot up");
            DoNextStartupPhaseL( EStartupOfflineModeQuery );
            }
        else
            {
            SendToBackground();

            iWelcomeAnimation->UpdateDrawInfo( EStartupDITouchScreenCalib );
            iWelcomeAnimation->DrawNow();
            TRACES("CStartupAppUi::WaitingTouchScreenCalibL(): Startup sequence halted until Touch Screen Calibration is done");
            }
        }
    else
        {
        TRACES("CStartupAppUi::WaitingTouchScreenCalibL(): Not first boot or calibration not supported. Continue boot up");
        DoNextStartupPhaseL( EStartupOfflineModeQuery );
        }

#else // !RD_SCALABLE_UI_V2
    TRACES("CStartupAppUi::WaitingTouchScreenCalibL(): Calibration not supported. Continue boot up");
    DoNextStartupPhaseL( EStartupOfflineModeQuery );

#endif // RD_SCALABLE_UI_V2
    TRACES("CStartupAppUi::WaitingTouchScreenCalibL(): End");
    }

#ifdef RD_SCALABLE_UI_V2
// ---------------------------------------------------------------------------
// CStartupAppUi::TouchScreenCalibrationDoneL()
// ---------------------------------------------------------------------------
void CStartupAppUi::TouchScreenCalibrationDoneL()
    {
    TRACES("CStartupAppUi::TouchScreenCalibrationDoneL()");
    if (iInternalState == EStartupWaitingTouchScreenCalib)
        {
        iTouchScreenCalibrationDone = ETrue;
        BringToForeground();
        DoNextStartupPhaseL( EStartupOfflineModeQuery );
        }
    else
        {
        iTouchScreenCalibrationDone = ETrue;
        }
    TRACES("CStartupAppUi::TouchScreenCalibrationDoneL(): End");
    }
#endif // RD_SCALABLE_UI_V2
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::CoverUIWelcomeAnimationSyncOKL()
// ---------------------------------------------------------------------------
void CStartupAppUi::CoverUIWelcomeAnimationSyncOKL()
    {
    TRACES("CStartupAppUi::CoverUIWelcomeAnimationSyncOKL()");
    DoNextStartupPhaseL( EStartupWaitingStartupTone );
    TRACES("CStartupAppUi::CoverUIWelcomeAnimationSyncOKL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingCoverUIWelcomeAnimationSyncL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingCoverUIWelcomeAnimationSyncL()
    {
    TRACES("CStartupAppUi::WaitingCoverUIWelcomeAnimationSyncL()");
    if (iCoverUISupported)
        {
        iStartupMediatorObserver->IssueCommand(SecondaryDisplay::ECmdStartupSync,
                                               SecondaryDisplay::EStartWelcomeAnimation);
        }
    else
        {
        DoNextStartupPhaseL( EStartupWaitingStartupTone );
        }
    TRACES("CStartupAppUi::WaitingCoverUIWelcomeAnimationSyncL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::CoverUIOperatorAnimationSyncOKL()
// ---------------------------------------------------------------------------
void CStartupAppUi::CoverUIOperatorAnimationSyncOKL()
    {
    TRACES("CStartupAppUi::CoverUIOperatorAnimationSyncOKL()");
    DoNextStartupPhaseL( EStartupShowingOperatorAnimation );
    TRACES("CStartupAppUi::CoverUIOperatorAnimationSyncOKL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingCoverUIOperatorAnimationSyncL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingCoverUIOperatorAnimationSyncL()
    {
    TRACES("CStartupAppUi::WaitingCoverUIOperatorAnimationSyncL()");
    if (iCoverUISupported)
        {
        if (iOperatorAnimation->ShowingTime())
            {
            iStartupMediatorObserver->IssueCommand(SecondaryDisplay::ECmdStartupSync,
                                                   SecondaryDisplay::EStartOperatorAnimation );
            }
        else
            {
            DoNextStartupPhaseL( EStartupShowingOperatorAnimation );
            }
        }
    else
        {
        DoNextStartupPhaseL( EStartupShowingOperatorAnimation );
        }
    TRACES("CStartupAppUi::WaitingCoverUIOperatorAnimationSyncL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingStartupToneL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingStartupToneL()
    {
    if( iStartupTone->ToneFound() && !iStartupTone->AudioReady() )
        {
        TRACES("CStartupAppUi::WaitingStartupToneL(): Startup tone found but not ready. Waiting tone to init");
        iToneInitTimer = CPeriodic::NewL( EPriorityNormal );
        iToneInitTimer->Start( KOneMilliSecondInMicroSeconds,
                               KOneMilliSecondInMicroSeconds,
                               TCallBack( ToneInitTimerTimeoutL, this ) );
        }
    else
        {
        TRACES("CStartupAppUi::WaitingStartupToneL(): Audio ready");
        DoNextStartupPhaseL( EStartupShowingWelcomeAnimation );
        }
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ToneInitTimerTimeoutL()
// ---------------------------------------------------------------------------
TInt CStartupAppUi::ToneInitTimerTimeoutL(TAny* aObject)
    {
    STATIC_CAST( CStartupAppUi*, aObject )->StartupToneWaitStatusL(); // cast, and call non-static function
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::StartupToneWaitStatusL()
// ---------------------------------------------------------------------------
void CStartupAppUi::StartupToneWaitStatusL()
    {
    iToneInitWaitTime++;
    TRACES1("CStartupAppUi::StartupToneWaitStatusL(): Total tone init wait time = %d ms", iToneInitWaitTime );
    TBool audioReady = iStartupTone->AudioReady();
    if ( audioReady || (iToneInitWaitTime>=KMaxToneInitWait) )
        {
        iToneInitTimer->Cancel();
        delete iToneInitTimer;
        iToneInitTimer = NULL;

        TRACES1("CStartupAppUi::StartupToneWaitStatusL(): AudioReady: %d, proceed", audioReady );
        DoNextStartupPhaseL( EStartupShowingWelcomeAnimation );
        }
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupAppUi::CoverUIStartupReadySyncOKL()
// ---------------------------------------------------------------------------
void CStartupAppUi::CoverUIStartupReadySyncOKL()
    {
    TRACES("CStartupAppUi::CoverUIStartupReadySyncOKL()");
    DoNextStartupPhaseL( EStartupStartupOK );
    TRACES("CStartupAppUi::CoverUIStartupReadySyncOKL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingCoverUIStartupReadySyncL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingCoverUIStartupReadySyncL()
    {
    TRACES("CStartupAppUi::WaitingCoverUIStartupReadySyncL()");
    if (iCoverUISupported)
        {
        iStartupMediatorObserver->IssueCommand(SecondaryDisplay::ECmdStartupSync,
                                               SecondaryDisplay::EStartStartupReady);
        }
    else
        {
        DoNextStartupPhaseL( EStartupStartupOK );
        }
    TRACES("CStartupAppUi::WaitingCoverUIStartupReadySyncL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::RaiseCoverUIEvent()
// ---------------------------------------------------------------------------
void CStartupAppUi::RaiseCoverUIEvent( TUid aCategory,
                                       TInt aEventId,
                                       const TDesC8& aData )
    {
    TRACES("CStartupAppUi::RaiseCoverUIEvent()");
    if (iCoverUISupported)
        {
        iStartupMediatorObserver->RaiseEvent( aCategory,
                                              aEventId,
                                              aData );
        }
    TRACES("CStartupAppUi::RaiseCoverUIEvent(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetCriticalBlockEndedL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetCriticalBlockEndedL()
    {
    TRACES("CStartupAppUi::SetCriticalBlockEndedL()");
    iCriticalBlockEnded = ETrue;
    if ( iInternalState == EStartupWaitingCriticalBlock )
        {
        WaitingCriticalBlockEndingL();
        }
    TRACES("CStartupAppUi::SetCriticalBlockEndedL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingCriticalBlockEndingL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingCriticalBlockEndingL()
    {
    TRACES("CStartupAppUi::WaitingCriticalBlockEndingL()");

    if( iCriticalBlockEnded )
        {
        TRACES("CStartupAppUi::WaitingCriticalBlockEndingL(): CriticalBlock has ended. Continue.");
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        DoNextStartupPhaseL( EStartupOfflineModeQuery );
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
        DoNextStartupPhaseL( EStartupWaitingTouchScreenCalib );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }
    TRACES("CStartupAppUi::WaitingCriticalBlockEndingL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetWaitingStartupAnimationStartL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetWaitingStartupAnimationStartL()
    {
    TRACES("CStartupAppUi::SetWaitingStartupAnimationStartL()");
    iStartupWaitingShowStartupAnimation = ETrue;
    if ( iInternalState == EStartupWaitingShowStartupAnimation )
        {
        WaitingStartupAnimationStartL();
        }
    TRACES("CStartupAppUi::SetWaitingStartupAnimationStartL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::WaitingStartupAnimationStartL()
// ---------------------------------------------------------------------------
void CStartupAppUi::WaitingStartupAnimationStartL()
    {
    TRACES("CStartupAppUi::WaitingStartupAnimationStartL()");

    if( iStartupWaitingShowStartupAnimation )
        {
        TRACES("CStartupAppUi::WaitingStartupAnimationStartL(): Continue.");
        DoNextStartupPhaseL( EStartupShowingWelcomeAnimation );
        }
    TRACES("CStartupAppUi::WaitingStartupAnimationStartL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetEmergencyCallsOnlyL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetEmergencyCallsOnlyL()
    {
    TRACES("CStartupAppUi::SetEmergencyCallsOnlyL()");
    iCriticalBlockEnded = ETrue;
    if ( iInternalState == EStartupWaitingCriticalBlock )
        {
        TRACES("CStartupAppUi::SetEmergencyCallsOnlyL(): Entered emergency calls only state.");

        DoNextStartupPhaseL( EStartupWaitingCUIStartupReady );
        }
    TRACES("CStartupAppUi::SetEmergencyCallsOnlyL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SwStateFatalStartupErrorL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SwStateFatalStartupErrorL(TBool aPropertyChanged)
    {
    TRACES("CStartupAppUi::SwStateFatalStartupErrorL()");

    iSwStateFatalStartupError = ETrue;

    if ( aPropertyChanged )
        {
        TRACES("CStartupAppUi::SwStateFatalStartupErrorL(): property changed");
        DoNextStartupPhaseL( EStartupSystemFatalError );
        }

    TRACES("CStartupAppUi::SwStateFatalStartupErrorL(): End");
    }


#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::GetOfflineModeQueryShown()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::GetOfflineModeQueryShown()
    {
    TRACES1("CStartupAppUi::GetOfflineModeQueryShown(): iOfflineModeQueryShown == %d ", iOfflineModeQueryShown );
    return iOfflineModeQueryShown;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SetOfflineModeQueryShown()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetOfflineModeQueryShown(TBool aValue)
    {
    TRACES1("CStartupAppUi::SetOfflineModeQueryShown(): iOfflineModeQueryShown == %d ", iOfflineModeQueryShown );
    iOfflineModeQueryShown = aValue;
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ----------------------------------------------------------------------------
// CStartAppUi::DosInOfflineModeL()
// ----------------------------------------------------------------------------

TBool CStartupAppUi::DosInOfflineModeL()
    {
    TRACES("CStartupAppUi::DosInOfflineModeL()");
    if ( iOfflineModeSupported )
        {
        TRACES1("CStartupAppUi::DosInOfflineModeL(): iOfflineModeSupported == %d", iOfflineModeSupported );
        return UiInOfflineMode();
        }
    else
        {
        TRACES("CStartupAppUi::DosInOfflineModeL(): return EFalse");
        return EFalse; // device that does not support Off-line Mode can never be in Off-line Mode.
        }
    }


// ----------------------------------------------------------------------------
// CStartupAppUi::UiInOfflineMode()
// ----------------------------------------------------------------------------

TBool CStartupAppUi::UiInOfflineMode()
    {
    TRACES("CStartupAppUi::UiInOfflineMode()");
    if ( iOfflineModeSupported )
        {
        TInt networkConnectionAllowed ( 0 );

        TRACES("CStartupAppUi::UiInOfflineMode(): Check Offline status from CenRep");
        CRepository* repository(NULL);

        TRAPD( err, repository = CRepository::NewL( KCRUidCoreApplicationUIs ) );
        if ( err == KErrNone )
            {
            err = repository->Get( KCoreAppUIsNetworkConnectionAllowed, networkConnectionAllowed );
            }

        #ifdef _DEBUG
            if ( err != KErrNone )
                TRACES1("CStartupAppUi::UiInOfflineMode(): Central repository access failed, error code %d.", err );
            else
                TRACES1("CStartupAppUi::UiInOfflineMode(): Offline: network connection allowed: %d", networkConnectionAllowed);
        #endif
        delete repository;

        TRACES1("CStartupAppUi::UiInOfflineMode():  returns %d", !( TBool ) networkConnectionAllowed);

        return ( ! ( TBool ) networkConnectionAllowed );
        }
    else
        {
        TRACES("CStartupAppUi::UiInOfflineMode(): DosInOfflineMode returns EFalse");
        return EFalse; // device that does not support Off-line Mode can never be in Off-line Mode.
        }
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowOfflineModeQueryL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowOfflineModeQueryL()
    {
    TRACES("CStartupAppUi::ShowOfflineModeQueryL()");

    TInt status( EStartupSimInitWait );
    RProperty::Get(KPSUidStartup, KPSSimStatus, status);

    TInt reply ( 0 );
    if (status == ESimUsable || status == ESimReadable || status == ESimNotReady || !iSimSupported)
        {
        TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Show offline mode query if needed");

        if ( HiddenReset() && DosInOfflineModeL() && !SimStatusChangedReset() )
            {
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode: Hidden reset and DOS in offline mode -> Boot to Offline");
            reply = 1;
            }
        else if ( iOfflineModeSupported && DosInOfflineModeL() )
            {
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline mode query needed");
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
            RProperty::Set( KPSUidStartup, KPSSplashShutdown, ESplashShutdown );
            iAnimation->BringToForeground();
            iMainView->DrawDeferred();
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
            iWelcomeAnimation->UpdateDrawInfo( EStartupDIQueriesOn );
            iWelcomeAnimation->DrawNow();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
            iOfflineModeQueryShown = ETrue;
            CAknQueryDialog* dlg = new (ELeave) CAknQueryDialog( CAknQueryDialog::ENoTone );
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Publish dialog for Secondary UI");
            dlg->PublishDialogL(SecondaryDisplay::ECmdShowOfflineQuery,
                                SecondaryDisplay::KCatStartup);
            if ( dlg->ExecuteLD( R_STARTUP_OFFLINE_MODE_QUERY ) )
                {
                TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode query: YES -> Boot to Offline");
                reply = 1;
                }
            else
                {
                TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode query: NO -> Boot to Online");
                reply = 0;
                }
            }
        else
            {
            TRACES("CStartupAppUi::ShowOfflineModeQueryL(): Offline Mode: DOS in online mode -> Boot to Online");
            reply = 0;
            }

        }
#ifdef _DEBUG
    else
        {
        TRACES("CStartupAppUi::ShowOfflineModeQueryL(): SIM card removed. No offline mode query.");
        }
#endif

    TRACES1("CStartupAppUi::ShowOfflineModeQueryL(): BootIntoOffline=%d.", reply);
    TInt err = RProperty::Set( KPSUidStartup,
                               KStartupBootIntoOffline,
                               reply ? EBootIntoOfflineMode : EBootIntoOnlineMode );

    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::ShowOfflineModeQueryL(): KStartupBootIntoOffline set err %d", err);
        }

    TRACES("CStartupAppUi::ShowOfflineModeQueryL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::CancelAnimation(...)
// ---------------------------------------------------------------------------
void CStartupAppUi::CancelAnimation()
    {
    TRACES("CStartupAppUi::CancelAnimation()");
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iAnimation->Cancel();
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iWelcomeAnimation->CancelAnimation();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    TRACES("CStartupAppUi::CancelAnimation(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowCountryAndCityListsL()
// ---------------------------------------------------------------------------
void CStartupAppUi::ShowCountryAndCityListsL()
    {
    TRACES("CStartupAppUi::ShowCountryAndCityListsL()");

    TInt cityselected( EFalse );
    while ( !cityselected )
        {
        TRACES1("CStartupAppUi::ShowCountryAndCityListsL(): City item to focus: %d", iCounryListIndex);
        TInt cityGroupId = ShowCountryListL();
        TRACES1("CStartupAppUi::ShowCountryAndCityListsL(): City group id: %d", cityGroupId);
        if ( cityGroupId != KErrCancel )
            {
            cityselected = ShowCityListL(cityGroupId);
            }
        else
            {
            cityselected = ETrue;
            }
        }
    TRACES("CStartupAppUi::ShowCountryAndCityListsL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowCountryListL()
// ---------------------------------------------------------------------------
TInt CStartupAppUi::ShowCountryListL()
    {
    TRACES("CStartupAppUi::ShowCountryListL()");

    CAknSinglePopupMenuStyleListBox* listBox =
        new(ELeave) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL(listBox);

    CStartupPopupList* popupList = CStartupPopupList::NewL(listBox, R_AVKON_SOFTKEYS_SELECT_CANCEL__SELECT,
                                                           AknPopupLayouts::EMenuGraphicHeadingWindow );

    CleanupStack::PushL(popupList);

    listBox->ConstructL(popupList, EAknListBoxSelectionList | EAknListBoxScrollBarSizeExcluded);
    listBox->CreateScrollBarFrameL( ETrue );
    listBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
                                                        CEikScrollBarFrame::EAuto );

    listBox->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );

    CDesCArrayFlat *items = new(ELeave)CDesCArrayFlat(1);

    CleanupStack::PushL(items);

    CTzLocalizer* tzLocalizer = CTzLocalizer::NewL();
    CleanupStack::PushL(tzLocalizer);

    CTzLocalizedCityGroupArray* countryList;
    countryList = tzLocalizer->GetAllCityGroupsL(CTzLocalizer::ETzAlphaNameAscending);
    CleanupStack::PushL(countryList);

    TRACES("CStartupAppUi::ShowCountryListL(): Create list of cities");
    for(TInt i = 0; i <countryList->Count(); i++)
        {
        CTzLocalizedCityGroup& cityGroup = countryList->At(i);

        // Check if the country name is blank.
        // If it is blank, ignore it. Empty name shouldn't be shown in the list.
        if(cityGroup.Name().Compare(KEmpty) != 0)
            {
            TBuf<KMaxCountryLength> countryitem;
            countryitem.Insert(0,cityGroup.Name());
            TRACES1("CStartupAppUi::ShowCountryListL(): Create country to list: %S", &countryitem);
            items->AppendL(countryitem);
            }
        }

    CleanupStack::PopAndDestroy( countryList );

    CTextListBoxModel* model=listBox->Model();
    model->SetItemTextArray(items);
    model->SetOwnershipType(ELbmOwnsItemArray);

    TRACES("CStartupAppUi::ShowCountryListL(): Set title");
    // Set title
    HBufC* title = StringLoader::LoadLC( R_QTN_SU_SELECT_COUNTRY );
    popupList->SetTitleL(title->Des());
    CleanupStack::PopAndDestroy( title );

    popupList->EnableAdaptiveFind();
    listBox->SetCurrentItemIndex(iCounryListIndex);

    TInt cityGroupId;

    if (iCoverUISupported)
        {
        TRACES("CStartupAppUi::ShowCountryListL(): Publish country list for Secondary UI");
        TPckgBuf<TInt> data( SecondaryDisplay::EShowCountryQuery );
        iStartupMediatorObserver->RaiseEvent( SecondaryDisplay::KCatStartup,
                                              SecondaryDisplay::EMsgStartupEvent,
                                              data );
        }

    TRACES("CStartupAppUi::ShowCountryListL(): Show the list");
    if (popupList->ExecuteLD())
        {
        iCounryListIndex = listBox->CurrentItemIndex();
        TRACES1("CStartupAppUi::ShowCountryListL(): CurrentItemIndex: %d", iCounryListIndex);
        TPtrC countryName = listBox->Model()->ItemText(iCounryListIndex);

        CTzLocalizedCityGroup* tzLocalizedCityGroup = tzLocalizer->FindCityGroupByNameL(countryName);
        CleanupStack::PushL(tzLocalizedCityGroup);

        cityGroupId = tzLocalizedCityGroup->Id();
        CleanupStack::PopAndDestroy( tzLocalizedCityGroup );

        TRACES1("CStartupAppUi::ShowCountryListL(): Selected country %S", &countryName);
        }
    else
        {
        TRACES("CStartupAppUi::ShowCountryListL(): Country list cancelled");
        cityGroupId = KErrCancel;
        }

    CleanupStack::PopAndDestroy( tzLocalizer );
    CleanupStack::Pop( items );
    CleanupStack::Pop( popupList );
    CleanupStack::PopAndDestroy( listBox ); 

    TRACES1("CStartupAppUi::ShowCountryListL(): End. Return city group id: %d", cityGroupId);
    return cityGroupId;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowCityListL()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::ShowCityListL(TUint8 cityGroupId)
    {
    TRACES("CStartupAppUi::ShowCityListL()");

    TBool retval( ETrue );

    CTzLocalizer* tzLocalizer = CTzLocalizer::NewL();
    CleanupStack::PushL(tzLocalizer);

    CTzLocalizedCityArray* cityList;

    TRACES1("CStartupAppUi::ShowCityListL(): Create list of cities in group %d", cityGroupId);

    cityList = tzLocalizer->GetCitiesInGroupL(cityGroupId,//cityGroup.Id(),
                                                  CTzLocalizer::ETzAlphaNameAscending );
    CleanupStack::PushL(cityList);

    if ( cityList->Count() == 1 )
        {
        TRACES("CStartupAppUi::ShowCityListL(): Only one city in citygroup. This can be selected automatically.");

        CTzLocalizedCity& city = cityList->At(0);

        CTzLocalizedCity* tzLocalizedCity = tzLocalizer->FindCityByNameL(city.Name());
        CleanupStack::PushL(tzLocalizedCity);

        TInt timeZoneId = tzLocalizedCity->TimeZoneId();

        tzLocalizer->SetTimeZoneL(timeZoneId);
        tzLocalizer->SetFrequentlyUsedZoneL(*tzLocalizedCity, CTzLocalizedTimeZone::ECurrentZone);

        CleanupStack::PopAndDestroy( tzLocalizedCity );
        CleanupStack::PopAndDestroy( cityList );
        CleanupStack::PopAndDestroy( tzLocalizer );

        TRACES1("CStartupAppUi::ShowCityListL(): End, returns %d", retval);
        return retval;
        }

    CAknSinglePopupMenuStyleListBox* listBox =
        new(ELeave) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL(listBox);

    CStartupPopupList* popupList = CStartupPopupList::NewL(listBox, R_AVKON_SOFTKEYS_SELECT_CANCEL__SELECT,
                                                           AknPopupLayouts::EMenuGraphicHeadingWindow );

    CleanupStack::PushL(popupList);

    CDesCArrayFlat *items = new(ELeave)CDesCArrayFlat(1);

    CleanupStack::PushL(items);

    for(TInt j = 0; j < cityList->Count(); j++)
        {
        CTzLocalizedCity& city = cityList->At(j);

        // Check if the city name is blank.
        // If it is blank, ignore it. Empty name shouldn't be shown in the list.
        if(city.Name().Compare(KEmpty) != 0)
            {
            TBuf<KMaxCityLength> homecityitem;
            homecityitem.Insert(0,city.Name());
            TRACES1("CStartupAppUi::ShowCityListL(): Create to list: %S", &homecityitem);
            items->AppendL(homecityitem);
            }
        }

    listBox->ConstructL(popupList, EAknListBoxSelectionList | EAknListBoxScrollBarSizeExcluded);
    listBox->CreateScrollBarFrameL( ETrue );
    listBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
                                                        CEikScrollBarFrame::EAuto );

    listBox->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );

    CTextListBoxModel* model=listBox->Model();
    model->SetItemTextArray(items);
    model->SetOwnershipType(ELbmOwnsItemArray);

    TRACES("CStartupAppUi::ShowCityListL(): Set title");
    // Set title
    HBufC* title = StringLoader::LoadLC( R_QTN_SU_SELECT_CITY );
    popupList->SetTitleL(title->Des());
    CleanupStack::PopAndDestroy(title);

    popupList->EnableAdaptiveFind();

    if (iCoverUISupported)
        {
        TRACES("CStartupAppUi::ShowCountryListL(): Publish city list for Secondary UI");
        TPckgBuf<TInt> data( SecondaryDisplay::EShowCityQuery );
        iStartupMediatorObserver->RaiseEvent( SecondaryDisplay::KCatStartup,
                                              SecondaryDisplay::EMsgStartupEvent,
                                              data );
        }

    TRACES("CStartupAppUi::ShowCityListL(): Show the list");
    if (popupList->ExecuteLD())
        {
        TInt index(listBox->CurrentItemIndex());
        TRACES1("CStartupAppUi::ShowCityListL(): CurrentItemIndex: %d", index);
        TPtrC cityName = listBox->Model()->ItemText(index);

        CTzLocalizedCity* tzLocalizedCity = tzLocalizer->FindCityByNameL(cityName);
        CleanupStack::PushL(tzLocalizedCity);

        TInt timeZoneId = tzLocalizedCity->TimeZoneId();

        tzLocalizer->SetTimeZoneL(timeZoneId);
        tzLocalizer->SetFrequentlyUsedZoneL(*tzLocalizedCity, CTzLocalizedTimeZone::ECurrentZone);

        CleanupStack::PopAndDestroy(tzLocalizedCity);

        TRACES1("CStartupAppUi::ShowCityListL(): Selected city    %S", &cityName);
        }
    else
        {
        TRACES("CStartupAppUi::ShowCityListL(): City list cancelled");
        retval = EFalse;
        }

    CleanupStack::Pop(items);
    CleanupStack::Pop(popupList);
    CleanupStack::PopAndDestroy(listBox);
    CleanupStack::PopAndDestroy(cityList);
    CleanupStack::PopAndDestroy(tzLocalizer);


    TRACES("CStartupAppUi::ShowCityListL(): Home city selected");
    TRACES1("CStartupAppUi::ShowCityListL(): End, return %d", retval);
    return retval;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowTimeQueryL()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::ShowTimeQueryL()
    {
    TRACES("CStartupAppUi::ShowTimeQueryL()");

    TTime time;
    GetDefaultTimeAndDate( time );

    CStartupQueryDialog* dlg = new (ELeave) CStartupQueryDialog(time, CAknQueryDialog::ENoTone);
    TRACES("CStartupAppUi::ShowTimeQueryL(): Publish dialog for Secondary UI");
    dlg->PublishDialogL(SecondaryDisplay::ECmdShowTimeQuery, SecondaryDisplay::KCatStartup);
    if( dlg->ExecuteLD( R_STARTUP_TIME_SETTING_QUERY ) )
        {
        TTime current;
        current.HomeTime();
        TDateTime cTime = current.DateTime();
        TDateTime atime = time.DateTime();
        atime.SetYear(cTime.Year());
        atime.SetMonth(cTime.Month());
        atime.SetDay(cTime.Day());
        time = atime;

        RTz rtz;
        User::LeaveIfError(rtz.Connect());
        User::LeaveIfError(rtz.SetHomeTime(time));
        rtz.Close();

        TRACES("CStartupAppUi::ShowTimeQueryL(): End, return ETrue");
        return ETrue;
        }
    else
        {
        //in case of poweroff key was pressed and shutdown is occuring
        TRACES("CStartupAppUi::ShowTimeQueryL(): End, return EFalse");
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::ShowDateQueryL()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::ShowDateQueryL()
    {
    TRACES("CStartupAppUi::ShowDateQueryL()");


    CStartupQueryDialog* dlg = new (ELeave) CStartupQueryDialog(iTime, CAknQueryDialog::ENoTone);
    TRACES("CStartupAppUi::ShowDateQueryL(): Publish dialog for Secondary UI");
    dlg->PublishDialogL(SecondaryDisplay::ECmdShowDateQuery, SecondaryDisplay::KCatStartup);

    TInt query( R_STARTUP_DATE_SETTING_QUERY_NOBACK );
    if ( iFirstBoot ) 
        {
        query = R_STARTUP_DATE_SETTING_QUERY;
        }

    if( dlg->ExecuteLD( query ) )
        {
        TTime current;
        current.HomeTime();
        TDateTime cTime = current.DateTime();
        TDateTime atime = iTime.DateTime();
        atime.SetHour(cTime.Hour());
        atime.SetMinute(cTime.Minute());
        atime.SetSecond(cTime.Second());
        atime.SetMicroSecond(cTime.MicroSecond());
        iTime = atime;

        RTz rtz;
        User::LeaveIfError(rtz.Connect());
        User::LeaveIfError(rtz.SetHomeTime(iTime));
        rtz.Close();

        TRACES("CStartupAppUi::ShowDateQueryL(): End, return ETrue");
        return ETrue;
        }
    else
        {
        // Back key pressed. ( Or poweroff key was pressed and shutdown is occuring )
        TRACES("CStartupAppUi::ShowDateQueryL(): End, return EFalse");
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::GetDefaultTimeAndDate()
// ---------------------------------------------------------------------------
void CStartupAppUi::GetDefaultTimeAndDate( TTime& aTime )
    {
    TRACES("CStartupAppUi::GetDefaultTimeAndDate(): Get Time and Date from CenRep");

    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
    if ( !err )
        {
        TBuf<KTimeFormatLength> buf;
        err = repository->Get( KStartupDefaultTime, buf );
        if( !err )
            {
            err = aTime.Set(buf); // returns error if cenrep time format not valid
            }
        }

    if ( err )
        {
        TRACES("CStartupAppUi::GetDefaultTimeAndDate(): Failed to get valid data from CenRep. Using default");
        aTime.Set(KDefaultDateTimeValue);
        }

    delete repository;
    TRACES("CStartupAppUi::GetDefaultTimeAndDate(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::FirstBoot()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::FirstBoot()
    {
    TRACES("CStartupAppUi::FirstBoot()");
    TInt value( 0 );

    TRACES("CStartupAppUi::FirstBoot(): Read first boot info from Central Repository ");
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartup ) );
    if ( err == KErrNone )
        {
        err = repository->Get( KStartupFirstBoot, value );
        }

    delete repository;

    if (value)
        {
        TRACES("CStartupAppUi::FirstBoot(): End, return EFalse");
        return EFalse;
        }

    else
        {
        TRACES("CStartupAppUi::FirstBoot(): End, return ETrue");
        return ETrue;
        }
    }
// ---------------------------------------------------------------------------
// CStartupAppUi::HiddenReset()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::HiddenReset()
    {
    TRACES("CStartupAppUi::HiddenReset()");

    TBool ret_val ( EFalse );
    if( !iFirstBoot )
        {
        TInt startupReason( ENormalStartup );
        TInt err = RProperty::Get( KPSUidStartup,
                                   KPSStartupReason,
                                   startupReason ); // Use default if this fails
        if( KErrNone != err )
            {
            TRACES1("CStartupAppUi::HiddenReset(): ENormalStartup get err %d", err);
            }
        ret_val = ( startupReason != ENormalStartup );
        }
    TRACES1("CStartupAppUi::HiddenReset(): End, return %d ",ret_val );
    return ret_val;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::RTCStatus()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::RTCStatus()
    {
    TRACES("CStartupAppUi::RTCStatus()");
    TBool ret_val ( EFalse );

    RStarterSession startersession;
    if( startersession.Connect() == KErrNone )
        {
        TRACES("CStartupAppUi::RTCStatus(): Connected to Starter ");
        ret_val = startersession.IsRTCTimeValid();
        startersession.Close();
        }
    else
        {
        TRACES("CStartupAppUi::RTCStatus(): Unable to connect to Starter ");
        }

    TRACES1("CStartupAppUi::RTCStatus(): End, return %d ",ret_val );
    return ret_val;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::MarkFirstBoot()
// ---------------------------------------------------------------------------
void CStartupAppUi::MarkFirstBoot()
    {
    TRACES("CStartupAppUi::MarkFirstBoot()");
    TRACES("CStartupAppUi::MarkFirstBoot(): Write first boot info to Central Repository ");

    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidStartup ) );
    if ( err == KErrNone )
        {
        err = repository->Set( KStartupFirstBoot, 1 );
        }

    delete repository;
    TRACES("CStartupAppUi::MarkFirstBoot(): End");
    }


#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::AnimationFinished()
// ---------------------------------------------------------------------------
void CStartupAppUi::AnimationFinished()
    {
    TRACES1("CStartupAppUi::AnimationFinished(): iInternalState=%d", iInternalState );

    // first notify animation completion, it doesn't matter if it was canceled
    if ( iAnimation->HasContent() )
        {
        switch ( iInternalState )
            {
            case EStartupShowingWelcomeAnimation:
                UpdateStartupUiPhase( EStartupUiPhaseSystemWelcomeDone );
                break;
        
            case EStartupShowingOperatorAnimation: 
                UpdateStartupUiPhase( EStartupUiPhaseOperatorWelcomeDone );
                break;
        
            default:
                TRACES("CStartupAppUi::AnimationFinished(): unknown state");
                break;                    
            }    
        }
    
    
    TInt err = KErrNone;

    if ( iAnimation->WasCancelled() )
        {
        TRACES("Animations cancelled");

        TRAP(err, DoNextStartupPhaseL( EStartupFirstBootAndRTCCheck ));
        }
    else if ( iInternalState == EStartupShowingWelcomeAnimation )
        {
        TRAP(err, DoNextStartupPhaseL( EStartupShowingOperatorAnimation ));
        }
    else if ( iInternalState == EStartupShowingOperatorAnimation )
        {
        TRAP(err, DoNextStartupPhaseL( EStartupShowingUserWelcomeNote ));
        }

    if ( err != KErrNone )
        {
        TRAP_IGNORE( DoNextStartupPhaseL(EStartupSystemFatalError) );
        }

    TRACES("CStartupAppUi::AnimationFinished(): End");
    }
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::StopStartupTone()
// ---------------------------------------------------------------------------
void CStartupAppUi::StopStartupTone()
    {
    TRACES("CStartupAppUi::StopStartupTone()");
    if ((iStartupTone) && (iStartupTone->Playing()))
        {
        iStartupTone->Stop();
        }
    TRACES("CStartupAppUi::StopStartupTone(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::StopOperatorTone()
// ---------------------------------------------------------------------------
void CStartupAppUi::StopOperatorTone()
    {
    TRACES("CStartupAppUi::StopOperatorTone()");
    if ((iOpStartupTone) && (iOpStartupTone->Playing()))
        {
        iOpStartupTone->Stop();
        }
    TRACES("CStartupAppUi::StopOperatorTone(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::StartupTonePlaying()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::StartupTonePlaying()
    {
    TRACES("CStartupAppUi::StartupTonePlaying()");
    return iStartupTone->Playing();
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::OperatorTonePlaying()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::OperatorTonePlaying()
    {
    TRACES("CStartupAppUi::OperatorTonePlaying()");
    return iOpStartupTone->Playing();
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupAppUi::SetCleanBoot()
// ---------------------------------------------------------------------------
void CStartupAppUi::SetCleanBoot()
    {
    TRACES("CStartupAppUi::SetCleanBoot()");
    iCleanBoot = EStartupCleanBoot;
    TRACES("CStartupAppUi::SetCleanBoot(): End");
    }

// ----------------------------------------------------------------------------
// CStartupAppUi::SimSupported()
// ----------------------------------------------------------------------------
TBool CStartupAppUi::SimSupported()
    {
    TRACES("CStartupAppUi::SimSupported()");
    return iSimSupported;
    }

// ----------------------------------------------------------------------------
// CStartupAppUi::CoverUISupported()
// ----------------------------------------------------------------------------
TBool CStartupAppUi::CoverUISupported()
    {
    TRACES("CStartupAppUi::CoverUISupported()");
    return iCoverUISupported;
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::DoNextStartupPhaseL( TStartupInternalState toState )
// State#                                Possible next states


// 0    EStartupStartingUp               1, 18
// 1    EStartupWaitingCriticalBlock     5, 18
// 5    EStartupWaitingTouchScreenCalib
// 6    EStartupWaitingPhoneLightIdle    8, 18
// 8    EStartupOfflineModeQuery         9, 18
// 9    EStartupWaitingCUIWelcomeAnim    10, 18
// 10   EStartupWaitingStartupTone       11, 18
// 11   EStartupShowingWelcomeAnimation  12, 14, 18
// 12   EStartupWaitingCUIOperatorAnim   13, 18
// 13   EStartupShowingOperatorAnimation 14, 14, 18
// 14   EStartupShowingUserWelcomeNote   15, 18
// 15   EStartupFirstBootAndRTCCheck     16, 18
// 16   EStartupWaitingCUIStartupReady   17, 18
// 17   EStartupStartupOK                -
// 18   EStartupSystemFatalError         -

// ---------------------------------------------------------------------------

void CStartupAppUi::DoNextStartupPhaseL( TStartupInternalState toState )
    {
    TRACES("CStartupAppUi::DoNextStartupPhaseL()");
    TRACES2("CStartupAppUi::DoNextStartupPhaseL(): Change internal state from %d to %d", iInternalState, toState );

    switch( iInternalState )
        {
        case EStartupStartingUp:
            {
            switch( toState )
                {
                case EStartupWaitingCriticalBlock:
                    iInternalState = EStartupWaitingCriticalBlock;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingCriticalBlock");
                    WaitingCriticalBlockEndingL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupWaitingCriticalBlock:
            {
            switch( toState )
                {
                case EStartupWaitingCUIStartupReady:
                    iInternalState = EStartupWaitingCUIStartupReady;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingCUIStartupReady");
                    WaitingCoverUIStartupReadySyncL();
                    break;
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupWaitingTouchScreenCalib:
                    iInternalState = EStartupWaitingTouchScreenCalib;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingTouchScreenCalib");
                    WaitingTouchScreenCalibL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupWaitingTouchScreenCalib:
            {
            switch( toState )
                {
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupOfflineModeQuery:
                    iInternalState = EStartupOfflineModeQuery;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupOfflineModeQuery");
                    ShowOfflineModeQueryL();
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
                    DoNextStartupPhaseL( EStartupWaitingShowStartupAnimation );
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
                    DoNextStartupPhaseL( EStartupWaitingCUIWelcomeAnim );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupOfflineModeQuery:
            {
            switch( toState )
                {
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupWaitingShowStartupAnimation:
                    iInternalState = EStartupWaitingShowStartupAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingShowStartupAnimation");
                    WaitingStartupAnimationStartL();
                    break;
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupWaitingCUIWelcomeAnim:
                    iInternalState = EStartupWaitingCUIWelcomeAnim;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingCUIWelcomeAnim");
                    WaitingCoverUIWelcomeAnimationSyncL();
                    break;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        case EStartupWaitingCUIWelcomeAnim:
            {
            switch( toState )
                {
                case EStartupWaitingStartupTone:
                    iInternalState = EStartupWaitingStartupTone;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingStartupTone");
                    WaitingStartupToneL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupWaitingStartupTone:
            {
            switch( toState )
                {
                case EStartupShowingWelcomeAnimation:
                    iInternalState = EStartupShowingWelcomeAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupShowingWelcomeAnimation");
                    iWelcomeAnimation->UpdateDrawInfo( EStartupDIWelcomeAnimStart );
                    iWelcomeAnimation->DrawNow();
                    DoStartupShowWelcomeAnimationL();
                    iWelcomeAnimation->UpdateDrawInfo( EStartupDIWelcomeAnimEnd );
                    iWelcomeAnimation->DrawNow();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        case EStartupWaitingShowStartupAnimation:
            {
            switch( toState )
                {
                case EStartupShowingWelcomeAnimation:
                    iInternalState = EStartupShowingWelcomeAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupShowingWelcomeAnimation");
                    DoStartupShowWelcomeAnimationL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG(
                        EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupShowingWelcomeAnimation:
            {
            switch( toState )
                {
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupShowingOperatorAnimation:
                    iInternalState = EStartupShowingOperatorAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState: EStartupShowingOperatorAnimation");
                    DoStartupShowOperatorAnimationL();
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
                case EStartupWaitingCUIOperatorAnim:
                    iOperatorAnimation = CStartupOperatorAnimation::NewL( this, ClientRect());
                    AddToStackL( iOperatorAnimation);
                    iInternalState = EStartupWaitingCUIOperatorAnim;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState: EStartupWaitingCUIOperatorAnim");
                    WaitingCoverUIOperatorAnimationSyncL();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                    break;
                case EStartupFirstBootAndRTCCheck:
                    iInternalState = EStartupFirstBootAndRTCCheck;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupFirstBootAndRTCCheck");
                    DoStartupFirstBootAndRTCCheckL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        case EStartupWaitingCUIOperatorAnim:
            {
            switch( toState )
                {
                case EStartupShowingOperatorAnimation:
                    iInternalState = EStartupShowingOperatorAnimation;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState: EStartupShowingOperatorAnimation");
                    iOperatorAnimation->UpdateDrawInfo( EStartupDIOperatorAnimStart );
                    DoStartupShowOperatorAnimationL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        case EStartupShowingOperatorAnimation:
            {
            switch( toState )
                {
                case EStartupShowingUserWelcomeNote:
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
                    iUserWelcomeNote = CStartupUserWelcomeNote::NewL( *this, ClientRect());
                    AddToStackL( iUserWelcomeNote );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                    iInternalState = EStartupShowingUserWelcomeNote;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState: EStartupShowingUserWelcomeNote");
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
                    iWelcomeAnimation->UpdateDrawInfo( EStartupDIWelcomeNoteStart );
                    iUserWelcomeNote->SetUserWelcomeNoteShowing(ETrue);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                    DoStartupShowUserWelcomeNoteL();
                    break;
                case EStartupFirstBootAndRTCCheck:
                    iInternalState = EStartupFirstBootAndRTCCheck;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupFirstBootAndRTCCheck");
                    DoStartupFirstBootAndRTCCheckL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupShowingUserWelcomeNote:
            {
            switch( toState )
                {
                case EStartupFirstBootAndRTCCheck:
                    iInternalState = EStartupFirstBootAndRTCCheck;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupFirstBootAndRTCCheck");
                    DoStartupFirstBootAndRTCCheckL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupFirstBootAndRTCCheck:
            {
            switch( toState )
                {
                case EStartupWaitingCUIStartupReady:
                    iInternalState = EStartupWaitingCUIStartupReady;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupWaitingCUIStartupReady");
                    WaitingCoverUIStartupReadySyncL();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupWaitingCUIStartupReady:
            {
            switch( toState )
                {
                case EStartupStartupOK:
                    iInternalState = EStartupStartupOK;
                    TRACES("CStartupAppUi::DoNextStartupPhaseL(): InternalState : EStartupStartupOK");
                    DoStartupEndPart();
                    break;
                case EStartupSystemFatalError:
                    SystemFatalErrorL();
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        case EStartupSystemFatalError:
            {
            switch( toState )
                {
                case EStartupStartingUp:
                case EStartupOfflineModeQuery:
                case EStartupShowingWelcomeAnimation:
                case EStartupShowingOperatorAnimation:
                case EStartupShowingUserWelcomeNote:
                case EStartupFirstBootAndRTCCheck:
                case EStartupWaitingCUIStartupReady:
                case EStartupStartupOK:
                case EStartupSystemFatalError:
                    //no need to continue, because state is
                    //already in EStartupSystemFatalError
                    //so stop the execution of the other branch of
                    //the startup application. The other
                    //is displayin the "Selftest failed. Contact service"-dialog.
                    TRACES1("CStartupAppUi::DoNextStartupPhaseL(): InternalState : from EStartupSystemFatalError to %d -> so stop here.", toState );
                    break;
                default:
                    __ASSERT_DEBUG( EFalse,
                        PANIC( EStartupInvalidInternalStateChange ) );
                    break;
                }
            }
            break;
        default:
            __ASSERT_DEBUG( EFalse, PANIC( EStartupInvalidInternalStateChange ) );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CStartupAppUi::SystemFatalErrorL()
// Call this only from DoNextStartupPhaseL()
// ---------------------------------------------------------------------------
void CStartupAppUi::SystemFatalErrorL()
    {
    TRACES("CStartupAppUi::SystemFatalErrorL()");

    iInternalState = EStartupSystemFatalError;

    TRACES("CStartupAppUi::SystemFatalErrorL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupAppUi::SimStatusChangedReset()
// ---------------------------------------------------------------------------
TBool CStartupAppUi::SimStatusChangedReset()
    {
    TRACES( "CStartupAppUi::SimStatusChangedReset()" );
    TBool ret_val( EFalse );
    if( !iFirstBoot )
        {
        TInt startupReason( 0 );
        RProperty::Get( KPSUidStartup, KPSStartupReason, startupReason );
        ret_val = ( startupReason == ESIMStatusChangeReset );
        }
    TRACES1( "CStartupAppUi::SimStatusChangedReset(): End, return %d", ret_val );
    return ret_val;
    }

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupAppUi::UpdateStartupUiPhase()
// ---------------------------------------------------------------------------
void CStartupAppUi::UpdateStartupUiPhase( TInt aValue )
    {
    TRACES1("CStartupAppUi::UpdateStartupUiPhase(): aValue=%d", aValue);
    
    TInt err = RProperty::Set( KPSUidStartup, KPSStartupUiPhase, aValue );
        
    if( KErrNone != err )
        {
        TRACES1("CStartupAppUi::UpdateStartupUiPhase(): KPSStartupUiPhase set err %d", err);
        }                          
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
// End of file
