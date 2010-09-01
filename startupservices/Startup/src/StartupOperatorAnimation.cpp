/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This class is the container class of the CStartupOperatorAnimation.
*     Is used to show operator animation.
*
*/


// INCLUDE FILES
#include <aknappui.h>
#include <aknnotewrappers.h>
#include <barsread.h> //use of TResourceReader
#include <aknbitmapanimation.h>
#include "StartupOperatorAnimation.h"
#include <Startup.rsg>
#include "StartupDefines.h"
#include "Startup.hrh"
#include "StartupAppUi.h"
#include <operatoranimation.rsg>
#include <ConeResLoader.h>

// CONSTANTS

// Path to operator variated animation
_LIT( KOperatorAnimationResource, "z:operatoranimation.rsc" );

//Constants used in OfferKeyEventL
const TInt KTimerDelay( 10000);
const TInt KTimerInterval( 10000);

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CStartupOperatorAnimation::ConstructL(const TRect& aRect)
// Symbian 2nd phase constructor can leave.
// Need different ConstructL from base class CStartupWelcomeAnimation
// because different animation file and time are loaded.
// ---------------------------------------------------------------------------
void CStartupOperatorAnimation::ConstructL(const TRect& /*aRect*/)
    {
    TRACES("CStartupOperatorAnimation::ConstructL()");
    UpdateDrawInfo( EStartupDIStart );
    CreateWindowL();
    iAnimCancelTimer = CPeriodic::NewL( EPriorityNormal );

    TRACES("CStartupOperatorAnimation::ConstructL(): Animation loading started");
    iAnim = CAknBitmapAnimation::NewL();
    iAnim->ExcludeAnimationFramesFromCache();
    iAnim->SetContainerWindowL( *this );
    iAnim->SetScaleModeForAnimationFrames(EAspectRatioPreservedAndUnusedSpaceRemoved);
    TResourceReader rr;
    RConeResourceLoader loader( *CEikonEnv::Static() );

    TParse* fp = new(ELeave) TParse(); 
    fp->Set(KOperatorAnimationResource, &KDC_APP_RESOURCE_DIR, NULL);
    TRACES1("CStartupOperatorAnimation::ConstructL(): Operator animation resource path: '%S'", &fp->FullName());
    TFileName name( fp->FullName() );
    delete fp;

    TInt fileError = loader.Open( name );
    if ( fileError == KErrNone )
        {
        CleanupClosePushL( loader );
        iCoeEnv->CreateResourceReaderLC(rr, R_OPERATOR_IMAGE);
        TRAPD(err, iAnim->ConstructFromResourceL( rr ));
        TRACES1("CStartupOperatorAnimation::ConstructL(): Operator animation: err = %d", err);
        if( err == KErrNone )
            {
            TResourceReader timeReader;
            iCoeEnv->CreateResourceReaderLC(timeReader, R_ANIM_DURATION);
            iShowingTime = timeReader.ReadInt16();
            TRACES1("CStartupOperatorAnimation::ConstructL(): Operator animation showing time: %d", iShowingTime );
            CleanupStack::PopAndDestroy(); // pop timeReader
            }
        else
            {
            iShowingTime = 0;
            TRACES("CStartupOperatorAnimation::ConstructL(): Animation loading failed");
            }
        CleanupStack::PopAndDestroy(); //pop rr
        TRACES("CStartupOperatorAnimation::ConstructL(): Animation loading ended");
        CleanupStack::PopAndDestroy(); //pop loader
        }
    else
        {
        TRACES("CStartupOperatorAnimation::ConstructL(): Resource file loading failed");
        }

    
    SetRect(iAvkonAppUi->ApplicationRect());
    iAnim->SetPosition( TPoint( (iAvkonAppUi->ApplicationRect().Width()/2) - (iAnim->BitmapAnimData()->Size().iWidth/2), 
                                (iAvkonAppUi->ApplicationRect().Height()/2) - (iAnim->BitmapAnimData()->Size().iHeight/2) ) );
    ActivateL();

    TRACES("CStartupOperatorAnimation::ConstructL(): End");
    }

// -----------------------------------------------------------------------------
// CStartupOperatorAnimation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CStartupOperatorAnimation* CStartupOperatorAnimation::NewL( CStartupAppUi* aStartupAppUi,
                                                            const TRect& aRect)
    {
    TRACES("CStartupOperatorAnimation::NewL()");
    CStartupOperatorAnimation* self = new (ELeave) CStartupOperatorAnimation( aStartupAppUi );
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CStartupOperatorAnimation::CStartupOperatorAnimation()
// ---------------------------------------------------------
CStartupOperatorAnimation::CStartupOperatorAnimation( CStartupAppUi* aStartupAppUi ) : 
    CStartupWelcomeAnimation(aStartupAppUi)
    { 
    TRACES("CStartupOperatorAnimation::CStartupOperatorAnimation()");
    iShowingTime = 0;
    }

// ---------------------------------------------------------------------------
// CStartupOperatorAnimation::OfferKeyEventL(...)
// ---------------------------------------------------------------------------
TKeyResponse CStartupOperatorAnimation::OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/, TEventCode /*aType*/)
    {
    TRACES("CStartupWelcomeAnimation::OfferKeyEventL()");
    if( iAnimationShowing && !iStartupAppUi->HiddenReset() && !iAnimationCancelled )
        {
        // Cancel animation
        UpdateDrawInfo( EStartupDIOperatorAnimCancelled );
        EndAnimation();
        TRACES("CStartupWelcomeAnimation::OfferKeyEventL(): Timer activated - before");
        iAnimCancelTimer->Start( KTimerDelay, KTimerInterval,
            TCallBack( iStartupAppUi->DoStopTimingL, iStartupAppUi ) );
        TRACES("CStartupOperatorAnimation::OfferKeyEventL(): Timer activated - after");
        iAnimationCancelled = ETrue;
        }
    else if( !iAnimationShowing && iStartupAppUi->OperatorTonePlaying())
        {
        TRACES("CStartupOperatorAnimation::OfferKeyEventL() Animation has completed but tone is still playing. Stop it.");
        iStartupAppUi->StopOperatorTone();
        }

    TRACES("CStartupOperatorAnimation::OfferKeyEventL(): End");
    return EKeyWasConsumed;
    }

// ---------------------------------------------------------------------------
// CStartupOperatorAnimation::DoDrawing()
// ---------------------------------------------------------------------------
void CStartupOperatorAnimation::DoDrawing() const
    {
//    These are the cases handled in this function
//    EStartupDIOperatorAnimStart,
//    EStartupDIOperatorAnimCancelled,
//    EStartupDIOperatorAnimEnd
//    EStartupDISystemFatalError
    TRACES("CStartupOperatorAnimation::DoDrawing()");
    TRACES1("CStartupOperatorAnimation::DoDrawing():  %d ", iDrawUpdateInfo );
    switch ( iDrawUpdateInfo )
        {

        case EStartupDIOperatorAnimStart:
            {
            TRACES("CStartupOperatorAnimation::DoDrawing(): EStartupDIOperatorAnimStart");
            }
            break;
        case EStartupDIOperatorAnimCancelled:
            {
            TRACES("CStartupOperatorAnimation::DoDrawing(): EStartupDIOperatorAnimCancelled");
            EndAnimation();
            TRAPD(err,iStartupAppUi->StopTimingL());
            if (err != KErrNone)
                {
                TRACES1("CStartupOperatorAnimation::DoDrawing(): StopTimingL() leaves, err = %d", err );
                }
            }
            break;
        case EStartupDIOperatorAnimEnd:
            {
            TRACES("CStartupOperatorAnimation::DoDrawing(): EStartupDIOperatorAnimEnd");
            }
            break;
        case EStartupDISystemFatalError:
            {
            }
            break;
        default:
            break;
        }
    }
