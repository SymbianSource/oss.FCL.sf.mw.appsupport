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
* Description: Main program.
*
*/

#include "controler.h"

void ExeMainL()
    {
    CControler::NewL()->ExecuteD();
    }

void RegisterWsExe(const TDesC&);

GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    
    TRAPD(error, ExeMainL());
    __ASSERT_ALWAYS(!error, User::Panic(_L("EXEUI"), error));
 
    delete cleanup;
    __UHEAP_MARKEND;
 
    return 0;
    }
