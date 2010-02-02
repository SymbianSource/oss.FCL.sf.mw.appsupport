/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This module implements the application core i.e. application
*     class and standard exported functions
*
*/


// INCLUDE FILES
#include "StartupApplication.h"
#include "StartupDocument.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CStartupApplication::AppDllUid()
// Returns application UID
// ---------------------------------------------------------------------------
TUid CStartupApplication::AppDllUid() const
    {
    return KUidStartUp;
    }

// ---------------------------------------------------------------------------
// CStartupApplication::CreateDocumentL()
// Creates CStartupDocument object
//
// ---------------------------------------------------------------------------
CApaDocument* CStartupApplication::CreateDocumentL()
    {
    return CStartupDocument::NewL(*this);
    }

// ===================== OTHER EXPORTED FUNCTIONS ============================
#include <eikstart.h>

LOCAL_C CApaApplication* NewApplication()
    {
    return new CStartupApplication;
    }

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

#if defined(__WINS__) && !defined(EKA2)
GLDEF_C TInt E32Dll(TDllReason)
    {
    return KErrNone;
    }

EXPORT_C TInt WinsMain(TDesC* aCmdLine)
    {
    return EikStart::RunApplication(NewApplication, aCmdLine);
    }
#endif
// End of file
