/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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

// Add a rule
// This class takes ownership of the given rule
void COomActionConfig::AddRuleL(MOomRuleConfig* aRule)    // Add the configuration for a rule (e.g. an idle time rule)
    {
    FUNC_LOG;
    
    iRules.AppendL(aRule);
    }
	
COomActionConfig::~COomActionConfig()
    {
    FUNC_LOG;
    
    iRules.ResetAndDestroy();
    iRules.Close();
    }

// Utility function to return the priority for this action for the given rule
TUint COomActionConfig::Priority(const COomWindowGroupList& aWindowGroupList, TInt aAppIndexInWindowGroup) const
    {
    FUNC_LOG;
    
    TUint priority = iDefaultPriority;
    
    // See if any rules apply
    TInt index = 0;
    while (index < iRules.Count())
        {
        if (iRules[index]->RuleIsApplicable(aWindowGroupList, aAppIndexInWindowGroup))
            {
            // If an applicable rule has been found, then apply the new priority
            // The last applicable rule in the config file should be used
            // This is ensured because the last rule in the list will be the last rule from the file
            if (iRules[index]->Priority())
                priority = iRules[index]->Priority();
            }
        
        index++;
        }
        
    return priority;
    }
    
void COomActionConfig::ConstructL()
    {
    FUNC_LOG;
    }
	
COomActionConfig::COomActionConfig(TInt32 aId) : iId(aId)
    {
    FUNC_LOG;
    }


