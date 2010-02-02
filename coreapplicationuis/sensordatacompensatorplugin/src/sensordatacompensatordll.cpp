/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Creates Sensor Data Compensator
*
*/


// INCLUDE FILES
#include <e32std.h>
#include "sensordatacompensatorplg.h"
#include "trace.h"

EXPORT_C CSensorDataCompensatorPlugin* CreateSensorDataCompensatorL(
    TSensrvChannelDataTypeId aDataType,
    TSensorCompensationType aType,
    MSensorDataCompensatorObserver& aObserver )
    {
	FUNC_LOG;

    CSensorDataCompensatorPlugin* plg =
        CSensorDataCompensatorPlg::NewL( aDataType, aType, aObserver );

    return plg;
    }
