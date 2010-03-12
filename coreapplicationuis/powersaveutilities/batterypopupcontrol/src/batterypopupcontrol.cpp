/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  BatteryPopupControl implementation
*
*/


#include <AknUtils.h>
#include <AknPanic.h>
#include <AknPreviewPopUpController.h>
#include <aknglobalpopupprioritycontroller.h>
#include <barsread.h> // TResourceReader
#include <eiklabel.h>
#include <fbs.h>
#include <aknappui.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsDrawUtils.h>
#include <AknBidiTextUtils.h>
#include <data_caging_path_literals.hrh> // KDC_RESOURCE_FILES_DIR
#include <batterypopupcontrol.mbg>
// ----------- Touch feedback additions start
#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#endif //RD_TACTILE_FEEDBACK
// ----------- Touch feedback additions end
#include <gulicon.h>
#include "batterypopupcontrol.h"
#include "trace.h"

// Constants
const TInt KPopupShowDelay = 0;       // Show immediately
const TInt KPopupHideDelay = 3000000; // hide after 3 sec
const TInt KMaxLinkTextLength = 255;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Default constructor.
// -----------------------------------------------------------------------------
//
CBatteryPopupControl::CBatteryPopupControl(): 
    iHighlightedItem( EFalse )
    {
    FUNC_LOG
    }

// -----------------------------------------------------------------------------
// CBatteryPopupControl::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CBatteryPopupControl* CBatteryPopupControl::NewL(
        const TDesC& aContentText,
        const TDesC& aLinkText )
    {
    FUNC_LOG
    CBatteryPopupControl* self =
                            new ( ELeave ) CBatteryPopupControl();
                                
    CleanupStack::PushL( self );
    self->ConstructL( aContentText, aLinkText );
    CleanupStack::Pop( self );
    return self;
    }    

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CBatteryPopupControl::~CBatteryPopupControl()
    {  
    FUNC_LOG   
    delete iController;
    delete iText;
    delete iIcon;
    delete iLinkText;
    } 

// -----------------------------------------------------------------------------
// CBatteryPopupControl::SetCommandObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CBatteryPopupControl::SetCommandObserver(
        MEikCommandObserver& aCommandObserver )
    {
    FUNC_LOG
    iCommandObserver = &aCommandObserver;
    }

// -----------------------------------------------------------------------------
// CBatteryPopupControl::ShowPopUpL
// -----------------------------------------------------------------------------
//
EXPORT_C void CBatteryPopupControl::ShowPopUpL()
    {    
    FUNC_LOG
    
    iController = CAknPreviewPopUpController::NewL( *this, CAknPreviewPopUpController::ELayoutDefault | CAknPreviewPopUpController::EDontClose );            
    iController->AddObserverL( *this );
    
    // Set popup's hide/show delays
    iController->SetPopUpShowDelay( KPopupShowDelay );
    iController->SetPopUpHideDelay( KPopupHideDelay );
               
    // Position
    TPoint uipRect;
    if( AknLayoutUtils::LayoutMetricsPosition( 
        AknLayoutUtils::EBatteryPane, uipRect ) )
        {
        iController->SetPosition( uipRect );
        }
		
    // Launch
    iController->ShowPopUp();  
    }

// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::HandleResourceChange
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::HandleResourceChange(
        TInt aType )
    {
    FUNC_LOG
    	
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SizeChanged();
        }
    }


// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::SetContainerWindowL
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::SetContainerWindowL(
        const CCoeControl& aContainer )
    {
    FUNC_LOG
    	
    CCoeControl::SetContainerWindowL( aContainer );
    
    if( iText )
        {
        iText->SetContainerWindowL( aContainer );
        }
    }

// -----------------------------------------------------------------------------
// From class MCoeControlObserver.
// CBatteryPopupControl::HandleControlEventL
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/ )
    {
    FUNC_LOG
    }
// ---------------------------------------------------------------------------
// From MAknPreviewPopUpObserver
// ---------------------------------------------------------------------------
//
void CBatteryPopupControl::HandlePreviewPopUpEventL(
    CAknPreviewPopUpController* /*aController*/,
    TPreviewPopUpEvent aEvent )
    {
    FUNC_LOG

    switch ( aEvent )
        {
        case EPreviewPopUpShown:
            AknGlobalPopupPriorityController::ShowPopup( *this, ETrue );
            break;
        case EPreviewPopUpHidden:
            AknGlobalPopupPriorityController::RemovePopupPriority( *this );
            break;
        }
    }
    
// -----------------------------------------------------------------------------
// CAknStylusActivatedPopUpContent::ConstructL
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::ConstructL( const TDesC& aContentText,
                                       const TDesC& aLinkText )
    {
    FUNC_LOG

    if( aContentText != KNullDesC )
        {           
        iText = new ( ELeave ) CEikLabel;
        iText->SetTextL( aContentText );
        CreateIconL();
        }
    
    if( aLinkText != KNullDesC )
        {
        // Check if only link is displayed.
        if( aContentText != KNullDesC )
            {
            iVariant = EVariantIconTextLink;
            }      
        iLinkText = aLinkText.AllocL(); 
        }
        
    TRect rectPopUpWindow = PopUpWindowRectFromLayout( 
        AknLayoutScalable_Avkon::popup_battery_window( iVariant ) );
      
    TAknWindowComponentLayout infoPaneLayout =
        AknLayoutScalable_Avkon::bg_popup_sub_pane_cp25( iVariant );
        
    TRect rectInfoPane = RectFromLayout( rectPopUpWindow, infoPaneLayout );

    TAknTextComponentLayout textLayout =
        AknLayoutScalable_Avkon::popup_battery_window_t1( iVariant );
    TAknLayoutText textRect;
    textRect.LayoutText( rectPopUpWindow, textLayout );
    
    // Font for command links
    iFont = textRect.Font();    
    }
    
// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CBatteryPopupControl::ComponentControl(
        TInt aIndex ) const
    {
    FUNC_LOG	
    switch ( aIndex )
        {
        case 0:
            {
            if( iText )
                {
                return iText;             
                }
            }        
        default:
            __ASSERT_ALWAYS( aIndex >= 0, User::Panic( 
                _L("CBatteryPopupControl::ComponentControl"), EAknPanicOutOfRange ) );
            return NULL;
        }        
    }


// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CBatteryPopupControl::CountComponentControls() const
    {
    FUNC_LOG
    TInt count = 0;   

    if( iText )
        {
        count++;
        }
    
    return count;
    }


// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::Draw
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::Draw( const TRect& /*aRect*/ ) const
    {
    FUNC_LOG
      	
    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
    if( iIcon )
        {
        CFbsBitmap* bmp = NULL;
        TRgb color( KRgbWhite ); // Default never used
        bmp = AknsUtils::GetCachedBitmap( skin, KAknsIIDQsnComponentColorBmpCG2 );
        AknsUtils::GetCachedColor( skin, color, 
            KAknsIIDQsnComponentColors, EAknsCIQsnComponentColorsCG2 );

        AknIconUtils::SetSize( iIcon->Mask(), iBitmapLayout.Rect().Size() );
        if ( bmp )
            {
            iBitmapLayout.DrawImage( gc, bmp, iIcon->Mask() );
            }
        else
            {
            AknIconUtils::SetSize( iIcon->Bitmap(), iBitmapLayout.Rect().Size() );     
            bmp = iIcon->Bitmap();
            iBitmapLayout.DrawImage( gc, bmp, iIcon->Mask() );
            }
        gc.Reset(); 
        }
    TRgb textColor;
    AknsUtils::GetCachedColor( skin, textColor, KAknsIIDQsnHighlightColors,
                               EAknsCIQsnHighlightColorsCG3 );

    gc.SetPenStyle( CGraphicsContext::ESolidPen ); 
    gc.SetPenColor( textColor );

    const CFont* font = iFont;
    if ( !font )
        {
        font = iCoeEnv->NormalFont();
        }
    gc.UseFont( font );

    gc.SetUnderlineStyle( EUnderlineOn );
    if(iLinkText)
        {
        TRect rect( iLinkRect );
        if ( Background() )
            {
            Background()->Draw( gc, *this, rect );
            }

        // buffer for visually ordered text
        HBufC* visualText = NULL; 
        TRAPD( err, visualText = HBufC::NewL( KMaxLinkTextLength + 
            KAknBidiExtraSpacePerLine ) ); 
        if ( err != KErrNone )
            {
            return; 
            }
        TPtr ptr = visualText->Des(); 
        TInt clipWidth = rect.Width();

        if ( iHighlightedItem ) 
            {
            TRgb highlightcolor = AKN_LAF_COLOR( 244 );
            AknsUtils::GetCachedColor( skin, highlightcolor, 
                                       KAknsIIDQsnHighlightColors, EAknsCIQsnHighlightColorsCG1 );

            gc.SetBrushColor( highlightcolor );
            gc.SetBrushStyle( CGraphicsContext::ESolidBrush ); 
            }
        else
            {
            gc.SetBrushStyle( CGraphicsContext::ENullBrush );
            }

        // bidi processing - using AknBidiTextUtils.
        AknBidiTextUtils::ConvertToVisualAndClip(
            *(iLinkText),
            ptr,
            *font,
            clipWidth,
            clipWidth );

        TInt baselineOffset = 0;
        baselineOffset = font->AscentInPixels() +
            ( rect.Height() - font->AscentInPixels() ) / 2;
        gc.DrawText( ptr, rect, baselineOffset, 
            CGraphicsContext::ELeft );
        delete visualText; 
        }
    }


// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::MinimumSize
// -----------------------------------------------------------------------------
//
TSize CBatteryPopupControl::MinimumSize()
    {
    FUNC_LOG
    TInt nbrOfInfoLines = 0;
    TInt minWidth = 0;
    
    if( iText )
        {
        nbrOfInfoLines = iText->NumberOfLines();
        CEikLabel* textLabel; 
        for ( TInt i=0; i < nbrOfInfoLines; i++ )
            {
            textLabel = iText;
            TPtrC ptrText = ( *textLabel->Text() );
            minWidth = Max( textLabel->CalcMinimumSize( ptrText ).iWidth, minWidth ); 
            }
        }
    if(iLinkText)
        {     
        TInt tempWidth = iFont->TextWidthInPixels( *( iLinkText ) );
        minWidth = Max( minWidth, tempWidth );
        }
    TInt rectWidth = 0;
    
    TAknWindowComponentLayout infoPaneLayout = 
        AknLayoutScalable_Avkon::bg_popup_sub_pane_cp25( iVariant );
               
    TRect rectPopUpWindow =  PopUpWindowRectFromLayout( 
        AknLayoutScalable_Avkon::popup_battery_window( iVariant ) );   

    TRect rectInfoPane = RectFromLayout( rectPopUpWindow, infoPaneLayout );
    
    rectWidth = Max( rectInfoPane.Width(), minWidth ); 
    
    TInt rectHeight = rectInfoPane.Height();
   
    return TSize( rectWidth, rectHeight ); 
    }


// -----------------------------------------------------------------------------
// From class CCoeControl.
// CBatteryPopupControl::SizeChanged
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::SizeChanged()
    {
    FUNC_LOG 	
    // Get popup window rect
    TRect rectPopUpWindow =  PopUpWindowRectFromLayout( 
        AknLayoutScalable_Avkon::bg_popup_sub_pane_cp25( iVariant ) );
    
    // Get pane icon and text layouts
    TAknWindowLineLayout paneIconLayout(
        AknLayoutScalable_Avkon::popup_battery_window_g1(iVariant));

    //Set icon and text rects
    if( iIcon )
        {
        iBitmapLayout.LayoutRect( rectPopUpWindow, paneIconLayout );
        }
    
    if( iText )
        {
        TAknTextComponentLayout textLayout =
        AknLayoutScalable_Avkon::popup_battery_window_t1( iVariant );
    
        TAknLayoutText textRect;
        textRect.LayoutText( rectPopUpWindow, textLayout );
    
        iText->SetRect( textRect.TextRect() );    
        // Set text font
        iText->SetFont( textRect.Font() );
        }
        
    // Set link text rect
    if( iLinkText )
        {
        TAknLayoutText linkLayoutText;
        // popup_battery_window_t2 doesn't have other variants as EVariantIconTextLink
        linkLayoutText.LayoutText( rectPopUpWindow,
            AknLayoutScalable_Avkon::popup_battery_window_t2( EVariantIconTextLink ) );
       
        iFont = linkLayoutText.Font();
        
        TInt tempWidth = iFont->TextWidthInPixels( *(iLinkText) );
        TRect tempRect;
        
        if( iText )
            {                     
            tempRect = TRect( linkLayoutText.TextRect() ); 
            }
        else
            {
            TAknLayoutRect linkLayout;
          
            linkLayout.LayoutRect( rectPopUpWindow, paneIconLayout );
  
            tempRect = TRect( linkLayout.Rect() );            
            }
        tempRect.SetWidth( tempWidth );
        iLinkRect = tempRect;
        }
    }

// -----------------------------------------------------------------------------
// RectFromLayout
// -----------------------------------------------------------------------------
//
TRect CBatteryPopupControl::RectFromLayout( const TRect& aParent,
        const TAknWindowComponentLayout& aComponentLayout ) const
    {
    TAknWindowLineLayout lineLayout = aComponentLayout.LayoutLine();
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect( aParent, lineLayout );
    return layoutRect.Rect();
    }

// -----------------------------------------------------------------------------
// PopUpWindowRectFromLayout
// -----------------------------------------------------------------------------
//
TRect CBatteryPopupControl::PopUpWindowRectFromLayout( const
    TAknWindowComponentLayout& aComponentLayout ) const
    {
    FUNC_LOG          
    // get popup window width from the widest variety
    TAknWindowLineLayout lineLayout = aComponentLayout.LayoutLine();
    
    lineLayout.iW = AknLayoutScalable_Avkon::
        popup_battery_window( iVariant ).LayoutLine().iW;
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect( iAvkonAppUi->ApplicationRect(), lineLayout );

    return layoutRect.Rect();
    }
   
// -----------------------------------------------------------------------------
// From class CCoeControl
// CBatteryPopupControl::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::HandlePointerEventL(
    const TPointerEvent& aPointerEvent )
    {
    FUNC_LOG    
    if(iLinkText)
        {
        if ( iLinkRect.Contains( aPointerEvent.iPosition ) )
            {
            if ( !iHighlightedItem )
                {         
                iHighlightedItem = ETrue;             
                DrawNow( iLinkRect );
                }
#ifdef RD_TACTILE_FEEDBACK
            if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
                {
                MTouchFeedback* feedback = MTouchFeedback::Instance();
                
                if ( feedback )
                    {
                    feedback->InstantFeedback( ETouchFeedbackBasic );
                    }
                }
#endif // RD_TACTILE_FEEDBACK                   
            // Nofity command observer  
            if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
                {
                iCommandObserver->ProcessCommandL( ELinkFirst );
                iHighlightedItem = EFalse; 
                }
            }
        else
            {
            if ( iHighlightedItem )
                {         
                iHighlightedItem = EFalse;             
                DrawNow( iLinkRect );
                }
            }
        }
    }
// -----------------------------------------------------------------------------
// CBatteryPopupControl::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
void CBatteryPopupControl::CreateIconL(  )
    {
    FUNC_LOG
    
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    _LIT( KBIPPluginIconDirAndName, "z:\\resource\\apps\\batterypopupcontrol.mif");
    fp->Set( KBIPPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );
    
    delete iIcon;
    iIcon = NULL;
    
    iIcon = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(),
                                       KAknsIIDQgnPropBatteryIcon,
                                       fp->FullName(),
                                       EMbmBatterypopupcontrolQgn_prop_battery_ps_deactivate,
                                       EMbmBatterypopupcontrolQgn_prop_battery_ps_deactivate_mask );

    CleanupStack::PopAndDestroy( fp );
    }      
//  End of File
