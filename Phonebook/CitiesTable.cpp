// CCitiesTable.cpp
//

#include "stdafx.h"
#include <atldbcli.h>
#include "CitiesTable.h"
#include "DatabaseConnection.h"

#define SELECT_ALL_FROM_CITIES _T( "SELECT * FROM CITIES" )
#define SELECT_ALL_FROM_CITIES_WHERE_ID _T( "SELECT * FROM CITIES WHERE ID=%d" )
#define SELECT_ALL_FROM_CITIES_WITH_UPDLOCK_WHERE_ID _T( "SELECT * FROM CITIES WITH (UPDLOCK) WHERE ID=%d" )
#define SELECT_TOP_0_FROM_CITIES _T( "SELECT TOP 0 * FROM CITIES" )

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

// Constructor / Destructor
// ----------------

CCitiesTable::CCitiesTable()
{
	
}

CCitiesTable::CCitiesTable(CSession & oSession)
{
	m_oSession = oSession;
	m_bExternalSession = true;
}

CCitiesTable::~CCitiesTable()
{
	
}

// Methods
// ----------------

bool CCitiesTable::SelectAll( CCitiesArray& oCitiesArray ) 
{
	HRESULT hResult = S_FALSE;
	if (!m_bExternalSession)
	{
		// Отваряме сесия 
		hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	// Конструираме заявката
	CString strQuery = SELECT_ALL_FROM_CITIES;

	// Изпълняваме командата
	hResult = Open( m_oSession, strQuery );
	if ( FAILED( hResult ) )
	{
		MessageBox( NULL, _T( "Грешка при изпълняване на заявка." ), NULL, MB_ICONHAND );
		
		CloseConnection();
		
		return false;
	}

	// Прочитаме всички данни
	while ( ( hResult = MoveNext() ) == S_OK )
	{
		oCitiesArray.Add( new CITIES( m_recCity ) );
	}

	if ( hResult !=  DB_S_ENDOFROWSET )
	{
		MessageBox(NULL, _T( "Грешка при прочитане на цялата таблица" ), NULL, MB_ICONHAND );

		CloseConnection();

		return false;
	}

	// Затваряме командата, сесията и връзката с базата данни. 
	CloseConnection();

	return true;
}

bool CCitiesTable::SelectWhereID( const long lID, CITIES& recCity )
{
	HRESULT hResult = S_FALSE;
	if (!m_bExternalSession)
	{
		// Отваряме сесия 
		hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	if ( !SelectWhereID( lID, false ) )
	{
		CloseConnection();
		return false;
	}

	recCity = m_recCity;
	
	// Затваряме командата и сесията. 
	CloseConnection();

	return true;
}

bool CCitiesTable::UpdateWhereID( const long lID, const CITIES& recCities )
{
	HRESULT hResult = S_FALSE;
	if (!m_bExternalSession)
	{
		// Отваряме сесия 
		hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	//Стартираме транзакция
	if (!m_bExternalSession)
	{
		hResult = m_oSession.StartTransaction();

		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при стартиране на транзакция."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	if ( !SelectWhereID( lID, true ) )
	{
		AbortSession();
		CloseConnection();

		return false;
	}

	//Проверка на UpdateCounter
	if ( m_recCity.lUpdateCounter != recCities.lUpdateCounter )
	{
		MessageBox( NULL, _T( "Записа е променен от друг потребител." ), NULL, MB_ICONHAND );

		AbortSession();
		CloseConnection();
		
		return false;
	}

	m_recCity = recCities;
	m_recCity.lUpdateCounter++;

	// Изпълняваме update командата
	hResult = SetData( AccessorOne );
	if ( FAILED( hResult ) )
	{
		MessageBox( NULL, _T( "Грешка при промяна на запис." ), NULL, MB_ICONHAND );
		
		AbortSession();
		CloseConnection();
		
		return false;
	}

	// Commit на транзакция
	if (!m_bExternalSession)
	{
		hResult = m_oSession.Commit();

		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при завършване на транзакция."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	CloseConnection();

	return true;
}

bool CCitiesTable::Insert( CITIES& recCity )
{
	HRESULT hResult = S_FALSE;
	if (!m_bExternalSession)
	{
		// Отваряме сесия 
		hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	// Конструираме заявката
	CString strQuery;
	strQuery.Format( SELECT_TOP_0_FROM_CITIES );

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet;
	InitializePropSetProperties( oUpdateDBPropSet );

	// Изпълняваме командата
	hResult = Open( m_oSession, strQuery, &oUpdateDBPropSet );
	if ( FAILED( hResult ) )
	{
		MessageBox( NULL, _T( "Грешка при изпълняване на заявка." ), NULL, MB_ICONHAND );

		CloseConnection();

		return false;
	}

	m_recCity = recCity;

	hResult = __super::Insert( AccessorOne, false );
	if ( FAILED( hResult ) )
	{
		MessageBox( NULL, _T( "Грешка при добавяне на запис" ), NULL, MB_ICONHAND );

		CloseConnection();

		return false;
	}

	hResult = MoveLast();
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка"), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}
	
	recCity.lID = m_recCity.lID;

	CloseConnection();

	return true;
}

bool CCitiesTable::DeleteWhereID( const long lID )
{
	HRESULT hResult = S_FALSE;
	if (!m_bExternalSession)
	{
		// Отваряме сесия 
		hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	if ( !SelectWhereID( lID, true ) )
	{
		CloseConnection();
		return false;
	}

	hResult = Delete();
	if( FAILED( hResult ) )
	{
		MessageBox( NULL, _T( "Грешка при изтриване на запис." ), NULL, MB_ICONHAND );

		CloseConnection();

		return false;
	}

	CloseConnection();

	return true;
}

void CCitiesTable::InitializePropSetProperties( CDBPropSet& oDBPropSet )
{
	oDBPropSet.SetGUID( DBPROPSET_ROWSET );
	oDBPropSet.AddProperty( DBPROP_CANFETCHBACKWARDS, true );
	oDBPropSet.AddProperty( DBPROP_IRowsetScroll, true );
	oDBPropSet.AddProperty( DBPROP_IRowsetChange, true );
	oDBPropSet.AddProperty( DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE );
}

void CCitiesTable::CloseConnection()
{
	Close();

	if (m_bExternalSession)
		return;

	m_oSession.Close();
}

bool CCitiesTable::SelectWhereID( const long lID, const bool bModifyRowset )
{
	// Конструираме заявката
	CString strQuery;
	HRESULT hResult = S_FALSE;

	if ( !bModifyRowset)
	{
		// Изпълняваме командата
		strQuery.Format( SELECT_ALL_FROM_CITIES_WHERE_ID, lID );
		hResult = Open( m_oSession, strQuery );
	}
	else
	{
		// Настройка на типа на Rowset-а
		CDBPropSet oUpdateDBPropSet;
		InitializePropSetProperties( oUpdateDBPropSet );

		// Изпълняваме командата
		strQuery.Format( SELECT_ALL_FROM_CITIES_WITH_UPDLOCK_WHERE_ID, lID );
		hResult = Open( m_oSession, strQuery, &oUpdateDBPropSet );
	}

	if ( FAILED( hResult ) )
	{
		MessageBox( NULL, _T( "Грешка при изпълняване на заявка." ), NULL, MB_ICONHAND );

		return false;
	}

	hResult = MoveFirst();
	if ( FAILED( hResult ) || hResult == DB_S_ENDOFROWSET )
	{
		MessageBox(NULL, _T("Грешка при отваряне на запис."), NULL, MB_ICONHAND);

		return false;
	}
	return true;
}

void CCitiesTable::AbortSession()
{
	if (!m_bExternalSession)
		m_oSession.Abort();
}

// Overrides
// ----------------

