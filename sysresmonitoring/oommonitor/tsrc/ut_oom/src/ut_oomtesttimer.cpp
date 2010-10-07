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

#include "ut_oomtesttimer.h"

CUt_OomTestTimer* CUt_OomTestTimer::NewL()
    {
    CUt_OomTestTimer* self = new( ELeave ) CUt_OomTestTimer();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CUt_OomTestTimer::~CUt_OomTestTimer()
    {

    }

CUt_OomTestTimer::CUt_OomTestTimer()
    : CTimer(CActive::EPriorityStandard), iIsTimerElapsed(EFalse)
    {
    }

void CUt_OomTestTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }


void CUt_OomTestTimer::RunL()
    {
    iIsTimerElapsed = ETrue;
    CActiveScheduler::Stop();
    }

void CUt_OomTestTimer::DoCancel()
    {
    CTimer::DoCancel(); 
    }

