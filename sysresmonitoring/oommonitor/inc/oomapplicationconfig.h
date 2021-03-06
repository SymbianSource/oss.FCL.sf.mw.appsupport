/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/


#ifndef OOMAPPLICATIONCONFIG_
#define OOMAPPLICATIONCONFIG_

#include <e32base.h>

class COomCloseAppConfig;
class MOomRuleConfig;

/*
 * A list of action configurations for a particular target application.
 *
 * All configured actions in this list must be for the same target application.
 * Actions may be configured to close the application or to run an OOM plugin.
 *  
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomApplicationConfig) : public CBase
    {
public:
    
    /**
     * Two-phased constructor
     * @param aApplicationId the UID of the application
     */
	static COomApplicationConfig* NewL(TUint aApplicationId);
    
// Functions for getting configuration  
    
    /**
     * Return a pointer to the close app config, this may be NULL if no app close config has been configured
     * Ownership is NOT passed to the caller
     */
    inline COomCloseAppConfig* GetAppCloseConfig();
        
    inline TUint Id();
    
// Functions for setting configuration  

    inline void SetAppCloseConfig(COomCloseAppConfig* aActionConfig);
    
    /**
     * Add a rule
     * 
     * This class takes ownership of the given rule
     * The rule would will apply to the "application close" event
     */
    void AddRuleL(MOomRuleConfig* aRule);
    
    ~COomApplicationConfig();

public:
    
    TUint iGoodRamThreshold;
    TUint iLowRamThreshold; 
    TUint iGoodSwapThreshold;
    TUint iLowSwapThreshold; 
    
private:
    
    void ConstructL();
    
    COomApplicationConfig(TUint aApplicationId);
    
    COomCloseAppConfig* iCloseAppConfig;
    
    TInt iIndex;
    
    TUint iApplicationId;
    };

#include "oomapplicationconfig.inl"

#endif /*OOMAPPLICATIONCONFIG_*/
