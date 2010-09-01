/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApDocument implementation.
*
*/


// INCLUDE FILES
#include <apgwgnam.h>
#include "SysApDocument.h"
#include "SysApAppUi.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApDocument::~CSysApDocument()
// ----------------------------------------------------------------------------

CSysApDocument::~CSysApDocument()
    {
    }

// ----------------------------------------------------------------------------
// CSysApDocument::CreateAppUiL()
// constructs CSysApUi
// ----------------------------------------------------------------------------
CEikAppUi* CSysApDocument::CreateAppUiL()
    {
    return new(ELeave) CSysApAppUi;
    }

// ----------------------------------------------------------------------------
// CSysApDocument::UpdateTaskNameL( CApaWindowGroupName* aWgName )
// ----------------------------------------------------------------------------
void CSysApDocument::UpdateTaskNameL( CApaWindowGroupName* aWgName )
	{
	CAknDocument::UpdateTaskNameL( aWgName );
	aWgName->SetHidden( ETrue );
	}

// End of File
