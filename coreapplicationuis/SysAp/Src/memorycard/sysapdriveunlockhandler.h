/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Drive unlock handler implementation
 *
*/


#ifndef SYSAPDRIVEUNLOCKHANDLER_H
#define SYSAPDRIVEUNLOCKHANDLER_H

// INCLUDES
#include <e32std.h>
#include <aknmemorycarddialog.h>

// FORWARD DECLARATIONS
class CSysApDriveList;
class CSysApAppUi;

// CLASS DECLARATION
class CSysApDriveUnlockHandler : public CBase,
                                 public MAknMemoryCardDialogObserver
{
public:
    /**
     * Two-phased constructor.
     */
    static CSysApDriveUnlockHandler* NewL(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported );

    /**
     * Destructor
     */
    ~CSysApDriveUnlockHandler();

public: // New methods

    /**
     * Starts unlock notifier if not already ongoing
     */
    void StartUnlock();

    /**
     * Stops unlock notifier for given drive if currently shown.
     * @param aDrive Drive to stop unlock notifier if ongoing
     */
    void StopUnlock( TInt aDrive );

private:
    /**
    * Constructor.
    */
    CSysApDriveUnlockHandler(
        CSysApDriveList& aSysApDriveList,
        CSysApAppUi& aSysApAppUi,
        const TBool aMemoryCardLockSupported );

    /**
    * Handles query show from CAsyncCallBack.
    */
    static TInt QueryShowCB( TAny* aPtr );

    /**
    * Shows unlock query.
    */
    void ShowUnlockQueryL();

    /**
    * Checks is unlock query ongoing.
    */
    TBool IsQueryOngoing() const;

    /**
    * Start query if drives left to be unlocked.
    */
    void DoStartQueryIfNeededL();

    /**
    * Start query using CAsyncCallBack if drives left to be unlocked.
    */
    void DoStartQueryAsyncIfNeeded();

    /**
    * Stops unlock.
    */
    void DoStopUnlock( TInt aError );

private: // From MAknMemoryCardDialogObserver
    void UnlockComplete( TInt aResult );

private: // Data
    // Reference to the drive list
    CSysApDriveList& iSysApDriveList;

    // Reference to the app ui
    CSysApAppUi& iSysApAppUi;

    // Indicates if memory card locking is supported
    TBool iMemoryCardLockSupported;

    // Pointer to unlock query. Own,
    CAknMemoryCardDialog* iMemoryCardDialog;

    // Current drive to be unlocked
    TInt iDriveToUnlock;

    // Pointer to the callback. Own.
    CAsyncCallBack* iQueryShowCB;

    // Indicates if the query result can be ignored i.e. query was canceled
    TBool iIgnoreQueryResult;

};

#endif // SYSAPDRIVEUNLOCKHANDLER_H

// End of File
