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
* Description:  Declarations of OOM Monitor.
*
*/


#include <e32hashtab.h>
#include <f32file.h>
#include <w32std.h>
#include <apgtask.h>
#include <e32property.h>

/**
 * Creates thread for OOM watchers.
 * Called by the server in which the OOM Monitor runs.
 */
IMPORT_C void CreateOOMWatcherThreadL();

// ---------------------------------------------------------
// CMemoryMonitor
// ---------------------------------------------------------
//
class COutOfMemoryWatcher;
class CSubscribeHelper;
class COomMonitorPlugin;
class COomPlugins;
class CAppCloseTimer;
class CAppCloseWatcher;
class CMemoryMonitorServer;
class CWservEventReceiver;
class RResourceFile;
class CApaWindowGroupName;

NONSHARABLE_CLASS(CMemoryMonitor) : public CBase
    {
public:
    static CMemoryMonitor* NewL();
    ~CMemoryMonitor();

public: // event handlers
    void FreeMemThresholdCrossedL();
    void CloseAppEvent();
    void AppNotExiting(TInt aWgId);
    void StartFreeSomeRamL(TInt aTargetFree);
    void HandleFocusedWgChangeL();
    
private:
    CMemoryMonitor();
    void ConstructL();
    void CloseAppsFinished(TBool aMemoryGood);
    static TInt WatchdogStatusStatusChanged(TAny* aPtr);
    void HandleWatchdogStatusCallBackL();
    void ColapseWindowGroupTree();
    void SetPluginMemoryGood(TBool aSetToGood);
    void GetWgsToCloseL();
    TBool FreeMemoryAboveThreshold();
    void CancelAppCloseWatchers();
    TInt AppCloseOrder(TInt aWgIndex, TInt aWgId);
    void CloseNextApp();
    void ReadAppResourceArrayL(RResourceFile& aResFile, TInt aResId, TInt aOrderBase, TInt aOrderInc);
	void RestartAppCloser();
	
public:
	// All members are owned
    // generally useful sessions
    RFs iFs;
    RWsSession iWs;

private:
	// All members are owned, except where stated
    // parameters for OOM watcher.
    TInt iLowThreshold;
    TInt iGoodThreshold;
    TInt iMaxExitTime; // Time we wait for application exit gracefully, after this we'll try to shut next application down.
    TInt iRamPluginRunTime; // Time we wait for plugins to free RAM before we start to shut apps.

    // parameters for app close order
    enum TAppCloseOrderConstants
        {
        ECloseFirst = -1000000,        // apps to close first are given an order around negative one million
        ECloseNormal = 0,            // apps to close normally are given an order around zero
        ECloseLast = 1000000,        // apps to close last are given an order around one million
        ENeverClose = 1000000000    // one billion signals apps that should never be closed
        };
    typedef RHashMap<TInt, TInt> RAppCloseOrderMap;    // maps app UID to close order
    RAppCloseOrderMap iAppCloseOrderMap;

    // internal
    CApaWindowGroupName* iWgName;
    HBufC* iWgNameBuf;              // owned by iWgName

    // app closer state
    TBool iAppCloserRunning;
    TInt iCurrentTarget;
    TApaTask iCurrentTask;
    RArray<RWsSession::TWindowGroupChainInfo> iWgIds;
    TInt iNextAppToClose;
    TBool iMemoryAboveThreshold;
    
    // event receivers
    CAppCloseTimer* iAppCloseTimer;
    CAppCloseWatcher* iAppCloseWatcher;
    COutOfMemoryWatcher* iOOMWatcher;
    CWservEventReceiver* iWservEventReceiver;

    // parameters for P&S watcher.
    RProperty iWatchdogStatusProperty;
    CSubscribeHelper* iWatchdogStatusSubscriber;

    // Plugin support
    COomPlugins* iPlugins;
    TBool iPluginMemoryGood;
    
    // Server
    CMemoryMonitorServer* iServer;
    };



NONSHARABLE_CLASS(COutOfMemoryWatcher) : public CActive
    {
public:
    static COutOfMemoryWatcher* NewL(CMemoryMonitor& aLafShutdown, TInt aLowThreshold, TInt aGoodThreshold);
    ~COutOfMemoryWatcher();
    void Start();
private:
    COutOfMemoryWatcher(CMemoryMonitor& aLafShutdown);
    void ConstructL(TInt aLowThreshold, TInt aGoodThreshold);
private: // from CActive
    void DoCancel();
    void RunL();
private: // data
    RChangeNotifier iChangeNotifier;
    CMemoryMonitor& iLafShutdown;
    };

NONSHARABLE_CLASS(CSubscribeHelper) : public CActive
    {
public:
    CSubscribeHelper(TCallBack aCallBack, RProperty& aProperty);
    ~CSubscribeHelper();

public: // New functions
    void SubscribeL();
    void StopSubscribe();

private: // from CActive
    void RunL();
    void DoCancel();

private:
    TCallBack   iCallBack;
    RProperty&  iProperty;
    };


NONSHARABLE_CLASS(COomPlugins) : public CBase
    {
public:
    COomPlugins();
    ~COomPlugins();
    void ConstructL();
    
public:
    void FreeRam();
    void MemoryGood();
    
private:
    struct TPlugin
        {
        TPlugin();
        COomMonitorPlugin* iImpl;
        TUid iDtorUid;
        };

private:
    RArray<TPlugin> iPlugins;
    };


NONSHARABLE_CLASS(CAppCloseTimer) : public CTimer
    {
public:
    static CAppCloseTimer* NewL(CMemoryMonitor& aMonitor);
private:
    CAppCloseTimer(CMemoryMonitor& aMonitor);
    void RunL();
private:
    CMemoryMonitor& iMonitor;
    };


NONSHARABLE_CLASS(CAppCloseWatcher) : public CActive
    {
public:
    CAppCloseWatcher(CMemoryMonitor& aMonitor);
    ~CAppCloseWatcher();
    void Start(const TApaTask& aTask);
private:
    void DoCancel();
    void RunL();
private:
    CMemoryMonitor& iMonitor;
    RThread iThread;
    TProcessPriority iOriginalProcessPriority;
    };


NONSHARABLE_CLASS(CWservEventReceiver) : public CActive
    {
public:
    CWservEventReceiver(CMemoryMonitor& aMonitor, RWsSession& aWs);
    ~CWservEventReceiver();
    void ConstructL();
private:
    void Queue();
    void DoCancel();
    void RunL();
private:
    CMemoryMonitor& iMonitor;
    RWsSession& iWs;
    RWindowGroup iWg;
    };


NONSHARABLE_CLASS(CMemoryMonitorServer) : public CServer2
    {
public:
    static CMemoryMonitorServer* NewL(CMemoryMonitor& aMonitor);
    ~CMemoryMonitorServer();

    CMemoryMonitor& Monitor();
    void CloseAppsFinished(TBool aMemoryGood);

private:
    CMemoryMonitorServer(CMemoryMonitor& aMonitor);
    void ConstructL();
    CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
    TInt RunError(TInt aError);

private:
    CMemoryMonitor& iMonitor;
    };


NONSHARABLE_CLASS(CMemoryMonitorSession) : public CSession2
    {
public:
    CMemoryMonitorSession();
    void CreateL();
    void CloseAppsFinished(TBool aMemoryGood);

private:
    ~CMemoryMonitorSession();
    CMemoryMonitorServer& Server();
    CMemoryMonitor& Monitor();
    void ServiceL(const RMessage2& aMessage);

private:
    RMessagePtr2 iRequestFreeRam;
    };


