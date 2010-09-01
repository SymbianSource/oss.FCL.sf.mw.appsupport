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
* Description:  CSysApGanHandler class definition.
*
*/

#ifndef SYSAPGANHANDLER_H_
#define SYSAPGANHANDLER_H_

#include <e32base.h>
#include <e32property.h>
#include "sysapganpropertylistener.h"

class CSysApAppUi;

/**
 *  Listen RProperty events from GAN server.
 */
NONSHARABLE_CLASS( CSysApGanHandler ) : 
    public CBase, 
    private MSysApGanPropertyObserver    
    {
public:
    /**
     * Static constructor.
     * @param aSysApUi Application UI.
     */
    static CSysApGanHandler* NewL( CSysApAppUi& aSysApAppUi );
    
    /**
     * Destructor.
     */
    ~CSysApGanHandler();
    
    /**
     * Tells if phone is in GAN mode.
     * @return ETrue if in GAN mode (not in GERAN)
     */
    TBool IsInGanMode();
    
    /**
     * Tells the signal bar level in GAN mode.
     * @return The number of signal bar blocks.
     */
    TInt GanSignalLevel();

private: // from MSysApGanPropertyObserver

    /**
     * Receive RProperty notification. This same interface can receive 
     * notifications from multiple listeners.
     * @param aPropertyListener Property lister that received the notification.
     * @param aStatus Status of the notification.
     */
    void PropertyEvent(
        CSysApGanPropertyListener& aPropertyListener, TInt aStatus );
    
private:
    /**
     * C++ constructor.
     * @param aSysApUi Application UI.
     */
    CSysApGanHandler( CSysApAppUi& aSysApAppUi );
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();
    

protected: // Data

    /**
     * Application UI.
     */
    CSysApAppUi& iSysApAppUi;    
    
    /**
     * Current GAN mode. 
     */
    TCoreAppUIsGanMode iGanMode;
    
    /**
     * Current GAN signal level. 
     */
    TInt iGanSignalLevel;
    
    /**
     * GAN mode property. 
     */
    RProperty iPropertyGanMode;
        
    /**
     * Signal level property. 
     */
    RProperty iPropertySignalLevel;
    
    /**
     * Listener for GAN mode property. 
     */
    CSysApGanPropertyListener* iGanModeListener;
    
    /**
     * Listener for signal level property. 
     */
    CSysApGanPropertyListener* iSignalLevelListener;
    
    };

#endif /* SYSAPGANHANDLER_H_ */
