// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apgcli.h
//

#ifndef __APGCLI_H__
#define __APGCLI_H__

#include <e32base.h>
#include <apmstd.h>
#include <apmrec.h>
#include <apaid.h>
#include <badesca.h>
#include <f32file.h>

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include "apgupdate.h"
//#include<usif/scr/scr.h>
#include<usif/scr/appregentries.h>
#endif

// classes referenced
class RFile;
class MArrayFiller;
class CApaCommandLine;
class CApaMaskedBitmap;
class CApaLsSessionExtension;
class CApaRegistrationResourceFileWriter;
class CApaLocalisableResourceFileWriter;
class CApaAppServiceInfoArray;

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
/** @internalComponent
*/
IMPORT_C TUint MinApplicationStackSize();

#endif //SYMBIAN_ENABLE_SPLIT_HEADERS


/**
This class holds the MIME-type recognition results of a recognized directory.
@publishedAll
@released
*/
class CDataRecognitionResultArray : public CBase
	{
public: 
	IMPORT_C CDataRecognitionResultArray();
	IMPORT_C ~CDataRecognitionResultArray();
	IMPORT_C const TFileName& Path() const;
	IMPORT_C TUint Count() const;
	IMPORT_C void GetDataRecognitionResultL(TDataRecognitionResult& aResult, const TUint aIndex) const;
	IMPORT_C void GetFileNameL(TFileName& aFileName, const TUint aIndex) const;
public: 
	TUint InternalizeL(const CBufFlat& aBuffer);
	void SetPath(const TFileName& aPath);
private:
	NONSHARABLE_CLASS(CItem);
	TFileName iPath;
	RPointerArray<CItem> iItems;
	};

/** A session with the application architecture server.

The server provides access to a cached list of the applications on the device. 

To use this class, clients first need to connect to the server. 

Then, one of GetAllApps(), GetEmbeddableApps() or GetFilteredApps() needs to be called. 
These functions do some initialisation of server data in preparation for returning the 
application list (which is owned by the server).

Then, GetNextApp() is called in a loop. The first time around, it populates the session's 
application list and retrieves the first application in the list. Further calls to GetNextApp() 
retrieve the remaining applications in the list, until RApaLsSession::ENoMoreAppsInList is 
returned.

The session should be closed when it is no longer needed, before it goes out of scope.

Various functions in RApaLsSession need to access the application list, and may return 
RApaLsSession::EAppListInvalid to indicate that the function was called before the initial 
population of the list is complete. If this occurs, you could try again after a short delay.

This class provides many other functions, for instance launching applications, querying 
application-specific information, recognising data.

@publishedAll
@released */
class RApaLsSession : public RSessionBase
	{
public:
	enum TApaLsErrorCode {
		/** A possible return value from GetNextApp(), to indicate that there are no more 
		applications in the list. */
		ENoMoreAppsInList = 1,
		/** Indicates that an RApaLsSession member function was called before the session's 
		application list is fully populated. */
		EAppListInvalid
		};
	
	/** Defines the way an application is launched. 
	This is no longer in use. Has no effect! */
	enum TLaunchType {
		ELaunchNewApp = 1,
		ESwitchFiles
		};
public: // Session management
	IMPORT_C RApaLsSession();
	IMPORT_C TInt Connect();
	IMPORT_C void Close(); //lint !e1511 Member hides non-virtual member
	IMPORT_C TVersion Version() const;
	IMPORT_C void RegisterListPopulationCompleteObserver(TRequestStatus& aStatus) const;
	IMPORT_C TInt CancelListPopulationCompleteObserver() const;
public:	// Utilities
	IMPORT_C TInt IsProgram(const TDesC& aFileName, TBool& aProgram) const;	
	IMPORT_C TInt MatchesSecurityPolicy(TBool& aMatches, TUid aAppUid, const TSecurityPolicy& aSecurityPolicy) const;
public: // Application list initialization
	// Set up list of all apps
	IMPORT_C TInt GetAllApps() const;
	IMPORT_C TInt GetAllApps(TInt aScreenMode) const;
	IMPORT_C TInt GetEmbeddableApps() const;
	IMPORT_C TInt GetEmbeddableApps(TInt aScreenMode) const;
	// Set up filtered list of apps
	IMPORT_C TInt GetFilteredApps(const TApaEmbeddabilityFilter& aFilter) const;
	IMPORT_C TInt GetFilteredApps(const TApaEmbeddabilityFilter& aFilter, TInt aScreenMode) const;
	IMPORT_C TInt GetFilteredApps(TUint aCapabilityAttributeMask, TUint aCapabilityAttributeValue) const;
	IMPORT_C TInt GetFilteredApps(TUint aCapabilityAttributeMask, TUint aCapabilityAttributeValue, TInt aScreenMode) const;
	// Set up list of server "service" apps
	IMPORT_C TInt GetServerApps(TUid aServiceUid) const;
	IMPORT_C TInt GetServerApps(TUid aServiceUid, TInt aScreenMode) const;
public: // Application list access	
	// Get application count
	IMPORT_C TInt AppCount(TInt& aCount) const;
	IMPORT_C TInt EmbeddableAppCount(TInt& aCount) const;
	// Get next app in list
	IMPORT_C TInt GetNextApp(TApaAppInfo& aInfo) const; // call repeatedly, returns err (inc ENoMoreAppsInList,EAppListInvalid)
	IMPORT_C TInt GetNextApp(TApaAppInfo& aInfo,TInt aScreenMode) const; // call repeatedly, returns err (inc ENoMoreAppsInList,EAppListInvalid)
public: // Application meta-data
	IMPORT_C TInt GetAppInfo(TApaAppInfo& aInfo, TUid aAppUid) const;
	IMPORT_C TInt GetAppViews(CApaAppViewArray& aAppViews, TUid aAppUid) const;
	
	IMPORT_C TInt SetAppShortCaption(const TDesC& aShortCaption, TLanguage aLanguage, TUid aAppUid);
	IMPORT_C TInt ApplicationLanguage(TUid aAppUid, TLanguage& aLanguage) const;
	IMPORT_C TInt GetAppCapability(TDes8& aCapabilityBuf,TUid aAppUid) const;
	IMPORT_C TInt GetAppOwnedFiles(CDesCArray& aAppOwnedFiles, TUid aAppUid) const;
	IMPORT_C TInt GetDefaultScreenNumber(TInt& aDefaultScreenNumber, TUid aAppUid) const;
	// Application icons
	IMPORT_C TInt GetAppIcon(TUid aAppUid, TInt aSize, CApaMaskedBitmap& aAppBitmap) const;
	IMPORT_C TInt GetAppIconSizes(TUid aAppUid, CArrayFixFlat<TSize>& aArrayToFill) const;
	IMPORT_C TInt GetAppIcon(TUid aAppUid, TSize aSize, CApaMaskedBitmap& aAppBitmap) const;
	IMPORT_C TInt GetAppIcon(TUid aAppUid, HBufC*& aFullFileName) const;
	IMPORT_C TInt GetAppIcon(TUid aAppUid, RFile& aFile) const;
	IMPORT_C TInt NumberOfOwnDefinedIcons(TUid aAppUid, TInt& aCount) const;
	// View icons
	IMPORT_C TInt GetAppViewIcon(TUid aAppUid, TUid aViewUid, const TSize& aSize, CApaMaskedBitmap& aViewBitmap) const;
	IMPORT_C TInt GetAppViewIcon(TUid aAppUid, TUid aViewUid, HBufC*& aFullFileName) const;
public: // Application start
	IMPORT_C TInt StartApp(const CApaCommandLine& aCommandLine); // This is only recommended for non document based apps
	IMPORT_C TInt StartApp(const CApaCommandLine& aCommandLine, TThreadId& aThreadId);
	IMPORT_C TInt StartApp(const CApaCommandLine& aCommandLine, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous);
	
	IMPORT_C TInt StartDocument(const TDesC& aDocFileName, TThreadId& aThreadId, TLaunchType aUnused = ELaunchNewApp);
	IMPORT_C TInt StartDocument(const TDesC& aDocFileName, const TDataType& aDataType, TThreadId& aThreadId, TLaunchType aUnused = ELaunchNewApp);
	IMPORT_C TInt StartDocument(const TDesC& aDocFileName, TUid aAppUid, TThreadId& aThreadId, TLaunchType aUnused = ELaunchNewApp);
	IMPORT_C TInt StartDocument(RFile& aDocFile, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous = NULL);
	IMPORT_C TInt StartDocument(RFile& aDocFile, const TDataType& aDataType, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous = NULL);
	IMPORT_C TInt StartDocument(RFile& aDocFile, TUid aAppUid, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous = NULL);
	IMPORT_C TInt CreateDocument(const TDesC& aDocFileName, TUid aAppUid, TThreadId& aThreadId, TLaunchType aUnused = ELaunchNewApp);
public: // Data MIME-type recognizion
	// Minimum accepted confidence by which a data may be considered recognized
	IMPORT_C TInt SetAcceptedConfidence(TInt aConfidence);
	IMPORT_C TInt GetAcceptedConfidence(TInt& aConfidence) const;
	// Get a complete list of all known (recognizable) MIME types
	IMPORT_C TInt GetSupportedDataTypesL(CDataTypeArray& aDataTypes) const;
	// Recognize data
	IMPORT_C TInt RecognizeData(const TDesC8& aBuffer, TDataRecognitionResult& aDataType) const;
	IMPORT_C TInt RecognizeData(const TDesC& aName, const TDesC8& aBuffer, TDataRecognitionResult& aDataType) const;
	IMPORT_C TInt RecognizeData(const RFile& aFile, TDataRecognitionResult& aDataType) const;
	IMPORT_C TInt RecognizeSpecificData(const TDesC& aName, const TDesC8& aBuffer, const TDataType& aDataType, TBool& aResult) const;
	IMPORT_C TInt RecognizeSpecificData(const RFile& aFile, const TDataType& aDataType, TBool& aResult) const;
	// Recognize files
	IMPORT_C TInt RecognizeFilesL(const TDesC& aPath, CDataRecognitionResultArray& aResult) const;
	IMPORT_C void RecognizeFilesL(const TDesC& aPath, CDataRecognitionResultArray& aResult, TRequestStatus& aStatus);
	IMPORT_C TInt RecognizeFilesL(const TDesC& aPath, const TDesC8& aDataType, CDataRecognitionResultArray& aResult) const;
	IMPORT_C void RecognizeFilesL(const TDesC& aPath, const TDesC8& aDataType, CDataRecognitionResultArray& aResult, TRequestStatus& aStatus);
	IMPORT_C void CancelRecognizeFiles();
	// Get application registered as capable of handling specified MIME data type
	IMPORT_C TInt AppForDataType(const TDataType& aDataType, TUid& aAppUid) const;
	IMPORT_C TInt AppForDocument(const TDesC& aFileName, TUid& aAppUid, TDataType& aDataType) const;
	IMPORT_C TInt AppForDocument(const RFile& aFile, TUid& aAppUid, TDataType& aDataType) const;
	// Register an applicatoin as capable of handling a specified MIME data type
	IMPORT_C TInt InsertDataMapping(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid);
	IMPORT_C TInt InsertDataMappingIfHigher(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid, TBool& aInserted);
	IMPORT_C TInt DeleteDataMapping(const TDataType& aDataType);
	// Amount of data used for recognition
	IMPORT_C TInt SetMaxDataBufSize(TInt aBufSize);
	IMPORT_C TInt GetMaxDataBufSize(TInt& aBufSize) const;
	IMPORT_C TInt GetPreferredBufSize(TInt& aPreferredBufSize) const;
public: // Support for service-applications
	IMPORT_C CApaAppServiceInfoArray* GetAppServicesLC(TUid aAppUid) const;
	IMPORT_C CApaAppServiceInfoArray* GetServiceImplementationsLC(TUid aServiceUid) const;
	IMPORT_C CApaAppServiceInfoArray* GetServiceImplementationsLC(TUid aServiceUid, const TDataType& aDataType) const;
	IMPORT_C void GetAppServicesL(TUid aAppUid, CArrayFixFlat<TUid>& aServiceUids) const;
	IMPORT_C CApaAppServiceInfoArray* GetAppServiceOpaqueDataLC(TUid aAppUid, TUid aServiceUid) const;
	IMPORT_C TInt AppForDataTypeAndService(const TDataType& aDataType, TUid aServiceUid, TUid& aAppUid) const;
	IMPORT_C TInt AppForDocumentAndService(const TDesC& aFileName, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType) const;
	IMPORT_C TInt AppForDocumentAndService(const RFile& aFile, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType) const;
public:	// for internal use only
	IMPORT_C TInt InsertDataMapping(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid, TUid aServiceUid);
	IMPORT_C TInt DeleteDataMapping(const TDataType& aDataType, TUid aServiceUid);
	IMPORT_C void NotifyOnDataMappingChange(TRequestStatus& aRequestStatus);
	IMPORT_C void CancelNotifyOnDataMappingChange();
	IMPORT_C TInt GetAppByDataType(const TDataType& aDataType, TUid aServiceUid, TUid& aAppUid) const;
public:	// for internal use only
	IMPORT_C static void SetFsSessionL(RFs& aFsSession);
	IMPORT_C static void ClearFsSession();
	IMPORT_C static RFs* FsSession();
	IMPORT_C void SetNotify(TBool aCompleteImmediatelyIfNoScanImpendingOrInProgress, TRequestStatus& aStatus);
	IMPORT_C void CancelNotify();
public: // for internal use only
	// Support for non-native applications	
	IMPORT_C void RegisterNonNativeApplicationTypeL(TUid aApplicationType, const TDesC& aNativeExecutable);
	IMPORT_C void DeregisterNonNativeApplicationTypeL(TUid aApplicationType);
	IMPORT_C void PrepareNonNativeApplicationsUpdatesL();
	IMPORT_C void RegisterNonNativeApplicationL(TUid aApplicationType, const TDriveUnit& aDrive, CApaRegistrationResourceFileWriter& aRegistrationResourceFile, CApaLocalisableResourceFileWriter* aLocalisableResourceFile, const RFile* aIconFile);
	IMPORT_C void DeregisterNonNativeApplicationL(TUid aApplication);
	IMPORT_C void CommitNonNativeApplicationsUpdatesL();
	IMPORT_C TInt RollbackNonNativeApplicationsUpdates();
	IMPORT_C TInt GetAppType(TUid& aTypeUid, TUid aAppUid) const;
	IMPORT_C TInt ForceRegistration(const RPointerArray<TDesC>& aRegFiles);
	
private: // Reserved for future use
	IMPORT_C virtual void RApaLsSession_Reserved1();
	IMPORT_C virtual void RApaLsSession_Reserved2();
	
public:
	/**
	@publishedPartner
	@released
	*/
	IMPORT_C void ForceCommitNonNativeApplicationsUpdatesL(); 

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	IMPORT_C TInt UpdateAppListL(RArray<TApaAppUpdateInfo>& aAppUpdateInfo);
    IMPORT_C TInt ForceRegistration(const RPointerArray<Usif::CApplicationRegistrationData>& aForceRegAppsInfo);
    IMPORT_C TInt UpdatedAppsInfoL(RArray<TApaAppUpdateInfo>& aUpdatedApps);
#endif
	
private:
	void DoGetAppOwnedFilesL(CDesCArray& aArrayToFill, TUid aAppUid) const;
	void DoGetAppViewsL(CApaAppViewArray& aArrayToFill, TUid aAppUid) const;
	void DoGetAppIconSizesL(TUid aAppUid, CArrayFixFlat<TSize>& aArrayToFill) const;
	TInt DoAppCount(TInt& aCount,TInt aCommand) const;
	void FetchArrayL(MArrayFiller& aArrayFiller,TUid aAppUid, TInt aOpcode, TInt aInitialBufSize) const;
	CBufBase* GetServiceBufferLC(TInt aOpcode, TUid aUid1, TUid aUid2 = KNullUid) const;
	CBufBase* GetServiceBufferLC(TInt aOpcode, TUid aUid1, const TDataType& aDataType) const;
	TInt SendReceiveWithReconnect(TInt aFunction,const TIpcArgs& aIpcArgs) const;
	TInt TransferAndInternalizeDataL(const TDesC& aPath, const TInt aRequiredBufferSize, CDataRecognitionResultArray& aResult) const;
	TInt GetExecutableNameAndNewOpaqueData(TDes& aNativeExecutableName, TDes& aLogicalExecutableName, HBufC8*& aOpaqueData, TIpcArgs& aIpcArgs, TInt aOpcode) const;
	TInt GetNewOpaqueData(HBufC8*& aOpaqueData, TInt aLengthOfOpaqueData) const;
	TInt StartApplicationPassingFileHandle(const TDesC& aNativeExecutableName, const TDesC& aLogicalExecutableName, const TDesC8* aOpaqueData, const RFile& aFile, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous);
	TInt StartApplicationPassingDocumentName(const TDesC& aNativeExecutableName, const TDesC& aLogicalExecutableName, const TDesC8* aOpaqueData, const TDesC& aDocumentName, TThreadId& aThreadId,TApaCommand aCommand, TRequestStatus* aRequestStatusForRendezvous);
	void DoStartApplicationL(const TDesC& aNativeExecutableName, const CApaCommandLine& aCommandLine, TThreadId& aThreadId, TRequestStatus* aRequestStatusForRendezvous);
	TInt DoAppForDocumentOptionallySpecifyingService(const TDesC& aFileName, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType, TInt aOpcode) const;
	TInt DoAppForDocumentOptionallySpecifyingService(const RFile& aFile, TUid aServiceUid, TUid& aAppUid, TDataType& aDataType, TInt aOpcode) const;
	TInt GetNewBufferFromFile(HBufC8*& aBuffer, const TDesC& aFileName) const;
	TInt DoStartApp(const CApaCommandLine& aCommandLine, TThreadId* aThreadId, TRequestStatus* aRequestStatusForRendezvous);
	void DoStartAppL(const CApaCommandLine& aCommandLine, TThreadId* aThreadId, TRequestStatus* aRequestStatusForRendezvous);
	static void GetMainThreadIdL(TThreadId& aThreadId, const RProcess& aProcess);
	static void DeletePointerToPointerToTAny(TAny* aPointerToPointerToTAny);
	
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK 	
	static CBufFlat* CreateRegFilesBufferL(const RPointerArray<TDesC>& aRegFiles);
#endif
	
	static void CleanupOperation(TAny* aAny);

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	CBufFlat* CreateAppUpdateInfoBufferL(RArray<TApaAppUpdateInfo>& aAppUpdateInfo);
	CBufFlat* CreateForceRegAppInfoBufferL(const RPointerArray<Usif::CApplicationRegistrationData>& aForceRegAppsInfo);	
#endif
private: // data
	friend class CApaLsSessionExtension;
	CApaLsSessionExtension* iExtension;
	};


#endif	// __APGCLI_H__

