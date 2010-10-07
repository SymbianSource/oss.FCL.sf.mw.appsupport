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
* Description: UTSlot class definition. 
*
*/


#ifndef UTSLOT_H_
#define UTSLOT_H_
#include <QObject>

class UTSlot : public QObject
    {
    Q_OBJECT    
    
public:
    UTSlot();
    virtual ~UTSlot();
public slots:
    void dataChangedSlot();
    void deactivateSlot();
public:
    TBool iDataChangedSlotCalled;
    TBool iDeactiveSlotCalled;
    };

#endif /* UTSLOT_H_ */
