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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/

#include <hal.h>
#include <u32hal.h>
#include <apgwgnam.h>

#include "oommonitorplugin.h"
#include "oommonitorplugin.hrh"
#include "oomactionlist.h"
#include "oomwindowgrouplist.h"
#include "oompanic.h"
#include "OomTraces.h"
#include "oomconstants.hrh"
#include "oommemorymonitor.h"
#include "oommemorymonitorserver.h"
#include "oomrunplugin.h"
#include "oomcloseapp.h"
#include "oomconfig.h"
#include "oomactionref.h"
#include "oomapplicationconfig.h"
#include "oomcloseappconfig.h"
#include "oomrunpluginconfig.h"

#include "globaldata.h"
#include "diclog.h"

template <class T>
COomPluginList<T>::COomPluginList()
    {

    }

template <class T>
COomPluginList<T>::~COomPluginList()
    {

    }

template <class T>
void COomPluginList<T>::ConstructL(TInt aInterfaceUid)
    {

    }

template <class T>
COomPluginList<T>::TPlugin::TPlugin()
: iImpl(0)
    {

    }

template <class T>
COomPluginList<T>* COomPluginList<T>::NewL(TInt aInterfaceUid)
    {
    FUNC_LOG;
    
    COomPluginList* self = new (ELeave) COomPluginList();
    CleanupStack::PushL(self);
    self->ConstructL(aInterfaceUid);
    CleanupStack::Pop(self);
    return self;
    }
    
COomActionList* COomActionList::NewL(CMemoryMonitor& aMonitor, CMemoryMonitorServer& aServer, RWsSession& aWs, COomConfig& aConfig)
    {
    FUNC_LOG;
    
    COomActionList* self = new (ELeave) COomActionList(aMonitor, aServer, aWs);
    CleanupStack::PushL(self);
    self->ConstructL(aConfig);
    CleanupStack::Pop(self);
    return self;
    }

COomActionList::~COomActionList()
    {

    }

void COomActionList::BuildActionListL(COomWindowGroupList& aWindowGroupList, COomConfig& aConfig)
    {
    TBuf<KIntValue18> buf(KActionListBuildActionListL);
    DicLog::WriteLog(TUid::Uid(0x0014), buf);    
    }

template <class T>
void COomActionList::BuildPluginActionsL(COomPluginList<T>& aPluginList, COomWindowGroupList& aWindowGroupList, COomConfig& aConfig, TInt& aActionsIndex)
    {

    }


// Execute the OOM actions according to their priority
// Run batches of OOM actions according to their sync mode
void COomActionList::FreeMemory(TInt aMaxPriority)
    {
    DicLog::WriteIntLog(TUid::Uid(0x0015),aMaxPriority);
    }

void COomActionList::SwitchOffPlugins()
    {
    TBuf<KIntValue18> buf(KActionListSwitchOffPlugins);
    DicLog::WriteLog(TUid::Uid(0x0007), buf);
    }

TInt COomActionList::ComparePriorities(const TActionRef& aPos1, const TActionRef& aPos2 )
    {  
    return 0;        
    }

void COomActionList::AppNotExiting(TInt aWgId)
    {
    DicLog::WriteIntLog(TUid::Uid(0x0016),aWgId);
    }

void COomActionList::StateChanged()
    {

    }

COomActionList::COomActionList(CMemoryMonitor& aMonitor, CMemoryMonitorServer& aServer, RWsSession& aWs) 
    : iWs(aWs), iMonitor(aMonitor), iServer(aServer)
    {

    }

void COomActionList::ConstructL(COomConfig& aConfig)
    {

    }
