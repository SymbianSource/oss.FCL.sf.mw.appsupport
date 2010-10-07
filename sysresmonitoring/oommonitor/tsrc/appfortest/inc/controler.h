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
* Description: This file is the head file of controler.cpp
*
*/

#ifndef CONTROL_H
#define CONTROL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>     // ws32.lib

class CRedrawMonitor;
class CEventMonitor;

class CControler: public CBase
    {
    public:
        ~CControler();
        static CControler* NewL();
        void ExecuteD();
        void Exit();
        void Paint(TUint aHandle, const TRect& aRect);
        void HandleChangeForeGroundL();
        TBool HandleKeyEventL(const TKeyEvent& aKey, TEventCode aCode);
        TUid GetForeGroundAppUid();
        
    private:
        CControler();
        void ConstructL();

    private:
        CActiveScheduler* iActiveScheduler;
        CWsScreenDevice* iScreenDevice;
        RWsSession       iWsSession;
        RWindowGroup     iWindowGroup;
        RWindow*         iWindow;
        CWindowGc*       iWindowGc;
        CFont*           iFont;
        CRedrawMonitor*  iRedrawMonitor;
        CEventMonitor*   iEventMonitor;
        
        TBuf<100>        iBuffer;
    };

#endif // CONTROL_H
