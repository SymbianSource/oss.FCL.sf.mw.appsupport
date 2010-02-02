/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CStartupMediatorObserver class
*
*/


#include <MediatorDomainUIDs.h>

#include "StartupMediatorObserver.h"
#include "StartupAppUi.h"
#include "startupdomainpskeys.h"

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------
// CStartupMediatorObserver::ConstructL()
// ----------------------------------------------------
void CStartupMediatorObserver::ConstructL()
    {
    TRACES("CStartupMediatorObserver::ConstructL()");

    TRACES("CStartupMediatorObserver::ConstructL(): iCommandInitiator");
    iCommandInitiator = CMediatorCommandInitiator::NewL(this);

    TRACES("CStartupMediatorObserver::ConstructL(): iEventProvider");
    iEventProvider = CMediatorEventProvider::NewL();

    TRACES("CStartupMediatorObserver::ConstructL(): Register event: EMsgWelcomeImageEvent ");
    iEventProvider->RegisterEvent( KMediatorSecondaryDisplayDomain,
                                   SecondaryDisplay::KCatStartup,
                                   SecondaryDisplay::EMsgWelcomeImageEvent,
                                   TVersion(0,0,0),
                                   ECapabilitySwEvent );

    TRACES("CStartupMediatorObserver::ConstructL(): Register event: EMsgStartupEvent ");
    iEventProvider->RegisterEvent( KMediatorSecondaryDisplayDomain,
                                   SecondaryDisplay::KCatStartup,
                                   SecondaryDisplay::EMsgStartupEvent,
                                   TVersion(0,0,0),
                                   ECapabilitySwEvent );

    TRACES("CStartupMediatorObserver::ConstructL(): End");
    }

// ----------------------------------------------------
// CStartupMediatorObserver::NewL( CStartupAppUi* aStartupAppUi )
// ----------------------------------------------------
CStartupMediatorObserver* CStartupMediatorObserver::NewL( CStartupAppUi* aStartupAppUi )
    {
    TRACES("CStartupMediatorObserver::NewL()");
    CStartupMediatorObserver* self = new (ELeave) CStartupMediatorObserver( aStartupAppUi );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    TRACES("CStartupMediatorObserver::NewL(): End");
    return self;
    }

// ----------------------------------------------------
// CStartupMediatorObserver::CStartupMediatorObserver( CStartupAppUi* aStartupAppUi )
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------
CStartupMediatorObserver::CStartupMediatorObserver( CStartupAppUi* aStartupAppUi ) :
    iStartupAppUi( aStartupAppUi ),
    iSyncData( NULL )
    {
    TRACES("CStartupMediatorObserver::CStartupMediatorObserver()");
    }

// ----------------------------------------------------
// CStartupMediatorObserver::~CStartupMediatorObserver()
// ----------------------------------------------------
CStartupMediatorObserver::~CStartupMediatorObserver()
    {
    TRACES("CStartupMediatorObserver::~CStartupMediatorObserver()");
    delete iCommandInitiator;
    delete iEventProvider;
    TRACES("CStartupMediatorObserver::~CStartupMediatorObserver(): End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void CStartupMediatorObserver::IssueCommand(TInt aCommandId, TInt aData)
    {
    TRACES("CStartupMediatorObserver::IssueCommand()");
    TRACES1("CStartupMediatorObserver::IssueCommand(): aCommandId %d", aCommandId);
    TRACES1("CStartupMediatorObserver::IssueCommand(): aData:     %d", aData);
    if (aCommandId == SecondaryDisplay::ECmdStartupSync)
        {
        iSyncData = aData;
        }

    TPckgBuf<TInt> data( aData );
    iCommandInitiator->IssueCommand( KMediatorSecondaryDisplayDomain,
                                     SecondaryDisplay::KCatStartup,
                                     aCommandId,
                                     TVersion(0,0,0),
                                     data);

    TRACES("CStartupMediatorObserver::IssueCommand(): End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void CStartupMediatorObserver::CommandResponseL( TUid /*aDomain*/,
                                                 TUid /*aCategory*/,
                                                 TInt aCommandId,
                                                 TInt /*aStatus*/,
                                                 const TDesC8& /*aData*/ )
    {
    TRACES("CStartupMediatorObserver::CommandResponseL()");
    if (aCommandId == SecondaryDisplay::ECmdStartupSync)
        {
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        if ( iSyncData == SecondaryDisplay::EStartWelcomeAnimation )
            {
            TRACES("CStartupMediatorObserver::CommandResponseL(): EStartWelcomeAnimation");
            iStartupAppUi->CoverUIWelcomeAnimationSyncOKL();
            }
        else if ( iSyncData == SecondaryDisplay::EStartOperatorAnimation )
            {
            TRACES("CStartupMediatorObserver::CommandResponseL(): EStartOperatorAnimation");
            iStartupAppUi->CoverUIOperatorAnimationSyncOKL();
            }
        else
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        if ( iSyncData == SecondaryDisplay::EStartStartupReady )
            {
            TRACES("CStartupMediatorObserver::CommandResponseL(): EStartStartupReady");
            iStartupAppUi->CoverUIStartupReadySyncOKL();
            }
        else
            {
            TRACES("CStartupMediatorObserver::CommandResponseL(): Unsupported command");
            }
        }

    TRACES("CStartupMediatorObserver::CommandResponseL(): End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
void CStartupMediatorObserver::RaiseEvent( TUid aCategory,
                                           TInt aEventId,
                                           const TDesC8& aData )
    {
    TRACES("CStartupMediatorObserver::RaiseEvent()");
    TRACES1("CStartupMediatorObserver::RaiseEvent(): domain  :%d", KMediatorSecondaryDisplayDomain);
    TRACES1("CStartupMediatorObserver::RaiseEvent(): category:%d", aCategory);
    TRACES1("CStartupMediatorObserver::RaiseEvent(): event id:%d", aEventId);
    TRACES1("CStartupMediatorObserver::RaiseEvent(): data    :%S", &aData);
    TInt err = iEventProvider->RaiseEvent( KMediatorSecondaryDisplayDomain,
                                           aCategory,
                                           aEventId,
                                           TVersion(0,0,0),
                                           aData );
    if ( err != KErrNone )
        {
        TRACES1("CStartupMediatorObserver::RaiseEvent(): Error raising event: err = d", err);
        }
    TRACES("CStartupMediatorObserver::RaiseEvent(): End");
    }

//  End of File
