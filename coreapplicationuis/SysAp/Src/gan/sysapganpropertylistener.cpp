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
* Description:  Listens to given P&S key and notifies
*                observer whenever the value changes.
*/


#include "sysapganpropertylistener.h" 

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CSysApGanPropertyListener::CSysApGanPropertyListener( 
    RProperty& aProperty,
    MSysApGanPropertyObserver& aObserver ) :
    CActive( CActive::EPriorityStandard ),
    iObserver( aObserver ),
    iProperty( aProperty )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Static constructor.
// ---------------------------------------------------------------------------
//
CSysApGanPropertyListener* CSysApGanPropertyListener::NewL(
    RProperty& aProperty,
    MSysApGanPropertyObserver& aObserver )
    {
    CSysApGanPropertyListener* self = 
        new ( ELeave ) CSysApGanPropertyListener( aProperty, aObserver ); 
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CSysApGanPropertyListener::~CSysApGanPropertyListener()
    {
    Cancel(); // Cancel any request, if outstanding
    }

// ---------------------------------------------------------------------------
// Subscribe for property value changes.
// ---------------------------------------------------------------------------
//
void CSysApGanPropertyListener::Subscribe()
    {
    // Cancel any outstanding request
    Cancel();

    iProperty.Subscribe( iStatus );
    SetActive();
    }

// ---------------------------------------------------------------------------
// Property value changed.
// ---------------------------------------------------------------------------
//
void CSysApGanPropertyListener::RunL()
    {
    // Notify the observer.
    iObserver.PropertyEvent( *this, iStatus.Int() );
    }

// ---------------------------------------------------------------------------
// Subscription cancelled.
// ---------------------------------------------------------------------------
//
void CSysApGanPropertyListener::DoCancel()
    {
    iProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// Error in RunL.
// ---------------------------------------------------------------------------
//
TInt CSysApGanPropertyListener::RunError( TInt /*aError*/ )
    {
    // ignore error
    return KErrNone;
    }
