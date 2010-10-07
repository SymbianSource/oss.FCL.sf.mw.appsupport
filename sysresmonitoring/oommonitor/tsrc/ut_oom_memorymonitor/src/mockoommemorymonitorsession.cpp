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

#include <oommonitorclientserver.h>
#include <oommemorymonitorsession.h>
#include <../../../inc/oommemorymonitor.h>
#include <oommemorymonitorserver.h>
#include <OomTraces.h>
#include <oomclientrequestqueue.h>

CMemoryMonitorSession::CMemoryMonitorSession()
    {

    }

CMemoryMonitorSession::~CMemoryMonitorSession()
    {

    }

CMemoryMonitorServer& CMemoryMonitorSession::Server()
    {
    return *static_cast<CMemoryMonitorServer*>(const_cast<CServer2*>(CSession2::Server()));
    }

#ifdef CLIENT_REQUEST_QUEUE
COomClientRequestQueue& CMemoryMonitorSession::ClientRequestQueue()
    {
    return Server().ClientRequestQueue();
    }
#endif

CMemoryMonitor& CMemoryMonitorSession::Monitor()
    {

#ifdef CLIENT_REQUEST_QUEUE
    return ClientRequestQueue().Monitor();
#else
    return Server().Monitor();    
#endif
    }
#ifndef CLIENT_REQUEST_QUEUE
TBool CMemoryMonitorSession::IsDataPaged(const RMessage2& aMessage)
    {
    RThread clientThread;
    TInt err = aMessage.Client(clientThread);
    TBool dataPaged = EFalse;
    if(err == KErrNone)
        {
        RProcess processName;
        err = clientThread.Process(processName);
        if(err == KErrNone)
            {
            dataPaged = processName.DefaultDataPaged();
            }
        else
            {
            PanicClient(aMessage, EPanicIllegalFunction);
            }
        }
    else
        {
        PanicClient(aMessage, EPanicIllegalFunction);
        }
    return dataPaged;            
    }
#endif

void CMemoryMonitorSession::ServiceL(const RMessage2& aMessage)
    {

    }

#ifndef CLIENT_REQUEST_QUEUE
void CMemoryMonitorSession::CloseAppsFinished(TInt aBytesFree, TBool aMemoryGood)
    {
    FUNC_LOG;

    if (!iRequestFreeRam.IsNull())
        {
        if (iFunction == EOomMonitorRequestOptionalRam)
            {
            TInt memoryAvailable = aBytesFree - CMemoryMonitor::GlobalConfig().iGoodRamThreshold;
            
            // If memory available is greater than the requested RAM then complete with the amount of free memory, otherwise complete with KErrNoMemory
            if (memoryAvailable >= iMinimumMemoryRequested)
                {
                iRequestFreeRam.Complete(memoryAvailable);
                }
            else
                {
                iRequestFreeRam.Complete(KErrNoMemory);
                }
               }
        else 
            iRequestFreeRam.Complete(aMemoryGood ? KErrNone : KErrNoMemory);
        }
    }
#endif
