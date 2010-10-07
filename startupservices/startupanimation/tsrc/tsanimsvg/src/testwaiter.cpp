/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of CTestWaiter class
*
*/


#include "testwaiter.h"

#pragma tagging OFF // To disable CodeTest tagging

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTestWaiter::NewL
//
// ---------------------------------------------------------------------------
//
CTestWaiter* CTestWaiter::NewL()
    {
    CTestWaiter* self = new( ELeave ) CTestWaiter;
    return self;
    }


// ---------------------------------------------------------------------------
// CTestWaiter::~CTestWaiter
//
// ---------------------------------------------------------------------------
//
CTestWaiter::~CTestWaiter()
    {
    }


// ---------------------------------------------------------------------------
// CTestWaiter::Activate
//
// ---------------------------------------------------------------------------
//
void CTestWaiter::Activate()
    {
    __ASSERT_ALWAYS( !IsActive(), User::Invariant() );

    SetActive();
    }


// ---------------------------------------------------------------------------
// CTestWaiter::CTestWaiter
//
// ---------------------------------------------------------------------------
//
CTestWaiter::CTestWaiter()
  : CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CTestWaiter::DoCancel
//
// ---------------------------------------------------------------------------
//
void CTestWaiter::DoCancel()
    {
    }


// ---------------------------------------------------------------------------
// CTestWaiter::RunL
//
// ---------------------------------------------------------------------------
//
void CTestWaiter::RunL()
    {
    CActiveScheduler::Stop();
    }

#pragma tagging ON // To stop disabling CodeTest tagging
