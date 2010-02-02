/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation for DllMain.
*
*/


#include "batindicatorpanepluginimpl.h"

#include <ecom/implementationproxy.h>
#include <AknIndicatorPluginImplUIDs.hrh>

// ---------------------------------------------------------------------------
// Define the interface UIDs
// Map the interface implementation UIDs to implementation factory functions
// ---------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
#ifdef KImplUIDBatteryIndicatorPlugin
    IMPLEMENTATION_PROXY_ENTRY( KImplUIDBatteryIndicatorPlugin, 
                                CBatIndicatorPanePluginImpl::NewL )
#else
    IMPLEMENTATION_PROXY_ENTRY( 0x2000E51D, 
                                CBatIndicatorPanePluginImpl::NewL )
#endif //KImplUIDBatteryIndicatorPlugin                                
    };
	
// ---------------------------------------------------------------------------
// Exported proxy for instantiation method resolution.
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy
        (TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(ImplementationTable[0]);
    return ImplementationTable;
    }
    
// End of file
