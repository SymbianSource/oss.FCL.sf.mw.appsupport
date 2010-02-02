/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ECOM proxy table for GSAccessoryPlugin
*
*/


#include <ecom/implementationproxy.h>

#include "gsaccessoryplugin.h"

// LOCAL CONSTANTS AND MACROS
const TImplementationProxy KImplTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x1020743D,	CGSAccessoryPlugin::NewL )
	};

// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Factory function
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount )
	{
	aTableCount = sizeof( KImplTable ) / sizeof( TImplementationProxy );
	return KImplTable;
	}
