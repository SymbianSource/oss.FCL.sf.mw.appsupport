/*
 ============================================================================
 Name		: testtspcontroller.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "testtspcontroller.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <apgtask.h>

#include <e32math.h>
#include <remconcoreapi.h>
#include <tspclientmapper.h>
#include <absolutevolumeapi.h>
#include <remcon/bearersecurity.h>
#include <remconaddress.h>

#include <StifLogger.h>

#include "remcontspcontroller.h"
#include "mockremcontspobserver.h"
#include "mockcfactionindicationimpl.h"

#define RUN_TEST_CASE(_tt, _tp, tf) \
{\
    _tt++;                  \
    if( KErrNone==tf )      \
        _tp++;              \
}        

//  Constants
const TUint32 KUid3BTAudioMan = 0x10208971;

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

// constant for stif log
// Logging path
_LIT( KutremcontspcontrollerLogPath, "\\logs\\testframework\\" ); 
// Log file
_LIT( KutremcontspcontrollerLogFile, "testtspcontroller.txt" ); 
//_LIT( KutremcontspcontrollerLogFileWithTitle, "testtspcontroller_[%S].txt" );

_LIT(KTTestNumberStart, "Test%d start. \n");
_LIT(KTTestNumberPassed, "Test%d passed. \n");

// literal constants for set event table
_LIT( KTsp, "Tsp" );    
_LIT( KRoutingTable, "routingTable" );
/*
_LIT( KRemConKeyEvent, "keyEventRemCon*" );
_LIT( KRemConKeyEventExt, "keyEventRemConExt*" );
_LIT( KRemConKeyEventExtCall, "keyEventRemConExtCall*" );
_LIT( KRemConKeyEventExtPoc, "keyEventRemConExtPoc*" );
_LIT( KRemConKeyEventExtSync, "keyEventRemConExtSync*" );
_LIT( KRuleText, "rule*" );
_LIT( KDefaultAppText, "defaultApplication*" );
_LIT( KTransparentAppText, "transparentApplication*" );
_LIT( KRemoveRoutingTable, "removeRoutingTable" );
_LIT( KDefaultLaunchAppText, "defaultLaunchApp*" );
_LIT( KStoreRoutingTable, "storeRoutingTable" );
_LIT( KRestoreRoutingTable, "restoreRoutingTable" );
*/
//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this

// stif logger pointer
CStifLogger* stifLog;
// variables for all test cases
CMockRemConTSPObserver* mockObserver(NULL);
CRemConTspController* remconTspController(NULL);
MCFListener* cfListener(NULL);
TSglQue<TClientInfo> clients;
RProcess thisProc;
TClientInfo thisClient;
TProcessId fakeId( thisProc.Id() + 100 );
TClientInfo fakeClient;
TClientInfo anotherFakeClient;
TClientInfo fakeClientPhone;
TClientInfo fakeTransparentClient;
TClientInfo* client(NULL);
MRemConTargetSelectorPluginInterfaceV2* tspIf2(NULL);
MRemConTargetSelectorPluginInterfaceV4* tspIf4(NULL);
MRemConTargetSelectorPluginInterfaceV5* tspIf5(NULL);
TInt err(KErrNone);    
TInt testCasesTotal(0);
TInt testCasesPassed(0);


/** Rules assumed by this test suite. 
<Tsp
routingTable='01'
keyEventRemConVolumeUp='41'
keyEventRemConVolumeDown='42'
/>

<Tsp
routingTable='01'
ruleForeground='01'
ruleActiveAudio='00'
ruleLastPlayedAudio='03'
ruleDefaultApp='02'
defaultApplicationPhone='100058B3'
/>
                    
<Tsp
routingTable='09'
keyEventRemConF4='74'            
/>

<Tsp
routingTable='09'
ruleDefaultLaunchApp='04'
defaultLaunchApp='EFFFFFFF'
/>
*/

LOCAL_C void TestSetupL()
    {
    _LIT(KSetupStart, "TestSetup start");
    stifLog->Log(KSetupStart);

    // create mock observer
    mockObserver = CMockRemConTSPObserver::NewL();
    // instantiate remote controller tsp controller
    remconTspController = CRemConTspController::NewL(*mockObserver);
    // get various interfaces for each test cases
    CRemConTargetSelectorPlugin* tspp(remconTspController);
    tspIf2 = static_cast<MRemConTargetSelectorPluginInterfaceV2*>( tspp->GetInterface( TUid::Uid( KRemConTargetSelectorInterface2 ) ) );
    tspIf4 = static_cast<MRemConTargetSelectorPluginInterfaceV4*>( tspp->GetInterface( TUid::Uid( KRemConTargetSelectorInterface4 ) ) );
    tspIf5 = static_cast<MRemConTargetSelectorPluginInterfaceV5*>( tspp->GetInterface( TUid::Uid( KRemConTargetSelectorInterface5 ) ) );
       
    // create action parameters according to test rule table, see comments for 
    // event table before this function
    CMockCFActionIndicationImpl* cfActInd(NULL);
    cfActInd= CMockCFActionIndicationImpl::NewL();
    CleanupStack::PushL(cfActInd);
    
    cfActInd->SetIdentifierL(KTsp);
    
    cfActInd->AddParameterL(KRoutingTable,_L("01"));
    cfActInd->AddParameterL(_L("keyEventRemConVolumeUp"),_L("41"));
    cfActInd->AddParameterL(_L("keyEventRemConVolumeDown"),_L("42"));
    cfActInd->AddParameterL(_L("keyEventRemConPlay"),_L("44"));

    cfActInd->AddParameterL(KRoutingTable,_L("01"));
    cfActInd->AddParameterL(_L("ruleForeground"),_L("01"));
    cfActInd->AddParameterL(_L("ruleActiveAudio"),_L("00"));
    cfActInd->AddParameterL(_L("ruleLastPlayedAudio"),_L("03"));
    cfActInd->AddParameterL(_L("ruleDefaultApp"),_L("02"));
    cfActInd->AddParameterL(_L("defaultApplicationPhone"),_L("100058B3"));    
    cfActInd->AddParameterL(_L("transparentApplication1"),_L("10005555"));
        
    cfActInd->AddParameterL(KRoutingTable,_L("09"));
    cfActInd->AddParameterL(_L("keyEventRemConF4"),_L("74"));

    cfActInd->AddParameterL(KRoutingTable,_L("09"));
    cfActInd->AddParameterL(_L("ruleDefaultLaunchApp"),_L("04"));
    cfActInd->AddParameterL(_L("defaultLaunchApp"),_L("EFFFFFFF"));
    
    // call ActionIndication through MCFListener interface
    cfListener = static_cast<MCFListener*> (remconTspController);
    cfListener->ActionIndicationL(*cfActInd);
    CleanupStack::PopAndDestroy(cfActInd);

    // itinilize all client for testing
    thisClient.ProcessId() = thisProc.Id();
    fakeClient.ProcessId() = fakeId.Id();
    anotherFakeClient.ProcessId() = fakeId.Id() + 100;
    fakeClientPhone.ProcessId() = fakeId.Id() + 200;
    fakeClientPhone.SecureId() = TUid::Uid( 0x100058B3 );
    fakeTransparentClient.ProcessId() = fakeId.Id() + 300;
    fakeTransparentClient.SecureId() = TUid::Uid( 0x10005555 );    
    }
LOCAL_C void TestTearDown()
    {
    _LIT(KSetupStart, "TestTearDown start");
    stifLog->Log(KSetupStart);
    if ( remconTspController!=NULL )
        {
        delete remconTspController;
        }
    if ( mockObserver!=NULL )
        {
        delete mockObserver;
        }    
    }
LOCAL_C TInt Test01()
    {
    stifLog->Log(KTTestNumberStart, 1);
    console->Printf(KTTestNumberStart, 1);
        
    //1.Test the foreground application rule    
    //Test the foreground application rule
    clients.AddLast( thisClient );
    clients.AddLast( fakeClient );
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    clients.AddLast( fakeTransparentClient );
    
    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );    
    
    if( mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
        
    if( clients.IsEmpty() )
        {
        stifLog->Log(_L("test case failed: no client found and returned.\n"));
        console->Printf(_L("test case failed: no client found and returned.\n"));
        return KErrGeneral;
        }
    
    client = clients.Last();
    TClientInfo* clientFirst = clients.First();
    
    //RProcess currProc;    
    //RThread thread;
    //TInt err = thread.Open( ( TUint )RThread().Id(), EOwnerProcess );
    if( (client->ProcessId() == thisProc.Id()) && 
        (clientFirst->SecureId() == fakeTransparentClient.SecureId()))         
        {
        }
    else if ( (clientFirst->ProcessId() == thisProc.Id()) && 
            (client->SecureId() == fakeTransparentClient.SecureId()))
        {
        }
    else
        {
        stifLog->Log(_L("test case failed: wrong client returned\n"));
        console->Printf(_L("test case failed: wrong client returned\n"));
        return KErrGeneral;
        }
    stifLog->Log(KTTestNumberPassed, 1);
    console->Printf(KTTestNumberPassed,1);
    return KErrNone;
    }
LOCAL_C TInt Test02()
    {
    stifLog->Log(KTTestNumberStart, 2);
    console->Printf(KTTestNumberStart, 2);
    // by now this function are empty, so call it here for just a coverage
    cfListener->Extension(TUid::Uid(0));

    stifLog->Log(KTTestNumberPassed, 2);
    console->Printf(KTTestNumberPassed,2);
    return KErrNone;
    }
LOCAL_C TInt Test03()
    {
    stifLog->Log(KTTestNumberStart, 3);
    console->Printf(KTTestNumberStart, 3);
    
    // test 03
    // 3. Test the active audio rule
    // current process is not added to list because events must not fall to foreground application
    clients.Reset();
    clients.AddLast( fakeClient );
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    
    CTspClientMapper* tspCm(NULL);
    TRAP(err, tspCm = CTspClientMapper::NewL()); // register fake client to TSP client mapper, so it is considered as active audio
    if( err!=KErrNone)
        {
        delete tspCm;
        return KErrGeneral;
        }
    CleanupStack::PushL(tspCm);
    tspCm->SetTspTargetClient( CTspClientMapper::EPlayingClients, fakeId );
    mockObserver->SetResponse(KErrGeneral);

    tspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
    tspCm->RemoveTspTargetClient( CTspClientMapper::EPlayingClients, fakeId );
    CleanupStack::PopAndDestroy( tspCm );
    
    if( mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }

    if( clients.IsEmpty() )
        {
        stifLog->Log(_L("test case failed: no client found and returned.\n"));
        console->Printf(_L("test case failed: no client found and returned.\n"));
        return KErrGeneral;
        }
    client = clients.Last();
    
    if( client->ProcessId() != fakeId)
        {
        stifLog->Log(_L("test case failed: wrong client returned\n"));
        console->Printf(_L("test case failed: wrong client returned\n"));
        return KErrGeneral;
        }
    stifLog->Log(KTTestNumberPassed, 3);
    console->Printf(KTTestNumberPassed,3);
    return KErrNone;
    }
LOCAL_C TInt Test04()
    {
    stifLog->Log(KTTestNumberStart, 4);
    console->Printf(KTTestNumberStart, 4);
    // test 04
    // 4. Test the last played application rule
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClient );
    clients.AddLast( fakeClientPhone );

    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
    if( mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }

    if( clients.IsEmpty() )
        {
        stifLog->Log(_L("test case failed: no client found and returned.\n"));
        console->Printf(_L("test case failed: no client found and returned.\n"));
        return KErrGeneral;
        }
    client = clients.Last();
    
    if( client->ProcessId() != fakeId)
        {
        stifLog->Log(_L("test case failed: wrong client returned\n"));
        console->Printf(_L("test case failed: wrong client returned\n"));
        return KErrGeneral;
        }
    stifLog->Log(KTTestNumberPassed, 4);
    console->Printf(KTTestNumberPassed,4);
    return KErrNone;
    }
LOCAL_C TInt Test05()
    {
    stifLog->Log(KTTestNumberStart, 5);
    console->Printf(KTTestNumberStart, 5);
    // test 05    
    // 5. Test the default application (=Phone) rule
    // drop the 1st fake client so we won't fall to last played application rule
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );

    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiVolumeUp, clients );
            
    if( mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }

    if( clients.IsEmpty() )
        {
        stifLog->Log(_L("test case failed: no client found and returned.\n"));
        console->Printf(_L("test case failed: no client found and returned.\n"));
        return KErrGeneral;
        }
    client = clients.Last();
    if( client->ProcessId() != fakeClientPhone.ProcessId())
        {
        stifLog->Log(_L("test case failed: wrong client returned\n"));
        console->Printf(_L("test case failed: wrong client returned\n"));
        return KErrGeneral;
        }
    stifLog->Log(KTTestNumberPassed, 5);
    console->Printf(KTTestNumberPassed,5);
    return KErrNone;
    }
LOCAL_C TInt Test06()
    {
    stifLog->Log(KTTestNumberStart, 6);
    console->Printf(KTTestNumberStart, 6);

    // test 06
    // 6. Test an key event that does not have a rule assigned to it
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiF5, clients );
    if( mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    
    stifLog->Log(KTTestNumberPassed, 6);
    console->Printf(KTTestNumberPassed,6);
    return KErrNone;
    }
LOCAL_C TInt Test07()
    {
    stifLog->Log(KTTestNumberStart, 7);
    console->Printf(KTTestNumberStart, 7);
    // 7. Test a key event that attempts to launch an (non-existing) application
    clients.Reset();
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressIncomingCommand( TUid::Uid(KRemConCoreApiUid), ERemConCoreApiF4, clients );
    if( mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    
    stifLog->Log(KTTestNumberPassed, 7);
    console->Printf(KTTestNumberPassed,7);
    return KErrNone;
    }
LOCAL_C TInt Test08()
    {
    stifLog->Log(KTTestNumberStart, 8);
    console->Printf(KTTestNumberStart, 8);
    // 8. addressincomingnotify
    anotherFakeClient.SecureId().iId = KUid3BTAudioMan;
    clients.Reset();    
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    mockObserver->SetResponse(KErrGeneral);
    // set par1 and par2, so that par1 in notification to observer's -- a client info
    // is not null
    tspIf2->AddressIncomingNotify(TUid::Uid(KRemConAbsoluteVolumeControllerApiUid), ERemConCoreApiF4, clients);
    
    // if the notification's first par is not null, then response is 
    if( mockObserver->Response()==KErrGeneral )
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    if( mockObserver->ClientInfo().SecureId()!=anotherFakeClient.SecureId() )
        {
        stifLog->Log(_L("test case failed: wrong client returned\n"));
        console->Printf(_L("test case failed: wrong client returned\n"));
        return KErrGeneral;
        }
    
    stifLog->Log(KTTestNumberPassed, 8);
    console->Printf(KTTestNumberPassed,8);
    return KErrNone;
    }
LOCAL_C TInt Test09()
    {
    stifLog->Log(KTTestNumberStart, 9);
    console->Printf(KTTestNumberStart, 9);

    // 9. addressincomingnotify
    anotherFakeClient.SecureId().iId = KUid3BTAudioMan - 1;
    clients.Reset();    
    clients.AddLast( anotherFakeClient );
    clients.AddLast( fakeClientPhone );
    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressIncomingNotify(TUid::Uid(KRemConAbsoluteVolumeControllerApiUid), ERemConCoreApiF4, clients);
    
    if( mockObserver->Response()==KErrGeneral )
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    if( mockObserver->ClientInfo().SecureId()==anotherFakeClient.SecureId() )
        {
        stifLog->Log(_L("test case failed: wrong client returned\n"));
        console->Printf(_L("test case failed: wrong client returned\n"));
        return KErrGeneral;
        }
    stifLog->Log(KTTestNumberPassed, 9);
    console->Printf(KTTestNumberPassed,9);
    return KErrNone;
    }
LOCAL_C TInt Test10()
    {
    stifLog->Log(KTTestNumberStart, 10);
    console->Printf(KTTestNumberStart, 10);
        
    // test 10 AddressOutgoingCommand
    TSglQue<TBearerSecurity> bearers;
    TSecurityPolicy secPolicy;
    TBearerSecurity bearer(TUid::Uid(100), secPolicy);
    TBearerSecurity anotherBearer(TUid::Uid(200), secPolicy);
    TSglQue<TRemConAddress> connections;
    
    bearers.AddLast(bearer);
    bearers.AddLast(anotherBearer);
    mockObserver->SetResponse(KErrGeneral);
    tspIf2->AddressOutgoingCommand(
            TUid::Uid(KRemConAbsoluteVolumeControllerApiUid), 
            ERemConCoreApiF4,
            anotherFakeClient,
            connections,
            bearers
            );

    if( mockObserver->Response()== KErrGeneral )
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }

    TRemConAddress* conn(connections.First());
    TRemConAddress* anotherConn(connections.Last());
    
    
    if( conn->BearerUid().iUid == bearer.BearerUid().iUid &&
        anotherConn->BearerUid().iUid == anotherBearer.BearerUid().iUid )
        {
        }
    else if( conn->BearerUid().iUid == anotherBearer.BearerUid().iUid &&
            anotherConn->BearerUid().iUid == bearer.BearerUid().iUid )
        {
        
        }
    else        
        {
        stifLog->Log(_L("test case failed: wrong connection returned\n"));
        console->Printf(_L("test case failed: wrong connection returned\n"));
        return KErrGeneral;
        }
    stifLog->Log(KTTestNumberPassed, 10);
    console->Printf(KTTestNumberPassed,10);
    return KErrNone;
    }
LOCAL_C TInt Test11()
    {
    stifLog->Log(KTTestNumberStart, 11);
    console->Printf(KTTestNumberStart, 11);
        
    // test 11 PermitOutgoingResponse
    mockObserver->SetResponse(KErrGeneral);
    TClientInfoConstIter iter(clients);
    tspIf2->PermitOutgoingResponse(
            TUid::Uid(KRemConCoreApiUid), 
            ERemConCoreApiF4, 
            anotherFakeClient,
            iter);
    
    if( mockObserver->Response()== KErrGeneral )
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    
    stifLog->Log(KTTestNumberPassed, 11);
    console->Printf(KTTestNumberPassed,11);
    return KErrNone;
    }
LOCAL_C TInt Test12()
    {
    stifLog->Log(KTTestNumberStart, 12);
    console->Printf(KTTestNumberStart, 12);
        
    
    // test 12 PermitIncomingCommand
    mockObserver->SetResponse(KErrGeneral);
    tspIf4->PermitIncomingCommand(
            TUid::Uid(KRemConCoreApiUid),
                    ERemConCoreApiPlay,
                    anotherFakeClient);
    
    if( mockObserver->Response()== KErrGeneral )
        {
        stifLog->Log(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    
    stifLog->Log(KTTestNumberPassed, 12);
    console->Printf(KTTestNumberPassed,12);
    return KErrNone;
    }
LOCAL_C TInt Test14()
    {
    stifLog->Log(KTTestNumberStart, 14);
    console->Printf(KTTestNumberStart, 14);        
    
    // test 14 TargetClientAvailable
    
    // this client will serve for 
    // TRAPD(err, GetCorrectClientL(TUid::Uid(KRemConCoreApiUid), ERemConCoreApiPlay, iTargetsForAddressing, EFalse));
    // as first rule is foreground, so this client should be the test process its self
    
    //14.1
    err = tspIf5->RegisterLocalAddressedClientObserver(TUid::Uid(300));
    if( err!=KErrNone)
        {
        stifLog->Log(_L("test case failed: RegisterLocalAddressedClientObserver return error = %d\n"), err);
        console->Printf(_L("test case failed: RegisterLocalAddressedClientObserver return error = %d\n"), err);
        return err;
        }
    //14.2
    mockObserver->SetResponse(KErrGeneral);
    tspIf5->TargetClientAvailable(thisClient);    
    if(mockObserver->Response() == KErrGeneral)
        {
        stifLog->Log(_L("test case 14.2 failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case 14.2 failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    //14.3
    mockObserver->SetResponse(KErrGeneral);
    tspIf5->TargetClientAvailable(thisClient);
    if(mockObserver->Response() != KErrGeneral)
        {
        stifLog->Log(_L("test case 14.3 failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case 14.3 failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    //14.4
    mockObserver->SetResponse(KErrGeneral);
    tspIf5->TargetClientUnavailable(thisClient);    
    if(mockObserver->Response() != KErrGeneral)
        {
        stifLog->Log(_L("test case 14.4 failed: MRemConTargetSelectorPluginObserver was not called\n"));
        console->Printf(_L("test case 14.4 failed: MRemConTargetSelectorPluginObserver was not called\n"));
        return KErrGeneral;
        }
    //14.5
    err = tspIf5->UnregisterLocalAddressedClientObserver(TUid::Uid(300));
    if( err != KErrNone)
        {
        stifLog->Log(_L("test case failed: UnregisterLocalAddressedClientObserver return error = %d\n"), err);
        console->Printf(_L("test case failed: UnregisterLocalAddressedClientObserver return error = %d\n"), err);
        return err;
        }
    
    stifLog->Log(KTTestNumberPassed, 14);
    console->Printf(KTTestNumberPassed,14);
    return KErrNone;
    }


LOCAL_C void MainL()
    {
    //
    // add your program code here, example code below
    //
    // create stif logger
    stifLog = CStifLogger::NewL( KutremcontspcontrollerLogPath, 
                          KutremcontspcontrollerLogFile);
    _LIT( KLogStart, "utremcontspcontroller logging starts!" );
    stifLog->Log( KLogStart );
    TInt err;
    TRAP(err, TestSetupL());
    if( err==KErrNone )
        {
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test01());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test02());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test03());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test04());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test05());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test06());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test07());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test08());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test09());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test10());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test11());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test12());
        RUN_TEST_CASE(testCasesTotal, testCasesPassed, Test14());
        }
    TestTearDown();
    stifLog->Log(_L("=========================================\n"));
    stifLog->Log(_L("Test Run Summary:\n"));
    stifLog->Log(_L("Test cases total:  %d\n"), testCasesTotal);
    stifLog->Log(_L("Test cases Passed: %d\n"), testCasesPassed);    
    stifLog->Log(_L("=========================================\n"));
    delete stifLog;
    }

LOCAL_C void DoStartL()
    {
    // Create active scheduler (to run active objects)
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    MainL();

    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    }

//  Global Functions

GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();

    // Create output console
    TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
            KConsFullScreen, KConsFullScreen)));
    if (createError)
        return createError;

    // Run application code inside TRAP harness, wait keypress when terminated
    TRAPD(mainError, DoStartL());
    if (mainError)
        console->Printf(KTextFailed, mainError);
    console->Printf(KTextPressAnyKey);
    //console->Getch();

    delete console;
    delete cleanup;
    __UHEAP_MARKEND;
    return KErrNone;
    }

