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


#ifndef UTOUTOFDISKPSOBSERVER_H
#define UTOUTOFDISKPSOBSERVER_H

// USER INCLUDES
#include <e32base.h>
#include "outofdiskpsobserver.h"

// CLASS DECLARATION
class COutOfDiskMonitor;

class CUTOutOfDiskPSObserver : public COutOfDiskPSObserver
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CUTOutOfDiskPSObserver* NewL( COutOfDiskMonitor* aOutOfDiskMonitor );

    public: // From MSysApPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );
    };

#endif      // UTOUTOFDISKPSOBSERVER_H   
            
// End of File
