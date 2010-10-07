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
* Description: This file contains STIFUnit implementation.
*
*/

/**
 * STIF_UNIT_INCLUDE SECTION - put all #includes between STIF_UNIT_INCLUDE_SECTION
 *                             and STIF_UNIT_INCLUDE_SECTION_END
 */
#ifdef STIF_UNIT_INCLUDE_SECTION
#include <e32base.h> 
#include "oomapplicationconfig.h"
#include "oomactionlist.h"
#include "oommemorymonitor.h"
#include "oomclientrequestqueue.h"
#include "oomcloseappconfig.h"
#include "oomidletimerule.h"
#include "oomforegroundrule.h"
#include "oomconstants.hrh"
#include "oomconfig.h"
#include "oomrunpluginconfig.h"
#include "oomwindowgrouplist.h"
#include "oompanic.h"
#include "oomsubscribehelper.h"
#include "ut_oomtesttimer.h"

#include "mock_oomactionlist.h"
#include "oomcloseapp.h"
#include "oomrunplugin.h"
#include "oommonitor.h"
#include "oomactionref.h"
#include "oomfriendclass.h"

#include <w32std.h>
#include <e32property.h>

const TUint Koomplugid = 0x2003397f;
const TUid KUtOomAppUid = {0x20033981};
const TInt KAppIndex = 0x12345;
const TInt KPlugFreeMem = 100;
const TInt KTestPriority = 100;
const TInt KDefaultOomVal = 255;

// Used for flagging called of callback method
static TBool isSubscribeCallbackCalled;

/*
 * Test function used as callback of CSubscribeHelper
 */
static TInt SubsribeCallbackStub(TAny* /*aPtr*/)
    {
    isSubscribeCallbackCalled = ETrue;
    CActiveScheduler::Stop();
    return KErrNone;
    }


#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
    RWsSession ws;
    RFs fs;
    
    CActiveScheduler* scheduler;
    CMemoryMonitor* oomMon;
    COomActionList* oomAct;
    COomFriendClass* oomFriend;
    COomActionList* oomActionList;
    COomForegroundRule* oomForeground;
    COomCloseAppConfig* oomActionCfg;
    COomRunPluginConfig* oomRunActionCfg;
    COomWindowGroupList* oomWinGroup;

    COomCloseApp* oomAction;
    COomRunPlugin* oomRunPlugin;
    COomRunPluginConfig* oomRunConfig;
    CAppOomMonitorPlugin* oomMonitorPlugin;
    CMoc_COomActionList* oomActList;
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
    // Install active scheduler at start up
    User::LeaveIfError(ws.Connect());
    CleanupClosePushL(ws);
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    STIF_LOG("Create RProperty of ut_oom");
    RProperty::Define( KUtOomWatcherTestProperty, KUtOomPluginWatcherTestKey, RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
    }

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
    {
    RProperty::Delete(KUtOomWatcherTestProperty, KUtOomPluginWatcherTestKey);
    // Delete active scheduler at last
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
    CleanupStack::PopAndDestroy(&fs);
    CleanupStack::PopAndDestroy(&ws);
    REComSession::FinalClose();
    }

/**
 * STIF_TESTDEFINE defines a test case
 */
 
 /**
 * @SYMTestCaseID              GAPS-OOM-ACTIONLIST-001
 * @SYMTestCaseDesc            To test create COomActionList object 
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create CMemoryMonitor object
 *                             
 * @SYMTestExpectedResults     1) After COomActionList build success, MemoryGood feed back form  plugin Property
 *                             
 * @SYMTestType                Unit Test
 * @SYMCreationDate            13-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-ACTIONLIST-001)
    {
    TInt err;
    TInt val;
    RProperty oomProperty;
    STIF_LOG("Create CMocMemoryMonitor Obj");
    STIF_ASSERT_NOT_LEAVES(oomMon = CMemoryMonitor::NewL());
    CleanupStack::PushL(oomMon);
    STIF_ASSERT_NOT_LEAVES(oomFriend = COomFriendClass::NewL());
    CleanupStack::PushL(oomFriend);
    oomAct = oomFriend->GetOomActionList(oomMon);
    STIF_ASSERT_NOT_NULL(oomAct);
    oomMon->AppNotExiting(0);
    oomAct->SwitchOffPlugins();
    oomAct->StateChanged();
    STIF_ASSERT_NOT_LEAVES(oomMon->StartFreeSomeRamL(KPlugFreeMem, KDefaultOomVal));
    err = oomProperty.Attach(KUtOomWatcherTestProperty, KUtOomPluginWatcherTestKey, EOwnerThread);
    CleanupClosePushL(oomProperty);
    STIF_ASSERT_EQUALS(err, KErrNone);
    err = oomProperty.Get(val);
    STIF_ASSERT_EQUALS(err, KErrNone);
    STIF_ASSERT_EQUALS(val,KUtOomPluginMemGood);
    STIF_LOG("After COomActionList called, MemoryGood feed back form  plugin Property");
    CleanupStack::PopAndDestroy(&oomProperty);
    CleanupStack::PopAndDestroy(oomFriend);
    CleanupStack::PopAndDestroy(oomMon);
    }

/**
 * @SYMTestCaseID              GAPS-OOM-ACTION-002
 * @SYMTestCaseDesc            To test COomActionConfig class 
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomActionConfig(COomCloseAppConfig) object
 *                             2) Create COomForegroundRule Obj
 *                             3) Add rule of Foreground
 *                             4) Create OomWindowGroupList Obj
 *                             5) Set default priority to 0
 *                             6) Check if return default priority  
 *                             
 * @SYMTestExpectedResults     1) Create COomActionConfig object success
 *                             2) Create foreground rule success
 *                             3) Add rule success and no leave
 *                             4) Create empty window group no leave   
 *                             5) Set default prority of action config no error
 *                             6) empty wondow group priority is same as default priority                          
 * @SYMTestType                Unit Test
 * @SYMCreationDate            13-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-ACTION-002)
    {
    TUint priority=0;
    STIF_LOG("1) Create COomActionConfig(COomCloseAppConfig) Object");
    STIF_ASSERT_NOT_LEAVES(oomActionCfg = COomCloseAppConfig::NewL(0));
    CleanupStack::PushL(oomActionCfg);

    STIF_LOG("2) Create COomForegroundRule Obj");
    STIF_ASSERT_NOT_LEAVES(oomForeground = new (ELeave) COomForegroundRule(0,0));
    CleanupStack::PushL(oomForeground);

    STIF_LOG("3) Add rule of Foreground");
    STIF_ASSERT_NOT_LEAVES(oomActionCfg->AddRuleL(oomForeground));
    CleanupStack::Pop(oomForeground);

    STIF_LOG("4) Create OomWindowGroupList Obj");
    STIF_ASSERT_NOT_LEAVES(oomWinGroup = COomWindowGroupList::NewL(ws));
    CleanupStack::PushL(oomWinGroup);

    STIF_LOG("5) Set default priority");
    oomActionCfg->SetDefaultPriority(priority);
    
    STIF_LOG("6) Check if return default priority");
    STIF_ASSERT_EQUALS(oomActionCfg->CalculateCloseAppPriority(*oomWinGroup,0),priority);

    STIF_LOG("Clean Enviroment");
    CleanupStack::PopAndDestroy(oomWinGroup);
    CleanupStack::PopAndDestroy(oomActionCfg);
    }

/**
 * @SYMTestCaseID              GAPS-OOM-ACTION-003
 * @SYMTestCaseDesc            To test COomActionConfig class 
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomActionConfig(COomRunPluginConfig) object
 *                             2) Create a foreground rule
 *                             3) Add foreground rule into action config
 *                             4) Create a empty window group
 *                             5) Set default priority of action config to 0
 *                             6) Get empty window group priority   
 *                             
 * @SYMTestExpectedResults     1) Create COomRunPluginConfig object success
 *                             2) Create foreground rule success
 *                             3) Add rule success and no leave
 *                             4) Create empty window group no leave   
 *                             5) Set default prority of action config no error
 *                             6) empty wondow group priority is same as default priority                          
 * @SYMTestType                Unit Test
 * @SYMCreationDate            13-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-ACTION-003)
    {
    TUint priority=0;
    STIF_LOG("1) Create COomActionConfig(COomCloseAppConfig) Object");
    STIF_ASSERT_NOT_LEAVES(oomRunActionCfg = COomRunPluginConfig::NewL(Koomplugid,EOomAppPlugin));
    CleanupStack::PushL(oomRunActionCfg);

    STIF_LOG("2) Create COomForegroundRule Obj");
    STIF_ASSERT_NOT_LEAVES(oomForeground = new (ELeave) COomForegroundRule(0,0));
    CleanupStack::PushL(oomForeground);

    STIF_LOG("3) Add rule of Foreground");
    STIF_ASSERT_NOT_LEAVES(oomRunActionCfg->AddRuleL(oomForeground));
    CleanupStack::Pop(oomForeground);

    STIF_LOG("4) Create OomWindowGroupList Obj");
    STIF_ASSERT_NOT_LEAVES(oomWinGroup = COomWindowGroupList::NewL(ws));
    CleanupStack::PushL(oomWinGroup);

    STIF_LOG("5) Set default priority");
    oomRunActionCfg->SetDefaultPriority(priority);
    
    STIF_LOG("6) Check if return default priority");
    STIF_ASSERT_EQUALS(oomRunActionCfg->CalculatePluginPriority(*oomWinGroup),priority);

    STIF_LOG("Clean Enviroment");
    CleanupStack::PopAndDestroy(oomWinGroup);
    CleanupStack::PopAndDestroy(oomRunActionCfg);
    }

/**
 * @SYMTestCaseID              GAPS-OOM-OOMMONITORPLUGIN-004
 * @SYMTestCaseDesc            To test COomActionConfig class 
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomRunPluginConfig Object
 *                             2) Create COomActionList Object
 *                             3) Create CMemoryMonitor Object
 *                             4) Create COomCloseApp Object
 *                             5) Call COomAction::FreeMemory
 *                             
 * @SYMTestExpectedResults     1) Create COomRunPluginConfig object success
 *                             2) Create COomActionList object success
 *                             3) Create CMemoryMonitor success
 *                             4) Create COomCloseApp  OK
 *                             5) oomActList->GetState() and the value is default
 * @SYMTestType                Unit Test
 * @SYMCreationDate            13-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMMONITORPLUGIN-004)
    {
    STIF_LOG("1) Create COomRunPluginConfig Object");
    STIF_ASSERT_NOT_LEAVES(oomRunConfig = COomRunPluginConfig::NewL(Koomplugid,EOomSystemPlugin));
    CleanupStack::PushL(oomRunConfig);
    
    STIF_LOG("2) Create COomActionList Object");
    STIF_ASSERT_NOT_LEAVES(oomActList = new (ELeave) CMoc_COomActionList());
    CleanupStack::PushL(oomActList);
    
    STIF_LOG("3) Create CMemoryMonitor Object");
    STIF_ASSERT_NOT_LEAVES(oomMon = CMemoryMonitor::NewL());
    CleanupStack::PushL(oomMon);
    Dll::SetTls(oomMon);

    STIF_LOG("4) Create COomCloseApp Object");
    STIF_ASSERT_NOT_LEAVES(oomAction = COomCloseApp::NewL(*oomActList,ws));
    CleanupStack::PushL(oomAction);

    STIF_LOG("5) Call COomAction::FreeMemory");
    oomAction->FreeMemory(0,EFalse);
    CActiveScheduler::Start();
    
    STIF_ASSERT_NOT_EQUALS(oomActList->GetState(),0);

    STIF_LOG("Clean Enviroment");
    CleanupStack::PopAndDestroy(oomAction);
    CleanupStack::PopAndDestroy(oomMon);
    CleanupStack::PopAndDestroy(oomActList);
    CleanupStack::PopAndDestroy(oomRunConfig);
    }

/**
 * @SYMTestCaseID              GAPS-OOM-OOMRUNPLUGIN-005
 * @SYMTestCaseDesc            To test COomMonitorPlugin(COomRunPlugin) class 
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomRunPluginConfig Object
 *                             2) Create COomActionList Object
 *                             3) Create CMemoryMonitor Object
 *                             4) Create COomMonitorPlugin(CAppOomMonitorPlugin) Object
 *                             5) Create COomMonitorPlugin(COomRunPlugin) Object
 *                             6) Free Some memory
 *                             7) Create TActionRef object
 *                             
 * @SYMTestExpectedResults     1) Create COomRunPluginConfig object success
 *                             2) Create COomActionList success
 *                             3) Create CMemoryMonitor success
 *                             4) Create COomMonitorPlugin no leave   
 *                             5) Create COomMonitorPlugin(COomRunPlugin) no leave
 *                             6) Free memory OK   
 *                             7) Get value of TActionRef is same as seting                     
 * @SYMTestType                Unit Test
 * @SYMCreationDate            13-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMRUNPLUGIN-005)
    {

    STIF_LOG("1) Create COomRunPluginConfig Object");
    STIF_ASSERT_NOT_LEAVES(oomRunConfig = COomRunPluginConfig::NewL(Koomplugid,EOomSystemPlugin));
    CleanupStack::PushL(oomRunConfig);

    STIF_LOG("2) Create COomActionList Object");
    STIF_ASSERT_NOT_LEAVES(oomActList = new (ELeave) CMoc_COomActionList());
    CleanupStack::PushL(oomActList);
    
    STIF_LOG("3) Create CMemoryMonitor Object");
    STIF_ASSERT_NOT_LEAVES(oomMon = CMemoryMonitor::NewL());
    CleanupStack::PushL(oomMon);
    Dll::SetTls(oomMon);

    STIF_LOG("4) Create COomMonitorPlugin(CAppOomMonitorPlugin) Object");
    STIF_ASSERT_NOT_LEAVES(oomMonitorPlugin = CAppOomMonitorPlugin::NewL(KUtOomAppUid));
    CleanupStack::PushL(oomMonitorPlugin);

    STIF_LOG("5) Create COomMonitorPlugin(COomRunPlugin) Object");
    STIF_ASSERT_NOT_LEAVES(oomRunPlugin = COomRunPlugin::NewL(Koomplugid,*oomRunConfig,*oomActList,*oomMonitorPlugin));
    CleanupStack::PushL(oomRunPlugin);
    
    STIF_LOG("6) Free Some memory");
    oomRunPlugin->FreeMemory(KPlugFreeMem,0);
    CActiveScheduler::Start();
    STIF_ASSERT_NOT_EQUALS(oomActList->GetState(),0);
    oomRunPlugin->MemoryGood();
    TActionRef::TActionType type(TActionRef::ESystemPlugin);
    TUint priority(KDefaultOomVal);
    TInt ramEstimate(KDefaultOomVal);
    TOomSyncMode sync(EContinue);
    TUint wgIndexOfTargetApp(KAppIndex);
    TInt  wgId(KDefaultOomVal);
    TInt wgIndex(KDefaultOomVal);
    
    STIF_LOG("7) Create TActionRef object");
    TActionRef actionRef(type,priority,sync,ramEstimate,*oomRunPlugin,wgIndexOfTargetApp);
    STIF_ASSERT_EQUALS(actionRef.Type(),type);
    STIF_ASSERT_EQUALS(actionRef.Priority(),priority);
    STIF_ASSERT_EQUALS(actionRef.SyncMode(),sync);
    STIF_ASSERT_EQUALS(actionRef.RamEstimate(),ramEstimate);
    STIF_ASSERT_EQUALS(&actionRef.RunPlugin(),oomRunPlugin);
    
    actionRef = TActionRef(type,priority,sync,ramEstimate,wgId,wgIndex);
    STIF_ASSERT_EQUALS(actionRef.WgId(),wgId);
    STIF_ASSERT_EQUALS(actionRef.WgIndex(),wgIndex);
    
    CleanupStack::PopAndDestroy(oomRunPlugin);
    CleanupStack::PopAndDestroy(oomMonitorPlugin);
    CleanupStack::PopAndDestroy(oomMon);
    CleanupStack::PopAndDestroy(oomActList);
    CleanupStack::PopAndDestroy(oomRunConfig);
    }
    
/**
 * @SYMTestCaseID              GAPS-OOM-OOMWATCHER-006
 * @SYMTestCaseDesc            To test oommonitor
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Call CreateOOMWatcherThreadL
 *                             
 * @SYMTestExpectedResults     1) CreateOOMWatcherThreadL no leave
 * @SYMTestType                Unit Test
 * @SYMCreationDate            13-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMWATCHER-006)
    {
    STIF_LOG("1) call CreateOOMWatcherThreadL");
    STIF_ASSERT_NOT_LEAVES(CreateOOMWatcherThreadL());
    }

/**
 * @SYMTestCaseID              GAPS-OOM-OOMPANIC-007
 * @SYMTestCaseDesc            To test OomMonitorPluginPanic  
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Set TLS to NULL
 *                             2) Try to create a CAppOomMonitorPlugin
 *                             
 * @SYMTestExpectedResults     1) Set TLS to NULL success
 *                             2) Create CAppOomMonitorPlugin been panic 
 * @SYMTestType                Unit Test
 * @SYMCreationDate            01-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMPANIC-007)
    {
    STIF_LOG("1) Set TLS to NULL");
    Dll::SetTls(NULL);
    STIF_LOG("2) Create COomMonitorPlugin(CAppOomMonitorPlugin) Object");
    STIF_ASSERT_PANIC(0,oomMonitorPlugin = CAppOomMonitorPlugin::NewL(KUtOomAppUid));
    delete oomMonitorPlugin;
    }

/**
 * @SYMTestCaseID              GAPS-OOM-APPLICATIONCONFIG-001
 * @SYMTestCaseDesc            Unit test for COomApplicationConfig and COomCloseAppConfig.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomApplicationConfig object.
 *                             2. Check applicatin ID is correct.
 *                             3. Construct a COomCloseAppConfig object.
 *                             4. Set close app config for the COomApplicationConfig object.
 *                             5. Check close app config for the COomApplicationConfig object.
 *                             6. Construct a COomForegroundRule object.
 *                             7. Add foreground rule into the COomApplicationConfig object.
 *                             8. Construct a COomIdleTimeRule object.
 *                             9. Add idle time rule into the COomApplicationConfig object.
 * @SYMTestExpectedResults     1. COomApplicationConfig object was successfully constructed.
 *                             2. Application ID is correct.
 *                             3. COomCloseAppConfig object was successfully constructed.
 *                             4. Close app config was set for the COomApplicationConfig object.
 *                             5. Close app config is correct in the COomApplicationConfig object.
 *                             6. COomForegroundRule object was successfully constructed.
 *                             7. COomForegroundRule object was successfully added into COomApplicationConfig object.
 *                             8. COomIdleTimeRule object was successfully constructed.
 *                             9. COomIdleTimeRule object was successfully added into COomApplicationConfig object.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-APPLICATIONCONFIG-001)
{
    // 
    STIF_LOG("1. Construct a COomApplicationConfig object.");
    TUint applicationID = 0x100058B3;
    COomApplicationConfig* applicationConfig = COomApplicationConfig::NewL(applicationID);
    CleanupStack::PushL(applicationConfig);

    STIF_LOG("2. Check applicatin ID is correct.");
    STIF_ASSERT_EQUALS(applicationConfig->Id(), applicationID);

    STIF_LOG("3. Construct a COomCloseAppConfig object.");
    COomCloseAppConfig* closeAppConfig = COomCloseAppConfig::NewL(applicationID);
    
    CleanupStack::PushL(closeAppConfig);

    STIF_LOG("4. Set close app config for the COomApplicationConfig object.");
    applicationConfig->SetAppCloseConfig(closeAppConfig);
    
    CleanupStack::Pop(closeAppConfig);

    STIF_LOG("5. Check close app config for the COomApplicationConfig object.");
    STIF_ASSERT_EQUALS(applicationConfig->GetAppCloseConfig(), closeAppConfig);

    STIF_LOG("6. Construct a COomForegroundRule object.");
    COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(KOomBusyAppId, KOomPriorityInfinate);
    CleanupStack::PushL(foregroundRule);

    STIF_LOG("7. Add foreground rule into the COomApplicationConfig object.");
    applicationConfig->AddRuleL(foregroundRule);
    CleanupStack::Pop(foregroundRule);

    STIF_LOG("8. Construct a COomIdleTimeRule object.");
    COomIdleTimeRule* idleRule = COomIdleTimeRule::NewL(KMicrosecondsInMillisecond, KOomPriorityInfinate);
    CleanupStack::PushL(idleRule);

    STIF_LOG("9. Add idle time rule into the COomApplicationConfig object.");
    applicationConfig->AddRuleL(idleRule);
    CleanupStack::Pop(idleRule);
    
    CleanupStack::PopAndDestroy(applicationConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-001
 * @SYMTestCaseDesc            Unit test for COomConfig::AddApplicationConfigL.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add default application config.
 *                             3. Add default application config again.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Default application config was added.
 *                             3. Panic with KAppConfigAddedTwice.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-001)
{
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);
    
    STIF_LOG("2. Add default application config.");
    COomApplicationConfig* defaultApplicationConfig = COomApplicationConfig::NewL(KOomDefaultAppId);
    CleanupStack::PushL(defaultApplicationConfig);
    
    oomConfig->AddApplicationConfigL(defaultApplicationConfig);
    
    STIF_LOG("3. Add default application config again.");
    STIF_ASSERT_PANIC(KAppConfigAddedTwice, oomConfig->AddApplicationConfigL(defaultApplicationConfig));
    CleanupStack::Pop(defaultApplicationConfig);

    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-002
 * @SYMTestCaseDesc            Unit test for COomConfig::GetApplicationConfig.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add default application config.
 *                             3. Get another application config.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Default application config was added.
 *                             3. KOomDefaultAppId should be returned.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-002)
{
    TUint applicationID = 0x100058B3;

    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);
    
    STIF_LOG("2. Add default application config.");
    COomApplicationConfig* defaultApplicationConfig = COomApplicationConfig::NewL(KOomDefaultAppId);
    CleanupStack::PushL(defaultApplicationConfig);
    
    oomConfig->AddApplicationConfigL(defaultApplicationConfig);

    CleanupStack::Pop(defaultApplicationConfig);

    STIF_LOG("3. Get application config.");
    COomApplicationConfig& defaultAppConfig = oomConfig->GetApplicationConfig(applicationID);
    STIF_ASSERT_EQUALS(defaultAppConfig.Id(), (TUint)KOomDefaultAppId);

    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-003
 * @SYMTestCaseDesc            Unit test for COomConfig add rule config before adding application config.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add rule config.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Panic with KRuleConfiguredBeforeApplication.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-003)
{
    TUint applicationID = 0x100058B3;

    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);
    
    STIF_LOG("2. Add rule config.");
    COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(applicationID, KOomPriorityInfinate);
    CleanupStack::PushL(foregroundRule);
    
    STIF_ASSERT_PANIC(KRuleConfiguredBeforeApplication, oomConfig->AddApplicationRuleL(applicationID, foregroundRule));
   
    CleanupStack::Pop(foregroundRule);

    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-004
 * @SYMTestCaseDesc            Unit test for COomConfig add the same configuration for an application closure twice.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add the configuration for an application closure.
 *                             3. Add the same configuration for an application closure again.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Application config was created. Application closure was added.
 *                             3. Panic with KSameAppClosureConfiguredTwice.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-004)
{
    TUint applicationID = 0x100058B3;

    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);

    STIF_LOG("2. Add the configuration for an application closure.");
    COomCloseAppConfig* closeAppConfig = COomCloseAppConfig::NewL(applicationID);
    CleanupStack::PushL(closeAppConfig);
    
    oomConfig->SetAppCloseConfigL(closeAppConfig);
    
    STIF_LOG("3. Add the same configuration for an application closure again.");
    STIF_ASSERT_PANIC(KSameAppClosureConfiguredTwice, oomConfig->SetAppCloseConfigL(closeAppConfig));
    
    CleanupStack::Pop(closeAppConfig);

    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-005
 * @SYMTestCaseDesc            Unit test for COomConfig add the same rule configuration.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add the configuration for an application closure.
 *                             3. Get application config.
 *                             4. Add the rule config.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Application config was created. Application closure was added.
 *                             3. Correct application id should be returned.
 *                             4. The rule config was added.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-005)
{
    TUint applicationID = 0x100058B3;

    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);

    STIF_LOG("2. Add the configuration for an application closure.");
    COomCloseAppConfig* closeAppConfig = COomCloseAppConfig::NewL(applicationID);
    CleanupStack::PushL(closeAppConfig);
    
    oomConfig->SetAppCloseConfigL(closeAppConfig);

    CleanupStack::Pop(closeAppConfig);

    STIF_LOG("3. Get application config.");
    COomApplicationConfig& appConfig = oomConfig->GetApplicationConfig(applicationID);
    STIF_ASSERT_EQUALS(appConfig.Id(), applicationID);

    STIF_LOG("4. Add the rule config.");
    COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(applicationID, KOomPriorityInfinate);
    CleanupStack::PushL(foregroundRule);
    
    oomConfig->AddApplicationRuleL(applicationID, foregroundRule);
    
    CleanupStack::Pop(foregroundRule);

    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-006
 * @SYMTestCaseDesc            Unit test for COomConfig::AddPluginConfigL.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add default run plugin config.
 *                             3. Add default run plugin config again.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Default run plugin config was added.
 *                             3. Panic with KPluginConfigAddedTwice.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-006)
{
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);

    STIF_LOG("2. Add default run plugin config.");
    COomRunPluginConfig* runPluginConfig = COomRunPluginConfig::NewL(KOomDefaultPluginId, EOomSystemPlugin);
    CleanupStack::PushL(runPluginConfig);
    
    oomConfig->AddPluginConfigL(runPluginConfig);
    
    STIF_LOG("3. Add default run plugin config again.");
    STIF_ASSERT_PANIC(KPluginConfigAddedTwice, oomConfig->AddPluginConfigL(runPluginConfig));

    CleanupStack::Pop(runPluginConfig);
    
    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-007
 * @SYMTestCaseDesc            Unit test for COomConfig::GetPluginConfig.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add run plugin config.
 *                             3. Get another run plugin config.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Default run plugin config was added.
 *                             3. KOomDefaultPluginId should be returned.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-007)
{
    TUint pluginID = 0x100058B3;
    
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);
    
    STIF_LOG("2. Add default run plugin config.");
    COomRunPluginConfig* runPluginConfig = COomRunPluginConfig::NewL(KOomDefaultPluginId, EOomSystemPlugin);
    CleanupStack::PushL(runPluginConfig);
    
    oomConfig->AddPluginConfigL(runPluginConfig);
    
    CleanupStack::Pop(runPluginConfig);

    STIF_LOG("3. Get another run plugin config.");
    COomRunPluginConfig& defaultPluginConfig = oomConfig->GetPluginConfig(pluginID);
    STIF_ASSERT_EQUALS(defaultPluginConfig.Id(), (TUint)KOomDefaultPluginId);

    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-008
 * @SYMTestCaseDesc            Unit test for COomConfig add rule config before adding run plugin config.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add rule config for run plugin config.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Panic with KRuleConfiguredBeforePlugin.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-008)
{
    TUint pluginID = 0x100058B3;
    
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);
    
    STIF_LOG("2. Add rule config for run plugin config.");
    COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(pluginID, KOomPriorityInfinate);
    CleanupStack::PushL(foregroundRule);
        
    STIF_ASSERT_PANIC(KRuleConfiguredBeforePlugin, oomConfig->AddPluginRuleL(pluginID, foregroundRule));

    CleanupStack::Pop(foregroundRule);
    
    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-009
 * @SYMTestCaseDesc            Unit test for COomConfig add run plugin config twicw.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add the configuration for a plugin action.
 *                             3. Add the configuration for a plugin action again.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Run plugin config was added.
 *                             3. 
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-009)
{
    TUint pluginID = 0x100058B3;
    
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);

    STIF_LOG("2. Add the configuration for a plugin action.");
    COomRunPluginConfig* runPluginConfig = COomRunPluginConfig::NewL(pluginID, EOomSystemPlugin);
    CleanupStack::PushL(runPluginConfig);
        
    oomConfig->AddPluginConfigL(runPluginConfig);

    STIF_LOG("3. Add the same configuration for a plugin action again.");
    STIF_ASSERT_PANIC(KPluginConfigAddedTwice, oomConfig->AddPluginConfigL(runPluginConfig));
    
    CleanupStack::Pop(runPluginConfig);
    
    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-010
 * @SYMTestCaseDesc            Unit test for COomConfig the rule config again.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Add the configuration for a plugin action.
 *                             3. Get run plugin config.
 *                             4. Add the rule config.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. The configuration for a plugin action was added.
 *                             3. Correct run plugin id should be returned.
 *                             4. The rule config was added.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-010)
{
    TUint pluginID = 0x100058B3;
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);

    STIF_LOG("2. Add the configuration for a plugin action.");
    COomRunPluginConfig* runPluginConfig = COomRunPluginConfig::NewL(pluginID, EOomSystemPlugin);
    CleanupStack::PushL(runPluginConfig);
    
    oomConfig->AddPluginConfigL(runPluginConfig);
    
    CleanupStack::Pop(runPluginConfig);

    STIF_LOG("3. Get run plugin config.");
    COomRunPluginConfig& pluginConfig = oomConfig->GetPluginConfig(pluginID);
    STIF_ASSERT_EQUALS(pluginConfig.Id(), pluginID);

    STIF_LOG("4. Add the rule config.");
    COomForegroundRule* foregroundRule = new (ELeave) COomForegroundRule(pluginID, KOomPriorityInfinate);
    CleanupStack::PushL(foregroundRule);
    
    oomConfig->AddPluginRuleL(pluginID, foregroundRule);
    
    CleanupStack::Pop(foregroundRule);
    
    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-OOMCONFIG-011
 * @SYMTestCaseDesc            Unit test for COomConfig iGlobalConfig related functions.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COomConfig object.
 *                             2. Set default low ram threshold.
 *                             3. Set default good ram threshold.
 *                             4. Set swap usage monitored.
 *                             5. Set default low swap threshold.
 *                             6. Set default good swap threshold.
 *                             7. Set max close app batch.
 *                             8. Set default wait after plugin.
 *                             9. Set max app exit time.
 *                             10. Get global config.
 *                             11. Check default low ram threshold.
 *                             12. Check default good ram threshold.
 *                             13. Check swap usage monitored.
 *                             14. Check default low swap threshold.
 *                             15. Check default good swap threshold.
 *                             16. Check max close app batch.
 *                             17. Check default wait after plugin.
 *                             18. Check max app exit time.
 * @SYMTestExpectedResults     1. COomConfig object was successfully constructed.
 *                             2. Default low ram threshold was set.
 *                             3. Default good ram threshold was set.
 *                             4. Swap usage monitored was set.
 *                             5. Default low swap threshold was set.
 *                             6. Default good swap threshold was set.
 *                             7. Max close app batch was set.
 *                             8. Default wait after plugin was set.
 *                             9. Max app exit time was set.
 *                             10. Global config is got.
 *                             11. Default low ram threshold is correct.
 *                             12. Default good ram threshold is correct.
 *                             13. Swap usage monitored is correct.
 *                             14. Default low swap threshold is correct.
 *                             15. Default good swap threshold is correct.
 *                             16. Max close app batch is correct.
 *                             17. Default wait after plugin is correct.
 *                             18. Max app exit time is correct.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-08-2010
 */
STIF_TESTDEFINE(GAPS-OOM-OOMCONFIG-011)
{
    STIF_LOG("1. Construct a COomConfig object.");
    COomConfig* oomConfig = COomConfig::NewL();
    CleanupStack::PushL(oomConfig);
    
    const TInt KBytesInMegabyte = 1024;

    STIF_LOG("2. Set default low ram threshold.");
    oomConfig->SetDefaultLowRamThreshold(KBytesInMegabyte);
    
    STIF_LOG("3. Set default good ram threshold.");
    oomConfig->SetDefaultGoodRamThreshold(KBytesInMegabyte);

    STIF_LOG("4. Set swap usage monitored.");
    oomConfig->SetSwapUsageMonitored(ETrue);
    
    STIF_LOG("5. Set default low swap threshold.");
    oomConfig->SetDefaultLowSwapThreshold(KBytesInMegabyte);

    STIF_LOG("6. Set default good swap threshold.");
    oomConfig->SetDefaultGoodSwapThreshold(KBytesInMegabyte);
    
    STIF_LOG("7. Set max close app batch.");
    TUint maxCloseAppBatch = 100;
    oomConfig->SetMaxCloseAppBatch(maxCloseAppBatch);

    STIF_LOG("8. Set default wait after plugin.");
    oomConfig->SetDefaultWaitAfterPlugin(KMicrosecondsInMillisecond);

    STIF_LOG("9. Set max app exit time.");
    oomConfig->SetMaxAppExitTime(KMicrosecondsInMillisecond);
    
    STIF_LOG("10. Get global config.");
    COomGlobalConfig& globalConfig = oomConfig->GlobalConfig();
    
    STIF_LOG("11. Check default low ram threshold.");
    STIF_ASSERT_EQUALS(globalConfig.iLowRamThreshold, KBytesInMegabyte);
    
    STIF_LOG("12. Check default good ram threshold.");
    STIF_ASSERT_EQUALS(globalConfig.iGoodRamThreshold, KBytesInMegabyte);

    STIF_LOG("13. Check swap usage monitored.");
    STIF_ASSERT_TRUE(globalConfig.iSwapUsageMonitored);

    STIF_LOG("14. Check default low swap threshold.");
    STIF_ASSERT_EQUALS(globalConfig.iLowSwapThreshold, KBytesInMegabyte);

    STIF_LOG("15. Check default good swap threshold.");
    STIF_ASSERT_EQUALS(globalConfig.iGoodSwapThreshold, KBytesInMegabyte);
    
    STIF_LOG("16. Check max close app batch.");
    STIF_ASSERT_EQUALS(globalConfig.iMaxCloseAppBatch, maxCloseAppBatch);

    STIF_LOG("17. Check default wait after plugin.");
    STIF_ASSERT_EQUALS(globalConfig.iDefaultWaitAfterPlugin, KMicrosecondsInMillisecond);

    STIF_LOG("18. Check max app exit time.");
    STIF_ASSERT_EQUALS(globalConfig.iMaxAppExitTime, KMicrosecondsInMillisecond);
    
    CleanupStack::PopAndDestroy(oomConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-PLUGINCONFIG-001
 * @SYMTestCaseDesc            Test constructor of COomRunPluginConfig
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomRunPluginConfig object
 *                             2) Check the values with getter methods
 * @SYMTestExpectedResults     1) Create COomRunPluginConfig object
 *                             2) Check the values with getter methods     
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-PLUGINCONFIG-001)
{
    TUint testPluginId = 1001;
    TOomPluginType testPluginType = EOomAppPlugin;
    COomRunPluginConfig* pluginConfig = COomRunPluginConfig::NewL(testPluginId, testPluginType);
    CleanupStack::PushL(pluginConfig);
    
    // Verify the object is created successfully
    STIF_ASSERT_NOT_NULL(pluginConfig);
    STIF_LOG("COomRunPluginConfig created successfully");
    
    // Verify member variable value
    STIF_ASSERT_EQUALS(pluginConfig->Id(), testPluginId);
    STIF_ASSERT_EQUALS(pluginConfig->PluginType(), testPluginType);
    STIF_ASSERT_TRUE(pluginConfig->CallIfTargetAppNotRunning());
    STIF_LOG("Member variables set in COomRunPluginConfig constuctor has been verified");

    CleanupStack::PopAndDestroy(pluginConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-PLUGINCONFIG-002
 * @SYMTestCaseDesc            Test setters and getters of COomRunPluginConfig
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomRunPluginConfig object
 *                             2) Check the values with getter methods
 *                             3) Call getter/setter methods
 * @SYMTestExpectedResults     1) Create COomRunPluginConfig object
 *                             2) Check the values with getter methods
 *                             3) Getter is getting same value as its paried setter
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-PLUGINCONFIG-002)
{
    TUint testPluginId = 1001;
    TOomPluginType testPluginType = EOomAppPlugin;
    COomRunPluginConfig* pluginConfig = COomRunPluginConfig::NewL(testPluginId, testPluginType);
    CleanupStack::PushL(pluginConfig);
    
    // Verify the object is created successfully
    STIF_ASSERT_NOT_NULL(pluginConfig);
    STIF_LOG("COomRunPluginConfig created successfully");
    
    // Verify member variable value
    STIF_ASSERT_EQUALS(pluginConfig->Id(), testPluginId);
    STIF_ASSERT_EQUALS(pluginConfig->PluginType(), testPluginType);
    STIF_ASSERT_TRUE(pluginConfig->CallIfTargetAppNotRunning());
    STIF_LOG("Member variables set in COomRunPluginConfig constuctor has been verified");
    
    pluginConfig->SetCallIfTargetAppNotRunning(EFalse);
    STIF_ASSERT_FALSE(pluginConfig->CallIfTargetAppNotRunning());
    STIF_LOG("CallIfTargetAppNotRunning setter/getter has been verified");
    
    TInt testTimeToWait = 1002;
    pluginConfig->SetWaitAfterPlugin(testTimeToWait);
    STIF_ASSERT_TRUE(pluginConfig->WaitAfterPluginDefined());
    STIF_ASSERT_EQUALS(pluginConfig->WaitAfterPlugin(), testTimeToWait);
    STIF_LOG("WaitAfterPlugin setter/getter has been verified");
    
    TUint testTargetApp = 1003;
    pluginConfig->SetTargetApp(testTargetApp);
    STIF_ASSERT_EQUALS(pluginConfig->TargetApp(), testTargetApp);
    STIF_LOG("TargetApp setter/getter has been verified");

    CleanupStack::PopAndDestroy(pluginConfig);
}

/**
 * @SYMTestCaseID              GAPS-OOM-PLUGINCONFIG-003
 * @SYMTestCaseDesc            Test COomRunPluginConfig::CalculatePluginPriority
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Create COomRunPluginConfig object
 *                             2) Create a COomWindowGroupList object
 *                             3) Call COomRunPluginConfig::CalculatePluginPriority() using the COomWindowGroupList
 *                                created in step 2 as parameter
 * @SYMTestExpectedResults     1) Create COomRunPluginConfig object
 *                             2) COomWindowGroupList object created
 *                             3) The returned priority is correct 
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-PLUGINCONFIG-003)
{
    TUint testPluginId = 1001;
    TOomPluginType testPluginType = EOomAppPlugin;
    COomRunPluginConfig* pluginConfig = COomRunPluginConfig::NewL(testPluginId, testPluginType);
    CleanupStack::PushL(pluginConfig);
    
    RWsSession ws;
    STIF_ASSERT_EQUALS(ws.Connect(), KErrNone);
    CleanupClosePushL(ws);
    COomWindowGroupList* wgList = COomWindowGroupList::NewL(ws);
    CleanupStack::PushL(wgList);
    
    // Verify the object is created successfully
    STIF_ASSERT_NOT_NULL(pluginConfig);
    STIF_LOG("COomRunPluginConfig created successfully");
    STIF_ASSERT_NOT_NULL(wgList);
    STIF_LOG("COomWindowGroupList created successfully");
    
    // Verify calculate plugin priority
    TUint priority = pluginConfig->CalculatePluginPriority(*wgList);
    TUint expectedPriority = 0;
    STIF_ASSERT_EQUALS(priority, expectedPriority);
    STIF_LOG("calculate priority has been verified");
    
    CleanupStack::PopAndDestroy(3);
}

/**
 * @SYMTestCaseID              GAPS-OOM-PANIC-001
 * @SYMTestCaseDesc            Test OomMonitorPanic
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Panic with OomMonitorPanic
 * @SYMTestExpectedResults     1) The client panic with specified panic code 
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-PANIC-001)
{
    TOomMonitorPanic panicReason = KRuleConfiguredBeforeApplication;
    TestModuleIf().SetExitReason(CTestModuleIf::EPanic, (TInt)panicReason);
    
    OomMonitorPanic(panicReason);
}

/**
 * @SYMTestCaseID              GAPS-OOM-PANIC-002
 * @SYMTestCaseDesc            Test OomConfigParserPanic
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Panic with OomConfigParserPanic
 * @SYMTestExpectedResults     1) The client panic with specified panic code                  
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-PANIC-002)
{
    TInt panicReason = 2;
    TestModuleIf().SetExitReason(CTestModuleIf::EPanic, panicReason);
    
    OomConfigParserPanic(panicReason);
}
     
/**
 * @SYMTestCaseID              GAPS-OOM-SUBSCRIBE-001
 * @SYMTestCaseDesc            Property issued when subscribed
 * @SYMTestPriority            Medium
 * @SYMTestActions             1) Define and attach to an RProperty
 *                             2) Create a CSubscribeHelper object, subscribe to the property create in step 1 
 *                                and specify a callback function
 *                             3) Set the value of the property created in step 1
 * @SYMTestExpectedResults     1) RProperty created and attached to
 *                             2) Object created successfully
 *                             3) The callback function is called 
 * @SYMTestType                Unit Test
 * @SYMCreationDate            20-07-2010
 */
STIF_TESTDEFINE(GAPS-OOM-SUBSCRIBE-001)
{
    TInt KDefaultWaitTimeInMicroSeconds = 5000000;
    isSubscribeCallbackCalled = EFalse;
    _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);

    // Create Callback
    TCallBack testCallback(&SubsribeCallbackStub);
    // Create properties used in test
    const TUid KOomSubscribeTestProperty = { 0x10000000 };
    const TUint32 KOomSubscribeTestKey = 0x00000001;
    RProperty testSubscribeProperty;
    RProperty testSetProperty;
    TInt error;
    error = RProperty::Define(KOomSubscribeTestProperty, KOomSubscribeTestKey, RProperty::EInt, 
                              KAllowAllPolicy, KAllowAllPolicy);
    STIF_ASSERT_EQUALS(error, KErrNone);
    error = testSubscribeProperty.Attach(KOomSubscribeTestProperty, KOomSubscribeTestKey, EOwnerThread);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(testSubscribeProperty);
    error = testSetProperty.Attach(KOomSubscribeTestProperty, KOomSubscribeTestKey, EOwnerThread);
    STIF_ASSERT_EQUALS(error, KErrNone);
    CleanupClosePushL(testSetProperty);
    STIF_LOG("Properties created successfully");

    // Create subscribe helper
    CSubscribeHelper* subscribeHelper = new (ELeave) CSubscribeHelper(testCallback, testSubscribeProperty);
    CleanupStack::PushL(subscribeHelper);
    // Verify the object is created successfully
    STIF_ASSERT_NOT_NULL(subscribeHelper);
    STIF_LOG("CSubscribeHelper created successfully");

    CUt_OomTestTimer* subscribeTimer = CUt_OomTestTimer::NewL();
    CleanupStack::PushL(subscribeTimer);
    // Verify the object is created successfully
    STIF_ASSERT_NOT_NULL(subscribeTimer);
    STIF_LOG("CUt_OomTestTimer created successfully");
    
    // start timer and subscribe to helper
    subscribeTimer->After(TTimeIntervalMicroSeconds32(KDefaultWaitTimeInMicroSeconds));
    subscribeHelper->Subscribe();
    // Set a property, and wait for timer or callback stop active scheduler
    error = testSetProperty.Set(1);
    CActiveScheduler::Start();
    
    // Verify the values are set in subscriber correctly
    STIF_ASSERT_FALSE(subscribeTimer->iIsTimerElapsed);
    STIF_ASSERT_TRUE(isSubscribeCallbackCalled);
    STIF_LOG("Property set when subscribed has been verified");
    
    // reset isSubscribeCallbackCalled to EFalse;
    isSubscribeCallbackCalled = EFalse;
    // Stop timer
    subscribeTimer->Cancel();
    
    // start timer and unsubscribe to helper
    subscribeTimer->After(TTimeIntervalMicroSeconds32(KDefaultWaitTimeInMicroSeconds));
    subscribeHelper->StopSubscribe();
    // Set a property, and wait for timer or callback stop active scheduler
    testSetProperty.Set(2);
    CActiveScheduler::Start();
    
    // Verify the values are set in subscriber correctly
    STIF_ASSERT_TRUE(subscribeTimer->iIsTimerElapsed);
    STIF_ASSERT_FALSE(isSubscribeCallbackCalled);
    STIF_LOG("Property set when unsubscribed has been verified");

    CleanupStack::PopAndDestroy(4);
    error = RProperty::Delete(KOomSubscribeTestProperty, KOomSubscribeTestKey);
    STIF_ASSERT_EQUALS(error, KErrNone);
}
#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
