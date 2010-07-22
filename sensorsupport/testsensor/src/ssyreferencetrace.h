// ssyreferencetrace.h

/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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




/**
 @file
 @internalComponent
*/
#ifndef SSYREFERENCETRACE_H
#define SSYREFERENCETRACE_H


// INCLUDES
#include <e32svr.h>


// PC-LINT OPTIONS
// Removes null statement not in line by itself warnings from 
// COMPONENT_TRACE macros
//lint -esym(960,54)

// Removes "area too small" pointer cast warnings.
//lint -e826



// CONSTANTS
// MACROS
#ifdef _DEBUG

    #ifdef THREAD_TRACE_FLAG
    
        #define THREAD_TRACE RDebug::Print(RThread().Name());
        
    #else
        
        #define THREAD_TRACE
        
    #endif // #ifdef THREAD_TRACE_FLAG
    
    #ifdef BUFFER_TRACE_FLAG
    
        #define BUFFER_TRACE_DEBUG
        #define DEBUG_PRINT_BUFFER DebugPrintBuffer();
        #define BUFFER_TRACE( a ) RDebug::Print a 
        
    #else
        
        #define DEBUG_PRINT_BUFFER
        #define BUFFER_TRACE( a )
        
    #endif // #ifdef THREAD_TRACE_FLAG

    #ifdef COMPONENT_TRACE_FLAG

        #define COMPONENT_TRACE( a ) THREAD_TRACE;RDebug::Print a 
        #define COMPONENT_TRACE_DEBUG   
        
    #else // #ifdef COMPONENT_TRACE_FLAG

        #define COMPONENT_TRACE( a )

    #endif //#ifdef COMPONENT_TRACE_FLAG

    #ifdef API_TRACE_FLAG

        #define API_TRACE( a ) THREAD_TRACE;RDebug::Print a
        #define API_TRACE_DEBUG

    #else //#ifdef API_TRACE_FLAG

        #define API_TRACE( a )

    #endif //#ifdef API_TRACE_FLAG

    #ifdef ERROR_TRACE_FLAG

        #define ERROR_TRACE( a ) THREAD_TRACE;RDebug::Print a
        #define ERROR_TRACE_DEBUG

    #else //#ifdef ERROR_TRACE_FLAG

        #define ERROR_TRACE( a )

    #endif //#ifdef ERROR_TRACE_FLAG

#else // #ifdef _DEBUG

    #define COMPONENT_TRACE( a )
    #define API_TRACE( a )
    #define ERROR_TRACE( a )
    #define BUFFER_TRACE( a )
    #define THREAD_TRACE
    #define DEBUG_PRINT_BUFFER

#endif //#ifdef _DEBUG

#endif // SSYREFERENCETRACE_H

