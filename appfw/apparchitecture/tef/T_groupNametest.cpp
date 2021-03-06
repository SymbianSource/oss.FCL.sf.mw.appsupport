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
// The following test case is used to test whether the group name specified in the registration file 
// is "overridden" by the group name specified in the localisable resource file 
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code 
*/


#include "testableapalssession.h"
#include <apaid.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaidpartner.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include "T_groupNametest.h"
#include "apparctestserver.h"
#include <test/testexecutestepbase.h>
#include "T_SisFileInstaller.h"

_LIT(KGroupNameTestAppSisFile, "z:\\apparctest\\apparctestsisfiles\\t_groupname.sis");
_LIT(KGroupNameTestAppComponent, "T_groupname");


// CT_GroupNameStep

const TUid KUidTGroupName={0x10208185};

/**
  Constructor
*/
CT_GroupNameStep::CT_GroupNameStep()
	{
	}


/**
  Destructor
*/
CT_GroupNameStep::~CT_GroupNameStep()
	{
	}


/**
   @SYMTestCaseID T-GroupNameStep-DoTestGroupNameL
  
   @SYMPREQ

   @SYMTestCaseDesc     T_groupname to test the Groupname in localisable resource file
   
   @SYMTestPriority High
   
   @SYMTestStatus Implemented
  
   @SYMTestActions	 
   It connects to the Application Architecture server to perform the
   following:
   To test whether the group name specified in the registration file 
   is "overridden" by the group name specified in the localisable resource file 
   It calls the following function to get information about the groupname.
   RApaLsSession::GetAppCapability(TDes8& aCapabilityBuf,TUid aAppUid) const\n
   After the call Succeeds call the RApaLsSession::GetAppCapability(TDes8& aCapabilityBuf,TUid aAppUid) const\n
   with the KNullUid . The Retrun value is KErrNotFound.
   
	
   @SYMTestExpectedResults
   GetAppCapability should return the groupname
   defined in the application's localisable resource file.
  
 */
void CT_GroupNameStep::DoTestGroupNameL(RApaLsSession& aLs)
	{
	TApaAppCapabilityBuf cbuf;
	TApaAppCapability capability;
	 
	// To read the groupname defined in localisable resource file

	TInt err = aLs.GetAppCapability(cbuf,KUidTGroupName);
	TEST(err == KErrNone);
	
    
	TEST(aLs.GetAppCapability(cbuf,KNullUid) == KErrNotFound);

	//since the value was kErrNotFound, the value of cbuf is unaltered
	capability = cbuf();
	_LIT(KGroupname,"Loc_groupname");
	
	TEST(capability.iGroupName == KGroupname);

	}

TVerdict CT_GroupNameStep::doTestStepPreambleL()
    {
    CSisFileInstaller sisFileInstaller;
    INFO_PRINTF2(_L("Installing sis file from -> %S"), &KGroupNameTestAppSisFile);
    sisFileInstaller.InstallSisAndWaitForAppListUpdateL(KGroupNameTestAppSisFile);

    SetTestStepResult(EPass);
    return TestStepResult();
    }
TVerdict CT_GroupNameStep::doTestStepPostambleL()
    {
    CSisFileInstaller sisFileInstaller;
    sisFileInstaller.UninstallSisL(KGroupNameTestAppComponent);
    
    return TestStepResult();
    }

/**
   @return - TVerdict code
   Override of base class virtual
 */
TVerdict CT_GroupNameStep::doTestStepL()
	{
	INFO_PRINTF1(_L(" Test Started"));

	//To test whether the group name specified in the registration file 
	//is "overridden" by the group name specified in the localisable resource file 

	RTestableApaLsSession ls;
	TEST(KErrNone == ls.Connect());
	CleanupClosePushL(ls);
	
	HEAP_TEST_LS_SESSION(ls, 0, 0, DoTestGroupNameL(ls), NO_CLEANUP);
	
	CleanupStack::PopAndDestroy(&ls);

	INFO_PRINTF1(_L(" TEST Finished"));
		
	return TestStepResult();

	}




