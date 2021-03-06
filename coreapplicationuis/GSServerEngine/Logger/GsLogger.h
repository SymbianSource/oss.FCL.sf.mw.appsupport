/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
*       Macro definition file for logging.
*
*
*/



#ifndef GSLOGGER_H
#define GSLOGGER_H

#include "GsLoggingConfiguration.h"
#include <e32std.h>
#include <e32svr.h>
#include <flogger.h>

_LIT( KLogsDir,                  "Gs");
_LIT( KLogFileName,              "Gs.log");

/*
 We have 3 possible logging methods:

 set GS_LOGGING_METHOD to 0 to get no logging at all
 set GS_LOGGING_METHOD to 1 to get logging via RDebug::Print
 set GS_LOGGING_METHOD to 2 to get logging to log file

 Switching on/off is done in the Configuration file (GsLoggingConfiguration.h)
 */

#if ( defined (_GSLOGGING) )
    #if ( defined (_GS_LOGGING_TO_FILE))
        #define GS_LOGGING_METHOD 2
    #else
        #define GS_LOGGING_METHOD 1
    #endif
#else
    #define GS_LOGGING_METHOD 0
#endif


#if (GS_LOGGING_METHOD==0)
    #define __GSLOGSTRING(C)
    #define __GSLOGSTRING1(C, X)
    #define __GSLOGSTRING2(C, X, Y)
    #define __GSLOGSTRING3(C, X, Y, Z)
    #define __GSLOGSTRING4(C, X, Y, Z, A)
#endif
#if (GS_LOGGING_METHOD==1)
    #define __GSLOGSTRING(C)                RDebug::Print(_L(C));
    #define __GSLOGSTRING1(C, X)            RDebug::Print(_L(C),X);
    #define __GSLOGSTRING2(C, X, Y)         RDebug::Print(_L(C),X, Y);
    #define __GSLOGSTRING3(C, X, Y, Z)      RDebug::Print(_L(C),X, Y, Z);
    #define __GSLOGSTRING4(C, X, Y, Z, A)   RDebug::Print(_L(C),X, Y, Z, A);
#endif
#if (GS_LOGGING_METHOD==2)
    #define __GSLOGSTRING(C)                {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, tempLogDes());}
    #define __GSLOGSTRING1(C, X)            {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X);}
    #define __GSLOGSTRING2(C, X, Y)         {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y);}
    #define __GSLOGSTRING3(C, X, Y, Z)      {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y,Z);}
    #define __GSLOGSTRING4(C, X, Y, Z, A)   {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y,Z,A);}
#endif

#endif // GSLOGGER_H
