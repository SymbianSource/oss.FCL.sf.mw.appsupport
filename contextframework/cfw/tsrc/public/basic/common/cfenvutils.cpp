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
* Description:  CFEnvUtils class implamentation.
*
*/


// SYSTEM INCLUDES
#include <eunitmacros.h>
#include <e32property.h>
#include <ScreenSaverInternalPSKeys.h>

// USER INCLUDES
#include "cfenvutils.h"

const TInt KScreenSaverNotAllowed = 666;
const TInt KScreenSaverAllowed = 0;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CFEnvUtils::CFEnvUtils ( )
    {

    }

//------------------------------------------------------------------------------
// CFEnvUtils::EnableScreenSaver
//------------------------------------------------------------------------------
//
TInt CFEnvUtils::EnableScreenSaver( TBool& aWasEnabled, const TBool aEnable )
    {
    TInt err( KErrNone );
    TInt val( KScreenSaverNotAllowed );
    
    err = RProperty::Get( KPSUidScreenSaver,  KScreenSaverAllowScreenSaver, val );
    EUNIT_PRINT( _L("Get screen saver mode: %d"), err );
    if( err == KErrNone )
        {
        aWasEnabled = ( val != KScreenSaverAllowed );
        err = EnableScreenSaver( aEnable );
        }
    
    return err;
    }

//------------------------------------------------------------------------------
// CFEnvUtils::EnableScreenSaver
//------------------------------------------------------------------------------
//
TInt CFEnvUtils::EnableScreenSaver( const TBool aEnable )
    {
    TInt err( KErrNone );

    TInt val = 0;
    if( aEnable )
        {
        val = KScreenSaverAllowed;
        }
    else
        {
        val = KScreenSaverNotAllowed;
        }
    err = RProperty::Set( KPSUidScreenSaver,  KScreenSaverAllowScreenSaver, val );
    EUNIT_PRINT( _L("Set screen saver mode: %d"), err );
    
    User::ResetInactivityTime();
    
    return err;
    }

// End of file
