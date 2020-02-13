// CPhoneTypesTable.cpp
//

#include "stdafx.h"
#include <atldbcli.h>
#include "PhoneTypesTable.h"
#include "DatabaseConnection.h"

#define SELECT_ALL_FROM_PHONE_TYPES _T( "SELECT * FROM PHONE_TYPES" )
#define SELECT_ALL_FROM_PHONE_TYPES_WHERE_ID _T( "SELECT * FROM PHONE_TYPES WHERE ID=%d" )
#define SELECT_ALL_FROM_PHONE_TYPES_WITH_UPDLOCK_WHERE_ID _T( "SELECT * FROM PHONE_TYPES WITH (UPDLOCK) WHERE ID=%d" )
#define SELECT_TOP_0_FROM_PHONE_TYPES _T( "SELECT TOP 0 * FROM PHONE_TYPES" )

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

// Constructor / Destructor
// ----------------

CPhoneTypesTable::CPhoneTypesTable()
{

}

CPhoneTypesTable::~CPhoneTypesTable()
{

}

// Methods
// ----------------

bool CPhoneTypesTable::SelectAll( CPhoneTypesArray& oPhoneTypesArray)
{
	// Отваряме сесия 
	HRESULT hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	// Конструираме заявката
	CString strQuery = SELECT_ALL_FROM_PHONE_TYPES;

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
		oPhoneTypesArray.Add(new PHONE_TYPES( m_recPhoneType ));
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

bool CPhoneTypesTable::SelectWhereID(const long lID, PHONE_TYPES& recPhoneType)
{
	// Отваряме сесия 
	HRESULT hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	if (!SelectWhereID(lID, false))
	{
		CloseConnection();
		return false;
	}

	recPhoneType = m_recPhoneType;

	// Затваряме командата и сесията. 
	CloseConnection();

	return true;
}

bool CPhoneTypesTable::UpdateWhereID(const long lID, const PHONE_TYPES& recPhoneType)
{
	// Отваряме сесия 
	HRESULT hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	//Стартираме транзакция
	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при стартиране на транзакция."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	if (!SelectWhereID(lID, true))
	{
		m_oSession.Abort();
		CloseConnection();

		return false;
	}

	//Проверка на UpdateCounter
	if (m_recPhoneType.lUpdateCounter != recPhoneType.lUpdateCounter)
	{
		MessageBox(NULL, _T("Записа е променен от друг потребител."), NULL, MB_ICONHAND);

		m_oSession.Abort();
		CloseConnection();

		return false;
	}

	m_recPhoneType = recPhoneType;
	m_recPhoneType.lUpdateCounter++;

	// Изпълняваме update командата
	hResult = SetData(AccessorOne);
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при промяна на запис."), NULL, MB_ICONHAND);

		m_oSession.Abort();
		CloseConnection();

		return false;
	}

	// Commit на транзакция
	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при завършване на транзакция."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	CloseConnection();

	return true;
}

bool CPhoneTypesTable::Insert(PHONE_TYPES& recPhoneType)
{
	// Отваряме сесия 
	HRESULT hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
	}

	// Конструираме заявката
	CString strQuery;
	strQuery.Format(SELECT_TOP_0_FROM_PHONE_TYPES);

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

	m_recPhoneType = recPhoneType;

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

	recPhoneType.lID = m_recPhoneType.lID;

	CloseConnection();

	return true;
}

bool CPhoneTypesTable::DeleteWhereID(const long lID)
{
	// Отваряме сесия 
	HRESULT hResult = m_oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);

		CloseConnection();

		return false;
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

void CPhoneTypesTable::InitializePropSetProperties(CDBPropSet& oDBPropSet)
{
	oDBPropSet.SetGUID(DBPROPSET_ROWSET);
	oDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

void CPhoneTypesTable::CloseConnection()
{
	Close();
	m_oSession.Close();
}

bool CPhoneTypesTable::SelectWhereID(const long lID, bool bModifyRowset)
{
	// Конструираме заявката
	CString strQuery;
	HRESULT hResult = S_FALSE;

	if (!bModifyRowset)
	{
		// Изпълняваме командата
		strQuery.Format(SELECT_ALL_FROM_PHONE_TYPES_WHERE_ID, lID);
		hResult = Open(m_oSession, strQuery);
	}
	else
	{
		// Настройка на типа на Rowset-а
		CDBPropSet oUpdateDBPropSet;
		InitializePropSetProperties(oUpdateDBPropSet);

		// Изпълняваме командата
		strQuery.Format(SELECT_ALL_FROM_PHONE_TYPES_WITH_UPDLOCK_WHERE_ID, lID);
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

// Overrides
// ----------------