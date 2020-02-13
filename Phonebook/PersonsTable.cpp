// CPersonsTable.cpp
//

#include "stdafx.h"
#include <atldbcli.h>
#include "PersonsTable.h"
#include "DatabaseConnection.h"

#define SELECT_ALL_FROM_PERSONS _T( "SELECT * FROM PERSONS" )
#define SELECT_ALL_FROM_PERSONS_WHERE_ID _T( "SELECT * FROM PERSONS WHERE ID=%d" )
#define SELECT_ALL_FROM_PERSONS_WITH_UPDLOCK_WHERE_ID _T( "SELECT * FROM PERSONS WITH (UPDLOCK) WHERE ID=%d" )
#define SELECT_TOP_0_FROM_PERSONS _T( "SELECT TOP 0 * FROM PERSONS" )
#define SELECT_ALL_FROM_PERSONS_WHERE_CITY_ID _T( "SELECT * FROM PERSONS WHERE CITY_ID=%d" )


/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

// Constructor / Destructor
// ----------------

CPersonsTable::CPersonsTable()
{
}

CPersonsTable::CPersonsTable(CSession & oSession)
{
	m_oSession = oSession;
	m_bExternalSession = true;
}

CPersonsTable::~CPersonsTable()
{

}

// Methods
// ----------------

bool CPersonsTable::SelectAll( CPersonsArray& oPersonsArray )
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
	CString strQuery = SELECT_ALL_FROM_PERSONS;

	// Изпълняваме командата
	hResult = Open(m_oSession, strQuery);
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при изпълняване на заявка."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	// Прочитаме всички данни
	while ((hResult = MoveNext()) == S_OK)
	{
		oPersonsArray.Add( new PERSONS( m_recPerson ) );
	}

	if (hResult != DB_S_ENDOFROWSET)
	{
		MessageBox(NULL, _T("Грешка при прочитане на цялата таблица"), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	// Затваряме командата, сесията и връзката с базата данни. 
	CloseConnection();

	return true;
}

bool CPersonsTable::SelectWhereID(const long lID, PERSONS& recPerson)
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

	if (!SelectWhereID(lID, false))
	{
		CloseConnection();
		return false;
	}

	recPerson = m_recPerson;

	// Затваряме командата и сесията. 
	CloseConnection();

	return true;
}

bool CPersonsTable::UpdateWhereID(const long lID, const PERSONS& recPerson)
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

		hResult = m_oSession.StartTransaction();

		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Грешка при стартиране на транзакция."), NULL, MB_ICONHAND);

			CloseConnection();

			return false;
		}
	}

	if (!SelectWhereID(lID, true))
	{
		AbortSession();
		CloseConnection();

		return false;
	}

	//Проверка на UpdateCounter
	if (m_recPerson.lUpdateCounter != recPerson.lUpdateCounter)
	{
		MessageBox(NULL, _T("Записа е променен от друг потребител."), NULL, MB_ICONHAND);

		AbortSession();
		CloseConnection();

		return false;
	}

	m_recPerson = recPerson;
	m_recPerson.lUpdateCounter++;

	// Изпълняваме update командата
	hResult = SetData(AccessorOne);
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при промяна на запис."), NULL, MB_ICONHAND);

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

bool CPersonsTable::Insert(PERSONS& recPerson)
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
	strQuery.Format(SELECT_TOP_0_FROM_PERSONS);

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet;
	InitializePropSetProperties(oUpdateDBPropSet);

	// Изпълняваме командата
	hResult = Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при изпълняване на заявка."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	m_recPerson = recPerson;

	hResult = __super::Insert(AccessorOne, false);
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при добавяне на запис"), NULL, MB_ICONHAND);

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

	recPerson.lID = m_recPerson.lID;

	CloseConnection();

	return true;
}

bool CPersonsTable::DeleteWhereID(const long lID)
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

	if (!SelectWhereID(lID, true))
	{
		CloseConnection();
		return false;
	}

	hResult = Delete();
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при изтриване на запис."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	CloseConnection();

	return true;
}

void CPersonsTable::InitializePropSetProperties(CDBPropSet& oDBPropSet)
{
	oDBPropSet.SetGUID(DBPROPSET_ROWSET);
	oDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

void CPersonsTable::CloseConnection()
{
	Close();

	if (m_bExternalSession)
		return;

	m_oSession.Close();
}

bool CPersonsTable::SelectWhereID(const long lID, bool bModifyRowset)
{
	// Конструираме заявката
	CString strQuery;
	HRESULT hResult = S_FALSE;

	if (!bModifyRowset)
	{
		// Изпълняваме командата
		strQuery.Format(SELECT_ALL_FROM_PERSONS_WHERE_ID, lID);
		hResult = Open(m_oSession, strQuery);
	}
	else
	{
		// Настройка на типа на Rowset-а
		CDBPropSet oUpdateDBPropSet;
		InitializePropSetProperties(oUpdateDBPropSet);

		// Изпълняваме командата
		strQuery.Format(SELECT_ALL_FROM_PERSONS_WITH_UPDLOCK_WHERE_ID, lID);
		hResult = Open(m_oSession, strQuery, &oUpdateDBPropSet);
	}

	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при изпълняване на заявка."), NULL, MB_ICONHAND);

		return false;
	}

	hResult = MoveFirst();
	if (FAILED(hResult) || hResult == DB_S_ENDOFROWSET)
	{
		MessageBox(NULL, _T("Грешка при отваряне на запис."), NULL, MB_ICONHAND);

		return false;
	}
	return true;
}

void CPersonsTable::AbortSession()
{
	if (!m_bExternalSession)
		m_oSession.Abort();
}

bool CPersonsTable::SelectWherePersonID(const long lID, CPersonsArray& oPersonsArray)
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

	// Изпълняваме командата
	strQuery.Format(SELECT_ALL_FROM_PERSONS_WHERE_CITY_ID, lID);
	hResult = Open(m_oSession, strQuery);

	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при изпълняване на заявка."), NULL, MB_ICONHAND);

		return false;
	}

	// Прочитаме всички данни
	while ((hResult = MoveNext()) == S_OK)
	{
		oPersonsArray.Add( new PERSONS( m_recPerson ) );
	}

	if (hResult != DB_S_ENDOFROWSET)
	{
		MessageBox(NULL, _T("Грешка при прочитане на цялата таблица"), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	// Затваряме командата и сесията. 
	CloseConnection();

	return true;
}

// Overrides
// ----------------