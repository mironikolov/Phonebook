#pragma once

#include <atldbcli.h>
#include "Structures.h"
#include "Enums.h"

class CCitiesAccessor
{
protected:
	CITIES m_recCity;

	BEGIN_ACCESSOR_MAP(CCitiesAccessor, 2)

		BEGIN_ACCESSOR(AccessorZero, true)
			COLUMN_ENTRY(1, m_recCity.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(AccessorOne, true)
			COLUMN_ENTRY(2, m_recCity.lUpdateCounter)
			COLUMN_ENTRY(3, m_recCity.szName)
			COLUMN_ENTRY(4, m_recCity.szDistrict)
		END_ACCESSOR()

	END_ACCESSOR_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

class CCitiesTable :private CCommand<CAccessor<CCitiesAccessor>>
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CCitiesTable();
	CCitiesTable(CSession& oSession);
	virtual ~CCitiesTable();

	// Methods
	// ----------------
public:
	///<summary> Избира всички записи в таблицата </summary>
	///<param name="oCitiesArray"> Масив от градове </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectAll( CCitiesArray& oCitiesArray );

	///<summary> Избира запис с определено ID </summary>
	///<param name="lID"> ID на града </param>
	///<param name="recCities"> Ред от таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool SelectWhereID( const long lID, CITIES& recCities );

	///<summary> Променя запис с определено ID </summary>
	///<param name="lID"> ID на запис </param>
	///<param name="recCities"> Ред за запис </param>
	///<returns> true при успех, в противен случай false </returns>
	bool UpdateWhereID( const long lID, const CITIES& recCities );

	///<summary> Добавя запис в таблица </summary>
	///<param name="recCities"> Ред за добавяне в таблица CITIES </param>
	///<returns> true при успех, в противен случай false </returns>
	bool Insert( CITIES& recCities );

	///<summary> Трие запис с определено ID </summary>
	///<param name="lID"> ID на ред за изтриване </param>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool DeleteWhereID( const long lID );

private:
	///<summary> Добавя свойства на PropSet </summary>
	///<param name="oUpdateDBPropSet"> PropSet, на който се добавят properties </param>
	void InitializePropSetProperties( CDBPropSet& oDBPropSet );
	
	///<summary> Затваря всички връзки с базата </summary>
	void CloseConnection();

	///<summary> Изпълнява заявка за избиране на ред </summary>
	///<param name="lID"> ID на ред </param>
	///<param name="bModifyRowset"> Флаг за промяна на Rowset </param>
	///<returns> Връща true при успех, в противен случай false </returns> 
	bool SelectWhereID( const long lID, const bool bModifyRowset);

	void AbortSession();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	CSession m_oSession;
	bool m_bExternalSession = false;
};
