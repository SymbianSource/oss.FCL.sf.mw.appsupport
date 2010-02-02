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
* Description:
*
*/


#ifndef __MT_COOMMONITORPLUGIN_H__
#define __MT_COOMMONITORPLUGIN_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <oommonitorplugin.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
NONSHARABLE_CLASS( MT_COomMonitorPlugin )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_COomMonitorPlugin* NewL();
        static MT_COomMonitorPlugin* NewLC();
        /**
         * Destructor
         */
        ~MT_COomMonitorPlugin();

    private:    // Constructors and destructors

        MT_COomMonitorPlugin();
        void ConstructL();

    private:    // New methods


         void SetupL();
        
         void Teardown();
         
         void BasicSetupL();
        
         void BasicTeardown();         
        
         void T_CAppOomMonitorPlugin_NewLL();
        
         void T_COomMonitorPlugin_FreeRamL();
        
         void T_COomMonitorPlugin_MemoryGoodL();
        
         void T_COomMonitorPlugin_FsSessionL();
        
         void T_COomMonitorPlugin_WsSessionL();
         
         void T_COomMonitorPlugin_ExtensionInterfaceL();
        

    private:    // Data
		
        CAppOomMonitorPlugin* iCOomMonitorPlugin;
        CMemoryMonitor* iMonitor;
        
        EUNIT_DECLARE_TEST_TABLE; 

    };
   
    
NONSHARABLE_CLASS( CTestOomMonitorPlugin ) : public COomMonitorPlugin
	{
public:
	CTestOomMonitorPlugin()
		{
		}
		
	~CTestOomMonitorPlugin()
		{
		}
		
	void ConstructL()
		{
		COomMonitorPlugin::ConstructL();
		}
		
public:
	void FreeRam()
		{
		}
		
	void MemoryGood()
		{
		}

	void ExtensionInterface(TUid aInterfaceId, TAny*& aImplementaion)
		{
		COomMonitorPlugin::ExtensionInterface(aInterfaceId, aImplementaion);
		}
	};
    

#endif      //  __MT_COOMMONITORPLUGIN_H__

// End of file
