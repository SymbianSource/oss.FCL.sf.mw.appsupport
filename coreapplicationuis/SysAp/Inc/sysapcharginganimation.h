/*
 ============================================================================
 Name		: sysapcharginganimation.h
 Author	  : 
 Version	 : 1.0
 Copyright   : 
 Description : CSysApChargingAnimation declaration
 ============================================================================
 */

#ifndef CCHARGINGANIMATION_H
#define CCHARGINGANIMATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// CLASS DECLARATION
class   CFbsBitmap;
/**
 *  CSysApChargingAnimation
 * 
 */

enum KChargingMode
{
    KChargingModeUnknown=-1,
    KChargingModeChargingOn=0,
    KChargingModeFull
};
 

class CSysApChargingAnimation : public CCoeControl
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSysApChargingAnimation();

    /**
     * Two-phased constructor.
     */
    static CSysApChargingAnimation* NewL();

    /**
     * Two-phased constructor.
     */
    static CSysApChargingAnimation* NewLC();
    
    void Draw(const TRect& aRect) const;
    
    static TInt TimerExpired( TAny* aObject );
    
    void StartChargingAnimationL();
    void StopAnimation();
    void StartChargingFullAnimationL();
private:
    void SetModeL(TInt aMode);    
    void SetFullFramesL();
    void SetChargingFramesL();
    void DeleteFrames();
    void SetNextFrame();

    void ResetTimer();
    void StopTimer();
    /**
     * Constructor for performing 1st stage construction
     */
    CSysApChargingAnimation();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    CFbsBitmap* ReadSVGL (TFileName aFileName);
    
    CFbsBitmap *iBitmap;
    TInt iFrameCount,iCurrentFrame,iMode;
    CPeriodic *iAnimTimer;
    RPointerArray<CFbsBitmap> iAnimation;
    };

#endif // CCHARGINGANIMATION_H
