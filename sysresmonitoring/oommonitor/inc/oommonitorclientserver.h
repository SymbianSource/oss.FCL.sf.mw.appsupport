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
* Description:  Command definitions for OOM Monitor client/server interface.
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

_LIT(KMemoryMonitorServerName, "OomMonitorServer");

enum TOomMonitorCmd
	{
	EOomMonitorRequestFreeMemory,
	EOomMonitorCancelRequestFreeMemory,
	EOomMonitorThisAppIsNotExiting
	};

#endif // OOMMONITORCLIENTSERVER_H
