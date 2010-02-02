/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#include <oommonitor.h>
#include <oommonitorplugin.h>

CMemoryMonitor* CMemoryMonitor::NewL()
	{
    CMemoryMonitor* self = new(ELeave) CMemoryMonitor();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;		
	}

CMemoryMonitor::CMemoryMonitor():iCurrentTask(iWs)
    {
    SetMemoryMonitorTls(this);
    }

void CMemoryMonitor::ConstructL()
	{
	
	}

CMemoryMonitor::~CMemoryMonitor()
    {
    }
