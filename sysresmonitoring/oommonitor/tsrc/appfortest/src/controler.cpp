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
* Description: This file create two active object.
*
*/

#include <apgcli.H>
#include <apgtask.h>
#include <apgwgnam.H>
#include <coedef.h>

#include "controler.h"
#include "eventmonitor.h"
#include "redrawmonitor.h"

CControler::CControler()
    {
    
    }

CControler::~CControler()
    {
    if(NULL!=iRedrawMonitor)
        {
        delete iRedrawMonitor;
        }
    
    if(NULL!=iEventMonitor)
        {
        delete iEventMonitor;
        }
    
    if(NULL!=iWindow)
        {
        iWindow->Close();
        delete iWindow;
        }
    
    iWindowGroup.Close();
    
    if(NULL!=iWindowGc)
        {       
        delete iWindowGc;
        }
    
    if(NULL!=iScreenDevice)
        {
        iScreenDevice->ReleaseFont(iFont);
        delete iScreenDevice;
        }
    
    iWsSession.Close();
    
    if(NULL!=iActiveScheduler)
        {      
        delete iActiveScheduler;
        }
    }

CControler* CControler::NewL()
    {
    CControler* self = new (ELeave) CControler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CControler::ExecuteD()
    {
    CActiveScheduler::Start();
    if(NULL!=this)
        {
        delete this;    // delete this object after scheduler stopped.
        }
    }

void CControler::Exit()
    {
    CActiveScheduler::Stop();
    }

void CControler::ConstructL()
    {
    iActiveScheduler = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(iActiveScheduler);

    User::LeaveIfError(iWsSession.Connect());
    iWsSession.ComputeMode(RWsSession::EPriorityControlDisabled);
    iScreenDevice = new (ELeave) CWsScreenDevice(iWsSession);
    User::LeaveIfError(iScreenDevice->Construct());
    iScreenDevice->SetScreenModeEnforcement(ESizeEnforcementNone);
    iWindowGroup = RWindowGroup(iWsSession);
    User::LeaveIfError(iWindowGroup.Construct((TUint32) this, ETrue));
    // This window group cannot receive keyboard focus
    iWindowGroup.EnableReceiptOfFocus(EFalse);
    
    // Create graphics context for the screen device
    User::LeaveIfError(iScreenDevice->CreateContext(iWindowGc));
       
    // font
    TFontSpec myFontSpec(_L("Series 60 Sans Regular"), 100);
    iScreenDevice->GetNearestFontInTwips(iFont, myFontSpec);
        
    // Finally, create a window, set its properties, and display it
    iWindow = new (ELeave) RWindow(iWsSession);
    User::LeaveIfError(iWindow->Construct(iWindowGroup, (TUint32) iWindow));
    iWindow->SetExtent(TPoint(100, 10), TSize(100, 100));
    //iWindow->SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront + 1);
    iWindow->SetCornerType(EWindowCorner5);
    iWindow->SetNonFading(ETrue);
    iWindow->SetVisible(ETrue);
    iWindow->Activate();

    iRedrawMonitor = CRedrawMonitor::NewL(*this, iWsSession, iWindowGroup);
    iEventMonitor = CEventMonitor::NewL(*this,iWsSession, iWindowGroup);
    iEventMonitor->CaptureKeyL();
    }

void CControler::Paint(TUint aHandle, const TRect&)
    {
    RWindow* aWindow = (RWindow*)aHandle;
    
    iWindowGc->Activate(*aWindow);
    aWindow->BeginRedraw();
    iWindowGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
    iWindowGc->SetBrushColor(TRgb(255, 0, 0));
    iWindowGc->Clear();
    aWindow->EndRedraw();
    iWindowGc->Deactivate();
    iWsSession.Flush();  
    }

void CControler::HandleChangeForeGroundL()
    {

    }

TBool CControler::HandleKeyEventL(const TKeyEvent& aKey, TEventCode aCode)
    {
    if(aCode == EEventKey && aKey.iCode == EKeyBackspace)
        {
        Exit();
        }
    
    return EFalse;
    }

TUid CControler::GetForeGroundAppUid()
    {
    TInt wgid = iWsSession.GetFocusWindowGroup();
    CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iWsSession, wgid);
    TUid aCurAppUid = gn->AppUid();
    CleanupStack::PopAndDestroy(gn);
    return aCurAppUid;
    }
