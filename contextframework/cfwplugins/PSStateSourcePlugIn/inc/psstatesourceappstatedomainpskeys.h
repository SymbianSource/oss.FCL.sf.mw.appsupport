/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This header file contains the definition for the app state P&S
*  API. Purpose of this API is for applications to publish their internal
*  state information. This information is used for example in light control
*  use cases
*
*/


#ifndef PSSTATESOURCEAPPSTATEDOMAINPSKEYS_H
#define PSSTATESOURCEAPPSTATEDOMAINPSKEYS_H

/**< The UID of the category we use. */
const TUid KPCFWAppStatePSUid = {0x10282BC4}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                 ///
/// Each application has own P&S key for publishing state information. Following definitions define ///
/// what key value corresponds to which application and what are the possible values for each				///
/// application state(= key).																																				///
///                                                                                                 ///
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Music player app key
* Value See <code>KPCFWAppStatePSMusicPlayerState</code>
* Type <code>RProperty::EInt</code>.
*/
const TUint32 KPCFWAppStatePSMusicPlayer = 0x00000000;

/**
* Possible states for music player
*/
enum KPCFWAppStatePSMusicPlayerState
    {
    MusicPlayerTextInput
    };
    
/**
* Gallery app key
* Value See <code>KPCFWAppStatePSGalleryState</code>
* Type <code>RProperty::EInt</code>.
*/
const TUint32 KPCFWAppStatePSGallery = 0x00000001;

/**
* Possible states for gallery application
*/
enum KPCFWAppStatePSGalleryState
    {
    GalleryInCarouselView, 
    GalleryInFullScreenView,
    GalleryInZoomedView,
    GalleryInVideoPlaybackView,
    GalleryInSlideshowView,
    GalleryInListView, 		/**< Covers a multitude of misc. views all based on non-carousel lists */
    GalleryInTagBrowserView,
    GalleryInMapBrowserView,
    GalleryTextInput
    };        
    
/**
* NGI app key
* Value See <code>KPCFWAppStatePSNGIState</code>
* Type <code>RProperty::EInt</code>.
*/
const TUint32 KPCFWAppStatePSNGI = 0x00000002;

/**
* Possible states for NGI (NGI inside)
*/
enum KPCFWAppStatePSNGIState
    {
    NGIGameForegroundLandscape,                /**< NGI game in foreground and in landscape 90 degree mode */
    NGIGameForegroundLandscapeUpsideDown,      /**< NGI game in foreground and in landscape 270 degree mode */ 
    NGIGameForegroundPortrait,                 /**< NGI game in foreground and in portrait mode  */
    NGIGameBackground                          /**< NGI game in background or closing down */
    };
  
/**
* Camera app key
* Value See <code>KPCFWAppStatePSNGIState</code>
* Type <code>RProperty::EInt</code>.
*/
const TUint32 KPCFWAppStatePSCamera = 0x00000003;
      
enum KPCFWAppStatePSCameraState
    {
    CameraStartUp,  /** < Startup*/
    CameraMainStillCaptureView, /** <Main Camera Still Capture Viewfinding */
    CameraMainStillCaptureViewShortcuts, /** < Main Camera Still Capture Viewfinding with shortcuts */
    CameraMainMultishotView, /** < Main Camera Multishot Viewfinding */
    CameraMainMultishotViewShortCuts, /** < Main Camera Multishot Viewfinding with shortcuts */
    CameraSecondaryStillCaptureView, /** < Secondary Camera Still Capture Viewfinding */
    CameraSecondaryMultishotView, /** < Secondary Camera Multishot Viewfinding */
    CameraMainVideoCaptureView, /** < Main Camera Video Capture Viewfinding */
    CameraMainVideoCaptureViewShortcuts, /** < Main Camera Video Capture Viewfinding with shortcuts */
    CameraSecondaryVideoCaptureView, /** < Secondary Camera Video Capture Viewfinding */
    CapturingStillImage, /** < Capturing Still Image */
    PostStillCapture, /** < Post Still Capture */
    CapturingMultishotImages, /** < Capturing Multishot Images */
    PostMultishotCapture, /** < Post Multishot Capture */
    MainRecordingVideo, /** < Main Camera Recording Video */
    MainRecordingVideoShortCuts, /** < Main Camera Recording Video with shortcuts */
    SecondaryRecordingVideo, /** < Secondary Camera Recording Video */
    PostVideoCapture, /** < Post Video Capture */
    CameraSettings, /** < Settings */
    CameraTextInput /** < Text Input */	
    };

/**
* Visual radio app key
* Value See <code>KPCFWAppStatePSVRState</code>
* Type <code>RProperty::EInt</code>.
*/
const TUint32 KPCFWAppStatePSVR = 0x00000004;

/**
* Possible states for Visual radio
*/
enum KPCFWAppStatePSVRState
    {
    VisualRadioBackgroundPlaying,   
    VisualRadioForegroundPlaying,  
    VisualRadioForegroundPlayingDisplayLightsAlwaysOn  
    };

/**
* Video center app key
* Value See <code>KPCFWAppStatePSVideoCenterPlayerState</code>
* Type <code>RProperty::EInt</code>.
*/
const TUint32 KPCFWAppStatePSVideoCenter = 0x00000005;

/**
* Possible states for Video center
*/
enum KPCFWAppStatePSVideoCenterState
    {
    VideoCenterPlaying,
    VideoCenterPaused,
    VideoCenterStopped,
    VideoCenterTextInput
    };


/**
* Navi power targets
*/
const TUint32 KNaviPowerPrimary =   0x00000006; 
const TUint32 KNaviPowerSecondary = 0x00000007;

/**
* Navi power modes
*/
enum TNaviPowerMode
    {
    ENaviPowerModeOff = 0,  
    ENaviPowerModeMin,      
    ENaviPowerModeNormal,  
    };

/**
* Cenrep page setting
* @type RProperty::EInt
*/
const TUint32 KNaviCenrepPage = 0x00000008;

/**
* Defines a cenrep page for Naviscroll
*/
enum TNaviCenrepPage
    {     
    ENaviCenrepPage0 = 0,      
    ENaviCenrepPage1,
    ENaviCenrepPage2,
    ENaviCenrepPage3
    };
#endif//PSSTATESOURCEAPPSTATEDOMAINPSKEYS_H

