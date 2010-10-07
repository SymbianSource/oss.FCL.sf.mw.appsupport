

#ifndef MOCKCFACTIONINDICATIONIMPL_H
#define MOCKCFACTIONINDICATIONIMPL_H

#include <cfactionindication.h>
#include <cfkeyvaluepair.h>

/**
* Action indication implementation.
* 
* @since S60 5.0
* @lib cfservices.lib
*/
NONSHARABLE_CLASS( CMockCFActionIndicationImpl ): public CCFActionIndication
    {
    public:

        // Two phased constructors
        IMPORT_C static CMockCFActionIndicationImpl* NewL ();
        IMPORT_C static CMockCFActionIndicationImpl* NewLC ();
        
        // Destructor
        ~CMockCFActionIndicationImpl();

    public: // From base classes
    
        // From CCFActionIndication
        void SetIdentifierL( const TDesC& aIdentifier );

        // From CCFActionIndication
        const TDesC& Identifier() const;
        
        // From CCFActionIndication
        void AddParameterL( const TDesC& aKey, const TDesC& aValue );
        
        // From CCFActionIndication    
        const RKeyValueArray& Parameters() const;

    private:
    
        CMockCFActionIndicationImpl();
        void ConstructL();

    private: // Data
    
        // Own: Parameters
        RKeyValueArray iParameters;

        // Own: Identifier
        HBufC* iIdentifier;
    };
#endif  //MOCKCFACTIONINDICATIONIMPL_H
