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
* Description: The stub for testing tsp events
*
*/

#include "ut_atsptspeventsstub.h"
#include "tsptriggerevents.h"

#include <e32property.h> 
#include <W32STD.H> 

CUt_AtspTspEventsStub* CUt_AtspTspEventsStub::NewL()
    {
    CUt_AtspTspEventsStub* self = new( ELeave ) CUt_AtspTspEventsStub();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CUt_AtspTspEventsStub::~CUt_AtspTspEventsStub()
    {
    Cancel(); 
    }

CUt_AtspTspEventsStub::CUt_AtspTspEventsStub()
    : CTimer(CActive::EPriorityStandard), iIsRunLCalled(EFalse), iIsTimerElapsed(EFalse)
    {
    }

void CUt_AtspTspEventsStub::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }

void CUt_AtspTspEventsStub::IssueAudioPropertyL()
    {
    _LIT(KAppName, "ut_atspaudioproperty.exe");
    RProcess process;
    TInt error = process.Create(KAppName, KNullDesC);
    User::LeaveIfError(error);
    CleanupClosePushL(process);
    TRequestStatus status;
    process.Rendezvous(status);
    process.Resume();
    User::WaitForRequest(status);
    User::LeaveIfError(status.Int());
    
    CleanupStack::PopAndDestroy();
    }

void CUt_AtspTspEventsStub::IssueTspPropertyL()
    {
    const TUid KTspClientMapperProperty = { 0x10200C70 };
    const TUint32 KTspClientMapperKeyPlaying = 0x00000001;
    
    RProperty myProperty;
    CleanupClosePushL(myProperty);
    // The value of the property 
    TUint16 array[4] = {10,20,30,40};
    TBuf16<10> buf;
    buf.Copy(&array[0],sizeof(array));

    // Attach and set the property
    User::LeaveIfError(myProperty.Attach(KTspClientMapperProperty, KTspClientMapperKeyPlaying, EOwnerThread));
    User::LeaveIfError(myProperty.Set(buf));

    // CLose the property
    CleanupStack::PopAndDestroy();
    }

void CUt_AtspTspEventsStub::IssueWsEventL(const TInt aType)
    {
    RWsSession  ws;
    User::LeaveIfError(ws.Connect());
    CleanupClosePushL(ws);

    TWsEvent   event;
    event.SetType(aType);
    ws.SendEventToAllWindowGroups(event);
    
    ws.Finish();
    CleanupStack::PopAndDestroy(&ws);
    }

void CUt_AtspTspEventsStub::MtrtoEvaluateRoutingRules()
    {
    iIsRunLCalled = ETrue;
    CActiveScheduler::Stop();
    }

void CUt_AtspTspEventsStub::RunL()
    {
    iIsTimerElapsed = ETrue;
    CActiveScheduler::Stop();
    }

void CUt_AtspTspEventsStub::DoCancel()
    {
    CTimer::DoCancel(); 
    }
