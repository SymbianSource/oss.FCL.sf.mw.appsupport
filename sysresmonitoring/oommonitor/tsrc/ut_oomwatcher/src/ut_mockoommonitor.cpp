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
* Description: Mock CMemoryMonitor
*
*/

#include "ut_oomwatchercommon.h"
#include "oommemorymonitor.h"
#include "oomclientrequestqueue.h"
#include "oommemorymonitorserver.h"
#include <e32property.h>



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

    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CMemoryMonitor::~CMemoryMonitor()
    {
#ifdef CLIENT_REQUEST_QUEUE
    delete iServer;

    delete iQueue;
#endif
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::ConstructL()
    {
#ifdef CLIENT_REQUEST_QUEUE
    iQueue = COomClientRequestQueue::NewL(*this);
    
    iServer = CMemoryMonitorServer::NewL(*iQueue);
#endif
    }

const COomGlobalConfig& CMemoryMonitor::GlobalConfig()
    {
    COomGlobalConfig* MyConfig=NULL; 
    return *MyConfig;
    }


// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CMemoryMonitor::FreeMemThresholdCrossedL()
    {
    // Create property for getting whether watcher has called correct method
    RProperty testWatcherProperty;

    User::LeaveIfError(testWatcherProperty.Attach(KOomWatcherTestProperty, KOomWatcherTestKey, EOwnerThread));
    CleanupClosePushL(testWatcherProperty);
    User::LeaveIfError(testWatcherProperty.Set(KOomTestWatcherPropertyValue));
    CleanupStack::PopAndDestroy();

    CActiveScheduler::Stop();
    }

void CMemoryMonitor::HandleFocusedWgChangeL()
    {
    }

void CMemoryMonitor::StartFreeSomeRamL(TInt /*aFreeRamTarget*/, TInt /*aFreeSwapTarget*/)
    {

    }

void CMemoryMonitor::StartFreeSomeRamL(TInt /*aFreeRamTarget*/, TInt /*aFreeSwapTarget*/, TInt /*aMaxPriority*/) // The maximum priority of action to run
    {

    
    // Build the list of memory freeing actions

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

void CMemoryMonitor::HandleWatchdogStatusCallBack()
    {

    }
#endif //CLIENT_REQUEST_QUEUE

void CMemoryMonitor::AppNotExiting(TInt /*aWgId*/)
    {
    // Create property for getting whether server has called correct method
    RProperty testServerProperty;

    testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    testServerProperty.Set(KOomTestMonitorAppNotExitingCalled);
    testServerProperty.Close();
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
    // Create property for getting whether server has called correct method
    RProperty testServerProperty;

    testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    testServerProperty.Set(KOomTestMonitorSetPriorityBusyCalled);
    testServerProperty.Close();
    }

void CMemoryMonitor::SetPriorityNormal(TInt /*aWgId*/)
    {
    // Create property for getting whether server has called correct method
    RProperty testServerProperty;

    testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    testServerProperty.Set(KOomTestMonitorSetPriorityNormalCalled);
    testServerProperty.Close();
    }

void CMemoryMonitor::SetPriorityHigh(TInt /*aWgId*/)
    {
    // Create property for getting whether server has called correct method
    RProperty testServerProperty;

    testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    testServerProperty.Set(KOomTestMonitorSetPriorityHighCalled);
    testServerProperty.Close();
    }

TActionTriggerType CMemoryMonitor::ActionTrigger() const
    {
    return iActionTrigger;
    }

#ifdef CLIENT_REQUEST_QUEUE 
TInt CMemoryMonitor::GoodRamThreshold() const
    {
    return 0;
    }

TInt CMemoryMonitor::LowRamThreshold() const
    {
    return 0;
    }

void CMemoryMonitor::ActionsCompleted(TInt /*aBytesFree*/, TBool /*aMemoryGood*/)
    {

    }
#endif //CLIENT_REQUEST_QUEUE
