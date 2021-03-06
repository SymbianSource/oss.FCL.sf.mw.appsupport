// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined __TAMASTARTER_SERVER_H__)
#define __TAMASTARTER_SERVER_H__

#include <test/testexecuteserverbase.h>

// Entry point
TInt E32Main(void);

/**
CAppfwkAmaStarterTestServer encapsulates the functionality required
to execute TEF based test cases for the AmaStarter component.
*/
class CAppfwkAmaStarterTestServer : public CTestServer
	{
public:
	static CAppfwkAmaStarterTestServer* NewL();
	
	//from CTestServer
	CTestStep* CreateTestStep(const TDesC& aStepName);
	};
	
#endif

