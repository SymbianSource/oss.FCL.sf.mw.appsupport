/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class implements a customized pop-up 
	             which disables LSK if no city/country match is there
*
*/


#ifndef STARTUPPOPUPLIST_H
#define STARTUPPOPUPLIST_H

#include <aknPopup.h>
#include <aknlists.h>
#include <eiklbx.h>

NONSHARABLE_CLASS(CStartupPopupList) : public CAknPopupList, 
                                       public MListBoxItemChangeObserver 
{
    private:
        
        /**
        * Constructor
        */
        CStartupPopupList();

        /**
        * @param aListBox       Pre-existing listbox-derived class
        * @param aCbaResource   Softkey pane to display while pop-up is active
        */
        void ConstructL(CAknSinglePopupMenuStyleListBox* aListBox, 
                        TInt aCbaResource, AknPopupLayouts::TAknPopupLayouts aType);
    
    public:
        /**
        * Destructor
        */    
        ~CStartupPopupList();

        /**
        * Two-phased constructor.
        * @param aListBox       Pre-existing listbox-derived class
        * @param aCbaResource   Softkey pane to display while pop-up is active
        * @return CStartupPopupList*
        */
        static CStartupPopupList* NewL(CAknSinglePopupMenuStyleListBox* 
                                       aListBox, TInt aCbaResource, 
                                       AknPopupLayouts::TAknPopupLayouts aType);
                                     
        /**
        * From CCoeControl, handle activation of control.
        */
        void ActivateL();

        /**
        * From MListBoxItemChangeObserver, handle enable/disable LSK
        */                            
        void ListBoxItemsChanged(CEikListBox* aListBox); 

        /**
        *  From CCoeControl, handles pointer events in this control.
        */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);
};

#endif //STARTUPPOPUPLIST_H



