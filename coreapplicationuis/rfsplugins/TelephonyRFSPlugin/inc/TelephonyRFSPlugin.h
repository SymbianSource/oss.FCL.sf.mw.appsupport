/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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



#ifndef TELEPHONYRFSPLUGIN_H
#define TELEPHONYRFSPLUGIN_H

//  INCLUDES
#include <e32base.h>

#include "rfsPlugin.h"

// CLASS DECLARATION

class CTelephonyRFSPlugin: public CRFSPlugin
    {
public:

    /**
    * Two-phased constructor.
	* @param	aInitParams initial parameters
    */
    static CTelephonyRFSPlugin* NewL(TAny* aInitParams);

    /**
    * Destructor.
    */
    virtual ~CTelephonyRFSPlugin();

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
private:

    /**
    * C++ default constructor.
    */
    CTelephonyRFSPlugin();

    /**
    * Constructor.
    * @param	aInitParams initial parameters
	*/
	CTelephonyRFSPlugin(TAny* aInitParams);

    /**
    * Constructor
    */
    void ConstructL();
    };



#endif      // TELEPHONYRFSPLUGIN_H

// End of File
