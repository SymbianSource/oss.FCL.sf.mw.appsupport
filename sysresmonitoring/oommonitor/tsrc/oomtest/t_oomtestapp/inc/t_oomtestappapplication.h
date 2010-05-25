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
* Description:  
*
*/

/*
 ============================================================================
 Name		: t_oomtestappApplication.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __T_OOMTESTAPPAPPLICATION_H__
#define __T_OOMTESTAPPAPPLICATION_H__

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION

/**
 * Ct_oomtestappApplication application class.
 * Provides factory to create concrete document object.
 * An instance of Ct_oomtestappApplication is the application part of the
 * AVKON application framework for the t_oomtestapp example application.
 */
class Ct_oomtestappApplication : public CAknApplication
	{
public:
	Ct_oomtestappApplication();
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID
	 */
	TUid AppDllUid() const;

	~Ct_oomtestappApplication();
protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates Ct_oomtestappDocument document object. The returned
	 * pointer in not owned by the Ct_oomtestappApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	
private:
	};

#endif // __T_OOMTESTAPPAPPLICATION_H__
// End of File
