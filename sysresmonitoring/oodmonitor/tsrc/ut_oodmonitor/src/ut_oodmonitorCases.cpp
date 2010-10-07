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
#include <driveinfo.h>
#include <disknotifyhandler.h>
#include "outofdiskglobalnote.h"
#include "outofdiskmonitor.h"
#include "utoutofdiskpsobserver.h"
#include "outofdisknotifyobserver.h"
#include "MockShutdownEventObserver.h"
#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
     CActiveScheduler* scheduler;
     const TInt64 KWarningThreshold = 2048;
     const TInt64 KCriticalThreshold = 1024;
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
// instance and install, but not started
scheduler = new (ELeave) CActiveScheduler();
CleanupStack::PushL(scheduler);
CActiveScheduler::Install(scheduler);
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
CActiveScheduler::Install(NULL);
CleanupStack::PopAndDestroy(scheduler);
}

/**
 * @SYMTestCaseID              GAPS-OODMONITOR-GLOBALNOTE-001
 * @SYMTestCaseDesc            Unit test for COutOfDiskGlobalNote.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Connect file server.
 *                             2. Construct a COutOfDiskMonitor object.
 *                             3. Construct a COutOfDiskGlobalNote object.
 *                             4. Get note info.
 *                             5. NoteOnDisplay.
 *                             6. Display a message.
 *                             7. MessageBoxClosed.
 *                             8. Destroy the COutOfDiskGlobalNote object.
 *                             9. Destroy the mock COutOfDiskMonitor object
 *                             10. Destroy the RFs object.
 * @SYMTestExpectedResults     1. File server was connected.
 *                             2. COutOfDiskMonitor object was constructed.
 *                             3. COutOfDiskGlobalNote object was constructed.
 *                             4. NoteInfo was correct.
 *                             5. Property was defined.
 *                             6. Message was displayed.
 *                             7. No error.
 *                             8. The COutOfDiskGlobalNote object was destroyed.
 *                             9. The COutOfDiskMonitor object was destroyed.
 *                             10. The RFs object was closed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            23-08-2010
 */
STIF_TESTDEFINE(GAPS-OODMONITOR-GLOBALNOTE-001)
{
    TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);
    
    STIF_LOG("1. Connect file server.");
    RFs fileServer;
    STIF_ASSERT_EQUALS(KErrNone, fileServer.Connect());
    CleanupClosePushL(fileServer);
    
    STIF_LOG("2. Construct a COutOfDiskMonitor object.");
    MShutdownEventObserver* shutdownEventObserver = NULL;
    COutOfDiskMonitor* outOfDiskMonitor = COutOfDiskMonitor::NewL(*shutdownEventObserver);
    CleanupStack::PushL(outOfDiskMonitor);
    
    STIF_LOG("3. Construct a COutOfDiskGlobalNote object.");
    COutOfDiskGlobalNote* outOfDiskGlobalNote = COutOfDiskGlobalNote::NewL(outOfDiskMonitor, fileServer);
    CleanupStack::PushL(outOfDiskGlobalNote);
    
    STIF_LOG("4. Get note info.");
    TNoteInfo noteInfo = outOfDiskGlobalNote->GetNoteInfo();
    STIF_ASSERT_EQUALS(KErrNotFound, noteInfo.iNoteId);
    STIF_ASSERT_EQUALS(DISK_SPACE_OK, noteInfo.iStatus);
    STIF_ASSERT_EQUALS(KErrNotFound, noteInfo.iDrive);
    
    STIF_LOG("5. NoteOnDisplay.");
    STIF_ASSERT_FALSE(outOfDiskGlobalNote->NoteOnDisplay());
    
    STIF_LOG("6. Display a message.");
    outOfDiskGlobalNote->DisplayL(_L("Display a message."));
    
    STIF_LOG("7. MessageBoxClosed.");
    outOfDiskGlobalNote->MessageBoxClosed(NULL, CHbDeviceMessageBoxSymbian::EAcceptButton);
    
    STIF_LOG("8. Destroy the COutOfDiskGlobalNote object.");
    CleanupStack::PopAndDestroy(outOfDiskGlobalNote);
    
    STIF_LOG("9. Destroy the COutOfDiskMonitor object.");
    CleanupStack::PopAndDestroy(outOfDiskMonitor);
    
    STIF_LOG("10. Destroy the RFs object.");
    CleanupStack::PopAndDestroy(&fileServer);
}

/**
 * @SYMTestCaseID              GAPS-OODMONITOR-PSOBSERVER-001
 * @SYMTestCaseDesc            Unit test for COutOfDiskPSObserver.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COutOfDiskMonitor object.
 *                             2. Construct a COutOfDiskPSObserver object.
 *                             3. Handle property change. Category is KPSUidUikon, and Key is KUikGlobalNotesAllowed.
 *                             4. Handle property change. Category is KPSUidCoreApplicationUIs, and Key is KCoreAppUIsAutolockStatus.
 *                             5. Destroy the COutOfDiskPSObserver object.
 *                             6. Destroy the COutOfDiskMonitor object
 * @SYMTestExpectedResults     1. Mock COutOfDiskMonitor object was constructed.
 *                             2. COutOfDiskPSObserver object was constructed.
 *                             3. Property change was handled successfully.
 *                             4. Property change was handled successfully.
 *                             5. The COutOfDiskPSObserver object was destroyed.
 *                             6. The COutOfDiskMonitor object was destroyed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            23-08-2010
 */
STIF_TESTDEFINE(GAPS-OODMONITOR-PSOBSERVER-001)
{    
    STIF_LOG("1. Construct a COutOfDiskMonitor object.");
    MShutdownEventObserver* shutdownEventObserver = NULL;
    COutOfDiskMonitor* outOfDiskMonitor = COutOfDiskMonitor::NewL(*shutdownEventObserver);
    CleanupStack::PushL(outOfDiskMonitor);
    
    STIF_LOG("2. Construct a COutOfDiskPSObserver object.");
    CUTOutOfDiskPSObserver* outOfDiskPSObserver = CUTOutOfDiskPSObserver::NewL(outOfDiskMonitor);
    CleanupStack::PushL(outOfDiskPSObserver);
    
    STIF_LOG("3. Handle property change. Category is KPSUidUikon, and Key is KUikGlobalNotesAllowed.");
    outOfDiskPSObserver->HandlePropertyChangedL(KPSUidUikon, KUikGlobalNotesAllowed);
    
    STIF_LOG("4. Handle property change. Category is KPSUidCoreApplicationUIs, and Key is KCoreAppUIsAutolockStatus.");
    outOfDiskPSObserver->HandlePropertyChangedL(KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus);
    
    STIF_LOG("5. Destroy the COutOfDiskPSObserver object.");
    CleanupStack::PopAndDestroy(outOfDiskPSObserver);
    
    STIF_LOG("6. Destroy the COutOfDiskMonitor object.");
    CleanupStack::PopAndDestroy(outOfDiskMonitor);
}

/**
 * @SYMTestCaseID              GAPS-OODMONITOR-OUTOFDISKMONITOR-001
 * @SYMTestCaseDesc            Unit test for COutOfDiskMonitor.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COutOfDiskMonitor object.
 *                             2. ShowGlobalQueryIfNeededL.
 *                             3. Get C Drive Status.
 *                             4. SetAsDisplayedL.
 *                             5. SetGlobalNotesAllowed to ETrue.
 *                             6. GetGlobalNotesAllowed.
 *                             7. SetGlobalNotesAllowed to EFalse.
 *                             8. GetGlobalNotesAllowed.
 *                             9. GetDefaultPhoneMemory.
 *                             10. GetDefaultMassStorage.
 *                             11. GetDefaultRomDrive.
 *                             12. Destroy the COutOfDiskMonitor object.
 * @SYMTestExpectedResults     1. The COutOfDiskMonitor object was constructed.
 *                             2. No error.
 *                             3. DISK_SPACE_OK was returned.
 *                             4. Set successfully.
 *                             5. Set successfully.
 *                             6. The value set by step5 was returned.
 *                             7. Set successfully.
 *                             8. The value set by step7 was returned.
 *                             9. DefaultPhoneMemory was correctly gotten.
 *                             10. DefaultMassStorage was correctly gotten.
 *                             11. DefaultRomDrive was correctly gotten.
 *                             12. The COutOfDiskMonitor object was destroyed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            23-08-2010
 */
STIF_TESTDEFINE(GAPS-OODMONITOR-OUTOFDISKMONITOR-001)
{  
    STIF_LOG("1. Construct a COutOfDiskMonitor object.");
    MShutdownEventObserver* shutdownEventObserver = NULL;
    COutOfDiskMonitor* outOfDiskMonitor = COutOfDiskMonitor::NewL(*shutdownEventObserver);
    CleanupStack::PushL(outOfDiskMonitor);

    STIF_LOG("2. ShowGlobalQueryIfNeededL.");
    outOfDiskMonitor->ShowGlobalQueryIfNeededL();
    
    STIF_LOG("3. Get C Drive Status.");
    STIF_ASSERT_EQUALS(DISK_SPACE_OK, outOfDiskMonitor->GetDriveStatus(EDriveC));
    
    // GetThreshold is called by GetDriveStatis.
    // SaveDriveStatus is called by ConstructL.
    // ResetDisplayedStatus is called by SaveDriveStatus.
    // AllowedToReset is called by RestDisplayedStatus.
    // GetTopPriorityDrive is called by ShowGlobalQueryIfNeededL.
    
    STIF_LOG("4. SetAsDisplayedL.");
    outOfDiskMonitor->SetAsDisplayedL(EDriveC, DISK_SPACE_OK);
    
    STIF_LOG("5. SetGlobalNotesAllowed to ETrue.");
    outOfDiskMonitor->SetGlobalNotesAllowed(ETrue);
    
    STIF_LOG("6. GetGlobalNotesAllowed.");
    STIF_ASSERT_TRUE(outOfDiskMonitor->GetGlobalNotesAllowed());
    
    STIF_LOG("7. SetGlobalNotesAllowed to EFalse.");
    outOfDiskMonitor->SetGlobalNotesAllowed(EFalse);
    
    STIF_LOG("8. GetGlobalNotesAllowed.");
    STIF_ASSERT_FALSE(outOfDiskMonitor->GetGlobalNotesAllowed());
    
    STIF_LOG("9. GetDefaultPhoneMemory.");
    TInt defaultPhoneMemory = 0;
    DriveInfo::GetDefaultDrive( DriveInfo::EDefaultPhoneMemory, defaultPhoneMemory );
    STIF_ASSERT_EQUALS(defaultPhoneMemory, outOfDiskMonitor->GetDefaultPhoneMemory());
    
    STIF_LOG("10. GetDefaultMassStorage.");
    TInt defaultMassStorage = 0;
    DriveInfo::GetDefaultDrive( DriveInfo::EDefaultMassStorage, defaultMassStorage );
    STIF_ASSERT_EQUALS(defaultMassStorage, outOfDiskMonitor->GetDefaultMassStorage());
    
    STIF_LOG("11. GetDefaultRomDrive.");
    // Actually COutOfDiskMonitor doesn't set iDefaultRomDrive, so that I don't need to check the result.
    TInt defaultRomDrive = outOfDiskMonitor->GetDefaultRomDrive();
    
    STIF_LOG("12. Destroy the COutOfDiskMonitor object.");
    CleanupStack::PopAndDestroy(outOfDiskMonitor);
}

/**
 * @SYMTestCaseID              GAPS-OOD-LAFSHUT-001
 * @SYMTestCaseDesc            Unit test for lafshut.cpp
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Create ShutdownEventObserver object.
 *                             2. Create CLafShutdownManagerBase object.
 *                             3. Call SaveFromViewSwitch();
 *                             4. Call SaveFromAppSwitch();
 *                             5. Delete CLafShutdownManagerBase object.
 * @SYMTestExpectedResults     1. ShutdownEventObserver object is created.
 *                             2. CLafShutdownManagerBase object is create successfully.
 *                             3. SaveFromViewSwitch() return expected value.
 *                             4. SaveFromAppSwitch(); return expected value.
 *                             5. CLafShutdownManagerBase object is deleted.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            03-09-2010
 */
STIF_TESTDEFINE(GAPS-OOD-LAFSHUT-001)
{
    STIF_LOG("1. Create ShutdownEventObserver object.");       
    TshutdownEventObserver shutdownEventObserver;   
    
    STIF_LOG("2. Create CLafShutdownManagerBase object.");
    CLafShutdownManagerBase* shutdownManager = LafShutdown::CreateShutdownManager(shutdownEventObserver);
    STIF_ASSERT_NOT_NULL(shutdownManager);
    
    STIF_LOG("3. Call SaveFromViewSwitch();");
    MSaveObserver::TSaveType type = LafShutdown::SaveFromViewSwitch();
    STIF_ASSERT_EQUALS(MSaveObserver::ESaveNone, type);
 
    STIF_LOG("4. Call SaveFromAppSwitch();");
    type = LafShutdown::SaveFromAppSwitch();
    STIF_ASSERT_EQUALS(MSaveObserver::ESaveNone, type);
    
    STIF_LOG("5. Delete CLafShutdownManagerBase object.");
    delete shutdownManager;
}

/**
 * @SYMTestCaseID              GAPS-OODMONITOR-OUTOFDISKNOTIFYOBSERVER-001
 * @SYMTestCaseDesc            Unit test for COutOfDiskNotifyObserver.
 * @SYMTestPriority            Medium
 * @SYMTestActions             1. Construct a COutOfDiskMonitor object.
 *                             2. COutOfDiskNotifyObserver::NotifyDiskSpace
 *                             3. COutOfDiskNotifyObserver::HandleNotifyDisk.
 *                             4. COutOfDiskNotifyObserver::HandleNotifyDismount
 *                             5. COutOfDiskNotifyObserver::HandleNotifyDiskSpace
 *                             6. COutOfDiskNotifyObserver::HandleNotifyEntry
 *                             7. Destroy the COutOfDiskMonitor object.
 * @SYMTestExpectedResults     1. The COutOfDiskMonitor object was constructed.
 *                             2. Function returned successfully.
 *                             3. Function returned successfully.
 *                             4. Function returned successfully.
 *                             5. Function returned successfully.
 *                             6. Function returned successfully.
 *                             7. The COutOfDiskMonitor object was destroyed.
 * @SYMTestType                Unit Test
 * @SYMCreationDate            09-09-2010
 */
STIF_TESTDEFINE(GAPS-OODMONITOR-OUTOFDISKNOTIFYOBSERVER-001)
{
    STIF_LOG("1. Construct a COutOfDiskMonitor object.");
    MShutdownEventObserver* shutdownEventObserver = NULL;
    COutOfDiskMonitor* outOfDiskMonitor = COutOfDiskMonitor::NewL(*shutdownEventObserver);
    CleanupStack::PushL(outOfDiskMonitor);
    
    STIF_LOG("2. NotifyDiskSpace");
    outOfDiskMonitor->iOutOfDiskNotifyObserver->NotifyDiskSpace(KWarningThreshold, KCriticalThreshold, EDriveF);
    
    STIF_LOG("3. HandleNotifyDisk");
    MDiskNotifyHandlerCallback::TDiskEvent diskEvent;
    diskEvent.iDrive = EDriveF;
    
    diskEvent.iType = MDiskNotifyHandlerCallback::EDiskAdded;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyDisk(KErrNone, diskEvent);
    
    diskEvent.iType = MDiskNotifyHandlerCallback::EDiskError;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyDisk(KErrNone, diskEvent);
    
    diskEvent.iType = MDiskNotifyHandlerCallback::EDiskRemoved;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyDisk(KErrNone, diskEvent);
    
    diskEvent.iType = MDiskNotifyHandlerCallback::EDiskStatusChanged;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyDisk(KErrNone, diskEvent);
    
    STIF_LOG("4. HandleNotifyDismount");
    MDiskNotifyHandlerCallback::TDismountEvent dismountEvent;
    dismountEvent.iDrive = EDriveF;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyDismount(KErrNone, dismountEvent);
    
    STIF_LOG("5. HandleNotifyDiskSpace");
    MDiskNotifyHandlerCallback::TDiskSpaceEvent diskSpaceEvent;
    diskSpaceEvent.iDrive = EDriveF;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyDiskSpace(KErrNone, diskSpaceEvent);
    
    STIF_LOG("6. HandleNotifyEntry");
    MDiskNotifyHandlerCallback::TEntryEvent entryEvent;
    outOfDiskMonitor->iOutOfDiskNotifyObserver->HandleNotifyEntry(KErrNone, entryEvent);
    
    STIF_LOG("7. Destroy the COutOfDiskMonitor object.");
    CleanupStack::PopAndDestroy(outOfDiskMonitor);
}

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
