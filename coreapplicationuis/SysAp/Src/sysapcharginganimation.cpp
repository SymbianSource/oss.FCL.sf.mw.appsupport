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
* Description:  CSysApLightsController implementation.
*
*/


#include "sysapcharginganimation.h"
#include <eikenv.h>
#include <SVGEngineInterfaceImpl.h>
#include <startupdomainpskeys.h>
#include <hwrmpowerstatesdkpskeys.h>

_LIT(KFrameDir, "z:\\resource\\apps\\");
_LIT(KFrame1, "qtg_anim_battery_charging_01.svg");
_LIT(KFrame2, "qtg_anim_battery_charging_02.svg");
_LIT(KFrame3, "qtg_anim_battery_charging_03.svg");
_LIT(KFrame4, "qtg_anim_battery_charging_04.svg");
_LIT(KFrame5, "qtg_anim_battery_charging_05.svg");
_LIT(KFrame6, "qtg_anim_battery_charging_06.svg");
_LIT(KFrame7, "qtg_anim_battery_charging_07.svg");
_LIT(KFrame8, "qtg_anim_battery_charging_08.svg");
_LIT(KFrame9, "qtg_anim_battery_charging_09.svg");
_LIT(KFrame10,"qtg_anim_battery_charging_10.svg");
_LIT(KFullFrame1, "qtg_anim_battery_full_01.svg");
_LIT(KFullFrame2, "qtg_anim_battery_full_02.svg");
_LIT(KFullFrame3, "qtg_anim_battery_full_03.svg");
_LIT(KFullFrame4, "qtg_anim_battery_full_04.svg");
_LIT(KFullFrame5, "qtg_anim_battery_full_05.svg");
_LIT(KFullFrame6, "qtg_anim_battery_full_06.svg");
_LIT(KFullFrame7, "qtg_anim_battery_full_07.svg");
_LIT(KFullFrame8, "qtg_anim_battery_full_08.svg");
_LIT(KFullFrame9, "qtg_anim_battery_full_09.svg");
_LIT(KFullFrame10,"qtg_anim_battery_full_10.svg");
#define SIZE_X 228//360 //360
#define SIZE_Y 228 //640
static TInt KChargingFrameDuration=800000;
static TInt KFullFrameDuration=200000;

CSysApChargingAnimation::CSysApChargingAnimation()
    {
    // No implementation required
    }

CSysApChargingAnimation::~CSysApChargingAnimation()
    {
    iAnimation.ResetAndDestroy();
    iAnimTimer->Cancel();
    delete iAnimTimer;
    iAnimTimer=NULL;    
    }

CSysApChargingAnimation* CSysApChargingAnimation::NewLC()
    {
    CSysApChargingAnimation* self = new (ELeave) CSysApChargingAnimation();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CSysApChargingAnimation* CSysApChargingAnimation::NewL()
    {
    CSysApChargingAnimation* self = CSysApChargingAnimation::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CSysApChargingAnimation::DeleteFrames()
    {
    iAnimation.ResetAndDestroy();
    }

void CSysApChargingAnimation::ResetTimer()
    {
    if(iAnimTimer)
        {
        iAnimTimer->Cancel();
        delete iAnimTimer;
        iAnimTimer=NULL;
        }

    TRAPD( err, iAnimTimer = CPeriodic::NewL( EPriorityNormal ) );
    if(err==KErrNone)
        {
            if(iMode==KChargingModeFull)
                iAnimTimer->Start(KFullFrameDuration,KFullFrameDuration,TCallBack( TimerExpired, this ));
            else if(iMode==KChargingModeChargingOn)
                iAnimTimer->Start(KChargingFrameDuration,KChargingFrameDuration,TCallBack( TimerExpired, this ));
        }
    }

void CSysApChargingAnimation::StopTimer()
    {
    if(iAnimTimer)
        {
        iAnimTimer->Cancel();
        delete iAnimTimer;
        iAnimTimer=NULL;
        }    
    }

void CSysApChargingAnimation::SetChargingFramesL()
    {
    DeleteFrames();
    TParse* fp = new(ELeave) TParse();
    fp->Set(KFrame1, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame2, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame3, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame4, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame5, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame6, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame7, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame8, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame9, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFrame10, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));    
    }

void CSysApChargingAnimation::SetFullFramesL()
    {
    DeleteFrames();
    TParse* fp = new(ELeave) TParse();
    fp->Set(KFullFrame1, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame2, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame3, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame4, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame5, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame6, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame7, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame8, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame9, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    fp->Set(KFullFrame10, &KFrameDir, NULL);
    iAnimation.Append(ReadSVGL (fp->FullName()));
    }

void CSysApChargingAnimation::SetModeL(TInt aMode)
    {
    if(iMode==aMode) return;

    if(aMode==KChargingModeChargingOn)
        {
        SetChargingFramesL();
        }
    else if(aMode==KChargingModeFull)
        {
        SetFullFramesL();
        }

    iMode=aMode;
    iCurrentFrame=0;     
    if(aMode!=KChargingModeUnknown)
        {
        iBitmap = iAnimation[iCurrentFrame];
        iFrameCount = iAnimation.Count();
        DrawDeferred();
        }
    else
        {
        DeleteFrames();
        iBitmap=NULL;
        iFrameCount=0;
        }
    }

void CSysApChargingAnimation::ConstructL()
    {
    iMode=KChargingModeUnknown;
    
    CreateWindowL();                                      
    SetExtentToWholeScreen();
    ActivateL();
    iEikonEnv->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront );
    }

void CSysApChargingAnimation::Draw(const TRect& aRect) const
{
    if(iBitmap && iBitmap->Handle())
        {
        CWindowGc& gc=SystemGc();    
        TSize bmpSizeInPixels = iBitmap->SizeInPixels();
        //center image to the center of the screen
        TInt xDelta = ( aRect.Width() - bmpSizeInPixels.iWidth ) / 2;
        TInt yDelta = ( aRect.Height() - bmpSizeInPixels.iHeight ) / 2;
        TPoint pos = TPoint( xDelta , yDelta );
        gc.BitBlt(pos, iBitmap);
        }
}

CFbsBitmap* CSysApChargingAnimation::ReadSVGL (TFileName aFileName)
    {
    TFontSpec fontspec;
    TDisplayMode mode = EColor16MA;    
    TSize size(SIZE_X, SIZE_Y);

    CFbsBitmap* frameBuffer = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( frameBuffer );
    frameBuffer->Create( size, mode );
    
    CSvgEngineInterfaceImpl* svgEngine = NULL;
    svgEngine = CSvgEngineInterfaceImpl::NewL(frameBuffer, NULL, fontspec );    
    
    if (svgEngine == NULL)
        {
            return NULL;
        }
    
    CleanupStack::PushL( svgEngine );
    TInt domHandle = 0;
    svgEngine->PrepareDom( aFileName, domHandle ) ;
    if (domHandle == 0)
        {
            return NULL;
        }
    TBool b = svgEngine->IsPanPossible(0,200,NULL);
    CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;    
    CleanupStack::PushL( bitmap );
    User::LeaveIfError( bitmap->Create( size, mode) );

    svgEngine->UseDom( domHandle, bitmap, NULL ) ;
     
    MSvgError* err;
    svgEngine->Start( err );
    if (err->HasError())
        {
            return NULL;
        }

    svgEngine->DeleteDom( domHandle );
    
    CleanupStack::Pop( bitmap );
    CleanupStack::PopAndDestroy( svgEngine );
    CleanupStack::PopAndDestroy( frameBuffer );
    
    return bitmap;
    }

void CSysApChargingAnimation::SetNextFrame()
    {
    if(iFrameCount<=0)
        return;
    
    iCurrentFrame++;
    if(iCurrentFrame>=iFrameCount)
        {
            iCurrentFrame=0;
        }
    
    iBitmap=iAnimation[iCurrentFrame];
    
    DrawDeferred();
    }

TInt CSysApChargingAnimation::TimerExpired( TAny* aObject )
    {
    static_cast<CSysApChargingAnimation*>(aObject)->SetNextFrame();
    return 0;
    }

void CSysApChargingAnimation::StartChargingAnimationL()
    {
    if(iMode!=KChargingModeChargingOn)
        SetModeL(KChargingModeChargingOn);
    
    ResetTimer();
    }

void CSysApChargingAnimation::StopAnimation()
    {
    StopTimer();
    }

void CSysApChargingAnimation::StartChargingFullAnimationL()
    {
    if(iMode!=KChargingModeFull)
        SetModeL(KChargingModeFull);
    
    ResetTimer();
    }
