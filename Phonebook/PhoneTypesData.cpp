#include "stdafx.h"
#include "PhoneTypesData.h"



// Constructor / Destructor
// ----------------

CPhoneTypesData::CPhoneTypesData()
{
}

CPhoneTypesData::~CPhoneTypesData()
{
}

// Methods
// ----------------

bool CPhoneTypesData::SelectAll(CPhoneTypesArray& oPhoneTypesArray)
{
	CPhoneTypesTable oPhoneTypesTable;

	if (!oPhoneTypesTable.SelectAll(oPhoneTypesArray))
		return false;

	return true;

}
bool CPhoneTypesData::SelectWhereID(const long lID, PHONE_TYPES& recPhoneTypes)
{
	CPhoneTypesTable oPhoneTypesTable;

	if (!oPhoneTypesTable.SelectWhereID(lID, recPhoneTypes))
		return false;

	return true;
}

bool CPhoneTypesData::UpdateWhereID(const long lID, const PHONE_TYPES& recPhoneTypes)
{
	CPhoneTypesTable oPhoneTypesTable;

	if (!oPhoneTypesTable.UpdateWhereID(lID, recPhoneTypes))
		return false;

	return true;
}

bool CPhoneTypesData::Insert(PHONE_TYPES& recPhoneTypes)
{
	CPhoneTypesTable oPhoneTypesTable;

	if (!oPhoneTypesTable.Insert(recPhoneTypes))
		return false;

	return true;
}

bool CPhoneTypesData::DeleteWhereID(const long lID)
{
	CPhoneTypesTable oPhoneTypesTable;

	if (!oPhoneTypesTable.DeleteWhereID(lID))
		return false;

	return true;
}
// Overrides
// ----------------