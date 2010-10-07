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
* Description: The stub for testing tsp events
*
*/

#ifndef UT_ATSPTSPEVENTSSTUB_H
#define UT_ATSPTSPEVENTSSTUB_H

#include <e32base.h>
#include "tsptriggerevents.h"

class CUt_AtspTspEventsStub
    : public CTimer,
      public MTspRulesTriggerObserver
    {
public:  // Constructors and destructor
    static CUt_AtspTspEventsStub* NewL();
    ~CUt_AtspTspEventsStub();

public: // Issue test event
    void IssueAudioPropertyL();
    void IssueTspPropertyL();
    void IssueWsEventL(const TInt aType);
public: // From MTspRulesTriggerObserver
    void MtrtoEvaluateRoutingRules();
    
protected: // From CTimer
    void RunL();
    void DoCancel();

private: // Constructors
    CUt_AtspTspEventsStub();
    void ConstructL();

public: // Data
    TBool iIsRunLCalled;
    TBool iIsTimerElapsed;
    
private:
    };

#endif /* UT_ATSPTSPEVENTSSTUB_H */
