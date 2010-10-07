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

#include "oomoutofmemorywatcher.h"
#include "oommemorymonitor.h"
#include "OomTraces.h"
#include <u32hal.h>

#include "diclog.h"
#include "globaldata.h"

COutOfMemoryWatcher* COutOfMemoryWatcher::NewL(CMemoryMonitor& aMonitor, TInt aLowRamThreshold, TInt aGoodRamThreshold, TBool aSwapUsageMonitored, TInt aLowSwapThreshold, TInt aGoodSwapThreshold)
    {
    FUNC_LOG;

    COutOfMemoryWatcher* self = new (ELeave) COutOfMemoryWatcher(aMonitor, aSwapUsageMonitored);
    CleanupStack::PushL(self);
    self->ConstructL(aLowRamThreshold, aGoodRamThreshold, aLowSwapThreshold, aGoodSwapThreshold);
    CleanupStack::Pop(self);
    return self;
    }

COutOfMemoryWatcher::~COutOfMemoryWatcher()
    {

    }

COutOfMemoryWatcher::COutOfMemoryWatcher(CMemoryMonitor& aMonitor, TBool aSwapUsageMonitored)
:   CActive(CActive::EPriorityStandard),
    iMemoryMonitor(aMonitor),
    iSwapUsageMonitored(aSwapUsageMonitored)
    {

    }

void COutOfMemoryWatcher::ConstructL(TInt aLowRamThreshold, TInt aGoodRamThreshold, TInt aLowSwapThreshold, TInt aGoodSwapThreshold)
    {

    }

void COutOfMemoryWatcher::UpdateThresholds(TInt aLowRamThreshold, TInt aGoodRamThreshold, TInt aLowSwapThreshold, TInt aGoodSwapThreshold)
    {
    DicLog::WriteIntLog(TUid::Uid(0x0009),aLowRamThreshold);
    DicLog::WriteIntLog(TUid::Uid(0x0010),aGoodRamThreshold);
    DicLog::WriteIntLog(TUid::Uid(0x0011),aLowSwapThreshold);
    DicLog::WriteIntLog(TUid::Uid(0x0012),aGoodSwapThreshold);
    }

void COutOfMemoryWatcher::Start()
    {
    TBuf<KIntValue7> buf(KOutOfMemoryWatcherStart);
    DicLog::WriteLog(TUid::Uid(0x0002), buf);
    }

void COutOfMemoryWatcher::DoCancel()
    {

    }

void COutOfMemoryWatcher::RunL()
    {

    }
