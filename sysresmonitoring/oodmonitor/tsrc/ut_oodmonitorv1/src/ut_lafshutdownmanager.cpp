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

#include <barsc.h>
#include <barsread.h>
#include <e32debug.h>
#include <e32property.h>
#include <f32file.h>
#include <lafshut.h>
#include <systemwarninglevels.hrh>
#include <w32std.h>

#include "ut_lafshutdownmanager.h"
#include "ut_oodmonitor_const.h"
#include "ood.h"

Tut_LafShutdownManager::Tut_LafShutdownManager()
    {

    }

Tut_LafShutdownManager::~Tut_LafShutdownManager()
    {

    }

RProperty Tut_LafShutdownManager::GetDiskStatusProperty(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iDiskStatusProperty;
    }

RProperty Tut_LafShutdownManager::GetMMCStatusProperty(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iMMCStatusProperty;
    }

RProperty Tut_LafShutdownManager::GetCanDisplayNotesProperty(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iCanDisplayNotesProperty;
    }

RProperty Tut_LafShutdownManager::GetAutolockStatusProperty(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iAutolockStatusProperty;
    }

RProperty Tut_LafShutdownManager::GetEnableMMCWatchProperty(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iEnableMMCWatchProperty;
    }

CBase* Tut_LafShutdownManager::GetDisplayNotesSubscriber(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iDisplayNotesSubscriber;
    }

CBase* Tut_LafShutdownManager::GetAutolockStatusSubscriber(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iAutolockStatusSubscriber;
    }

CBase* Tut_LafShutdownManager::GetMMCWatchSubscriber(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iMMCWatchSubscriber;
    }

TInt Tut_LafShutdownManager::GetOODWarningThreshold(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iOODWarningThreshold;
    }

TInt Tut_LafShutdownManager::GetOODCriticalThreshold(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iOODCriticalThreshold;
    }

CLafShutdownManager::CGlobalQueryActive*  Tut_LafShutdownManager::GetGlobalQueryActive(CLafShutdownManager* aShutdownManager)
    {
    return aShutdownManager->iGlobalQueryActive;
    }

CActive* Tut_LafShutdownManager::GetOODWarningWatcher(CLafShutdownManager* aShutdownManager)
    {
    RDebug::Print(_L("Tut_LafShutdownManager::iOODWarningWatcher=%d"), aShutdownManager->iOOMMCWarningWatcher);
    return aShutdownManager->iOODWarningWatcher;
    }

CActive* Tut_LafShutdownManager::GetOODCriticalWatcher(CLafShutdownManager* aShutdownManager)
    {
    RDebug::Print(_L("Tut_LafShutdownManager::iOODCriticalWatcher=%d"), aShutdownManager->iOOMMCCriticalWatcher);
    return aShutdownManager->iOODCriticalWatcher;
    }

void Tut_LafShutdownManager::CallReportEventL(CLafShutdownManager* aShutdownManager, 
        MSaveObserver::TSaveType aAction,TBool aPowerOff)
    {
    aShutdownManager->ReportEventL(aAction, aPowerOff);
    }

void Tut_LafShutdownManager::CallGetShutdownState(CLafShutdownManager* aShutdownManager, 
        TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest)
    {
    aShutdownManager->GetShutdownState(aPowerOff, aAllSessionsHavePendingRequest);
    }

TInt Tut_LafShutdownManager::CallGetDiskStatusL(CLafShutdownManager* aShutdownManager, 
        TVolumeInfo& aVolInfo,TDriveNumber aDrive)
    {
    return aShutdownManager->GetDiskStatusL(aVolInfo, aDrive);
    }

void Tut_LafShutdownManager::CreateFileL(const TDesC &aFileName, TInt aSize)
    {   
    RDebug::Print(_L("CLafShutdownManager:: create filename = %S "), &aFileName);
    RDebug::Print(_L("CLafShutdownManager:: size = %d "), aSize);
    RFs fs;
        
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);  
    
    RFile file;
    CleanupClosePushL(file);     
    User::LeaveIfError(file.Replace(fs, aFileName, EFileWrite));
    User::LeaveIfError(file.SetSize(aSize));
            
    CleanupStack::PopAndDestroy(2); 
    RDebug::Print(_L("CLafShutdownManager:: CreateFileL end")); 
    }

void Tut_LafShutdownManager::DeleteFileL(const TDesC &aFileName)
    {   
    RDebug::Print(_L("CLafShutdownManager:: delete filename = %S "), &aFileName);
    RFs fs;       
    User::LeaveIfError(fs.Connect());    
    CleanupClosePushL(fs);     
    User::LeaveIfError(fs.Delete(aFileName));
    CleanupStack::PopAndDestroy(); 
    RDebug::Print(_L("CLafShutdownManager:: DeleteFileL end"));
    }

void Tut_LafShutdownManager::GetDiskInfoL(TVolumeInfo& aVolInfo, TDriveNumber aDrive)
    {
    RDebug::Print(_L("CLafShutdownManager::GetDiskInfoL"));
    RFs fs;       
    User::LeaveIfError(fs.Connect());    
    CleanupClosePushL(fs);  

    User::LeaveIfError(fs.Volume(aVolInfo,aDrive));
    
    CleanupStack::PopAndDestroy(); 
    }

void Tut_LafShutdownManager::EatDiskL(CDesCArrayFlat* aDescArray, TDriveNumber aDriver, TDiskLevel aLevel )
    {
    RDebug::Print(_L("CLafShutdownManager::EatDiskL"));
    TVolumeInfo volInfo;
    TInt err;
    TInt threshod;
    
    if(EWarning == aLevel)
        {
        threshod = KDRIVECWARNINGTHRESHOLD;
        }
    else if (ECritical == aLevel)
        {
        threshod = KDRIVECCRITICALTHRESHOLD;
        }
    else
        {
        threshod =0;
        }
    
    TRAP(err,GetDiskInfoL(volInfo, aDriver));
    
    if (KErrNone == err)
        {
        TInt leftFileSize;
        TInt64 freeSpace = volInfo.iFree;
        RDebug::Print(_L("CLafShutdownManager:: free disk size = %ld"),freeSpace);
        RDebug::Print(_L("CLafShutdownManager:: threshod = %d"),threshod);
        
        if((freeSpace + KOverlaySize) > threshod)
            {
            TInt fileNum = (freeSpace - threshod + KOverlaySize) / KMaxFileSize;
            leftFileSize = freeSpace + KOverlaySize - fileNum*KMaxFileSize - threshod;
            RDebug::Print(_L("CLafShutdownManager:: fileNume = %d"),fileNum);
            RDebug::Print(_L("CLafShutdownManager:: leftFileSize = %d"),leftFileSize);
            
            TBuf<KMaxFileNameLength> drivers;
            
            if(EDriveE == aDriver)
                {
                TBuf<KMaxFileNameLength> driver(KDriverE);
                drivers.Append(driver);
                }
            else if(EDriveF == aDriver)
                {
                TBuf<KMaxFileNameLength> driver(KDriverF);
                drivers.Append(driver);
                }
            else
                {
                TBuf<KMaxFileNameLength> driver(KDriverC);
                drivers.Append(driver);
                }
            TBuf<KMaxFileNameLength> fileName(KTextFileName);
            TBuf<KMaxFileNameLength> path;
            path.Append(drivers);
            path.Append(fileName);
            TRAP(err, CreateFileL(path, leftFileSize));
            if(KErrNone == err)
                {
                aDescArray->AppendL(path);            
                }
            
            for(TInt i = 1; i<=fileNum; i++)
                {
                TBuf <KMaxFileNameLength> fileNo;
                fileNo.Num(i);
    
                TBuf<KMaxFileNameLength> newPath;
                newPath.Append(drivers);
                newPath.Append(fileName);
                newPath.Append(fileNo);
                
                TRAP(err, CreateFileL(newPath, KMaxFileSize));
                if(KErrNone == err)
                    {
                    aDescArray->AppendL(newPath);            
                    }
                }
            }
        }
    }

void Tut_LafShutdownManager::FreeDiskL(CDesCArrayFlat* aDescArray)
    {
    RDebug::Print(_L("CLafShutdownManager::FreeDiskL"));
    TInt fileNum = aDescArray->MdcaCount();
    
    for(TInt i=0; i<fileNum; i++)
        {
        TBuf<KMaxFileNameLength> fileName=(aDescArray->MdcaPoint(i));
        DeleteFileL(fileName);
        }
    }

CLafShutdownManager::CGlobalQueryActive::CMessageInfo* Tut_LafShutdownManager::
        GetMessageInfoList(CLafShutdownManager::CGlobalQueryActive* aGlobalQueryActive, TInt aIndex)
    {
    return aGlobalQueryActive->iMessageInfo[aIndex];
    }

CLafShutdownManager::CGlobalQueryActive::TMessageType Tut_LafShutdownManager::
        GetMessageType(CLafShutdownManager::CGlobalQueryActive* aGlobalQueryActive)
    {
    return aGlobalQueryActive->iMessageType;
    }

void Tut_LafShutdownManager::CallFreeDiskThresholdCrossedL(CLafShutdownManager* aShutdownManager, 
        TDriveNumber aDrive, TBool aIsCallBack, TBool aForcedNote)
    {
    aShutdownManager->FreeDiskThresholdCrossedL(aDrive, aIsCallBack, aForcedNote);
    }


