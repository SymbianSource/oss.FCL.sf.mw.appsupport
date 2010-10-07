/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: COutOfDiskPSObserver contains a protected function and some private functions which can't be directly accessed 
* by test code, so that this derived class is used to public those functions.
*
*/

// USER INCLUDES
#include "utoutofdiskpsobserver.h"
#include "outofdiskmonitor.h"

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApPubSubObserver::NewL() 
// ----------------------------------------------------------------------------
CUTOutOfDiskPSObserver* CUTOutOfDiskPSObserver::NewL( COutOfDiskMonitor* aOutOfDiskMonitor )
    {
    return static_cast<CUTOutOfDiskPSObserver*> (COutOfDiskPSObserver::NewL(aOutOfDiskMonitor));
    }


// ----------------------------------------------------------------------------
// CSysApPubSubObserver::HandlePropertyChangedL()
// ----------------------------------------------------------------------------
void CUTOutOfDiskPSObserver::HandlePropertyChangedL( const TUid& aCategory, const TUint aKey )
    {
    COutOfDiskPSObserver::HandlePropertyChangedL(aCategory, aKey);
    }

// End of File
