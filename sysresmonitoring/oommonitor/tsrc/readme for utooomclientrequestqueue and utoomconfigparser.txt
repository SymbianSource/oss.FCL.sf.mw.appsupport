1.	For test code to run, a Macro inserted in to oommonitorclientserver.h, should ask developer to update code. Here is it:
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

#ifndef  FCC_UNIT_TEST
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

2.	As pointed out earlier from Tong Sun's email, one panic in oomclientrequestqueue.cpp, cause one of the test cases failed.
3.	CTC++ report are in group folder.

// for utoomconfigparser
1. copy file:
w:\epoc32\release\armv5\urel\utoomconfigparser.dll c:\sys\bin\utoomconfigparser.dll

2. run 
z:\sys\bin\atsinterface.exe -testmodule utoomconfigparser

// for utoomclientrequestqueue
1. copy file:
w:\epoc32\release\armv5\urel\utoomclientrequestqueue.dll c:\sys\bin\utoomclientrequestqueue.dll
w:\epoc32\release\armv5\urel\mockoommonitorserver.exe    c:\sys\bin\mockoommonitorserver.exe

2. run
z:\sys\bin\atsinterface.exe -testmodule utoomclientrequestqueue
