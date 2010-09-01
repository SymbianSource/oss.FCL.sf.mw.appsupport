/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*           This class is the container class of the CStartupUerWelcomeNote.
*           It is used for showing predefined animation.
*
*/



#ifndef STARTUPWELCOMEANIMATION_H
#define STARTUPWELCOMEANIMATION_H

// INCLUDES
#include <coecntrl.h>
#include "Startup.hrh"

// CONSTANTS

// FORWARD DECLARATIONS
class CStartupModel;
class CAknBitmapAnimation;
class CStartupAppUi;

// CLASS DECLARATION

/**
*  This class takes care of showing welcome animatio to the user.
*/
class CStartupWelcomeAnimation : public CCoeControl , MCoeControlObserver 
    {
    public:  // Constructors and destructor

        /**
        *  C++ default constructor.
        */
        CStartupWelcomeAnimation( CStartupAppUi* aStartupAppUi );

        /**
        * Two-phased constructor.
        */
        static CStartupWelcomeAnimation* NewL( CStartupAppUi* aStartupAppUi, const TRect& aRect);

        /**
        *  Destructor
        */
        ~CStartupWelcomeAnimation();      

        /**
        *  This handles the pointer events in this control.
        */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);

        /**
        *  This handles the key events in this control.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

        /**
        *  This cancels the animation.
        */
        void CancelAnimation();

        /**
        *  This function performs the actual user welcome note showing.
        */
        void StartL();
    
        /**
        *  This makes the animation module to stop showing animation.
        */
        void EndAnimation() const;
        
        /**
        *  Returns the whole duration of the animation, in milliseconds.
        */
        TInt ShowingTime();

        /**
        *  Update screen with white bitmap.
        */
        void DrawBlankScreen() const;

        /**
        *  EPOC default constructor
        */
        void ConstructL(const TRect& aRect);

        /**
        *  Set the info about which state in execution the 
        *  application is, so that view class can draw window 
        *  properly.
        */
        void UpdateDrawInfo( TStartupDrawInfo aValue );

        /**
        *  Removes the splashscreen
        *  @return   void
        */
        void RemoveSplashScreen() const;

        /**
        *  Tells is the animation cancelled by user
        *  @return   TBool
        */
        TBool IsAnimationCancelled();

        /**
        *  Sets the iAnimationShowing member value
        *  @return   void
        */
        void SetAnimationShowing(TBool aValue);
        
        /**
        * Handle resource change
        * @param aType Type of change
        */
        void HandleResourceChange(TInt aType);        
        
        //TEJ
        void CancelAnimCancelTimer();
    private:
        
        /**
        *  Is called when size is changed.
        */
        void SizeChanged();

        /**
        *  Returns the count of the components in the container.
        */
        TInt CountComponentControls() const;

        /**
        *  Returns the component specified by aIndex parameter.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
        *  Handles the event of the control.
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

        /**
        *  Is called by Draw()-function and contains
        *  the drawing intelligence about different states of the execution.
        */
        virtual void DoDrawingL() const;

    private: // Functions from base classes
        
        /**
        *  Returns the count of the components in the container.
        */
        void Draw(const TRect& aRect) const;
    
    protected: // Data

        //Used for showing animation    
        CAknBitmapAnimation *iAnim; //owns

        //Used for showing white background
        CFbsBitmap* iBackgroundBitmap; //owns

        //Is used for setting the timer over the animation
        TInt iShowingTime; //in milli

        //stores the information about the execution state
        //of the application. This information is used
        //in drawing properly in current situation.
        TStartupDrawInfo iDrawUpdateInfo;

        //is uded for guarantee that RStarterSession is made
        //and used only once when RemoveSplashScreen is called.
        //In other words it is used for preventing needless work...
        TBool iSplashScreenRemoved;

        //is used when user cancels the welcome note showing by
        //pressing any key. 
        CStartupAppUi* iStartupAppUi; //uses

        //is used when user cancels the welcome note showing by
        //pressing any key. The reason for using callback in OfferKeyEvent()
        //guarantees that EKeyWasConsumed is returned properly before application
        //continues the tight execution.
        CPeriodic* iAnimCancelTimer; //owns

        //used for telling when the animation is showing
        TBool iAnimationShowing;

        //used for telling if animation is cancelled by user.
        TBool iAnimationCancelled;
    };

#endif      // STARTUPWELCOMEANIMATION_H
            
// End of File
