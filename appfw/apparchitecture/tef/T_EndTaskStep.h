// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__T_ENDTASK_STEP_H__)
#define __T_ENDTASK_STEP_H__

#include <apgcli.h>
#include "appfwk_test_appui.h"
#include <apgtask.h>

class CTEndTaskStep : public CTestStep
	{
public:
	CTEndTaskStep();
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();	
	virtual TVerdict doTestStepL();
	void ExecuteL();
private:
	~CTEndTaskStep();
	void EndTaskTest1L(RApaLsSession& aLs);
	void EndTaskTest2L(RApaLsSession& aLs);
	void EndTaskTest3L(RApaLsSession& aLs);
	TInt LaunchAppL(RApaLsSession& aLs, const TUid& aAppUid);
private:
	RWsSession iWs;
	};

_LIT(KT_EndTaskStep,"T_EndTask");

#endif
