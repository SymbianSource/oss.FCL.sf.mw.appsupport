
#include "mockcfactionindicationimpl.h"

EXPORT_C CMockCFActionIndicationImpl* CMockCFActionIndicationImpl::NewL()
    {
    
    CMockCFActionIndicationImpl* self = CMockCFActionIndicationImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CMockCFActionIndicationImpl* CMockCFActionIndicationImpl::NewLC()
    {
    
    CMockCFActionIndicationImpl* self = new( ELeave ) CMockCFActionIndicationImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

CMockCFActionIndicationImpl::~CMockCFActionIndicationImpl()
    {
    
    delete iIdentifier;
    iParameters.ResetAndDestroy();
    }
    
CMockCFActionIndicationImpl::CMockCFActionIndicationImpl()
    {
    }
    
void CMockCFActionIndicationImpl::ConstructL()
    {
    
    iIdentifier = KNullDesC().AllocL();
    }
    
// METHODS

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::SetIdentifierL
//-----------------------------------------------------------------------------
//
void CMockCFActionIndicationImpl::SetIdentifierL(
    const TDesC& aIdentifier )
    {
    
    TPtr identifierPtr = iIdentifier->Des();
    if( identifierPtr.MaxLength() >= aIdentifier.Length() )
        {
        // Just copy
        identifierPtr.Copy( aIdentifier );
        }
    else
        {
        // Delete and create a new one
        delete iIdentifier;
        iIdentifier = NULL;
        iIdentifier = aIdentifier.AllocL();
        }
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::Identifier
//-----------------------------------------------------------------------------
//
const TDesC& CMockCFActionIndicationImpl::Identifier() const
    {
    
    return *iIdentifier;
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::AddParameterL
//-----------------------------------------------------------------------------
//
void CMockCFActionIndicationImpl::AddParameterL( const TDesC& aKey,
    const TDesC& aValue )
    {
    
    CCFKeyValuePair* obj = CCFKeyValuePair::NewLC( aKey, aValue );
    iParameters.AppendL( obj );
    CleanupStack::Pop( obj );
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl::Parameters
//-----------------------------------------------------------------------------
//
const RKeyValueArray& CMockCFActionIndicationImpl::Parameters() const
    {
    return iParameters;
    }

