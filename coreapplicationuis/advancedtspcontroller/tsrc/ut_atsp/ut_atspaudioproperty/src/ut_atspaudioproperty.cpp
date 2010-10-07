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
* Description: The test application to issue audio property with specified sid.
*
*/

//  Include Files  

#include "ut_atspaudioproperty.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console

#include <e32property.h> 
#include <AudioClientsListPSKeys.h>

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


//  Local Functions

LOCAL_C void MainL()
    {
    RProperty myProperty;
    CleanupClosePushL(myProperty);
    // The value of the property 
    TUint16 array[4] = {10,20,30,40};
    TBuf16<10> buf;
    buf.Copy(&array[0],sizeof(array));

    // Attach and set the property
    User::LeaveIfError(myProperty.Attach(KPSUidMMFAudioServer, KAudioPolicyApplicationAudioStatePlaying, EOwnerThread));
    User::LeaveIfError(myProperty.Set(buf));

    // CLose the property
    CleanupStack::PopAndDestroy();  
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
        {
        delete cleanup;
        return createError;
        }

    // Run application code inside TRAP harness, wait keypress when terminated
    TRAPD(mainError, DoStartL());
    if (mainError)
        console->Printf(KTextFailed, mainError);

    delete console;
    delete cleanup;
    __UHEAP_MARKEND;
    return KErrNone;
    }

