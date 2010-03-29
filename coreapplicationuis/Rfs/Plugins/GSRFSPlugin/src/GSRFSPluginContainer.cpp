/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*/


//  INCLUDES
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <AknsFrameBackgroundControlContext.h>
#include <bldvariant.hrh>
#include <GSRFSPluginRsc.rsg>
#include <aknlists.h>
#include <StringLoader.h>
#include <csxhelp/cp.hlp.hrh>
#include <gsfwviewuids.h>
#include <gslistbox.h>
#include <settingsinternalcrkeys.h>
#include "GSRFSPluginContainer.h"
#include "GSRFSPluginTraces.h"
#include "GSRFSPlugin.h"
#include "gsrfsplugin.hrh"


// ---------------------------------------------------------
// CGSRFSPluginContainer::CGSRFSPluginContainer()
// ---------------------------------------------------------
CGSRFSPluginContainer::CGSRFSPluginContainer( CGSRFSPlugin* aGSRFSPlugin )
    :iGSRFSPlugin( aGSRFSPlugin )
    {
    TRACES("CGSRFSPluginContainer::CGSRFSPluginContainer()");
    }

// -----------------------------------------------------------------------------
// CGSRFSPluginContainer::ConstructL
//
// -----------------------------------------------------------------------------
void CGSRFSPluginContainer::ConstructL(const TRect& aRect)
    {
    TRACES("CGSRFSPluginContainer::ConstructL()");
    
    iListBox = new( ELeave ) CAknSettingStyleListBox;
    BaseConstructL( aRect, R_RFS_FACTORY_SETTINGS_TITLE, R_RFS_FACTORY_SETTINGS_LBX );
    }

// -----------------------------------------------------------------------------
// CGSRFSPluginContainer::~CGSRFSPluginContainer
//
// -----------------------------------------------------------------------------
CGSRFSPluginContainer::~CGSRFSPluginContainer()
    {
    TRACES("CGSRFSPluginContainer::~CGSRFSPluginContainer()");
    delete iListboxItemArray;
    TRACES("CGSRFSPluginContainer::~CGSRFSPluginContainer(): End");
    }

// ---------------------------------------------------------------------------
// CGSRFSPluginContainer::CurrentFeatureId()
//
// Return the feature id of selected listitem  
// ---------------------------------------------------------------------------
TInt CGSRFSPluginContainer::CurrentFeatureId( ) const
    {
    return iListboxItemArray->CurrentFeature( );
    }

// ---------------------------------------------------------------------------
// CGSRFSPluginContainer::ConstructListBoxL()
// 
// Construct the listbox from resource array.
// ---------------------------------------------------------------------------
void CGSRFSPluginContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId, *iListBox, *iCoeEnv );
    
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    }

// ---------------------------------------------------------------------------
// CGSRFSPluginContainer::GetHelpContext() const
//  
// Gets help context 
// ---------------------------------------------------------------------------
void CGSRFSPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_GENERAL_ORIG_SET;
    }

// End of File  

