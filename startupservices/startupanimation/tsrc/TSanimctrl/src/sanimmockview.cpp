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
* Description:  Implementation of CSAnimMockView class
*
*/


#include "sanimmockview.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimMockView::NewL
//
// ---------------------------------------------------------------------------
//
CSAnimMockView* CSAnimMockView::NewL()
    {
    FUNC_LOG;

    CSAnimMockView* self = new( ELeave ) CSAnimMockView;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::~CSAnimMockView
//
// ---------------------------------------------------------------------------
//
CSAnimMockView::~CSAnimMockView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::SetComponent
//
// ---------------------------------------------------------------------------
//
void CSAnimMockView::SetComponent( CCoeControl& aComponent )
    {
    FUNC_LOG;

    iComponent = &aComponent;
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::RemoveComponent
//
// ---------------------------------------------------------------------------
//
void CSAnimMockView::RemoveComponent()
    {
    FUNC_LOG;

    iComponent = NULL;
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::CSAnimMockView
//
// ---------------------------------------------------------------------------
//
CSAnimMockView::CSAnimMockView()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::ConstructL
//
// ---------------------------------------------------------------------------
//
void CSAnimMockView::ConstructL()
    {
    FUNC_LOG;

    CreateWindowL();
    SetRect( TRect( 0, 0, 256, 256 ) );
    ActivateL();
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::CountComponentControls
//
// ---------------------------------------------------------------------------
TInt CSAnimMockView::CountComponentControls() const
    {
    if ( iComponent ) return 1;
    return 0;
    }


// ---------------------------------------------------------------------------
// CSAnimMockView::ComponentControl
//
// ---------------------------------------------------------------------------

CCoeControl* CSAnimMockView::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iComponent;
    }


// ---------------------------------------------------------
// CSAnimMockView::Draw
//
// ---------------------------------------------------------
//
void CSAnimMockView::Draw( const TRect& aRect ) const
    {
    FUNC_LOG;

	CWindowGc& gc = SystemGc();

    gc.Clear( aRect );
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbDarkYellow );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.Clear();
    
    }

