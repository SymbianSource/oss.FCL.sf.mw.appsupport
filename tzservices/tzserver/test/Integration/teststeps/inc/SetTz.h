/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __SET_TZ_H__
#define __SET_TZ_H__

#include <tz.h>
#include <test/testexecutestepbase.h>

/**
 * Teststep that sets the timezone to a value passed from the ini, for example, 
 * Asia/Calcutta and checks if it has been set correctly.
 */
class CSetTz: public CTestStep
	{
public:
	CSetTz();
	~CSetTz();
	
	TVerdict 		doTestStepL();
	
	void 					PrintHomeTimeL();
	void 					PrintTimeL(const TTime& aTime);
	};

_LIT(KSetTz,"SetTz");

#endif // __SET_TZ_H__
