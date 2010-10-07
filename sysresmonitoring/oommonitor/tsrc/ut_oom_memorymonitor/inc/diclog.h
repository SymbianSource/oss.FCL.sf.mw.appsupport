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
* Description: This file contains STIFUnit implementation.
*
*/

#ifndef DICLOG_H_
#define DICLOG_H_

#include <e32base.h>
#include <f32file.h> 

#define DICLOGMAXLENGTH 10;

class DicLog
    {
public:
    DicLog();
    virtual ~DicLog();
    static TInt WriteIntLog(const TDesC& aFileName, const TUid aUid, const TInt32 aValue);
    static TInt WriteIntLog(const TUid aUid, const TInt32 aValue);
    
    static TInt WriteLog(const TDesC& aFileName, const TUid aUid, const TDesC& aLog);
    static TInt WriteLog(const TUid aUid, const TDesC& aLog);
    
    static TInt ReadIntLog(const TDesC& aFileName, const TUid aUid, TInt32& aValue);
    static TInt ReadIntLog(const TUid aUid, TInt32& aValue);
    
    static TInt ReadLog(const TDesC& aFileName, const TUid aUid, TDes& aLog);    
    static TInt ReadLog(const TUid aUid, TDes& aLog);
    
    static TInt RemoveLogFile();    
    static TInt RemoveLogFile(const TDesC& aFileName);    
    
private:   
    static void WriteIntLogL(const TDesC& aFileName, const TUid aUid, const TInt32 aValue);
    static void WriteLogL(const TDesC& aFileName, const TUid aUid, const TDesC& aLog);
    static void ReadIntLogL(const TDesC& aFileName, const TUid aUid, TInt32& aValue);
    static void ReadLogL(const TDesC& aFileName, const TUid aUid, TDes& aLog);
    static void RemoveLogFileL(const TDesC& aFileName);
    };

#endif /* DICLOG_H_ */
