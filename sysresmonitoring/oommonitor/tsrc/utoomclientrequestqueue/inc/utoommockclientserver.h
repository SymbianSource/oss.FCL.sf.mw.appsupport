/*
 * utoomconstant.h
 *
 *  Created on: Aug 9, 2010
 *      Author: t4sun
 */

#ifndef UTOOMMOCKCLIENTSERVER_H_
#define UTOOMMOCKCLIENTSERVER_H_
// define properties names 
const TUid KUTOomPropertyCategory = {0x10000000};
const TUint32 KUTOomStubPropertyKey = 0;
const TUint32 KUTOomFreeMemorySetKey = 1;
const TUint32 KUTOomMemoryGoodSetKey = 2;
const TInt Stub_CMemoryMonitor_FreeOptionalRamL = 100;
const TInt Stub_CMemoryMonitor_RequestFreeMemoryL = 200;
const TInt Stub_CMemoryMonitor_RequestFreeMemoryPandSL = 300;


const TUint KDefaultHeapSize=0x10000;

enum TOomServerPanic
    {
    EBadRequest = 1,
    EBadDescriptor,
    EMainSchedulerError,
    ESvrCreateServer,
    ESvrStartServer,
    ECreateTrapCleanup,
    ENotImplementedYet,
    };

IMPORT_C TInt StartThread(RThread& aServerThread);

#endif /* UTOOMMOCKCLIENTSERVER_H_ */
