#include "stdafx.h"
#include "DatabaseConnection.h"

CDatabaseConnection* CDatabaseConnection::_pInstance = 0;

// Constructor / Destructor
// ----------------

CDatabaseConnection::CDatabaseConnection()
{
}

CDatabaseConnection& CDatabaseConnection::operator=(const CDatabaseConnection & oDatabaseConnection)
{
	if (this != &oDatabaseConnection)
	{
	}
	return *this;
}

CDatabaseConnection::~CDatabaseConnection()
{
	Close();
	
	delete _pInstance;
}

// Methods
// ----------------
CDatabaseConnection* CDatabaseConnection::GetInstance()
{
	if (!_pInstance)
	{
		_pInstance = new CDatabaseConnection;
	}
	return _pInstance;
}

bool CDatabaseConnection::Connect()
{
	if ( !m_bInitialized )
	{
		CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
		oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("DESKTOP-KMRV1H7\\CS2019"));
		oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
		oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("PhonebookDB"));
		oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
		oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
		oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

		//Свързваме се към базата данни
		HRESULT hResult = m_oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
		if (FAILED(hResult))
		{
			MessageBox(NULL, _T("Неуспешно свързване с базата данни"), NULL, MB_ICONHAND);
			return false;
		}

		m_bInitialized = true;
	}
	
	return true;
}

CDataSource& CDatabaseConnection::GetDataSource()
{
	return m_oDataSource;
}

void CDatabaseConnection::Close()
{
	m_oDataSource.Close();
}

// Overrides
// ----------------