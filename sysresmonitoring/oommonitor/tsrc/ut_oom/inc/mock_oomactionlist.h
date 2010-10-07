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
* Description:  Main classes for Out of Memory Monitor.
*
*/


#ifndef MOCK_OOMACTIONLIST_H
#define MOCK_OOMACTIONLIST_H

#include "oomactionlist.h"
#include "ut_oom.h"

NONSHARABLE_CLASS(CMoc_COomActionList) : public CBase, public MOomActionObserver
    {
public:
    void StateChanged();
    TInt GetState();    	
private:
	TInt  iStateChange;  
    };
    
_LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
const TUid KUtOomWatcherTestProperty = { 0x10000002 };
const TUint32 KUtOomPluginWatcherTestKey = 0x00000001;
const TInt KUtOomPluginMemGood = 1;

#endif /*MOCK_OOMACTIONLIST_H*/
