#pragma once

#include <atldbcli.h>
#include "Structures.h"
#include "Enums.h"

class CPersonsAccessor
{
protected:
	PERSONS m_recPerson;

	BEGIN_ACCESSOR_MAP(CPersonsAccessor, 2)

		BEGIN_ACCESSOR(AccessorZero, true)
		COLUMN_ENTRY( 1, m_recPerson.lID )
		END_ACCESSOR()

		BEGIN_ACCESSOR(AccessorOne, true)
		COLUMN_ENTRY(2, m_recPerson.lUpdateCounter)
		COLUMN_ENTRY(3, m_recPerson.szName)
		COLUMN_ENTRY(4, m_recPerson.szSurname)
		COLUMN_ENTRY(5, m_recPerson.szLastName)
		COLUMN_ENTRY(6, m_recPerson.szUCN)
		COLUMN_ENTRY(7, m_recPerson.lCityID)
		COLUMN_ENTRY(8, m_recPerson.szAddress)
		END_ACCESSOR()

	END_ACCESSOR_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

class CPersonsTable :private CCommand<CAccessor<CPersonsAccessor>>
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CPersonsTable();
	CPersonsTable( CSession& oSession );
	virtual ~CPersonsTable();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oPersonsArray"> Масив от типове телефони </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll(CPersonsArray& oPersonsArray);

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на човека </param>
	///<param name="recPerson"> Ред от таблица PERSONS </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID(const long lID, PERSONS& recPerson);

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recPerson"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID(const long lID, const PERSONS& recPerson);

	///<summary> Добавя запис в таблица </summary>
	///<param name="recPerson"> Ред за добавяне в таблица PERSONS </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert(PERSONS& recPerson);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на човека </param>
	///<param name="oPhoneNumberArray">Масив от хора </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWherePersonID( const long lID, CPersonsArray& oPersonsArray );

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