// CPhoneNumbersTable.cpp
//

#include "stdafx.h"
#include <atldbcli.h>
#include "PhoneNumbersTable.h"
#include "DatabaseConnection.h"

#define SELECT_ALL_FROM_PHONE_NUMBERS _T( "SELECT * FROM PHONE_NUMBERS" )
#define SELECT_ALL_FROM_PHONE_NUMBERS_WHERE_ID _T( "SELECT * FROM PHONE_NUMBERS WHERE ID=%d" )
#define SELECT_ALL_FROM_PHONE_NUMBERS_WITH_UPDLOCK_WHERE_ID _T( "SELECT * FROM PHONE_NUMBERS WITH (UPDLOCK) WHERE ID=%d" )
#define SELECT_TOP_0_FROM_PHONE_NUMBERS _T( "SELECT TOP 0 * FROM PHONE_NUMBERS" )
#define SELECT_ALL_FROM_PHONE_NUMBERS_WHERE_PERSON_ID _T( "SELECT * FROM PHONE_NUMBERS WHERE PERSON_ID=%d" )

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

// Constructor / Destructor
// ----------------

CPhoneNumbersTable::CPhoneNumbersTable()
{

}

CPhoneNumbersTable::CPhoneNumbersTable( CSession& oSession )
{
	m_oSession = oSession;
	m_bExternalSession = true;
}

CPhoneNumbersTable::~CPhoneNumbersTable()
{

}

// Methods
// ----------------

bool CPhoneNumbersTable::SelectAll( CPhoneNumbersArray& oPhoneNumbersArray )
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
	CString strQuery = SELECT_ALL_FROM_PHONE_NUMBERS;

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
		oPhoneNumbersArray.Add( new PHONE_NUMBERS( m_recPhoneNumber ) );
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

bool CPhoneNumbersTable::SelectWhereID(const long lID, PHONE_NUMBERS& recPhoneNumber)
{
	HRESULT hResult = S_FALSE;
	if ( !m_bExternalSession)
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

	recPhoneNumber = m_recPhoneNumber;

	// Затваряме командата и сесията. 
	CloseConnection();

	return true;
}

bool CPhoneNumbersTable::UpdateWhereID(const long lID, const PHONE_NUMBERS& recPhoneNumber)
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

	if (!SelectWhereID(lID, true))
	{
		AbortSession();
		CloseConnection();

		return false;
	}

	//Проверка на UpdateCounter
	if (m_recPhoneNumber.lUpdateCounter != recPhoneNumber.lUpdateCounter)
	{
		MessageBox(NULL, _T("Записа е променен от друг потребител."), NULL, MB_ICONHAND);

		AbortSession();
		CloseConnection();

		return false;
	}

	m_recPhoneNumber = recPhoneNumber;
	m_recPhoneNumber.lUpdateCounter++;

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

bool CPhoneNumbersTable::Insert(PHONE_NUMBERS& recPhoneNumber)
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
	strQuery.Format(SELECT_TOP_0_FROM_PHONE_NUMBERS);

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

	m_recPhoneNumber = recPhoneNumber;

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

	recPhoneNumber.lID = m_recPhoneNumber.lID;

	CloseConnection();

	return true;
}

bool CPhoneNumbersTable::DeleteWhereID( const long lID )
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

void CPhoneNumbersTable::InitializePropSetProperties(CDBPropSet& oDBPropSet)
{
	oDBPropSet.SetGUID(DBPROPSET_ROWSET);
	oDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

void CPhoneNumbersTable::CloseConnection()
{
	Close();
	if (m_bExternalSession)
	{
		return;
	}
	m_oSession.Close();
}

bool CPhoneNumbersTable::SelectWhereID(const long lID, bool bModifyRowset)
{
	// Конструираме заявката
	CString strQuery;

	HRESULT hResult = S_FALSE;

	if (!bModifyRowset)
	{
		// Изпълняваме командата
		strQuery.Format(SELECT_ALL_FROM_PHONE_NUMBERS_WHERE_ID, lID);
		hResult = Open(m_oSession, strQuery);
	}
	else
	{
		// Настройка на типа на Rowset-а
		CDBPropSet oUpdateDBPropSet;
		InitializePropSetProperties(oUpdateDBPropSet);

		// Изпълняваме командата
		strQuery.Format(SELECT_ALL_FROM_PHONE_NUMBERS_WITH_UPDLOCK_WHERE_ID, lID);
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

void CPhoneNumbersTable::AbortSession()
{
	if (!m_bExternalSession)
		m_oSession.Abort();
}

bool CPhoneNumbersTable::SelectWherePersonID(const long lID, CPhoneNumbersArray& oPhoneNumberArray)
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
	strQuery.Format(SELECT_ALL_FROM_PHONE_NUMBERS_WHERE_PERSON_ID, lID);
	hResult = Open(m_oSession, strQuery);

	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при изпълняване на заявка."), NULL, MB_ICONHAND);

		return false;
	}

	// Прочитаме всички данни
	while ((hResult = MoveNext()) == S_OK)
	{
		oPhoneNumberArray.Add( new PHONE_NUMBERS( m_recPhoneNumber ) );
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