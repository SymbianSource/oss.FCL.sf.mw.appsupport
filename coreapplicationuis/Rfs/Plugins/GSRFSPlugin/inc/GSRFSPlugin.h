/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  GS Restore Factory Settings plugin
*
*/


#ifndef GSRFSPLUGIN_H
#define GSRFSPLUGIN_H

// Includes
#include <aknsettingpage.h>
#include <gsbaseview.h>
#include <ConeResLoader.h>
#include <gsplugininterface.h>

// Classes referenced
class CRfsHandler;
class CGSRFSPluginContainer;
class CAknViewAppUi;

// Constants
const TUid KGSRFSPluginUid = { 0x10275117 };
  

// Class Declaration
/**
 *
 * @since Series60_3.1
 */
class CGSRFSPlugin : public CGSBaseView
    {
public: // Constructors and destructor
        
    /**
     * Symbian OS two-phased constructor
     * @return 
     */
    static CGSRFSPlugin* NewL( TAny* aInitParams );
    
    /**
     * Destructor.
     */
     ~CGSRFSPlugin();

public: // From CAknView
                
     /**
      * See base class.
      *
      * This function is used also for identifying the plugin so be sure to
      * return correct UID.
      */
     TUid Id() const;
        
     /**
      * See base class.
      */
     void HandleClientRectChange();
        
     /**
      * See base class.
      */
     void DoActivateL( const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage );
        
     /**
      * See base class.
      */
     void DoDeactivate();
        
     /**
      * See base class.
      */
     void HandleCommandL( TInt aCommand );
        

public: // From CGSPluginInterface
       
     /**
      * See base class.
      */
     void GetCaptionL( TDes& aCaption ) const;
        
     /**
      * See base class.
      */
     TInt PluginProviderCategory() const;

     /**
      * See base class.
      */
     CGulIcon* CreateIconL( const TUid aIconType );
        
protected: // New

     /**
      * C++ default constructor.
      */
     CGSRFSPlugin();

     /**
      * Symbian OS default constructor.
      */
     void ConstructL();
        
     void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
        
private: // from CGSBaseView
        
     void NewContainerL();
        
     void HandleListBoxSelectionL();    
        
private: // Data

     // Resource loader
     RConeResourceLoader iResourceLoader;
    };

#endif // GSRFSPLUGIN_H

// End of File
