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
* Description:  Logging functionality for OOM monitor profiling.
*
*/

#ifdef _DEBUG

#include <hal.h>
#include <w32std.h>
#include <e32std.h>
#include <e32hal.h>
#include <apgwgnam.h>
#include <flogger.h>

#include "oomlog.h"
#include "oompanic.h"

#include "globaldata.h"
#include "diclog.h"

COomLogger* COomLogger::NewL(RWsSession& aWs, RFs& aFs)
    {
    COomLogger* self = new (ELeave) COomLogger(aWs, aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
void COomLogger::StartL()
    {
    TBuf<KIntValue8> buf(KOomLoggerStartL);
    DicLog::WriteLog(TUid::Uid(0x0013), buf);
    }

void COomLogger::RunL()
    {

    }

void COomLogger::DoCancel()
    {

    }

COomLogger::~COomLogger()
    {

    }

void COomLogger::Write(const TDesC8& aBuffer)
    {

    }

void COomLogger::LogApplicationIds()
    {

    }

void COomLogger::LogFreeMemory()
    {

    }

COomLogger::COomLogger(RWsSession& aWs, RFs& aFs) : CTimer(EPriorityStandard), iWs(aWs), iFs(aFs)
    {
    
    }

void COomLogger::ConstructL()
    {

    }

void COomLogger::ColapseWindowGroupTree()
    {

    }

TUid COomLogger::GetUidFromWindowGroupId(TInt aWgId)
    {
    TUid uid=TUid::Uid(0);
    return uid;
    }

#endif //_DEBUG
