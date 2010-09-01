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
* Description:  MSysApGanPropertyObserver class definition. This is a callback
*                interface from RProperty listener to client application.
*
*/

#ifndef MSYSAPGANPROPERTYOBSERVER_H_
#define MSYSAPGANPROPERTYOBSERVER_H_

class CSysApGanPropertyListener;

NONSHARABLE_CLASS( MSysApGanPropertyObserver )
    {
public:
    /**
     * Method is called when property value changes.
     * @param aPropertyListener Listener of the changed property.
     * @param aStatus Status of the event.
     */
    virtual void PropertyEvent( 
        CSysApGanPropertyListener& aPropertyListener, 
        TInt aStatus ) = 0;
    };

#endif /* MSYSAPGANPROPERTYOBSERVER_H_ */
