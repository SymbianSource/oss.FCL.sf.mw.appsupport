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
#include <coreapplicationuisdomainpskeys.h>
#include <e32debug.h>
#include <e32property.h>
#include <f32file.h>
#include <lafshut.h>
#include <w32std.h>


#include "ut_oodmonitor_const.h"
#include "ut_oodmonitor_timer.h"

Cut_OODMonitor_Timer* Cut_OODMonitor_Timer::NewLC(CLafShutdownManager* aShutdownManager, Tut_LafShutdownManager& aFriendLafShutdownManager, TTestCase aTestCase)
    {
    Cut_OODMonitor_Timer* self=new(ELeave) Cut_OODMonitor_Timer(aShutdownManager, aFriendLafShutdownManager, aTestCase);  
    CleanupStack::PushL(self);  
    self->ConstructL();  
    return self;  
    }
    
Cut_OODMonitor_Timer* Cut_OODMonitor_Timer::NewL(CLafShutdownManager* aShutdownManager, Tut_LafShutdownManager& aFriendLafShutdownManager, TTestCase aTestCase)
    {
    Cut_OODMonitor_Timer* self=Cut_OODMonitor_Timer::NewLC(aShutdownManager, aFriendLafShutdownManager, aTestCase);  
    CleanupStack::Pop(self); 
    return self;    
    }

Cut_OODMonitor_Timer::Cut_OODMonitor_Timer(CLafShutdownManager* aShutdownManager, Tut_LafShutdownManager& aFriendLafShutdownManager, TTestCase aTestCase):
        CTimer(EPriorityStandard), iFriendLafShutdownManager(aFriendLafShutdownManager), iShutdownManager(aShutdownManager), iTestCase(aTestCase),
        iPhase(EPhase1)
    {
    
    }
    
Cut_OODMonitor_Timer::~Cut_OODMonitor_Timer()
    {
    iFileNameArray->Reset();
    delete iFileNameArray;
    }

void Cut_OODMonitor_Timer::ConstructL()
    {
    iFileNameArray = new (ELeave) CDesCArrayFlat(KArraySize);
    CTimer::ConstructL();  
    CActiveScheduler::Add(this);  
    After(KTwoSecondInMicro);
    }

void Cut_OODMonitor_Timer::RunL()
    {
    switch(iTestCase)
        {
        case EMMCTest:
            {
            switch(iPhase)
                {
                case EPhase1:
                    {
                    ChangeMMCStatus();                    
                    iPhase = EPhase2;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                case EPhase2:
                    {
                    ChangeMMCStatus();
                    iPhase = EPhase3;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                default:
                    {
                    CActiveScheduler::Stop();
                    break;
                    }
                }
            break;
            }   // EMMCTest
        case EDisplayNotes:
            {
            switch(iPhase)
                {
                case EPhase1:
                    {
                    iFriendLafShutdownManager.EatDiskL(iFileNameArray, EDriveC, ECritical); 
                    ChangeDisplayNotes();                   
                    iPhase = EPhase2;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                case EPhase2:
                    {
                    ChangeDisplayNotes();
                    iPhase = EPhase3;                    
                    After(KTwoSecondInMicro); 
                    break;
                    }
                case EPhase3:
                   {
                   iPhase = EPhase4;
                   iFriendLafShutdownManager.FreeDiskL(iFileNameArray);
                   iFileNameArray->Reset();
                   After(KTwoSecondInMicro); 
                   break;
                   }
                default:
                    {
                    CActiveScheduler::Stop();
                    break;
                    }
                }
            break;
            }   // EDisplayNotes
        case EAutolock:
            {
            switch(iPhase)
                {
                case EPhase1:
                    {
                    iFriendLafShutdownManager.EatDiskL(iFileNameArray, EDriveC, EWarning); 
                    ChangeAutoLockStatus();                    
                    iPhase = EPhase2;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                case EPhase2:
                    {
                    ChangeAutoLockStatus();
                    iPhase = EPhase3;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                case EPhase3:
                    {
                    iPhase = EPhase4;
                    iFriendLafShutdownManager.FreeDiskL(iFileNameArray);
                    iFileNameArray->Reset();
                    After(KTwoSecondInMicro); 
                    break;
                    }
                default:
                    {
                    CActiveScheduler::Stop();
                    break;
                    }
                }
            break;
            }   //   EAutolock     
        case EOODWatcher:
            {
            switch(iPhase)
                {
                case EPhase1:
                    {
                    iFriendLafShutdownManager.EatDiskL(iFileNameArray, EDriveC, ECritical);                  
                    iPhase = EPhase2;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                case EPhase2:
                    {
                    iFriendLafShutdownManager.FreeDiskL(iFileNameArray);
                    iFileNameArray->Reset();
                    iPhase = EPhase3;
                    After(KTwoSecondInMicro); 
                    break;
                    }
                default:
                    {
                    CActiveScheduler::Stop();
                    break;
                    }
                }
            break;
            }   //   EOODWatcher  
        default:
            {
            CActiveScheduler::Stop();
            }   // default
        }
    }
void Cut_OODMonitor_Timer::ChangeMMCStatus()
    {
    TInt value;
        
    RProperty enableMMCWatchProperty = iFriendLafShutdownManager.GetEnableMMCWatchProperty(iShutdownManager);
    enableMMCWatchProperty.Get(value);

    RDebug::Print(_L("CLafShutdownManager::enableMMCWatchProperty=%d"), value);
    
    const TInt mmcRemovedStatus = 0;
    const TInt mmcInsertedStatus = 1;
    
    if(mmcRemovedStatus == value)
        {
        value = mmcInsertedStatus;
        }
    else
        {
        value = mmcRemovedStatus;
        }
    
    enableMMCWatchProperty.Set(value);
    }

void Cut_OODMonitor_Timer::ChangeDisplayNotes()
    {
    TInt value;
    
    RProperty canDisplayNotesProperty = iFriendLafShutdownManager.GetCanDisplayNotesProperty(iShutdownManager);
    canDisplayNotesProperty.Get(value);

    RDebug::Print(_L("CLafShutdownManager::canDisplayNotesProperty=%d"), value);
    
    const TInt notShowDisplayNotes = 0;
    const TInt ShowDisplayNotes = 1;
    
    if(notShowDisplayNotes == value)
        {
        value = ShowDisplayNotes;
        }
    else
        {
        value = notShowDisplayNotes;
        }
    
    canDisplayNotesProperty.Set(value);
    }

void Cut_OODMonitor_Timer::ChangeAutoLockStatus()
    {
    TInt value;
    
    RProperty autolockStatusProperty = iFriendLafShutdownManager.GetAutolockStatusProperty(iShutdownManager);
    autolockStatusProperty.Get(value);

    RDebug::Print(_L("CLafShutdownManager::autolockStatusProperty=%d"), value);
    
    if(EAutolockOff == value)
        {
        value = EManualLocked;
        }
    else
        {
        value = EAutolockOff;
        }
    
    autolockStatusProperty.Set(value);
    }
