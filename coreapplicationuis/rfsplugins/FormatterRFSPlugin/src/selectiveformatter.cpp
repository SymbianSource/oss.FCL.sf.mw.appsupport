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
* Description:  Implementation of CSelectiveFormatter class
*
*/


#include <f32file.h>

#include "dirstackentry.h"
#include "selectiveformatter.h"
#include "formatterrfsplugincommon.h"
#include "rfsfileman.h"
#include "trace.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CSelectiveFormatter::NewL
// ---------------------------------------------------------------------------
//
CSelectiveFormatter* CSelectiveFormatter::NewLC( const TDesC& aExcludeListFile )
    {
    CSelectiveFormatter* self = new ( ELeave ) CSelectiveFormatter;
    CleanupStack::PushL( self );
    self->ConstructL( aExcludeListFile );
    return self;
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::~CSelectiveFormatter
// ---------------------------------------------------------------------------
//
CSelectiveFormatter::~CSelectiveFormatter()
    {
    FUNC_LOG;

    delete iFileMan;

    iExcludeList.ResetAndDestroy();
    iExcludeList.Close();

    iDirStack.ResetAndDestroy();
    iDirStack.Close();

    iFs.Close();
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::FindAndDeleteL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::FindAndDeleteL( const TDesC& aFullPath )
    {
    FUNC_LOG;

    iDirStack.ResetAndDestroy();
    if ( PreserveType( aFullPath ) != CExcludeListEntry::EPreserveAll )
        {
        HBufC* buffer = HBufC::NewLC( KMaxPath );
        TPtr ptr = buffer->Des();
        CDirStackEntry::PushDirEntryL( iDirStack, aFullPath, iFs ); // Setup root dir
        while ( iDirStack.Count() > 0 )
            {
            CDirStackEntry& stackEntry = *( iDirStack[ iDirStack.Count() - 1 ] );
            const TEntry* dirEntry = stackEntry.GetNextEntry( ptr );
            if ( dirEntry )
                {
                HandleDirEntryL( ptr, *dirEntry );
                }
            else
                {
                // Dir has been processed. Pop it and continue.
                CDirStackEntry::PopAndDestroyDirEntry( iDirStack );
                }
            }
        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        INFO_1( "Keep dir: '%S'", &aFullPath );
        }
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::CSelectiveFormatter
// ---------------------------------------------------------------------------
//
CSelectiveFormatter::CSelectiveFormatter()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::ConstructL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::ConstructL( const TDesC& aExcludeListFile )
    {
    FUNC_LOG;

    User::LeaveIfError( iFs.Connect() );
    iFileMan = CRfsFileMan::NewL( iFs );

    // Handle excludelist.txt
    if ( aExcludeListFile.Match( KEmptyParameter ) == KErrNotFound )
        {
        ReadExcludeListL( aExcludeListFile );
        }
    // Handle app specific files
    HandleAppExcludeListsL();
    
    if( !iValidExcludeListFound )
        {
        User::Leave( KErrInvalidExcList );
        }
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::ReadExcludeListL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::ReadExcludeListL( const TDesC& aExcludeListFile )
    {
    FUNC_LOG;

    RFile file;
    TInt err = file.Open( iFs, aExcludeListFile, EFileRead );
    ERROR_1( err, "Failed to open '%S'", &aExcludeListFile );
    if ( err == KErrNone )
        {
        // exclude list successfully opened -> Selective format.
        INFO_1( "Using exclude list '%S'", &aExcludeListFile );
        CleanupClosePushL( file );

        // Add exclude list file itself to exclude list
        CExcludeListEntry* entry = CExcludeListEntry::NewLC( aExcludeListFile ); // take ownership
        iExcludeList.AppendL( entry );
        CleanupStack::Pop( entry );

        TFileText text;
        text.Set( file );
        TFileName buffer;
        text.Read( buffer ); //carry on if read fails
        while ( buffer.Length() > 0 )
            {
            entry = CExcludeListEntry::NewL( buffer );
            err = iExcludeList.InsertInOrder( entry, CExcludeListEntry::Compare ); // take ownership
            if( err != KErrNone )
                {
                delete entry; // delete entry if ownership not transferred

                if( err != KErrAlreadyExists )
                    {
                    User::Leave( err );
                    }
                }            

            text.Read( buffer ); //carry on if read fails
            iValidExcludeListFound = ETrue;
            }

        CleanupStack::PopAndDestroy( &file );
        }
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::PreserveType
// ---------------------------------------------------------------------------
//
CExcludeListEntry::TPreserveType
CSelectiveFormatter::PreserveType( const TDesC& aFullPath ) const
    {
    FUNC_LOG;

    CExcludeListEntry::TPreserveType ret = CExcludeListEntry::EPreserveNone;
    CExcludeListEntry::TMatchEntry entry( aFullPath );
    TInt pos = iExcludeList.Find( entry, CExcludeListEntry::CompareMatch );
    if ( pos >= 0 && pos < iExcludeList.Count() )
        {
        TPtrC matchPath = iExcludeList[pos]->Path();
        ret = iExcludeList[pos]->Type( entry );
        INFO_3( "'%S' matches '%S' type %d", &aFullPath, &matchPath, ret );
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleDirEntryL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleDirEntryL(
    const TDesC& aFullPath,
    const TEntry& aEntry )
    {
    FUNC_LOG;

    CExcludeListEntry::TPreserveType preserve = PreserveType( aFullPath );

    if ( preserve == CExcludeListEntry::EPreserveNone )
        {
        TRAPD_ERR( err, iFileMan->DeleteL( aFullPath, aEntry ) );
        ERROR_1( err, "Failed to delete '%S'", &aFullPath );
        }
    else if ( aEntry.IsDir() && preserve != CExcludeListEntry::EPreserveAll )
        {
        INFO_1( "Keep matches from: '%S'", &aFullPath );
        CDirStackEntry::PushDirEntryL( iDirStack, aFullPath, iFs );
        }
    else
        {
        INFO_1( "Keep '%S'", &aFullPath );
        }
    }

// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleAppExcludeListsL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleAppExcludeListsL()
    {
    FUNC_LOG;
    HBufC* buf = HBufC::NewLC( KMaxPath );
    TPtr bufPtr = buf->Des();
    iSystemDrive = iFs.GetSystemDriveChar();
    
    // Search from rom and system drives.
    bufPtr.Append( KApplicationExcludeListPath );
    bufPtr.Append( KApplicationExcludeListFileSearchPattern );
    HandleAppExcludeListsOnDriveL( bufPtr, KDefaultRom()[0] );
    bufPtr.Zero();
    bufPtr.Append( KApplicationExcludeListPath );
    bufPtr.Append( KApplicationExcludeListFileSearchPattern );
    HandleAppExcludeListsOnDriveL( bufPtr, iSystemDrive );

    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------------------------
// CSelectiveFormatter::HandleAppExcludeListsOnDriveL
// ---------------------------------------------------------------------------
//
void CSelectiveFormatter::HandleAppExcludeListsOnDriveL( TPtr aBuf, TChar aDrive )
    {
    FUNC_LOG;
    CDir* dir = NULL;
    
    aBuf[0] = aDrive;
    TInt err = iFs.GetDir( aBuf, KEntryAttNormal, ESortNone, dir );

    ERROR_1( err, "RFs::GetDir error while reading drive %c", (TUint)aDrive );
    
    if( err == KErrNone )
        {
        CleanupStack::PushL( dir );
        // Remove search pattern.
        aBuf.Delete( KApplicationExcludeListPath().Length(), 
            KApplicationExcludeListFileSearchPattern().Length() );
    
        for( TInt i = 0; i < dir->Count(); i++ )
            {
            TBool upgradeFound( EFalse );
            const TEntry& entry = ( *dir )[i];
            aBuf.Append( entry.iName );
    
            if( aDrive == KDefaultRom()[0] )
                {
                // Change to system drive for upgrade check
                aBuf[0] = iSystemDrive;
                RFile file;
                err = file.Open( iFs, aBuf, EFileRead );
                if( err == KErrNone )
                    {
                    // Upgrade found - rom file not used
                    upgradeFound = ETrue;
                    file.Close();
                    }
                else
                    {
                    // Upgrade not found - use rom file                
                    aBuf[0] = KDefaultRom()[0];
                    }
                }

            if( !upgradeFound )
                {
                RFile file;
                err = file.Open( iFs, aBuf, EFileRead );
                if ( err == KErrNone )
                    {
                    TInt fileSize( 0 );
                    err = file.Size( fileSize );
                    file.Close();
                    INFO_3( "Application exclude list '%S', err %d, size %d ", &aBuf, err, fileSize );
                    if ( err != KErrNone || fileSize == 0 )
                        {
                        // Empty file
                        INFO_1( "Application exclude list '%S' is empty", &aBuf );
                        }
                    else
                        {
                        ReadExcludeListL( aBuf );                    
                        }
                    }
                }
            // Remove file name.
            aBuf.Delete( KApplicationExcludeListPath().Length(), entry.iName.Length() );
            }
        CleanupStack::PopAndDestroy( dir );
        dir = NULL;
        }
    }
