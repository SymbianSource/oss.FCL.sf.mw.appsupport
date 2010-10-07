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
* Description: STIF testclass declaration
*
*/

#ifndef ACCFWUINOTIFIERTESTMODULE_H
#define ACCFWUINOTIFIERTESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <ecom/implementationproxy.h>
#include <ecom/implementationinformation.h>
#include <ecom/ecom.h>





// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KAccFwUiNotifierTestModuleLogPath, "\\logs\\testframework\\AccFwUiNotifierTestModule\\" ); 
// Log file
_LIT( KAccFwUiNotifierTestModuleLogFile, "AccFwUiNotifierTestModule.txt" ); 
_LIT( KAccFwUiNotifierTestModuleLogFileWithTitle, "AccFwUiNotifierTestModule_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CAccFwUiNoteNotifier;
class CAccFwUiNotifierTestModule;

// DATA TYPES
//enum 
// Forward Declarations
enum TTestPanic
     {
     EPanicBadDescriptor,
     EPanicIllegalFunction,
     EPanicAlreadyReceiving
     };

//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CAccFwUiNotifierTestModule test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CAccFwUiNotifierTestModule) : public CScriptBase, public RSessionBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAccFwUiNotifierTestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CAccFwUiNotifierTestModule();

    public: // New functions
        

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
        CAccFwUiNotifierTestModule( CTestModuleIf& aTestModuleIf );

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
         * Panic Client
         */
        void PanicClient(TInt panic);

        /**
        * Test methods are listed below. 
        */
        TInt CreateNotifierPlugin( CStifItemParser& aItem );
        TInt TestSupportedAccessoryUiNoteL (CStifItemParser& aItem);
        TInt TestUnsupportedAccessoryUiNoteL (CStifItemParser& aItem);
        TInt TestUpdateNotifierL (CStifItemParser& aItem);
        TInt TestStartNotifierL (CStifItemParser& aItem);
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
        CAccFwUiNoteNotifier* iNotifierPlugin;
        TUid idtorID;
        RImplInfoPtrArray iImplementations;
        RMessagePtr2 iMessagePtr;
    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // ACCFWUINOTIFIERTESTMODULE_H

// End of File
