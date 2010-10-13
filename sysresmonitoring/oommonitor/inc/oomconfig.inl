/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef OOMCONFIGINL_H_
#define OOMCONFIGINL_H_

inline COomGlobalConfig& COomConfig::GlobalConfig()
    {
    return iGlobalConfig;
    }

void COomConfig::SetDefaultLowRamThreshold(TInt aLowRamThreshold)
    {
    iGlobalConfig.iLowRamThreshold = aLowRamThreshold;
    }

void COomConfig::SetDefaultGoodRamThreshold(TInt aGoodRamThreshold)
    {
    iGlobalConfig.iGoodRamThreshold = aGoodRamThreshold;
    }

void COomConfig::SetMaxCloseAppBatch(TUint aMaxCloseAppBatch)
    {
    iGlobalConfig.iMaxCloseAppBatch = aMaxCloseAppBatch;
    }
void COomConfig::SetDefaultWaitAfterPlugin(TInt aMilliseconds)
    {
    iGlobalConfig.iDefaultWaitAfterPlugin = aMilliseconds;
    }

void COomConfig::SetMaxAppExitTime(TInt aMilliseconds)
    {
    iGlobalConfig.iMaxAppExitTime = aMilliseconds;
    }

#endif /*OOMCONFIGINL_H_*/
