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
*		Implements Exe interface for HW builds.
*
*
*/


#include "rfsServer.h"

TInt SetSignal( CRfsServer::TSignal& aSignal )
    {
    TBuf<50> buf;
    User::CommandLine( buf );
    TInt ret( KErrNone );
    ret = aSignal.Set( buf );
    return ret;
    }

GLDEF_C TInt E32Main()
    {
    CRfsServer::TSignal signal;
    TInt err = SetSignal( signal );

    if ( !err )
        {
        err = CRfsServer::ThreadStart( signal );
        }

    return err;
    }
