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
 * Description:
 *
 */

// SYSTEM INCLUDES

// USER INCLUDES
#include <ssm/ssmcustomcommand.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CSsmCustomCommandEnv::CSsmCustomCommandEnv( RFs& aRfs )
:iFs( aRfs )
    {

    }


// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CSsmCustomCommandEnv* CSsmCustomCommandEnv::NewL( RFs& aRfs )
    {
    CSsmCustomCommandEnv* self = new ( ELeave ) CSsmCustomCommandEnv( aRfs );
    return self;
    }


// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CSsmCustomCommandEnv::~CSsmCustomCommandEnv()
    {

    }
// ---------------------------------------------------------------------------
// CSsmCustomCommandEnv::Rfs
// ---------------------------------------------------------------------------
//
const RFs& CSsmCustomCommandEnv::Rfs() const
    {
    return iFs;
    }
// End of file
