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
* Description:  CSysApCenRepSilentModeObserver implementation.
 *
*/


// INCLUDE FILES
#include "SysApCenRepSilentModeObserver.h"
#include "SysApAppUi.h"
#include <hbindicatorsymbian.h>

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

CSysApCenRepSilentModeObserver* CSysApCenRepSilentModeObserver::NewL( )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::NewL" ) ) );
    CSysApCenRepSilentModeObserver* self = new ( ELeave ) CSysApCenRepSilentModeObserver(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

CSysApCenRepSilentModeObserver::CSysApCenRepSilentModeObserver(  )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::CSysApCenRepSilentModeObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

CSysApCenRepSilentModeObserver::~CSysApCenRepSilentModeObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepSilentModeObserver") ) );
    if ( iSilentModeHandler )
        {
        iSilentModeHandler->StopListening();
        }
    delete iSilentModeHandler;
    delete iSession;
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void CSysApCenRepSilentModeObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::ConstructL: trying CRepository::NewL( KCRUidBluetoothPowerState )") ) );
    iSession = CRepository::NewL( KCRUidProfileEngine );
    
    iSilentModeHandler = 
        CCenRepNotifyHandler::NewL( *this, 
                                    *iSession, 
                                    CCenRepNotifyHandler::EIntKey, 
                                    KProEngSilenceMode );
    iSilentModeHandler->StartListeningL();
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void CSysApCenRepSilentModeObserver::HandleNotifyInt( TUint32 aId, TInt aNewValue  )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::HandleNotifyInt(), aId=0x%x, aNewValue=%d" ), aId, aNewValue ) );
     
    SilenceIndicatorL(aNewValue);
        
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void CSysApCenRepSilentModeObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::HandleNotifyError()" ) ) );
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void CSysApCenRepSilentModeObserver::SilenceIndicatorL(TInt aValue)
    {
    TRACES( RDebug::Print( _L(" CSysApCenRepSilentModeObserver::SilenceIndicatorL" )));
    CHbIndicatorSymbian* indicator = CHbIndicatorSymbian::NewL();
    CleanupStack::PushL(indicator);
    _LIT(KSilentmodeindicator,"com.nokia.hb.indicator.controlpanel.cpsilenceindicatorplugin/1.0");
    
    
    if (aValue) 
       {
       TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::HandleNotifyInt() activate silence indicator" )));
       // activate the indicator plugin to display the icon in status bar        
       indicator->Activate(KSilentmodeindicator); 
       } 
    else
       {
       TRACES( RDebug::Print( _L("CSysApCenRepSilentModeObserver::HandleNotifyInt() Deactivate silence indicator" )));
       // deactivate the indicator plugin to remove the icon in the status bar  
       indicator->Deactivate(KSilentmodeindicator);
       }
    CleanupStack::PopAndDestroy(indicator); // indicator
    }
    

// End of File

