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

#ifndef TSANIMCTRL_H
#define TSANIMCTRL_H

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
_LIT( KTSanimctrlLogPath, "\\logs\\testframework\\TSanimctrl\\" ); 
// Log file
_LIT( KTSanimctrlLogFile, "TSanimctrl.txt" ); 
_LIT( KTSanimctrlLogFileWithTitle, "TSanimctrl_[%S].txt" );


#include <e32def.h>
#include "sanimstartupctrl.h"

class CSAnimMockView;
class CTestWaiter;
class CTSanimctrl;

NONSHARABLE_CLASS(CTSanimctrl) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTSanimctrl* NewL( CTestModuleIf& aTestModuleIf,CSAnimMockView& aview  );

        /**
        * Destructor.
        */
        virtual ~CTSanimctrl();

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
        CTSanimctrl( CTestModuleIf& aTestModuleIf,CSAnimMockView& aview );

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
        void SetupParamsL(
            const TDesC& aImageFileName,
            const TDesC& aToneFileName,
            const TInt aFrameDelay,
            const TInt aVolume,
            const TInt aVolumeRamp );
        TInt SetupProfileL( const TInt aRingingType, const TInt aRingingVolume );
        TInt InitObjL();
        TInt SetupEmptyL();
        TInt SetupEmptyValuesL();
        TInt SetupUnknownFileL();
        TInt SetupProtectedFileL();
        TInt SetupExistingImageFileL();
        TInt SetupExistingToneFileSilentL();
        TInt SetupExistingToneFileVeryLowVolumeL();
        TInt SetupExistingToneFileLowVolumeL();
        TInt SetupExistingToneFileNormalVolumeL();
        TInt SetupExistingToneFileTooLowVolumeL();
        TInt SetupExistingToneFileTooHighVolumeL();
        TInt SetupUnknownToneFileNormalVolumeL();
        TInt SetupObjL();
        TInt SetupEmptyShowL();
        TInt SetupShowL();
        TInt Teardown();

        TInt TestConstructorL();
        TInt TestConstructorNoMemoryL();
        TInt TestLoadNoContentL();
        TInt TestLoadHasContentL();
        TInt TestLoadNoMemoryL();
        TInt TestCancelLoadL();
        TInt TestLoadWhenActiveL();
        TInt TestStartL();
        TInt TestCancelStartL();
        TInt TestStartWhenActiveL();
        
        
        // CtrTestSuite
        TInt SetupEmptyCtrL();
        TInt SetupObjCtrL();
        TInt SetupObjLoadedCtrL();
        TInt TeardownCtr();

        TInt TestConstructorCtrL();
        TInt TestConstructorNoMemoryCtrL();
        TInt TestLoadEmptyCtrL();
        TInt TestLoadImageCtrL();
        TInt TestLoadToneCtrL();
        TInt TestLoadBothCtrL();
        TInt TestLoadWhenActiveCtrL();
        TInt TestCancelLoadCtrL();

        TInt TestStartCtrL();
        TInt TestCancelStartCtrL();
        TInt TestStartNoMemoryCtrL();
        TInt TestStartWhenActiveCtrL();

        
       
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();
    private: // data
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

        /** Parent control for the object to be tested. */
        CSAnimMockView& iView;
          

          /** Parameters to use in testing. */
          CSAnimStartupCtrl::TAnimationParams iParams;


          /** Instance of the class that this test suite is testing. */
          CSAnimStartupCtrl* iTestObj;
          
          /** Instance of the class that this test suite is testing. */
          CSAnimCtrl* iTestObj2;

          /** For testing async calls. */
          CTestWaiter* iWaiter;
          CTestWaiter* iWaiter2;
          
         
    };

#endif      // TSANIMCTRL_H

// End of File
