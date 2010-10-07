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
* Description:  Main classes for Out of Memory Monitor.
*
*/


#include <hal.h>
#include <u32hal.h>

#include <UikonInternalPSKeys.h>

#include "oommemorymonitor.h"
#include "oomconfig.h"
#include "oommonitorplugin.h"
#include "oomconstants.hrh"

// ======================================================================
// class CMemoryMonitor
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor* CMemoryMonitor::NewL()
    { // static
    CMemoryMonitor* self = new(ELeave) CMemoryMonitor();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitor::CMemoryMonitor()
    {
    SetMemoryMonitorTls(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor::~CMemoryMonitor()
    {
    delete iConfig;    
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::ConstructL()
    {
    iConfig = COomConfig::NewL(); 
    
    iConfig->SetMaxAppExitTime(KMicrosecondsInMillisecond);    
    }

const COomGlobalConfig& CMemoryMonitor::GlobalConfig()
    {
    CMemoryMonitor* globalMemoryMonitor = static_cast<CMemoryMonitor*>(Dll::Tls());
    return globalMemoryMonitor->iConfig->GlobalConfig();
    }


// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::FreeMemThresholdCrossedL()
    {
    }

void CMemoryMonitor::HandleFocusedWgChangeL()
    {
    TAny* pointer=Dll::Tls();
    TInt* Count=STATIC_CAST(TInt*, pointer);
    (*Count)++;
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt /*aFreeRamTarget*/, TInt /*aFreeSwapTarget*/)
    {
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt /*aFreeRamTarget*/, TInt /*aFreeSwapTarget*/, TInt /*aMaxPriority*/) // The maximum priority of action to run
    {
    }

void CMemoryMonitor::RequestFreeMemoryPandSL(TInt /*aBytesRequested*/)
    {
    }

void CMemoryMonitor::RequestFreeMemoryL(TInt /*aBytesRequested*/, TBool /*aDataPaged*/)
    {
    }

void CMemoryMonitor::FreeOptionalRamL(TInt /*aBytesRequested*/, TInt /*aPluginId*/, TBool /*aDataPaged*/) // The ID of the plugin that will clear up the allocation, used to determine the priority of the allocation
    {
    }

void CMemoryMonitor::GetFreeMemory(TInt& /*aCurrentFreeMemory*/)
    {
    }

void CMemoryMonitor::GetFreeSwapSpace(TInt& /*aCurrentFreeSwapSpace*/)
    {
    }

#ifndef CLIENT_REQUEST_QUEUE 
TInt CMemoryMonitor::WatchdogStatusStatusChanged(TAny* /*aPtr*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::HandleWatchdogStatusCallBack()
    {
    }
#endif //CLIENT_REQUEST_QUEUE

void CMemoryMonitor::AppNotExiting(TInt /*aWgId*/)
    {
    }


void CMemoryMonitor::RefreshThresholds()
    {
    }

// SetMemoryMonitorStatusProperty - updates the property value only if it has changed
void CMemoryMonitor::SetMemoryMonitorStatusProperty(const TMemoryMonitorStatusPropertyValues /*aValue*/)
    {
    }

void CMemoryMonitor::ResetTargets()
    {
    }

void CMemoryMonitor::SetPriorityBusy(TInt /*aWgId*/)
    {
    }

void CMemoryMonitor::SetPriorityNormal(TInt /*aWgId*/)
    {
    }

void CMemoryMonitor::SetPriorityHigh(TInt /*aWgId*/)
    {
    }

TActionTriggerType CMemoryMonitor::ActionTrigger() const
    {
    return iActionTrigger;
    }

#ifdef CLIENT_REQUEST_QUEUE 
TInt CMemoryMonitor::GoodRamThreshold() const
    {
    return iGoodRamThreshold;
    }

TInt CMemoryMonitor::LowRamThreshold() const
    {
    return iLowRamThreshold;
    }

void CMemoryMonitor::ActionsCompleted(TInt /*aBytesFree*/, TBool /*aMemoryGood*/)
    {
    }
#endif //CLIENT_REQUEST_QUEUE
