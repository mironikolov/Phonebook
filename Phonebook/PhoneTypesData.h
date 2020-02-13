#pragma once

#include "PhoneTypesTable.h"

class CPhoneTypesData
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesData();

	virtual ~CPhoneTypesData();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oPhoneTypesArray"> Масив от градове</param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll(CPhoneTypesArray& oPhoneTypesArray);

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на града </param>
	///<param name="recPhoneType"> Ред от таблица PHONE_TYPES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID(const long lID, PHONE_TYPES& recPhoneType);

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recPhoneType"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID(const long lID, const PHONE_TYPES& recPhoneType);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recPhoneType"> Ред за добавяне в таблица PHONE_TYPES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert(PHONE_TYPES& recPhoneType);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	// Overrides
	// ----------------

	// Members
	// ----------------
};
