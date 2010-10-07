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
* Description: Common variables used in COutOfMemoryWatcher testing
*
*/

#ifndef UT_OOMWATCHERCOMMON_H
#define UT_OOMWATCHERCOMMON_H

#include <e32base.h> 

_LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);
const TInt KOomTestWatcherPropertyValue = 11;
const TUid KOomWatcherTestProperty = { 0x10000000 };
const TUint32 KOomWatcherTestKey = 0x00000001;
const TUid KOomServerTestProperty = { 0x10000001 };
const TUint32 KOomServerTestKey = 0x00000001;
const TInt KOomTestInitialValue = 0;
const TInt KOomTestClientQueueFreeMemoryCalled = 100;
const TInt KOomTestClientQueueOptionalRamCalled = KOomTestClientQueueFreeMemoryCalled+1;
const TInt KOomTestMonitorAppNotExitingCalled = 200;
const TInt KOomTestMonitorSetPriorityBusyCalled = KOomTestMonitorAppNotExitingCalled+1;
const TInt KOomTestMonitorSetPriorityNormalCalled = KOomTestMonitorAppNotExitingCalled+2;
const TInt KOomTestMonitorSetPriorityHighCalled = KOomTestMonitorAppNotExitingCalled+3;


#endif /* UT_OOMWATCHERCOMMON_H */
