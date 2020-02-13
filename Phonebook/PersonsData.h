#pragma once

#include "PersonsTable.h"
#include "PhoneNumbersTable.h"
#include "CitiesTable.h"
#include "PersonWithNumbers.h"
#include "PhoneTypesTable.h"
#include "DatabaseConnection.h"

class CPersonsData
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CPersonsData();

	virtual ~CPersonsData();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oPersonsArray"> Масив от хора</param>
	///<param name="oCitiesArray"> Масив от градове</param>
	///<param name="oPhoneTypesArray"> Масив от типове телефони </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll( CPersonsArray& oPersonsArray, CCitiesArray& oCitiesArray, CPhoneTypesArray& oPhoneTypesArray );

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на човек </param>
	///<param name="recPerson"> Ред от таблица PERSONS </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID(const long lID, CPersonWithNumbers& oPersonWithNumbers );

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="oPersonWithNumbers"> Човек за промяна </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID( const long lID, CPersonWithNumbers& oPersonWithNumbers );

	///<summary> Добавя запис в таблица </summary>
	///<param name="recPerson"> Ред за добавяне в таблица PERSONS </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert(CPersonWithNumbers& oPersonWithNumbers);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	// Overrides
	// ----------------

	// Members
	// ----------------
};