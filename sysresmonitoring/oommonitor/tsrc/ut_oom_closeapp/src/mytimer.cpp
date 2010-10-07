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
* Description: This file contains STIFUnit implementation.
*
*/

#include <e32base.h>
#include <e32std.h>
#include "mytimer.h"

_LIT (KExeName, "appfortest.exe");

CMyTimer::CMyTimer(CWservEventReceiver* aWsEventRecv):CTimer(EPriorityStandard),
        iWsEventRecv(aWsEventRecv)
    {
    currentStatus=EPhase1;
    } 

CMyTimer::~CMyTimer()
    {
    iProcess.Close();
    }

CMyTimer* CMyTimer::NewLC(CWservEventReceiver* aWsEventRecv)
    {
    CMyTimer* self=new(ELeave) CMyTimer(aWsEventRecv);  
    CleanupStack::PushL(self);  
    self->ConstructL();  
    return self;  
    }

CMyTimer* CMyTimer::NewL(CWservEventReceiver* aWsEventRecv)
    {
    CMyTimer* self=CMyTimer::NewLC(aWsEventRecv);  
    CleanupStack::Pop(); 
    return self;
    }

void CMyTimer::ConstructL()
    {
    CTimer::ConstructL();  
    CActiveScheduler::Add(this);  
    After(5000000);
    }

void CMyTimer::RunL()
    {
    switch(currentStatus)
        {
        case EPhase1:
            {
            User::LeaveIfError(iProcess.Create(KExeName, KNullDesC) );      
            iProcess.Resume();   
            currentStatus=EPhase2;
            After(5000000); 
            break;
            }
        case EPhase2:
            {
            iProcess.Kill(0);
            iProcess.Close();
            iWsEventRecv->Cancel();
            currentStatus=EPhase3;
            After(5000000); 
            break;
            }
        case EPhase3:
            {
            User::LeaveIfError(iProcess.Create(KExeName, KNullDesC) );      
            iProcess.Resume();   
            currentStatus=EPhase4;
            After(5000000); 
            break;
            }
        case EPhase4:
            {
            iProcess.Kill(0);
            iProcess.Close();
            currentStatus=EPhase5;
            After(5000000);
            break;
            }
        default:
            {
            CActiveScheduler::Stop();
            }
        }
    }
