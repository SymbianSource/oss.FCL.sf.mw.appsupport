/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Environment utils.
*
*/


#ifndef CFENVUTILS_H
#define CFENVUTILS_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Context FW environment utils.
 *
 * @lib None.
 * @since S60 5.1
 */
class CFEnvUtils
    {
public:

    /**
     * Enables / disbales screen saver.
     * @param aWasEnabled Returns the current value of screen saver.
     * @param aEnable Enable screen saver.
     * @return TInt KErrNone if no errors.
     */
    static TInt EnableScreenSaver( TBool& aWasEnabled, const TBool aEnable );
    
    /**
     * Enables / disbales screen saver.
     * @param aEnable Enable screen saver.
     * @return TInt KErrNone if no errors.
     */
    static TInt EnableScreenSaver( const TBool aEnable );

private:

    /**
     * C++ constructor.
     */
    CFEnvUtils();

private:
    // data
    };

#endif // CFENVUTILS_H
