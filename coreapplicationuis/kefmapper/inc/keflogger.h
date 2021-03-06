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
*       Macro definition file for logging.
*
*
*/



#ifndef KEFLOGGER_H
#define KEFLOGGER_H

#include "kefloggingconfiguration.h"
#include <e32std.h>
#include <flogger.h>
#include <e32svr.h>

_LIT( KLogsDir,                  "kef");
_LIT( KLogFileName,              "kef.log");

/*
 We have 3 possible logging methods:

 set KEF_LOGGING_METHOD to 0 to get no logging at all
 set KEF_LOGGING_METHOD to 1 to get logging via RDebug::Print
 set KEF_LOGGING_METHOD to 2 to get logging to log file

 Switching on/off is done in the Configuration file (KefLoggingConfiguration.h)
 */

#if ( defined (_KEF_LOGGING) )
    #if ( defined (_KEF_LOGGING_TO_FILE))
        #define KEF_LOGGING_METHOD 2 
    #else
        #define KEF_LOGGING_METHOD 1
    #endif
#else
    #define KEF_LOGGING_METHOD 0
#endif


#if (KEF_LOGGING_METHOD==0)
    #define __KEFLOGSTRING(C)
    #define __KEFLOGSTRING1(C, X)
    #define __KEFLOGSTRING2(C, X, Y)
    #define __KEFLOGSTRING3(C, X, Y, Z)
#endif
#if (KEF_LOGGING_METHOD==1)
    #define __KEFLOGSTRING(C)            RDebug::Print(_L(C));
    #define __KEFLOGSTRING1(C, X)        RDebug::Print(_L(C),X);
    #define __KEFLOGSTRING2(C, X, Y)     RDebug::Print(_L(C),X, Y);
    #define __KEFLOGSTRING3(C, X, Y, Z)  RDebug::Print(_L(C),X, Y, Z);
#endif
#if (KEF_LOGGING_METHOD==2)
    #define __KEFLOGSTRING(C)            {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, tempLogDes());}
    #define __KEFLOGSTRING1(C, X)        {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X);}
    #define __KEFLOGSTRING2(C, X, Y)     {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y);}
    #define __KEFLOGSTRING3(C, X, Y, Z)  {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y,Z);}
#endif

#endif // KEFLOGGER_H

// End of File
