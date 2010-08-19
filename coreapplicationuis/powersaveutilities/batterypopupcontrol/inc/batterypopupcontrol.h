/*
* Copyright (c) 2007-10 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Content for preview pop-up.
*
*/


#ifndef CBATTERYPOPUPCONTROL_H
#define CBATTERYPOPUPCONTROL_H

#include <AknControl.h>
#include <AknUtils.h>
#include <AknPreviewPopUpController.h> // Controlling the preview pop-up component

class MEikCommandObserver; 
class CEikLabel;
class CGulIcon;


/**
*  Class defining stylus activated pop-up content.
*  
*  This content is intended to be used in connection with preview pop-up
*  component. Content shows an icon, text and
*  an optional stylus tappable function link. If using link, the user gives link text and
*  corresponding ID for link in construction. The user must also set
*  itself as an observer for this content when using function links. This
*  way the user gets callbacks (with ID) for its ProcessCommandL() function
*  when the link is tapped.  
*
*  @lib BatteryPopupControl.lib
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CBatteryPopupControl ) :
    public CAknControl,
    public MCoeControlObserver,
    public MAknPreviewPopUpObserver
    {
public:
    
    /** Preview popup command ids */
    enum TLinkCommandID
        {
        ELinkNone = 0,
        ELinkFirst = 1
        };
        
    /**
     * Two-phased constructor. Constructs the content
     * with text and function link.
     *
     * @since S60 5.0
     * @param aContentText Text for the popup. Mandatory.
     * @param aLinkText    Link text.
     * @return Pointer to the created content object.
     */
                           
    IMPORT_C static CBatteryPopupControl* NewL( const TDesC& aContentText,
                                                const TDesC& aLinkText );

    /**
     * Destructor.
     */
    ~CBatteryPopupControl();
       
    /**
     * Sets the command observer of the preview pop-up content. When
     * link that is set to the content is pressed, the command
     * observer's ProcessCommandL() method is called with the command ID
     * of the pressed component.
     *
     * @since S60 5.0
     * @param aCommandObserver Command observer.
     */
    IMPORT_C void SetCommandObserver( MEikCommandObserver& aCommandObserver );
    
    /**
     * Shows preview popup
     * 
     * @since S60 5.0    
     */ 
    IMPORT_C void ShowPopUpL();

private:   
// from base class CCoeControl
        
	  /**
	   * Handles a change to the control's resources of type aType which are
	   * shared across the environment.
	   *
	   * @param aType The type of resource that have changed.
	   */
    void HandleResourceChange( TInt aType );
	
    /**
     * Sets container window.
     *
     * @param aContainer Container.
     */
    void SetContainerWindowL( const CCoeControl& aContainer );

    /**
     * Handles pointer events
     *
     * @param aPointerEvent information about the pointer event.
     */
    void HandlePointerEventL( const TPointerEvent& aPointerEvent );


// from base class MCoeControlObserver
    
    /**
     * Handles an event from an observed control.
     *
     * @param aControl The control that sent the event.
     * @param aEventType The event type.
     */
    void HandleControlEventL( CCoeControl *aControl,
                                           TCoeEvent aEventType );
    
// From MAknPreviewPopUpObserver
   
    /**
     * @see MAknPreviewPopUpObserver
     */
    void HandlePreviewPopUpEventL( CAknPreviewPopUpController* aController,
                                   TPreviewPopUpEvent aEvent );     

    /**
     * Default constructor.
     */
    CBatteryPopupControl();
    
    /**
     * Symbian 2nd phase constructor.
     *
     * @since S60 5.0
     * @param aContentText Text for the popup.
     * @param aLinkText    Link text.
     */
    void ConstructL( const TDesC& aContentText,                    
                     const TDesC& aLinkText );

    /**
     * Gets rect from layout data.
     *
     * @since S60 5.0
     * @param aParent Parent rect.
     * @param aComponentLayout Layout data.
     */
    TRect RectFromLayout( const TRect& aParent,
        const TAknWindowComponentLayout& aComponentLayout ) const;

    /**
     * Gets rect from layout data.
     *
     * @since S60 5.0
     * @param aComponentLayout Layout data.
     */
    TRect PopUpWindowRectFromLayout( const TAknWindowComponentLayout&
        aComponentLayout ) const;
    
    /**
     * Creates CGulIcon.
     *
     * @since S60 5.0
     */
    void CreateIconL( );
   
// from base class CCoeControl

    /**
     * Returns a control determined by control index.
     *
     * @param aIndex Index of a control to be returned.
     * @return Pointer to control.
     */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
     * Returns the number of controls inside the control.
     *
     * @return The number of component controls.
     */
    TInt CountComponentControls() const;
    
    /**
     * Draws the control.
     *
     * @param aRect The control's area.
     */
    void Draw( const TRect& aRect ) const;

    /**
     * Returns the control's minimum required size.
     *
     * @return The minimum size required by the control.
     */
    TSize MinimumSize();

    /**
     * Responds to size changes to set the size and position for 
     * the contents of this control.
     */
    void SizeChanged();
   
private: // data
    /** Variant ids */
    enum TVariantID
        {
        EVariantIconText = 0,
        EVariantIconTextLink = 1
        };
        
    /**
     * Label for informative text.
     * Own.
     */
    CEikLabel* iText;

    /**
     * Link text.
     * Own.
     */    
    HBufC* iLinkText;

    /**
     * Command observer.
     * Not own.
     */
    MEikCommandObserver* iCommandObserver;

    /**
     * Font
     * Not own
     */
    const CFont* iFont;
    
    /**
     * Status of highlighted item.  
     */
    TBool iHighlightedItem;
    
    /**
     * Id for icon bitmap    
     */ 
    TInt iIconId;
        
    /**
     * Id for icon mask    
     */ 
    TInt iMaskId;
    
    /**
     * Icon.
     * Own.  
     */ 
    CGulIcon* iIcon;
        
    /**
     * Layout for icon  
     */
    TAknLayoutRect iBitmapLayout;  

    /**
     * Variant, 
     * 0 = icon and text
     * 1 = icon, text and function link
     */
    TInt iVariant;
    
    /**
     * Popup controller
     * Own.  
     */    
    CAknPreviewPopUpController* iController;
    
    /**
     * Rect for link  
     */     
    TRect iLinkRect;

    /**
     * Alignment of the Layout either ERight or ELeft
     */
    CGraphicsContext::TTextAlign iAlign;
    };
    
    
#endif // CBATTERYPOPUPCONTROL_H

