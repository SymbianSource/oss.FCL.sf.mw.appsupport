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

#ifndef MOCKSHUTDOWNEVENTOBSERVER_H_
#define MOCKSHUTDOWNEVENTOBSERVER_H_

#include <shutdownsrv.h>

class TshutdownEventObserver : public MShutdownEventObserver
    {
public:
    TshutdownEventObserver();
    virtual ~TshutdownEventObserver();
    virtual void HandleShutdownEventL(MSaveObserver::TSaveType aAction,TBool aPowerOff, 
            TPowerState aEvent = EPwStandby);
    virtual CArrayFix<TThreadId>* ClientArrayLC();
    virtual TBool IsClientHung(TThreadId aId) const;
    virtual void GetShutdownState(TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest) const;
    };

#endif /* MOCKSHUTDOWNEVENTOBSERVER_H_ */
