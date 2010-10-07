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
* CSysErrCmdTest class implementation.
*
*/

#include <e32debug.h>
#include <stifparser.h>
#include <stiftestinterface.h>

//#include <aknglobalnote.h>
//#include <akncapserverclient.h>
#include <ssm/ssmcustomcommand.h>

#include "syserrcmdtest.h"
#include "syserrcmdfactory.h"
#include "syserrcmdtestsstub.h"
#include "trace.h"

//  INTERNAL INCLUDES
NONSHARABLE_CLASS( TWaitInfo )
    {
    public:
    
        CPeriodic* iPeriodic;
        CActiveSchedulerWait* iWait;
    };
    

/**
* Call back method when we need to stop active scheduler wait.
*/
LOCAL_C TInt WaitCallBack( TAny* aSelf )
    {
    if( aSelf )
        {
        TWaitInfo* info = static_cast<TWaitInfo*>( aSelf );
        if( info->iPeriodic )
            {
            info->iPeriodic->Cancel();
            }
        if( info->iWait )
            {
            if( info->iWait->IsStarted() )
                {
                info->iWait->AsyncStop();
                }
            }
        }
    
    return KErrNone;
    }

/**
* Helper method to wait current scheduler before teardown is completed.
*/
LOCAL_C void WaitL( TInt aIntervalInMicorseconds )
    {
    TWaitInfo info;
    
    // Construct periodic
    CPeriodic* periodic = CPeriodic::NewL( CActive::EPriorityStandard );
    CleanupStack::PushL( periodic );
    info.iPeriodic = periodic;
    
    // Construct active scheduler wait
    CActiveSchedulerWait* wait = new( ELeave ) CActiveSchedulerWait;
    CleanupStack::PushL( wait );
    info.iWait = wait;
    
    // Start timer and wait
    TCallBack cb( WaitCallBack, &info );
    periodic->Start( aIntervalInMicorseconds, aIntervalInMicorseconds, cb );
    wait->Start();
    
    // Cleanup
    CleanupStack::PopAndDestroy( wait );
    CleanupStack::PopAndDestroy( periodic );
    }

// ---------------------------------------------------------
// CSysErrCmdTest::LibEntryL
// Returns: Poiter to CSysErrCmdTest class
// ---------------------------------------------------------
EXPORT_C CSysErrCmdTest* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;
    
    CSysErrCmdTest* libEntry( CSysErrCmdTest::NewL( aTestModuleIf ) );
    return libEntry;
    }

// ---------------------------------------------------------
// CSysErrCmdTest::CSysErrCmdTest
// ---------------------------------------------------------
CSysErrCmdTest::CSysErrCmdTest( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf )
    {
    FUNC_LOG;    
    }

// ---------------------------------------------------------
// CSysErrCmdTest::ConstructL
// ---------------------------------------------------------
void CSysErrCmdTest::ConstructL()
    {
    FUNC_LOG;
    
    iExecuteHandler = CAsyncRequestHandler<CSysErrCmdTest>::NewL(
                                    *this,
                                    HandleIssueRequest,
                                    HandleRunL,
                                    HandleRunError,
                                    HandleDoCancel,
                    CAsyncRequestHandler<CSysErrCmdTest>::ERequestOneShot );

    User::LeaveIfError( iFs.Connect() );
    
    iCustCmdEnvStub = SysErrCmdTestsStub::CustomCommandEnvStubL( iFs );
    }

// ---------------------------------------------------------
// CSysErrCmdTest::NewL
// ---------------------------------------------------------
CSysErrCmdTest* CSysErrCmdTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    FUNC_LOG;
    
    CSysErrCmdTest* self = new (ELeave) CSysErrCmdTest( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CSysErrCmdTest::~CSysErrCmdTest
// ---------------------------------------------------------
CSysErrCmdTest::~CSysErrCmdTest()
    {
    iFs.Close();
    delete iExecuteHandler;
    delete iCustCmdEnvStub;
    FUNC_LOG;    
    }

// ---------------------------------------------------------
// CSysErrCmdTest::RunMethodL
// ---------------------------------------------------------
TInt CSysErrCmdTest::RunMethodL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    
    const TStifFunctionInfo KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "CreateAndDestroy", CSysErrCmdTest::CreateAndDestroyL ),
        ENTRY( "InitAndClose", CSysErrCmdTest::InitAndCloseL ),
        ENTRY( "Execute", CSysErrCmdTest::ExecuteL ),
        ENTRY( "ExecuteCancel", CSysErrCmdTest::ExecuteCancelL ),
        ENTRY( "ExecuteAfterGlobalNote", CSysErrCmdTest::ShowAfterAknGlobalNoteL ),
        ENTRY( "ExecuteAfterUiServiceGlobalNote", CSysErrCmdTest::ShowAfterUiServerGlobalNoteL )
        };
    const TInt count( sizeof( KFunctions ) / sizeof( TStifFunctionInfo ) );
    TInt ret( RunInternalL( KFunctions, count, aItem ) );
    return ret;
    }

// ---------------------------------------------------------
// CSysErrCmdTest::CreateAndDestroyL
// ---------------------------------------------------------
    
TInt CSysErrCmdTest::CreateAndDestroyL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    ( void )aItem;
    MSsmCustomCommand* sysErrCmd = SysErrCmdFactory::SysErrCmdNewL();
    sysErrCmd->Release();
    return KErrNone;
    }

// ---------------------------------------------------------
// CSysErrCmdTest::InitAndCloseL
// ---------------------------------------------------------
TInt CSysErrCmdTest::InitAndCloseL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    ( void )aItem;

    MSsmCustomCommand* sysErrCmd = SysErrCmdFactory::SysErrCmdNewL();
     
    TInt err( sysErrCmd->Initialize( iCustCmdEnvStub ) );
    ERROR( err, "Failed to init syserrcmd" );
    User::LeaveIfError( err );
    
    sysErrCmd->Close();    
    sysErrCmd->Release();

    return KErrNone;
    }
// ---------------------------------------------------------
// CSysErrCmdTest::ExecuteL
// ---------------------------------------------------------

TInt CSysErrCmdTest::ExecuteL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    ( void )aItem;
    iSysErrCmd = SysErrCmdFactory::SysErrCmdNewL();
    TInt err( iSysErrCmd->Initialize( iCustCmdEnvStub ) );
    ERROR( err, "Failed to init syserrcmd" );
    User::LeaveIfError( err );
    
    iExecuteHandler->IssueRequest();
    
    WaitL( 5000 );
    
    iSysErrCmd->Close();
    iSysErrCmd->Release();
    
    INFO_1( "Execution result %d", iExecutionResult );
    
    return iExecutionResult;
    }

// ---------------------------------------------------------
// CSysErrCmdTest::ExecuteL
// ---------------------------------------------------------

TInt CSysErrCmdTest::ExecuteCancelL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    ( void )aItem;
    iSysErrCmd = SysErrCmdFactory::SysErrCmdNewL();
    TInt err( iSysErrCmd->Initialize( iCustCmdEnvStub ) );
    ERROR( err, "Failed to init syserrcmd" );
    User::LeaveIfError( err );
    
    iExecuteHandler->IssueRequest();
    
    WaitL( 5000 );

    iSysErrCmd->ExecuteCancel();
    
    WaitL( 5000 );
        
    iSysErrCmd->Close();
    iSysErrCmd->Release();
    
    INFO_1( "ExecutionCancel result %d", iExecutionResult );
    
    return ( iExecutionResult == KErrCancel ) ? KErrNone : KErrGeneral;    
    }

// ---------------------------------------------------------
// CSysErrCmdTest::ShowAfterAknGlobalNoteL
// ---------------------------------------------------------
TInt CSysErrCmdTest::ShowAfterAknGlobalNoteL( CStifItemParser& aItem )
    {/*
    CAknGlobalNote* note = CAknGlobalNote::NewLC();
    note->ShowNoteL( EAknGlobalInformationNote, _L("CAknGlobalNote::ShowNoteL()") );
    CleanupStack::PopAndDestroy( note );

    return ExecuteL( aItem );
    */
    return KErrNone;
    }

// ---------------------------------------------------------
// CSysErrCmdTest::ShowAfterUiServerGlobalNoteL
// ---------------------------------------------------------
TInt CSysErrCmdTest::ShowAfterUiServerGlobalNoteL( CStifItemParser& aItem )
    {
  /*  RAknUiServer aknSrv;
    
    User::LeaveIfError( aknSrv.Connect() );
    
    CleanupClosePushL( aknSrv );
    
    aknSrv.ShowGlobalNoteL(  _L("RAknUiServer::ShowGlobalNoteL()"), EAknGlobalInformationNote );
    
    CleanupStack::PopAndDestroy( &aknSrv );
    */
    
    return ExecuteL( aItem );
    }

// ---------------------------------------------------------
// CSysErrCmdTest::HandleIssueRequest
// ---------------------------------------------------------

void CSysErrCmdTest::HandleIssueRequest( TRequestStatus& aRequest )
    {
    FUNC_LOG;
    
    iSysErrCmd->Execute( KNullDesC8, aRequest );

    }
// ---------------------------------------------------------
// CSysErrCmdTest::HandleRunL
// ---------------------------------------------------------
   
void CSysErrCmdTest::HandleRunL( TInt aStatus )
    {
    FUNC_LOG;
    INFO_1( "CSysErrCmdTest::HandleRunL %d", aStatus );
    
    if ( KErrNone != aStatus )
        {
        iExecutionResult = aStatus;
        }
    }
// ---------------------------------------------------------
// CSysErrCmdTest::HandleRunError
// ---------------------------------------------------------
   
TInt CSysErrCmdTest::HandleRunError( TInt aError )
    {
    FUNC_LOG;
    ERROR( aError, "CSysErrCmdTest::HandleRunError" );
    return KErrNone;
    }
// ---------------------------------------------------------
// CSysErrCmdTest::HandleDoCancel
// ---------------------------------------------------------
   
void CSysErrCmdTest::HandleDoCancel()
    {
    FUNC_LOG;
    }

    
