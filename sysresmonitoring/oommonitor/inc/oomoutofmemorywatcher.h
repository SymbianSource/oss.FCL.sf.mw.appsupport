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
* Description:  Main classes for Out of Memory Monitor.
*
*/


#ifndef OOMOUTOFMEMORYWATCHER_H
#define OOMOUTOFMEMORYWATCHER_H

#include <e32base.h>

class CMemoryMonitor;

/**
 *  This class is an active object which monitors a kernel change notifer for changes in the 
 *  low and good memory thresholds. 
 *  
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COutOfMemoryWatcher) : public CActive
    {
public:
    static COutOfMemoryWatcher* NewL(CMemoryMonitor& aLafShutdown, TInt aLowThreshold, TInt aGoodThreshold);
    ~COutOfMemoryWatcher();
    void Start();
    
    /**
     * Set the Low and Good thresholds that we are listening for.
     * These values can change from the global default values when certain applications are in the foreground.
     * @param aLowThreshold If Ram Level drops below the low threshold Oom Monitor actions are started.
     * @param aGoodThreshold When memory returns above the Good threshold then Oom Monitor stops freeing memory.
     */
    void UpdateThresholds(TInt aLowThreshold, TInt aGoodThreshold);
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

#endif /*OOMOUTOFMEMORYWATCHER_H*/
