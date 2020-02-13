#pragma once

#include <atldbcli.h>
#include "Structures.h"
#include "Enums.h"

class CPhoneTypesAccessor
{
protected:
	PHONE_TYPES m_recPhoneType;

	BEGIN_ACCESSOR_MAP( CPhoneTypesAccessor, 2 )

		BEGIN_ACCESSOR( AccessorZero, true )
		COLUMN_ENTRY( 1, m_recPhoneType.lID )
		END_ACCESSOR()

		BEGIN_ACCESSOR( AccessorOne, true )
		COLUMN_ENTRY( 2, m_recPhoneType.lUpdateCounter )
		COLUMN_ENTRY( 3, m_recPhoneType.szType )
		END_ACCESSOR()

	END_ACCESSOR_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

class CPhoneTypesTable :private CCommand<CAccessor<CPhoneTypesAccessor>>
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesTable();
	virtual ~CPhoneTypesTable();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oPhoneTypesArray"> Масив от типове телефони </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll( CPhoneTypesArray& oPhoneTypesArray );

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на типа телефон </param>
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
	bool Insert( PHONE_TYPES& recPhoneType);

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID(const long lID);

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

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	CSession m_oSession;
};