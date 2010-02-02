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
*           This class is the container class of the CStartupTone.
*           It is used for playing startup tone.
*
*/


#ifndef __STARTUPTONE_H__
#define __STARTUPTONE_H__

//  INCLUDES
#include <MdaAudioSamplePlayer.h>
#include "startupdefines.h"
#include "startup.hrh"

// CLASS DECLARATION
/**
*  CStartupTone
*  This class is used for playing of startup tone.
*/
class CStartupAppUi;

class CStartupTone : public CBase, public MMdaAudioPlayerCallback
	{
	public:		//Constructors and destructor
        /**
        * C++ constructor.
        */
        CStartupTone( CStartupAppUi* aStartupAppUi );
        
        /**
        * Two-phased constructor.
        */
        static CStartupTone* NewL( CStartupAppUi* aStartupAppUi, TToneType aToneType );

		/**
		* Destructor
		*/
		virtual ~CStartupTone();

		/**
		* Two phase constructor - this creates the audio player object
		*/
		void ConstructL(TToneType aToneType);

	public: // New Functions

		/**
		* Play tone
		*/
		TInt Play();

		/**
		* Stop tone
		*/
        void Stop();

		/**
		* Check is tone currectly playing
		*/        
        TBool Playing();

		/**
		* Audio ready query
		* @return ETrue= audio ready, EFalse=audio not ready
		*/
		TBool AudioReady();

		/**
		* Check if Startup tone is defined and found 
		*/
        TBool ToneFound();

        /**
		*/
        void StartupWaiting(TBool aValue);
    private:

        /**
		*/
        TInt GetRingingToneVolumeL();

	public: // Functions from base classes

        /**
        * From MMdaAudioPlayerCallback, audio initialization complete (Audio ready)
        * @param aError
        * @param aDuration not used internally
        */
		void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);

        /**
        * From MMdaAudioPlayerCallback, audio playing complete
        * @param aError
        */
		void MapcPlayComplete(TInt aError);

	private: //data
		CMdaAudioPlayerUtility*		iTone;
		TBool						iAudioReady;
		TBool						iPlaying;
        TToneType                   iToneType;
        CStartupAppUi*              iStartupAppUi; //uses
        TBool                       iHiddenReset;
        TInt                        iVolume;
        TBool                       iStartupWaitingForTone;
	};

#endif // __STARTUPTONE_H__

// End of File
