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
* Description:  Implementation of CSAnimCtrlRootSuite class
*
*/


#include "sanimctrlrootsuite.h"
#include "sanimmockview.h"

#pragma tagging OFF // To disable CodeTest tagging

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimCtrlRootSuite::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimCtrlRootSuite* CSAnimCtrlRootSuite::NewL()
    {
    CSAnimCtrlRootSuite* self = CSAnimCtrlRootSuite::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlRootSuite::NewLC
//
// ---------------------------------------------------------------------------
//
CSAnimCtrlRootSuite* CSAnimCtrlRootSuite::NewLC()
    {
    CSAnimCtrlRootSuite* self = new( ELeave ) CSAnimCtrlRootSuite;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlRootSuite::~CSAnimCtrlRootSuite
//
// ---------------------------------------------------------------------------
//
CSAnimCtrlRootSuite::~CSAnimCtrlRootSuite()
    {
    delete iView;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlRootSuite::MainView
//
// ---------------------------------------------------------------------------
//
CSAnimMockView& CSAnimCtrlRootSuite::MainView()
    {
    return *iView;
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlRootSuite::CSAnimCtrlRootSuite
//
// ---------------------------------------------------------------------------
//
CSAnimCtrlRootSuite::CSAnimCtrlRootSuite()
    {
    }


// ---------------------------------------------------------------------------
// CSAnimCtrlRootSuite::ConstructL
//
// ---------------------------------------------------------------------------
//
void CSAnimCtrlRootSuite::ConstructL()
    {
    iView = CSAnimMockView::NewL();
    }


#pragma tagging ON // To stop disabling CodeTest tagging
