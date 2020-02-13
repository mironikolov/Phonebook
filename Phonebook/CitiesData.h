#pragma once

#include "CitiesTable.h"
#include "PersonsTable.h"
#include "DatabaseConnection.h"
#include "PhoneNumbersTable.h"

class CCitiesData
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CCitiesData();

	virtual ~CCitiesData();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oCitiesArray"> Масив от градове</param>
	///<param name="oPersonsArray"> Масив от хора</param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll(CCitiesArray& oCitiesArray, CPersonsArray& oPersonsArray);

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на града </param>
	///<param name="recCities"> Ред от таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID( const long lID, CITIES& recCities );

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recCities"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID(const long lID, const CITIES& recCities);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recCities"> Ред за добавяне в таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert( CITIES& recCities);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	// Overrides
	// ----------------

	// Members
	// ----------------
};