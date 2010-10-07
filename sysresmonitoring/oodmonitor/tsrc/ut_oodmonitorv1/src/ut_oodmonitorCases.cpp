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

#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh>
#include <e32debug.h>
#include <e32property.h>
#include <f32file.h>
#include <lafmemorywatcher.rsg>
#include <lafshut.h>
#include <savenotf.h>
#include <systemwarninglevels.hrh>
#include <UiklafInternalCRKeys.h>
#include <w32std.h>


#include "mockshutdowneventobserver.h"
#include "ut_oodmonitor_const.h"
#include "ut_oodmonitor_timer.h"
#include "Ood.h"

#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS

	 
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
    // property for setting AccMode 
    RProperty::Define(KUTSysApPropertyCategory, KHandleShutdownEventKey, 
        RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
    RProperty::Define(KUTSysApPropertyCategory, KGetShutdownStateKey, 
        RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
    RProperty::Delete(KUTSysApPropertyCategory, KHandleShutdownEventKey);
    RProperty::Delete(KUTSysApPropertyCategory, KGetShutdownStateKey);
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-001
 * @SYMTestCaseDesc            Create CLafShutdownManager object then delete it.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create friend class object of CLafShutdownManager.
 *                             3. Create CLafShutdownManager object.
 *                             4. Check all the proterties have been initialized.
 *                             5. Check subscribers have been initialized.
 *                             6. Check thresholds of OODWarning and OODCritical have been assigned correct value.
 *                             7. Check GlobalQueryActive has been initialized.
 *                             8. Check OODWarningWatcher and iOODCriticalWatcher has been initialized.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. Friend class object of CLafShutdownManager is created.
 *                             3. CLafShutdownManager object is created.
 *                             4. All the proterties can be accessed.
 *                             5. All the pointer of subscribers are not null.
 *                             6. The thresholds of OODWarning and OODCritical have been assigned correct value.
 *                             7. The pointer of GlobalQueryActive is not null.
 *                             8. Pointers to OODWarningWatcher and iOODCriticalWatcher are not null.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */
STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-001)
{
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-001 start"));
    // Create active scheduler (to run active objects)  
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
        
    Tut_LafShutdownManager friendClass;
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver);
    
    // Check properties
    TInt value;
    TInt err;
    RProperty diskStatusProperty = friendClass.GetDiskStatusProperty(shutdownManager);
    err = diskStatusProperty.Get(value);
    STIF_ASSERT_EQUALS(KErrNone, err);
    
    RProperty mmcStatusProperty = friendClass.GetMMCStatusProperty(shutdownManager);
    err = mmcStatusProperty.Get(value);
    STIF_ASSERT_EQUALS(KErrNone, err);

    RProperty canDisplayNotesProperty = friendClass.GetCanDisplayNotesProperty(shutdownManager);
    err = canDisplayNotesProperty.Get(value);
    iLog->Log(_L("CLafShutdownManager:: iDisplayNotesProperty=%d"), value);
    STIF_ASSERT_EQUALS(KErrNone, err);
    
    RProperty autolockStatusProperty = friendClass.GetAutolockStatusProperty(shutdownManager);
    err = autolockStatusProperty.Get(value);
    iLog->Log(_L("CLafShutdownManager:: iAutolockStatusProperty=%d"), value);
    STIF_ASSERT_EQUALS(KErrNone, err);
    
    RProperty enableMMCWatchProperty = friendClass.GetEnableMMCWatchProperty(shutdownManager);
    err = enableMMCWatchProperty.Get(value);
    STIF_ASSERT_EQUALS(KErrNone, err);
    
    // Check subscriber
    CBase* pointer = friendClass.GetDisplayNotesSubscriber(shutdownManager);
    STIF_ASSERT_NOT_NULL(pointer);
    pointer = friendClass.GetAutolockStatusSubscriber(shutdownManager);
    STIF_ASSERT_NOT_NULL(pointer);
    pointer = friendClass.GetMMCWatchSubscriber(shutdownManager);
    STIF_ASSERT_NOT_NULL(pointer);
    
    TInt oodWarningThreshold = friendClass.GetOODWarningThreshold(shutdownManager);
    STIF_ASSERT_EQUALS(KDRIVECWARNINGTHRESHOLD, oodWarningThreshold);
    TInt oodCriticalThreshold = friendClass.GetOODCriticalThreshold(shutdownManager);
    STIF_ASSERT_EQUALS(KDRIVECCRITICALTHRESHOLD, oodCriticalThreshold);

    CLafShutdownManager::CGlobalQueryActive* globalQuaryActive = friendClass.GetGlobalQueryActive(shutdownManager);
    iLog->Log(_L("CLafShutdownManager::iGlobalQueryActive=%d"), globalQuaryActive);
    STIF_ASSERT_NOT_NULL(globalQuaryActive);
         
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);    
    
    RResourceFile resFile;
    TFileName filename(KDriveZ);
    filename.Append(KDC_RESOURCE_FILES_DIR);
    filename.Append(KOOMWatcherResourceFileName);
    BaflUtils::NearestLanguageFile(fs, filename);
    resFile.OpenL(fs, filename);
    CleanupClosePushL(resFile);
    
    TResourceReader theReader;
    
    CLafShutdownManager::CGlobalQueryActive::CMessageInfo* messageInfoPointer = 
            friendClass.GetMessageInfoList(globalQuaryActive, CLafShutdownManager::CGlobalQueryActive::ECritical);
   
    HBufC8* message = resFile.AllocReadLC(R_QTN_MEMLO_MEMORY_FULL);
    theReader.SetBuffer(message);
    HBufC* criticalMessage = theReader.ReadHBufCL();
    
    iLog->Log(_L("CLafShutdownManager::criticalMessage=%S"), criticalMessage);
    iLog->Log(_L("CLafShutdownManager::criticalMessage1=%S"), messageInfoPointer->iMessage);
    
    TInt result = criticalMessage->Compare(*(messageInfoPointer->iMessage));
    iLog->Log(_L("CLafShutdownManager::result=%d"), result);
    
    STIF_ASSERT_TRUE(0 == result);
    delete criticalMessage;   
    CleanupStack::PopAndDestroy(message);
    
    messageInfoPointer = 
            friendClass.GetMessageInfoList(globalQuaryActive, CLafShutdownManager::CGlobalQueryActive::EWarning);
    message = resFile.AllocReadLC(R_QTN_MEMLO_MEMORY_RUNNING_OUT);
    theReader.SetBuffer(message);
    HBufC* warningMessage = theReader.ReadHBufCL();
    
    iLog->Log(_L("CLafShutdownManager::warningMessage=%S"), warningMessage);
    iLog->Log(_L("CLafShutdownManager::warningMessage1=%S"), messageInfoPointer->iMessage);
    
    result = warningMessage->Compare(*(messageInfoPointer->iMessage));
    iLog->Log(_L("CLafShutdownManager::result=%d"), result);
    
    STIF_ASSERT_TRUE(0 == result);
    delete warningMessage;   
    CleanupStack::PopAndDestroy(message);
    
    messageInfoPointer =
            friendClass.GetMessageInfoList(globalQuaryActive, CLafShutdownManager::CGlobalQueryActive::EWarningMMC);
    message = resFile.AllocReadLC(R_QTN_MEMLO_MMC_MEMORY_RUNNING_OUT);
    theReader.SetBuffer(message);
    HBufC* warningMMCMessage = theReader.ReadHBufCL();
    
    iLog->Log(_L("CLafShutdownManager::warningMMCMessage=%S"), warningMMCMessage);
    iLog->Log(_L("CLafShutdownManager::warningMMCMessage1=%S"), messageInfoPointer->iMessage);
    
    result = warningMMCMessage->Compare(*(messageInfoPointer->iMessage));
    iLog->Log(_L("CLafShutdownManager::result=%d"), result);
    
    STIF_ASSERT_TRUE(0 == result);
    delete warningMMCMessage;   
    CleanupStack::PopAndDestroy(message);
    
    messageInfoPointer = 
                friendClass.GetMessageInfoList(globalQuaryActive, CLafShutdownManager::CGlobalQueryActive::ECriticalMMC);
    message = resFile.AllocReadLC(R_QTN_MEMLO_MMC_MEMORY_FULL);
    theReader.SetBuffer(message);
    HBufC* criticalMMCMessage = theReader.ReadHBufCL();
    
    iLog->Log(_L("CLafShutdownManager::criticalMMCMessage=%S"), criticalMMCMessage);
    iLog->Log(_L("CLafShutdownManager::criticalMMCMessage1=%S"), messageInfoPointer->iMessage);
    
    result = criticalMMCMessage->Compare(*(messageInfoPointer->iMessage));
    iLog->Log(_L("CLafShutdownManager::result=%d"), result);
    
    STIF_ASSERT_TRUE(0 == result);
    delete criticalMMCMessage;   
    CleanupStack::PopAndDestroy(message);
        
    CleanupStack::PopAndDestroy(&resFile);
    CleanupStack::PopAndDestroy(&fs);
    
    CActive* pointer1 = friendClass.GetOODWarningWatcher(shutdownManager);
    iLog->Log(_L("CLafShutdownManager::iOODWarningWatcher=%d"), pointer1);
    STIF_ASSERT_NOT_NULL(pointer1);
    pointer1 = friendClass.GetOODCriticalWatcher(shutdownManager);
    iLog->Log(_L("CLafShutdownManager::iOODCriticalWatcher=%d"), pointer1);
    STIF_ASSERT_NOT_NULL(pointer1);
    
    delete shutdownManager;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-001 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-002
 * @SYMTestCaseDesc            Test LafShutdown class.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Test LafShutdown::CreateShutdownManager();
 *                             3. Test LafShutdown::SaveFromViewSwitch();
 *                             4. Test LafShutdown::SaveFromAppSwitch();
 *                             5. Delete CLafShutdownManagerBase object.
 *                             6. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManagerBase object is created.
 *                             3. LafShutdown::SaveFromViewSwitch(); return expected value.
 *                             4. LafShutdown::SaveFromAppSwitch(); return expected value.
 *                             5. CLafShutdownManagerBase object is deleted.
 *                             6. Scheduler is deleted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */
STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-002)
{
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-002 start"));
    
    // Create active scheduler (to run active objects)    
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
        
    ShutdownEventObserver shutdownEventObserver;   
    
    CLafShutdownManagerBase* shutdownManager = LafShutdown::CreateShutdownManager(shutdownEventObserver);
    STIF_ASSERT_NOT_NULL(shutdownManager);
    
    MSaveObserver::TSaveType type = LafShutdown::SaveFromViewSwitch();
    STIF_ASSERT_EQUALS(MSaveObserver::ESaveNone, type);
 
    type = LafShutdown::SaveFromAppSwitch();
    STIF_ASSERT_EQUALS(MSaveObserver::ESaveNone, type);
    
    delete shutdownManager;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-002 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-003
 * @SYMTestCaseDesc            Test MMCWatchSubscriber
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create CLafShutdownManager object.
 *                             3. Create Cut_OODMonitor_Timer object.
 *                             4. Start scheduler.
 *                             5. Change enableMMCWatchProperty.
 *                             6. Change enableMMCWatchProperty after two seconds.
 *                             7. Stop shceduler.
 *                             8. Check mmcStatusProperty property.
 *                             9. Delete Cut_OODMonitor_Timer object and CLafShutdownManager object.
 *                             10. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManager object is created.
 *                             3. Cut_OODMonitor_Timer object is created.
 *                             4. Scheduler starts.
 *                             5. enableMMCWatchProperty changed;
 *                             6. enableMMCWatchProperty changed after two seconds.
 *                             7. Shceduler stoped.
 *                             8. mmcStatusProperty property is assign value as expected.
 *                             9. Cut_OODMonitor_Timer object and CLafShutdownManager object are deleted.
 *                             10. Scheduler is deleted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */
STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-003)
{
    // Create active scheduler (to run active objects)
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-003 start"));
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
        
    Tut_LafShutdownManager friendClass;
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver);
    
    Cut_OODMonitor_Timer* oodMonitorTimer = Cut_OODMonitor_Timer::NewLC(shutdownManager, friendClass, EMMCTest);
    CActiveScheduler::Start();
    
    TInt value;
    TInt err;
    RProperty mmcStatusProperty = friendClass.GetMMCStatusProperty(shutdownManager);
    err = mmcStatusProperty.Get(value);
    iLog->Log(_L("CLafShutdownManager:: value=%d"),value);
    STIF_ASSERT_EQUALS(KErrNone, err);
    STIF_ASSERT_EQUALS(value, DISK_SPACE_OK);
        
    CleanupStack::PopAndDestroy(oodMonitorTimer);
    delete shutdownManager;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-003 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-004
 * @SYMTestCaseDesc            Test DisplayNotesSubscriber.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create CLafShutdownManager object.
 *                             3. Create Cut_OODMonitor_Timer object.
 *                             4. Start scheduler.
 *                             5. Change canDisplayNotesProperty and fill the disk space to 
 *                                critical level.
 *                             6. Change canDisplayNotesProperty after two seconds.
 *                             7. Free disk space after two seconds.
 *                             8. Stop shceduler.
 *                             9. Check FreeDiskThresholdCrossedL() is called.
 *                             10. Delete Cut_OODMonitor_Timer object and CLafShutdownManager object.
 *                             11. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManager object is created.
 *                             3. Cut_OODMonitor_Timer object is created.
 *                             4. Scheduler start.
 *                             5. canDisplayNotesProperty changed and disk space is filled to 
 *                                critical level.
 *                             6. canDisplayNotesProperty changed after two seconds.
 *                             7. Disk space is freed after two seconds.
 *                             8. Stop shceduler.
 *                             9. FreeDiskThresholdCrossedL() is called.
 *                             10. Cut_OODMonitor_Timer object and CLafShutdownManager object are deleted.
 *                             11. Scheduler is deleted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */
STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-004)
{
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-004 start"));
    
    // Create active scheduler (to run active objects)    
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
        
    Tut_LafShutdownManager friendClass;
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver);
    
    Cut_OODMonitor_Timer* oodMonitorTimer = Cut_OODMonitor_Timer::NewLC(shutdownManager, friendClass, EDisplayNotes);
    CActiveScheduler::Start();
    
    CLafShutdownManager::CGlobalQueryActive* globalQuaryActive = friendClass.GetGlobalQueryActive(shutdownManager);
    iLog->Log(_L("CLafShutdownManager::iGlobalQueryActive=%d"), globalQuaryActive);
    STIF_ASSERT_NOT_NULL(globalQuaryActive);
    
    CLafShutdownManager::CGlobalQueryActive::CMessageInfo* messageInfoPointer = 
            friendClass.GetMessageInfoList(globalQuaryActive, CLafShutdownManager::CGlobalQueryActive::ECritical);
    
    TInt locate = messageInfoPointer->iMessage->Des().Locate('s');
    iLog->Log(_L("CLafShutdownManager::locate=%d"), locate);
    STIF_ASSERT_EQUALS(0, locate);
    
    
    
    CleanupStack::PopAndDestroy(oodMonitorTimer);
    delete shutdownManager;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-004 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-005
 * @SYMTestCaseDesc            Test AutolockStatusSubscriber.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create CLafShutdownManager object.
 *                             3. Create Cut_OODMonitor_Timer object.
 *                             4. Start scheduler.
 *                             5. Change autolockStatusProperty and fill the disk space to 
 *                                warning level.
 *                             6. Change autolockStatusProperty after two seconds.
 *                             7. Free disk space after two seconds.
 *                             8. Stop shceduler.
 *                             9. Check FreeDiskThresholdCrossedL() is called.
 *                             10. Delete Cut_OODMonitor_Timer object.
 *                             11. Delete CLafShutdownManager object.
 *                             12. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManager object is created.
 *                             3. Cut_OODMonitor_Timer object is created.
 *                             4. Scheduler start.
 *                             5. autolockStatusProperty changed and disk space is filled to 
 *                                warning level.
 *                             6. autolockStatusProperty changed after two seconds.
 *                             7. Disk space is freed after two seconds.
 *                             8. Stop shceduler.
 *                             9. FreeDiskThresholdCrossedL() is called.
 *                             10. Cut_OODMonitor_Timer object is deleted.
 *                             11. CLafShutdownManager object is deleted.
 *                             12. Scheduler is deleted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */

STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-005)
{
    // Create active scheduler (to run active objects)
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-005 start"));
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
        
    Tut_LafShutdownManager friendClass;
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver);
    
    Cut_OODMonitor_Timer* oodMonitorTimer = Cut_OODMonitor_Timer::NewLC(shutdownManager, friendClass, EAutolock);
    CActiveScheduler::Start();
    
    CLafShutdownManager::CGlobalQueryActive* globalQuaryActive = friendClass.GetGlobalQueryActive(shutdownManager);
    iLog->Log(_L("CLafShutdownManager::iGlobalQueryActive=%d"), globalQuaryActive);
    STIF_ASSERT_NOT_NULL(globalQuaryActive);
    
    CLafShutdownManager::CGlobalQueryActive::CMessageInfo* messageInfoPointer = 
            friendClass.GetMessageInfoList(globalQuaryActive, CLafShutdownManager::CGlobalQueryActive::EWarning);
    
    TInt locate = messageInfoPointer->iMessage->Des().Locate('s');
    iLog->Log(_L("CLafShutdownManager::locate=%d"), locate);
    STIF_ASSERT_EQUALS(0, locate);
    
    CleanupStack::PopAndDestroy(oodMonitorTimer);
    delete shutdownManager;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-005 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-006
 * @SYMTestCaseDesc            Test ReportEventL() and GetShutdownState().
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create CLafShutdownManager object.
 *                             3. Call ReportEventL().
 *                             4. Call GetShutdownState().
 *                             5. Delete CLafShutdownManager object.
 *                             6. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManager object is created.
 *                             3. ReportEventL() is called correctly.
 *                             4. GetShutdownState is called correctly.
 *                             5. CLafShutdownManager object is deleted.
 *                             6. Scheduler is delted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */

STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-006)
{   
    // Create active scheduler (to run active objects)
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-006 start"));
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    
    TInt err;
    err=RProperty::Set(KUTSysApPropertyCategory, KHandleShutdownEventKey, KHandleShutdownEventValue0);
    iLog->Log(_L("CLafShutdownManager:: err=%d"),err);
    err=RProperty::Set(KUTSysApPropertyCategory, KGetShutdownStateKey, KGetShutdownStateValue0);
    iLog->Log(_L("CLafShutdownManager:: err=%d"),err);
        
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver);
    Tut_LafShutdownManager friendClass;
    
    // Check ReportEventL()
    TBool boolValue = ETrue;
    friendClass.CallReportEventL(shutdownManager, MSaveObserver::ESaveData, boolValue);
    
    TInt propertyValue;
    RProperty::Get(KUTSysApPropertyCategory, KHandleShutdownEventKey, propertyValue);
    iLog->Log(_L("CLafShutdownManager:: KHandleShutdownEventKey = %d"), propertyValue);
    STIF_ASSERT_EQUALS(KHandleShutdownEventValue1, propertyValue);
    
    // Check GetShutdownState()
    friendClass.CallGetShutdownState(shutdownManager, boolValue, boolValue);

    RProperty::Get(KUTSysApPropertyCategory, KGetShutdownStateKey, propertyValue);
    iLog->Log(_L("CLafShutdownManager:: KGetShutdownStateKey = %d"), propertyValue);
    STIF_ASSERT_EQUALS(KGetShutdownStateValue1, propertyValue);
       
    delete shutdownManager;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-006 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-007
 * @SYMTestCaseDesc            Test CallGetDiskStatusL().
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create CLafShutdownManager object.
 *                             3. Fill the disk space to critical level.
 *                             4. Call CallGetDiskStatusL().
 *                             5. Free disk space.
 *                             6. Fill the disk space to warning level.
 *                             7. Call CallGetDiskStatusL().
 *                             8. Free disk space.
 *                             9. Call CallGetDiskStatusL().
 *                             10. Delete CLafShutdownManager object.
 *                             11. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManager object is created.
 *                             3. Disk space is filled to critical level.
 *                             4. CallGetDiskStatusL() return correct value.
 *                             5. Disk space is freed.
 *                             6. Disk space is filled to warning level.
 *                             7. CallGetDiskStatusL() return correct value.
 *                             8. Disk space is freed.
 *                             9. CallGetDiskStatusL() return correct value.
 *                             10. CLafShutdownManager object is deleted.
 *                             11. Scheduler is deleted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */

STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-007)
{
    // Create active scheduler (to run active objects)
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-007 start"));
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver);
    
    CDesCArrayFlat* fileNameArray = new (ELeave) CDesCArrayFlat(KArraySize);
    CleanupStack::PushL(fileNameArray);   
    
    Tut_LafShutdownManager friendClass;
    
    // Critical
    iLog->Log(_L("CLafShutdownManager:: Critical"));
    friendClass.EatDiskL(fileNameArray, EDriveC, ECritical);
    
    TVolumeInfo volInfo;

    TInt value = friendClass.CallGetDiskStatusL(shutdownManager, volInfo, EDriveC);
    iLog->Log(_L("CLafShutdownManager:: value=%d"),value);
    STIF_ASSERT_TRUE(value == DISK_SPACE_CRITICAL);
    
    friendClass.FreeDiskL(fileNameArray);
    
    //Warning
    iLog->Log(_L("CLafShutdownManager:: Warning"));
    fileNameArray->Reset();
    friendClass.EatDiskL(fileNameArray, EDriveC, EWarning);

    value = friendClass.CallGetDiskStatusL(shutdownManager, volInfo, EDriveC);
    iLog->Log(_L("CLafShutdownManager:: value=%d"),value);
    STIF_ASSERT_TRUE(value == DISK_SPACE_WARNING)
    
    User::After(KTwoSecondInMicro);
    friendClass.FreeDiskL(fileNameArray);
    
    // Normal
    iLog->Log(_L("CLafShutdownManager:: Normal"));
    value = friendClass.CallGetDiskStatusL(shutdownManager, volInfo, EDriveC);
    iLog->Log(_L("CLafShutdownManager:: value=%d"),value);
    STIF_ASSERT_TRUE(value == DISK_SPACE_OK)
    
    CleanupStack::PopAndDestroy(fileNameArray);
    delete shutdownManager;   
  
    //Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-007 end"));
}

/**
 * @SYMTestCaseID              GAPS-OOD-SHUTDOWNMANAGER-008
 * @SYMTestCaseDesc            Test OOD disk watcher.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create scheduler.
 *                             2. Create CLafShutdownManager object.
 *                             3. Create Cut_OODMonitor_Timer object.
 *                             4. Start scheduler.
 *                             5. Fill the disk space to critical level.
 *                             6. Free disk space after two seconds.
 *                             7. Stop shceduler.
 *                             8. Check DiskStatusProperty.
 *                             9. Delete Cut_OODMonitor_Timer object and CLafShutdownManager object.
 *                             10. Delete scheduler.
 * @SYMTestExpectedResults     1. Sechduler is created.
 *                             2. CLafShutdownManager object is created.
 *                             3. Cut_OODMonitor_Timer object is created.
 *                             4. Scheduler started.
 *                             5. Disk space is filled to critical level.
 *                             6. Disk space is freed after two seconds.
 *                             7. Shceduler stopped.
 *                             8. DiskStatusProperty is assigned value as expected.
 *                             9. Delete Cut_OODMonitor_Timer object and CLafShutdownManager object.
 *                             10. Delete scheduler.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */

STIF_TESTDEFINE(GAPS-OOD-SHUTDOWNMANAGER-008)
{
    // Create active scheduler (to run active objects)
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-008 start"));
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    
    ShutdownEventObserver shutdownEventObserver;   
    CLafShutdownManager* shutdownManager = CLafShutdownManager::NewL(shutdownEventObserver); 
    
    Tut_LafShutdownManager friendClass;
    
    Cut_OODMonitor_Timer* oodMonitorTimer = Cut_OODMonitor_Timer::NewLC(shutdownManager, friendClass, EOODWatcher);
    CActiveScheduler::Start();
    
    TInt value;
    TInt err;
    RProperty diskStatusProperty = friendClass.GetDiskStatusProperty(shutdownManager);
    err = diskStatusProperty.Get(value);
    iLog->Log(_L("CLafShutdownManager:: value=%d"),value);
    STIF_ASSERT_EQUALS(KErrNone, err);
    STIF_ASSERT_EQUALS(value, DISK_SPACE_OK);

    CleanupStack::PopAndDestroy(oodMonitorTimer);
    delete shutdownManager;   
  
    //Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    iLog->Log(_L("CLafShutdownManager:: GAPS-OOD-SHUTDOWNMANAGER-008 end"));
}
#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
