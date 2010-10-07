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

#ifndef UT_OODMONITOR_TIMER_H_
#define UT_OODMONITOR_TIMER_H_

#include <e32base.h>

#include "ut_lafshutdownmanager.h"

enum TPhase
    {
    EPhase1, //=0
    EPhase2,
    EPhase3,
    EPhase4,
    EPhase5,
    EPhase6,
    EPhase7,
    EPhase8,
    EPhase9,
    EPhase10,
    EPhase11
    };

enum TTestCase
    {
    EMMCTest, //=0
    EDisplayNotes,
    EAutolock,
    EOODWatcher
    };

class Cut_OODMonitor_Timer : public CTimer
    {
public:
    static Cut_OODMonitor_Timer* NewLC(CLafShutdownManager* aShutdownManager, Tut_LafShutdownManager& aFriendLafShutdownManager, TTestCase aTestCase);  
    static Cut_OODMonitor_Timer* NewL(CLafShutdownManager* aShutdownManager, Tut_LafShutdownManager& aFriendLafShutdownManager, TTestCase aTestCase);     
    virtual ~Cut_OODMonitor_Timer();

private:   
    Cut_OODMonitor_Timer(CLafShutdownManager* aShutdownManager, Tut_LafShutdownManager& aFriendLafShutdownManager, TTestCase aTestCase);
    void ConstructL();  
    virtual void RunL(); 
    void ChangeMMCStatus();
    void ChangeDisplayNotes();
    void ChangeAutoLockStatus();
   
private:
    Tut_LafShutdownManager& iFriendLafShutdownManager;
    CLafShutdownManager* iShutdownManager;
    TTestCase iTestCase;
    TPhase iPhase;
    CDesCArrayFlat* iFileNameArray;
    };

#endif /* UT_OODMONITOR_TIMER_H_ */
