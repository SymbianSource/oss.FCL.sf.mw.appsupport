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
* Description: This file contains STIFUnit implementation.
*
*/

const TInt KIntValue8=8;
const TInt KIntValue7=7;
const TInt KIntValue9=9;
const TInt KIntValue18=18;
const TInt KIntValue60=60;
const TInt KMaxPriority1=889;
const TInt KGoodRamThreshold=200;
const TInt KLowRamThreshold=100;
const TInt KGoodSwapThreshold=400;
const TInt KLowSwapThreshold=300;
const TInt KMSize=1024*1024;

_LIT(KConfigParserParser, "ParseL()");                      // 0x0001   8
_LIT(KOutOfMemoryWatcherStart, "Start()");                  // 0x0002   7
_LIT(KWingoupListRefresh, "Refresh()");                     // 0x0003   9
//oomwindowgrouplist SetPriorityHigh                        // 0x0004   int
//oomwindowgrouplist SetPriorityNormal                      // 0x0005   int
//oomwindowgrouplist SetPriorityBusy                        // 0x0006   int
_LIT(KActionListSwitchOffPlugins, "SwitchOffPlugins()");    // 0x0007   18
_LIT(KClientRequestQueue, "ActionsCompleted()");            // 0x0008   18
//oomoutofmemorywatcher aLowRamThreshold                    // 0x0009   int
//oomoutofmemorywatcher aGoodRamThreshold                   // 0x0010   int
//oomoutofmemorywatcher aLowSwapThreshold                   // 0x0011   int
//oomoutofmemorywatcher aGoodSwapThreshold                  // 0x0012   int
_LIT(KOomLoggerStartL, "StartL()");                         // 0x0013   8
_LIT(KActionListBuildActionListL, "BuildActionListL()");    // 0x0014   18
//COomActionList FreeMemory aMaxPriority                    // 0x0015   int
//COomActionList AppNotExiting aWgId                        // 0x0016   int





