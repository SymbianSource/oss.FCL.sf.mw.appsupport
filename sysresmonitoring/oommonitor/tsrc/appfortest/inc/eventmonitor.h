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
* Description: This file is the head file of eventmonitor.cpp
*
*/

#ifndef EVENTMONITOR_H
#define EVENTMONITOR_H

#include <w32std.h>

class CControler;

class CEventMonitor: public CActive
    {
    public:
        ~CEventMonitor();
        static CEventMonitor* NewL(CControler&, RWsSession&, RWindowGroup&);
    
        void CaptureKeyL();
        void CancelCapture();
        void HandleChangeForeGroundL();
    
    protected:
        CEventMonitor(CControler&, RWsSession&, RWindowGroup&);
        void ConstructL();
        
    private:
        void RunL();
        void DoCancel();
    private:
        
        CControler&     iYlEnv;
        RArray<TInt>    iKeyCodeHandles;
        RArray<TInt>    iKeyCodes;
        RWsSession      iWsSession;
        RWindowGroup    iWindowGroup;
    };
#endif // EVENTMONITOR_H
