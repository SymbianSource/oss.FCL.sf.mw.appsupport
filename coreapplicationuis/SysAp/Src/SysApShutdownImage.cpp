/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApShutdownImage implementation.
*
*/


// INCLUDE FILES
#include <aknappui.h>
#include <AknIconUtils.h>
#include <AknUtils.h>
#include <coemain.h>
#include <barsread.h> //use of TResourceReader
#include <sysap.mbg>
#include "SysApShutdownImage.h"
#include "SysAp.hrh"
#include <data_caging_path_literals.hrh>

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApShutdownImage::NewL()
// ----------------------------------------------------------------------------

CSysApShutdownImage* CSysApShutdownImage::NewL()
    {
    TRACES( RDebug::Print( _L("CSysApSystemAgentObserver::NewL") ) );
    CSysApShutdownImage* self = new ( ELeave ) CSysApShutdownImage();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::ConstructL()
// ----------------------------------------------------------------------------

void CSysApShutdownImage::ConstructL()
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::ConstructL" ) ) );
    CreateWindowL();
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::CSysApShutdownImage()
// ----------------------------------------------------------------------------

CSysApShutdownImage::CSysApShutdownImage()
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::CSysApShutdownImage()" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::ShowShutdownImage()
// ----------------------------------------------------------------------------

void CSysApShutdownImage::ShowShutdownImageL(TInt aBitmapId)
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::ShowShutdownImageL:start" ) ) );
    TInt err ( 0 );

    SetRect(iAvkonAppUi->ApplicationRect());
    ActivateL();

    if ( aBitmapId )
        {
        _LIT( KDirAndFile, "z:sysap.mif" );
        TParse* fp = new (ELeave) TParse();
        CleanupStack::PushL(fp);
        fp->Set( KDirAndFile, &KDC_APP_BITMAP_DIR, NULL );
        TRACES( RDebug::Print(_L("CSysApShutdownImage::ShowShutdownImageL shutdown image: %S" ), &(fp->FullName())) );
        RFs fs;

        err = fs.Connect();
        if ( err == KErrNone )
            {
            CleanupClosePushL(fs);
            TFindFile findFile( fs );
            err = findFile.FindByPath( fp->FullName(), NULL );

            if ( err != KErrNone )
                {
                TRACES( RDebug::Print(_L("CSysApShutdownImage::ShowShutdownImageL: shutdown image not found, err=%d"), err ) );
                }
            else
                {
                delete iBitmap;
                iBitmap = NULL;
                // Ownership of bitmap is transferred to CSysApShutdownImage in CreateIconL
                iBitmap = AknIconUtils::CreateIconL( fp->FullName(), aBitmapId );
                TAknLayoutRect bitmapRect;
                bitmapRect.LayoutRect( Rect(), AKN_LAYOUT_WINDOW_screen );
                AknIconUtils::SetSize( iBitmap, bitmapRect.Rect().Size(), EAspectRatioPreservedAndUnusedSpaceRemoved );
                TInt xDelta=0; // for x coordinates
                TInt yDelta=0; // for y coordinates
                TSize bmpSizeInPixels = iBitmap->SizeInPixels();
                //center image to the center of the screen
                TRect rect = Rect();
                xDelta=( rect.Width() - bmpSizeInPixels.iWidth ) / 2;
                yDelta=( rect.Height() - bmpSizeInPixels.iHeight ) / 2;
                TPoint pos = TPoint( xDelta , yDelta ); // displacement vector
                //pos += rect.iTl; // bitmap top left corner position
                CWindowGc& gc = SystemGc();
                ActivateGc();
                Window().Invalidate( rect );
                Window().BeginRedraw( rect );
                gc.Clear();
                gc.BitBlt( pos, iBitmap ); // CWindowGc member function
                Window().EndRedraw();
                DeactivateGc();
                ControlEnv()->WsSession().Flush(); // force draw of the context
                TRACES( RDebug::Print(_L("CSysApShutdownImage::ShowShutdownImageL:end" ) ) );
                }
            }

        CleanupStack::PopAndDestroy(2); //fp, fs
        }
    else
        {
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
        DrawDeferred();
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
        TRACES( RDebug::Print(_L("CSysApShutdownImage::ShowShutdownImageL - Bitmap not defined, blank screen only" ) ) );
        CWindowGc& gc = SystemGc();
        ActivateGc();
        Window().Invalidate();
        Window().BeginRedraw();
        gc.SetBrushColor(KRgbWhite);
        gc.Clear();
        Window().EndRedraw();
        DeactivateGc();
        ControlEnv()->WsSession().Flush(); // force draw of the context
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        }

    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::ShutdownCoeControlWindow()
// ----------------------------------------------------------------------------

RWindow& CSysApShutdownImage::ShutdownCoeControlWindow()
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::ShutdownCoeControlWindow()" ) ) );
    return Window();
    }

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CSysApShutdownImage::SetComponent
//
// ---------------------------------------------------------------------------
//
void CSysApShutdownImage::SetComponent( CCoeControl& aComponent )
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::SetComponent()" ) ) );

    iComponent = &aComponent;

    TRACES( RDebug::Print(_L("CSysApShutdownImage::SetComponent(): End" ) ) );
    }


// ---------------------------------------------------------------------------
// CSysApShutdownImage::RemoveComponent
//
// ---------------------------------------------------------------------------
//
void CSysApShutdownImage::RemoveComponent()
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::RemoveComponent()" ) ) );

    iComponent = NULL;

    TRACES( RDebug::Print(_L("CSysApShutdownImage::RemoveComponent(): End" ) ) );
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION


// ----------------------------------------------------------------------------
// CSysApShutdownImage::~CSysApShutdownImage()
// ----------------------------------------------------------------------------

CSysApShutdownImage::~CSysApShutdownImage()
    {
    delete iBitmap;
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::ComponentControl(TInt /*aIndex*/)
// ----------------------------------------------------------------------------

CCoeControl* CSysApShutdownImage::ComponentControl(TInt /*aIndex*/) const
    {
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    return iComponent;
#else // RD_STARTUP_ANIMATION_CUSTOMIZATION
    return NULL;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    }

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
// ---------------------------------------------------------------------------
// CSysApShutdownImage::SizeChanged
//
// ---------------------------------------------------------------------------
void CSysApShutdownImage::SizeChanged()
    {
    TRACES( RDebug::Print(_L("CSysApShutdownImage::SizeChanged()" ) ) );

    if ( iComponent )
        {
        iComponent->SetRect( Rect() );
        }

    DrawNow();

    TRACES( RDebug::Print(_L("CSysApShutdownImage::SizeChanged(): End" ) ) );
    }
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

// ----------------------------------------------------------------------------
// CSysApShutdownImage::CountComponentControls()
// ----------------------------------------------------------------------------

TInt CSysApShutdownImage::CountComponentControls() const
    {
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    if ( iComponent ) return 1;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    return 0; // return nbr of controls inside this container
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::Draw(const TRect& /*aRect*/ )
// ----------------------------------------------------------------------------

void CSysApShutdownImage::Draw(const TRect& /*aRect*/ ) const
    {
#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION
    TRACES( RDebug::Print(_L("CSysApShutdownImage::Draw()" ) ) );
    TRACES( RDebug::Print(_L("CSysApShutdownImage: height: %d, width: %d" ), Rect().Height(), Rect().Width() ) );

	CWindowGc& gc = SystemGc();
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbWhite );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.Clear( Rect() );

    TRACES( RDebug::Print(_L("CSysApShutdownImage::Draw(): End" ) ) );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
// ----------------------------------------------------------------------------

void CSysApShutdownImage::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
    {
    //pure virtual from MCoeControlObserver
    }

// ----------------------------------------------------------------------------
// CSysApShutdownImage::HandleControlEventL(const TKeyEvent& /*aKeyEvent*/, TEventCode /*aType*/)
// ----------------------------------------------------------------------------

TKeyResponse CSysApShutdownImage::OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/, TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

// End of File
