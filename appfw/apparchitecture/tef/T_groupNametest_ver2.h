// Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent - Internal Symbian test code 
*/

#if (!defined __T_groupNametest_ver2_H__)
#define __T_groupNametest_ver2_H__

#include "apparctestserver.h"

//  A CT_GroupNameStep_ver2 test class. 

class CT_GroupNameStep_ver2 : public CTestStep
	{
public:
	CT_GroupNameStep_ver2();
	~CT_GroupNameStep_ver2();
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepPostambleL();	
	virtual TVerdict doTestStepL();
	void DoTestCaptionNameL(RApaLsSession& aLs);
	
private:
	};

_LIT(KT_GroupNameStep_ver2, "T_GroupName_ver2");


#endif

