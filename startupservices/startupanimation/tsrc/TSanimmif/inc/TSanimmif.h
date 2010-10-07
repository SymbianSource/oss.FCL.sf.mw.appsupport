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

#ifndef TSANIMMIF_H
#define TSANIMMIF_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <e32def.h>
#include <f32file.h>
#include "sanimobserver.h"


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KTSanimmifLogPath, "\\logs\\testframework\\TSanimmif\\" ); 
// Log file
_LIT( KTSanimmifLogFile, "TSanimmif.txt" ); 
_LIT( KTSanimmifLogFileWithTitle, "TSanimmif_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CTSanimmif;
class CSAnimImagePlugin;
class CTestWaiter;

NONSHARABLE_CLASS( TMockMifAnimationObserver ) : public MSAnimObserver
    {
public:
    TMockMifAnimationObserver( CTSanimmif& aTestSuite ) : iTestSuite( aTestSuite ) {}
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ );
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/ );
private:
    CTSanimmif& iTestSuite;
    };


/**
*  CTSanimmif test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTSanimmif) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTSanimmif* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTSanimmif();

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
        CTSanimmif( CTestModuleIf& aTestModuleIf );

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
        TInt SetupEmptyL();
        TInt SetupObjL();
        TInt SetupFileL( const TDesC& aFileName );
        TInt SetupStillImageNoMaskL();
        TInt SetupStillImageL();
        TInt SetupScaledAnimationL();
        TInt SetupUnscaledAnimationL();
        TInt SetupFinishedL();
        TInt Teardown();

        TInt TestConstructorL();
        TInt TestConstructorNoMemoryL();
        TInt TestUnknownFileL();
        TInt TestCorruptFileL();
        TInt TestNoFramesFileL();
        TInt TestLoadStillImageL();
        TInt TestLoadScaledAnimationL();
        TInt TestLoadUnscaledAnimationL();
        TInt TestLoadNoMemoryL();
        TInt TestCancelLoadL();
        TInt TestShowStillImageL();
        TInt TestShowAnimationL();
        TInt TestCancelStillImageL();
        TInt TestCancelAnimationL();
        TInt TestCancelNotActiveL();
        TInt TestShowBeforeLoadL();
        TInt TestSetDisplayModeL();
        TInt TestSetSizeL();
        TInt TestSetFrameDelayL();
        TInt TestBackroundColourL();
           

    private: // data
        friend class TMockMifAnimationObserver;

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
        TMockMifAnimationObserver iObserver;

    };

#endif      // TSANIMMIF_H

// End of File
