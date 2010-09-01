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
* Description: 
*        GS RFS Plugin
*
*/


// SYSTEM INCLUDES
#include <aknViewAppUi.h>
#include <bautils.h>
#include <rfsHandler.h>
#include <gsprivatepluginproviderids.h>
#include <StringLoader.h>
#include <GSRFSPluginRsc.rsg>
#include <GSRFSPlugin.mbg>

#include <hlplch.h>             
#include <gscommon.hrh>
#include <gsfwviewuids.h>
#include "GSRFSPlugin.h"
#include "GSRFSPluginTraces.h"
#include "GSRFSPluginContainer.h"
#include "gsrfsplugin.hrh"

// Constants
_LIT( KGSRFSPluginResourceFileName, "z:GSRFSPluginRsc.rsc" );


// ---------------------------------------------------------------------------
// CGSRFSPlugin::CGSRFSPlugin()
// Constructor
// ---------------------------------------------------------------------------
CGSRFSPlugin::CGSRFSPlugin()
    :iResourceLoader( *iCoeEnv )
    {
    TRACES("CGSRFSPlugin::CGSRFSPlugin()");
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::~CGSRFSPlugin()
// Destructor
// ---------------------------------------------------------------------------
CGSRFSPlugin::~CGSRFSPlugin()
    {
    TRACES("CGSRFSPlugin::~CGSRFSPlugin()");
	  iResourceLoader.Close();
    TRACES("CGSRFSPlugin::~CGSRFSPlugin(): End");
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CGSRFSPlugin::ConstructL()
    {
    TRACES("CGSRFSPlugin::ConstructL()");
    
    // Find the resource file
    TParse parse;
    parse.Set( KGSRFSPluginResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );

    BaseConstructL( R_RFS_FACTORY_SETTINGS_VIEW );

    TRACES("CGSRFSPlugin::ConstructL(): End");
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::NewL()
// Static constructor
// ---------------------------------------------------------------------------
CGSRFSPlugin* CGSRFSPlugin::NewL( TAny* /*aInitParams*/ )
    {
    TRACES("CGSRFSPlugin::NewL()");
    CGSRFSPlugin* self = new( ELeave ) CGSRFSPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TRACES("CGSRFSPlugin::NewL(): End");
    return self;
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::Id()
// ---------------------------------------------------------------------------
TUid CGSRFSPlugin::Id() const
    {
    return KGSRFSPluginUid;
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::DoActivateL()
// ---------------------------------------------------------------------------
void CGSRFSPlugin::DoActivateL( const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage )
    {
    TRACES("CGSRFSPlugin::DoActivateL()");
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    }
    
// ---------------------------------------------------------------------------
// CGSRFSPlugin::DoDeactivate()
// ---------------------------------------------------------------------------
void CGSRFSPlugin::DoDeactivate()
    {
    TRACES("CGSRFSPlugin::DoDeactivate()");
    CGSBaseView::DoDeactivate();
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::HandleCommandL()
// ---------------------------------------------------------------------------
void CGSRFSPlugin::HandleCommandL( TInt aCommand )
    {
    TRACES("CGSRFSPlugin::HandleCommandL()");
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            iAppUi->ActivateLocalViewL( KGSDeviceManagementPluginUid  );
            break;
            }
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
            break;
            }
        default:
            {
            iAppUi->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::GetCaptionL()
// ---------------------------------------------------------------------------
void CGSRFSPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // the resource file is already opened
    HBufC* result = StringLoader::LoadL( R_RFS_FACTORY_SETTINGS );    
    aCaption.Copy( *result );
    delete result;
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::PluginProviderCategory()
// ---------------------------------------------------------------------------
TInt CGSRFSPlugin::PluginProviderCategory() const
    {
    return KGSPluginProviderInternal;
    }

// -----------------------------------------------------------------------------
// CGSNetworkPlugin::CreateIconL()
// -----------------------------------------------------------------------------
CGulIcon* CGSRFSPlugin::CreateIconL( const TUid aIconType )
    {
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    _LIT( KGSRFSPluginIconDirAndName, "z:gsrfsplugin.mbm");
    fp->Set( KGSRFSPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), KAknsIIDQgnPropSetGenePhone,
                                          fp->FullName(),EMbmGsrfspluginQgn_prop_set_gene_phone,
                                          EMbmGsrfspluginQgn_prop_set_gene_phone_mask );
        }
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    return icon;
    }

// -----------------------------------------------------------------------------
// CGSRFSPlugin::DynInitMenuPaneL()
//
// Dynamically handle help item if not supported
// -----------------------------------------------------------------------------
void CGSRFSPlugin::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );
        aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
        }
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::NewContainerL()
//
// Creates new iContainer.
// ---------------------------------------------------------------------------
void CGSRFSPlugin::NewContainerL()
    {
    iContainer = new( ELeave ) CGSRFSPluginContainer( this );
    }

// ---------------------------------------------------------------------------
// CGSRFSPlugin::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
void CGSRFSPlugin::HandleListBoxSelectionL()
    {    
    CGSRFSPluginContainer *pluginContainer = static_cast<CGSRFSPluginContainer*>( iContainer );
    const TInt currentFeatureId = pluginContainer->CurrentFeatureId();         
    
    CRfsHandler* iRfsHandler = new ( ELeave ) CRfsHandler;
    CleanupStack :: PushL(iRfsHandler);
    
    TRfsType rfsType = ERfsNormal;
    if ( currentFeatureId == EGSSettIdOptDeepRfs )
        {
        rfsType = ERfsDeep;
        }
    iRfsHandler->ActivateRfsL( rfsType );
    
    CleanupStack :: PopAndDestroy(iRfsHandler);
    }

// End of file
