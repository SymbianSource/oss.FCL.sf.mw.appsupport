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
* Description:
*
*/

#ifndef UT_LAFSHUTDOWNMANAGER_H_
#define UT_LAFSHUTDOWNMANAGER_H_

#include <e32property.h>
#include "ood.h"

enum TDiskLevel
    {
    EWarning,
    ECritical
    };

class Tut_LafShutdownManager
    {
public:
    Tut_LafShutdownManager();
    virtual ~Tut_LafShutdownManager();
    
    RProperty GetDiskStatusProperty(CLafShutdownManager* aShutdownManager);
    RProperty GetMMCStatusProperty(CLafShutdownManager* aShutdownManager);
    RProperty GetCanDisplayNotesProperty(CLafShutdownManager* aShutdownManager);
    RProperty GetAutolockStatusProperty(CLafShutdownManager* aShutdownManager);
    RProperty GetEnableMMCWatchProperty(CLafShutdownManager* aShutdownManager);

    CBase* GetDisplayNotesSubscriber(CLafShutdownManager* aShutdownManager);
    CBase* GetAutolockStatusSubscriber(CLafShutdownManager* aShutdownManager);
    CBase* GetMMCWatchSubscriber(CLafShutdownManager* aShutdownManager);
    
    CLafShutdownManager::CGlobalQueryActive*  GetGlobalQueryActive(CLafShutdownManager* aShutdownManager);
    CActive* GetOODWarningWatcher(CLafShutdownManager* aShutdownManager);
    CActive* GetOODCriticalWatcher(CLafShutdownManager* aShutdownManager);
    
    TInt GetOODWarningThreshold(CLafShutdownManager* aShutdownManager);
    TInt GetOODCriticalThreshold(CLafShutdownManager* aShutdownManager);
    
    void CallReportEventL(CLafShutdownManager* aShutdownManager, 
            MSaveObserver::TSaveType aAction,TBool aPowerOff);
    void CallGetShutdownState(CLafShutdownManager* aShutdownManager, 
            TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest);
    TInt CallGetDiskStatusL(CLafShutdownManager* aShutdownManager,
            TVolumeInfo& aVolInfo,TDriveNumber aDrive);
    
    void CreateFileL(const TDesC &aFileName, TInt aSize);
    void DeleteFileL(const TDesC &aFileName);
    void FreeDiskL(CDesCArrayFlat* aDescArray);
    void EatDiskL(CDesCArrayFlat* aDescArray, TDriveNumber aDriver, TDiskLevel aLevel);
    void GetDiskInfoL(TVolumeInfo& aVolInfo, TDriveNumber aDrive);
    
    void CallFreeDiskThresholdCrossedL(CLafShutdownManager* aShutdownManager, TDriveNumber aDrive, TBool aIsCallBack, TBool aForcedNote);
    CLafShutdownManager::CGlobalQueryActive::CMessageInfo* GetMessageInfoList(CLafShutdownManager::CGlobalQueryActive* aGlobalQueryActive,
            TInt aIndex);
    CLafShutdownManager::CGlobalQueryActive::TMessageType 
            GetMessageType(CLafShutdownManager::CGlobalQueryActive* aGlobalQueryActive);
    };

#endif /* UT_LAFSHUTDOWNMANAGER_H_ */


