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
* Description:  CSysApCenRepSilentModeObserver class definition. 
*
*/


#ifndef SYSAPCENREPSILENTMODEOBSERVER_H
#define SYSAPCENREPSILENTMODEOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include "SysAp.hrh"
#include <cenrepnotifyhandler.h>
#include <ProfileEngineDomainCRKeys.h>

// CLASS DECLARATION


class CSysApCenRepSilentModeObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
    public: // Constructors and destructor
        /**
        * Default constructor
        */
        static CSysApCenRepSilentModeObserver* NewL(  );

        /**
        * Destructor.
        */
        ~CSysApCenRepSilentModeObserver();
        
        // From MCenRepNotifyHandlerCallback
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );

    private:

        /**
        * @param None
        * @return void
        */
        void ConstructL( );
        
        
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApCenRepSilentModeObserver();
       
        
        //To activate and Deactivate the indicator
        void SilenceIndicatorL(TInt avalue);
        
   

  
    private:
               
        CRepository*          iSession;
        //To listen to the silentmodekey
        CCenRepNotifyHandler* iSilentModeHandler;
    };

#endif      // SYSAPCENREPSILENTMODEOBSERVER_H
            
// End of File
