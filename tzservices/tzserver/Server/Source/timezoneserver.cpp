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
//
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzuserdefineddata.h>
#endif
#include "timezoneserver.h"
#include "timezonesession.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "timezoneserverTraces.h"
#endif


// Set server priority
const TInt KServerPriority = 0;  // EStandard Priority

// indexes into TPolicyElement::KPolicyElements
#define KPolicyElementWriteDeviceData  		0
#define KPolicyElementReadUserData  		1
#define KPolicyElementWriteUserData  		2

// opcodes ranges with different security policies 
const TUint KRangeCount = 5; 

const TInt KOpCodeRanges[KRangeCount] = 
	{	
	KCapabilityNone,
	KCapabilityWriteDeviceData,
	KCapabilityReadUserData,
	KCapabilityWriteUserData,
	CTzServer::ENotSupported
	};

const TUint8 KElementsIndex[KRangeCount] =
	{
	CPolicyServer::EAlwaysPass,		
	KPolicyElementWriteDeviceData,
	KPolicyElementReadUserData,
	KPolicyElementWriteUserData,
	CPolicyServer::ENotSupported	
	};

const CPolicyServer::TPolicyElement KPolicyElements[] = 
	{
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient}, // KPolicyElementWriteDeviceData
	{_INIT_SECURITY_POLICY_C1(ECapabilityReadUserData), CPolicyServer::EFailClient}, // KPolicyElementReadUserData
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteUserData), CPolicyServer::EFailClient}, // KPolicyElementWriteUserData
	};

const CPolicyServer::TPolicy KTzSrvrPolicy =
	{
	CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
	KRangeCount,
	KOpCodeRanges,
	KElementsIndex, 			// what each range is compared to 
	KPolicyElements 			// what policies range is compared to
	};

// Create Tz Server
CTzServer* CTzServer::NewL()
	{
	CTzServer* self = new (ELeave) CTzServer(KServerPriority);
	CleanupStack::PushL(self);

	self->ConstructL();

	CleanupStack::Pop(self);
	return self;
	}

CTzServer::~CTzServer()
	{
	delete iTzLocalizationDb;
	delete iTimeZoneMgr;
	delete iTzUserDataDb;
	delete iTzSwiObserver;
	}

CTzServer::CTzServer(TInt aPriority)
:CPolicyServer(aPriority, KTzSrvrPolicy, ESharableSessions)
	{
	}

// Create and connect to the file server
// Rename the server process to "TZServer"
// Start the server
//
void CTzServer::ConstructL()
	{
    OstTraceDef0( OST_TRACE_CATEGORY_DEBUG,TRACE_FLOW_PARAM, CTZSERVER_CONSTRUCTL_ENTRY, "CTzServer::ConstructL Entry" );
    
	iTzUserDataDb = CTzUserDataDb::NewL();
	iTzSwiObserver = CTzSwiObserver::NewL();
	iTimeZoneMgr = CTzConfigAgent::NewL(*this, *iTzUserDataDb, *iTzSwiObserver);
	iTzLocalizationDb = CTzLocalizationDb::NewL(*this);
	iTimeZoneMgr->SetLocalizationDb(iTzLocalizationDb);
	iTzUserDataDb->AddObserverL(iTimeZoneMgr);
	iTimeZoneMgr->AddObserverL(iTzLocalizationDb);
	StartL(KTimeZoneServerName); 
	OstTraceDef0( OST_TRACE_CATEGORY_DEBUG,TRACE_FLOW_PARAM, CTZSERVER_CONSTRUCTL_EXIT, "CTzServer::ConstructL Exit" );
	
	}

// Create a new server session
CSession2* CTzServer::NewSessionL(const TVersion& aVersion,const RMessage2& /*aMessage*/) const
	{
    OstTraceDefExt3( OST_TRACE_CATEGORY_DEBUG,TRACE_FLOW_PARAM, CTZSERVER_NEWSESSIONL_ENTRY, "CTzServer::NewSessionL Entry;Major version No=%hhd;Minorr version No=%hhd;Build  version No=%hd", aVersion.iMajor, aVersion.iMinor, aVersion.iBuild );
    
    
	TVersion ver(KTimeZoneServerMajorVersion, KTimeZoneServerMinorVersion, KTimeZoneServerBuildVersion);
	if (!User::QueryVersionSupported(ver,aVersion))
		{
	    OstTraceDef1( OST_TRACE_CATEGORY_DEBUG,TRACE_ERROR, CTZSERVER_NEWSESSIONL, "CTzServer::NewSessionL Version not supported  Error value: %d", KErrNotSupported );
	    
		User::Leave(KErrNotSupported);
		}

	CTzServerSession* session = CTzServerSession::NewL();

	SessionAdded();
	OstTraceDef0( OST_TRACE_CATEGORY_DEBUG,TRACE_FLOW_PARAM, DUP1_CTZSERVER_NEWSESSIONL_EXIT, "CTzServer::NewSessionL Exit" );
	
	return session;
	}

void CTzServer::SessionClosed() const
	{
	// Called when a session exits
	// If this is the last session for the server, then stop the scheduler so server can be destroyed
	// outside the context of this call
	--iSessionCount;
	if (iSessionCount == 0)
		{
		CActiveScheduler::Stop();
		}
	}

void CTzServer::SessionAdded() const
	{
	iSessionCount++;
	}

// Notifies Change to all client sessions
// each client is responsible for rejecting the call 
// if there is no pending request
void CTzServer::NotifyTZStatusChange(RTz::TTzChanges aChange, const TAny* aRequester)
	{
    
	iSessionIter.SetToFirst();
	CSession2* pS;
	while ((pS = iSessionIter++)!=NULL)
		{
		(static_cast<CTzServerSession*>(pS))->NotifyTZStatusChange(aChange, aRequester);
		}
	}

//
//	Message servicing failed. Complete message with error code and restart
//
void CTzServer::Error(TInt aError)
	{
	Message().Complete(aError);
	ReStart();
	}
