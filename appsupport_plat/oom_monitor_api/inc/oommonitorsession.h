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


#ifndef R_OOMMONITORSESSION_H
#define R_OOMMONITORSESSION_H

#include <e32std.h>

class ROomMonitorSession : public RSessionBase
	{
public:
	/**
	* Connects a new session.
	* Sessions must be connected before any other APIs can be used.
	* When the client has finished using a session, Close() must be called.
	* @return KErrNone if successful, error code otherwise.
	*/
	IMPORT_C TInt Connect();
	/**
	* Request that the OOM monitor attempts to free some memory.
	* This function may take several seconds to execute, depending on
	* the memory state. It will not return until the attempt to recover
	* memory has completed.
	* @param aBytesRequested The number of bytes of free memory that the client requests.
	* @return KErrNone if the request memory is free. KErrNoMemory if that
	*         amount of memory could not be recovered. Other error codes may
	*         also be returned.
	*/
	IMPORT_C TInt RequestFreeMemory(TInt aBytesRequested);
	/**
	* Request that the OOM monitor attempts to free some memory.
	* This is an asynchronous version of the request for free memory.
	* @param aBytesRequested The number of bytes of free memory that the client requests.
	* @param aStatus will be completed when the attempt to recover memory
	*        has finished. This may take several seconds, depending on
	*        the memory state. On completion, aStatus will be set to 
	*        KErrNone if the request memory is free. KErrNoMemory if that
	*        amount of memory could not be recovered. Other error codes may
	*        also be set.
	*/
	IMPORT_C void RequestFreeMemory(TInt aBytesRequested, TRequestStatus& aStatus);
	/**
	* Cancels the asynchronous request for free memory.
	*/
	IMPORT_C void CancelRequestFreeMemory();
	/**
	* Notifiy the OOM monitor that this application is not responding
	* to the EEikCmdExit request to exit the application.
	* This function is used by the Avkon framework's app shutter.
	* @param aWgId the window group identifier of the app that is not exiting.
	*/
	IMPORT_C void ThisAppIsNotExiting(TInt aWgId);
	};

#endif // R_OOMMONITORSESSION_H
