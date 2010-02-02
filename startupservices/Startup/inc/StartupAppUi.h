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
* Description: 
*       AppUi class of the application.
*       Includes the execution sequence which contains
*           1. selftest check
*           2. security check
*           3. user welcome note showing
*           4. first boot check
*           5. time and date value queries
*           6. real time clock status check
*
*/


#ifndef STARTUPAPPUI_H
#define STARTUPAPPUI_H

// FLAGS
//#define USE_STARTUPTEST_APP


// SYSTEM INCLUDES
#include <aknappui.h>           //appui

#include <data_caging_path_literals.hrh>
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
 #include <secondarydisplay/SecondaryDisplaySystemStateAPI.h>
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>


// USER INCLUDES
#include "startup.hrh"          //internal state types
#include "StartupDefines.h"     //some common defines
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
 #include "StartupTone.h"
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


// CONSTANTS
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
 const TInt KConnectionRetryTime = 50000;        // 50 ms
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// FORWARD DECLARATIONS
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
 class CStartupWelcomeAnimation;
 class CStartupOperatorAnimation;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
class CStartupUserWelcomeNote;
class CStartupPubSubObserver;
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
 class CStartupAnimationWrapper;
 class CStartupView;
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
 class CStartupTone;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
class CStartupMediatorObserver;

/**
*  'AppUi' class.
*
*/
class CStartupAppUi : public CAknAppUi
{
    public: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CStartupAppUi();

        /**
        * Destructor.
        */
        ~CStartupAppUi();

        // from CCoeAppUiBase
        void PrepareToExit();

    public:

        /**
        *  Actually stops the timer that is used for showing
        *  User Welcome Note.
        *  @return    void
        */
        void StopTimingL(); // indirectly called

        /**
        *  Just perform Exit(); from the callback function.
        *  @return    void
        */
        void ExitApplication(); // indirectly called

        /**
        *  Callback function to stop timer
        *  This timer is used for showing user welcome note.
        *  @return    TInt
        */
        static TInt DoStopTimingL(TAny* aObject);

        /**
        *  This callback function is used for exiting the application after
        *  some code in somewhere else in this application is finished.
        */
        static TInt DoExitApplication(TAny* aObject);

        /**
        * Check boot reason from the SysUtil
        * @param        None
        * @return       TBool
        */
        TBool HiddenReset();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Continue startup when startup tone completes.
        * @param        None
        * @return       void
        */
        void ContinueStartupAfterToneL(TToneType aToneType);

        /**
        * Stop startuptone
        * @param        None
        * @return       void
        */
        void StopStartupTone();

        /**
        * Stop startuptone
        * @param        None
        * @return       void
        */
        void StopOperatorTone();

        /**
        * Checks if StartupTone is playing
        * @param        None
        * @return       TBool
        */
        TBool StartupTonePlaying();

        /**
        * Checks if OperatorTone is playing
        * @param        None
        * @return       TBool
        */
        TBool OperatorTonePlaying();

#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Called when welcome or operator animation has finished.
        *
        * @since S60 3.2
        */
        void AnimationFinished();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Brings Startup application to foregound after Touch Screen Calibration and emergency call from PIN query.
        * @param        None
        * @return       void
        */
        void BringToForeground();

        /**
        * Send Startup application to background before Touch Screen Calibration.
        * @param        None
        * @return       void
        */
        void SendToBackground();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * Sets iCleanBoot to EStartupCleanBoot.
        * @param        None
        * @return       void
        */
        void SetCleanBoot();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        *
        * @param        None
        * @return       void
        */
        void WaitingTouchScreenCalibL();

#ifdef RD_SCALABLE_UI_V2
        /**
        *
        * @param        None
        * @return       void
        */
        void TouchScreenCalibrationDoneL();
#endif // RD_SCALABLE_UI_V2
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        *
        * @param        None
        * @return       void
        */
		void CoverUIWelcomeAnimationSyncOKL();

        /**
        *
        * @param        None
        * @return       void
        */
		void WaitingCoverUIWelcomeAnimationSyncL();

        /**
        *
        * @param        None
        * @return       void
        */
		void CoverUIOperatorAnimationSyncOKL();

        /**
        *
        * @param        None
        * @return       void
        */
 		void WaitingCoverUIOperatorAnimationSyncL();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        *
        * @param        None
        * @return       void
        */
        void WaitingCoverUIStartupReadySyncL();

        /**
        *
        * @param        None
        * @return       void
        */
		void CoverUIStartupReadySyncOKL();

        /**
        *
        * @param        None
        * @return       void
        */
        void RaiseCoverUIEvent( TUid aCategory,
                                TInt aEventId,
                                const TDesC8& aData );


        void SetCriticalBlockEndedL();
        void WaitingCriticalBlockEndingL();

        /**
        * Starts to wait permission to start startup animation.
        *
        * @since S60 3.2
        */
        void SetWaitingStartupAnimationStartL();

        /**
        * Called when it is allowed to start startup animation.
        *
        * @since S60 3.2
        */
        void WaitingStartupAnimationStartL();

        /** System state has changed to EmergencyCallsOnly. Skip the animations. */
        void SetEmergencyCallsOnlyL();

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Check if animation should be loaded in advance and do it.
        */
        void TryPreLoadAnimation();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * Propagates fatal startup error state.
        * @param        aPropertyChanged
        *               (ETrue, if state received via property changed event)
        * @return       void
        */
        void SwStateFatalStartupErrorL( TBool aPropertyChanged );

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Returns value of iOfflineModeQueryShown.
        * @param        None
        * @return       TBool
        */
        TBool GetOfflineModeQueryShown();

        /**
        * Sets value of iOfflineModeQueryShown.
        * @param        TBool
        * @return       void
        */
        void SetOfflineModeQueryShown(TBool aValue);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * Return value of the__SIMCARD feature
        * @param None
        * @return TBool
        */
        TBool SimSupported();

        /**
        * Return value of KFeatureIdCoverDisplay feature
        * @param None
        * @return TBool
        */
        TBool CoverUISupported();

        /**
        *  Checks if DOS is in Offline Mode
        *
        *  @return    TBool
        */
        TBool DosInOfflineModeL();

        /**
        *  This cancels the animation.
        */
        void CancelAnimation();

        /**
        * Checks if boot reason is SIM changed reset
        * @param        None
        * @return       TBool
        */
        TBool SimStatusChangedReset();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    private:
        /**
        *   For starting startup tone initialization timer when needed
        *   @param      None
        *   @return     void
        */
        void WaitingStartupToneL();

        /**
        *   Callback function of startup tone initialization timer
        *   @param      TAny*
        *   @return     TInt
        */
        static TInt ToneInitTimerTimeoutL(TAny* aObject);

        /**
        *   For checking startup tone initialization status
        *   @param      None
        *   @return     void
        */
        void StartupToneWaitStatusL();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    private: // from MEikMenuObserver

        /**
        * EPOC default constructor.
        */
        void ConstructL();

    private: // from CEikAppUi

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * From CAknAppUi.
        * Handles a change to the application's resources which are shared across
        * the environment.
        *
        * @since S60 3.2
        *
        * @param aType The type of resources that have changed.
        */
        void HandleResourceChangeL( TInt aType );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        *  Takes care of command handling.
        *  @param   aCommand        Command to be handled.
        *  @return  void
        */
        void HandleCommandL(TInt aCommand);

        /**
        *  Handles key events.
        *  @param  aKeyEvent           Event to be handled.
        *  @param  aType               Type of the key event
        *  @return TKeyResponse
        */
        virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

    private:
        /**
        *  Checks the validity of change and perform needed actions
        *  @param   toState     next phase state phase
        *  @return  void
        */
        void DoNextStartupPhaseL( TStartupInternalState toState );

        /**
        *  First part of the startup phases.
        *  This part does some initial things and in the end
        *  calls DoNextStartupPhaseL() with parameter of next phase
        *  @return    void
        */
        void DoStartupStartPartL();

        /**
        *  This part of the startup phases
        *  shows the welcome animation.
        */
        void DoStartupShowWelcomeAnimationL();

        /**
        *  This part of the startup phases
        *  shows the operator animation.
        */
        void DoStartupShowOperatorAnimationL();

        /**
        *  This part of the startup phases
        *  shows the user welcome note.
        */
        void DoStartupShowUserWelcomeNoteL();

        /**
        *  Returns EFalse if date, time and city
		*  queries are disabled for testing purposes
		*  @return TBool
        */
		TBool StartupQueriesEnabled();

        /**
        *  Predictive Time and Country selection support
        *  Returns ETrue when enabled.
        *  @return TBool
        */
		TBool PredictiveTimeEnabled();
		
        /**
        *  This part takes care of asking some values
        *  from the user in case of first boot or
        *  real time clock value is invalid
        *  @return    void
        */
        void DoStartupFirstBootAndRTCCheckL();

        /**
        *  Shows the needed startup queries in first boot
        *  or when real time clock value is invalid
        *  @return    void
        */
        void ShowStartupQueriesL();

        /**
        *  Last part of the startup phases.
        *  This part does some cleaning things and
        *  calls the Exit().
        *  @return    void
        */
        void DoStartupEndPart();

        /**
        * Check Real Time Clock status from the SysUtil
        * @param        None
        * @return       TBool
        */
        TBool RTCStatus();

        /**
        *  Shows welcome animation to the user.
        *  @return    void
        */
        void ShowWelcomeAnimationL();

        /**
        *  Shows operator animation to the user.
        *  @return    void
        */
        void ShowOperatorAnimationL();

        /**
        *  Shows UserWelcomeNote to the user.
        *  It can be default animation, or user defined text or image.
        *  @return    void
        */
        void ShowUserWelcomeNoteL();

        /**
        *  Shows Off-line Mode query to the user in case
        *  the device is being booted into Off-line Mode.
        *  Sends System Application user's answers via
        *  a Shared Data notification.
        *
        *  @return    void
        */
        void ShowOfflineModeQueryL();

        /**
        * Check Off-line Mode status from the UI side
        * @param None
        * @return TBool : Off-line Mode status
        */
        TBool UiInOfflineMode();

        /**
        *  Shows country and city selection lists to the user.
        *  This is shown in first boot.
        */
        void ShowCountryAndCityListsL();

        /**
        *  Shows country selection list to the user.
        *  This is shown in first boot.
        *  @return    TInt
        */
        TInt ShowCountryListL();

        /**
        *  Shows city selection list to the user.
        *  This is shown in first boot.
        *  @return    TBool
        */
        TBool ShowCityListL(TUint8 cityGroupId);

        /**
        *  Shows time query to the user.
        *  This is shown in first boot or when
        *  real time clock isn't valid.
        *  @return    TBool
        */
        TBool ShowTimeQueryL();

        /**
        *  Shows date query to the user.
        *  This is shown in first boot or when
        *  real time clock isn't valid.
        *  @return    TBool
        */
        TBool ShowDateQueryL();

        /**
        *  Gets default time and date from cenrep
        *  @param     aTime
        *  @return    void
        */
        void GetDefaultTimeAndDate( TTime& aTime );

        /**
        *  Returns information about is this the first boot happening.
        *  @return    TBool
        */
        TBool FirstBoot();

        /**
        *  Mark first boot happened for the next boot.
        *  @return   void
        */
        void MarkFirstBoot();

        /**
        *  Includes routines that are performed
        *  when global state change to EStartupSystemFatalError.
        *  This method is called only from DoNextStartupPhaseL()!!
        */
        void SystemFatalErrorL();

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION        
        /**
        * Updates startup UI phase to Publish&Subscribe key KPSStartupUiPhase.
        *
        * @param aValue the new value to be written to the key KPSStartupUiPhase.
        */
        void UpdateStartupUiPhase( TInt aValue );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    private: // ***** Member Data ********************************************

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        // The only window-owning control of the Startup application.
        CStartupView* iMainView;

        // Used for showing Welcome Animation. Owned. May not be NULL.
        CStartupAnimationWrapper* iAnimation;

#else // RD_STARTUP_ANIMATION_CUSTOMIZATION

        //used for showing Welcome Animation
        CStartupWelcomeAnimation* iWelcomeAnimation;  //owns

        //used for showing Operator Animation
        CStartupOperatorAnimation* iOperatorAnimation;  //owns
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        //used for showing User Welcome Note
        CStartupUserWelcomeNote* iUserWelcomeNote;  //owns

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        //used for showing welcome animation
        CPeriodic* iAnimTimer; //owns
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        //used for showing user welcome note
        CPeriodic* iNoteTimer; //owns

        //used for exiting application, smoothly without tricky errors
        CPeriodic* iExitTimer; //owns


#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        // Used for playing startup tone
        CStartupTone* iStartupTone; //owns

        // Used for waiting startup tone initialization
        CPeriodic* iToneInitTimer; //owns

        // Used for playing operator startup tone
        CStartupTone* iOpStartupTone; //owns

        // Used for following tone initialization time
        TInt iToneInitWaitTime;

        //used for telling if the user welcome note is animation
        TBool iAnimation;
#endif RD_STARTUP_ANIMATION_CUSTOMIZATION

        //internal execution state
        TStartupInternalState iInternalState;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        //is used for telling the application that
        //splashscreen should be removed for showing the code queries
        //or welcome note
        TBool iSplashScreenShouldBeRemoved;
#endif RD_STARTUP_ANIMATION_CUSTOMIZATION

        //is used for quarantee only one time continuing
        TBool iStartupFirstBootAndRTCCheckAlreadyCalled;

        //is used for indicating charger or alarm boot
        TBool iChargingOrAlarmBoot;

        //is used to inform if this is the first boot
        TInt iFirstBoot;

        //is used to inform if "clean boot key" is pressed during animation
        TInt iCleanBoot;

        //is used for telling if offline mode is supported
        TBool iOfflineModeSupported;

        //is used for telling if offline mode query is already shown
        TBool iOfflineModeQueryShown;

        //is used for telling if Starter has completed critical block starting
        TBool iCriticalBlockEnded;

        // The result of offline query
        TBool iBootIntoOffline;

        // fatal error during startup
        TBool iSwStateFatalStartupError;

        // Indicates whether it is allowed to start the UI sequence containing animation(s).
        TBool iStartupWaitingShowStartupAnimation;

        CStartupPubSubObserver* iStartupPubSubObserver; //owns

        //is used for telling if SIM card is supported
        TBool iSimSupported;

        CStartupMediatorObserver* iStartupMediatorObserver; //owns

        TBool iCoverUISupported;

        TInt iCounryListIndex;

        TTime iTime;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        TBool iTouchScreenCalibSupport;
        TBool iTouchScreenCalibrationDone;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
};

#endif // STARTUPAPPUI_H

// End of file
