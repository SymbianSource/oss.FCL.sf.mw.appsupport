/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <QObject>
#include <QVariant>
#include <e32des16.h> 

#include <hbindicatorinterface.h>
#include "accindicator.h" 
#include <accpolgenericiddefinitions.h>

#include "utslot.h"
#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
AccIndicatorPlugin* accIndicatorPlugin;  
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
    accIndicatorPlugin = new (ELeave) AccIndicatorPlugin();
    CleanupStack::PushL(accIndicatorPlugin);
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
    CleanupStack::PopAndDestroy(accIndicatorPlugin);
}

/**
 * STIF_TESTDEFINE defines a test case
 *
 *  Example test case - length of string is checked.
 *  The only argument of macro is a name of test case.
 */


STIF_TESTDEFINE(createIndicator)
    {    
    //
    QString par1;
    STIF_ASSERT_EQUALS((HbIndicatorInterface*)accIndicatorPlugin, accIndicatorPlugin->createIndicator(par1));
    }

STIF_TESTDEFINE(indicatorTypes)
    {
    QString KIndicatorType = "com.nokia.accessory.indicatorplugin/1.0";
    QStringList qStringList = accIndicatorPlugin->indicatorTypes();
    STIF_ASSERT_EQUALS(KIndicatorType, qStringList[0]);
    }

STIF_TESTDEFINE(accessAllowed)
    {
    QVariantMap mapValues;
    QString KAccMode = "AccMode";
    QString KAccType = "AccType";
    mapValues[KAccMode] = (TInt)EAccModeWiredHeadset;
    mapValues[KAccType] = (TInt)KPCWired;
    QString par1;
    
    bool expectedResult = true;
    STIF_ASSERT_EQUALS(expectedResult, accIndicatorPlugin->accessAllowed(par1, mapValues));
    }

STIF_TESTDEFINE(handleClientRequest_indicatorData)
    {
    // connect test slot to signal
    UTSlot *utSlot = new (ELeave) UTSlot();
    CleanupStack::PushL(utSlot);
    QObject::connect( accIndicatorPlugin, SIGNAL(dataChanged()),                       
                      utSlot, SLOT(dataChangedSlot()));
    QObject::connect( accIndicatorPlugin, SIGNAL(deactivate()),                       
                      utSlot, SLOT(deactivateSlot()));
    utSlot->iDataChangedSlotCalled = EFalse;
    
    // make a qmap to pass parameters
    QVariantMap mapValues;
    QString KAccMode = "AccMode";
    QString KAccType = "AccType";
    mapValues[KAccMode] = (TInt)EAccModeWiredHeadset;
    mapValues[KAccType] = (TInt)KPCWired;

    // handleclientrequest activate
    HbIndicatorInterface::RequestType requestType = HbIndicatorInterface::RequestActivate;
    accIndicatorPlugin->handleClientRequest(requestType, mapValues);
    
    // verify that slot had been called 
    TBool expectedResult = ETrue;
    STIF_ASSERT_EQUALS(expectedResult, utSlot->iDataChangedSlotCalled);
    
    // verify data values
    QString displayName = accIndicatorPlugin->indicatorData(HbIndicatorInterface::PrimaryTextRole).toString();
    QString iconName = accIndicatorPlugin->indicatorData(HbIndicatorInterface::DecorationNameRole).toString();
    QString KDisplayName = "Wired Headset";
    QString KIconName = "z:/resource/accindicator/wired_accessory.svg";    
    STIF_ASSERT_EQUALS(KDisplayName, displayName);
    STIF_ASSERT_EQUALS(KIconName, iconName);
            
    // handleClientReqeust deactive
    utSlot->iDeactiveSlotCalled = EFalse;
    requestType = HbIndicatorInterface::RequestDeactivate;
    accIndicatorPlugin->handleClientRequest(requestType, mapValues);
    
    // verify that slot had been called 
    expectedResult = ETrue;
    STIF_ASSERT_EQUALS(expectedResult, utSlot->iDeactiveSlotCalled);
    // verify data values
    QString emptyString;
    displayName = accIndicatorPlugin->indicatorData(HbIndicatorInterface::PrimaryTextRole).toString();
    STIF_ASSERT_EQUALS(emptyString, displayName);
        
    CleanupStack::PopAndDestroy(utSlot);
    }

STIF_TESTDEFINE(handleInteraction_EAccModeWiredHeadset)
    {
    // make a qmap to pass parameters
    QVariantMap mapValues;
    QString KAccMode = "AccMode";
    QString KAccType = "AccType";
    mapValues[KAccMode] = (TInt)EAccModeWiredHeadset;
    mapValues[KAccType] = (TInt)KPCWired;

    // handleclientrequest activate
    HbIndicatorInterface::RequestType requestType = HbIndicatorInterface::RequestActivate;
    accIndicatorPlugin->handleClientRequest(requestType, mapValues);
    
    HbIndicatorInterface::InteractionType interactionType;
    interactionType = HbIndicatorInterface::NoInteraction;
    
    // NoInteraction, handled = false
    bool expectedResult = false;
    STIF_ASSERT_EQUALS(expectedResult, accIndicatorPlugin->handleInteraction(interactionType));
    
    // InteractionActivated, handled = true
    interactionType = HbIndicatorInterface::InteractionActivated;
    expectedResult = true;
    STIF_ASSERT_EQUALS(expectedResult, accIndicatorPlugin->handleInteraction(interactionType));    

    }
STIF_TESTDEFINE(handleInteraction_EAccModeTVOut)
    {
    // make a qmap to pass parameters
    QVariantMap mapValues;
    QString KAccMode = "AccMode";
    QString KAccType = "AccType";
    mapValues[KAccMode] = (TInt)EAccModeTVOut;
    mapValues[KAccType] = (TInt)KPCHDMI;
    // handleclientrequest activate
    HbIndicatorInterface::RequestType requestType = HbIndicatorInterface::RequestActivate;
    accIndicatorPlugin->handleClientRequest(requestType, mapValues);
    
    HbIndicatorInterface::InteractionType interactionType;
    interactionType = HbIndicatorInterface::NoInteraction;
    
    // NoInteraction, handled = false
    bool expectedResult = false;
    STIF_ASSERT_EQUALS(expectedResult, accIndicatorPlugin->handleInteraction(interactionType));
    
    // InteractionActivated, handled = true
    interactionType = HbIndicatorInterface::InteractionActivated;
    expectedResult = true;
    STIF_ASSERT_EQUALS(expectedResult, accIndicatorPlugin->handleInteraction(interactionType));    
    }

#endif
/**
 * END OF TEST CASES SECTION
 */

// End of File
