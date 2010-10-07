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

#ifndef TSANIMIHL_H
#define TSANIMIHL_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include <f32file.h>
#include "sanimobserver.h"

class CSAnimImagePlugin;
class CSAnimIhlPluginTestSuite;
class CTestWaiter;

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CTsanimihl;

class TMockIhlAnimationObserver : public MSAnimObserver
    {
public:
    TMockIhlAnimationObserver( CTsanimihl& aTestSuite ) : iTestSuite( aTestSuite ) {}
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ );
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/ );
private:
    CTsanimihl& iTestSuite;
    };


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KTsanimihlLogPath, "\\logs\\testframework\\Tsanimihl\\" ); 
// Log file
_LIT( KTsanimihlLogFile, "Tsanimihl.txt" ); 
_LIT( KTsanimihlLogFileWithTitle, "Tsanimihl_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);



// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CTsanimihl test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTsanimihl) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTsanimihl* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTsanimihl();

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

   
    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CTsanimihl( CTestModuleIf& aTestModuleIf );
      
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
       
    private:    // New methods

           /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt ExampleL( CStifItemParser& aItem );
        TInt SetupEmptyL();
        TInt SetupObjL();
        TInt SetupStillImageL();
        TInt SetupAnimationL();
        TInt SetupMultiFrameL();
        TInt SetupStillImageWithBgColourL();
        TInt Teardown();

        TInt TestConstructorL();
        TInt TestConstructorNoMemoryL();
        TInt TestUnknownFileL();
        TInt TestCorruptFileL();
        TInt TestLoadStillImageGray2L();
        TInt TestLoadStillImageColourL();
        TInt TestLoadAnimationL();
        TInt TestLoadMultiFrameL();
        TInt TestLoadStillImageNoMemoryL();
        TInt TestLoadAnimationNoMemoryL();
        TInt TestCancelLoadStillImageL();
        TInt TestCancelLoadAnimationL();
        TInt TestCancelLoadMultiFrameL();
        TInt TestShowStillImageL();
        TInt TestShowStillImageWithFrameDelayL();
        TInt TestShowAnimationL();
        TInt TestShowMultiFrameL();
        TInt TestCancelPlayL();
        TInt TestBgColourL();
        TInt TestBgColourBeforeLoadL();
        TInt TestScaleStillImageL();
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        friend class TMockIhlAnimationObserver;

        /** File server session used in testing. */
        RFs iFs;

        /** Instance of the class that this test suite is testing. */
        CSAnimImagePlugin* iTestObj;

        /** A 32 bit identifer returned by ECom to identify this instance to the framework. */
        TUid iDtorKey;

        /** For testing async calls. */
        CTestWaiter* iWaiter;

        /** For testing correct number of calls to animation observer. */
        TInt iFrameCounter;

        /** Animation observer for testing. */
        TMockIhlAnimationObserver iObserver;
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

       };

#endif      // TSANIMIHL_H

// End of File
