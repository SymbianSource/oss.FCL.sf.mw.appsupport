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
* Description:  Declaration of CSAnimMifPluginTestSuite class
*
*/


#ifndef SANIMMIFPLUGINTESTSUITE_H
#define SANIMMIFPLUGINTESTSUITE_H

#include <ceunittestsuiteclass.h>
#include <e32def.h>
#include <f32file.h>
#include "sanimobserver.h"

class CSAnimImagePlugin;
class CSAnimMifPluginTestSuite;
class CTestWaiter;

NONSHARABLE_CLASS( TMockMifAnimationObserver ) : public MSAnimObserver
    {
public:
    TMockMifAnimationObserver( CSAnimMifPluginTestSuite& aTestSuite ) : iTestSuite( aTestSuite ) {}
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ );
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/ );
private:
    CSAnimMifPluginTestSuite& iTestSuite;
    };

/**
*  Test cases for CSAnimSvgPlugin class.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimMifPluginTestSuite ) : public CEUnitTestSuiteClass
    {

public:

    /**
    * Constructs a CSAnimMifPluginTestSuite object.
    *
    * @since S60 3.2
    *
    * @return The new object
    */
    static CSAnimMifPluginTestSuite* NewL();
    static CSAnimMifPluginTestSuite* NewLC();

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimMifPluginTestSuite();

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    CSAnimMifPluginTestSuite();

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    */
    void ConstructL();

private:    // New methods

    void SetupEmptyL();
    void SetupObjL();
    void SetupFileL( const TDesC& aFileName );
    void SetupStillImageNoMaskL();
    void SetupStillImageL();
    void SetupScaledAnimationL();
    void SetupUnscaledAnimationL();
    void SetupFinishedL();
    void Teardown();

    void TestConstructorL();
    void TestConstructorNoMemoryL();
    void TestUnknownFileL();
    void TestCorruptFileL();
    void TestNoFramesFileL();
    void TestLoadStillImageL();
    void TestLoadScaledAnimationL();
    void TestLoadUnscaledAnimationL();
    void TestLoadNoMemoryL();
    void TestCancelLoadL();
    void TestShowStillImageL();
    void TestShowAnimationL();
    void TestCancelStillImageL();
    void TestCancelAnimationL();
    void TestCancelNotActiveL();
    void TestShowBeforeLoadL();
    void TestSetDisplayModeL();
    void TestSetSizeL();
    void TestSetFrameDelayL();
    void TestBackroundColourL();

private: // data

    EUNIT_DECLARE_TEST_TABLE;

    friend class TMockMifAnimationObserver;

    /** File server session used in testing. */
    RFs iFs;

    /** Instance of the class that this test suite is testing. */
    CSAnimImagePlugin* iTestObj;

    /** A 32 bit identifer returned by ECom to identify this instance to the framework. */
    TUid iDtorKey;

    /** For testing async calls. */
    CTestWaiter* iWaiter;

    /** For testing correct number of calls to animation observer. */
    TInt iFrameCounter;

    /** Animation observer for testing. */
    TMockMifAnimationObserver iObserver;

    };

#endif // SANIMMIFPLUGINTESTSUITE_H
