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
* Description:  Declaration of CSAnimSvgPluginTestSuite class
*
*/


#ifndef SANIMSVGPLUGINTESTSUITE_H
#define SANIMSVGPLUGINTESTSUITE_H

#include <ceunittestsuiteclass.h>
#include <e32def.h>
#include <f32file.h>
#include "sanimobserver.h"

class CSAnimImagePlugin;
class CSAnimSvgPluginTestSuite;
class CTestWaiter;

NONSHARABLE_CLASS( TMockSvgAnimationObserver ) : public MSAnimObserver
    {
public:
    TMockSvgAnimationObserver( CSAnimSvgPluginTestSuite& aTestSuite ) : iTestSuite( aTestSuite ) {}
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/, const CFbsBitmap& /*aMask*/ );
    virtual void UpdateScreen( const CFbsBitmap& /*aFrame*/ );
private:
    CSAnimSvgPluginTestSuite& iTestSuite;
    };

/**
*  Test cases for CSAnimSvgPlugin class.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimSvgPluginTestSuite ) : public CEUnitTestSuiteClass
    {

public:

    /**
    * Constructs a CSAnimSvgPluginTestSuite object.
    *
    * @since S60 3.2
    *
    * @return The new object
    */
    static CSAnimSvgPluginTestSuite* NewL();
    static CSAnimSvgPluginTestSuite* NewLC();

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimSvgPluginTestSuite();

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    CSAnimSvgPluginTestSuite();

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
    void SetupStillImageL();
    void SetupScaledAnimationL();
    void SetupUnscaledAnimationL();
    void SetupAnimationWithTextL();
    void SetupStillImageBinaryL();
    void SetupScaledAnimationBinaryL();
    void SetupFinishedL();
    void Teardown();

    void TestConstructorL();
    void TestConstructorNoMemoryL();
    void TestUnknownFileL();
    void TestCorruptFileL();
    void TestLoadStillImageL();
    void TestLoadScaledAnimationL();
    void TestLoadUnscaledAnimationL();
    void TestLoadAnimationWithTextL();
    void TestLoadStillImageBinaryL();
    void TestLoadScaledAnimationBinaryL();
    void TestLoadNoMemoryL();
    void TestCancelLoadL();
    void TestShowStillImageL();
    void TestShowAnimationL();
    void TestShowNoMemoryL();
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

    friend class TMockSvgAnimationObserver;

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
    TMockSvgAnimationObserver iObserver;

    };

#endif // SANIMSVGPLUGINTESTSUITE_H
