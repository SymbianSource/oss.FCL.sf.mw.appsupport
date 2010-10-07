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
* Description:  Implementation of COutOfDiskNotifyObserver class.
*
*/

#include <e32debug.h>
#include <e32property.h>

#include "MockShutdownEventObserver.h"

TshutdownEventObserver::TshutdownEventObserver()
    {

    }

TshutdownEventObserver::~TshutdownEventObserver()
    {

    }

void TshutdownEventObserver::HandleShutdownEventL(MSaveObserver::TSaveType aAction,TBool aPowerOff, TPowerState aEvent)
    {
    
    }

CArrayFix<TThreadId>* TshutdownEventObserver::ClientArrayLC()
    {
    return (CArrayFix<TThreadId>*)NULL;
    }

TBool TshutdownEventObserver::IsClientHung(TThreadId aId) const
    {
    return ETrue;
    }

void TshutdownEventObserver::GetShutdownState(TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest) const
    {

    }
