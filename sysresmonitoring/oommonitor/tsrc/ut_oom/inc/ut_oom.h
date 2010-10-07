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
* Description:  Main classes for Out of Memory Monitor.
*
*/
#ifndef UT_OOM_H
#define UT_OOM_H

//redeine plugin uid for unit testing 
//This is must because OS oom plugin can only created by OOM monitor thread
#ifdef  KOomPluginInterfaceUidValue
#undef  KOomPluginInterfaceUidValue  
#define KOomPluginInterfaceUidValue 0x2003397d
#endif

#ifdef KOomPluginInterfaceUid
#undef KOomPluginInterfaceUid
#define KOomPluginInterfaceUid (TUid::Uid(KOomPluginInterfaceUidValue))
#endif 

#ifdef  KOomPluginInterfaceV2UidValue
#undef  KOomPluginInterfaceV2UidValue  
#define KOomPluginInterfaceV2UidValue 0x2003397e
#endif

#ifdef KOomPluginInterfaceV2Uid
#undef KOomPluginInterfaceV2Uid
#define KOomPluginInterfaceV2Uid (TUid::Uid(KOomPluginInterfaceV2UidValue))
#endif 
#endif //UT_OOM_H
