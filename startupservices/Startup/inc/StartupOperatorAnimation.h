/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*           This class is the container class of the CStartupOperatorAnimation.
*           It is used for showing predefined animation.
*
*/



#ifndef STARTUPOPERATORANIMATION_H
#define STARTUPOPERATORANIMATION_H

// INCLUDES
#include <coecntrl.h>
#include "Startup.hrh"
#include "StartupWelcomeAnimation.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CStartupModel;
class CAknBitmapAnimation;
class CStartupAppUi;

// CLASS DECLARATION

/**
*  This class takes care of showing welcome animatio to the user.
*/
class CStartupOperatorAnimation : public CStartupWelcomeAnimation 
    {
    public:


        /**
        * Two-phased constructor.
        */
        static CStartupOperatorAnimation* NewL( CStartupAppUi* aStartupAppUi, const TRect& aRect);

        /**
        *  This handles the key events in this control.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

    private:
        /**
        *  C++ default constructor.
        */
        CStartupOperatorAnimation( CStartupAppUi* aStartupAppUi );

        /**
        *  EPOC default constructor
        */
        void ConstructL(const TRect& aRect);

		/**
        *  Is called by Draw()-function and contains
        *  the drawing intelligence about different states of the execution.
        */
        void DoDrawing() const;
    };

#endif      // STARTUPOPERATORANIMATION_H
            
// End of File
