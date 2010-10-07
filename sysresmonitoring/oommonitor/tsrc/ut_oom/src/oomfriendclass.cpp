/*
 * oomfriendclass.cpp
 *
 *  Created on: Aug 8, 2010
 *      Author: gecheng
 */

#include "oomfriendclass.h"
#include <../../../inc/oommemorymonitor.h>
#include <oomwindowgrouplist.h>
#include <oomconfig.h>
#include <oomclientrequestqueue.h>
#include <oommemorymonitorserver.h>
#include <oomactionlist.h>
#include <oomlog.h>
#include <oomoutofmemorywatcher.h>
#include <oomwserveventreceiver.h>


COomFriendClass::COomFriendClass()
    {

    }

COomFriendClass::~COomFriendClass()
    {

    }

COomFriendClass* COomFriendClass::COomFriendClass::NewLC()
    {
    COomFriendClass* self=new(ELeave) COomFriendClass();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;    
    }

COomFriendClass* COomFriendClass::COomFriendClass::NewL()
    {
    COomFriendClass* self=COomFriendClass::NewLC();
    CleanupStack::Pop(self);
    return self;    
    }

void COomFriendClass::ConstructL()
    {
   
    }

COomWindowGroupList* COomFriendClass::GetWindowGroupList(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iOomWindowGroupList;
    }

COomConfig* COomFriendClass::GetOomConfig(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iConfig;
    }

COomClientRequestQueue* COomFriendClass::GetClientRequestQueue(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iQueue;
    }

CMemoryMonitorServer* COomFriendClass::GetMemoryMonitorServer(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iServer;
    }

COomActionList* COomFriendClass::GetOomActionList(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iOomActionList;
    }

#ifdef _DEBUG    
COomLogger* COomFriendClass::GetOomLogger(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iLogger;
    }
#endif

COutOfMemoryWatcher* COomFriendClass::GetOutOfMemoryWatcher(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iOOMWatcher;
    }

CWservEventReceiver* COomFriendClass::GetEventReceiver(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iWservEventReceiver;
    }

void COomFriendClass::CallRefreshThresholds(CMemoryMonitor* aMemoryMonitor)
    {
    aMemoryMonitor->RefreshThresholds();
    }

TInt COomFriendClass::GetGoodRamThreshold(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iGoodRamThreshold;
    }

TInt COomFriendClass::GetLowRamThreshold(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iLowRamThreshold;
    }

TInt COomFriendClass::GetGoodSwapThreshold(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iGoodSwapThreshold;
    }

TInt COomFriendClass::GetLowSwapThreshold(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iLowSwapThreshold;
    }

TInt COomFriendClass::GetCurrentRamTarget(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iCurrentRamTarget;
    }

TInt COomFriendClass::GetCurrentSwapTarget(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iCurrentSwapTarget;
    }
    
TInt COomFriendClass::GetActionTrigger(CMemoryMonitor* aMemoryMonitor)
    {
    return STATIC_CAST(TInt, aMemoryMonitor->iActionTrigger);    
    }

TBool COomFriendClass::GetDataPaged(CMemoryMonitor* aMemoryMonitor)
    {   
    return aMemoryMonitor->iDataPaged;    
    }

TInt COomFriendClass::GetLastMemoryMonitorStatusProperty(CMemoryMonitor* aMemoryMonitor)
    {
    return aMemoryMonitor->iLastMemoryMonitorStatusProperty;
    }
