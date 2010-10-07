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
* Description: This file contains STIFUnit implementation.
*
*/

#ifndef MYTIMER_H_
#define MYTIMER_H_

#include <e32base.h>
#include <oomwserveventreceiver.h>

enum TStatus
    {
    EPhase1, //=0
    EPhase2,
    EPhase3,
    EPhase4,
    EPhase5,
    };

class CMyTimer : public CTimer
    {
public:
    static CMyTimer* NewLC(CWservEventReceiver*);  
    static CMyTimer* NewL(CWservEventReceiver*); 
    virtual ~CMyTimer();
    
protected:   
    CMyTimer(CWservEventReceiver*);

private:   
    void ConstructL();  
    virtual void RunL();   
    
private:
    RProcess iProcess;
    enum TStatus currentStatus;
    CWservEventReceiver* iWsEventRecv;
    };

#endif /* MYTIMER_H_ */
