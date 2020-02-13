#pragma once

#include <atldbcli.h>

class CDatabaseConnection
{
// Constants
// ----------------

// Constructor / Destructor
// ----------------
private:
	CDatabaseConnection();

	CDatabaseConnection(const CDatabaseConnection& oDatabaseConnection);

	CDatabaseConnection& operator=(const CDatabaseConnection& oDatabaseConnection);

	~CDatabaseConnection();

	// Methods
	// ----------------
public:
	static CDatabaseConnection* GetInstance();

	///<summary> Отваря връзка с базата </summary>
	///<returns> Връща true при успех, в противен случай false </returns>
	bool Connect();

	CDataSource& GetDataSource();

	///<summary> Затваря връзка с базата </summary>
	void Close();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	static CDatabaseConnection* _pInstance;

	CDataSource m_oDataSource;

	bool m_bInitialized = false;
};