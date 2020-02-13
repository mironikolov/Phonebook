#include "stdafx.h"
#include "PersonsData.h"



// Constructor / Destructor
// ----------------

CPersonsData::CPersonsData()
{
}

CPersonsData::~CPersonsData()
{
}

// Methods
// ----------------

bool CPersonsData::SelectAll( CPersonsArray& oPersonsArray, CCitiesArray& oCitiesArray, CPhoneTypesArray& oPhoneTypesArray )
{
	CPersonsTable oPersonsTable;
	if (!oPersonsTable.SelectAll( oPersonsArray ))
		return false;

	CCitiesTable oCitiesTable;
	if ( !oCitiesTable.SelectAll( oCitiesArray ) )
		return false;

	CPhoneTypesTable oPhoneTypesTable;
	if ( !oPhoneTypesTable.SelectAll( oPhoneTypesArray ) )
		return false;

	return true;

}

bool CPersonsData::SelectWhereID( const long lID, CPersonWithNumbers& oPersonWithNumbers )
{
	CPersonsTable oPersonsTable;

	if ( !oPersonsTable.SelectWhereID( lID, oPersonWithNumbers.GetPerson() ) )
		return false;

	CPhoneNumbersTable oPhoneNumbersTable;

	if ( !oPhoneNumbersTable.SelectWherePersonID( lID, oPersonWithNumbers.GetNumbers() ) )
		return false;

	return true;
}

bool CPersonsData::UpdateWhereID( const long lID, CPersonWithNumbers& oPersonWithNumbers )
{
	CSession oSession;
	HRESULT hResult = oSession.Open( CDatabaseConnection::GetInstance()->GetDataSource() );
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);
		return false;
	}

	hResult = oSession.StartTransaction();
	if ( FAILED( hResult ) )
	{
		MessageBox(NULL, _T("Грешка при стартиране на транзакция."), NULL, MB_ICONHAND);

		oSession.Close();

		return false;
	}

	CPhoneNumbersTable oPhoneNumbersTable( oSession );

	CPhoneNumbersArray& oPhoneNumbersArray = oPersonWithNumbers.GetNumbers();

	CPhoneNumbersArray oPhoneNumbersArrayDB;
	if (!oPhoneNumbersTable.SelectWherePersonID(oPersonWithNumbers.GetPerson().lID, oPhoneNumbersArrayDB))
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	CPhoneNumbersMap oPhoneNumbersMap;
	for (int i = 0; i < oPhoneNumbersArrayDB.GetCount(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = new PHONE_NUMBERS( *oPhoneNumbersArrayDB.GetAt(i) );
		oPhoneNumbersMap.SetAt(pPhoneNumber->lID, pPhoneNumber);
	}

	for (int i = 0; i < oPhoneNumbersArray.GetCount(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oPhoneNumbersArray.GetAt(i);
		if (!pPhoneNumber)
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}

		if (pPhoneNumber->lID == 0)
		{
			pPhoneNumber->lPersonID = lID;
			if( !oPhoneNumbersTable.Insert(*pPhoneNumber) )
			{
				oSession.Abort();
				oSession.Close();
				return false;
			}
			continue;
		}
		
		PHONE_NUMBERS* pPhoneNumberDB = nullptr;
		if ( oPhoneNumbersMap.Lookup(pPhoneNumber->lID, pPhoneNumberDB ) && pPhoneNumberDB )
		{
			if ( memcmp( pPhoneNumber, pPhoneNumberDB, sizeof( PHONE_NUMBERS ) ) != 0 )
			{
				if (!oPhoneNumbersTable.UpdateWhereID(pPhoneNumber->lID, *pPhoneNumber))
				{
					oSession.Abort();
					oSession.Close();
					return false;
				}
			}
			oPhoneNumbersMap.RemoveKey( pPhoneNumber->lID );
		}
	}

	POSITION pos = oPhoneNumbersMap.GetStartPosition();
	while (pos != NULL)
	{
		long lID = 0;
		PHONE_NUMBERS* recPhoneNumber = nullptr;
		oPhoneNumbersMap.GetNextAssoc(pos, lID, recPhoneNumber);

		if ( !oPhoneNumbersTable.DeleteWhereID( lID ) )
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}
	}

	CPersonsTable oPersonsTable( oSession );

	if ( !oPersonsTable.UpdateWhereID( oPersonWithNumbers.GetPerson().lID, oPersonWithNumbers.GetPerson() ) )
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	oSession.Commit();
	oSession.Close();
	return true;
}

bool CPersonsData::Insert( CPersonWithNumbers& oPersonWithNumbers )
{
	CSession oSession;

	HRESULT hResult = oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);
		return false;
	}

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при стартиране на транзакция."), NULL, MB_ICONHAND);
		return false;
	}

	CPersonsTable oPersonsTable( oSession );
	if ( !oPersonsTable.Insert( oPersonWithNumbers.GetPerson() ) )
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	CPhoneNumbersTable oPhoneNumbersTable( oSession );

	CPhoneNumbersArray& oPhoneNumbersArray = oPersonWithNumbers.GetNumbers();
	for (int i = 0; i < oPhoneNumbersArray.GetCount(); i++)
	{
		PHONE_NUMBERS& recPhoneNumber = *oPhoneNumbersArray.GetAt(i);

		recPhoneNumber.lPersonID = oPersonWithNumbers.GetPerson().lID;

		if (!oPhoneNumbersTable.Insert(recPhoneNumber))
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}	
	}

	oSession.Commit();
	oSession.Close();

	return true;
}

bool CPersonsData::DeleteWhereID(const long lID)
{
	CSession oSession;
	HRESULT hResult = oSession.Open(CDatabaseConnection::GetInstance()->GetDataSource());
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при отваряне на сесия."), NULL, MB_ICONHAND);
		return false;
	}

	hResult = oSession.StartTransaction();
	if (FAILED(hResult))
	{
		MessageBox(NULL, _T("Грешка при стартиране на транзакция."), NULL, MB_ICONHAND);
		return false;
	}

	CPhoneNumbersTable oPhoneNumbersTable( oSession );

	CPhoneNumbersArray oPhoneNumbersArray;

	if ( !oPhoneNumbersTable.SelectWherePersonID( lID, oPhoneNumbersArray ) )
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	for (int i = 0; i < oPhoneNumbersArray.GetCount(); i++)
	{
		if ( !oPhoneNumbersTable.DeleteWhereID( oPhoneNumbersArray.GetAt(i)->lID ) )
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}
	}

	CPersonsTable oPersonsTable( oSession );

	if ( !oPersonsTable.DeleteWhereID( lID ) )
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	oSession.Commit();
	oSession.Close();

	return true;
}

// Overrides
// ----------------