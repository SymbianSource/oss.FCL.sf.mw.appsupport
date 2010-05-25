/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Accessory selection dialog.
*
*/


// INCLUDE FILES
#include "AccFwUiDialogNotifier.h"
#include "acc_debug.h"

#include <featmgr.h>
#include <secondarydisplay/SecondaryDisplayAccFwAPI.h>
#include <accfwnoteuinotifier.rsg>
#include <aknlistquerydialog.h> 
#include <aknlists.h> 
#include <bautils.h>
#include <data_caging_path_literals.hrh> 

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

const TUid KScreenOutputChannel = { 0x10282E0A };
_LIT(KAccFwUiNotifierResource, "accfwnoteuinotifier.rsc"); 


// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================
   
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::CAccFwUiDialogNotifier()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAccFwUiDialogNotifier::CAccFwUiDialogNotifier()	
	: CActive( EPriorityStandard)
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::CAccFwUiDialogNotifier() - enter" );
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::CAccFwUiDialogNotifier() - return void" );
    }
   
// Destructor
CAccFwUiDialogNotifier::~CAccFwUiDialogNotifier()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::~CAccFwUiDialogNotifier() - enter" );
	
	delete iDialog;
	delete iCoverUIITems;
    iSelections.Close();
    CActive::Cancel();

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccPolAccessoryPolicy::~CAccFwUiDialogNotifier() - return void" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAccFwUiDialogNotifier::ConstructL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::ConstructL() - enter" );
    
    iErr = KErrNone;
    
    //get pointer to CEikonEnv
	iCoeEnv = CEikonEnv::Static();    
    
    if ( !iCoeEnv )
        {
        User::Leave( KErrGeneral );        
        }
    
    iLockKeys = ETrue;
    
	//Open resource file
    TFileName fileName;
    fileName.Zero();

    TFileName drivePath;
    Dll::FileName( drivePath );

    fileName.Append( TParsePtrC( drivePath ).Drive() );
    fileName.Append( KDC_RESOURCE_FILES_DIR );
    fileName.Append( KAccFwUiNotifierResource );
    
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

	//handle to resource file
    iResourceFileOffset = iCoeEnv->AddResourceFileL( fileName );

	//add to scheduler
    CActiveScheduler::Add( this );
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::ConstructL() - return void" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAccFwUiDialogNotifier* CAccFwUiDialogNotifier::NewL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::NewL() - enter" );

    CAccFwUiDialogNotifier* self = new( ELeave ) CAccFwUiDialogNotifier;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::CAccFwUiDialogNotifier::NewL() - return" );
    return self;
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::Release()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAccFwUiDialogNotifier::Release()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Release() - enter" );
    
    if( iResourceFileOffset )
        {
        iCoeEnv->DeleteResourceFile( iResourceFileOffset );
        }

    delete this;

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Release() - return void" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::RegisterL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
CAccFwUiDialogNotifier::TNotifierInfo CAccFwUiDialogNotifier::RegisterL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RegisterL() - enter" );
    
    iInfo.iUid=KAccFwUiDialogNotifierUid;
    iInfo.iChannel=KScreenOutputChannel;
    iInfo.iPriority=ENotifierPriorityHigh;
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::CAccFwUiDialogNotifier::RegisterL() - return" );
    return iInfo;
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::Info()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
CAccFwUiDialogNotifier::TNotifierInfo CAccFwUiDialogNotifier::Info() const
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Info() - enter" );
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Info() - return void" );
    return iInfo;
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::StartL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TPtrC8 CAccFwUiDialogNotifier::StartL( const TDesC8& /*aBuffer*/ )
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::StartL() - enter" );
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::StartL() - return void" );
    return KNullDesC8();
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::StartL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAccFwUiDialogNotifier::StartL(
    const TDesC8& aBuffer, 
    TInt aReplySlot, 
    const RMessagePtr2& aMessage )
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::StartL() - enter" );
     
    // Connect to AknKeylock2 server;
    iErr = iServer.Connect();
    
    //get selection bit mask
	TUint32 selectionBitmask( 0 );
	TPckg<TUint32> selectioPckg( selectionBitmask );
	selectioPckg.Copy( aBuffer );

	//prepare selection array	
	iSelections.Close();
	
	
	// construct listbox item array
	iItemList = new ( ELeave ) CDesCArrayFlat( 10 );
	
	//if cover UI exist
	if( FeatureManager::FeatureSupported( KFeatureIdCoverDisplay ) )
        {	
		delete iCoverUIITems;
		iCoverUIITems = 0;
		iCoverUIITems = new ( ELeave ) CArrayFixFlat<TInt>( 10 );
        }
	TResourceReader reader;
	HBufC8* literals = iCoeEnv->AllocReadResourceAsDes8LC( R_SELECTION_DIALOG_LITERALS );
	reader.SetBuffer( literals );

	TInt count( reader.ReadInt16() );
	TInt coverUIItemCounter( 0 );
		
	//add items to dialog
	for ( TInt i(0); i < count; i++ )
	    {
		//read from resource
		TUint32 index( reader.ReadUint32() );
		HBufC * buf( reader.ReadHBufCL() );
		CleanupStack::PushL( buf );
		
		if ( index & selectionBitmask )
		    {
			iItemList->AppendL( *buf );
			iSelections.Append( index );
			
			//append cover UI items
			if ( iCoverUIITems )
			    {
				iCoverUIITems->AppendL( coverUIItemCounter++ );	
			    }
		    }

		CleanupStack::PopAndDestroy( buf );

		if ( ( index << 1  ) > selectionBitmask )
		    {
			//stop for-loop, selectionbitmask can't match anymore
			break;
		    }
	    }
    CleanupStack::PopAndDestroy( literals );
	iMessage = aMessage;
	iReplySlot = aReplySlot;
	iIsCancelled = EFalse;
    
    //prepare active object
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );//RunL() function will get called

    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::StartL() - return void" );
    }
    
    
// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::RunL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------

void CAccFwUiDialogNotifier::RunL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - enter" );

	if ( iIsCancelled )
	    {
		return;	
	    }
    
    if( iErr == KErrNone )
        {
        iWasLocked = iServer.IsKeyLockEnabled();
        // Change the bit on and off. SysAp will detect that
        // the lights should be switched on for the specified time.
        iServer.DisableKeyLock();
        }
    
	TInt replyValue;

	//create and prepare dialog
	iDialog =  new ( ELeave ) CAknListQueryDialog ( &replyValue ); 
    iDialog->PrepareLC( R_SELECTION_DIALOG_LIST_QUERY );

	//add items to popup list
	iDialog->SetItemTextArray( iItemList );
		
	if( FeatureManager::FeatureSupported( KFeatureIdCoverDisplay ) )
        {
		API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Publish cover UI ");
    	iDialog->PublishDialogL( ECmdShowAccessorySelectionDialog,
        					  	 KCatAccFw,
        					  	 iCoverUIITems );
		//iDialog take ownership
		iCoverUIITems = 0;        					
        }	
	
	//show dialog...
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Show dialog -> RunLD()" );
	TBool result = iDialog->RunLD();
	iDialog = 0;

    //create package
    TInt returnValue( 0 );
    TPckg<TInt> valuePck( returnValue );

    if ( !iIsCancelled )
        {
	    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Check dialog result" );

    	if ( result )
        	{
			//write selected item to reply slot
			if ( returnValue >= 0 && iSelections.Count() )
	    		{
				returnValue = iSelections[ replyValue ];
			    API_TRACE_1( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Set selection: %d", returnValue );
		    	}
		    // Don't lock keys if something was selected
		    iLockKeys = EFalse;
    	    }
    	else
    	    {
		    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Cancel pressed" );
    		returnValue = 0;
    		iLockKeys = ETrue;
        	}

    	//complete client requests
	    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Write response value to message" );
		iMessage.Write( iReplySlot, valuePck, 0 );	
	    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Complete message" );
 		iMessage.Complete( KErrNone );	
        }
    else
        {
        // Message is being completed with KErrCancel in Cancel-method
        }

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - Release resources" );
	delete iCoverUIITems;
	iCoverUIITems = 0;
    iSelections.Close();
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunL() - return void" );    
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::RunError()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CAccFwUiDialogNotifier::RunError(TInt aError)
    {
    API_TRACE_1( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunError() - %d", aError );
    if( !iMessage.IsNull() )
        {
        iMessage.Complete( aError );
        }
    aError = KErrNone;
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::RunError() - return" );
    return aError;
    }
    
// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::DoCancel()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAccFwUiDialogNotifier::DoCancel()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::DoCancel() - enter" );
    if( !iMessage.IsNull() )
        {
        iMessage.Complete( KErrCancel );
        }
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::DoCancel() - return" );
    }
    

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::Cancel()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAccFwUiDialogNotifier::Cancel()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Cancel() - enter" );

    iIsCancelled = ETrue;
    
    if( iLockKeys && iWasLocked && ( iErr == KErrNone ) )
        {
        iServer.EnableKeyLock();
        }
    
    iLockKeys = ETrue;
    
    if ( iDialog )
        {
	   //cancel active object, delete dialog and release resources
        if ( IsActive() )
            {
            CActive::Cancel();    
            }
	    delete iDialog;
	    iDialog = 0;
		delete iCoverUIITems;
		iCoverUIITems = 0;
	    iSelections.Close();    	

        TInt returnValue( 0 );
        TPckg<TInt> valuePck( returnValue );
        API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Cancel() - Write response value to message" );
        iMessage.Write( iReplySlot, valuePck, 0 );
        API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Cancel() - Complete message with Cancel" );
        iMessage.Complete( KErrCancel );
        }
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::Cancel() - return void" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiDialogNotifier::UpdateL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TPtrC8 CAccFwUiDialogNotifier::UpdateL(
    const TDesC8& /*aBuffer*/ )
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::UpdateL() - enter" );
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiDialogNotifier::UpdateL() - return void" );
    return KNullDesC8();
    }

//  End of File
