#include "stdafx.h"
#include "PhoneTypesUpdateHint.h"


/////////////////////////////////////////////////////////////////////////////
// CSampleClass

// Constructor / Destructor
// ----------------

CPhoneTypesUpdateHint::CPhoneTypesUpdateHint(const PHONE_TYPES& recPhoneType)
	: m_recPhoneType( recPhoneType )
{
}

CPhoneTypesUpdateHint::~CPhoneTypesUpdateHint()
{
}

// Methods
// ----------------
const PHONE_TYPES& CPhoneTypesUpdateHint::GetPhoneType()
{
	return m_recPhoneType;
}


// Overrides
// ----------------