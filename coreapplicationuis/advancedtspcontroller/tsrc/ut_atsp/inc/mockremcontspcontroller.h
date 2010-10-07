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
* Description: This file contains a mock CRemConTspControllerNoError declaration whose CreateAfterIdleL can return different value according the parameter passed by NewL.
*
*/
#ifndef MOCK_CREMCONTSPCONTROLLER_H
#define MOCK_CREMCONTSPCONTROLLER_H

//  INCLUDES
#include <e32base.h>
#include "remconidlelistener.h"

// DATA TYPES
enum MockControllerCtlValue
    {
    ECorrect = 0,
    ELeaveNoChange,
    ELeaveChange
    };

// CLASS DECLARATION

/**
*  Controls delivering of commands between Series 60 components and remote 
*  targets.
*/
class CRemConTspController : public CBase
    {
    public:     // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CRemConTspController* NewL(MockControllerCtlValue aCtlValue);
        
        /**
        * Destructor.
        */
        virtual ~CRemConTspController();
    
    public:     // Functions from base classes
    
        void CreateAfterIdleL();
    
    private:
        
        /**
        * C++ default constructor.
        */
        CRemConTspController(MockControllerCtlValue aCtlValue);
    
    protected:  // Data
    
        void ConstructL();

    private:    // Data
        // owned
        CRemConIdleListener* iIdle;
        
        MockControllerCtlValue iCtlValue;
    };

#endif      // MOCK_CREMCONTSPCONTROLLER_H
