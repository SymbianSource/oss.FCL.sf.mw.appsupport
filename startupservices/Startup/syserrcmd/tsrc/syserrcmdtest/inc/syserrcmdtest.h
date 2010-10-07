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
* Description:
* Csyserrcmdtest class declaration.
*
*/


#ifndef SYSERRCMDTEST_H
#define SYSERRCMDTEST_H

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

// SYSTEM INCLUDES
#include <testscripterinternal.h>

#include "asyncrequesthandler.h"

// FORWARD DECLARATIONS
class TCaseInfo;
class CSysErrCmdTest;
class MSsmCustomCommand;
class CSsmCustomCommandEnv;

// DESCRIPTION
// This a Test Module interface template
// that does not really do anything.
typedef TInt ( CSysErrCmdTest::*TestFunction )( TTestResult& );

NONSHARABLE_CLASS( CSysErrCmdTest ) : public CScriptBase
    {
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CSysErrCmdTest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CSysErrCmdTest();

public: // Functions from base classes

    TInt RunMethodL( CStifItemParser& aItem );

protected: // New functions

    TInt CreateAndDestroyL( CStifItemParser& aItem );
    TInt InitAndCloseL( CStifItemParser& aItem );
    TInt ExecuteL( CStifItemParser& aItem );
    TInt ExecuteCancelL( CStifItemParser& aItem );
    TInt ShowAfterAknGlobalNoteL( CStifItemParser& aItem );
    TInt ShowAfterUiServerGlobalNoteL( CStifItemParser& aItem );
    
    /** HandleIssueRequest callback */
    void HandleIssueRequest( TRequestStatus& );
       
    /** HandleRunL callback */
    void HandleRunL( TInt );
       
    /** HandleRunError callback */
    TInt HandleRunError( TInt );
       
    /** HandleDoCancel callback */
    void HandleDoCancel();

private:

    /**
    * C++ default constructor.
    */
    CSysErrCmdTest( CTestModuleIf& aTestModuleIf );

    /**
    * By default Symbian OS constructor is private.
    */
    void ConstructL();

    /**
    * Function returning test case name and pointer to test case function
    */
    const TCaseInfo Case( const TInt aCaseNumber ) const;

private: // Data

    TestFunction iMethod;
    
    CAsyncRequestHandler<CSysErrCmdTest>*  iExecuteHandler;
    
    RFs                                    iFs;
    
    MSsmCustomCommand*                     iSysErrCmd;
    
    CSsmCustomCommandEnv*                  iCustCmdEnvStub;
    
    TInt                                   iExecutionResult;
    };

// Function pointer related internal definitions

// Hack around known GCC bug.
#ifndef __GCC32__
    #define GETPTR
#else
    #define GETPTR &
#endif


// An internal structure containing a test case name and
// the pointer to function doing the test
class TCaseInfoInternal
    {
    public:
        const TText* iCaseName;
        TestFunction iMethod;
    };

// An internal structure containing a test case name and
// the pointer to function doing the test
class TCaseInfo
    {
    public:
        TPtrC iCaseName;
        TestFunction iMethod;

    TCaseInfo( const TText* a ) : iCaseName( ( TText* ) a )
        {
        };
    };

#endif // SYSERRCMDTEST_H

