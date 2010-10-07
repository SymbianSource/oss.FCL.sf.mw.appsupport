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

#ifndef UT_OODMONITOR_CONST_H_
#define UT_OODMONITOR_CONST_H_

#include <e32base.h>

const TInt KTwoSecondInMicro = 2000000;

const TUid KUTSysApPropertyCategory = {0x10000000};
const TUint32 KHandleShutdownEventKey = 9000;
const TUint32 KGetShutdownStateKey = 9001;

const TInt KHandleShutdownEventValue0 = 1000;
const TInt KHandleShutdownEventValue1 = 99;
const TInt KGetShutdownStateValue0 = 66;
const TInt KGetShutdownStateValue1 = 1;

_LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);

const TInt KMaxFileSize = 0x1000000;   
const TInt KOverlaySize = 1000;
const TInt KMaxFileNameLength = 256;

const TInt KArraySize = 10;

_LIT(KTextFileName, "\\testframework\\oodtest");
_LIT(KDriverC, "c:");
_LIT(KDriverE, "e:");
_LIT(KDriverF, "f:");

#endif  //UT_OODMONITOR_CONST_H_
