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
* Description:  The client / server classes allowing clients to make requests to OOM monitor.
*
*/


#ifndef OOMMONITORCLIENTSERVER_H
#define OOMMONITORCLIENTSERVER_H

#include <e32std.h>

enum TOomMonitorClientPanic
    {
    EPanicIllegalFunction,
    EPanicRequestActive
    };

void PanicClient(const RMessagePtr2& aMessage,TOomMonitorClientPanic aPanic);

#ifndef FCC_UNIT_TEST
_LIT(KMemoryMonitorServerName, "OomMonitorServer");
#else
_LIT(KMemoryMonitorServerName, "MockOomMonitorServer");
#endif

enum TOomMonitorCmd
    {
    EOomMonitorRequestFreeMemory,
    EOomMonitorCancelRequestFreeMemory,
    EOomMonitorThisAppIsNotExiting,
    EOomMonitorRequestOptionalRam,
    EOomMonitorSetPriorityBusy,
    EOomMonitorSetPriorityNormal,
    EOomMonitorSetPriorityHigh
    };

#endif // OOMMONITORCLIENTSERVER_H
