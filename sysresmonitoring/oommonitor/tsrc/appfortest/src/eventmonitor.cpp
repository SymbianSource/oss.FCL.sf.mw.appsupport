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
* Description: Eventmonitor active object.
*
*/

#include <coedef.h>
#include <apgtask.h>
#include <apgwgnam.h>
#include <apgcli.h>
#include <w32std.h>

#include "eventmonitor.h"
#include "controler.h"

CEventMonitor* CEventMonitor::NewL(CControler& aControler, RWsSession& aWsSession, 
        RWindowGroup& aWindowGroup)
    {
    CEventMonitor* self = new (ELeave) CEventMonitor(aControler, aWsSession, aWindowGroup);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CEventMonitor::CEventMonitor(CControler& aControler, RWsSession& aWsSession, 
        RWindowGroup& aWindowGroup): CActive(CActive::EPriorityHigh), iYlEnv(aControler)
        ,iWsSession(aWsSession), iWindowGroup(aWindowGroup)
    {
    CActiveScheduler::Add(this);
    }

void CEventMonitor::ConstructL()
    {
    iWindowGroup.EnableFocusChangeEvents();

    CApaWindowGroupName* wgName = CApaWindowGroupName::NewLC(iWsSession);
    wgName->SetHidden(ETrue);
    wgName->SetWindowGroupName(iWindowGroup);
    CleanupStack::PopAndDestroy(wgName);

    for (TInt i = '0'; i <= '9'; i++)
        {
        iKeyCodes.AppendL(i);
        }
    for (TInt i = 'a'; i <= 'z'; i++)
        {
        iKeyCodes.AppendL(i);
        }
    iKeyCodes.AppendL('*');
    iKeyCodes.AppendL('#');
    iKeyCodes.AppendL(EKeyBackspace);
    iKeyCodes.AppendL(EKeyDevice3);
    iKeyCodes.AppendL(EKeyDevice0);
    iKeyCodes.AppendL(EKeyDevice1);
    iKeyCodes.AppendL(EKeyLeftArrow);
    iKeyCodes.AppendL(EKeyRightArrow);
    iKeyCodes.AppendL(EKeyUpArrow);
    iKeyCodes.AppendL(EKeyDownArrow);
    iKeyCodes.AppendL(EKeyYes);

    iWsSession.EventReady(&iStatus);
    SetActive();
    }

CEventMonitor::~CEventMonitor()
    {
    Cancel();
    iKeyCodes.Close();
    iKeyCodeHandles.Close();
    }

void CEventMonitor::CaptureKeyL()
    {
    Cancel();
    iKeyCodeHandles.Reset();
    for (TInt i = 0; i < iKeyCodes.Count(); i++)
        {
        iKeyCodeHandles.AppendL(iWindowGroup.CaptureKey(iKeyCodes[i], 0, 0));
        }
    iWsSession.EventReady(&iStatus);
    SetActive();
    }

void CEventMonitor::CancelCapture()
    {
    Cancel();
    for (TInt i = 0; i < iKeyCodeHandles.Count(); i++)
        {
        iWindowGroup.CancelCaptureKey(iKeyCodeHandles[i]);
        }
    iKeyCodeHandles.Reset();
    iWsSession.EventReady(&iStatus);
    SetActive();
    }
void CEventMonitor::RunL()
    {
    TWsEvent event;
    iWsSession.GetEvent(event);

    TInt eventType = event.Type();
    if (eventType == EEventFocusGroupChanged)
        {
        iYlEnv.HandleChangeForeGroundL();
        }
    else if (eventType == EEventKey)
        {
        if (!iYlEnv.HandleKeyEventL(*event.Key(), EEventKey))
            {
            TInt wgId = iWsSession.GetFocusWindowGroup();
            if (wgId != KErrNotFound)
                {
                iWsSession.SendEventToWindowGroup(wgId, event);
                }
            }
        }
    iWsSession.EventReady(&iStatus);
    SetActive();
    }

void CEventMonitor::DoCancel()
    {
    iWsSession.EventReadyCancel();
    }

