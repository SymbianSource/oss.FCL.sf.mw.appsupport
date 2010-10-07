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

#include <S32STOR.H> 
#include <S32FILE.H> 

#include "diclog.h"
#include "globaldata.h"

_LIT(KDicLogFileName, "c:\\logs\\diclog");

DicLog::DicLog()
    {

    }

DicLog::~DicLog()
    {

    }

void DicLog::WriteIntLogL(const TDesC& aFileName, const TUid aUid, const TInt32 aValue)
    {
    RFs fs;    
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    TInt err=KErrGeneral;
    err = fs.MkDir(aFileName);
    if(KErrAlreadyExists == err || KErrNone == err)
        {
        CDictionaryStore *dicStore = CDictionaryFileStore::OpenLC(fs, aFileName, TUid::Uid(0x0001));
        
        RDictionaryWriteStream in;
        in.AssignLC(*dicStore,aUid);
        in.WriteInt32L(aValue);
        in.CommitL();  
        dicStore->CommitL();
        
        CleanupStack::PopAndDestroy(&in);
        CleanupStack::PopAndDestroy(dicStore);    
        }
    else
        {
        User::Leave(err);
        }
               
    CleanupStack::PopAndDestroy(&fs);
    }

TInt DicLog::WriteIntLog(const TDesC& aFileName, const TUid aUid, const TInt32 aValue)
    {
    TRAPD(err, DicLog::WriteIntLogL(aFileName, aUid, aValue));
    return err;
    }

TInt DicLog::WriteIntLog(const TUid aUid, const TInt32 aValue)
    {
    TBuf<KIntValue60> fileNmae(KDicLogFileName);
    return DicLog::WriteIntLog(fileNmae, aUid, aValue);
    }

void DicLog::WriteLogL(const TDesC& aFileName, const TUid aUid, const TDesC& aLog)
    {
    RFs fs;   
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    TInt err=KErrGeneral;
    err = fs.MkDir(aFileName);
    if(KErrAlreadyExists == err || KErrNone == err)
        {
        CDictionaryStore *dicStore = CDictionaryFileStore::OpenLC(fs, aFileName, TUid::Uid(0x0001));
        
        RDictionaryWriteStream in;
        in.AssignLC(*dicStore,aUid);
        in.WriteL(aLog);
        in.CommitL();  
        dicStore->CommitL();
        
        CleanupStack::PopAndDestroy(&in);
        CleanupStack::PopAndDestroy(dicStore);    
        }
    else
        {
        User::Leave(err);
        }
               
    CleanupStack::PopAndDestroy(&fs);
    }

TInt DicLog::WriteLog(const TDesC& aFileName, const TUid aUid, const TDesC& aLog)
    {
    TRAPD(err, DicLog::WriteLogL(aFileName, aUid, aLog));
    return err;
    }

TInt DicLog::ReadLog(const TDesC& aFileName, const TUid aUid, TDes& aLog)
    {
    TInt err=KErrGeneral;
    TRAP(err, DicLog::ReadLogL(aFileName, aUid, aLog));      

    return err;
    }

void DicLog::ReadLogL(const TDesC& aFileName, const TUid aUid, TDes& aLog)
    {
    RFs fs;    
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CDictionaryStore *dicStore = CDictionaryFileStore::OpenLC(fs, aFileName, TUid::Uid(0x0001));
    
    RDictionaryReadStream  out;
    out.OpenLC(*dicStore,aUid);   
    out.ReadL(aLog);
    
    CleanupStack::PopAndDestroy(&out);
    CleanupStack::PopAndDestroy(dicStore);    
   
    CleanupStack::PopAndDestroy(&fs);
    }

TInt DicLog::ReadIntLog(const TUid aUid, TInt32& aValue)
    {
    TBuf<KIntValue60> fileNmae(KDicLogFileName);
    return DicLog::ReadIntLog(fileNmae, aUid, aValue);
    }

TInt DicLog::ReadIntLog(const TDesC& aFileName, const TUid aUid, TInt32& aValue)
    {
    TInt err=KErrGeneral;
    TRAP(err, DicLog::ReadIntLogL(aFileName, aUid, aValue));      

    return err;
    }

void DicLog::ReadIntLogL(const TDesC& aFileName, const TUid aUid, TInt32& aValue)
    {
    RFs fs;   
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CDictionaryStore *dicStore = CDictionaryFileStore::OpenLC(fs, aFileName, TUid::Uid(0x0001));
    
    RDictionaryReadStream  out;
    out.OpenLC(*dicStore,aUid);   
    aValue=out.ReadInt32L();
    
    CleanupStack::PopAndDestroy(&out);
    CleanupStack::PopAndDestroy(dicStore);    
   
    CleanupStack::PopAndDestroy(&fs);
    }

TInt DicLog::WriteLog(const TUid aUid, const TDesC& aLog)
    {
    TBuf<KIntValue60> fileNmae(KDicLogFileName);
    return DicLog::WriteLog(fileNmae, aUid, aLog);
    }

TInt DicLog::ReadLog(const TUid aUid, TDes& aLog)
    {
    TBuf<KIntValue60> fileNmae(KDicLogFileName);
    return DicLog::ReadLog(fileNmae, aUid, aLog);
    }

TInt DicLog::RemoveLogFile()
    {
    TBuf<KIntValue60> fileNmae(KDicLogFileName);
    return RemoveLogFile(fileNmae);
    }

TInt DicLog::RemoveLogFile(const TDesC& aFileName)
    {
    TInt err=KErrGeneral;
    TRAP(err, DicLog::RemoveLogFileL(aFileName));      

    return err;
    }

void DicLog::RemoveLogFileL(const TDesC& aFileName)
    {
    RFs fs;          
    User::LeaveIfError(fs.Connect());    
    CleanupClosePushL(fs);   
    
    User::LeaveIfError(fs.Delete(aFileName));
    CleanupStack::PopAndDestroy(&fs); 
    }
