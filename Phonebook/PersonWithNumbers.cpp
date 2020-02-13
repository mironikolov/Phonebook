#include "stdafx.h"
#include "PersonWithNumbers.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonWithNumbers

// Constructor / Destructor
// ----------------

//CPersonWithNumbers::CPersonWithNumbers( PERSONS recPerson, CPhoneNumbersArray& oPhoneNumbersArray )
//	:m_recPerson( recPerson )
//	,m_oPhoneNumbersArray( oPhoneNumbersArray )
//{
//}

CPersonWithNumbers::CPersonWithNumbers()
{
}

CPersonWithNumbers::~CPersonWithNumbers()
{
}

// Methods
// ----------------
PERSONS& CPersonWithNumbers::GetPerson()
{
	return m_recPerson;
}

CPhoneNumbersArray & CPersonWithNumbers::GetNumbers()
{
	return m_oPhoneNumbersArray;
}

// Overrides
// ----------------