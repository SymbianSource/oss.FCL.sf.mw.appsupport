// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#if (!defined __APPFWK_VIEWSRV_TEST_SERVER_H__)
#define __APPFWK_VIEWSRV_TEST_SERVER_H__

#include <test/testexecuteserverbase.h>

// Entry point
GLDEF_C TInt E32Main(void);

/**
CAppfwkViewSrvTestServer encapsulates the functionality required
to execute Test Execute based test cases for the ViewSrv
component.
*/
class CAppfwkViewSrvTestServer : public CTestServer
	{
public:
	static CAppfwkViewSrvTestServer* NewL();
	
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};
	
#endif

