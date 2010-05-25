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
 Name		: t_oomtestappDocument.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __T_OOMTESTAPPDOCUMENT_h__
#define __T_OOMTESTAPPDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class Ct_oomtestappAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * Ct_oomtestappDocument application class.
 * An instance of class Ct_oomtestappDocument is the Document part of the
 * AVKON application framework for the t_oomtestapp example application.
 */
class Ct_oomtestappDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a Ct_oomtestappDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of Ct_oomtestappDocument.
	 */
	static Ct_oomtestappDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a Ct_oomtestappDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of Ct_oomtestappDocument.
	 */
	static Ct_oomtestappDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~Ct_oomtestappDocument
	 * Virtual Destructor.
	 */
	virtual ~Ct_oomtestappDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a Ct_oomtestappAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * Ct_oomtestappDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	Ct_oomtestappDocument(CEikApplication& aApp);

	};

#endif // __T_OOMTESTAPPDOCUMENT_h__
// End of File
