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
*     This class export ECom interface implementations.
*
*/


#include <e32std.h>
#include <ecom/implementationproxy.h>

#include "StarterRFSPlugin.h"

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] =
    {
    	IMPLEMENTATION_PROXY_ENTRY(0x2000F8B0,	CStarterRFSPlugin::NewL)
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    }

