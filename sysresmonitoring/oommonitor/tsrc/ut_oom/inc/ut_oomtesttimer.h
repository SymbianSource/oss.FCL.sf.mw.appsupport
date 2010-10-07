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
* Description: This file contains stub for testing CSubscribeHelper
*
*/
#ifndef UT_OOMTESTTIMER_H
#define UT_OOMTESTTIMER_H

#include <e32base.h>

class CUt_OomTestTimer
    : public CTimer
    {
public:  // Constructors and destructor
    static CUt_OomTestTimer* NewL();
    ~CUt_OomTestTimer();

protected: // From CTimer
    void RunL();
    void DoCancel();

private: // Constructors
    CUt_OomTestTimer();
    void ConstructL();

public: // Data
    TBool iIsTimerElapsed;
    
private:
    };

#endif /* UT_OOMTESTTIMER_H */
