#pragma once
#include "Structures.h"
/////////////////////////////////////////////////////////////////////////////
// CPersonWithNumbers
class CPersonWithNumbers : public CObject
{

	// Constants
	// ----------------



	// Constructor / Destructor
	// ----------------
public:
	//CPersonWithNumbers( PERSONS recPerson, CPhoneNumbersArray oPhoneNumbersArray );
	CPersonWithNumbers();
	virtual ~CPersonWithNumbers();


	// Methods
	// ----------------
public:
	PERSONS& GetPerson();
	CPhoneNumbersArray& GetNumbers();
	// Overrides
	// ----------------



	// Members
	// ----------------
private:
	PERSONS m_recPerson;
	CPhoneNumbersArray m_oPhoneNumbersArray;
};