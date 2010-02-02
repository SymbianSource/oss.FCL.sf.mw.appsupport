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
* Description:  Client/server interface for OOM Monitor.
*
*/


#include "oommonitorsession.h"
#include "oommonitorclientserver.h"

EXPORT_C TInt ROomMonitorSession::Connect()
	{
	return CreateSession(KMemoryMonitorServerName, TVersion(0,0,0));
	}

EXPORT_C TInt ROomMonitorSession::RequestFreeMemory(TInt aBytesRequested)
	{
	TIpcArgs p(aBytesRequested);
	return SendReceive(EOomMonitorRequestFreeMemory, p);
	}

EXPORT_C void ROomMonitorSession::RequestFreeMemory(TInt aBytesRequested, TRequestStatus& aStatus)
	{
	TIpcArgs p(aBytesRequested);
	SendReceive(EOomMonitorRequestFreeMemory, p, aStatus);
	}

EXPORT_C void ROomMonitorSession::CancelRequestFreeMemory()
	{
	SendReceive(EOomMonitorCancelRequestFreeMemory, TIpcArgs());
	}

EXPORT_C void ROomMonitorSession::ThisAppIsNotExiting(TInt aWgId)
	{
	TIpcArgs p(aWgId);
	SendReceive(EOomMonitorThisAppIsNotExiting, p);
	}
