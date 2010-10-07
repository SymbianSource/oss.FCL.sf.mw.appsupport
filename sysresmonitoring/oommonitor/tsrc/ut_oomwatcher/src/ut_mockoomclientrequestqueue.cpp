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
* Description: Mock COomClientRequestQueue implementation for unit test
*
*/

#include "oomclientrequestqueue.h"
#include "ut_oomwatchercommon.h"
//#include "OomTraces.h"
//#include "oomsubscribehelper.h"
//#include "oompanic.h"
//#include "oommemorymonitor.h"
//#include <UikonInternalPSKeys.h>
//
//const TInt KOomWatchDogStatusIdle = -1;
//const TInt KClientTimeToFreeMemory = 500000; //microseconds

COomClientRequestQueue::COomClientRequestQueue(CMemoryMonitor& aMonitor)
    :iQueue(_FOFF(TClientRequest,iLink)),
    iMonitor(aMonitor)
    {

    }

COomClientRequestQueue::~COomClientRequestQueue()
    {
    TClientRequest* request;
    TSglQueIter<TClientRequest> iter(iQueue);
    iter.SetToFirst(); 
    while (iter)
        {
        request = iter++;
        iQueue.Remove(*request);
        delete request;
        };
    }

COomClientRequestQueue* COomClientRequestQueue::NewL(CMemoryMonitor& aMonitor)
    {
    COomClientRequestQueue* self = new (ELeave) COomClientRequestQueue(aMonitor);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); 
    return self;
    }

void COomClientRequestQueue::ConstructL()
    {

    }

void COomClientRequestQueue::RequestFreeMemoryL(const RMessage2& aMessage)
    {
    // Create property for getting whether server has called correct method
    RProperty testServerProperty;

    testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    testServerProperty.Set(KOomTestClientQueueFreeMemoryCalled);
    testServerProperty.Close();

    aMessage.Complete(KErrNone);
    }

void COomClientRequestQueue::RequestOptionalRamL(const RMessage2& aMessage)
    {
    // Create property for getting whether server has called correct method
    RProperty testServerProperty;

    testServerProperty.Attach(KOomServerTestProperty, KOomServerTestKey, EOwnerThread);
    testServerProperty.Set(KOomTestClientQueueOptionalRamCalled);
    testServerProperty.Close();
    
    aMessage.Complete(KErrNone);
    }

TInt COomClientRequestQueue::WatchdogStatusStatusChanged(TAny* /*aPtr*/)
    {
    return KErrNone;
    }

void COomClientRequestQueue::HandleWatchdogStatusCallBack()
    {

    }

// The new request is added to the queue, then we have the following conditions: 
// 1. A client request is currently being processed
// 2. The last client request completed less than KClientTimeToFreeMemory microseconds ago -> start the timer
// 3. The timer has already been started
// 4. none of the above -> process this request
void COomClientRequestQueue::AddClientRequestL(TClientRequest& /*request*/)
    {

    }

void COomClientRequestQueue::StartClientRequestL()
    {    

    }

CMemoryMonitor& COomClientRequestQueue::Monitor()
    {
    return iMonitor;
    }

TClientRequest::TClientRequest(TActionTriggerType aClientRequestType, TInt aBytesRequested)    
    : iClientRequestType(aClientRequestType), iBytesRequested(aBytesRequested)   
    {

    }

TClientRequest::TClientRequest(TActionTriggerType aClientRequestType, const RMessage2& aRequestFreeRam)    
    : iClientRequestType(aClientRequestType), iRequestFreeRamMessage(aRequestFreeRam)
    {
    iBytesRequested = aRequestFreeRam.Int0();
    }

void COomClientRequestQueue::ActionsCompleted(TInt /*aBytesFree*/, TBool /*aMemoryGood*/)
    {

    }

void COomClientRequestQueue::RequestTimerCallbackL()
    {

    }

COomClientRequestTimer* COomClientRequestTimer::NewL(COomClientRequestQueue& aQueue)
    {
    COomClientRequestTimer* self = new (ELeave) COomClientRequestTimer(aQueue);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

COomClientRequestTimer::COomClientRequestTimer(COomClientRequestQueue& aQueue)
: CTimer(CActive::EPriorityStandard), iClientRequestQueue(aQueue)
    {
    CActiveScheduler::Add(this);
    }


void COomClientRequestTimer::RunL()
    {
    iClientRequestQueue.RequestTimerCallbackL();
    }



