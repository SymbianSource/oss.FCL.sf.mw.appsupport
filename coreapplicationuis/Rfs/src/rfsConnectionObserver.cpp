/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*   Implements CRfsPdpObserver class.
*
*
*/

// SYSTEM INCLUDES
#include <rfs.rsg>                      
#include <PSVariables.h>
#include <AknWaitDialog.h>
#include <featmgr.h>
#include <centralrepository.h>
#include <AknWaitDialog.h>
#include <pdpcontextmanagerpskeys.h>
#include <pdpcontextmanagerinternalcrkeys.h>

// P&S KEYS FROM SIP & PDP CONNECTION
#include <e32property.h>
#include <pdpcontextmanagerpskeys.h>
#include <sipsystemstatemonitorpskeys.h>

// USER INCLUDES
#include "rfsConnectionObserver.h"
#include "RfsTraces.h"

// CONSTANTS
const TInt KRfsAlwaysOnDisabled     = 0;
const TInt KRfsAlwaysOnEnabled      = 1;


// ---------------------------------------------------------------------------
// two phased constructor
// ---------------------------------------------------------------------------
CRfsConnectionObserver* CRfsConnectionObserver::NewLC()
    {
    TRACES("CRfsConnectionObserver::NewLC()");
    
    CRfsConnectionObserver* self = new( ELeave ) CRfsConnectionObserver;
    CleanupStack::PushL( self );
    self->ConstructL();
    
    TRACES("CRfsConnectionObserver::NewLC(): End");
    return self;
    }

// ---------------------------------------------------------------------------
// second phased constructor
// ---------------------------------------------------------------------------
void CRfsConnectionObserver::ConstructL()
    {
    TRACES("CRfsConnectionObserver::ConstructL()");
    
    iIsPDPFeatureEnabled = IsAlwaysOnFeatureEnabledL();
    
    // We first check whether the required keys are defined or not 
    // i.e. SIP is there or not and PDP feature is there or not.
    // Following cases can arise:
    // 1. SIP present and PDP not defined -- Close the SIP connection (&) proceed with RFS
    // 2. SIP not defined and PDP present -- Close the PDP connection (&) proceed with RFS
    // 3. SIP and PDP not defined         -- Proceed to perform the RFS. Here no need to close any connection.
    // 4. Both defined                    -- Close SIP and PDP connection (&) proceed with RFS  
    //
    // OutCome: also record which is present and which is not present. This information will be needed
    // to set the future displaying of the dialog and to know whomo to send notifications and to whom not.
    //
    // If none of the active connections are present then no need to display the dialog. 
    // 
    // Here we need to inform the outside world (CRfsHandler) that we don't need to call rest of the methods
    // related to active connections closing operations if the third case is present.
    // The methods are:
    //      1. CloseAlwaysOnConnectionL()
    //      2. ReOpenConnection()
    
    TInt val(KErrNone);
    User::LeaveIfError( iSIPProperty.Attach(KPSSipRfsUid, KSipRfsState));
    TInt err = iSIPProperty.Get(KPSSipRfsUid, KSipRfsState, val);
    if (KErrNotFound == err)
        {
        // Record that SIP connection is not present
        iIsSIPConnectionsPresent = EFalse;
        }
    
    // No need to have a similar check for the PDP as the following boolean is sufficient
    if(iIsPDPFeatureEnabled)
        {
        User::LeaveIfError( iPDPProperty.Attach(KPDPContextManager2,KPDPContextManagerFactorySettingsReset ) );
        }
    
    // Following is the condition used to record that the dialog needs to be displayed
    // and the closing of the connections is possible
    if (iIsSIPConnectionsPresent || iIsPDPFeatureEnabled)
        {
        iIsDialogNeedToBeDisplayed = ETrue;
        iIsClosingConnectionsApplicable = ETrue;
        }
    
    // After finding out which type of connections need RFS notifications, we will 
    // proceed further to create a common dialog. This will be a wait for dialog which will be 
    // of synchrnous type and can only be closed inside the RunL() of this active class.
    // Provisions needed to be added to know whether this dialog needs resetting the pointer,
    // this will be the case when dialog is actually started using 'ExecuteLD()', 
    // Otherwise we need to delete this dialog and reset the pointer to NULL.
    if (iIsDialogNeedToBeDisplayed)
        {
        iWaitDialog = new( ELeave ) CAknWaitDialog(reinterpret_cast<CEikDialog**>( &iWaitDialog ) );
        }
   
    // Now we proceed further to setting of the next state i,e, enter the state machine of this active object
    // which will start from closing of the active connections and proceed to close the other connections
    // serially in a state wise manner.
    // N.B the states are put in order and should be maintained. High Priority
    //
    // As soon as 'iStatus' changes we will go to RunL() to close the SIP connection
    // and hence forth take up the activity of closing PDP connection.
    if (iIsSIPConnectionsPresent)
        {
        iState =  ESipConnectionClose;
        }
    else if (iIsPDPFeatureEnabled)
        {
        iState =  EPdpConnectionClose;
        }
    
    // After setting the state machine i.e. proper entry point into the RunL()
    // we will give an asynchronous listening and publishing request to the keys.  
    if (iIsSIPConnectionsPresent || iIsPDPFeatureEnabled)
        {
        Subscribe();
        }
    
    TRACES("CRfsConnectionObserver::ConstructL(): End");
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
CRfsConnectionObserver::CRfsConnectionObserver():CActive( EPriorityStandard )
    {
    TRACES("CRfsConnectionObserver::CRfsConnectionObserver()");

    iIsSIPConnectionsPresent = ETrue;
    iIsPDPFeatureEnabled = ETrue;
    iIsWaitForDialogExecuted = EFalse;
    iIsDialogNeedToBeDisplayed = EFalse;
    iIsSipInformedForClosingAllConnection = EFalse;
    iIsPDPInformedforClosingAllConnection = EFalse;
    iIsClosingConnectionsApplicable = EFalse;
    
    CActiveScheduler::Add( this );
    
    TRACES("CRfsConnectionObserver::CRfsConnectionObserver(): End");
    }
             
// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
CRfsConnectionObserver::~CRfsConnectionObserver()
    {
    TRACES("CRfsConnectionObserver::~CRfsConnectionObserver()");
    Cancel();
    TRACES("CRfsConnectionObserver::~CRfsConnectionObserver(): End");
    }

// ---------------------------------------------------------------------------
// CRfsConnectionObserver::CloseAlwaysOnConnectionL
// ---------------------------------------------------------------------------
TBool CRfsConnectionObserver::CloseAlwaysOnConnectionL()
    {
    TRACES("CRfsConnectionObserver::CloseAlwaysOnConnectionL()");
    
    if (iIsClosingConnectionsApplicable == EFalse)
        {
        // pretend that we have dsplayed the dialog and closed all the active connections
        return ETrue;
        }

    // only perform the following operation if the 'iIsClosingConnectionsApplicable' is ETrue 
    iAllConnectionClosed = EFalse;
      
    // Send P&S notification to SIP that RFS has started
    TInt err(KErrNone);
    if (iIsSIPConnectionsPresent && iState == ESipConnectionClose)
        {
        err = iSIPProperty.Set(KPSSipRfsUid, KSipRfsState, ESipRfsStarted );
        iIsSipInformedForClosingAllConnection = ETrue;
        }
    else if (iIsPDPFeatureEnabled && iState == EPdpConnectionClose)
        {
        err = iPDPProperty.Set(KPDPContextManager2,KPDPContextManagerFactorySettingsReset,EPDPContextManagerFactorySettingsResetStart );
        iIsPDPInformedforClosingAllConnection = ETrue;
        }
    
    
    // Leave from here is there is any error setting the intial handshake information
    // This means that RFS has failed as there was some problem setting the P&S keys
    User::LeaveIfError(err);
    if(err != KErrNone)
        {
        TRACES1("CRfsConnectionObserver::CloseAlwaysOnConnectionL(): Err = %d", err);
        // This means that the RFS has failed
        return EFalse; 
        }
    
    // we set the flag to indicate ExecuteLD is called and the dialog needs to be
    // dismissed from within the RunL()
    iIsWaitForDialogExecuted = ETrue;
    // Start displaying the dialog which will then be closed form the RunL()
    // Here the code execution blocks and we will proceed further only when 
    // this dialog is canceled
    err = iWaitDialog->ExecuteLD( R_CLOSING_CONNECTIONS );
    
    
    // following is the case when the user presses the Cancel button from the wait 
    // for dialog and in that case we need to resend the notificaiton of RFS failed
    // to whom all we have told previous of this to close the RFS connection
    if (err == EEikBidCancel)
        {
        if (iIsSipInformedForClosingAllConnection)
            {
            err = iSIPProperty.Set(KPSSipRfsUid, KSipRfsState, ESipRfsFailed );
            }
        if (iIsPDPInformedforClosingAllConnection)
            {
            iPDPProperty.Set(KPDPContextManager2,KPDPContextManagerFactorySettingsReset,EPDPContextManagerFactorySettingsResetStop);
            }
        }
    return iAllConnectionClosed;
    }

void CRfsConnectionObserver::ReportRfsCompletionToSip()
    {
    TRACES("CRfsConnectionObserver::ReportRfsCompletionToSip()");
    
    // Only perform the following operation if the 'iIsClosingConnectionsApplicable' 
    if (!iIsClosingConnectionsApplicable)
        {
        // Pretend that we have re-opened all the closed connections and return doinng nothing
        return;
        }
    if (iIsSipInformedForClosingAllConnection)
        {
        iSIPProperty.Set(KPSSipRfsUid, KSipRfsState, ESipRfsCompleted );
        }
    
    TRACES("CRfsConnectionObserver::ReportRfsCompletionToSip(): End");
    }

void CRfsConnectionObserver::ReportRfsFailureToSip()
    {
    TRACES("CRfsConnectionObserver::ReportRfsFailureToSip()");
    
    // Only perform the following operation if the 'iIsClosingConnectionsApplicable' 
    if (!iIsClosingConnectionsApplicable)
        {
        // Pretend that we have re-opened all the closed connections and return doinng nothing
        return;
        }
    if (iIsSipInformedForClosingAllConnection)
        {
        iSIPProperty.Set(KPSSipRfsUid, KSipRfsState, ESipRfsFailed );
        }
    
    TRACES("CRfsConnectionObserver::ReportRfsFailureToSip(): End");
    }

void CRfsConnectionObserver::ReOpenPDPConnection()
    {
    TRACES("CRfsConnectionObserver::ReOpenPDPConnection()");
    
    // Only perform the following operation if the 'iIsClosingConnectionsApplicable' 
    if (!iIsClosingConnectionsApplicable)
        {
        // Pretend that we have re-opened all the closed connections and return doinng nothing
        return;
        }
    
    if (iIsPDPInformedforClosingAllConnection )
         {
         iPDPProperty.Set(KPDPContextManager2,KPDPContextManagerFactorySettingsReset,EPDPContextManagerFactorySettingsResetStop );
         }
    
    TRACES("CRfsConnectionObserver::ReOpenPDPConnection(): End");
    }

// ---------------------------------------------------------------------------
// CRfsConnectionObserver::RunL
// ---------------------------------------------------------------------------
void CRfsConnectionObserver::RunL()
    {
    // First call this is to enroll for the next handshake information to be recieved either from SIP or PDP
   Subscribe(); 
    
    switch(iState)
        {
        case ESipConnectionClose:
            {
            // Send P&S message to SIP 
            TInt val;
            TInt err = iSIPProperty.Get(KPSSipRfsUid, KSipRfsState, val);
            if ( err == KErrNone && val == ESipRfsEventProcessingCompleted)
                {
                if (iIsWaitForDialogExecuted && !iIsPDPFeatureEnabled)
                    {
                    // set the information that we have closed all the active connections
                    // here itself because the PDP connection/feature doen't exist
                    iAllConnectionClosed = ETrue;
                    DismissWaitDialog();
                    }
                
                if (iIsPDPFeatureEnabled)
                    {
                    iPDPProperty.Set(KPDPContextManager2,KPDPContextManagerFactorySettingsReset,EPDPContextManagerFactorySettingsResetStart );
                    
                    // change the state to the next from within this as we are done with closing the SIP connection
                    iState = EPdpConnectionClose;
                    //
                    // Under following cases the 'iIsPDPInformedforClosingAllConnection' will be set
                    //
                    // CASE 1:
                    //          When from within CloseAlwaysOnConnectionL we inform the PDP to 
                    //          close its connection, that time this will be set.
                    // CASE 2: 
                    //          The other case covers when the SIP exists, then this flag will be set 
                    //          when we come to RunL straight to this  switch-case after just previous to
                    //          switch-case being of closing All the SIP active connections.
                    //
                    // RATIONALE: (behind the use of following flag):
                    //            Setting of this flag will actually help us in the case when
                    //            the user cancels the PDP closing connections
                    iIsPDPInformedforClosingAllConnection = ETrue;
                    }
                TRACES1("CRfsConnectionObserver::RunL(): iPDPProperty.Set returned Err = %d", err);
                }
            break;
            }
        case EPdpConnectionClose:
            {
            // When we reach here we should assume that:
            //      1. the WAIT FOR dialog is being tothe user of the device 
            //      2. If SIP existed then it has successfully closed all its active connections
            //               
            // What needs to be done at this point of time is to :
            //      1. Check of what message has been published to the 'KPDPContextManagerFactorySettingsReset' key
            //      2. If the PDP has replied with all its active connections closed then we should close the dialog 
            //         and set the information that we have closed all the active connections including SIP if it existed
            //         at the time when the user requested for an RFS 
             
            TInt val;
            TInt err = iPDPProperty.Get(KPDPContextManager2,KPDPContextManagerFactorySettingsReset, val);
            if (err == KErrNone && val == EPDPContextManagerFactorySettingsResetStartReply && iAllConnectionClosed == EFalse )
                {
                // The response from PDP of closing all its connections have been recieved 
                // Now we may proceed to dsmiss the dialog and also set the state to the True for 
                // all active connections closed
                iAllConnectionClosed = ETrue;
                DismissWaitDialog();
                }
            }
        } // end switch-case block
    }


// ---------------------------------------------------------------------------
// CRfsConnectionObserver::IsAlwaysOnFeatureEnabledL
// ---------------------------------------------------------------------------
TInt CRfsConnectionObserver::IsAlwaysOnFeatureEnabledL()
    {
    TInt alwaysOnEnabled = KRfsAlwaysOnDisabled;
    if( FeatureManager::FeatureSupported( KFeatureIdAlwaysOnlinePDPContext2 ) )
        {
        // Check if always on feature enabled
        CRepository* cenRep = CRepository::NewLC( KCRUidPDPContextManager );
        TInt err = cenRep->Get( KPDPContextAlwaysOnEnabled, alwaysOnEnabled );
        if( err != KErrNone )
            {
            alwaysOnEnabled = KRfsAlwaysOnEnabled;
            }
        CleanupStack::PopAndDestroy(cenRep); 
        }
    return alwaysOnEnabled;
    }

// ---------------------------------------------------------------------------
// CRfsConnectionObserver::DoCancel
// ---------------------------------------------------------------------------
void CRfsConnectionObserver::DoCancel()
    {
    TRACES("CRfsConnectionObserver::DoCancel()");
    
    if(iIsPDPFeatureEnabled)
        {
        iPDPProperty.Cancel();
        }
    if(iIsSIPConnectionsPresent)
        {
        iSIPProperty.Cancel();
        }

    DismissWaitDialog();
    
    TRACES("CRfsConnectionObserver::DoCancel(): End");
    }

// ---------------------------------------------------------------------------
// CRfsConnectionObserver::Subscribe
// ---------------------------------------------------------------------------
void CRfsConnectionObserver::Subscribe()
    {
    switch(iState)
        {
        case ESipConnectionClose: 
            {
            // SIP connection first [Always]
            iSIPProperty.Subscribe(iStatus);
            break;
            }
        case EPdpConnectionClose:
            {
            iPDPProperty.Subscribe(iStatus);
            break;
            }
        }
    SetActive();
    }

// ---------------------------------------------------------------------------
// CRfsConnectionObserver::DismissWaitDialog
// ---------------------------------------------------------------------------
void CRfsConnectionObserver::DismissWaitDialog()
    {
    TRACES("CRfsConnectionObserver::DismissWaitDialog()");
    
    if ( iWaitDialog && iIsWaitForDialogExecuted)
        {
        TRAP_IGNORE( iWaitDialog->ProcessFinishedL() );
        }
    
    // Sanity Check:
    // It can be a case when dialog was need to be displayed but was not due to some error
    // this means that the 'iWaitDialog' was constructed but never used and destroyed
    // i.e. the ExecuteLD() was never called
    else if(iIsDialogNeedToBeDisplayed && !iIsWaitForDialogExecuted)
        {
        delete iWaitDialog;
        }
    
    // Reset the pointer to NULL
    iWaitDialog = NULL;
    
    TRACES("CRfsConnectionObserver::DismissWaitDialog(): End");
    }

