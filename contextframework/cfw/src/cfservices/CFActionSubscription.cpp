/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionSubscription class implementation.
*
*/


#include "cfactionsubscription.h"
#include "CFActionSubscriptionImpl.h"


EXPORT_C CCFActionSubscription* CCFActionSubscription::NewL()
    {
    return CCFActionSubscriptionImpl::NewL();
    }

EXPORT_C CCFActionSubscription* CCFActionSubscription::NewLC()
    {
    return CCFActionSubscriptionImpl::NewLC();
    }
