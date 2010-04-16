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
*/


// INCLUDE FILES
#include <FavouritesDb.h>
#include <FavouritesLimits.h>
#include "FavouritesRFSPluginPrivateCRKeys.h"
#include "FavouritesRFSPlugin.h"

_LIT( KRfsDefaultBookmarkDbPath, "Z:\\private\\10008D38\\DBS_101FD685_BrowserBookmarks.db" );

const TInt KHexLength = 8;
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CFavouritesRFSPlugin::CFavouritesRFSPlugin()
    {
    }

CFavouritesRFSPlugin::CFavouritesRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CFavouritesRFSPlugin::~CFavouritesRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CFavouritesRFSPlugin* CFavouritesRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CFavouritesRFSPlugin::NewL()"));
    #endif
    CFavouritesRFSPlugin* self = new (ELeave) CFavouritesRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CFavouritesRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CFavouritesRFSPlugin::ConstructL()"));
    #endif
    }

void CFavouritesRFSPlugin::RestoreFactorySettingsL( const TRfsReason aType )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CFavouritesRFSPlugin::RestoreFactorySettingsL(%d)"), aType);
    #endif
    if (aType == ENormalRfs)
        {    
    	RFavouritesDb::RestoreFactorySettingsL(KBrowserBookmarks, KRfsDefaultBookmarkDbPath, *this );
        }
    #ifdef _DEBUG
        RDebug::Print(_L("CFavouritesRFSPlugin::RestoreFactorySettingsL: End"));
    #endif
    }

void CFavouritesRFSPlugin::GetScriptL( const TRfsReason aType, TDes& aPath )
    {
    if (aType == EInitRfs)
        {
        // Add CenRep UID and key
        TUid hexuid(KCRUidFavouritesRFSPlugin);
        TUint32 hexkey(KInitFavouritesRFSPlugin);
        HBufC* hbuf = HBufC::New( KMaxPath );
        TPtr buf = hbuf->Des();
        buf.Zero();
        buf.AppendNumFixedWidthUC( hexuid.iUid, EHex, KHexLength );
        buf.Append(_L(":"));
        buf.AppendNumFixedWidthUC( hexkey, EHex, KHexLength );
        buf.Append(_L(";"));
        RDebug::Print( buf );
        aPath.Copy( buf );    
        delete hbuf;
        }        
    }

void CFavouritesRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& aCommand )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CFavouritesRFSPlugin::ExecuteCustomCommandL"));
        RDebug::Print(_L("CFavouritesRFSPlugin::ExecuteCustomCommandL Command: %S"), &aCommand);
    #endif
    }

TBool CFavouritesRFSPlugin::MapAccessPoint( TUint aOldUid, TUint& aNewUid )
    {
    aNewUid = aOldUid;
    return ETrue;
    }

// End of file
