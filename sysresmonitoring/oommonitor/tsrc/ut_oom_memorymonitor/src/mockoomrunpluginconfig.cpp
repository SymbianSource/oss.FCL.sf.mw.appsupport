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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/


#include "oomrunpluginconfig.h"
#include "oomwindowgrouplist.h"
#include "OomTraces.h"
#include "globaldata.h"

COomRunPluginConfig* COomRunPluginConfig::NewL(TUint aPluginId, TOomPluginType aPluginType)
    {
    FUNC_LOG;

    COomRunPluginConfig* self = new (ELeave) COomRunPluginConfig(aPluginId, aPluginType);
    return self;
    }

TUint COomRunPluginConfig::CalculatePluginPriority(const COomWindowGroupList& aWindowGroupList)
    {
    return KMaxPriority1;
    }


COomRunPluginConfig::~COomRunPluginConfig()
    {

    }
    

COomRunPluginConfig::COomRunPluginConfig(TUint aPluginId, TOomPluginType aPluginType) : COomActionConfig(aPluginId), iPluginId(aPluginId), iPluginType(aPluginType)
    {

    }
