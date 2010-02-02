/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Declaration of CSysErrCmd class.
*
*/

#ifndef SYSERRCMD_H
#define SYSERRCMD_H

#include <e32base.h>
#include <ssm/ssmcustomcommand.h>

class CAknGlobalNote;

/**
 *  A custom command for displaying a note about unrecoverable
 *  system error.
 */
NONSHARABLE_CLASS( CSysErrCmd ) : public CBase, public MSsmCustomCommand
    {

public:

    /**
     * Two-phased constructor
     */
	static CSysErrCmd* NewL();

    /**
     * Destructor
     */
    virtual ~CSysErrCmd();

private: // From MSsmCustomCommand

    /**
     * @see MSsmCustomCommand
     */
    TInt Initialize( CSsmCustomCommandEnv* aCmdEnv );

    /**
     * @see MSsmCustomCommand
     */
	void Execute( const TDesC8& aParams, TRequestStatus& aRequest );

    /**
     * @see MSsmCustomCommand
     */
	void ExecuteCancel();

    /**
     * @see MSsmCustomCommand
     */
	void Close();

    /**
     * @see MSsmCustomCommand
     */
	void Release();
	
private: // New functions
    
    void DoExecuteL( TRequestStatus& aRequest );

private: // data

    // Custom command environment. Not owned. Set in Initialize.
    CSsmCustomCommandEnv* iEnv;

    /** Global note object used to show the notification on UI. Own */
    CAknGlobalNote* iNote;

    /** Global note id for cancelling the note. */
    TInt iNoteId;
    };

#endif // SYSERRCMD_H
