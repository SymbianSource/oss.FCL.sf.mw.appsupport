/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This class is the container class of the CStartupTone.
*     Is used to play startup tone.
*
*/


// INCLUDE FILES
#include <Profile.hrh>
#include <centralrepository.h>
#include <StartupDomainCRKeys.h>
#include <ProfileEngineSDKCRKeys.h>
#include "StartupTone.h"
#include "StartupAppUi.h"
#include "AudioPreference.h"

#define MIN_VOLUME 0
#define MAX_VOLUME 10

//=============================== MEMBER FUNCTIONS ============================
// ---------------------------------------------------------
// Constructor & destructor
// EPOC two phased constructor
// ---------------------------------------------------------
//
CStartupTone::CStartupTone( CStartupAppUi* aStartupAppUi ) :
    iTone( NULL ),
    iAudioReady( EFalse ),
    iPlaying( EFalse ),
    iStartupAppUi( aStartupAppUi ),
    iHiddenReset (EFalse ),
    iStartupWaitingForTone ( EFalse )
    {
    }

CStartupTone::~CStartupTone()
    {
    TRACES("CStartupTone::~CStartupTone()");

    if (iTone)
        {
        if (iPlaying)
            {
            TRACES("CStartupTone::~CStartupTone(): Still playing. Stop it!");
            iTone->Stop();
            }
        delete iTone;
        iTone = NULL;
        TRACES("CStartupTone::~CStartupTone(): iTone deleted");
        }
    TRACES("CStartupTone::~CStartupTone(): End");
    }

// ----------------------------------------------------
// CStartupTone::NewL( CStartupAppUi* aStartupAppUi )
// ----------------------------------------------------
CStartupTone* CStartupTone::NewL( CStartupAppUi* aStartupAppUi, TToneType aToneType )
    {
    TRACES("CStartupTone::NewL()");
    CStartupTone* self = new (ELeave) CStartupTone( aStartupAppUi );
    
    CleanupStack::PushL( self );
    self->ConstructL(aToneType);
    CleanupStack::Pop(); // self

    TRACES("CStartupTone::NewL(): End");
    return self;
    }

void CStartupTone::ConstructL(TToneType aToneType)
    {
    TRACES("CStartupTone::ConstructL()");
    // Check tone volume
    iVolume = GetRingingToneVolumeL();
    // Check if hidden reset
    iHiddenReset = iStartupAppUi->HiddenReset();

    iToneType = aToneType;

    if ((!iHiddenReset) && (iVolume))
        {
        TPath tonePath;
        TRACES("CStartupTone::ConstructL(): Get tone path from CenRep");

        CRepository* repository(NULL);

        TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
        if ( err != KErrNone )
            {
            TRACES("CStartupTone::ConstructL(): End, ERROR: Failed to get startup tone path");
            return;
            }
        if (iToneType == EStartupTone)
            {
            TRACES("CStartupTone::ConstructL(): Tone type EStartupTone");
            err = repository->Get( KStartupTonePath, tonePath );
            }
        else
            {
            TRACES("CStartupTone::ConstructL(): Tone type EStartupOpTone");
            err = repository->Get( KStartupOperatorTonePath, tonePath );
            }
        delete repository;

        TRACES2("CStartupTone::ConstructL(): Get tone to play. err = %d, Path = '%S'", err, &tonePath );

        RFs fs;
        err = fs.Connect();
        TFindFile findExe(fs);
        err = findExe.FindByPath( tonePath, NULL );
        fs.Close();
        if (err != KErrNone)
            {
            TRACES1("CStartupTone::ConstructL(): Tone to play: Cannot find tone. err = %d", err);
            }
        else
            {
            TRACES("CStartupTone::ConstructL(): Tone found");
            iTone = CMdaAudioPlayerUtility::NewFilePlayerL(
                        tonePath, 
                        *this, KAudioPriorityPhonePower, 
                        TMdaPriorityPreference( KAudioPrefDefaultTone));
            }
        }
    }
// ---------------------------------------------------------
// void CStartupTone::Play()
// ---------------------------------------------------------
//

TInt CStartupTone::Play()
    {
    TRACES("CStartupTone::Play()");
    TRACES1("CStartupTone::Play(): Tone type: %d", iToneType);
    if (iAudioReady && !iHiddenReset && iVolume && iTone)
        {
        TRACES("CStartupTone::Play(): Audio ready. Play tone");
        iVolume = Max( MIN_VOLUME, Min( iVolume, MAX_VOLUME ) );
        iTone->SetVolume(iVolume);
        iTone->Play();
        iPlaying = ETrue;
        TRACES("CStartupTone::Play(): End, return KErrNone");
        return KErrNone;
        }
    else
        {
        TRACES("CStartupTone::Play(): Audio not ready, hidden reset, volume null or tone is not initialized. Unable to play tone.");
        TRACES1("CStartupTone::Play(): Audio ready:  %d",iAudioReady);
        TRACES1("CStartupTone::Play(): Hidden reset: %d",iHiddenReset);
        TRACES1("CStartupTone::Play(): Volume:       %d",iVolume);
        TRACES1("CStartupTone::Play(): Tone:         %d",iTone);
        TRACES("CStartupTone::Play(): End, return KErrNotReady");
        return KErrNotReady;
        }
    }

// ---------------------------------------------------------
// void CStartupTone::Stop()
// ---------------------------------------------------------
//

void CStartupTone::Stop()
    {
    TRACES("CStartupTone::Stop()");
    if (iTone)
        {
        TRACES("CStartupTone::Stop(): Stop the tone");
        iPlaying=EFalse;
        iTone->Stop();
        iToneType = EStartupNoTone;
        MapcPlayComplete(KErrNone);
        }
    TRACES("CStartupTone::Stop(): End");
    }

// ---------------------------------------------------------
// CStartupTone::ToneFound()
// ---------------------------------------------------------
//
TBool CStartupTone::ToneFound()
    {
    TBool status(EFalse);
    if(iTone)
        status = ETrue;
    return status;
    }

// ---------------------------------------------------------
// CStartupTone::AudioReady()
// ---------------------------------------------------------
//
TBool CStartupTone::AudioReady()
    {
    return iAudioReady;
    }

// ---------------------------------------------------------
// void CStartupTone::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/)
// ---------------------------------------------------------
//
void CStartupTone::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/)
    {
    TRACES("CStartupTone::MapcInitComplete()");
    if (aError == KErrNone)
        {
        TRACES("CStartupTone::MapcInitComplete(): Ready to play startup tone");
        iAudioReady = ETrue;
        }
    else
        {
        TRACES("CStartupTone::MapcInitComplete(): Unable to play startup tone");
        }
    TRACES("CStartupTone::MapcInitComplete(): End");
    }

// ---------------------------------------------------------
// void CStartupTone::MapcPlayComplete(TInt /*aError*/)
// ---------------------------------------------------------
//
void CStartupTone::MapcPlayComplete(TInt /*aError*/)
    {
    TRACES("StartupTone::MapcPlayComplete()");
    iPlaying=EFalse;
    if (iStartupWaitingForTone)
        {
        TRACES("StartupTone::MapcPlayComplete(): Startup waiting ");
        TRAPD(err, iStartupAppUi->ContinueStartupAfterToneL(iToneType));
        if (err != KErrNone)
            {
            TRACES1("CStartupTone::MapcPlayComplete(): ContinueStartupAfterToneL() leaves, err = %d", err );
            }
        }
    TRACES("StartupTone::MapcPlayComplete(): End");
    }

// ---------------------------------------------------------
// TBool CStartupTone::Playing()
// ---------------------------------------------------------
//
TBool CStartupTone::Playing()
    {
    TRACES1("StartupTone::Playing(): Return %d", iPlaying );
    return iPlaying;
    }

// ---------------------------------------------------------
// void CStartupTone::StartupWaiting()
// ---------------------------------------------------------
//
void CStartupTone::StartupWaiting(TBool aValue)
    {
    TRACES1("StartupTone::StartupWaiting(): aValue == %d", aValue);
    iStartupWaitingForTone = aValue;
    }

// ----------------------------------------------------------
// CStartupTone::GetRingingToneVolumeL
// Startup tone volume is always 4 but when ringing type is 
// silent or ringing volume is 0 or 1 startup tone is silent.
// ----------------------------------------------------------
//
TInt CStartupTone::GetRingingToneVolumeL()
    {
    TRACES("StartupTone::GetRingingToneVolumeL()");

    TInt retval(0);
    TInt ringingType(EProfileRingingTypeSilent);
    TInt ringingVol(0);
    
    CRepository* repository(NULL);

    TRAPD( err, repository = CRepository::NewL( KCRUidProfileEngine ) );
    if ( err != KErrNone )
        {
        TRACES("StartupTone::GetRingingToneVolumeL(): End, ERROR, Cannot connect to CenRep");
        return 0;    
        }

    User::LeaveIfError( repository->Get( KProEngActiveRingingVolume, ringingVol ));
    User::LeaveIfError( repository->Get( KProEngActiveRingingType, ringingType ));
    delete repository;

    TRACES1("StartupTone::GetRingingToneVolumeL(): Ringing tone volume = %d", ringingVol);
    TRACES1("StartupTone::GetRingingToneVolumeL(): Ringing type = %d", ringingType);

    if ((ringingType != EProfileRingingTypeSilent) && 
        (ringingVol != 0) &&
        (ringingVol != EProfileRingingVolumeLevel1))
        {
        TRACES("StartupTone::GetRingingToneVolumeL(): Get startup tone volume");
        TInt defaultRingingVol;
        CRepository* repository(NULL);

        TRAPD( err, repository = CRepository::NewL( KCRUidStartupConf ) );
        if ( err != KErrNone )
            {
            return 0;    
            }

        User::LeaveIfError( repository->Get( KStartupToneVolume, defaultRingingVol ));
        delete repository;

        ringingVol = defaultRingingVol;
        retval =  ringingVol;
        }

    TRACES1("StartupTone::GetRingingToneVolumeL(): End, return %d", retval);
    return retval;
    }

// End of File
