/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/

#ifndef GSRFSPLUGINCONTAINER_H
#define GSRFSPLUGINCONTAINER_H

// INCLUDES
#include <gsbasecontainer.h>
#include "gssettingid.h"

// CLASS DECLARATION
class CGSRFSPlugin;
class CGSListBoxItemTextArray;


class CGSRFSPluginContainer : public CGSBaseContainer
    {
public: 
        
    /**
     *  C++ default constructor.
     */
    CGSRFSPluginContainer( CGSRFSPlugin* aGSRFSPlugin );

    /**
     * By default Symbian OS constructor is private.
     */
    void ConstructL(const TRect& aRect);

    /**
     * Destructor.
     */
    ~CGSRFSPluginContainer();
        
public: // new methods
        
    TInt CurrentFeatureId() const;

protected: // from CGSBaseContainer
    
    void ConstructListBoxL( TInt aResLbxId );
    
private: // from CCoeControl    
        
    void GetHelpContext( TCoeHelpContext& aContext ) const;

private: // data

    CGSRFSPlugin* iGSRFSPlugin;                 // not own    
    CGSListBoxItemTextArray* iListboxItemArray;
    };


#endif  // GSRFSPLUGINCONTAINER_H

// End of File
