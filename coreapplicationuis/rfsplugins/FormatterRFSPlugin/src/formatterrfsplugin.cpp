/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CFormatterRFSPlugin class
*
*/


// SYSTEM INCLUDE
#include <centralrepository.h>
#include <driveinfo.h>
#include <swi/sisregistrysession.h>
#include <swi/sisregistryentry.h>
#include <swi/sisregistrypackage.h>
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <starterdomaincrkeys.h>
// USER INCLUDE
#include "formatterrfsplugin.h"
#include "formatterrfspluginprivatecrkeys.h"
#include "formatterrfsplugincommon.h"
#include "trace.h"

// LOCAL CONSTANTS
const TUid KCRUidCommsDb = { 0xcccccc00 };

_LIT(KDelimeter, ":");
_LIT(KPathDelimeter, "\\");


// ================= LOCAL FUNCTIONS =======================

// ---------------------------------------------------------------------------
// ExcludeListNameL
// ---------------------------------------------------------------------------
//
static void FileWriteL(RPointerArray<HBufC> &files)
    {
    RFs fileSession;
    RFile file;
    User::LeaveIfError(fileSession.Connect());
    TInt err = file.Open(fileSession,_L("c:\\private\\100059C9\\excludelistcache.txt"),EFileWrite|EFileStreamText);
        
    if ( err != KErrNone )
        {
        RDebug::Print(_L("CFormatterRFSPlugin::ExcludeListNameL , FileWrite : Failed to open the file"));
        return;
        }

    TInt pos = 0;
    file.Seek(ESeekEnd,pos);
    TInt size = files.Count();
    RBuf filenameBuf;

    for ( TInt i=0; i < size; i++)
        {
        HBufC8* fileName = HBufC8::NewLC(files[i]->Size());
        TPtr8 fileNamePtr(fileName->Des());
        fileNamePtr.Copy(*files[i]);

        filenameBuf.Create(fileNamePtr.Length());
        filenameBuf.Copy(fileNamePtr);
        TFileText fileText ;
        fileText.Set(file) ;
        fileText.Seek(ESeekStart);
        fileText.Write(filenameBuf);
        CleanupStack::PopAndDestroy();//Filename
        file.Flush();
        }

    file.Close();
    fileSession.Close();    
    }

static void MergeFilesL()
    {
    
    RFs fileSession;
    RFile excludeFileName;
    
    RFile fileName;
    TInt pos = 0;
    TInt size_of_script( 0 );
    TInt buffer_size( sizeof(TText) );
    TInt number_of_chars;
    
    User::LeaveIfError(fileSession.Connect());
    TInt ret = excludeFileName.Open(fileSession,_L("c:\\private\\100059C9\\excludelist.txt"),EFileRead);

		if(ret != KErrNone)
			{
			RDebug::Print(_L("CFormatterRFSPlugin::ExcludeListNameL , MergeFiles : Failed to open the file"));
			return;
			}
    ret = fileName.Open(fileSession,_L("c:\\private\\100059C9\\excludelistcache.txt"),EFileWrite|EFileStreamText);
    if ( ret != KErrNone)
            {
            excludeFileName.Close();
            RDebug::Print(_L("CFormatterRFSPlugin::ExcludeListNameL , MergeFiles : Failed to open the file"));
            return;
            }
    fileName.Seek(ESeekEnd,pos);
    
    HBufC* buffer = HBufC::NewMaxLC( buffer_size );        
    TPtr8 bufferPtr( (TUint8*)buffer->Ptr(), buffer_size);
    
    TInt err(0);
    err = excludeFileName.Size( size_of_script );
    number_of_chars = size_of_script / sizeof(TText);

    TInt i(0);
    for (i = 0; i < number_of_chars; i++)
       {
       if ( err == KErrNone )
            {
            err = excludeFileName.Read( bufferPtr);                
            }
        fileName.Write(bufferPtr);
        }
    fileName.Flush();
    fileName.Close();
    
    excludeFileName.Close();
    fileSession.Close();
    CleanupStack::PopAndDestroy();//buffer

    }

static HBufC* ExcludeListNameL( TChar aSystemDrive )
    {
    FUNC_LOG;
		
    RDebug::Print(_L("CFormatterRFSPlugin::ExcludeListNameL"));
    
    RFs fileSession;
    RFile file;
    
    _LIT8(KFileName, "c:\\private\\100059C9\\excludelistcache.txt\n");
    TBuf8 <50> fileName;
    fileName.Copy(KFileName);

    User::LeaveIfError(fileSession.Connect());
    
    RDir dir;
    if(dir.Open(fileSession,_L("c:\\private\\100059C9\\"),KEntryAttNormal) != KErrNone)
        {
        User::LeaveIfError(fileSession.MkDir(_L("c:\\private\\100059C9\\")));
        }
    
    TInt rev = file.Replace(fileSession,_L("c:\\private\\100059C9\\excludelistcache.txt"),EFileWrite|EFileStreamText);
    
    RDebug::Print(_L("CFormatterRFSPlugin::ExcludeListNameL, Replace returned %d"),rev);
    
    file.Flush();
    file.Close();
    dir.Close();
    fileSession.Close();

    Swi::RSisRegistrySession session;
    CleanupClosePushL(session);
    User::LeaveIfError(session.Connect());
    
    // Get the installed application UIDs
    RArray<TUid> uids;
    CleanupClosePushL(uids);
    session.InstalledUidsL(uids);
    TInt uidcount = uids.Count(); 
    
    Swi::RSisRegistryEntry entry;
    Swi::RSisRegistryEntry entry2;
    CleanupClosePushL(entry);
    CleanupClosePushL(entry2);
    
    RPointerArray<HBufC> registryFiles;
    RPointerArray<HBufC> augmentedRegistryFiles;
    RPointerArray<HBufC> nonRemovableFiles;
    RPointerArray<HBufC> nonRemovableAugmentedFiles;
    CleanupResetAndDestroyPushL(registryFiles);
    CleanupResetAndDestroyPushL(augmentedRegistryFiles);
    CleanupResetAndDestroyPushL(nonRemovableFiles);
    CleanupResetAndDestroyPushL(nonRemovableAugmentedFiles);
    
     TInt count;
     RPointerArray<Swi::CSisRegistryPackage> augmentationPackages;
     CleanupResetAndDestroyPushL(augmentationPackages);
     for ( TInt iter=0; iter<uidcount; iter++)
         {
         User::LeaveIfError(entry.Open(session,uids[iter]));
         if(EFalse == entry.RemovableL())
             {
             entry.FilesL(nonRemovableFiles);
             entry.RegistryFilesL(registryFiles);
             TInt fileCount = nonRemovableFiles.Count(); 
             for (TInt z=fileCount-1; z>=0;z--)
                 {
                 TPtr firstChar(nonRemovableFiles[z]->Des());
                 if(firstChar.Mid(0,1) == _L("z"))
                     {
                     delete nonRemovableFiles[z];
                     nonRemovableFiles.Remove(z);
                     }
                 }
             // Look for augmentations.
             if(entry.IsAugmentationL())
                 {
                 entry.AugmentationsL(augmentationPackages);
                 count = entry.AugmentationsNumberL();
                 for (TInt i=0; i < count; ++i)
                     {
                     User::LeaveIfError(entry2.OpenL(session,*augmentationPackages[i]));
                     if(EFalse == entry2.RemovableL())
                         {
                         entry2.FilesL(nonRemovableAugmentedFiles);
                         entry2.RegistryFilesL(augmentedRegistryFiles);
                         for (TInt c=0; c<nonRemovableAugmentedFiles.Count();c++)
                               {
                               TPtr firstChar(nonRemovableAugmentedFiles[c]->Des());
                               if(firstChar.Mid(0,1) == _L("z"))
                                   {
                                   delete nonRemovableAugmentedFiles[c];
                                   nonRemovableAugmentedFiles.Remove(c);
                                   }
                               }
											}
                     	entry2.Close();
                     	}
                 }
             }
         entry.Close();
         }
     RDebug::Print(_L("CFormatterRFSPlugin::ExcludeListNameL Writing the file names to the excludelist.txt"));
 
     MergeFilesL();
		 FileWriteL(nonRemovableAugmentedFiles);
		 FileWriteL(augmentedRegistryFiles);
		 FileWriteL(nonRemovableFiles);
 		 FileWriteL(registryFiles);

     TInt pos = 0;
     User::LeaveIfError(fileSession.Connect());
     User::LeaveIfError(file.Open(fileSession,_L("c:\\private\\100059C9\\excludelistcache.txt"),EFileWrite|EFileStreamText));
          
     file.Seek(ESeekEnd,pos);

     TBuf<KMaxFileName> configurationLine ;
     TFileText fileText ;
     fileText.Set(file) ;
     fileText.Seek(ESeekStart);
     configurationLine.Format(_L("c:\\private\\100059C9\\excludelistcache.txt")) ;
     fileText.Write(configurationLine);
     
     file.Flush();
     file.Close();
     fileSession.Close();
    
     
     CleanupStack::PopAndDestroy(9,&session);

     HBufC* buf = HBufC::NewLC( KExcludeListcache().Length() + KExcludeListPathNameLenExt );
     TPtr bufPtr = buf->Des();
     bufPtr.Append( aSystemDrive );
     bufPtr.Append( KDriveDelimiter );
     bufPtr.Append( KExcludeListcache );
     CleanupStack::Pop( buf );
    return buf;
    }

// ---------------------------------------------------------------------------
// CheckFileExist
// ---------------------------------------------------------------------------
//
static TBool CheckFileExist( RFs& aFs, const TDesC& aFullPath )
    {
    FUNC_LOG;

    TBool ret( EFalse );
    RDir dir;
    TEntry entry;
    
    if ( dir.Open( aFs, aFullPath, KEntryAttNormal ) != KErrNone )
        {
        INFO_1( "Application exclude list '%S' does not exist", &aFullPath );
        return EFalse;
        }
    if ( dir.Read( entry ) == KErrNone )
        {
        ret =  ETrue;
        }
    dir.Close();            
    return ret;
    }

// ---------------------------------------------------------------------------
// CheckAppExcludeLists
// ---------------------------------------------------------------------------
//
static TBool CheckAppExcludeListsL( RFs& aFs, TChar aSystemDrive, TChar aRomDrive )
    {
    FUNC_LOG;
    TBool ret( EFalse );
    HBufC* buf = HBufC::NewLC( KMaxPath );
    TPtr bufPtr = buf->Des();
    
    // Create search pattern and dir scanner    
    bufPtr.Append( KApplicationExcludeListPath );
    bufPtr.Append( KApplicationExcludeListFileSearchPattern );
    
    // Check system drive first
    bufPtr[0] = aSystemDrive;
    
    ret = CheckFileExist( aFs, bufPtr );
    
    if( !ret )
        {
        bufPtr[0] = aRomDrive;
        ret = CheckFileExist( aFs, bufPtr );
        }

    CleanupStack::PopAndDestroy( buf );
    
    return ret;
    }

// ---------------------------------------------------------------------------
// ResetRepository
// ---------------------------------------------------------------------------
//
static void ResetRepository( const TUid& aRepositoryUid )
    {
    FUNC_LOG;

    //enforce central repository to re-read setting from .cre file…
    //in next phase try to modify CentRep setting. Because .cre file is locked by CRepositorySession
    //CentRep cannot make update and causes that repository goes to inconsistent state. Inconsistent
    //state ensures that new security settings are readed from drive before any other operation
    CRepository* repository( NULL );
    TRAPD( err, repository = CRepository::NewL( aRepositoryUid ) );
    ERROR_1( err, "Failed to open repository 0x%08x (continue)", aRepositoryUid.iUid );
    if ( err == KErrNone )
        {
        err = repository->Reset();
        ERROR_1( err, "Failed to reset repository 0x%08x (continue)", aRepositoryUid.iUid );
        err = repository->Create( 0, 0 );
        ERROR_1( err, "Failed to create repository 0x%08x (continue)", aRepositoryUid.iUid );
        if ( err == KErrAlreadyExists )
            {
            err = repository->Set( 0, 0 );
            ERROR_1( err, "Failed to set value in repository 0x%08x (continue)", aRepositoryUid.iUid );
            }
        }
    delete repository;
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::NewL
// ---------------------------------------------------------------------------
//
CFormatterRFSPlugin* CFormatterRFSPlugin::NewL( TAny* /*aInitParams*/ )
    {
    FUNC_LOG;

    CFormatterRFSPlugin* self = new ( ELeave ) CFormatterRFSPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CFormatterRFSPlugin::~CFormatterRFSPlugin()
    {
    FUNC_LOG;

    delete iExcludeListName;
    iFs.Close();
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::RestoreFactorySettingsL
// ---------------------------------------------------------------------------
//
void CFormatterRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    FUNC_LOG;
    
    if(iDriveCountInt > 0)
        {
        for( TInt i = EDriveA; i <= EDriveZ; ++i )
            {
            if(iDriveListInt[i])
                {
                RDebug::Print(_L("CFormatterRFSPlugin::RestoreFactorySettingsL Performing format on drive = %d"), i);
                TChar driveChar;
                User::LeaveIfError( iFs.DriveToChar( i, driveChar ) );
                TBuf<3> driveLetter;
                driveLetter.Append( driveChar);
                driveLetter.Append( KDelimeter);
                driveLetter.Append( KPathDelimeter);
                // perform format on the drive
                PerformFormat(driveLetter);
                }
            }
        }
    
    if ( iExcludeListName || iAppExcludeList )
        {
        RDebug::Print(_L("CFormatterRFSPlugin::RestoreFactorySettingsL Performing selective RFS on System Drive"));
    
        TIMESTAMP( "RFS begin" );

        TRAPD( err, DoSecureFormatL() );
               
        if( err == KErrInvalidExcList )
            {
            if( iExcludeListName )
                {
                delete iExcludeListName;
                iExcludeListName = NULL;
                }
            iAppExcludeList = EFalse;
            return;
            }
        else
            {
            User::LeaveIfError( err );
            }
        
        // Create directory structure for Central Repository
        CreateDirectory( KCenRepPrivatePath );
        CreateDirectory( KCenRepPersistsPath );
        CreateDirectory( KCenRepBurPath );

        ResetRepository( KCRUidCommsDb );

        TIMESTAMP( "RFS end" );
        }
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::GetScriptL
// ---------------------------------------------------------------------------
//
void CFormatterRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    FUNC_LOG;

    if ( !iExcludeListName && !iAppExcludeList )
        {
        // Add CenRep UID and key
        aPath.Zero();
        aPath.AppendNumFixedWidthUC( KCRUidFormatterRFSPlugin.iUid, EHex, KHexLength );
        aPath.Append( KScriptKeySeparator );
        aPath.AppendNumFixedWidthUC( KDeepFormatterRFSPlugin, EHex, KHexLength );
        aPath.Append( KScriptUidSeparator );
        INFO_1( "Script = '%S'", &aPath );
        }
    else
        {
        RDebug::Print(_L("Resetting the KStartupFirstBoot value"));
		CRepository* repository = CRepository::NewL(KCRUidStartup);
        CleanupStack::PushL( repository );
        repository->Reset(KStartupFirstBoot);
        CleanupStack::PopAndDestroy( repository );
        }
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::ExecuteCustomCommandL
// ---------------------------------------------------------------------------
//
void CFormatterRFSPlugin::ExecuteCustomCommandL(const TRfsReason /*aType*/, TDesC& /*aCommand*/ )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::ConstructL
// ---------------------------------------------------------------------------
//
void CFormatterRFSPlugin::ConstructL()
    {
    FUNC_LOG;

    User::LeaveIfError( iFs.Connect() );

    User::LeaveIfError(DriveInfo::GetDefaultDrive( DriveInfo::EDefaultSystem, iSystemDrive ) );

    
    // Get the list of drives which have attribute of either internal or removable 
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives(iFs, iDriveListInt, iDriveCountInt, KDriveAttInternal | KDriveAttRemovable ));
    RDebug::Print(_L("CFormatterRFSPlugin::ConstructL The number of internal/removable drives found = %d"), iDriveCountInt);
    for( TInt i = EDriveA; i <= EDriveZ; ++i )
        {
        // Filter in the list of drives that are internal mass memory and are externally mountable i.e. are user accessible
        if (iDriveListInt[i])
            {
            TUint drvStatus( 0 );
            User::LeaveIfError( DriveInfo::GetDriveStatus( iFs, i, drvStatus ) );
            if ( !((drvStatus & DriveInfo::EDriveInternal)&& (drvStatus & DriveInfo::EDriveExternallyMountable )))
                {
                iDriveCountInt--;
                iDriveListInt[ i ] = NULL;
                RDebug::Print(_L("CFormatterRFSPlugin::ConstructL() drive excluded = %d"), i);
                }
            }
        }
    RDebug::Print(_L("CFormatterRFSPlugin::ConstructL The number of internal/removable drives left after filtering = %d"), iDriveCountInt);
    
    iExcludeListName = ExcludeListNameL( iSystemDrive );

    RFile file;
    TInt err = file.Open( iFs, *iExcludeListName, EFileRead );
    if ( err == KErrNone )
        {
        TInt fileSize( 0 );
        err = file.Size( fileSize );
        if ( err != KErrNone || fileSize == 0 )
            {
            // Empty file
            INFO_1( "Exclude list '%S' is empty", iExcludeListName );
            delete iExcludeListName;
            iExcludeListName = NULL;
            }
        }
    else
        {
        // File not found
        INFO_1( "Exclude list '%S' does not exist", iExcludeListName );
        delete iExcludeListName;
        iExcludeListName = NULL;
        }
    file.Close();
    
    User::LeaveIfError( DriveInfo::GetDefaultDrive(DriveInfo::EDefaultRom, iDefaultRomDrive ) );
    
    iAppExcludeList = CheckAppExcludeListsL( iFs, iSystemDrive, iDefaultRomDrive );
    
    if( iAppExcludeList && !iExcludeListName )
        {
        iExcludeListName = KEmptyParameter().AllocL();
        }  
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::DoSecureFormatL
// ---------------------------------------------------------------------------
//
void CFormatterRFSPlugin::DoSecureFormatL() const
    {
    FUNC_LOG;

    TIMESTAMP( "Secure format begin" );
	
	// check the exclude list file is present
	if( NULL == iExcludeListName )
	{
	TInt error = KErrNotFound;
	ERROR( error, "Exclude list file is not found. Secure formatter failed" );
	User::LeaveIfError( error );
	}

    HBufC* arg = HBufC::NewLC(iExcludeListName->Length() + KSecureFormatterArgLenExt );
    TPtr argPtr = arg->Des();
    argPtr.Format(KSecureFormatterArgFmt, iExcludeListName,static_cast<TUint>( iSystemDrive ), KDriveDelimiter, KPathDelimiter );

    RProcess process;
    CleanupClosePushL( process );
    TInt err = process.Create( KSecureFormatter, *arg );
    ERROR( err, "Failed to create process" );
    User::LeaveIfError( err );

    TRequestStatus status;
    process.Logon( status );
    process.Resume();
    User::WaitForRequest( status );
    ERROR( status.Int(), "secure formatter failed" );
    // Continue

    CleanupStack::PopAndDestroy( &process );
    CleanupStack::PopAndDestroy( arg );

    TIMESTAMP( "Secure format end" );
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::CreateDirectory
// ---------------------------------------------------------------------------
//
void CFormatterRFSPlugin::CreateDirectory( const TDesC& aDirName )
    {
    FUNC_LOG;

    TBuf<KMaxPath> path;
    path.Append( iSystemDrive );
    path.Append( KDriveDelimiter );
    path.Append( aDirName );
    iFs.MkDirAll( path ); // Ignore errors
    }

// ---------------------------------------------------------------------------
// CFormatterRFSPlugin::ExecuteFormatCommand
// ---------------------------------------------------------------------------
//
TInt CFormatterRFSPlugin::PerformFormat(const TDesC& aDriveLetter)
    {
    RDebug::Print(_L("CRfsCommand::ExecuteFormatCommand()"));
    RFormat formatter;
    TInt ret( KErrNone );
    
    TInt count(0);
    // Open formatter
    ret = formatter.Open(iFs, aDriveLetter, EHighDensity | EFullFormat, count );
    RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): Formatter open returned : %d"), ret);    
    if ( ret == KErrInUse ) // Locked files in drive
        {
        TInt driveNumber = iMassStorageDrive; 

        // Shouldn't assume it's the internal mass storage drive is E:, check the parameter 
        ret = iFs.CharToDrive( aDriveLetter[ 0 ], driveNumber ); 
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): drive numer %d"), driveNumber);   

        // Get the file system name from the drive before dismounting it 
        TFullName name; 
        ret = iFs.FileSystemName(name, driveNumber); 
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): FileSystemName returned : %d, name %S"), ret, &name); 

        // Force a dismount
        TRequestStatus dummyStat;
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): Setting notifier"));
        iFs.NotifyDismount( driveNumber, dummyStat, EFsDismountForceDismount );
        iFs.DismountFileSystem( name, driveNumber );
        
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): Waiting for request complete"));
        User::WaitForRequest(dummyStat);
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): Request completed"));

        // Now mount the file system again
        ret = iFs.MountFileSystem(name, driveNumber);
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): MountFileSystem returned : %d"), ret);

        // Try to open formatter second time
        ret = formatter.Open(iFs, aDriveLetter, EHighDensity | EFullFormat, count );
        RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): Second try: Formatter open returned : %d"), ret);
        }

    if ( ret == KErrNone )
        {
        while ( count > 0 )
            {
            ret = formatter.Next( count );
            RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): Format status: %d, tracks remaining %d"), ret, count);
            }
        }
            
    formatter.Close();
 
    RDebug::Print(_L("CFormatterRFSPlugin::ExecuteFormatCommand(): End: returns %d"), ret);
    return ret;
    }
