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

#ifndef TSANIMENGINE_H
#define TSANIMENGINE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KTSanimengineLogPath, "\\logs\\testframework\\TSanimengine\\" ); 
// Log file
_LIT( KTSanimengineLogFile, "TSanimengine.txt" ); 
_LIT( KTSanimengineLogFileWithTitle, "TSanimengine_[%S].txt" );


#include <f32file.h>
#include "sanimobserver.h"

class CSAnimEngine;
class CTestWaiter;

class TMockAnimationObserver : public MSAnimObserver
    {
public:
    TMockAnimationObserver() {}
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ ) {}
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/ ) {}
    };

class CTSanimengine;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CTSanimengine test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTSanimengine) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTSanimengine* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTSanimengine();

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
        CTSanimengine( CTestModuleIf& aTestModuleIf );

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
    TInt SetupImagePluginL();
    TInt SetupTonePluginL();
    TInt SetupBothPluginsL();
    TInt SetupImagePluginFasterL();
    TInt SetupTonePluginFasterL();
    TInt SetupImagePluginReturnsErrorL();
    TInt SetupTonePluginReturnsErrorL();
    TInt SetupBothPluginsReturnErrorL();
    TInt LoadL();
    TInt SetupImageLoadedL();
    TInt SetupToneLoadedL();
    TInt SetupBothLoadedL();
    TInt SetupLoadFailedL();
    TInt SetupBothFinishedL();
    TInt Teardown();

    TBool CheckNoImagePluginOutputL();
    TBool CheckImagePluginOutputL(
        const TUint32 aPluginUid,
        const TDesC& aFileToLoad,
        const TInt aDisplayMode,
        const TSize& aSize,
        const TInt aFrameDelay );
    TBool CheckNoTonePluginOutputL();
    TBool CheckTonePluginOutputL(
        const TUint32 aPluginUid,
        const TDesC& aFileToLoad,
        const TInt aVolume,
        const TInt64& aVolumeRamp );

    TInt TestConstructorL();
    TInt TestConstructorNoMemoryL();
    TInt TestExtensionlessFileL();
    TInt TestTooLongExtensionL();
    TInt SetImagePropertiesUnknownExtensionL();
    TInt SetImagePropertiesUnknownExtensionNoMemoryL();
    TInt SetImagePropertiesKnownExtensionL();
    TInt SetImagePropertiesKnownExtensionNoMemoryL();
    TInt SetImagePropertiesManyPluginsL();
    TInt SetImagePropertiesManyPluginsNoMemoryL();
    TInt SetImagePropertiesWildcardMatchL();
    TInt SetImagePropertiesWildcardMatchNoMemoryL();
    TInt SetImagePropertiesManyExtensionsL();
    TInt SetImagePropertiesManyExtensionsNoMemoryL();
    TInt SetTonePropertiesUnknownExtensionL();
    TInt SetTonePropertiesUnknownExtensionNoMemoryL();
    TInt SetTonePropertiesKnownExtensionL();
    TInt SetTonePropertiesKnownExtensionNoMemoryL();
    TInt SetTonePropertiesManyPluginsL();
    TInt SetTonePropertiesManyPluginsNoMemoryL();
    TInt SetTonePropertiesWildcardMatchL();
    TInt SetTonePropertiesWildcardMatchNoMemoryL();
    TInt TestLoadEmptyL();
    TInt TestCancelLoadEmptyL();
    TInt TestLoadNoMemoryL();
    TInt TestCancelLoadL();
    TInt TestLoadImagePluginFasterL();
    TInt TestLoadTonePluginFasterL();
    TInt TestLoadImagePluginReturnsErrorL();
    TInt TestLoadTonePluginReturnsErrorL();
    TInt TestBackroundColourL();
    TInt TestCancelNotActiveL();
    TInt TestLoadWhenActiveL();
    TInt TestStartWhenActiveL();
    TInt TestStartL();
    TInt TestStartNotReadyL();
    TInt TestLoadAgainL();
    TInt TestStartAgainL();
    TInt TestLoadAndStartAnotherShowL();
    TInt TestLoadAndStartEmptyShowAfterAnotherShowL();
    TInt TestStartWhenLoadFailedL();

private: // data

    /** File server session used in testing. */
    RFs iFs;

    /** Animation observer for testing. */
    TMockAnimationObserver iObserver;

    /** Instance of the class that this test suite is testing. */
    CSAnimEngine* iTestObj;

    /** For testing async calls. */
    CTestWaiter* iWaiter;
    CTestWaiter* iWaiter2;

    

    };

#endif      // TSANIMENGINE_H

// End of File
