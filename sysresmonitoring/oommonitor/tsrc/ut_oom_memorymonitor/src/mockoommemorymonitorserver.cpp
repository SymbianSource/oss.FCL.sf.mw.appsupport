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

#include "oommemorymonitorserver.h"
#include "oommonitorclientserver.h"
#include "oommemorymonitorsession.h"
#include "oommemorymonitor.h"
#include "OomTraces.h"

#ifdef CLIENT_REQUEST_QUEUE        
CMemoryMonitorServer* CMemoryMonitorServer::NewL(COomClientRequestQueue& aQueue)
#else
CMemoryMonitorServer* CMemoryMonitorServer::NewL(CMemoryMonitor& aMonitor)
#endif
    {
//    FUNC_LOG;

#ifdef CLIENT_REQUEST_QUEUE        
    CMemoryMonitorServer* self=new(ELeave) CMemoryMonitorServer(aQueue);
#else
    CMemoryMonitorServer* self=new(ELeave) CMemoryMonitorServer(aMonitor);
#endif
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMemoryMonitorServer::~CMemoryMonitorServer()
    {
//    FUNC_LOG;
    }

#ifdef CLIENT_REQUEST_QUEUE        
CMemoryMonitorServer::CMemoryMonitorServer(COomClientRequestQueue& aQueue)
    :CServer2(CActive::EPriorityStandard), iQueue(aQueue)
#else
CMemoryMonitorServer::CMemoryMonitorServer(CMemoryMonitor& aMonitor)
    :CServer2(CActive::EPriorityStandard), iMonitor(aMonitor)    
#endif
    {

    }

void CMemoryMonitorServer::ConstructL()
    {

    }

CSession2* CMemoryMonitorServer::NewSessionL(const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/) const
    {
    return new(ELeave) CMemoryMonitorSession();
    }

TInt CMemoryMonitorServer::RunError(TInt aError)
    {
    return KErrNone;    // handled the error fully
    }

#ifdef CLIENT_REQUEST_QUEUE        
COomClientRequestQueue& CMemoryMonitorServer::ClientRequestQueue()
    {
    FUNC_LOG;

    return iQueue;
    }
#else
CMemoryMonitor& CMemoryMonitorServer::Monitor()
    {

    }
#endif

#ifndef CLIENT_REQUEST_QUEUE        
void CMemoryMonitorServer::CloseAppsFinished(TInt aBytesFree, TBool aMemoryGood)
    {

    }
#endif
