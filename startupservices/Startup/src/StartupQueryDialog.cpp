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
*           This class adds send-key to acknowledge the time/date query.
*           Feature is implemented to needs of the PET-chamber in production tests.
*           More information can be found in Change Request-database.
*
*/



// INCLUDE FILES
#include "StartupQueryDialog.h"

// ========================= MEMBER FUNCTIONS ================================

// -----------------------------------------------------------------------------
// CStartupQueryDialog::CStartupQueryDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CStartupQueryDialog::CStartupQueryDialog( TTime& aTime, const TTone aTone ) :
    CAknTimeQueryDialog( aTime, aTone )
    {
    }

// Destructor
CStartupQueryDialog::~CStartupQueryDialog()
    {
    }

// -----------------------------------------------------------------------------
// CStartupQueryDialog::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CStartupQueryDialog::OfferKeyEventL(const TKeyEvent& aKeyEvent, 
                                                            TEventCode aType)
    {
    if( aType != EEventKey )
        return EKeyWasNotConsumed;

    if( NeedToDismissQueryL( aKeyEvent ) )
            return EKeyWasConsumed;
    return CAknDialog::OfferKeyEventL(aKeyEvent,aType);
    }

// -----------------------------------------------------------------------------
// CStartupQueryDialog::NeedToDismissQueryL
// Allows user to acknowledge time and date queries with the send key.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CStartupQueryDialog::NeedToDismissQueryL(const TKeyEvent& aKeyEvent)
    {
    if (aKeyEvent.iCode == EKeyPhoneSend)
        {
        DismissQueryL();
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CStartupQueryDialog::DismissQueryL
// Query is accepted if the left softkey is displayed 
// (left softkey is displayed only if there is valid data in the query).
// Query is discarded if the left softkey is not displayed.
// Clients can override this and implement something different.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CStartupQueryDialog::DismissQueryL()
    {
    if (IsLeftSoftkeyVisible())
        {
        TryExitL(EEikBidOk);
        }
    }

// -----------------------------------------------------------------------------
// CStartupQueryDialog::IsLeftSoftkeyVisible
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CStartupQueryDialog::IsLeftSoftkeyVisible()
    {
    return ButtonGroupContainer().ButtonGroup()->IsCommandVisible(
                      ButtonGroupContainer().ButtonGroup()->CommandId(0));
    }

// End of file
