// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains CEASShdOpCodeAlarmAtIndex_DifferentSID1Step. There is one Test Step generated 
// for each IPC.
// 
//

#ifndef EASShdOpCodeAlarmAtIndex_DifferentSID1_H_
#define EASShdOpCodeAlarmAtIndex_DifferentSID1_H_

// Include the suite header
#include "CSuite.h"



class CEASShdOpCodeAlarmAtIndex_DifferentSID1Step: public CCapabilityTestStep
	{

public:

	inline TVersion Version();
	
	CEASShdOpCodeAlarmAtIndex_DifferentSID1Step() ;

	// Destructor
	~CEASShdOpCodeAlarmAtIndex_DifferentSID1Step()
		{
		iChildThread.Close();
		}
	
	// This is the Function called from "doTestStepL" by the test Suite,and it creates an 
	// child thread which internally calls the corresponding Exec_SendReceiveL_SERVERNAME fn.
	TVerdict MainThread();

	// Here's where the connection and testing the message takes place
	TInt Exec_SendReceiveL();
	
	};

/**
Get the version of the server to be tested
*/ 
TVersion CEASShdOpCodeAlarmAtIndex_DifferentSID1Step::Version()
	{
	return TVersion(2, 0, 0);
	}	
	

#endif
