/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains STIFUnit implementation.
*
*/

/**
 * STIF_UNIT_INCLUDE SECTION - put all #includes between STIF_UNIT_INCLUDE_SECTION
 *                             and STIF_UNIT_INCLUDE_SECTION_END
 */
#ifdef STIF_UNIT_INCLUDE_SECTION
#include <f32file.h> 
#include <xml/parser.h>
#include <platform/xml/documentparameters.h>
#include <stringpool.h>

#include "oomrunpluginconfig.h"
#include "oomconfig.h"
#include "oomconfigparser.h"
#include "oomwindowgrouplist.h"
#include "oomcloseappconfig.h"
#include "oomapplicationconfig.h"

#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
	/**
	 * Example of variable common for some test cases
	 */
     TInt err;
     RFs configRFs;
	 COomConfig* oomConfig;
	 COomConfigParser* oomConfigParser;
	 // this plugin should be defined in oomconfig.xml 
	 // locate at z:\\private\\10207218\\oomconfig.xml
	 // if this changes, also update this value to let the 
	 // test valid
	 const TUint KTestPluginId = 0x10281f93;
	 const TUint KTestPluginPriority = 30;
	 const TUint KTestTargetAPPId = 0x10008d39;
	 const TUint KTestAppId = 0x10005903;
	 const TInt KOomDefaultAppId = 0;
     const TUint KTestDefaultAppPriority = 80;
     const TUint KOomErrBadXml = 45;
     
     // 
#endif
/**
 * END OF GLOBAL VARIABLES SECTION
 */


/**
 * TEST CASES SECTION
 */
#ifdef TEST_CASES
/**
 * STIF_SETUP defines activities needed before every test case.
 */
STIF_SETUP
{
	/** Example of use of STIF_SETUP - a variable common for some test cases is initialized
	 */
     oomConfig = COomConfig::NewL();
     STIF_ASSERT_NOT_NULL(oomConfig);
     CleanupStack::PushL(oomConfig);

     err = configRFs.Connect();  
     CleanupClosePushL(configRFs);
     STIF_ASSERT_EQUALS(err, KErrNone);

     oomConfigParser = new (ELeave) COomConfigParser(*oomConfig, configRFs);    
     STIF_ASSERT_NOT_NULL(oomConfigParser);
     CleanupStack::PushL(oomConfigParser);
     
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
	/** Example of use of STIF_TEARDOWN - a variable common for some test cases is destroyed
	 */
    iLog->Log(_L("enter tear down"));
    CleanupStack::PopAndDestroy(oomConfigParser);
    iLog->Log(_L("oomConfigParser pop and desed"));
    CleanupStack::PopAndDestroy();      //configRFs
    iLog->Log(_L("configRFs closed"));
    CleanupStack::PopAndDestroy(oomConfig);
    iLog->Log(_L("oomconfig pop and desed"));
}

/**
 * STIF_TESTDEFINE defines a test case
 *
 *  Example test case - length of string is checked.
 *  The only argument of macro is a name of test case.
 */
STIF_TESTDEFINE(oomconfigparser_ParseL)
{
    
    iLog->Log(_L("ParseL start"));
    oomConfigParser->ParseL();
    iLog->Log(_L("ParseL returned"));
    
    COomRunPluginConfig& runPC = oomConfig->GetPluginConfig(KTestPluginId);

    iLog->Log(_L("GetPluginConfig returned %x, %x"), runPC.Id(), runPC.TargetApp());
    if( runPC.Id()!=KTestPluginId )
        {
        aResult.SetResult( KErrGeneral, _L("expected plugin did not found") );
        }
    if( runPC.TargetApp()!=KTestTargetAPPId )
        {
        aResult.SetResult( KErrGeneral, _L("expected Target APP id  did not match") );
        }
    aResult.SetResult( KErrNone, _L("ParseL passed"));
}

STIF_TESTDEFINE(oomconfigparser_OnStartElementL_Error)
{
    RTagInfo element;
    RAttributeArray attrArray;
    
    STIF_ASSERT_PANIC(KOomErrBadXml, oomConfigParser->OnStartElementL(element, attrArray, KErrGeneral));
}

// call OnStartDocumentL, no leave
STIF_TESTDEFINE(oomconfigparser_OnStartDocumentL)
    {
    const RDocumentParameters configDocument;
    TRAP(err, oomConfigParser->OnStartDocumentL(configDocument, KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);    
    }

// call OnEndDocumentL, no leave
STIF_TESTDEFINE(oomconfigparser_OnEndDocumentL)
    {
    TRAP(err, oomConfigParser->OnEndDocumentL(KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);    
    }

// call OnStartPrefixMappingL, no leave
STIF_TESTDEFINE(oomconfigparser_OnStartPrefixMappingL)
    {
    RString par1;
    RString par2;
    
    TRAP(err, oomConfigParser->OnStartPrefixMappingL(par1, par2, KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);    
    }

STIF_TESTDEFINE(oomconfigparser_OnEndPrefixMappingL)
    {
    RString par1;
    
    TRAP(err, oomConfigParser->OnEndPrefixMappingL(par1, KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);    
    }

STIF_TESTDEFINE(oomconfigparser_OnIgnorableWhiteSpaceL)
    {
    TPtrC8 par1; 
    
    TRAP(err, oomConfigParser->OnIgnorableWhiteSpaceL(par1, KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);        
    }
STIF_TESTDEFINE(oomconfigparser_OnSkippedEntityL)
    {
    RString par1;
    
    TRAP(err, oomConfigParser->OnSkippedEntityL(par1, KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);    
    }

STIF_TESTDEFINE(oomconfigparser_OnProcessingInstructionL)
    {
    TPtrC8 par1; 
    TPtrC8 par2; 
    
    TRAP(err, oomConfigParser->OnProcessingInstructionL(par1, par2, KErrNone));
    STIF_ASSERT_EQUALS(err, KErrNone);        
    }

STIF_TESTDEFINE(oomconfigparser_OnError)
    {
    oomConfigParser->OnError(KErrNone);
    }
STIF_TESTDEFINE(oomconfigparser_GetExtendedInterface)
    {
    TInt32 par1(KErrNone);
    TAny* retPtr;
    retPtr = oomConfigParser->GetExtendedInterface(par1);
    STIF_ASSERT_NULL(retPtr);        
    }
STIF_TESTDEFINE(OomIdleTimeRule_RuleIsApplicable)
    {

    RWsSession ws;
    err = ws.Connect();
    
    if( err!=KErrNone)
        {
        ws.Close();
        aResult.SetResult( KErrGeneral, _L("WsSession connect() return error") );
        }
    else
        {    
        CleanupClosePushL(ws);
        oomConfigParser->ParseL();

        COomWindowGroupList* oomWindowGroupList = COomWindowGroupList::NewL(ws);
        CleanupStack::PushL(oomWindowGroupList);

        // get appconfig
        COomCloseAppConfig* oomCloseAppConfig = oomConfig->GetApplicationConfig(KOomDefaultAppId).GetAppCloseConfig();
        STIF_ASSERT_NOT_NULL(oomCloseAppConfig);
        // get prioity, as we did not wait about 15 minutes, so default value -- 80 returned.
        TUint priority = oomCloseAppConfig->CalculateCloseAppPriority(*oomWindowGroupList, oomWindowGroupList->Count() - 1);
        iLog->Log(_L("call CalculateCloseAppPriority returned with %d"), priority);
        STIF_ASSERT_EQUALS(priority, KTestDefaultAppPriority);      
            
        CleanupStack::PopAndDestroy(oomWindowGroupList);
        CleanupStack::PopAndDestroy();  //ws
        }
    }

STIF_TESTDEFINE(OomForegroundRule_RuleIsApplicable)
{
    
    RWsSession ws;
    err = ws.Connect();
    if( err!=KErrNone)
        {
        ws.Close();
        aResult.SetResult( KErrGeneral, _L("WsSession connect() return error") );
        }
    else
        {    
        CleanupClosePushL(ws);        
        
        oomConfigParser->ParseL();
        iLog->Log(_L("ParseL returned"));

        COomWindowGroupList* oomWindowGroupList = COomWindowGroupList::NewL(ws);
        CleanupStack::PushL(oomWindowGroupList);

        COomRunPluginConfig& runPC = oomConfig->GetPluginConfig(KTestPluginId);
        
        iLog->Log(_L("GetPluginConfig returned %x, %x"), runPC.Id(), runPC.TargetApp());
        if( runPC.Id()!=KTestPluginId )
            {
            aResult.SetResult( KErrGeneral, _L("expected plugin did not found") );
            }
        else
            {            
            TUint priority = runPC.CalculatePluginPriority(*oomWindowGroupList);
            // as this plugin's target app is Browser, which is not on foreground by now
            // so we got default priority --30 returned
            iLog->Log(_L("call CalculateCloseAppPriority returned with %d"), priority);
            STIF_ASSERT_EQUALS(priority, KTestPluginPriority);      
            }
        
        CleanupStack::PopAndDestroy(oomWindowGroupList);
        CleanupStack::PopAndDestroy();  //ws
        }
}
STIF_TESTDEFINE(oomconfigparser_SetForceCheckConfigL)
    {
    //const TUint KCurrPluginId = 0x200267DB;
    TInt expectedPriority = 100;
    RTagInfo elementTagInfo;
    RAttribute priAttr;
    RAttribute idleAttr;
    RAttributeArray elementAttrs;

    // tag name and attribute
    CleanupClosePushL(elementTagInfo);
    CleanupClosePushL(priAttr);
    CleanupClosePushL(idleAttr);
    CleanupClosePushL(elementAttrs);    
    
    
    // string table
    _LIT8(KOomConfigForceCheckAtPriority, "force_check");
    TPtrC8 par1(KOomConfigForceCheckAtPriority); 
    _LIT8(KOomAttibutePriority, "priority");    
    TPtrC8 par2(KOomAttibutePriority); 
    _LIT8(KValue, "100");
    TPtrC8 par3(KValue); 

    _LIT8(KOomAttributePluginIdlePriority, "plugin_idle_priority");
    TPtrC8 par11(KOomAttributePluginIdlePriority); 
    _LIT8(KOomAttibuteIdleTime, "idle_time");    
    TPtrC8 par12(KOomAttibuteIdleTime); 
    _LIT8(KIdleTimeValue, "900");
    TPtrC8 par13(KIdleTimeValue); 
    _LIT8(KOomAttibuteIdlePriority, "priority");
    TPtrC8 par14(KOomAttibuteIdlePriority); 
    _LIT8(KPriorityValue, "100");
    TPtrC8 par15(KPriorityValue);
    
    // Intermediate table of pointers to strings
    const void * const KStringPointers2[] =    
            {    
            (const void*)&KOomConfigForceCheckAtPriority,    
            (const void*)&KOomAttibutePriority,    
            (const void*)&KValue,    
            (const void*)&KOomAttributePluginIdlePriority,    
            (const void*)&KOomAttibuteIdleTime,    
            (const void*)&KIdleTimeValue,    
            (const void*)&KOomAttibuteIdlePriority,    
            (const void*)&KPriorityValue    
            };

    const TStringTable exampleStringTable = {8, KStringPointers2, EFalse};   
    iLog->Log(_L("1"));
    // open string table and push to cleanup
    RStringPool stringPool;
    stringPool.OpenL(exampleStringTable);
    CleanupClosePushL(stringPool);

    // all rstrings
    RString pluginIdlePriority;         
    RString idleTimeName;
    RString idleTimeValue;     
    RString idlePriorityName;
    RString idlePriorityValue;
    RString forceCheck;
    RString attrNamePriority;
    RString attrValuePriority;
    
    pluginIdlePriority = stringPool.OpenStringL(par11);    
    CleanupClosePushL(pluginIdlePriority);
    
    idleTimeName = stringPool.OpenStringL(par12);
    CleanupClosePushL(idleTimeName);
    idleTimeValue = stringPool.OpenStringL(par13);    
    CleanupClosePushL(idleTimeValue);
    
    idlePriorityName = stringPool.OpenStringL(par14);
    CleanupClosePushL(idlePriorityName);
    idlePriorityValue = stringPool.OpenStringL(par15);    
    CleanupClosePushL(idlePriorityValue);

    forceCheck = stringPool.OpenStringL(par1);
    CleanupClosePushL(forceCheck);

    attrNamePriority = stringPool.OpenStringL(par2);
    CleanupClosePushL(attrNamePriority);
    attrValuePriority = stringPool.OpenStringL(par3);
    CleanupClosePushL(attrValuePriority);
    iLog->Log(_L("2"));

    iLog->Log(_L("start oomconfigparser_SetForceCheckConfigL"));
    
    
    oomConfigParser->ParseL();
    iLog->Log(_L("ParseL returned"));

    // insert a rule for force check
    elementTagInfo.Open(forceCheck, forceCheck, forceCheck);
    priAttr.Open(forceCheck, forceCheck, attrNamePriority, attrValuePriority);
    elementAttrs.AppendL(priAttr);
    
    iLog->Log(_L("3"));
    
    oomConfigParser->OnStartElementL(elementTagInfo, elementAttrs, KErrNone);
    iLog->Log(_L("4"));
    
    // get global force_check rule with non-exist priority, should return a null pointer
    TUint *priority(NULL);
    priority = oomConfig->GlobalConfig().iForceCheckPriorities.Find(expectedPriority-1);
    STIF_ASSERT_NULL(priority);
    // get global force_check rule with a priority value that just added in
    // so should return a correct value
    priority = oomConfig->GlobalConfig().iForceCheckPriorities.Find(expectedPriority);
    STIF_ASSERT_NOT_NULL(priority);
    iLog->Log(_L("call CalculateCloseAppPriority returned with %d"), *priority);
    
    // reset element content for ...
    elementAttrs.Close();
    priAttr.Close();
    elementTagInfo.Close();
    
    // insert a rule for plugin idle time    
    elementTagInfo.Open(pluginIdlePriority, pluginIdlePriority, pluginIdlePriority);
    
    idleAttr.Open(pluginIdlePriority, pluginIdlePriority, idleTimeName, idleTimeValue);
    priAttr.Open(pluginIdlePriority, pluginIdlePriority, idlePriorityName, idlePriorityValue);
    elementAttrs.AppendL(idleAttr);
    elementAttrs.AppendL(priAttr);
    
    iLog->Log(_L("3"));
    
    oomConfigParser->OnStartElementL(elementTagInfo, elementAttrs, KErrNone);
    iLog->Log(_L("add a fake plugin idle priority rule successfully."));
    
    
    CleanupStack::PopAndDestroy(13);      //
    /*
    // close all RStrings
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    CleanupStack::PopAndDestroy();      //
    
    // string pool
    CleanupStack::PopAndDestroy();      //
    //
    CleanupStack::PopAndDestroy();      //elementAttrs
    CleanupStack::PopAndDestroy();      //idleAttr
    CleanupStack::PopAndDestroy();      //priAttr
    CleanupStack::PopAndDestroy();      //elementTagInfo
    */ 
    }

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
