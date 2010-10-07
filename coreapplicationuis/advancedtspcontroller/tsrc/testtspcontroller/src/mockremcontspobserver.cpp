
#include <e32base.h>
#include <remcon/clientinfo.h>
#include "mockremcontspobserver.h"

CMockRemConTSPObserver* CMockRemConTSPObserver::NewL()
    {
    CMockRemConTSPObserver * self = new ( ELeave ) CMockRemConTSPObserver( );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
CMockRemConTSPObserver::~CMockRemConTSPObserver()
    {
    
    }
CMockRemConTSPObserver::CMockRemConTSPObserver()
    {
    
    }
void CMockRemConTSPObserver::ConstructL()
    {
    

    }

TInt CMockRemConTSPObserver::Response()
    {return iObserverResponse; } 

void CMockRemConTSPObserver::SetResponse(TInt aValue) 
    { iObserverResponse = aValue; }


void CMockRemConTSPObserver::MrctspoDoOutgoingCommandAddressed(TInt aError) 
    { 
    iObserverResponse = aError; 
    }
void CMockRemConTSPObserver::MrctspoDoOutgoingCommandPermitted(TBool aIsPermitted) 
    {
    if( aIsPermitted )
        {
        iObserverResponse = KErrNone;
        }
    else
        {
        iObserverResponse = KErrNotFound;
        }
    }
void CMockRemConTSPObserver::MrctspoDoIncomingCommandAddressed(TInt aError ) 
    { 
    iObserverResponse = aError;
    }
TInt CMockRemConTSPObserver::MrctspoDoGetConnections(TSglQue<TRemConAddress>& /*aConnections*/) 
    { return 0; }
void CMockRemConTSPObserver::MrctspoDoOutgoingResponsePermitted(TBool aIsPermitted)
    {
    if( aIsPermitted )
        {
        iObserverResponse = KErrNone; 
        }
    else
        {
        iObserverResponse = KErrNotFound;
        }
    }
void CMockRemConTSPObserver::MrctspoDoIncomingNotifyAddressed(TClientInfo* aClientInfo, TInt aError )
    {
    iObserverResponse = aError;
    if( aClientInfo )
        {
        iClientInfo = *aClientInfo; 
        }
    else
        {
        iClientInfo.SecureId().iId = 0;
        }
    }
void CMockRemConTSPObserver::MrctspoDoReAddressNotifies() 
    {}

void CMockRemConTSPObserver::MrctspoDoOutgoingNotifyCommandAddressed(TRemConAddress* aConnection, TInt aError)
    {}        
void CMockRemConTSPObserver::MrctspoDoOutgoingNotifyCommandPermitted(TBool aIsPermitted) 
    {
    if( aIsPermitted )
        {
        iObserverResponse = KErrNone; 
        }
    else
        {
        iObserverResponse = KErrNotFound;
        }
    }
void CMockRemConTSPObserver::MrctspoDoIncomingCommandPermitted(TBool aIsPermitted) 
    {
    if( aIsPermitted )
        {
        iObserverResponse = KErrNone; 
        }
    else
        {
        iObserverResponse = KErrNotFound;
        }
    }
void CMockRemConTSPObserver::MrctspoDoIncomingNotifyPermitted(TBool aIsPermitted) 
    {
    if( aIsPermitted )
        {
        iObserverResponse = KErrNone; 
        }
    else
        {
        iObserverResponse = KErrNotFound;
        }
    }
TInt CMockRemConTSPObserver::MrctspoSetLocalAddressedClient(const TUid& aBearerUid, const TClientInfo& aClientInfo) 
    {
    iObserverResponse = KErrNone;
    iBearerUid = aBearerUid;
    iClientInfo = aClientInfo;
    return KErrNone;
    }

