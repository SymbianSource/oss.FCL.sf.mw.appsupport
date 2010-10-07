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
* Description: STIF testclass declaration
*
*/

#ifndef TSANIMMMFPLUGIN_H
#define TSANIMMMFPLUGIN_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

// Praveen
#include <e32def.h>
#include <f32file.h>

class CSAnimTonePlugin;
class CTestWaiter;
// Praveen
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KTSanimmmfpluginLogPath, "\\logs\\testframework\\TSanimmmfplugin\\" ); 
// Log file
_LIT( KTSanimmmfpluginLogFile, "TSanimmmfplugin.txt" ); 
_LIT( KTSanimmmfpluginLogFileWithTitle, "TSanimmmfplugin_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CTSanimmmfplugin;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CTSanimmmfplugin test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTSanimmmfplugin) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTSanimmmfplugin* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTSanimmmfplugin();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CTSanimmmfplugin( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

		TInt SetupEmptyL();
		TInt SetupObjL();
		TInt SetupFileL( const TDesC& aFileName );
		TInt Setup8bitMonoToneL();
		TInt Setup16bitMonoToneL();
		TInt SetupGsm6_10ToneL();
		TInt SetupFinishedL();
		TInt Teardown();

		TInt TestConstructorL();
		TInt TestConstructorNoMemoryL();
		TInt TestUnknownFileL();
		TInt TestCorruptFileL();
		TInt TestLoad8bitMonoToneL();
		TInt TestLoad16bitMonoToneL();
		TInt TestLoadGsm6_10ToneL();
		TInt TestLoadNoMemoryL();
		TInt TestCancelLoadL();
		TInt TestStartL();
		TInt TestCancelStartL();
		TInt TestCancelNotActiveL();
		TInt TestStartBeforeLoadL();
		TInt TestSetVolumeL();
		TInt TestSetVolumeRampL();
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;
		
    /** File server session used in testing. */
    RFs iFs;

    /** Instance of the class that this test suite is testing. */
    CSAnimTonePlugin* iTestObj;

    /** A 32 bit identifer returned by ECom to identify this instance to the framework. */
    TUid iDtorKey;

    /** For testing async calls. */
    CTestWaiter* iWaiter;
    };

#endif      // TSANIMMMFPLUGIN_H

// End of File
