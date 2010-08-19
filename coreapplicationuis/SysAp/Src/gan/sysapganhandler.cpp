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
* Description:  Handle GAN server events
*
*/

#include <e32base.h>
#include "coreapplicationuisdomainpskeys.h"
#include "sysapganhandler.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CSysApGanHandler::CSysApGanHandler( CSysApAppUi& aSysApAppUi ) :
    iSysApAppUi( aSysApAppUi ),
    iGanMode( EGanModeUnspecified )
    {
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CSysApGanHandler::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApGanHandler::ConstructL IN") ) ); 
    TInt err( KErrNone );
    
    // Start listening for GAN mode
    err = iPropertyGanMode.Attach( KPSUidCoreApplicationUIs, KCoreAppUIsGanPropertyGanMode );
    User::LeaveIfError( err );
    iGanModeListener = CSysApGanPropertyListener::NewL( iPropertyGanMode, *this );
    iGanModeListener->Subscribe();

    // Start listening for signal strength
    err = iPropertySignalLevel.Attach( KPSUidCoreApplicationUIs, KCoreAppUIsGanPropertySignalLevel );
    User::LeaveIfError( err );
    iSignalLevelListener = CSysApGanPropertyListener::NewL( iPropertySignalLevel, *this );
    iSignalLevelListener->Subscribe();

    // Get current GAN mode
    TInt mode;
    err = iPropertyGanMode.Get( mode );
    if( KErrNone == err )
    	{
    	iGanMode = (TCoreAppUIsGanMode)mode;
    	}
    
    // Get current GAN signal level
    iPropertySignalLevel.Get( iGanSignalLevel );
    
    TRACES( RDebug::Print( _L("CSysApGanHandler::ConstructL OUT") ) ); 
    }

// ---------------------------------------------------------------------------
// Static constructor.
// ---------------------------------------------------------------------------
//
CSysApGanHandler* CSysApGanHandler::NewL( CSysApAppUi& aSysApAppUi )
    {
    CSysApGanHandler* self = new (ELeave) CSysApGanHandler( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CSysApGanHandler::~CSysApGanHandler()
    {
    delete iGanModeListener;
    delete iSignalLevelListener;
    iPropertyGanMode.Close();
    iPropertySignalLevel.Close();    
    }

TBool CSysApGanHandler::IsInGanMode()
    {
    return iGanMode == EGanModeGAN;
    }

TInt CSysApGanHandler::GanSignalLevel()
    {
    return iGanSignalLevel;
    }

// ---------------------------------------------------------------------------
// Handle UI Status RProperty change event. Read the new RProperty value
// and publish the content on idle screen.
// ---------------------------------------------------------------------------
//
void CSysApGanHandler::PropertyEvent( 
    CSysApGanPropertyListener& aPropertyListener, 
    TInt aStatus )
    {
    TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent IN") ) );    
    TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent status %d"), aStatus ) );
    
    if( KErrNone == aStatus )
        {
        if( iGanModeListener == &aPropertyListener )
            {
            // GAN mode event
            TInt mode;
            TInt err = iPropertyGanMode.Get( mode );
            iGanMode = (TCoreAppUIsGanMode)mode;
            TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent RProp err %d"), err ) );
            TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent GAN mode %d"), iGanMode ) );
            iSysApAppUi.SetSignalIndicatorL();
            }
        else if( iSignalLevelListener == &aPropertyListener )
            {
            // Signal level event
            TInt err = iPropertySignalLevel.Get( iGanSignalLevel );
            TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent RProp err %d"), err ) );
            TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent signal level %d"), iGanSignalLevel ) );
            iSysApAppUi.UpdateSignalBarsL();
            }
        }
    aPropertyListener.Subscribe();
    TRACES( RDebug::Print( _L("CSysApGanHandler::PropertyEvent OUT") ) ); 
    }

