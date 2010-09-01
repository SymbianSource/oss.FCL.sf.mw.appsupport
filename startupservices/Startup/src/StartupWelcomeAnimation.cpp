/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This class is the container class of the CStartupWelcomeAnimation.
*     Is used to show welcome animation.
*
*/


// INCLUDE FILES
#include <aknappui.h>
#include <aknnotewrappers.h>
#include <barsread.h> //use of TResourceReader
#include <StarterClient.h>     //used for RemoveSplashScreen
#include <aknbitmapanimation.h>
#include <startup.mbg>
#include "StartupWelcomeAnimation.h"
#include <Startup.rsg>
#include "StartupDefines.h"
#include "Startup.hrh"
#include "StartupAppUi.h"
#include <akniconutils.h>

// CONSTANTS
const TInt KStartupAnimationShowingDuration( 4200 );//4.2 sec

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::ConstructL(const TRect& aRect)
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::ConstructL(const TRect& /*aRect*/)
    {
    TRACES("CStartupWelcomeAnimation::ConstructL()");
    iShowingTime = KStartupAnimationShowingDuration;
    UpdateDrawInfo( EStartupDIStart );

    iAvkonAppUi->StatusPane()->MakeVisible(EFalse);
    CreateWindowL();
    iAnimCancelTimer = CPeriodic::NewL( EPriorityNormal );

    TRACES("CStartupWelcomeAnimation::ConstructL(): animation loading started");
    iAnim = CAknBitmapAnimation::NewL();
    iAnim->ExcludeAnimationFramesFromCache();
    iAnim->SetContainerWindowL( *this );
    iAnim->SetScaleModeForAnimationFrames(EAspectRatioPreservedAndUnusedSpaceRemoved);
    TResourceReader rr;
    iCoeEnv->CreateResourceReaderLC(rr, R_ANIM_IMAGE);
    TRAPD(err, iAnim->ConstructFromResourceL( rr ))
    if (err != KErrNone)
        {
        TRACES1("CStartupWelcomeAnimation::ConstructL(): ConstructFromResourceL() leaves, err = %d", err );
        }
    CleanupStack::PopAndDestroy();

    SetRect(iAvkonAppUi->ApplicationRect());  // Results in a call to SizeChanged()
    ActivateL();

    TRACES("CStartupWelcomeAnimation::ConstructL(): animation loading ended");
    TRACES("CStartupWelcomeAnimation::ConstructL(): End");
    }

// -----------------------------------------------------------------------------
// CStartupWelcomeAnimation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CStartupWelcomeAnimation* CStartupWelcomeAnimation::NewL( CStartupAppUi* aStartupAppUi,
                                                          const TRect& aRect)
    {
    TRACES("CStartupWelcomeAnimation::NewL()");
    CStartupWelcomeAnimation* self = new (ELeave) CStartupWelcomeAnimation( aStartupAppUi );
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    TRACES("CStartupWelcomeAnimation::NewL(): End");
    return self;
    }

// ---------------------------------------------------------
// CStartupWelcomeAnimation::CStartupWelcomeAnimation()
// ---------------------------------------------------------
CStartupWelcomeAnimation::CStartupWelcomeAnimation( CStartupAppUi* aStartupAppUi ) :
    iBackgroundBitmap( NULL ),
    iSplashScreenRemoved( EFalse ),
    iStartupAppUi( aStartupAppUi ),
    iAnimationShowing( EFalse ),
    iAnimationCancelled ( EFalse )
    {
    TRACES("CStartupWelcomeAnimation::CStartupWelcomeAnimation()");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::StartL()
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::StartL()
    {
    TRACES("CStartupWelcomeAnimation::StartL()");
    iAnim->StartAnimationL();
    ControlEnv()->WsSession().Flush(); // force draw of the context
    TRACES("CStartupWelcomeAnimation::StartL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::UpdateDrawInfo( TStartupDrawInfo aValue )
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::UpdateDrawInfo( TStartupDrawInfo aValue )
    {
    TRACES("CStartupWelcomeAnimation::UpdateDrawInfo()");
    TRACES1("CStartupWelcomeAnimation::UpdateDrawInfo(): Value %d", aValue);
    //Prevent state change if already EStartupDISystemFatalError
    if( iDrawUpdateInfo == EStartupDISystemFatalError )
        {
        TRACES("CStartupWelcomeAnimation::UpdateDrawInfo(): End, preventing state change - EStartupDISystemFatalError");
        return;
        }

    iDrawUpdateInfo = aValue;
    TRACES("CStartupWelcomeAnimation::UpdateDrawInfo(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::DoDrawingL()
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::DoDrawingL() const
    {
    TRACES("CStartupWelcomeAnimation::DoDrawingL()");
    switch ( iDrawUpdateInfo )
        {
        case EStartupDIStart:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIStart");
            DrawBlankScreen(); // clears screen after code query emergency call
            }
            break;
        case EStartupDITouchScreenCalib:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDITouchScreenCalib");
            RemoveSplashScreen();
            DrawBlankScreen();
            }
            break;
        case EStartupDICharging:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDICharging");
            RemoveSplashScreen();
            DrawBlankScreen();
            }
            break;
        case EStartupDIAlarm:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIAlarm");
            RemoveSplashScreen();
            DrawBlankScreen();
            }
            break;
        case EStartupDIHiddenReset:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIHiddenReset");
            RemoveSplashScreen();
            DrawBlankScreen();            
            }
            break;
        case EStartupDIQueriesOn:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIQueriesOn");
            RemoveSplashScreen();
            DrawBlankScreen();
            }
            break;
        case EStartupDIQueriesOff:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIQueriesOff");
            }
            break;
        case EStartupDIWelcomeAnimStart:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIWelcomeAnimStart");
			RemoveSplashScreen();
			DrawBlankScreen();
            }
            break;
        case EStartupDIWelcomeAnimCancelled:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIWelcomeAnimCancelled");
            }
            break;
        case EStartupDIWelcomeAnimEnd:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIWelcomeAnimEnd");
            DrawBlankScreen();
            }
            break;
        case EStartupDIOperatorAnimEnd:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIOperatorAnimEnd");
            DrawBlankScreen();
            }
            break;
        case EStartupDICityTimeDateQueries:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDICityTimeDateQueries");
            DrawBlankScreen();
            }
            break;
        case EStartupDIEnd:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDIEnd");
            }
            break;
        case EStartupDISystemFatalError:
            {
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): EStartupDISystemFatalError");
            }
            break;
        default:
            TRACES("CStartupWelcomeAnimation::DoDrawingL(): default");
            break;
        }
    TRACES("CStartupWelcomeAnimation::DoDrawingL(): End");
    }

// ---------------------------------------------------------
// CStartupWelcomeAnimation::RemoveSplashScreen()
// ---------------------------------------------------------
void CStartupWelcomeAnimation::RemoveSplashScreen() const
    {
    TRACES("CStartupWelcomeAnimation::RemoveSplashScreen()");
    //Remove SplashScreen
    if( !iSplashScreenRemoved )
        {
        TRACES("CStartupWelcomeAnimation::RemoveSplashScreen(): Connect to Starter");
        RStarterSession startersession;
        if( startersession.Connect() == KErrNone )
            {
            TRACES("CStartupWelcomeAnimation::RemoveSplashScreen(): Connected to Starter");
            startersession.EndSplashScreen();
            TRACES("CStartupWelcomeAnimation::RemoveSplashScreen(): Splash screen removed");
            startersession.Close();
            }
        }
    TRACES("CStartupWelcomeAnimation::RemoveSplashScreen(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::~CStartupWelcomeAnimation()
// ---------------------------------------------------------------------------
CStartupWelcomeAnimation::~CStartupWelcomeAnimation()
    {
    TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation()");

    if( iAnim )
        {
        if( iAnimationShowing )
            {
            iAnim->CancelAnimation();
            TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation(): Animation cancelled");
            }
        }
    delete iAnim;
    TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation(): iAnim deleted");

    if (iBackgroundBitmap)
        {
        delete iBackgroundBitmap;
        TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation(): iBackgroundBitmap deleted");
        }

    if( iAnimCancelTimer )
        {
        iAnimCancelTimer->Cancel();
        TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation(): Timer cancelled");
        }
    delete iAnimCancelTimer;
    TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation(): iAnimCancelTimer deleted");

    TRACES("CStartupWelcomeAnimation::~CStartupWelcomeAnimation(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::ComponentControl(TInt aIndex)
// ---------------------------------------------------------------------------
CCoeControl* CStartupWelcomeAnimation::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            {
            return iAnim;
            }
        default:
            {
            return NULL;
            }
        }
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::CountComponentControls()
// ---------------------------------------------------------------------------
TInt CStartupWelcomeAnimation::CountComponentControls() const
    {
    return iAnim ? 1 : 0; // return nbr of controls inside this container
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::Draw(const TRect& aRect) const
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::Draw(const TRect& /*aRect*/ ) const
    {
    TRACES("CStartupWelcomeAnimation::Draw()");
    TRAPD(err, DoDrawingL());
    if (err != KErrNone)
        {
        TRACES1("CStartupWelcomeAnimation::Draw(): DoDrawingL() leaves, err = %d", err );
        }
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::DrawBlankScreen() const
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::DrawBlankScreen() const
    {
    TRACES("CStartupWelcomeAnimation::DrawBlankScreen()");
    CWindowGc& gc = SystemGc();
    TRect rect = Rect();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbWhite);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(rect);
    ControlEnv()->WsSession().Flush(); // force draw of the context
    TRACES("CStartupWelcomeAnimation::DrawBlankScreen(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::ShowingTime()
// ---------------------------------------------------------------------------
TInt CStartupWelcomeAnimation::ShowingTime()
    {
    TRACES1("CStartupWelcomeAnimation::ShowingTime(): time = %d", iShowingTime);
    return iShowingTime;
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::SizeChanged()
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::SizeChanged()
    {
    if( iAnim )
        {
        iAnim->SetRect(Rect());
        iAnim->SetPosition( TPoint(
            (iAvkonAppUi->ApplicationRect().Width()/2) - (iAnim->BitmapAnimData()->Size().iWidth/2), 
            (iAvkonAppUi->ApplicationRect().Height()/2) - (iAnim->BitmapAnimData()->Size().iHeight/2)
            ) );
        }
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::EndAnimation()
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::EndAnimation() const
    {
    TRACES("CStartupWelcomeAnimation::EndAnimation()");
    if( iAnim )
        {
        TRACES("CStartupWelcomeAnimation::EndAnimation(): Cancel animation");
        iAnim->CancelAnimation();
        }
    TRACES("CStartupWelcomeAnimation::EndAnimation(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::HandleControlEventL(...)
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
    {
    //pure virtual from MCoeControlObserver
    TRACES("CStartupWelcomeAnimation::HandleControlEventL()");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::HandlePointerEventL(...)
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    TRACES("CStartupWelcomeAnimation::HandlePointerEventL()");
    if (AknLayoutUtils::PenEnabled())
        {
        TRACES1("CStartupWelcomeAnimation::HandlePointerEventL: aPointerEvent.iType == %d",aPointerEvent.iType );
        switch (aPointerEvent.iType)
            {
            case TPointerEvent::EButton1Down:
                CancelAnimation();
                break;                    

            default:
                break;
            }
        }
    TRACES("CStartupWelcomeAnimation::HandlePointerEventL(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::OfferKeyEventL(...)
// ---------------------------------------------------------------------------
TKeyResponse CStartupWelcomeAnimation::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode /*aType*/ )
    {
    TRACES("CStartupWelcomeAnimation::OfferKeyEventL()");
    TRACES1("CStartupWelcomeAnimation::OfferKeyEventL(): aKeyEvent.iCode == %d",aKeyEvent.iCode);
    TRACES1("CStartupWelcomeAnimation::OfferKeyEventL(): aKeyEvent.iScanCode == %d",aKeyEvent.iScanCode);
    TRACES1("CStartupWelcomeAnimation::OfferKeyEventL(): iAnimationShowing == %d",iAnimationShowing);
    TRACES1("CStartupWelcomeAnimation::OfferKeyEventL(): iAnimationCancelled == %d",iAnimationCancelled);
    
    if ( iStartupAppUi->GetOfflineModeQueryShown() )
        {
        TRACES("CStartupWelcomeAnimation::OfferKeyEventL(): Key event from offline mode query");
        // first key event comes from Offline Mode Query
        iStartupAppUi->SetOfflineModeQueryShown( EFalse );
        }
    else
        {
        if( iAnimationShowing && !iStartupAppUi->HiddenReset() && !iAnimationCancelled )
            {
            if (aKeyEvent.iScanCode == EStdKeyNkpAsterisk || aKeyEvent.iScanCode == 0x2a)
                {               
                TRACES("CStartupWelcomeAnimation::OfferKeyEventL(): Set clean boot");
                iStartupAppUi->SetCleanBoot();                
                }        
            // Cancel animation
            CancelAnimation();
            }
        else if( !iAnimationShowing && iStartupAppUi->StartupTonePlaying())
            {
            TRACES("CStartupWelcomeAnimation::OfferKeyEventL() Animation has completed but tone is still playing. Stop it.");
            iStartupAppUi->StopStartupTone();
            }
        }
    
    TRACES("CStartupWelcomeAnimation::OfferKeyEventL(): End");
    return EKeyWasConsumed;
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::CancelAnimCancelTimer()
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::CancelAnimCancelTimer()
    {
    TRACES("CStartupWelcomeAnimation::CancelAnimCancelTimer()");
    iAnimCancelTimer->Cancel();
    TRACES("CStartupWelcomeAnimation::CancelAnimCancelTimer(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::CancelAnimation(...)
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::CancelAnimation()
    {
    TRACES("CStartupWelcomeAnimation::CancelAnimation()");
    if( iAnimationShowing && !iStartupAppUi->HiddenReset() && !iAnimationCancelled )
        {
        UpdateDrawInfo( EStartupDIWelcomeAnimCancelled );
        EndAnimation();
        TRACES("CStartupWelcomeAnimation::CancelAnimation(): Timer activated - before");
        iAnimCancelTimer->Start( 10000, 10000, 
                           TCallBack( iStartupAppUi->DoStopTimingL, iStartupAppUi ) );
        TRACES("CStartupWelcomeAnimation::CancelAnimation(): Timer activated - after");
        iAnimationCancelled = ETrue;
        }
    TRACES("CStartupWelcomeAnimation::CancelAnimation(): End");
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::IsAnimationCancelled(...)
// ---------------------------------------------------------------------------
TBool CStartupWelcomeAnimation::IsAnimationCancelled()
    {
    TRACES1("CStartupWelcomeAnimation::IsAnimationCancelled(): iAnimationCancelled: %d", iAnimationCancelled);
    return iAnimationCancelled;
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::SetAnimationShowing(...)
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::SetAnimationShowing(TBool aValue)
    {
    TRACES("CStartupWelcomeAnimation::SetAnimationShowing()");
    TRACES2("CStartupWelcomeAnimation::SetAnimationShowing(): iAnimationShowing changed from %d to %d",iAnimationShowing, aValue );
    iAnimationShowing = aValue;
    }

// ---------------------------------------------------------------------------
// CStartupWelcomeAnimation::HandleResourceChange(...)
// ---------------------------------------------------------------------------
void CStartupWelcomeAnimation::HandleResourceChange(TInt aType)
    {
    CCoeControl::HandleResourceChange(aType);
    if(aType==KEikDynamicLayoutVariantSwitch)
        {
        SetRect(iAvkonAppUi->ApplicationRect()); // update rect
        }
   }



//  End of File  
