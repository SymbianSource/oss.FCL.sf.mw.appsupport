/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains STIFUnit module implementation.
*
*/

/******************************************************************************/
/*
 *
 * Test module configuration 
*/

// Version
#define TEST_MODULE_VERSION_MAJOR 1
#define TEST_MODULE_VERSION_MINOR 1
#define TEST_MODULE_VERSION_BUILD 1


/*
 * Test module internals. Please do not edit them. 
*/

// Includes
#include <e32base.h>

// Literals
_LIT( KUnitLogPath, "\\logs\\testframework\\ut_oom\\" ); 
_LIT( KUnitLogFile, "ut_oom.txt" ); 
_LIT( KLogStart, "ut_oom logging starts!" );

// Defines
#define STIF_UNIT_TEST_CASES "../src/ut_oomcases.cpp"
#define STIF_UNIT_MODULE_CLASS_NAME Cut_oom
#define STIF_UNIT_MODULE_NAME _L("ut_oom.dll")

// Include STIF unit generic file
#include <StifUnitGeneric.h>

/*
 * Implementation of setHeapAndStack virtual methods.
 * To changes heap and stack size provide new values to iTestThreadStackSize, iTestThreadMinHeap and iTestThreadMaxHeap. 
*/

#ifdef STIFUNIT_SETHEAPANDSTACKSIZE
EXPORT_C TInt SetRequirements( CTestModuleParam*& aTestModuleParam, 
                               TUint32& aParameterValid )
    {
    aParameterValid = KStifTestModuleParameterChanged;
    CTestModuleParamVer01* param;
    TRAPD(err, param = CTestModuleParamVer01::NewL()); 
    // Stack size 
    param->iTestThreadStackSize= 16384; // 16K stack 
    // Heap sizes 
    param->iTestThreadMinHeap = 4096; // 4K heap min 
    param->iTestThreadMaxHeap = 1048576;// 1M heap max 

    return KErrNone;
    }
#undef STIFUNIT_SETHEAPANDSTACKSIZE
#endif

/*
 * User implementation of OOM virtual methods.
 * Providing own implementation requires uncommenting defines at the
 * beginnig of this file.   
*/
#ifdef STIFUNIT_OOMTESTQUERYL
TBool Cut_oom::OOMTestQueryL( const TFileName& /*aTestCaseFile*/, 
                                   const TInt aCaseNumber, 
                                   TOOMFailureType& aFailureType, 
                                   TInt& aFirstMemFailure, 
                                   TInt& aLastMemFailure)
    {
    }
#undef STIFUNIT_OOMTESTQUERYL
#endif

#ifdef STIFUNIT_OOMTESTINITIALIZEL
void Cut_oom::OOMTestInitializeL( const TFileName& /*aTestCaseFile*/, 
                                       const TInt /*aCaseNumber*/ )
    {
    }
#undef STIFUNIT_OOMTESTINITIALIZEL
#endif

#ifdef STIFUNIT_OOMHANDLEWARNINGL
void Cut_oom::OOMHandleWarningL( const TFileName& /*aTestCaseFile*/,
                                      const TInt /*aCaseNumber*/, 
                                      TInt& /*aFailNextValue*/)
    {
    }
#undef STIFUNIT_OOMHANDLEWARNINGL
#endif
    
#ifdef STIFUNIT_OOMTESTFINALIZEL
void Cut_oom::OOMTestFinalizeL( const TFileName& /*aTestCaseFile*/, 
                                     const TInt /*aCaseNumber*/ )
    {
    }
#undef STIFUNIT_OOMTESTFINALIZEL
#endif

// End of File
