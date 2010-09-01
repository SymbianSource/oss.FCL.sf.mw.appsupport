/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*  
*
*/


#ifndef FAVOURITESRFSPLUGIN_H
#define FAVOURITESRFSPLUGIN_H

//  INCLUDES
#include <e32base.h>
#include <rfsApMapper.h>
#include "rfsPlugin.h"

// CLASS DECLARATION

class CFavouritesRFSPlugin: public CRFSPlugin, public MRfsApMapper
    {
public:

    /**
    * Two-phased constructor.
	* @param	aInitParams initial parameters
    */
    static CFavouritesRFSPlugin* NewL(TAny* aInitParams);

    /**
    * Destructor.
    */
    virtual ~CFavouritesRFSPlugin();

    /**
    * 
	* @param
    */
    void RestoreFactorySettingsL( const TRfsReason aType );

    /**
    * 
	* @param
    */
    void GetScriptL( const TRfsReason aType, TDes& aPath );

    /**
    * 
	* @param
    */
    void ExecuteCustomCommandL( const TRfsReason aType, TDesC& aCommand );

public: // from MRfsApMapper

    TBool MapAccessPoint( TUint aOldUid, TUint& aNewUid );

private:

    /**
    * C++ default constructor.
    */
    CFavouritesRFSPlugin();

    /**
    * Constructor.
    * @param	aInitParams initial parameters
	*/
	CFavouritesRFSPlugin(TAny* aInitParams);

    /**
    * Constructor
    */
    void ConstructL();
    };



#endif      // FAVOURITESRFSPLUGIN_H

// End of File
