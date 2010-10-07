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
* Description:  Declaration of CSAnimCtrlRootSuite class
*
*/


#ifndef SANIMCTRLROOTSUITE_H
#define SANIMCTRLROOTSUITE_H

class CSAnimMockView;

/**
*  Root test suite for SAnimCtrl tests.
*
*  @lib None
*  @since S60 3.2
*/
class CSAnimCtrlRootSuite 
    {

public:

    /**
    * Constructs a CSAnimCtrlRootSuite object.
    *
    * @since S60 3.2
    *
    * @return The new object
    */
    static CSAnimCtrlRootSuite* NewL();
    static CSAnimCtrlRootSuite* NewLC();

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimCtrlRootSuite();

    /**
    * Return a reference to the main view.
    *
    * @since S60 3.2
    *
    * @return A reference to the main view.
    */
    CSAnimMockView& MainView();

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    CSAnimCtrlRootSuite();

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    */
    void ConstructL();

private: // data

    /** Parent control for the objects to be tested. */
    CSAnimMockView* iView;

    };

#endif // SANIMCTRLROOTSUITE_H
