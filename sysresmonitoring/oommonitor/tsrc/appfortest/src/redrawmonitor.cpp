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
* Description: Redrawmonitor active object
*
*/

#include <w32std.h>

#include "controler.h"
#include "redrawmonitor.h"

CRedrawMonitor::CRedrawMonitor(CControler& aControler, RWsSession& aWsSession, 
        RWindowGroup& aWindowGroup) : CActive(CActive::EPriorityLow),iYlEnv(aControler)
        ,iWsSession(aWsSession), iWindowGroup(aWindowGroup)
    {
    CActiveScheduler::Add(this);
    }
CRedrawMonitor* CRedrawMonitor::NewL(CControler& aControler, RWsSession& aWsSession, 
        RWindowGroup& aWindowGroup)
    {
    CRedrawMonitor* self = new (ELeave) CRedrawMonitor(aControler, aWsSession, aWindowGroup);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }
void CRedrawMonitor::ConstructL()
    {
    iWsSession.RedrawReady(&iStatus);
    SetActive();
    }
CRedrawMonitor::~CRedrawMonitor()
    {
    Cancel();
    }
void CRedrawMonitor::RunL()
    {
    if (iStatus == KErrNone)
        {
        TWsRedrawEvent event;
        iWsSession.GetRedraw(event);
        iYlEnv.Paint(event.Handle(), event.Rect());
        iWsSession.RedrawReady(&iStatus);
        SetActive();
        }
    }
void CRedrawMonitor::DoCancel()
    {
    iWsSession.RedrawReadyCancel();
    }

