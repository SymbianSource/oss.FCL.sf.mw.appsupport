/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_OOMMONITOR_H__
#define __MT_OOMMONITOR_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <oommonitorsession.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( MT_OomMonitor )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_OomMonitor* NewL();
        static MT_OomMonitor* NewLC();
        /**
         * Destructor
         */
        ~MT_OomMonitor();

    private:    // Constructors and destructors

        MT_OomMonitor();
        void ConstructL();

    private:    // New methods

     	 void EmptySetupL();
    
    	 void EmptyTeardown();
    
         void SetupL();
        
         void Teardown();
        
         void T_ROomMonitorSession_ConnectL();
        
         void T_ROomMonitorSession_RequestFreeMemoryL();
        
         void T_ROomMonitorSession_RequestFreeMemory2L();
        
         void T_ROomMonitorSession_CancelRequestFreeMemoryL();
        
         void T_ROomMonitorSession_ThisAppIsNotExitingL();
        

    private:    // Data
		
        ROomMonitorSession iROomMonitorSession;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __MT_OOMMONITOR_H__

// End of file
