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
*           This class adds send-key to acknowledge the time/date query.
*           Feature is implemented to needs of the PET-chamber in production tests.
*           More information can be found in Change Request-database.
*
*/


#ifndef STARTUPQUERYDIALOG_H
#define STARTUPQUERYDIALOG_H

// INCLUDES
#include <AknQueryDialog.h>       //used for Time and Date query

//  FORWARD DECLARATIONS

// CLASS DECLARATION

// CONSTANTS

// CLASS DECLARATION

/**
* CStartupQueryDialog class.
*/
class CStartupQueryDialog : public CAknTimeQueryDialog
    {
    public://construction and destruction
        /**
         * C++ Constructor.
         */
        CStartupQueryDialog(TTime& aTime,const TTone aTone = ENoTone);

        /**
         * C++ Destructor.
         */
        virtual ~CStartupQueryDialog();

    public:// from CCoeControl

        /**
        * From CCoeControl  Handle key events. When a key event occurs, 
        *                   CONE calls this function for each control on the control stack, 
        *                   until one of them returns EKeyWasConsumed to indicate that it processed the key event.  
        * @param aKeyEvent  The key event.
        * @param aType      The type of the event: EEventKey, EEventKeyUp or EEventKeyDown.
        * @return           Indicates whether or not the key event was used by this control.
        */
	    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

    public://from MAknQueryControlObeserver

		/**
		* Called by OfferkeyEventL(), gives a change to dismiss the query with
		* send/answer key.
		*/
		virtual TBool NeedToDismissQueryL(const TKeyEvent& aKeyEvent);
		
		/**
		* Called by NeedToDismissQueryL(), gives a change to either accept or reject
		* the query. Default implementation is to accept the query if the Left soft
		* key is displayed and reject it otherwise. Left softkey is only displayed if
		* the query has valid data into it.
		*/
		virtual void DismissQueryL();

        /**
        * Returns whether the left softkey is visible
        * @return ETrue is the left softkey is visible
        */
        TBool IsLeftSoftkeyVisible(); 

    };

#endif // STARTUPAPPLICATION_H

// End of file
