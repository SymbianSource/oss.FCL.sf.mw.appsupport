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
* Description:  Main classes for Out of Memory Monitor.
*
*/



#include "oomoutofmemorywatcher.h"
#include "oommemorymonitor.h"
#include "oomtraces.h"

// ======================================================================
// class COutOfMemoryWatcher
// - notifies when free memory crosses preset thresholds
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher* COutOfMemoryWatcher::NewL(CMemoryMonitor& aMonitor, TInt aLowThreshold, TInt aGoodThreshold)
    {
    FUNC_LOG;

    COutOfMemoryWatcher* self = new (ELeave) COutOfMemoryWatcher(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL(aLowThreshold, aGoodThreshold);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher::~COutOfMemoryWatcher()
    {
    FUNC_LOG;

    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
COutOfMemoryWatcher::COutOfMemoryWatcher(CMemoryMonitor& aMonitor)
:   CActive(CActive::EPriorityStandard),
    iLafShutdown(aMonitor)
    {
    FUNC_LOG;

    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::ConstructL(TInt aLowThreshold, TInt aGoodThreshold)
    {
    FUNC_LOG;

    UserSvr::SetMemoryThresholds(aLowThreshold,aGoodThreshold);
    User::LeaveIfError(iChangeNotifier.Create());
    }

void COutOfMemoryWatcher::UpdateThresholds(TInt aLowThreshold, TInt aGoodThreshold)
    {
    FUNC_LOG;

    UserSvr::SetMemoryThresholds(aLowThreshold,aGoodThreshold);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::Start()
    {
    FUNC_LOG;

    if (!IsActive())
        {
        iChangeNotifier.Logon(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::DoCancel()
    {
    FUNC_LOG;

    iChangeNotifier.LogonCancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void COutOfMemoryWatcher::RunL()
    {
    FUNC_LOG;

    TInt status = iStatus.Int();

    if (status < 0)
        {
        User::Leave(status);
        }

    // Check for memory status change.
    if (status & EChangesFreeMemory)
        {
        iLafShutdown.FreeMemThresholdCrossedL();
        }

    // We are not active until FreeMemThresholdCrossedL returns.
    Start();
    }
