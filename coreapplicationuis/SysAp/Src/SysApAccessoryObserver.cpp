/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApAccessoryObserver implementation.
*
*/


// INCLUDE FILES
#include "SysApAccessoryObserver.h"
#include "SysAp.hrh"
#include "SysApAppUi.h"

#include <e32svr.h>      // RDebug

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::NewL()
// ----------------------------------------------------------------------------

CSysApAccessoryObserver* CSysApAccessoryObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApAccessoryObserver::NewL") ) ); 
    CSysApAccessoryObserver* self = new ( ELeave ) CSysApAccessoryObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::CSysApAccessoryObserver() 
// ----------------------------------------------------------------------------

CSysApAccessoryObserver::CSysApAccessoryObserver( CSysApAppUi& aSysApAppUi ) 
  : CActive( EPriorityStandard ),
    iSysApAppUi( aSysApAppUi )
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::~CSysApAccessoryObserver( )
// ----------------------------------------------------------------------------

CSysApAccessoryObserver::~CSysApAccessoryObserver()
    {
    TRACES( RDebug::Print( _L("CSysApAccessoryObserver::~CSysApAccessoryObserver") ) );
    Cancel();
    iAccessoryMode.CloseSubSession();
    iAccessoryServer.Close();
    }

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApAccessoryObserver::ConstructL( )
    {
    TInt err( KErrNone );
    TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): trying RAccessoryServer::Connect" ) ) );
    err = iAccessoryServer.Connect();
    if ( err == KErrNone )
        {
        TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): trying RAccessoryMode::CreateSubSession" ) ) );
        err = iAccessoryMode.CreateSubSession( iAccessoryServer );
        if ( err == KErrNone )
            {
            TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): connection to RAccessoryMode established" ) ) );
            
            err = iAccessoryMode.GetAccessoryMode( iAccMode );
                        
            if ( err != KErrNone )
                {
                TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): RAccessoryMode::AccessoryMode() - Error: %d"), err ) );
                iAccMode.iAccessoryMode = EAccModeHandPortable;
                iPreviousMode = EAccModeHandPortable;
                }
            else
                {
                TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): iAccMode.iAccessoryMode=%d" ), iAccMode.iAccessoryMode ) );
                iPreviousMode = iAccMode.iAccessoryMode;
                }                
            
            iAccessoryMode.NotifyAccessoryModeChanged( iStatus, iAccMode );
            SetActive();
            }
        else
            {
            TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): RAccessoryMode::CreateSubSession - Error: %d" ), err ) );
            }
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApAccessoryObserver::ConstructL(): RAccessoryServer::Connect - Error: %d" ), err ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::DoCancel()
// ----------------------------------------------------------------------------

void CSysApAccessoryObserver::DoCancel()
    {
    TRACES( RDebug::Print( _L("CSysApAccessoryObserver::DoCancel()" ) ) );
    iAccessoryMode.CancelNotifyAccessoryModeChanged();
    }

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::RunL()
// ----------------------------------------------------------------------------

void CSysApAccessoryObserver::RunL()
    {
    TRACES( RDebug::Print( _L("CSysApAccessoryObserver::RunL status=%d, iAccMode=%d, iPreviousMode=%d" ),
                               iStatus.Int(), iAccMode.iAccessoryMode, iPreviousMode ) );

    // Handle accessory mode change
    if ( iAccMode.iAccessoryMode == EAccModeHandPortable )
        {
        iSysApAppUi.HandleAccessoryDisconnectedL();
        }
    else if ( iAccMode.iAccessoryMode != iPreviousMode ) // for filtering audio output change notifications
        {
        iSysApAppUi.HandleAccessoryConnectedL( iAccMode.iAccessoryMode );
        }
    
    iPreviousMode = iAccMode.iAccessoryMode;
    
    iAccessoryMode.NotifyAccessoryModeChanged( iStatus, iAccMode );
    SetActive();

    TRACES( RDebug::Print( _L("CSysApAccessoryObserver::RunL: status: %d" ), iStatus.Int() ) ); 
    }

// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::RunError( TInt aError )
// ----------------------------------------------------------------------------

#ifdef _DEBUG
TInt CSysApAccessoryObserver::RunError( TInt aError )
#else
TInt CSysApAccessoryObserver::RunError( TInt /* aError */ )
#endif

    {
    TRACES( RDebug::Print( _L( "CSysApAccessoryObserver::RunError: %d started" ), aError ) );
    return KErrNone;            
    }
    
// ----------------------------------------------------------------------------
// CSysApAccessoryObserver::GetAccessoryMode() const
// ----------------------------------------------------------------------------

TAccMode CSysApAccessoryObserver::GetAccessoryMode() const
    {
    return iAccMode.iAccessoryMode;
    }

// End of File





