/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbWidgets module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/

#ifndef HBDEVICEINPUTDIALOGSYMBIAN_H
#define HBDEVICEINPUTDIALOGSYMBIAN_H

#if defined(__SYMBIAN32__) || defined(SYMBIAN) || defined(HB_DOXYGEN)

#include <e32base.h>
#include <hbsymbianvariant.h>
#include <e32debug.h>
#ifdef _DEBUG

    #define TRACES( x ) x

#else

    #define TRACES( x )

#endif

class CHbDeviceInputDialogPrivate;
class MHbDeviceInputDialogObserver;

class CHbDeviceInputDialogSymbian : public CBase
{
public:
      enum InputMode {
            TextInput,
            IntInput,
            RealInput,
            IpInput 
      };
    
    enum TButtonId {
        EAcceptButton,
        ERejectButton
    };

public:

    static CHbDeviceInputDialogSymbian* NewL(MHbDeviceInputDialogObserver *aObserver = 0);
    ~CHbDeviceInputDialogSymbian();
    
    TPtrC getTextL();
   
    void ShowL();
    void Close(); 

    void SetObserver(MHbDeviceInputDialogObserver *aObserver);
    
private:
    friend class CHbDeviceInputDialogPrivate;
    CHbDeviceInputDialogSymbian();
    CHbDeviceInputDialogPrivate* d;
    TBuf<16> iPassword;
    TBuf8<16> iTemp;
    
};

class MHbDeviceInputDialogObserver
{
public:
    virtual void InputDialogClosed(const CHbDeviceInputDialogSymbian* aInputDialog,
        CHbDeviceInputDialogSymbian::TButtonId aButton) = 0;
    virtual void DataReceived(CHbSymbianVariantMap data) = 0; 
};

#endif // defined(__SYMBIAN32__) || defined(SYMBIAN) || defined(HB_DOXYGEN)
#endif // HBDEVICEINPUTDIALOGSYMBIAN_H
