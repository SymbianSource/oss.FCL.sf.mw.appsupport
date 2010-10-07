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



#include "mock_oomactionlist.h"

void CMoc_COomActionList::StateChanged()
	  {
	  iStateChange++;
	  CActiveScheduler::Stop();
	  }
TInt CMoc_COomActionList::GetState()
	  {
	  return iStateChange;
	  }
