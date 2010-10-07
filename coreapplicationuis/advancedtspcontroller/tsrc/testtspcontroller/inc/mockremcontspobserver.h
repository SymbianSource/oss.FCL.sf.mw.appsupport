/*
 * MockRemConTSPObserver.h
 *
 *  Created on: 21.7.2010
 *      Author: tongsun
 */

#ifndef MOCKREMCONTSPOBSERVER_H
#define MOCKREMCONTSPOBSERVER_H

#include <e32base.h>
#include <remcon/remcontargetselectorpluginobserver.h>


// CLASS DEFINITION
/**
 * Unit test mock class.
 */
NONSHARABLE_CLASS(CMockRemConTSPObserver) 
    : public MRemConTargetSelectorPluginObserver //,public MCFListener
    {
public:          
        // Constructors and destructor
        static CMockRemConTSPObserver* NewL();
        static CMockRemConTSPObserver* NewLC();
        ~CMockRemConTSPObserver();

        // from MRemConTargetSelectorPluginObserver
        void OutgoingCommandAddressed(TInt aError) ;
        void OutgoingCommandPermitted(TBool aIsPermitted);
        void IncomingCommandAddressed(TInt aError);
        void OutgoingResponsePermitted(TBool aIsPermitted) ;
        void IncomingNotifyAddressed(TClientInfo* aClientInfo, TInt aError); 
        void ReAddressNotifies() ;
        TInt GetConnections(TSglQue<TRemConAddress>& aConnections) ;
public:
        TInt Response();
        void SetResponse(TInt aValue) ;
        TUid BearerUid() {return iBearerUid;}
        TClientInfo ClientInfo() {return iClientInfo; }
private:
        virtual void MrctspoDoOutgoingCommandAddressed(TInt aError);
        virtual void MrctspoDoOutgoingCommandPermitted(TBool /*aIsPermitted*/) ;
        virtual void MrctspoDoIncomingCommandAddressed(TInt aError );
        virtual TInt MrctspoDoGetConnections(TSglQue<TRemConAddress>& /*aConnections*/) ;
        virtual void MrctspoDoOutgoingResponsePermitted(TBool /*aIsPermitted*/);
        virtual void MrctspoDoIncomingNotifyAddressed(TClientInfo* /*aClientInfo*/, TInt aError );
        virtual void MrctspoDoReAddressNotifies() ;

        virtual void MrctspoDoOutgoingNotifyCommandAddressed(TRemConAddress* aConnection, TInt aError);        
        virtual void MrctspoDoOutgoingNotifyCommandPermitted(TBool aIsPermitted) ;
        virtual void MrctspoDoIncomingCommandPermitted(TBool aIsPermitted) ;
        virtual void MrctspoDoIncomingNotifyPermitted(TBool aIsPermitted) ;
        virtual TInt MrctspoSetLocalAddressedClient(const TUid& aBearerUid, const TClientInfo& aClientInfo) ;

    private: // Constructors
        CMockRemConTSPObserver();
        void ConstructL();
    private:
        TInt iObserverResponse;
        TUid iBearerUid;
        TClientInfo iClientInfo;
    };
#endif  //MOCKREMCONTSPOBSERVER_H
