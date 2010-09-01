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
* Description:  CSysApCenRepFmTxObserver class definition. 
*
*/


#ifndef SYSAPCENREPFMTXOBSERVER_H
#define SYSAPCENREPFMTXOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <cenrepnotifyhandler.h>


// FORWARD DECLARATIONS
class CRepository;
class CSysApAppUi;

// CLASS DECLARATION

/**
*  CSysApCenRepFmTxObserver
*
*  Observer class for FM TX related Central Repository keys.
*
*  @lib None.
*  @since 3.2
*/

class CSysApCenRepFmTxObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
    public: // Constructors and destructor
        /**
        * Symbian two-phased constructor.
        */
        static CSysApCenRepFmTxObserver* NewL( CSysApAppUi& aSysApAppUi );

        /**
        * Destructor.
        */
        virtual ~CSysApCenRepFmTxObserver();

    private:

        /**
        * Symbian 2nd-phase constructor.
        * @param None
        * @return void
        */
        void ConstructL( );
        
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @return void
        */         
        CSysApCenRepFmTxObserver( CSysApAppUi& aSysApAppUi );
    
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApCenRepFmTxObserver();
        
    public: // From MCenRepNotifyHandlerCallback
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );
        
    public: // Other functions
        
        /**
        * Returns the current frequency
        *
        * @return TInt
        */               
        TInt Frequency() const;

    private:
        // By default, prohibit copy constructor
        CSysApCenRepFmTxObserver( const CSysApCenRepFmTxObserver& );
    
        // Prohibit assigment operator
        CSysApCenRepFmTxObserver& operator= ( const CSysApCenRepFmTxObserver& );
    
    private:
        /**
        * SysAp application class.
        */
        CSysApAppUi& iSysApAppUi;
        
        /**
        * CenRep session for FM TX keys.
        * Own.
        */
        CRepository* iSession;
        
        /**
        * Notify handler for FM TX CenRep key.
        * Own.
        */ 
        CCenRepNotifyHandler* iFmTxPowerStateHandler;
        
        /**
        * Tuned FM TX frequency.
        */
        TInt iFrequency;
    };

#endif      // SYSAPCENREPFMTXOBSERVER_H
            
// End of File
