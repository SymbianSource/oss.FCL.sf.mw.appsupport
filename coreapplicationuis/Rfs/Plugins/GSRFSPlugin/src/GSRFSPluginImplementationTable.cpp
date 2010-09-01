/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*        ECOM proxy table for this plugin
*
*/


// System includes
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "GSRFSPluginTraces.h"

// User includes
#include "GSRFSPlugin.h"

// Constants
const TImplementationProxy KGSRFSPluginImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x10275118,	CGSRFSPlugin::NewL )
	};


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
	{
    TRACES("ImplementationGroupProxy()");
	aTableCount = sizeof( KGSRFSPluginImplementationTable ) 
        / sizeof( TImplementationProxy );
    TRACES("ImplementationGroupProxy(): End");
	return KGSRFSPluginImplementationTable;
	}

// End of File
