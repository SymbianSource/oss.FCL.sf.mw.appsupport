/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Includes some common defines used in the startup application
*
*/


#ifndef RFSTRACES_H
#define RFSTRACES_H

//CONSTANTS
_LIT( KAppName, "RFS" );           // The application name

// MACROS

#define PANIC(aPanic) User::Panic( KAppName, aPanic )

#define TRACE_ADDPREFIX(aText) (TPtrC((const TText *)L"RFS: \"" L##aText L"\""))

#ifdef _DEBUG
#define TRACES(aMsg) RDebug::Print( TRACE_ADDPREFIX(aMsg) )
#define TRACES1(aFormat,aP1) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1) )
#define TRACES2(aFormat,aP1,aP2) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2) )
#define TRACES3(aFormat,aP1,aP2,aP3) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2),(aP3) )
#define TRACE_IF_ERROR_1(aFormat,aP1) { if ( aP1 != KErrNone ) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1));}
#else
#define TRACES(aMsg)
#define TRACES1(aFormat,aP1)
#define TRACES2(aFormat,aP1,aP2)
#define TRACES3(aFormat,aP1,aP2,aP3)
#define TRACE_IF_ERROR_1(aFormat,aP1)
#endif
#endif      // RFSTRACES_H
            
// End of File
