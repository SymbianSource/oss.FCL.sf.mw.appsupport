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
* Description: This file contains STIFUnit implementation.
*
*/

#ifndef OOMFRIENDCLASS_H_
#define OOMFRIENDCLASS_H_

#include <e32base.h>

class CMemoryMonitor;
class COomWindowGroupList;
class COomConfig;
class COomClientRequestQueue;
class CMemoryMonitorServer;
class COomActionList;
class COomLogger;
class COutOfMemoryWatcher;
class CWservEventReceiver;

class COomFriendClass : public CBase
    {
public:
    static COomFriendClass* NewL();
    static COomFriendClass* NewLC();
    virtual ~COomFriendClass();
    COomWindowGroupList* GetWindowGroupList(CMemoryMonitor* aMemoryMonitor);
    COomConfig* GetOomConfig(CMemoryMonitor* aMemoryMonitor);
    COomClientRequestQueue* GetClientRequestQueue(CMemoryMonitor* aMemoryMonitor);
    CMemoryMonitorServer* GetMemoryMonitorServer(CMemoryMonitor* aMemoryMonitor);
    COomActionList* GetOomActionList(CMemoryMonitor* aMemoryMonitor);
    COomLogger* GetOomLogger(CMemoryMonitor* aMemoryMonitor);
    COutOfMemoryWatcher* GetOutOfMemoryWatcher(CMemoryMonitor* aMemoryMonitor);
    CWservEventReceiver* GetEventReceiver(CMemoryMonitor* aMemoryMonitor);
    void CallRefreshThresholds(CMemoryMonitor* aMemoryMonitor);
    TInt GetGoodRamThreshold(CMemoryMonitor* aMemoryMonitor);
    TInt GetLowRamThreshold(CMemoryMonitor* aMemoryMonitor);
    TInt GetGoodSwapThreshold(CMemoryMonitor* aMemoryMonitor);
    TInt GetLowSwapThreshold(CMemoryMonitor* aMemoryMonitor);
    TInt GetCurrentRamTarget(CMemoryMonitor* aMemoryMonitor);
    TInt GetCurrentSwapTarget(CMemoryMonitor* aMemoryMonitor);
    TInt GetDataPaged(CMemoryMonitor* aMemoryMonitor);
    TInt GetActionTrigger(CMemoryMonitor* aMemoryMonitor);
    TInt GetLastMemoryMonitorStatusProperty(CMemoryMonitor* aMemoryMonitor);
    
private:    
    COomFriendClass();
    void ConstructL();    
    };

#endif /* OOMFRIENDCLASS_H_ */




