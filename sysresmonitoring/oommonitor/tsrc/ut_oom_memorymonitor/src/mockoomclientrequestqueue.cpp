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
* Description:  COomClientRequestQueue.cpp.
*
*/

#include "oomclientrequestqueue.h"
#include "OomTraces.h"
#include "oomsubscribehelper.h"
#include "oompanic.h"
#include "oommemorymonitor.h"
#include <UikonInternalPSKeys.h>

#include "globaldata.h"
#include "diclog.h"

COomClientRequestQueue::COomClientRequestQueue(CMemoryMonitor& aMonitor)
    :iQueue(_FOFF(TClientRequest,iLink)),
    iMonitor(aMonitor)
    {

    }

COomClientRequestQueue::~COomClientRequestQueue()
    {

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

    }

void COomClientRequestQueue::RequestOptionalRamL(const RMessage2& aMessage)
    {

    }

TInt COomClientRequestQueue::WatchdogStatusStatusChanged(TAny* aPtr)
    {
    return KErrNone;
    }

void COomClientRequestQueue::HandleWatchdogStatusCallBack()
    {

    }

void COomClientRequestQueue::AddClientRequestL(TClientRequest& request)
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

    }

void COomClientRequestQueue::ActionsCompleted(TInt aBytesFree, TBool aMemoryGood)
    {
    TBuf<KIntValue18> buf(KClientRequestQueue);
    DicLog::WriteLog(TUid::Uid(0x0008), buf); 
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
    
    }


void COomClientRequestTimer::RunL()
    {

    }




    
    
