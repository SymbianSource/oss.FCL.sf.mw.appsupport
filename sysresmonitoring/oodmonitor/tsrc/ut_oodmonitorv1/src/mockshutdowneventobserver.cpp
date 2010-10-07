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

#include <e32debug.h>
#include <e32property.h>

#include "mockshutdowneventobserver.h"
#include "ut_oodmonitor_const.h"

ShutdownEventObserver::ShutdownEventObserver()
    {

    }

ShutdownEventObserver::~ShutdownEventObserver()
    {

    }

void ShutdownEventObserver::HandleShutdownEventL(MSaveObserver::TSaveType aAction,TBool aPowerOff, TPowerState aEvent)
    {
    if((MSaveObserver::ESaveData == aAction) && aPowerOff)
        {
        TInt err;
        err=RProperty::Set(KUTSysApPropertyCategory, KHandleShutdownEventKey, KHandleShutdownEventValue1);
        RDebug::Print(_L("CLafShutdownManager:: err=%d"),err);
        }
    }

CArrayFix<TThreadId>* ShutdownEventObserver::ClientArrayLC()
    {
    return (CArrayFix<TThreadId>*)NULL;
    }

TBool ShutdownEventObserver::IsClientHung(TThreadId aId) const
    {
    return ETrue;
    }

void ShutdownEventObserver::GetShutdownState(TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest) const
    {
    if(aAllSessionsHavePendingRequest && aPowerOff)
        {
        TInt err;
        err=RProperty::Set(KUTSysApPropertyCategory, KGetShutdownStateKey, KGetShutdownStateValue1);
        RDebug::Print(_L("CLafShutdownManager:: err=%d"),err);
        }
    }
