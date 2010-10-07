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
* Description: This file is the head file of redrawmonitor.cpp
*
*/

#ifndef REDRAWMONITOR_H
#define REDRAWMONITOR_H

#include <e32base.h>
#include <coedef.h>

class CControler;

class CRedrawMonitor: public CActive
    {
    public:
        static  CRedrawMonitor* NewL(CControler&, RWsSession&, RWindowGroup&);
        virtual ~CRedrawMonitor();
    protected:
        CRedrawMonitor(CControler&, RWsSession&, RWindowGroup&);
        void    ConstructL();
        void    DoCancel();
        void    RunL();
    private:
        CControler& iYlEnv;
        RWsSession      iWsSession;
        RWindowGroup    iWindowGroup;
    };

#endif // REDRAWMONITOR_H
