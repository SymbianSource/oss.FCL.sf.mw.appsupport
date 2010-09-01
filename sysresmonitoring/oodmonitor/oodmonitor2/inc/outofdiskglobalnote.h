/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of COutOfDiskMonitor class.
*
*/


#ifndef __OUTOFDISKGLOBALNOTE_H__
#define __OUTOFDISKGLOBALNOTE_H__

// SYSTEM INCLUDES
#include <uiklaf/private/lafshut.h>
#include <coemain.h>
#include <AknGlobalNote.h>

// USER INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS
class COutOfDiskMonitor;
class TNoteInfo
    {
public:
    TInt iNoteId;
    TInt iStatus;
    TInt iDrive;
    };
/**
*  
*  
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS(COutOfDiskGlobalNote) : public CActive
    {
    public:
        COutOfDiskGlobalNote( COutOfDiskMonitor* aOutOfDiskMonitor, RFs& aFs  );    
        static COutOfDiskGlobalNote* NewL( COutOfDiskMonitor* aOutOfDiskMonitor, RFs& aFs  );
        ~COutOfDiskGlobalNote();        
        void DisplayL(const TDesC& aMessage);
        void ShowGlobalQueryL(TInt aStatus, TInt aDrive);
        void CancelNoteL();
        TBool NoteOnDisplay();
        TNoteInfo GetNoteInfo();
    private:
        void ConstructL();
        HBufC* FormatStringL(const TDesC& aSource, const MDesCArray& aStrings);
    private: // From CActive    
    	void DoCancel();
    	void RunL();                
    private: // Data
        COutOfDiskMonitor*  iOutOfDiskMonitor; //uses
        RFs&                iFs;
        CAknGlobalNote*     iQuery;
        RResourceFile       iOODResourceFile;
        TNoteInfo           iNoteInfo;
    };
#endif //__OUTOFDISKGLOBALNOTE_H__
