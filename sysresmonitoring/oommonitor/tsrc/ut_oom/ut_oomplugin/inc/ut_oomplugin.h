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




#ifndef UT_OOMPLUGIN_H
#define UT_OOMPLUGIN_H
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <oommonitorplugin.h>
#include "mock_oomactionlist.h"

class CUtOomMonitorPlugin : public COomMonitorPlugin
	{
public:
	CUtOomMonitorPlugin();
	~CUtOomMonitorPlugin();
	void ConstructL();
		
public:
	void FreeRam();
	void MemoryGood();

private:
	void ExtensionInterface(TUid aInterfaceId, TAny*& aImplementaion);
	};

#endif //UT_OOMPLUGIN_H
