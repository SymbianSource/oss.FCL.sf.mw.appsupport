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
* Description:  BatIndicatorPanePlugin plugin implementation
*
*/


#ifndef BATINDICATORPANEPLUGINIMPL_H
#define BATINDICATORPANEPLUGINIMPL_H

//  INCLUDES
#include <AknIndicatorPlugin.h> // CAknIndicatorPlugin
#include <eikcmobs.h>
#include <psmclientobserver.h> // CPsmClient
#include <ConeResLoader.h> // RConeResourceLoader
#include "batterypopupcontrol.h"

class CPsmClient;
class CBSUtil;

/**
*  BatIndicatorPanePlugin Plug-in implementation
*
*  @lib BatIndicatorPanePlugin
*  @since S60 5.0
*/
class CBatIndicatorPanePluginImpl : public CAknIndicatorPlugin,                                    
                                    public MEikCommandObserver,
                                    public MPsmClientObserver
{

private:
    
    // Preview popup command ids
    enum TLinkCommandID
        {
        ELinkNone = 0,
        ELinkFirst = 1
        };             
        
public:  // Constructors and destructor

    /**
     * Two-phased constructor.
     * @since S60 5.0
     * @param aCallback Callback for psm client.
     */        
    static CBatIndicatorPanePluginImpl* NewL( );
      
    /**
     * Destructor.
     */
    ~CBatIndicatorPanePluginImpl();

// From MEikCommandObserver
    
    /**
     * @see MEikCommandObserver
     */
    void ProcessCommandL( TInt aCommandId );

private:

    /**
     * C++ default constructor.
     */
    CBatIndicatorPanePluginImpl();
        
    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL( );

    /**
     * Shows preview popup. 
     *
     * @since S60 5.0
     */
    void ShowPreviewPopupL( );
        
    /**
     * Enables or disables power save mode. 
     * If the wished state is already active, does nothing.
     *
     * @since S60 5.0     
     */
    void SetPowerSaveModeEnabled( );
    
    /**
     * Gets local variation for plugin. 
     *
     * @since S60 5.0     
     */
    void GetFeatures( );
    
    /**
     * This method can check if feature is supported or not.
     * @param TInt aFeatureId (defined in batindpanepluginprivatecrkeys.h)  
     * @return TInt: aFeatureId supported if != 0
     * 
     * @since S60 5.0
     */         
    TInt Supported( TInt aFeatureId );

// From CAknIndicatorPlugin
        
    /**
     * @see CAknIndicatorPlugin
     */
    void HandleIndicatorTapL( const TInt aUid );
           
// from MPsmClientObserver
    
    /**
     * @see MPsmClientObserver
     */
    virtual void PowerSaveModeChanged( const TPsmsrvMode aMode );
    
    /**
     * @see MPsmClientObserver
     */
    virtual void PowerSaveModeChangeError( const TInt aError );
       
private: // data
    
	/**
     * Actual content
     * Own.
     */
	HBufC* iContentText;
	
	/**
     * Link text
     * Own.
     */
	HBufC* iLinkText;
        
    /**
     * PSM Server client.
     * Own.
     */
    CPsmClient* iPsmClient;

    /**
     * Battery status utility.
     * Own.
     */
    CBSUtil* iBatteryStateUtil;
    
    /**
     * Control for preview popup.
     * Own.
     */
    CBatteryPopupControl* iPopupControl;
	  
	/**
     * Resource loader.
     */
    RConeResourceLoader iResourceLoader;
    
    /**
     * Value of KBatIndPanePlugin.
     */
    TInt iBatIndPanePluginFeatures;    
    };

#endif      // BATINDICATORPANEPLUGINIMPL_H

// End of File
