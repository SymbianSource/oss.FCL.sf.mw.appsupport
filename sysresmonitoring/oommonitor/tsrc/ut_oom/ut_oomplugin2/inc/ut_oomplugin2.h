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




#ifndef UT_OOMPLUGIN2_H
#define UT_OOMPLUGIN2_H

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <oommonitorplugin.h>
#include "mock_oomactionlist.h"



class CUtOomMonitorPluginV2 : public COomMonitorPluginV2
	{
public:
	static CUtOomMonitorPluginV2* NewL(TInt aInstance);
	~CUtOomMonitorPluginV2();
private:
	CUtOomMonitorPluginV2(TInt aInstance);
	void ConstructL();
	//from COomMonitorPlugin
	virtual void FreeRam(TInt aBytesRequested);
	virtual void MemoryGood();

private:
	TInt iInstance;
	TInt iLowMemoryCallCount;
	TInt iGoodMemoryCallCount;
	};



#endif //UT_OOMPLUGIN2_H
