/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <s32file.h>
#include <hlplch.h>

#include <t_oomdummyapp_0xE6CFBA00.rsg>
#include <oommonitorsession.h>

//#include "t_oomdummyapp_0xE6CFBA00.hlp.hrh"
#include "t_oomdummyapp.hrh"
#include "t_oomdummyapp.pan"
#include "t_oomdummyappApplication.h"
#include "t_oomdummyappAppUi.h"
#include "t_oomdummyappAppView.h"
#include "CMsgHandler.h"
#include "t_oomdummyappmsgs.h"

_LIT( KFileName, "C:\\private\\E6CFBA00\\t_oomdummyapp.txt" );
_LIT( KText, "OOM dummy app");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// Ct_oomdummyappAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	// Create view object
	iAppView = Ct_oomdummyappAppView::NewL(ClientRect());

	// Create a file to write the text to
	TInt err = CCoeEnv::Static()->FsSession().MkDirAll(KFileName);
	if ((KErrNone != err) && (KErrAlreadyExists != err))
		{
		return;
		}

	RFile file;
	err = file.Replace(CCoeEnv::Static()->FsSession(), KFileName, EFileWrite);
	CleanupClosePushL(file);
	if (KErrNone != err)
		{
		CleanupStack::PopAndDestroy(1); // file
		return;
		}

	RFileWriteStream outputFileStream(file);
	CleanupClosePushL(outputFileStream);
	outputFileStream << KText;

	CleanupStack::PopAndDestroy(2); // outputFileStream, file

	TRAP_IGNORE(iMsgHandler = CMsgHandler::NewL(*this)); //if not launched by test harness, the message queue won't exist
	}
// -----------------------------------------------------------------------------
// Ct_oomdummyappAppUi::Ct_oomdummyappAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappAppUi::Ct_oomdummyappAppUi()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppUi::~Ct_oomdummyappAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
Ct_oomdummyappAppUi::~Ct_oomdummyappAppUi()
	{
	if (iAppView)
		{
		delete iAppView;
		iAppView = NULL;
		}
	delete iMsgHandler;
	}

// -----------------------------------------------------------------------------
// Ct_oomdummyappAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
			break;

		case ECommand1:
			{

			// Load a string from the resource file and display it
			HBufC* textResource = StringLoader::LoadLC(R_COMMAND1_TEXT);
			CAknInformationNote* informationNote;

			informationNote = new (ELeave) CAknInformationNote;

			// Show the information Note with
			// textResource loaded with StringLoader.
			informationNote->ExecuteLD(*textResource);

			// Pop HBuf from CleanUpStack and Destroy it.
			CleanupStack::PopAndDestroy(textResource);
			}
			break;
		case ECommand2:
			{
			RFile rFile;

			//Open file where the stream text is
			User::LeaveIfError(rFile.Open(CCoeEnv::Static()->FsSession(),
					KFileName, EFileStreamText));//EFileShareReadersOnly));// EFileStreamText));
			CleanupClosePushL(rFile);

			// copy stream from file to RFileStream object
			RFileReadStream inputFileStream(rFile);
			CleanupClosePushL(inputFileStream);

			// HBufC descriptor is created from the RFileStream object.
			HBufC* fileData = HBufC::NewLC(inputFileStream, 32);

			CAknInformationNote* informationNote;

			informationNote = new (ELeave) CAknInformationNote;
			// Show the information Note
			informationNote->ExecuteLD(*fileData);

			// Pop loaded resources from the cleanup stack
			CleanupStack::PopAndDestroy(3); // filedata, inputFileStream, rFile
			}
			break;
		case EAbout:
			{

			CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
			dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(); //title
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(); //msg
			dlg->RunLD();
			}
			break;
		default:
			Panic(Et_oomdummyappUi);
			break;
		}
	}

void Ct_oomdummyappAppUi::HandleHarnessCommandL(TInt aCommand)
	{
	switch(aCommand)
		{
		case 0:
			Exit();
			break;
		case 1:
			ActivateTopViewL();
			break;
		case KOomDummyAppSetBusy:
			SetPriorityBusy();
			break;
		case KOomDummyAppSetNormalPriority:
			SetPriorityNormal();
			break;
		case KOomDummyAppSetHighPriority:
			SetPriorityHigh();
			break;
		}
	}

void Ct_oomdummyappAppUi::SetPriorityBusy()
	{
	ROomMonitorSession oomSession;
	oomSession.Connect();
	oomSession.SetOomPriority(ROomMonitorSession::EOomPriorityBusy);
	oomSession.Close();
	}

void Ct_oomdummyappAppUi::SetPriorityNormal()
	{
	ROomMonitorSession oomSession;
	oomSession.Connect();
	oomSession.SetOomPriority(ROomMonitorSession::EOomPriorityNormal);
	oomSession.Close();
	}

void Ct_oomdummyappAppUi::SetPriorityHigh()
	{
	ROomMonitorSession oomSession;
	oomSession.Connect();
	oomSession.SetOomPriority(ROomMonitorSession::EOomPriorityHigh);
	oomSession.Close();
	}


// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void Ct_oomdummyappAppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect(ClientRect());
	}


// End of File
