/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApAppUi class definition. This class handles all user
*                actions and also System Agent events.
*
*/


#ifndef SYSAPAPPUI_H
#define SYSAPAPPUI_H

// INCLUDES
#include <aknappui.h>
#include <aknkeylock.h>
#ifndef RD_MULTIPLE_DRIVE
 #include <aknmemorycarddialog.h>
#endif // RD_MULTIPLE_DRIVE
#include <starterclient.h>
#include <starter.hrh>
#include <babackup.h>
#include <rmmcustomapi.h>

#include <DosSvrServices.h>
#include "SysAp.hrh"
#include "SysApMMCObserver.h"
#include "MSysApPropertyResponder.h"
#include "SysApTimer.h"
#include <e32property.h>
#include "CoreApplicationUIsPrivateCRKeys.h"
#include <sysapcallback.h>

#include "SysApAccessoryObserver.h"
#ifdef RD_MULTIPLE_DRIVE
  #include "sysapdrivelist.h"
#endif // RD_MULTIPLE_DRIVE

#include "sysapusbchargerdetector.h"
#include "sysappsmcontroller.h"

// FORWARD DECLARATIONS
class CAknSmallIndicator;
class CAknSignalNotify;
class CAknBatteryNotify;
class CAknGlobalListQuery;
class CAknGlobalNote;
class CSystemLock;
class CSysApLightsController;
class CSysApPubSubObserver;
class CSysApProfileObserver;
class CSysApConnectionMonitorObserver;
class CSysApCenRepLightSettingsObserver;
class CSysApCenRepLogsObserver;
class CSysApCenRepBtObserver;
class CSysApCenRepFmTxObserver;
class CSysApCenRepController;
class CSysApStartupController;
class CSysApPowerKeyMenuObserver;
class CSysApWaitNote;
class CSysApSsSettingsObserver;
class CSysApShutdownImage;
class CSysApShutdownAnimation;
class CSysApEtelConnector;
class CSysApMsgSimMemLowQuery;
class CSysApNspsHandler;
class CSysApConfirmationQuery;
class CSysApFeatureManager;
class MSysApOfflineModeController;
class MSysApBtController;
class MSysApBtSapController;
class MSysApUsbIndicator;
class MSysApLocationPrivacyIndicator;
class MProfileEngine;
class MProfilesNamesArray;
class CSysApKeySndHandler;
class CSysApKeyManagement;  // KEF
class CSysApMediatorObserver;
class CAknAppUiBase;
class CSysApCenRepHacSettingObserver;
class CSysApAudioRoutingObserver;
class CSysApCenRepCallForwardingObserver;
class CSysApCenRepMsgWaitingObserver;

// POC launching
class CAiwServiceHandler;

class CSysApDefaultKeyHandler;
class CSysApDriveUnlockHandler;
class CSysApDriveEjectHandler;

class CSysApBatteryInfoController;

//CONSTANTS
const TInt KDummyReason( -1 );
const TInt KBluetoothModulePowerModeOn ( 1 );
const TInt KBluetoothModulePowerModeOff ( 0 );
const TInt KGeneralProfileId ( 0 );
const TInt KOfflineModeProfileId ( 5 );
const TInt KApplicationScanningInterval( 1 ); // 1 second
_LIT( KFSName, "Fat" );                 // MMC file system name
const TDriveNumber KMMCDrive = EDriveE; // MMC drive number



//CLASS DECLARATION
/**
*  CSysApAppUi class. This class serves as a link between the other
*  SysAp classes and performs the most UI specific operations on SysAp's
*  responsibility.
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApAppUi :
                    public CAknAppUi,
#ifndef RD_MULTIPLE_DRIVE
                    public MAknMemoryCardDialogObserver,
#endif // RD_MULTIPLE_DRIVE
                    public MSysApMemoryCardObserver,
                    public MSysApTimerResponder,
                    public MSysapCallback,
                    public MSysApPsmControllerNotifyCallback
{
    public:
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApAppUi();

        /**
        * Destructor
        * @param None
        * @return void
        */
        virtual ~CSysApAppUi();

    protected: // From MSysApTimerResponder

        /**
        * Handle events from CSysApTimer
        * @param None
        * @return void
        */
        void TimerExpiredL();

    public:

        /**
        * Gets called when shutdown continues after shutdown animation
        * @param None
        * @return void
        */
        void ContinueShutdown();

        /**
        * Gets called when there is need to shutdown the device initiated by Symbian side
        * @param None
        * @return void
        */
        void DoShutdownL( const TBool aReset, const TInt aResetReason );

        /**
        * Gets called by CSysApSharedDataObserver if user has selected "No" in
        * "Do you want to use the phone in Off-line mode?" query in startup.
        * @param aDoProfileChange if ETrue, the last active profile is restored
        * @return void
        */
        void GoOnlineL( TBool aDoProfileChange = ETrue );

        /**
        * Gets called by CSysApSharedDataObserver in Simless Offline Mode startup.
        * @param None
        * @return void
        */
        void GoOfflineL();

        /**
        * Calls CSysApOfflineModeController::SwitchFromOnlineToOfflineModeL()
        * @param None
        * @return void
        */
        void SwitchFromOnlineToOfflineModeL();

        /**
        * Called when offline mode change has been finished.
        */
        void OfflineModeChangedL();

        /**
        * Gets called after changing the SW state.
        * @param aSwState The new state
        * @return void
        */
        void DoStateChangedL( const RStarterSession::TGlobalState aSwState  );

#ifdef SYSAP_USE_STARTUP_UI_PHASE        
        /**
        * Called when UI is ready after boot.
        */
        void HandleUiReadyAfterBootL();

#endif // SYSAP_USE_STARTUP_UI_PHASE
        
        /**
        * Returns whether UI is ready after boot.
        */
        TBool UiReady() const;

        /**
        * Gets called when any case selection on powerkey menu is done.
        * Does what is needed per selection.
        * @param TInt aSelection
        * @return void
        */
        void PowerKeyPopUpMenuSelectionDoneL( TInt aSelection );

        /**
        * Gets called when Alarm is being shown to user and lights need to be blinked.
        * @param TBool aAlerting
        * @return void
        */
        void AlertUiAlertingL( const TBool aAlerting );

        /**
        * Gets called when Alarm is being shown to user and keylock need to be turned off.
        * @param TBool aKeyLockOff
        * @return void
        */
        void AlertUiKeyLockOff( const TBool aKeyLockOff );

        /**
        * Notifies changes in KPSUidCoreApplicationUIs::KLightControl to light controller.
        */
        void HandleLightsRequireL() const;

        /**
        * Notifies changes in KPSUidCoreApplicationUIs::KCoreAppUIsLightsRawKeyEvent to light controller.
        *
        * @since S60 3.2
        */
        void HandleRawKeyEventLightsRequireL() const;

        /**
        * Gets called when Video Telephony requests to force/cancel lights on/
        * @param const TInt aLightsParameter
        * @return void
        */
        void HandleForcedLightsVTRequireL( const TInt aLightsParameter ) const;

        /**
        * Gets called when Screen Saver requests to force lights on/off
        * @param const TInt aLightsParameter
        * @return void
        */
        void HandleForcedLightsSSRequireL( const TInt aLightsParameter ) const;
				
				/**
        * Gets called when AT Command requests to force lights on/off
        * @param const TInt aLightsParameter
        * @return void
        */
				void HandleForcedLightsATCRequireL( const TInt aLightsParameter ) const;
        /**
        * Calls SysApLightsController::SwitchLightsOnSoftRejectL
        *
        * @since S60 3.1
        * @param None.
        * @return None.
        */
        void SwitchLightsOnSoftRejectL();

        /**
        * Asks state of one property from PubSub
        * @param const TUid&
        * @param const TUint
        * @return const TInt ( value of property or error code )
         */
        TInt StateOfProperty( const TUid& aCategory, const TUint aKey ) const;

        /**
        * Sets the indicator's aIndicator state to state aState.
        * @param TInt aIndicator
        * @param TInt aState
        * @return void
        */
        void SetIndicatorStateL( const TInt aIndicator, const TInt aState ) const;

        /**
        * Returns the state of keylock
        * @param none
        * @return TBool the state of keylock
        */
        TBool KeyLockState() const;

        /**
        * Returns the state of devicelock
        * @param none
        * @return TBool the state of devicelock
        */
        TBool DeviceLockState() const;

        /*
        * @param None
        * @return void
        */
        void SetIhfIndicatorL();

        /*
        * Handles actions needed when either SIM memory gets too full to receive any more SMS's.
        * @param aSimStoreFull If true, SIM memory is full.
        * @return void
        */
        void HandleSmsStorageNotificationL( TBool aSimStoreFull );

        void HandleNspsRawKeyEventL();

        void HandleNetworkNspsNotification( RMmCustomAPI::TNspsStatus aNspsStatus );

        void HandleMessageTonePlayingQuitability( TInt aTonePlayingStatus );

        /*
        * Called by SysApLightsController when the lights are switced on/off
        * @param aActivate If ETrue forwarding is activated, otherwise deactivated
        * @return void
        */
        void ActivateKeyeventForwardingForLights(TBool aActivate);

        /**
        * Gets called when shutdown requested when sw state == ESWStateAlarm
        * context active.
        * @param None
        * @return void
        */
        void StartShutDownTimerOnAlarmAndChargingStates();
        static TInt DoShutdownOnAlarmStateAfterCallBack( TAny* aObject );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Starts shutdown animation timing after shutdown tone sync is done
        */
        void StartAnimTiming();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * Stops shutdown animation timing and continues shutdown.
        */
        static TInt DoStopAnimTiming( TAny* aObject );

        /**
        * Gets called when GPRS is suspended because of a phone call established while
        * context active. Delay (KDelayBeforeShowingGprsSuspendedNote) required to ensure
        * that the phone call was really established.
        * @param None
        * @return void
        */
        void StartGprsSuspendedTimer();
        static TInt ShowGprsSuspendedNoteAfterCallBack( TAny* aObject );

        /**
        * Gets called when an accessory was connected to device. The indicator
        * is shown and profile gets changed if needed.
        * @param aAccessoryState Indicates the new accessory state
        * @return void
        */
        void HandleAccessoryConnectedL( TAccMode aAccessoryState );

        /**
        * Gets called when headset/loopset was disconnected from device. The indicator
        * is hidden and the original profile gets restored if needed.
        * @param None
        * @return void
        */
        void HandleAccessoryDisconnectedL();

        /**
        * Gets called when SysAp receives KSysApSatRefreshStart. All
        * non-system UI applications are closed
        * and an information note is shown to the user.
        * @param None
        * @return void
        */
        void HandleSatRefreshL();

#ifndef RD_MULTIPLE_DRIVE
        /**
        * Mounts MMC drive
        * @param None
        * @return TInt : error value from RFs::MountFileSystem()
        */
        TInt MountMMC();

        /**
        * Dismounts MMC drive
        * @param None
        * @return TInt : error value from RFs::MountFileSystem()
        */
        TInt DismountMMC();
#endif // RD_MULTIPLE_DRIVE

        /**
        * MMC Hot Swap Ejection from powerkey menu
        * @param None
        * @return void
        */
        void EjectMMCL();

        /**
        * Disconnect BtSap from powerkey meny
        * @param None
        * @return void
        */
        void DisconnectBtSap();

         /**
         * Enable or disable FM transmission 
         */
         void ChangeFmTxStateL( TBool aEnable );

#ifndef RD_MULTIPLE_DRIVE
    public: // from MAknMemoryCardDialogObserver

        /**
        * To be notified when unlock completed
        * @param TInt aResult
        * @return void
        */
        void UnlockComplete( TInt aResult );
#endif // RD_MULTIPLE_DRIVE

    public:

        /**
        * Plays the sound EAvkonSIDInformationTone
        * @param None
        * @return void
        */
        void Beep();

#ifndef RD_MULTIPLE_DRIVE
        /**
        * Unlocks MMC
        * @param None
        * @return void
        */
        void RunUnlockNotifierL( TSysApMemoryCardStatus aMemoryCardStatus = ESysApMemoryCardStatusNotKnown );

        /**
        * From MSysApMemoryCardObserver.
        * Gets called by CSysApMMCObserver when an MMC is inserted/removed
        * @return void
        */
        void MMCStatusChangedL();

        /**
        * Shows "MMC removed..." dialogs
        * @param None
        * @return void
        */
        void ShowMMCDismountedDialogL();
#else // RD_MULTIPLE_DRIVE

        /**
        * Updates PS key during boot
        * @return void
        */
        void MMCInsertedL();

        /**
        * Unlocks MMC
        * @param None
        * @return void
        */
        void RunUnlockNotifierL();

        /**
        * From MSysApMemoryCardObserver.
        * Gets called by CSysApMMCObserver when an MMC is inserted/removed
        * @param aDrive Changed drive
        * @return void
        */
        void MMCStatusChangedL( TInt aDrive );

        /**
        * Shows "MMC removed..." dialogs
        * @param aDrive Drive to dismount
        * @param aEjectType Eject type used for the drive
        * @return void
        */
        TBool ShowMMCDismountedDialogL(
            TInt aDrive, CSysApDriveList::TDriveEjectType aEjectType );
#endif // RD_MULTIPLE_DRIVE

        /**
        * Called after confirming dialog "MMC removed..."
        * @param None
        * @return void
        */
        void MMCDismountedDialogConfirmed();

        /**
        * Gets called by CSysApSharedDataObserver when "Eject" is started
        * or finished in File Manager application.
        * @param None
        * @return void
        */
        void EjectStarted( TBool ejectStarted );

    private:
        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL();
	
	    /**
        * Deactivate PSM on reboot if battery level is above threshold value.
        * @param None
        * @return void
        */
	    void DeactivatePSMifBatteryNotLowL();

    private:

#ifndef RD_MULTIPLE_DRIVE

        /**
        * Resolves process UID based on its thread id
        *
        * @since S60 3.1
        * @param aThreadId id of the thread
        * @return TInt32 process UID
        */
        TInt32 ResolveUidFromThread( TUint aThreadId ) const;

        /**
        * Called recursively when shutting down apps in Hot Swap
        * @param None
        * @return void
        */
        void CloseUIAppsInHotSwapL();

        /**
        * Called by CloseUIApplicationsHotSwapL when completing
        * shutting apps
        * @param None
        * @return void
        */
        void CompleteAppsShuttingInHotSwapL();
#endif // RD_MULTIPLE_DRIVE

     public:

        /**
        * Initializes parameters used in shutting down apps in BT SAP
        * @param None
        * @return void
        */
        void InitCloseSimApplicationsL();

      private:

        /**
        * Called recursively when shutting down apps in BT SAP
        * @param None
        * @return void
        */
        void CloseSimApplicationsL();

        /**
        * Called to re-envoke CloseSimApplicationsL()
        * @param None
        * @return void
        */
        static TInt DoCloseSimApplicationsAfterCallBack( TAny* aObject );

    private:
        /**
        * Takes care of command handling.
        * @param aCommand command to be handled
        * @return void
        */
        void HandleCommandL( TInt aCommand);

        /**
        * handles key events
        * @param TKeyEvent& aKeyEvent
        * @param TEventCode aType
        * @return TKeyResponse
        */
        TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    private:
        // By default, prohibit copy constructor
        CSysApAppUi( const CSysApAppUi& );

        // Prohibit assigment operator
        CSysApAppUi& operator= ( const CSysApAppUi& );

    private:
        void HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent);
        /**
        * Gets called by ConstructL function to set initial states to indicators etc
        * Needed state information is got from Shared Data and status pane's components
        * are set to the correct states.
        * @param None
        * @return void
        */
        void InitializeStatusPaneAreaL();

        /**
        * Gets called by HandleKeyEventL after detecting a long press of powerkey.
        * According to current state of the device desides if it is time to either
        * shutdown or make startup (from charging state ) to normal state
        * @param None
        * @return void
        */
        void HandleLongPowerKeyPressedL();

        /**
        * Gets called by HandleKeyEventL after a short press of powerkey.
        * According to current state of the device desides if it is time to
        * either switch lights on and show mode selection dialog(normal state)
        * or to shutdown the phone (in alarm state).
        * @param None
        * @return void
        */
        void HandleShortPowerKeyPressedL();

        /**
        * Frees SysAp's reserved memory, closes connections to servers etc.
        * @param None
        * @return void
        */
        void FreeResources();

    public:

        /**
        * Displays a UI query.
        * @param TSysApConfirmationQueryIds
        * @param aValue query related value
        * @return void
        */
        void ShowQueryL( const TSysApConfirmationQueryIds aQueryId,
            const TDesC& aValue = KNullDesC );

        /**
        * Displays a UI note.
        * @param TSysApNoteIds aSysApNoteIds
        * @return void
        */
        void ShowUiNoteL( const TSysApNoteIds aNote ) const;

        /**
        * Displays a "Charger connected" note.
        * @param None
        * @return void
        */
        void ShowChargingNoteL();

        /**
        * Handles charger related notes
        * @param TInt : KPSUidChargerCStatusValue
        * @return void
        */
        void HandleChargerNotesL( const TInt aValue );

        /**
        * Displays a "Selected profile" note.
        * @param None
        * @return void
        */
        void ShowProfileNoteL();

        /**
        * Cancels a UI wait note.
        * @param None
        * @return void
        */
        void CancelWaitNote();

        /**
        * Cancels the global list query
        * @param None
        * @return void
        */
        void CancelGlobalListQuery();

        /**
        * Updates signal bars to the value cached by Etel connector.
        * @return void
        */
        void UpdateSignalBarsL();

        /**
        * Updates signal bars to specified state.
        * @param TInt aState Number of bars or KAknSignalOffLineMode
        * @return void
        */
        void UpdateSignalBarsL( const TInt aState );

        /**
        * Gets called by HandleSystemAgentEventL to singal availability indicator
        * ie. Gprs state icon or common signal icon.
        * @param None
        * @return void
        */
        void SetSignalIndicatorL();

        /**
        * Gets called by SetSignalIndicatorL() when in GSM network
        * @param None
        * @return void
        */
        void SetSignalIndicatorGsmL();

        /**
        * Gets called by SetSignalIndicatorL() when in Wcdma network
        * @param None
        * @return void
        */
        void SetSignalIndicatorWcdmaL();

        /**
        * Handles GPRS suspend/resume notes if feature supported.
        * ie. Gprs state icon or common signal icon.
        * @param None
        * @return void
        */
        void HandleGprsNotesL();

        /**
        * Returns the current bluetooth power mode setting using CSysApSharedDataObserver
        * @param None
        * @return TInt
        */
        TInt BluetoothPowerMode() const;

        /**
        * Activates the profile given as a parameter
        * @param TInt aProfileId
        * @return void
        */
        void ActivateProfileL( const TInt aProfileId ) const;

        /**
        * Activates the profile saved in iProfileToBeActivated.
        * @param None
        * @return void
        */
        void ActivateOnlineProfileL() const;

        /**
        * Checks whether Offline Mode is activated
        * @param TBool
        * @return void
        */
        TBool OfflineModeActive();

        /**
        * Tolds Offline Mode controller not to activate BT
        * when next going online.
        * @param None
        * @return void
        */
        void DoNotActivateBt();

        /**
        * Switches BT module on/off using BT Engine
        * @param TBool: ETrue = on, EFalse = off
        * @return void
        */
        TInt SetBtPowerState( TBool aBtState );

        /**
        * Sets iKeyLockOnBeforeCall = EFalse.
        * @param None
        * @return void
        */
        void DoNotEnableKeylock();

#ifdef RD_MULTIPLE_DRIVE
        /*
        * Starts wait note for eject
        */
        void ShowEjectWaitNoteL(  TInt aDriveToEject );

        /*
        * Checks is eject query visible
        */
        TBool IsEjectQueryVisible();
#endif // RD_MULTIPLE_DRIVE

        /**
        * Gets called by CSysApSharedDataObserver when "Eject" is selected
        * in File Manager application.
        * @param aDrive Drive to eject
        * @return void
        */
        void EjectUsed( TInt aDrive );

        /**
        * MMC Hot Swap Ejection from powerkey menu was canceled
        * @param None
        * @return void
        */
        void EjectMMCCanceled();

    private:

        /**
        * Gets currently active profile's name.
        * @param TPtr aProfileName
        * @return void
        */
        void ActiveProfileNameL( TPtr aProfileName );

    public:

        /**
        * Gets called by HandleSystemAgentEventL to update battery bars.
        * @param TInt aState
        * @return void
        */
        void UpdateBatteryBarsL( const TInt aState );

        /**
        * Gets called by HandleSystemAgentEventL when profile was changed.
        * Profile selected Global Note is shown.
        * @param TInt aValue
        * @return void
        */
        void HandleProfileChangedL( const TInt aValue );

        /**
        * Checks if the active profile is silent and updates CSysApLightsController
        * if needed.
        * @param None
        * @return void
        */
        void CheckSilentModeL();

        /**
        * Gets called by CSysApOfflineModeController when activation/deactivation
        * of Offline Mode fails
        * @param TBool aOffline
        * @return void
        */
        void RestoreProfileL( const TBool aOffline );

        /**
        * Returns the id of the active profile.
        * @param None
        * @return Int
        */
        TInt ActiveProfileId();

        /**
        * Gets called when the value of the SD key KGSDisplayLightsTimeout
        * changes
        * @param TInt : lights timeout in seconds
        * @return void
        */
        void DoLightsTimeoutChangedL( const TInt aTimeout );

    private:

        /**
        * Checks whether it is necessary to change profiles in case an accessory
        * was connected/disconnected while the device was powered down.
        *
        * @param TInt aValue
        * @return void
        */
        void HandleAccessoryProfileInStartupL( );

    public:
        /**
        * Gets called by HandleSystemAgentEventL to Disable keylock when devicelock is activated.
        * This is done already when device lock is activated because this
        * @param None
        * @return void
        */
        void SetDeviceLockEnabledL( const TBool aLockEnabled );

    private:

        /**
        * Gets called by HandleShortPowerKeyPressedL to display the power key popup menu
        * @param None
        * @return void
        */
        void ShowPowerKeyPopUpMenuL();

        /**
        * Does what is needed to do inside SysAp before actually doing
        * other shutdown related things. This is used when shutdown image is enabled.
        * @param None
        * @return void
        */
        void PrepareForShutdownImage();

        /**
        * Does what is needed to do inside SysAp before actually doing
        * other shutdown related things. This is used when shutdown animation is enabled.
        * @param None
        * @return void
        */
        void PrepareForShutdownAnimation();

        /**
        * Show shutdown animation
        *
        * @param
        * @return ETrue if animation is enabled
        */
    #ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        void
    #else // RD_STARTUP_ANIMATION_CUSTOMIZATION
        TBool
    #endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        ShowAnimationL();

        /**
        * Makes Status pane visible/invisible when needed and changes it's layout between Charging/Alarm
        * @param None
        * @return void
        */
        void SetStatusPaneLayoutL( const TSysApPhoneState aState );

        /**
        * Checks from current Symbian swstate if it is ok to initiate shutdown
        * @param None
        * @return const TBool
        */
        TBool OkToInitiateShutdown();

        /**
        * Shows shutdown animation or shutdown image
        * @param None
        * @return void
        */
        void PrepareForCompleteShutdown();

        /**
        * Makes the end part of shutdown sequence.
        * @param None
        * @return void
        */
        void CompleteShutdown( const TBool aReset = EFalse, const TInt aResetReason = KDummyReason );

        /**
        * Sets calls forwarding indicators on/off accrding to aCallsForwardingState
        * @param TInt aCallsForwardingState
        * @return void
        */
        void DoSwStateNormalConstructionL();

    public:

        /**
        * Starts an application or brings an already running application
        * to foreground.
        * @param TUid: UID of the application that is activated
        * @return void
        */
        void ActivateApplicationL( const TUid aUid );

        void HandleCurrentCallStateChangeL( TInt aCurrentCallState );

        void DoSimChangedFromPreviousBootL();

        /**
        * Gets called in SysAp construction or when SysAp gets a SA/P&S notification
        * @param const TInt: indicator status
        * @return void
        */
        void HandleLocationPrivacyIndicatorL( const TInt aState );

        /**
        * Return value of the__SIMCARD feature
        * @param None
        * @return TBool
        */
        TBool SimSupported();

        /**
        * Notify CSysApLightController of the empty battery.
        * @param None
        * @return void
        */
        void BatteryEmptyL();

        /**
        * Prompt starter to change mode
        * @param aState The new state
        * @return KErrArgument if the state change is illegal (from current state
        * it is not possible to change to the requested state), KErrNone if the
        * global state change has been successfully initiated.
        */
        TInt SetStarterState( const RStarterSession::TGlobalState aState );

        /**
        * Creates the device name string for BT Sap disconnect prompt.
        * @return device name string.
        */
        HBufC* GetBtSapDeviceNameL();

        /**
        * Handle alarm indicator
        * @param aShowIndicator If ETrue, indicator is show, otherwise it is hidden.
        */
        void ShowAlarmIndicatorL( TBool aShowIndicator );

        /**
        * Informs lights controller that flip has been opened/close
        * @param aFlipOpen ETrue if flip is open, EFalse otherwise
        */
        void SetFlipOpenL( TBool aFlipOpen );

        /**
        * Creates a virtual flip key event that is passed to the key management
        * of SysAp.
        *
        * @since S60 3.2
        * @param aFlipOpen ETrue if flip is open, EFalse otherwise
        * @return None
        */
        void SimulateFlipKeyEventL( TBool aFlipOpen );

        /**
        * Informs lights controller that grip has been opened/close
        * @param aGripOpen ETrue if grip is open, EFalse otherwise
        */
        void SetGripOpenL( TBool aGripOpen );

        /**
        * Sets the status of the Central Repository key
        * KCRUidCoreApplicationUIs / KCoreAppUIsNetworkConnectionAllowed
        * @param TInt
        * @return void
        */
        void SetNetworkConnectionAllowed( TCoreAppUIsNetworkConnectionAllowed aNetworkConnectionAllowed );

        /**
        * Get pointer to RTelServer
        *
        * @return pointer to RTelServer if iSysApEtelConnector is initialized. Otherwise return NULL.
        */
        RTelServer* GetTelServer();

        /**
        * Show WLan indicator
        *
        * @param aValue The value of KPSWlanIndicator PS key.
        */
        void HandleWlanIndicatorL( TInt aValue );

        /**
        * Show TARM indicator
        *
        * @param aValue The value of KCoreAppUIsTarmIndicator PS key
        */
        void HandleTarmIndicatorL( TInt aValue );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Continue shutdown animation showing after sync request
        */
        void ShutdownAnimationSyncOK();
#endif RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * Skip shutdown animation (i.e. stop showing it). Initiated by CoverUI.
        */
        void SkipShutdownAnimation();

        /**
        * Notifies CoverUI about skipping shutdown animation.
        */
        void NotifyShutdownAnimationSkip();

        /**
        * Check cover display support
        *
        * @return ETrue if cover display is supported, EFalse otherwise
        */
        TBool CoverDisplaySupported();

        /**
        * Returns reference to RStarterSession
        * RStarterSession is connected when this method is called
        * for the first time.
        *
        * Always (except when freeing resources) access the starter session via this method,
        * don't use the member variable directly.
        *
        * @return reference to RStarterSession
        */
        RStarterSession& StarterSession();

        /**
        * Returns reference to SysAp's CenRep controller
        *
        * @since S60 3.1
        * @param None
        * @return reference to CSysApCenRepController
        */
        CSysApCenRepController& CenRepController();

        /**
        * Ignore zero network bars signal notifies.
        * This is needed in BT SAP mode.
        *
        * @param aIgnore If ETrue, ignore any future zero network bars notifications.
        */
        void IgnoreZeroNetworkBarNotifications(TBool aIgnore);

        /**
        * Check if BT SAP is enabled
        *
        * @return ETrue if BT SAP is enabled, EFalse otherwise
        */
        TBool BtSapEnabled();

        /**
        * Deactivate BT when changing to normal mode.
        * Can only be used during startup, has no effect after that.
        */
        void DeactivateBT();

    private:
        /**
        * Returns reference to SysAp's keylock session
        * RAknKeylock2 session is connected when this method is called
        * for the first time.
        *
        * Always (except when freeing resources) access the keylock session via this method,
        * don't use the member variable directly.
        *
        * @since S60 3.1
        * @param None
        * @return reference to RAknKeylock2
        */
        RAknKeylock2& KeyLock();



#ifdef __SYSAP_MODULE_TEST

        /**
        * Can be used for showing a CAknGlobalNote in module tests
        * @param noteText : text to be shown in the note
        * @return void
        */
        void ModuleTestShowUiNoteL( const TDesC& noteText ) const;
#endif

        /**
        * shows power down note before displaying shutdown image
        * @param: none
        * @return void
        */
        void ShowShutdownNoteL();

        /** Return ETrue if the global system state is one of the 'normal'
        * states: ESwStateNormalRfOn, ESwStateNormalRfOff or
        * ESwStateNormalBTSap.
        */
        TBool IsStateNormal() const;

        /**
        * Returns max of three integers
        * @param: TInt
        * @return void
        */
        TInt Max( const TInt aA, const TInt aB, const TInt aC );

        /**
        * Displays shutdown image.
        * This method is just a nonleaving implementation,
        * calls ShowShutdownImageL for performing the task.
        *
        * @since S60 3.1
        * @param aBitmapId  bitmap identifier
        * @return TInt an error value
        * @see CSysApAppUi::ShowShutdownImageL
        */
        TInt ShowShutdownImage( TInt aBitmapId );

        /**
        * Displays shutdown image. If a CSysApShutdownImage instance
        * does not exist, it will be created.
        *
        * @since S60 3.1
        * @param aBitmapId  bitmap identifier
        * @return None
        * @leave Symbian error code
        */
        void ShowShutdownImageL( TInt aBitmapId );

    private: // from MSysapCallback
        /**
        * Executes a command in SysAp.
        *
        * @since S60 3.1
        * @param[in] aCommand   command identifier
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapCommand
        */
        void ExecCommandL( TSysapCommand aCommand );

        /**
        * Executes a command in SysAp.
        *
        * @since S60 3.1
        * @param aCommand   command identifier
        * @param aParam     command specific parameter data
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapCommand
        */
        void ExecCommandL( TSysapCommand aCommand, const TDesC8& aParam );

        /**
        * Queries information from SysAp.
        *
        * @since S60 3.1
        * @param[in]  aQuery    query identifier
        * @param[out] aReturn   query specific return data
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapQuery
        */
        void ExecQueryL( TSysapQuery aQuery, TDes8& aReturn );

        /**
        * Queries information from SysAp.
        *
        * @since S60 3.1
        * @param[in]  aQuery    query identifier
        * @param[out] aReturn   query specific return data
        * @param[in]  aParam    query specific parameter data
        * @return None
        * @leave an error code, system wide/command specific
        *
        * @see MSysapCallback::TSysapQuery
        */
        void ExecQueryL( TSysapQuery aQuery, TDes8& aReturn, const TDesC8& aParam );

    public:
        /**
        * Sets HAC (Hearing Aid Compatibility) indicator according to its CenRep-setting
        * state.
        * HAC indicator is not displayed if an accessory is connected.
        *
        * @param None
        * @return None
        */
        void SetHacIndicatorL();

        /**
        * Returns reference to SysAp's internal feature manager.
        *
        * @since S60 3.1
        * @return CSysApFeatureManager&
        */
        CSysApFeatureManager& SysApFeatureManager();
        
        /**
        * Handles response to power save activation query.
        *
        * @param aEnable If ETrue, the user has accepted the query.
        */
        void HandleActivatePsmQueryResponse( TBool aEnable );

        /**
        * Handles response to power save deactivation query.
        *
        * @param aDisable If ETrue, the user has accepted the query.
        */
        void HandleDeactivatePsmQueryResponse( TBool aDisable );

    private: // from CAknAppUi
        /**
        * Handles a change to the application's resources which are shared across the environment.
        *
        * @since S60 3.1
        * @param aType The type of resources that have changed.
        * @return None.
        */
        void HandleResourceChangeL(TInt aType);

    private:
        /**
        * Shows and hides the memory card indicator depending on whether the memory card is inserted or not.
        *
        * @since S60 3.1
        * @param None.
        * @return None.
        */
        void SetMemoryCardIndicatorL();

        /**
        * Returns whether IHF has been enabled.
        * Creates audio routing observer if it does not exist..
        *
        * @since S60 3.2
        * @return TBool ETrue if IHF is enabled
        */
        TBool IhfEnabledL();

#ifdef RD_MULTIPLE_DRIVE
        void UpdateInsertedMemoryCardsL();
#endif // RD_MULTIPLE_DRIVE

    	/**
    	* Handles setup of USB charger from given charging status.
    	*
    	* @since S60 3.2
    	* @param aValue Given charging status
    	*/
        void HandleUsbCharger( const TInt aValue );

    public:
        /**
        * Sets USB connection attach status.
        * This prevents double beep when USB file transfer gets activated.
        *
        * @since S60 3.2
        * @param aUsbConnected ETrue if USB cable has just been attached, otherwise EFalse
        */
        void SetUsbAttachStatus( const TBool aUsbAttached );

        /**
    	* Returns whether the key event is considered to be a device mode key event,
    	* which usually causes a transition in the device mode (e.g. grip, flip or twist).
    	*
    	* @since S60 3.2
    	* @param aKeyEvent key event
    	* @return TBool ETrue key event is considered to be a device mode key
    	*               EFalse not a device moed key
    	*/
    	TBool IsDeviceModeKey( const TKeyEvent& aKeyEvent ) const;

    	/**
    	* Returns whether resources (objects, server connections) owned by this
    	* class has been freed.
    	*
    	* @since S60 3.2
    	* @return TBool ETrue if resource deallocation has been performed.
    	*/
    	TBool ResourcesFreed() const;

    	/**
    	* Returns pointer to CSysApEtelConnector.
    	*
    	* @since S60 3.2
    	* @return pointer to CSysApEtelConnector instance owned by this class.
    	*         NULL if the object creation has failed.
    	*/
    	CSysApEtelConnector* EtelConnector() const;
    	
    	/**
    	* Returns reference to USB charger detector, TSysApUsbChargerDetector.
    	*
    	* @since S60 3.2
    	* @return Reference to USB charger detector.
    	*/
        TSysApUsbChargerDetector& UsbChargerDetector();

    	/**
    	* Sets envelope indicator according to current messaging status.
    	*
    	* @since S60 3.2
    	*/
        void SetEnvelopeIndicatorL();

    	/**
    	* Handles value changes of the key KHWRMBatteryStatus.
    	*
    	* @since S60 3.2
    	* @param aValue the updated key value
    	*/
    	void HandleBatteryStatusL( const TInt aValue );
        
        /**
    	* Handles value changes of the key KHWRMChargingStatus.
    	*
    	* @since S60 3.2
    	* @param aValue the updated key value
    	*/
        void HandleChargingStatusL( const TInt aValue );
        
        /**
        * Cancel the specified query if it is active.
        *
        * @param aQueryId ID of the query that must be cancelled.
        *                 If ESysApNoQuery is given, the active query is cancelled.
        *                 without checking its ID.
        */
        void CancelQuery( TSysApConfirmationQueryIds aQueryId );
    
    private:
        // from MSysApPsmControllerNotifyCallback
        void NotifyPowerSaveModeL( TSysApPsmStatus aStatus );

    private:
        /**
        * Get the logs observer. Created if does not exist.
        * 
        * @since S60 3.2
        * @return reference to CSysApCenRepLogsObserver object
        */
        CSysApCenRepLogsObserver& CSysApAppUi::LogsObserverL();

    /**
     * Adds MMC removal item(s) to power menu.
     * 
     * @since S60 5.2
     * 
     * @param aProfileNameCDesCArray Array holding menu item labels
     * @param aItemIdArray Item labels for cover ui
     * @param aPowerMenuItemIndex last used index in power menue 
     */
    void AddMmcMenuItemsL( CDesCArray*& aProfileNameCDesCArray,
                           RArray<TInt>& aItemIdArray, 
                           TInt& aPowerMenuItemIndex );

private:
    //Data members

        /*****************************************************
         * Series 60 Customer / DOS
         * Series 60 Domestic OS API
         *****************************************************/

        CSysApLightsController*         iSysApLightsController;

        CSysApPubSubObserver*           iSysApPubSubObserver;
        CSysApProfileObserver*          iSysApProfileObserver;

        CSysApCenRepLightSettingsObserver*  iSysApCenRepLightSettingsObserver;
        CSysApCenRepLogsObserver*                iSysApCenRepLogsObserver;
        CSysApCenRepBtObserver*                  iSysApCenRepBtObserver;
        CSysApCenRepHacSettingObserver* iSysApCenRepHacSettingObserver;
        CSysApCenRepFmTxObserver*      iSysApCenRepFmTxObserver;
        CSysApCenRepController*                  iSysApCenRepController;
        CSysApStartupController*        iSysApStartupController;
        CSysApConnectionMonitorObserver*	iSysApConnectionMonitorObserver;
        CSysApPowerKeyMenuObserver*     iSysApPowerKeyMenuObserver;
        CSysApSsSettingsObserver*       iSysApSsSettingsObserver;
        CSysApShutdownImage*            iSysApShutdownImage;
        CSysApShutdownAnimation*        iSysApShutdownAnimation;
        CSysApEtelConnector*            iSysApEtelConnector;
        CSysApAccessoryObserver*        iSysApAccessoryObserver;
        CSysApMsgSimMemLowQuery*        iSysApMsgSimMemLowQuery;
        CSysApWaitNote*                 iSysApWaitNote;
        CSysApConfirmationQuery*        iSysApConfirmationQuery;
        CSysApConfirmationQuery*        iSysApConfirmationQueryForRestart;
        CSysApFeatureManager*           iSysApFeatureManager;
        MSysApOfflineModeController*    iSysApOfflineModeController;
        CSysApMMCObserver*              iSysApMMCObserver;
        CSysApNspsHandler*              iSysApNspsHandler;
        MSysApUsbIndicator*             iSysApUsbIndicatorController;
        MSysApBtController*             iSysApBtController;
        MSysApBtSapController*          iSysApBtSapController;
        MSysApLocationPrivacyIndicator* iSysApLocationPrivacyIndicator;
        CAknBatteryNotify*              iBatteryNotify;
        CAknSignalNotify*               iSignalNotify;
        CAknGlobalListQuery*            iGlobalListQuery;

        CSystemLock*                    iSysApSystemLock;
        MProfileEngine*                 iProfileEngine;
        MProfilesNamesArray*            iProfileNamesArray;
        RStarterSession                 iStarterSession;
        RAknKeylock2                    iKeyLock;
#ifndef RD_MULTIPLE_DRIVE
        CAknMemoryCardDialog*           iMemoryCardDialog;
#endif // RD_MULTIPLE_DRIVE
        TInt                            iCapturedEKeyPowerOff;
        TInt                            iCapturedEKeyPowerOffUpAndDowns;
        TInt                            iCapturedAppskey;
        TInt                            iCapturedAppskeyUpAndDowns;


        TInt                            iNumberOfProfileNamesInPowerKeyMenu;

        TBool                           iPowerkeyMenuLockKeypadShown;
        TBool                           iPowerkeyMenuExitSapShown;
        TBool                           iPowerkeyMenuLockSystemShown;
        TBool                           iPowerkeyMenuEjectShown;

        TInt                            iPowerkeyMenuLockKeypadSelection;
        TInt                            iPowerkeyMenuExitSapSelection;
        TInt                            iPowerkeyMenuLockSystemSelection;
#ifndef RD_MULTIPLE_DRIVE
        TInt                            iPowerkeyMenuEjectSelection;
#endif // RD_MULTIPLE_DRIVE

        TInt                            iProfileToBeActivated;
        TInt                            iActiveProfileBeforeOfflineMode;

        TInt                            iApplicationScanningRoundNumberInBtSap;
        TBool                           iTimeToKillInBtSap;

        CAknGlobalNote*                 iProfileNote;
        TInt                            iProfileNoteId;

        TBool                           iLastPowerKeyWasShort;
        TBool                           iIgnoreNextPowerKeyUpEvent;
        TBool                           iIgnoreNextPowerKeyRepeats;
        TBool                           iPowerKeyPopupMenuActive;
        TBool                           iDisablePowerkeyMenu;
        TBool                           iPowerkeyMenuPowerOffShown;

        TBool                           iDeviceLockEnabled;
        TBool                           iKeyLockEnabled;
        TBool                           iKeyLockOnBeforeCall;
        TBool                           iKeyLockOnBeforeAlarm;
        TBool                           iKeyLockOnBeforeCradle;
        TBool                           iShowkeypadActivatedNoteAfterSoftReject;

        TBool                           iEmergencyCallActive;
        TBool                           iSimChangedDone;
        TBool                           iResourcesFreed;
        TBool                           iNsps;
#ifndef RD_MULTIPLE_DRIVE
        TBool                           iHideFirstBeep;
#endif // RD_MULTIPLE_DRIVE
        TBool                           iShutdownStarted;
        TBool                           iAlarmOrChargingStateShutdownStarted;

        TBool                           iIgnoreAccessorySpecificProfileChanges;
        TBool                           iAccessoryJustConnected;

        TBool                           iHandleNextProfileEvent;
        TBool                           iActivateBt;
        TBool                           iDeactivateBt;
#ifndef RD_MULTIPLE_DRIVE
        TBool                           iTimeToKill;                     // MMC Hot Swap
        TInt                            iApplicationScanningRoundNumber; // MMC Hot Swap
#endif // RD_MULTIPLE_DRIVE
        TBool                           iGprsActivated; // used    in defining whether to show GPRS suspended note
        TBool                           iGprsSuspended; // used    in defining whether to show GPRS suspended note
        TBool                           iCallActivated; // used    in defining whether to show GPRS suspended note
        TBool                           iGprsSuspendedNoteShown;

        TBool                           iMMCEjectUsed;
#ifndef RD_MULTIPLE_DRIVE
        TBool                           iMMCPowerMenuEjectUsed;
        TBool                           iMMCInserted;
#endif // RD_MULTIPLE_DRIVE

        CPeriodic*                      iTimer;
        CPeriodic*                      iSapTimer;
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        CPeriodic*                      iAnimTimer;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        CSysApTimer*                    iSysApTimer;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        TInt                            iAnimationShowingTime;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        CSysApKeySndHandler*            iSysApKeySndHandler;

        TBool                           iIgnoreZeroNetworkBarNotifications;

        TBool                           iCharging;  // Indicates if charging is currently on
        CSysApKeyManagement* 			iSysApKeyManagement;

        CSysApMediatorObserver*         iSysApMediatorObserver;
        TBool                           iShutdownContinued;

        /**
        * The default key handler.
        * Own.
        */
        CSysApDefaultKeyHandler* iSysApDefaultKeyHandler;

        TBool iPowerKeyPopupMenuDismissed; // Indicates if the popup menu has been dismissed

#ifndef RD_MULTIPLE_DRIVE
        TBool iHideNextBeep; // Prevents double beep when USB file transfer gets activated
        TBool iFileManagerCloseDisabled; // Prevents File Manager to close on hotswap
#endif // RD_MULTIPLE_DRIVE

        CSysApAudioRoutingObserver* iSysApAudioRoutingObserver;

        /**
        * Call forwarding status CenRep key observer.
        * Own.
        */
        CSysApCenRepCallForwardingObserver* iSysApCenRepCallForwardingObserver;

        /**
        * Message waiting status CenRep key observer.
        * Own.
        */
        CSysApCenRepMsgWaitingObserver* iSysApCenRepMsgWaitingObserver;

#ifdef RD_MULTIPLE_DRIVE
        // First power menu index used for eject
        TInt iPowerkeyMenuEjectSelectionBase;

        /**
        * Drive list container
        * Own.
        */
        CSysApDriveList* iSysApDriveList;

        /**
        * Drive unlock handler.
        * Own.
        */
        CSysApDriveUnlockHandler* iSysApDriveUnlockHandler;

        /**
        * Drive eject handler.
        * Own.
        */
        CSysApDriveEjectHandler* iSysApDriveEjectHandler;

        /**
        * Array for storing inserted memory cards.
        * Own.
        */
        RArray< CSysApDriveList::TMemoryCardEntry > iInsertedMemoryCards;

        // Stores drive for dismount query
        TInt iDriveToDismount;

        // Stores drive for eject query
        TInt iDriveToEject;
#endif // RD_MULTIPLE_DRIVE

        /**
        * For USB charger detection.
        */
        TSysApUsbChargerDetector iSysApUsbChargerDetector;

        /**
        * SysAp power save mode controller.
        * Own.
        */
        CSysApPsmController* iSysApPsmController;
        
        /**
        * SysAp battery info controller.
        * Own.
        */
        CSysApBatteryInfoController* iSysApBatteryInfoController;

    /**
     * Returns whether a de/encrypting operation is ongoing
     *
     * @since S60 3.1
     * @return TBool ETrue if a de/encrypting operation is in progress
     */
    TBool IsEncryptionOperationOngoingL() const;
    TInt iKeyBoardRepeatCount;
 
    //Offset of profile-related menu items in power key menu
    TInt iProfileItemsOffset;
    };

#endif      // SYSAPAPPUI_H

// End of File
