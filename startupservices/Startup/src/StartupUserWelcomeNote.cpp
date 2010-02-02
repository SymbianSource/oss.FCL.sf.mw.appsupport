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
*     This class is the container class of the CStartupUerWelcomeNote.
*     Is user for showing user selected picture, text or predefined animation.
*
*/



// INCLUDE FILES
#include <aknappui.h>
#include <coemain.h>
#include <aknnotewrappers.h>
#include <AknGlobalNote.h>
#include <barsread.h> //use of TResourceReader
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
#include "startupview.h"
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <StarterClient.h>     //used for RemoveSplashScreen
#include <startup.mbg>
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
#include <e32const.h>
#include "StartupUserWelcomeNote.h"
#include <startup.rsg>
#include <centralrepository.h>
#include <startupdomaincrkeys.h>
#include "StartupDefines.h"
#include "startup.hrh"
#include "StartupAppUi.h"
#include "aknSDData.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CStartupUseWelcomeNote::ConstructL
//
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::ConstructL( const TRect& /*aRect*/ )
    {
    TRACES("CStartupUserWelcomeNote::ConstructL()");

    iAvkonAppUi->StatusPane()->MakeVisible( EFalse );

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION

    SetContainerWindowL( iView );
    iView.SetComponent( *this );

#else // RD_STARTUP_ANIMATION_CUSTOMIZATION

    CreateWindowL();
    iNoteCancelTimer = CPeriodic::NewL( EPriorityNormal );

#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    iBitmap = new(ELeave) CFbsBitmap();

    //get user selected userwelcomenotetype from Central Repository
    TInt err = GetUserWelcomeNoteTypeInfo();

    if( err != KErrNone )
        {
        TRACES("CStartupUserWelcomeNote::ConstructL(): Show default note");
        //in error case default uwn (no note) is shown
        iNoteType = EDefaultWelcomeNote;
        iNoteDefaultVariationType = EStartupUWNDefaultNoNote;
        }
    //Do some preparations for showing user welcome note later
    //This makes the starting to show much more quicker
    switch ( iNoteType )
        {
        case ETextWelcomeNote:
            {
            TRACES("CStartupUserWelcomeNote::ConstructL(): iNoteType == ETextWelcomeNote");
            }
            break;
        case EImageWelcomeNote:
            {
            TRACES("CStartupUserWelcomeNote::ConstructL(): iNoteType == EImageWelcomeNote");
            TInt errorCode = iBitmap->Load( TPtrC(iNotePath.Ptr()) );
            TRACES1("CStartupUserWelcomeNote::ConstructL(): Load returned %d", errorCode);
            if(iStartupAppUi.CoverUISupported())
                {
                SecondaryDisplay::TWelcomeImage data(TPtrC(iNotePath.Ptr()));
                SecondaryDisplay::TWelcomeImagePckg Pckg( data );
                iStartupAppUi.RaiseCoverUIEvent( SecondaryDisplay::KCatStartup,
                                                 SecondaryDisplay::EMsgWelcomeImageEvent,
                                                 Pckg);
                }
            }
            break;
        default:
            {
            TRACES("CStartupUserWelcomeNote::ConstructL(): iNoteType == EDefaultWelcomeNote");
            switch ( iNoteDefaultVariationType )
                {
                case EStartupUWNDefaultOperatorGraphic:
                    {
                    TRACES("CStartupUserWelcomeNote::ConstructL(): iNoteDefaultVariationType == EStartupUWNDefaultOperatorGraphic");
                    iBitmap->Load( TPtrC(iNoteOperPath.Ptr()) );
                    if(iStartupAppUi.CoverUISupported())
                        {
                        SecondaryDisplay::TWelcomeImage data(TPtrC(iNoteOperPath.Ptr()));
                        SecondaryDisplay::TWelcomeImagePckg Pckg( data );
                        iStartupAppUi.RaiseCoverUIEvent( SecondaryDisplay::KCatStartup,
                                                         SecondaryDisplay::EMsgWelcomeImageEvent,
                                                         Pckg);
                        }
                    }
                    break;
                case EStartupUWNDefaultOperatorText:
                case EStartupUWNDefaultNoNote:
                default:
                    {
                    //nothing preparation
                    TRACES("CStartupUserWelcomeNote::ConstructL(): iNoteDefaultVariationType == EStartupUWNDefaultOperatorText or EStartupUWNDefaultNoNote");
                    }
                    break;
                }
            }
            break;
        }
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
    //get information for draw-function
    iWelcomeNoteType = NoteTypeInformation();
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    SetRect(iAvkonAppUi->ApplicationRect());
    ActivateL();
    TRACES("CStartupUserWelcomeNote::ConstructL(): End");
    }

// -----------------------------------------------------------------------------
// CStartupUserWelcomeNote::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CStartupUserWelcomeNote* CStartupUserWelcomeNote::NewL(
    CStartupAppUi& aStartupAppUi,
    const TRect& aRect
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
  , CStartupView& aView
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    )
    {
    TRACES("CStartupUserWelcomeNote::NewL()");
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    CStartupUserWelcomeNote* self = new (ELeave) CStartupUserWelcomeNote( aStartupAppUi, aView );
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    CStartupUserWelcomeNote* self = new (ELeave) CStartupUserWelcomeNote( aStartupAppUi );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    TRACES("CStartupUserWelcomeNote::NewL(): End");
    return self;
    }

// ---------------------------------------------------------
// CStartupUserWelcomeNote::CStartupUserWelcomeNote()
// ---------------------------------------------------------
CStartupUserWelcomeNote::CStartupUserWelcomeNote( CStartupAppUi& aStartupAppUi
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
  , CStartupView& aView
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
 ) :
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iView( aView ),
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iSplashScreenRemoved( EFalse ),
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    iStartupAppUi( aStartupAppUi )
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
  , iUserWelcomeNoteShowing( EFalse ),
    iUserWelcomeNoteCancelled( EFalse)
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    {
    TRACES("CStartupUserWelcomeNote::CStartupUserWelcomeNote()");
    }

// ---------------------------------------------------------------------------
// CStartupUseWelcomeNote::StartL()
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::StartL()
    {
    TRACES("CStartupUserWelcomeNote::StartL()");
    switch ( iNoteType )
        {
        case ETextWelcomeNote:
            {
            TRACES("CStartupUserWelcomeNote::StartL(): ETextWelcomeNote");
            ShowInformationNoteWrapperL();
            }
            break;
        case EImageWelcomeNote:
            {
            TRACES("CStartupUserWelcomeNote::StartL(): EImageWelcomeNote");
            DrawImageWelcomeNote();
            }
            break;
        default:
            {
            TRACES("CStartupUserWelcomeNote::StartL(): default");
            switch ( iNoteDefaultVariationType )
                {
                case EStartupUWNDefaultOperatorGraphic:
                    {
                    TRACES("CStartupUserWelcomeNote::StartL(): EStartupUWNDefaultOperatorGraphic");
                    DrawImageWelcomeNote();
                    }
                    break;
                case EStartupUWNDefaultOperatorText:
                    {
                    TRACES("CStartupUserWelcomeNote::StartL(): EStartupUWNDefaultOperatorText");
                    ShowInformationNoteWrapperL();
                    }
                    break;
                case EStartupUWNDefaultNoNote:
                default:
                    TRACES("CStartupUserWelcomeNote::StartL(): EStartupUWNDefaultNoNote");
                    break;
                }
            }
            break;
        }
    ControlEnv()->WsSession().Flush(); // force draw of the context
    TRACES("CStartupUserWelcomeNote::StartL(): End");
    }

// ---------------------------------------------------------
// CStartupUserWelcomeNote::NoteTypeInformation()
// ---------------------------------------------------------
TStartupNoteTypeInformation CStartupUserWelcomeNote::NoteTypeInformation()
    {
    TRACES("CStartupUserWelcomeNote::NoteTypeInformation()");
    if( iNoteType == EDefaultWelcomeNote && iNoteDefaultVariationType == EStartupUWNDefaultNoNote )
        {
        TRACES("CStartupUserWelcomeNote::NoteTypeInformation(): End, return EStartupNoNote");
        return EStartupNoNote;
        }
    else if( ( iNoteType == ETextWelcomeNote ) ||
             ( iNoteType == EDefaultWelcomeNote &&
               iNoteDefaultVariationType == EStartupUWNDefaultOperatorText ) )
        {
        TRACES("CStartupUserWelcomeNote::NoteTypeInformation(): End, return EStartupText");
        return EStartupText;
        }
    else if( ( iNoteType == EImageWelcomeNote ) ||
                ( iNoteType == EDefaultWelcomeNote &&
                  iNoteDefaultVariationType == EStartupUWNDefaultOperatorGraphic ) )
        {
        TRACES("CStartupUserWelcomeNote::NoteTypeInformation(): End, return EStartupImage");
        return EStartupImage;
        }
    else
        {
        __ASSERT_DEBUG( EFalse, PANIC( EStartupNeverShouldBeHere ) );
        return EStartupNoNote;
        }
    }

// ---------------------------------------------------------------------------
// CStartupUseWelcomeNote::DrawImageWelcomeNote
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::DrawImageWelcomeNote()
    {
    TRACES("CStartupUserWelcomeNote::DrawImageWelcomeNote()");

    CWindowGc& gc = SystemGc();
    TInt xDelta=0; // for x coordinates
    TInt yDelta=0; // for y coordinates
    TSize bmpSizeInPixels = iBitmap->SizeInPixels();
    //center image to the center of the screen
    TRect rect = Rect();
    xDelta=( rect.Width() - bmpSizeInPixels.iWidth ) / 2;
    yDelta=( rect.Height() - bmpSizeInPixels.iHeight ) / 2;
    TPoint pos = TPoint( xDelta , yDelta ); // displacement vector
    ActivateGc();
    Window().Invalidate( rect );
    Window().BeginRedraw( rect );
    gc.BitBlt( pos, iBitmap ); // CWindowGc member function
    DrawUtils::ClearBetweenRects(gc, Rect(), TRect(pos,bmpSizeInPixels));
    Window().EndRedraw();
    DeactivateGc();
    TRACES("CStartupUserWelcomeNote::DrawImageWelcomeNote(): End");
    }

// ---------------------------------------------------------------------------
// CStartupUseWelcomeNote::~CStartupUserWelcomeNote()
// ---------------------------------------------------------------------------
CStartupUserWelcomeNote::~CStartupUserWelcomeNote()
    {
    TRACES("CStartupUserWelcomeNote::~CStartupUserWelcomeNote()");

    delete iBitmap;

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    iView.RemoveComponent();
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    if( iNoteCancelTimer )
        {
        iNoteCancelTimer->Cancel();
        }
    delete iNoteCancelTimer;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    TRACES("CStartupUserWelcomeNote::~CStartupUserWelcomeNote(): End");
    }

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::ComponentControl(TInt aIndex)
// ---------------------------------------------------------------------------
CCoeControl* CStartupUserWelcomeNote::ComponentControl(TInt /*aIndex*/) const
    {
    return NULL;
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::DrawBlankScreen() const
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::DrawBlankScreen() const
    {
    TRACES("CStartupUserWelcomeNote::DrawBlankScreen()");
    CWindowGc& gc = SystemGc();
    TRect rect = Rect();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbWhite);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(rect);
    ControlEnv()->WsSession().Flush(); // force draw of the context
    TRACES("CStartupUserWelcomeNote::DrawBlankScreen(): End");
    }

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::HandleControlEventL(...)
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
    {
    //pure virtual from MCoeControlObserver
    TRACES("CStartupUserWelcomeNote::HandleControlEventL()");
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::UserWelcomeNoteType()
// ---------------------------------------------------------------------------
TStartupWelcomeNoteType CStartupUserWelcomeNote::UserWelcomeNoteType()
    {
    return ( iNoteType );
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::OfferKeyEventL(...)
// ---------------------------------------------------------------------------
TKeyResponse CStartupUserWelcomeNote::OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/, TEventCode /*aType*/)
    {
    TRACES("CStartupUserWelcomeNote::OfferKeyEventL()");
    if( iUserWelcomeNoteShowing && !iStartupAppUi.HiddenReset() && !iUserWelcomeNoteCancelled )
        {
        // Cancel UWN
        TRACES("CStartupUserWelcomeNote::OfferKeyEventL(): Timer activated - before");
        iNoteCancelTimer->Start( 10000, 10000,
                           TCallBack( iStartupAppUi.DoStopTimingL, &iStartupAppUi ) );
        iUserWelcomeNoteCancelled = ETrue;
        TRACES("CStartupUserWelcomeNote::OfferKeyEventL(): Timer activated - after");
        }
    TRACES("CStartupUserWelcomeNote::OfferKeyEventL(): End");
    return EKeyWasConsumed;
    }

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::CancelNoteCancelTimer()
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::CancelNoteCancelTimer()
    {
    TRACES("CStartupUserWelcomeNote::CancelNoteCancelTimer()");
    iNoteCancelTimer->Cancel();
    TRACES("CStartupUserWelcomeNote::CancelNoteCancelTimer(): End");
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo()
// ---------------------------------------------------------------------------
TInt CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo()
    {
    TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo()");
    TInt err( KErrGeneral );

    CRepository* repository(NULL);

    TRAP( err, repository = CRepository::NewL( KCRUidStartupConf ) );
    if ( err == KErrNone )
        {
        TInt type;
        TBuf<KStartupTBufMaxLength> atext;
        TBuf<KStartupTBufMaxLength> apath;
        TBuf<KStartupTBufMaxLength> aoperatortext;
        TBuf<KStartupTBufMaxLength> aoperatorpath;

        err = repository->Get( KStartupWelcomeNoteType, type );
        TRACES2("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Welcome note type = %d, err = %d", type, err );

        err = repository->Get( KStartupWelcomeNoteText, atext );
        TRACES2("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Welcome note text = '%S', err = %d", &atext, err );

        err = repository->Get( KStartupWelcomeNoteImage, apath );
        TRACES2("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Welcome note image path = '%S', err = %d", &apath, err);

        switch (type)
            {
            case EDefaultWelcomeNote:
                {
                TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): EDefaultWelcomeNote");
                iNoteType = EDefaultWelcomeNote;

                repository->Get( KStartupOperatorNoteImage, aoperatorpath );
                TRACES1("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Operator image path: '%S'", &aoperatorpath);
                TInt opImageStatus = CheckImage(aoperatorpath);
                TRACES1("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Operator image status: %d", opImageStatus);

                repository->Get( KStartupOperatorNoteText, aoperatortext );
                TRACES1("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Operator text: '%S'", &aoperatortext);

                if ( opImageStatus == KErrNone )
                    {
                    TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Default note type is EStartupUWNDefaultOperatorGraphic");
                    iNoteDefaultVariationType = EStartupUWNDefaultOperatorGraphic;
                    }
                else if ( aoperatortext.Length() > 0 )
                    {
                    TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Default note type is EStartupUWNDefaultOperatorText");
                    iNoteDefaultVariationType = EStartupUWNDefaultOperatorText;
                    }
                else
                    {
                    TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): Default note type is EStartupUWNDefaultNoNote");
                    iNoteDefaultVariationType = EStartupUWNDefaultNoNote;
                    }
                }
                break;
            case ETextWelcomeNote:
                {
                TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): ETextWelcomeNote");
                iNoteType = ETextWelcomeNote;
                }
                break;
            case EImageWelcomeNote:
                {
                TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): EImageWelcomeNote");
                if (CheckImage(apath) != KErrNone)
                    {
                    //in error case default uwn (no note) is shown
                    iNoteType = EDefaultWelcomeNote;
                    iNoteDefaultVariationType = EStartupUWNDefaultNoNote;
                    }
                else
                    {
                    iNoteType = EImageWelcomeNote;
                    }
                }
                break;
            default:
                {
                TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): default");
                delete repository;
                TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): End, return KErrNotFound");
                return KErrNotFound;
                }
            }
        iNoteText = atext;
        iNotePath = apath;
        iNoteOperText = aoperatortext;
        iNoteOperPath = aoperatorpath;
        }

    delete repository;
    TRACES("CStartupUserWelcomeNote::GetUserWelcomeNoteTypeInfo(): End, return KErrNone");
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::ShowInformationNoteWrapperL()
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::ShowInformationNoteWrapperL()
    {
    TRACES("CStartupUserWelcomeNote::ShowInformationNoteWrapperL()");
    TRACES1("CStartupUserWelcomeNote::ShowInformationNoteWrapperL(): Note type = %d", iNoteDefaultVariationType);

    TRequestStatus status;
    CAknGlobalNote* note = CAknGlobalNote::NewLC();
    if ( iNoteDefaultVariationType == EStartupUWNDefaultOperatorText)
        {
        // Set secondary display data if necessary
        if ( iStartupAppUi.CoverUISupported() )
            {
            SecondaryDisplay::TWelcomeNotePckg pckg(TPtrC(iNoteOperText.Ptr()));
            CAknSDData* sd = CAknSDData::NewL(SecondaryDisplay::KCatStartup, SecondaryDisplay::ECmdShowWelcomeNote, pckg);
            note->SetSecondaryDisplayData(sd); // ownership to notifier client
            }
        TRACES1("CStartupUserWelcomeNote::ShowInformationNoteWrapperL(): Operator text is '%S'", &iNoteOperText);
        note->ShowNoteL( status, EAknGlobalTextNote, TPtrC(iNoteOperText.Ptr()) );
        }
    else
        {
        // Set secondary display data if necessary
        if ( iStartupAppUi.CoverUISupported() )
            {
            SecondaryDisplay::TWelcomeNotePckg pckg(TPtrC(iNoteText.Ptr()));
            CAknSDData* sd = CAknSDData::NewL(SecondaryDisplay::KCatStartup, SecondaryDisplay::ECmdShowWelcomeNote, pckg);
            note->SetSecondaryDisplayData(sd); // ownership to notifier client
            }
        TRACES1("CStartupUserWelcomeNote::ShowInformationNoteWrapperL(): Welcome text is '%S'", &iNoteText);
        note->ShowNoteL( status, EAknGlobalTextNote, TPtrC(iNoteText.Ptr()) );
        }
    User::WaitForRequest( status );
    CleanupStack::PopAndDestroy(); // note
    TRACES("CStartupUserWelcomeNote::ShowInformationNoteWrapperL(): End");
    }

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::SetUserWelcomeNoteShowing(...)
// ---------------------------------------------------------------------------
void CStartupUserWelcomeNote::SetUserWelcomeNoteShowing(TBool aValue)
    {
    TRACES1("CStartupUserWelcomeNote::SetUserWelcomeNoteShowing(): aValue = %d", aValue);
    iUserWelcomeNoteShowing = aValue;
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ---------------------------------------------------------------------------
// CStartupUserWelcomeNote::SetUserWelcomeNoteShowing(...)
// ---------------------------------------------------------------------------
TInt CStartupUserWelcomeNote::CheckImage( const TDesC& aPath)
    {
    TRACES("CStartupUserWelcomeNote::CheckImage()");
    // Check if given welcome image is available
    RFile welcomeimage;
    TInt err( KErrNone );
    RFs fs;

    // Connect to file server
    err = fs.Connect();
    if (err != KErrNone)
        {
        TRACES("CStartupUserWelcomeNote::CheckImage(): Unable to connect to file server. Do not show welcome image.");
        fs.Close();
        TRACES1("CStartupUserWelcomeNote::CheckImage(): End, return %d", err);
        return err;
        }

    // Open welcome image
    err = welcomeimage.Open(fs, aPath, EFileRead);
    if (err != KErrNone)
        {
        TRACES("CStartupUserWelcomeNote::CheckImage(): Welcome image does not exists. Do not try to show it.");
        welcomeimage.Close();
        fs.Close();
        TRACES1("CStartupUserWelcomeNote::CheckImage(): End, return %d", err);
        return err;
        }

    welcomeimage.Close();
    fs.Close();
    TRACES1("CStartupUserWelcomeNote::CheckImage(): End, return %d", err);
    return err;
    }

//  End of File

