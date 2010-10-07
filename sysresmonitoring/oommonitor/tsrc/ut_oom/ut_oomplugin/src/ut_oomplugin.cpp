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
 * Description:  
 *
 */
#include <e32property.h> 
#include "ut_oomplugin.h"

const TUid KMyAppUid =  { 0x20033981 }; // a nonsense UID for testing

CUtOomMonitorPlugin::CUtOomMonitorPlugin()
    {
    RDebug::Print(_L("CUtOomMonitorPlugin::COomMonitorTestPlugin"));
    }

CUtOomMonitorPlugin::~CUtOomMonitorPlugin()
    {
    RDebug::Print(_L("CUtOomMonitorPlugin::~COomMonitorTestPlugin"));
    }

void CUtOomMonitorPlugin::ConstructL()
    {
    COomMonitorPlugin::ConstructL();
    RDebug::Print(_L("CUtOomMonitorPlugin::ConstructL"));
    }
void CUtOomMonitorPlugin::FreeRam()
    {
    RDebug::Print(_L("CUtOomMonitorPlugin::FreeRam"));
    }
    
void CUtOomMonitorPlugin::MemoryGood()
    {
    TInt err;
    TAny * pointer = NULL;
    RDebug::Print(_L("CUtOomMonitorPlugin::MemoryGood"));
    ExtensionInterface(KMyAppUid,pointer);
    COomMonitorPlugin::FsSession();
    RProperty oomProperty;
    err = oomProperty.Attach(KUtOomWatcherTestProperty, KUtOomPluginWatcherTestKey, EOwnerThread);
    if(!err)
        {
        oomProperty.Set(KUtOomPluginMemGood);
        }
    oomProperty.Close();
    }

void CUtOomMonitorPlugin::ExtensionInterface(TUid aInterfaceId,
        TAny*& aImplementaion)
    {
    RDebug::Print(_L("CUtOomMonitorPlugin::ExtensionInterface(%08x, %08x)"),
            aInterfaceId.iUid, aImplementaion);
    COomMonitorPlugin::ExtensionInterface(aInterfaceId, aImplementaion);
    }

TAny* CreateFullPluginL()
    {
    CUtOomMonitorPlugin* plugin = new (ELeave) CUtOomMonitorPlugin;
    CleanupStack::PushL(plugin);
    plugin->ConstructL();
    CleanupStack::Pop(plugin);
    plugin->FreeRam();
    plugin->MemoryGood();
    return plugin;
    }

TAny* CreateAppPluginL()
    {
    return CAppOomMonitorPlugin::NewL(KMyAppUid);
    }

//
// The ECom registration code
//

const TImplementationProxy ImplementationTable[] =
    {
    {{0x2003397f}, ::CreateFullPluginL},
    {{0x20033980}, ::CreateAppPluginL}
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
        TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
