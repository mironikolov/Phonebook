#pragma once

#include <atldbcli.h>
#include "Structures.h"
#include "Enums.h"

class CPhoneNumbersAccessor
{
protected:
	PHONE_NUMBERS m_recPhoneNumber;

	BEGIN_ACCESSOR_MAP( CPhoneNumbersAccessor, 2 )

		BEGIN_ACCESSOR( AccessorZero, true )
		COLUMN_ENTRY( 1, m_recPhoneNumber.lID )
		END_ACCESSOR()

		BEGIN_ACCESSOR(AccessorOne, true)
		COLUMN_ENTRY( 2, m_recPhoneNumber.lUpdateCounter )
		COLUMN_ENTRY( 3, m_recPhoneNumber.lPersonID )
		COLUMN_ENTRY( 4, m_recPhoneNumber.lPhoneTypeID )
		COLUMN_ENTRY( 5, m_recPhoneNumber.szNumber )
		END_ACCESSOR()

	END_ACCESSOR_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

class CPhoneNumbersTable :private CCommand<CAccessor<CPhoneNumbersAccessor>>
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CPhoneNumbersTable();
	CPhoneNumbersTable( CSession& oSession );
	virtual ~CPhoneNumbersTable();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oPhoneTypesArray"> Масив от типове телефони </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll( CPhoneNumbersArray& oPhoneNumbersArray);

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на телефонен номер </param>
	///<param name="recPhoneNumber"> Ред от таблица PHONE_NUMBERS </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID(const long lID, PHONE_NUMBERS& recPhoneNumber);

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recPhoneNumber"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID(const long lID, const PHONE_NUMBERS& recPhoneNumber);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recPhoneNumber"> Ред за добавяне в таблица PHONE_NUMBERS </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert(PHONE_NUMBERS& recPhoneNumber);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	///<summary> Изпълнява заявка за избиране на ред </summary>
	///<param name="lID"> ID на ред </param>
	///<param name="bModifyRowset"> Флаг за промяна на Rowset </param>
	///<returns> Връща true при успех, в противен случай false </returns> 
	bool SelectWherePersonID(const long lID, CPhoneNumbersArray& recPhoneNumber);

private:
	///<summary> Добавя свойства на PropSet </summary>
	///<param name="oUpdateDBPropSet"> PropSet, на който се добавят properties </param>
	void InitializePropSetProperties(CDBPropSet& oDBPropSet);

	///<summary> Затваря всички връзки с базата </summary>
	void CloseConnection();

	///<summary> Изпълнява заявка за избиране на ред </summary>
	///<param name="lID"> ID на ред </param>
	///<param name="bModifyRowset"> Флаг за промяна на Rowset </param>
	///<returns> Връща true при успех, в противен случай false </returns> 
	bool SelectWhereID(const long lID, bool bModifyRowset);

	void AbortSession();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	CSession m_oSession;
	bool m_bExternalSession = false;
};