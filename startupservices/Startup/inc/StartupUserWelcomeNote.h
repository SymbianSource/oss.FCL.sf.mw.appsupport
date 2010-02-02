/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*           It is used for showing user defined image or text.
*
*/


#ifndef STARTUPUSERWELCOMENOTE_H
#define STARTUPUSERWELCOMENOTE_H

// INCLUDES
#include <coecntrl.h>
#include "startup.hrh"
#include "startupdomaincrkeys.h"

// CONSTANTS
const TInt KStartupTBufMaxLength( 100 );

// FORWARD DECLARATIONS
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
class CStartupView;
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
class CStartupModel;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
class CStartupAppUi;

// CLASS DECLARATION

/**
*  This class takes care of showing user welcome note to the user.
*  User welcome note type can be predefined default animation,
*  user defined image or text.
*/
class CStartupUserWelcomeNote
  : public CCoeControl
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
  , MCoeControlObserver
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    {
    public:  // Constructors and destructor

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Two-phased constructor.
        *
        * @param aView The compound control that is the container for this
        * control.
        */
        static CStartupUserWelcomeNote* NewL(
            CStartupAppUi& aStartupAppUi,
            const TRect& aRect,
            CStartupView& aView );
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        *  C++ default constructor.
        */
        CStartupUserWelcomeNote( CStartupAppUi& aStartupAppUi );

        /**
        * Two-phased constructor.
        */
        static CStartupUserWelcomeNote* NewL( CStartupAppUi& aStartupAppUi, const TRect& aRect);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        *  Destructor
        */
        ~CStartupUserWelcomeNote();

        /**
        *  Returns the information about the type of 
        *  the selected welcome note. This also consider
        *  the product variant types of welcome note.
        *  Return type can be animation, text or image.
        */
        TStartupNoteTypeInformation NoteTypeInformation();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        *  This handles the key events in this control.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

        /**
        *  This cancels iNoteCancelTimer
        */
        void CancelNoteCancelTimer();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        *  This function performs the actual user welcome note showing.
        */
        void StartL();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        *  Update screen with white bitmap.
        */
        void DrawBlankScreen() const;

        /**
        *  EPOC default constructor
        */
        void ConstructL(const TRect& aRect);

        /**
        *  Sets the iUserWelcomeNoteShowing member value
        *  @return   void
        */
        void SetUserWelcomeNoteShowing(TBool aValue);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    private:
        
        /**
        *  Returns the component specified by aIndex parameter.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        *  Handles the event of the control.
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        *  Loads the data about selected welcome note info from the Cental Repository.
        */
        TInt GetUserWelcomeNoteTypeInfo();

        /**
        *  Shows the text type of user welcome note.
        */
        void ShowInformationNoteWrapperL();

        /**
        *  Returns information about the selected user welcome note type.
        *  Text, image or default(no note).
        */
        TStartupWelcomeNoteType UserWelcomeNoteType();

        /**
        *  Draws a image type of welcome note to the center of 
        *  the screen. Both user selected image and operator
        *  image as a product variant feature.
        */
        void DrawImageWelcomeNote();

        TInt CheckImage( const TDesC& aPath);

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    private:

        /**
        *  C++ default constructor.
        *
        * @param aView The compound control that is the container for this
        * control.
        */
        CStartupUserWelcomeNote( CStartupAppUi& aStartupAppUi, CStartupView& aView );

        /**
        * Second phase constructor.
        */
        void ConstructL( const TRect& aRect );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    private: // Data

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /** Parent control for the animation control(s). */
        CStartupView& iView;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        //Used for showing user selected image
        CFbsBitmap* iBitmap; //owns
        
        //contains the type information of the user welcome note as selected from
        //General Settings (variant information doesn't include in this)
        TStartupWelcomeNoteType iNoteType;

        //contains the possible variation information of the default user welcome note
        TStartupUserWelcomeNoteDefaultVariationType iNoteDefaultVariationType;

        //is used for storing image path information
        TBuf<KStartupTBufMaxLength> iNotePath;

        //is used for storing text note information
        TBuf<KStartupTBufMaxLength> iNoteText;

        //is used for storing image path information
        TBuf<KStartupTBufMaxLength> iNoteOperPath;

        //is used for storing text note information
        TBuf<KStartupTBufMaxLength> iNoteOperText;

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        //stores the information about the execution state
        //of the application. This information is used
        //in drawing properly in current situation.
        TStartupDrawInfo iDrawUpdateInfo;

        //is uded for guarantee that RStarterSession is made
        //and used only once when RemoveSplashScreen is called.
        //In other words it is used for preventing needless work...
        TBool iSplashScreenRemoved;

        //is used in redrawing in various execution phases (DoDrawingL()-function)
        TStartupNoteTypeInformation iWelcomeNoteType;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        //is used when user cancels the welcome note showing by
        //pressing any key. 
        CStartupAppUi& iStartupAppUi; //uses

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        //is used when user cancels the welcome note showing by
        //pressing any key. The reason for using callback in OfferKeyEvent()
        //guarantees that EKeyWasConsumed is returned properly before application
        //continues the tight execution.
        CPeriodic* iNoteCancelTimer; //owns

        //used for telling when the UWN is showing
        TBool iUserWelcomeNoteShowing;

        //used for telling if UWN is cancelled by user.
        TBool iUserWelcomeNoteCancelled;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    };

#endif      // STARTUPUSERWELCOMENOTE_H
            
// End of File
