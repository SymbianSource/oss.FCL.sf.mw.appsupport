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
* Description: This file contains a mock CRemConTspControllerNoError implementation whose CreateAfterIdleL can return different value according the parameter passed by NewL.
*
*/

#include "mockremcontspcontroller.h"

// -----------------------------------------------------------------------------
// CRemConTspController::CRemConTspController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CRemConTspController::CRemConTspController(MockControllerCtlValue aCtlValue) : iCtlValue(aCtlValue)
    {
    }
    
// -----------------------------------------------------------------------------
// CRemConTspController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRemConTspController* CRemConTspController::NewL(MockControllerCtlValue aCtlValue)
    {    
    CRemConTspController* self = new( ELeave ) CRemConTspController(aCtlValue);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CRemConTspController::ConstructL()
    {
    iIdle = CRemConIdleListener::NewL(*this);
    }

// Destructor
CRemConTspController::~CRemConTspController()
    {
    if( iIdle )
        {
        delete iIdle;
        iIdle = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CRemConTspController::CreateAfterIdleL
// Define the contexts when idle state has been loaded.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CRemConTspController::CreateAfterIdleL()
    {
    if (iCtlValue == ECorrect)
        {
        return;
        }
    
    if (iCtlValue == ELeaveNoChange)
        {
        User::Leave(KErrNoMemory);
        }
    
    if (iCtlValue == ELeaveChange)
        {
        // delete iIdle now, so that CRemConIdleListener won't call this method again if a leave occurs hereafter
        if( iIdle )
            {
            delete iIdle;
            iIdle = NULL;
            }
        }
    }
