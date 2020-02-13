#include "stdafx.h"
#include "CitiesData.h"



// Constructor / Destructor
// ----------------

CCitiesData::CCitiesData()
{
}

CCitiesData::~CCitiesData()
{
}

// Methods
// ----------------

bool CCitiesData::SelectAll( CCitiesArray& oCitiesArray, CPersonsArray& oPersonsArray )
{
	CCitiesTable oCitiesTable;

	if( !oCitiesTable.SelectAll( oCitiesArray ) )
		return false;

	CPersonsTable oPersonsTable;

	if ( !oPersonsTable.SelectAll( oPersonsArray ) )
		return false;

	return true;
	
}
bool CCitiesData::SelectWhereID( const long lID, CITIES& recCities )
{
	CCitiesTable oCitiesTable;

	if( !oCitiesTable.SelectWhereID( lID, recCities) )
		return false;

	return true;
}

bool CCitiesData::UpdateWhereID( const long lID, const CITIES & recCities )
{
	CCitiesTable oCitiesTable;

	if ( !oCitiesTable.UpdateWhereID( lID, recCities ) )
		return false;

	return true;
}

bool CCitiesData::Insert( CITIES & recCities )
{
	CCitiesTable oCitiesTable;

	if ( !oCitiesTable.Insert( recCities ) )
		return false;

	return true;
}

bool CCitiesData::DeleteWhereID(const long lID)
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

	CPersonsTable oPersonsTable(oSession);

	CPersonsArray oPersonsArray;

	if ( !oPersonsTable.SelectWherePersonID( lID, oPersonsArray ) )
	{
		oSession.Abort();
		oSession.Close();
		return false;
	}

	for (int i = 0; i < oPersonsArray.GetCount(); i++)
	{
		CPhoneNumbersTable oPhoneNumbersTable(oSession);

		CPhoneNumbersArray oPhoneNumbersArray;

		if (!oPhoneNumbersTable.SelectWherePersonID(oPersonsArray.GetAt(i)->lID, oPhoneNumbersArray))
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}

		for (int i = 0; i < oPhoneNumbersArray.GetCount(); i++)
		{
			if (!oPhoneNumbersTable.DeleteWhereID(oPhoneNumbersArray.GetAt(i)->lID))
			{
				oSession.Abort();
				oSession.Close();
				return false;
			}
		}

		if (!oPersonsTable.DeleteWhereID( oPersonsArray.GetAt(i)->lID) )
		{
			oSession.Abort();
			oSession.Close();
			return false;
		}
	}

	CCitiesTable oCitiesTable(oSession);

	if ( !oCitiesTable.DeleteWhereID( lID ) )
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