/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Declaration of CSAnimMockView class
*
*/


#ifndef SANIMMOCKVIEW_H
#define SANIMMOCKVIEW_H

#include <coecntrl.h>

/**
*  For testing CSAnimCtrl.
*
*  @lib None
*  @since S60 3.2
*/
class CSAnimMockView  : public CCoeControl
    {

public:

    /**
    * Constructs a CSAnimMockView object.
    *
    * @since S60 3.2
    *
    * @return The new object
    */
    static CSAnimMockView* NewL();

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimMockView();

    /**
    * Set a sub-control to this control.
    *
    * @since S60 3.2
    */
    void SetComponent( CCoeControl& aComponent );

    /**
    * Remove the sub-control from this control.
    *
    * @since S60 3.2
    */
    void RemoveComponent();

protected:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    CSAnimMockView();

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    */
    void ConstructL();

    /**
    * From CCoeControl.
    *
    * @since S60 3.2
    *
    * @return The number of controls contained in this control.
    */
    TInt CountComponentControls() const;

    /**
    * From CCoeControl.
    *
    * @since S60 3.2
    *
    * @param aIndex Identifies the component control to return.
    * @return The component control at index given as parameter.
    */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
    * From CCoeControl.
    *
    * @since S60 3.2
    *
    * @param aRect The rectangle to draw into.
    */
    void Draw( const TRect& aRect ) const;

private:

    /** Component control. */
    CCoeControl* iComponent;

    };

#endif // SANIMMOCKVIEW_H
