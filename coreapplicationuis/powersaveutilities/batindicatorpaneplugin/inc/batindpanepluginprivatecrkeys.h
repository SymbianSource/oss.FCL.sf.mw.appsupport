/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Battery Indicator Pane Plug-in central repository
*                configuration keys.
*
*/


#ifndef BATINDPANEPLUGINPRIVATECRKEYS_H
#define BATINDPANEPLUGINPRIVATECRKEYS_H

// INCLUDES

// =============================================================================
// Battery Indicator Pane Plug-in Configuration API
// =============================================================================
const TUid KCRUidBatIndPanePluginConf = { 0x2001011A };

/**
* Bitmask used in configuring Battery Indicator Pane Plug-in.
*/
const TUint32 KBatIndPanePlugin = 0x00000001;

// =============================================================================
// Battery Indicator Pane Plug-in Local Variation constants
// =============================================================================

/**
* KBatIndPanePluginLinkOnly is an on/off setting for enabling
* a preview popup with battery level information in percents. If enabled, this popup
* is shown only with link for activate/deactivate power save mode. If disabled, this
* popup is shown with icon, battery level information and link.
*
*/
const TUint32 KBatIndPanePluginLinkOnly = 0x01; // 2^0

#endif      // BATINDPANEPLUGINPRIVATECRKEYS_H

// End of File

