/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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


//  CLASS HEADER
#include "MT_OomMonitor.h"
#include "MT_COomMonitorPlugin.h"

//  EXTERNAL INCLUDES
#include <CEUnitTestSuite.h>

EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L("OomMonitor API tests") );

    rootSuite->AddL( MT_OomMonitor::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( MT_COomMonitorPlugin::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );
    return rootSuite;    
    }
/*
#ifndef EKA2
GLDEF_C TInt E32Dll( TDllReason )
	{
	return KErrNone;
	}
#endif
*/
//  END OF FILE
