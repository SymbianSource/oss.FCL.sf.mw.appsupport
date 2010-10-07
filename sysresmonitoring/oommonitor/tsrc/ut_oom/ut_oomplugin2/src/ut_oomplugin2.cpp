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
 * Description:  
 *
 */

#include "ut_oomplugin2.h"

#define DECLARE_CREATE_INSTANCEV2(UID) CUtOomMonitorPluginV2* CreateInstance##UID() { return CUtOomMonitorPluginV2::NewL(UID); }
#define IMPLEMENTATION_INSTANCEV2(UID) IMPLEMENTATION_PROXY_ENTRY(UID, CreateInstance##UID)

//ECOM factory functions
DECLARE_CREATE_INSTANCEV2(0x20033982)
DECLARE_CREATE_INSTANCEV2(0x20033983)

// Define the private interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_INSTANCEV2(0x20033982),
    IMPLEMENTATION_INSTANCEV2(0x20033983)
    };

CUtOomMonitorPluginV2* CUtOomMonitorPluginV2::NewL(TInt aInstance)
    {
    CUtOomMonitorPluginV2* self = new (ELeave) CUtOomMonitorPluginV2(
            aInstance);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CUtOomMonitorPluginV2::CUtOomMonitorPluginV2(TInt aInstance) :
    iInstance(aInstance)
    {

    }

void CUtOomMonitorPluginV2::ConstructL()
    {
    RDebug::Print(_L("CUtOomMonitorPluginV2::ConstructL"));
    FreeRam(0);
    MemoryGood();
    }

CUtOomMonitorPluginV2::~CUtOomMonitorPluginV2()
    {
    RDebug::Print(_L("CUtOomMonitorPluginV2::~CUtOomMonitorPluginV2"));
    }

void CUtOomMonitorPluginV2::FreeRam(TInt /*aBytesRequested*/)
    {
    iLowMemoryCallCount++;
    COomMonitorPluginV2::FreeRam();
    }

void CUtOomMonitorPluginV2::MemoryGood()
    {
    iGoodMemoryCallCount++;
    }

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
