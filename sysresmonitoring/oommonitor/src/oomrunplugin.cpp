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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/

#include "oomrunplugin.h"
#include "OomTraces.h"
#include "oommemorymonitor.h"
#include "oommonitorplugin.h"
#include "oompanic.h"

#include <apgtask.h>
#include <w32std.h>

COomRunPlugin* COomRunPlugin::NewL(TUint aPluginId, COomRunPluginConfig& aConfig, MOomActionObserver& aStateChangeObserver, COomMonitorPlugin& aPlugin, COomMonitorPluginV2* aV2Plugin)
    {
    FUNC_LOG;

    COomRunPlugin* self = new (ELeave) COomRunPlugin(aPluginId, aConfig, aStateChangeObserver, aPlugin, aV2Plugin);
    CleanupStack::PushL(self);
    self->ConstructL(aConfig);
    CleanupStack::Pop(self);
    return self;
    }

// Run the OOM plugin in order to free memory
// Call the COomAction::MemoryFreed when it is done
void COomRunPlugin::FreeMemory(TInt aBytesRequested, TBool aIsDataPaged)
    {
    FUNC_LOG;
    iFreeMemoryCalled = ETrue;
    
    // Do we have a V2 plugin, if so then use it
    if (iV2Plugin)
        {
        //RDebug::Printf("COomRunPlugin::FreeMemory: Requesting to free the RAM iV2Plugin->FreeRam(%d) \n",aBytesRequested);
        iV2Plugin->FreeRam(aBytesRequested);
        }
     else if( aIsDataPaged )
        {
        if (IsAppDataPaged(iPlugin) )
            {
            //the plugins implemented by the application which are WDP enabled
            //are notified.
            //RDebug::Printf("COomRunPlugin::FreeMemory: Requesting to free the RAM to Data paged app \n");
            iPlugin.FreeRam();
            }
        else
            {
            //Request is for freeing the paged memory(swap) but the App(iPlugin belongs to)   
            //is non WDP no need to notify; as it can't free the swap memory 
            iFreeMemoryCalled = EFalse;
            return;
            }
        }
     else
        {
        //Unpaged memory will be freed in case of non WDP app 
        //RDebug::Printf("COomRunPlugin::FreeMemory: Requesting to free the RAM \n");
        iPlugin.FreeRam();
        }
    
    // Wait for the required time before we signal completion.
    __ASSERT_DEBUG(!iPluginWaiter->IsActive(), OomMonitorPanic(KStartingActivePluginWaiter));    
    iPluginWaiter->Start();
    }

// Call the memory good function on the plugin but...
// only if there is an outstanding FreeMemory request
void COomRunPlugin::MemoryGood()
    {
    FUNC_LOG;

    if (iFreeMemoryCalled)
        {
        iPlugin.MemoryGood();
        iFreeMemoryCalled = EFalse;
        }
    }

COomRunPlugin::~COomRunPlugin()
    {
    FUNC_LOG;

    delete iPluginWaiter;
    }

COomRunPlugin::COomRunPlugin(TUint aPluginId, COomRunPluginConfig& aConfig, MOomActionObserver& aStateChangeObserver, COomMonitorPlugin& aPlugin, COomMonitorPluginV2* aV2Plugin) : COomAction(aStateChangeObserver), iPluginId(aPluginId), iPlugin(aPlugin), iConfig(aConfig), iV2Plugin(aV2Plugin)
    {
    FUNC_LOG;
    }

void COomRunPlugin::ConstructL(COomRunPluginConfig& aPluginConfig)
    {
    FUNC_LOG;

    TInt waitDuration = CMemoryMonitor::GlobalConfig().iDefaultWaitAfterPlugin;
    
    if (aPluginConfig.WaitAfterPluginDefined())
        {
        // If the wait duration for this plugin is overridden then use the overridden value
        waitDuration = aPluginConfig.WaitAfterPlugin();
        }
    
    iPluginWaiter = COomPluginWaiter::NewL(waitDuration, *this);
    }

//-----------------------------------------------------------------------------
// Function: IsAppDataPaged
// Checks whether the application which implements the given 
// COomMonitorPlugin is data paged
//-----------------------------------------------------------------------------
//
TBool COomRunPlugin::IsAppDataPaged
    ( COomMonitorPlugin& aPlugin ) const
    {
    FUNC_LOG;
    /*
    /TRACES1("COomRunPlugin::IsAppDataPaged: aPlugin.AppId() = %x ", aPlugin.AppId() );        
    //RDebug::Printf("COomRunPlugin::IsAppDataPaged: aPlugin.AppId() = %x ", aPlugin.AppId() );
     */
    //the function find the process with given appid and check
    //application supports the data paging.
    RProcess clientprocess;    
    TApaTaskList taskList(aPlugin.WsSession());
    RThread clientthread; 
    TInt err = clientthread.Open
            (taskList.FindApp(aPlugin.AppId()).ThreadId());
    
    //RDebug::Printf("COomRunPlugin::IsAppDataPaged: clientthread.Open() err = %d ", err );
    
    if( !err )
        {
        //getting the process which provides the plugin
        err = clientthread.Process(clientprocess);
        RDebug::Printf("COomRunPlugin::IsAppDataPaged: clientthread.Process() err = %d ", err );
        
        if( !err )
            {
            //checking whether the process is Data paged or not
            TBool isdatapaged = clientprocess.DefaultDataPaged();
            clientprocess.Close();
            clientthread.Close();
            //RDebug::Printf("COomRunPlugin::IsAppDataPaged: isdatapaged = %d ", isdatapaged );
            
            return isdatapaged;
            }
        clientthread.Close();
        }    
    //could not successfully open the handle of process or thread, 
    //the return EFalse 
    return EFalse;
    }
