/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef MOC_OOMACTIONLIST_H
#define MOC_OOMACTIONLIST_H

#include "oomactionlist.h"

NONSHARABLE_CLASS(CMoc_COomActionList) : public CBase, public MOomActionObserver
    {
public:
    void StateChanged();
    TInt GetState();    	
private:
	TInt  iStateChange;  
    };

#endif /*MOC_OOMACTIONLIST_H*/
