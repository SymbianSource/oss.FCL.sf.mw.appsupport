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

#include "oomactionconfig.h"
#include "oomruleconfig.h"
#include "OomTraces.h"

void COomActionConfig::AddRuleL(MOomRuleConfig* aRule)    // Add the configuration for a rule (e.g. an idle time rule)
    {

    }
	
COomActionConfig::~COomActionConfig()
    {

    }

TUint COomActionConfig::Priority(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const
    {
    return 0;
    }
    
void COomActionConfig::ConstructL()
    {

    }
	
COomActionConfig::COomActionConfig(TInt32 aId) : iId(aId)
    {

    }


