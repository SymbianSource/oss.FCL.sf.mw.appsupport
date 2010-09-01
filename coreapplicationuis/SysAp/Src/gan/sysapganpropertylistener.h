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
* Description:  CSysApGanPropertyListener class definition. Class for
*                RProperty changes from GAN server.
*/


#ifndef C_SYSAPGANPROPERTYLISTENER_H
#define C_SYSAPGANPROPERTYLISTENER_H

#include <e32base.h>
#include <e32property.h>
#include "msysapganpropertyobserver.h"

/**
 *  Listen RProperty events.
 */
NONSHARABLE_CLASS( CSysApGanPropertyListener ) : 
    public CActive
    {
public:
    /**
     * Static constructor.
     * @param aProperty Property to listen to.
     * @param aObserver Object that is informed about about new IAP usage.
     */
    static CSysApGanPropertyListener* NewL( 
        RProperty& aProperty,
        MSysApGanPropertyObserver& aObserver );
    
    /**
     * Destructor.
     */
    ~CSysApGanPropertyListener();

protected:
    /**
     * C++ constructor.
     * @param aProperty Property to listen to.
     * @param aObserver Object that is informed about about new IAP usage.
     */
    CSysApGanPropertyListener( 
        RProperty& aProperty,
        MSysApGanPropertyObserver& aObserver );
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();
    
public:
    /**
     * Subscribes to a property. 
     */
    void Subscribe();

protected: // From CActive

    /**
     * Handles an active object's request completion event.
     */
    void RunL();
    
    /**
     * Cancels asynchronous request.
     */
    void DoCancel();
    
    /**
     * Handles a leave occurring in RunL().
     * @param aError Error that caused RunL to leave.
     */
    TInt RunError( TInt aError );   

protected: // Data

    /**
     * Observer to notifiy about property changes.
     */
    MSysApGanPropertyObserver& iObserver;
    
    /**
     * Property to listen to.
     */
    RProperty&   iProperty;
    
    };

#endif // C_SYSAPGANPROPERTYLISTENER_H
