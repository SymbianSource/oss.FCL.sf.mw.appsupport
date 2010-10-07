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
* Description:  Declaration of CTestWaiter class
*
*/


#ifndef TESTWAITER_H
#define TESTWAITER_H

#include <e32base.h>

/**
*  Stops active scheduler in RunL.
*
*  @lib None
*  @since S60 3.2
*/
class CTestWaiter  : public CActive
    {

public:

    /**
    * Constructs a CTestWaiter object.
    *
    * @return The new object
    * @since S60 3.2
    */
    static CTestWaiter* NewL();

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CTestWaiter();

    /**
    * Set active.
    *
    * @since S60 3.2
    */
    void Activate();

protected:

    /**
    * From CActive.
    * Implements cancellation of an outstanding request.
    *
    * @since S60 3.2
    */
    virtual void DoCancel();

    /**
    * From CActive.
    * Handles an active object's request completion event.
    * Never leaves.
    *
    * @since S60 3.2
    */
    virtual void RunL();

private:

    /**
    * First phase constructor.
    *
    */
    CTestWaiter();

private: // data

    };

#endif // TESTWAITER_H
