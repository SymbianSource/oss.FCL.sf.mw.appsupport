/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*                which disables LSK if no city/country match is there
*
*/


// INCLUDE FILES

#include "StartupPopupList.h"
#include <aknsfld.h>
#include "StartupDefines.h"

// ---------------------------------------------------------------------------
// CStartupPopupList::CStartupPopupList()
// 
// ---------------------------------------------------------------------------
CStartupPopupList::CStartupPopupList()
    {   
    }

// ---------------------------------------------------------------------------
// CStartupPopupList::~CStartupPopupList()
// ---------------------------------------------------------------------------
//
CStartupPopupList::~CStartupPopupList()
    {
    }

// ---------------------------------------------------------------------------
// CStartupPopupList::NewL()
// ---------------------------------------------------------------------------
//
CStartupPopupList* CStartupPopupList::NewL(
                                    CAknSinglePopupMenuStyleListBox* aListBox, 
                                    TInt aCbaResource,
                                    AknPopupLayouts::TAknPopupLayouts aType)
    {
    TRACES("CStartupPopupList::NewL()");
    CStartupPopupList* self = new(ELeave)CStartupPopupList();        
    CleanupStack::PushL(self);
    self->ConstructL(aListBox, aCbaResource, aType);
    CleanupStack::Pop(); // self
    TRACES("CStartupPopupList::NewL(): End");
    return self;
    }

// ---------------------------------------------------------------------------
// CStartupPopupList::ConstructL()
// Symbian OS second phase constructor 
// ---------------------------------------------------------------------------
//
 void CStartupPopupList::ConstructL(CAknSinglePopupMenuStyleListBox* aListBox, 
                                    TInt aCbaResource,
                                    AknPopupLayouts::TAknPopupLayouts aType)
    {
    TRACES("CStartupPopupList::ConstructL()");
    CAknPopupList::ConstructL(aListBox, aCbaResource,aType);
    TRACES("CStartupPopupList::ConstructL(): End");
    }


// ---------------------------------------------------------------------------
// CStartupPopupList::ListBoxItemsChanged()
// Handle the ListBox Change Event and Disable "Select" & "MSK" CBA Buttons
// ---------------------------------------------------------------------------
//
void CStartupPopupList::ListBoxItemsChanged(CEikListBox* aListBox)
    {
    TRACES("CStartupPopupList::ListBoxItemsChanged()");
    // get the CBA button group container
    CEikButtonGroupContainer* cbaContainer = ButtonGroupContainer();
    // check if there's no match of items
    if( !aListBox->Model()->NumberOfItems() )
        {
        // Disable the 'Select' button 
        cbaContainer->MakeCommandVisible(EAknSoftkeySelect,EFalse);
        // Disable the 'Middle softkey' button
        cbaContainer->MakeCommandVisibleByPosition(
                      CEikButtonGroupContainer::EMiddleSoftkeyPosition,EFalse);
        }
    // check if 'Select' is disabled
    else if(!cbaContainer->IsCommandVisible(EAknSoftkeySelect)) 
        {
        // Enable the 'Select' button if disabled
        cbaContainer->MakeCommandVisible(EAknSoftkeySelect,ETrue);
        // Enable the 'Middle softkey' button if disabled
        cbaContainer->MakeCommandVisibleByPosition(
                      CEikButtonGroupContainer::EMiddleSoftkeyPosition,ETrue);
        }
    TRACES("CStartupPopupList::ListBoxItemsChanged(): End");
    }

// ---------------------------------------------------------------------------
// CStartupPopupList::ActivateL()
// Activate the View and add the ListBox Observer
// ---------------------------------------------------------------------------
//
void CStartupPopupList::ActivateL()
    {
    TRACES("CStartupPopupList::ActivateL()");
    // call Base class ActivateL()
    CAknPopupList::ActivateL();
    // add the listbox item change observer
    ListBox()->AddItemChangeObserverL(this);
    TRACES("CStartupPopupList::ActivateL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupPopupList::HandlePointerEventL()
// ---------------------------------------------------------------------------
//
void CStartupPopupList::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    TRACES("CStartupPopupList::HandlePointerEventL()");
    TRACES1("CStartupPopupList::HandlePointerEventL: aPointerEvent.iType == %d",aPointerEvent.iType );
    
    /* Handle all taps except when EButton1Down outside of country/city query 
       -> query is not cancelled and scroll bar does not remain pressed down */
    if ( Rect().Contains( aPointerEvent.iPosition ) || ( !Rect().Contains( aPointerEvent.iPosition ) &&
         aPointerEvent.iType != TPointerEvent::EButton1Down ) )
        {
        CAknPopupList::HandlePointerEventL( aPointerEvent );
        }
    
    TRACES("CStartupPopupList::HandlePointerEventL(): End");
    }

// End of File
