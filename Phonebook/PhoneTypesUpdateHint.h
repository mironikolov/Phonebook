#pragma once
#include <afx.h>
#include "Structures.h"
#include "Enums.h"

class CPhoneTypesUpdateHint : public CObject
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesUpdateHint(const PHONE_TYPES& recPhoneTypes);
	virtual ~CPhoneTypesUpdateHint();

	// Methods
	// ----------------
public:
	const PHONE_TYPES& GetPhoneType();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	const PHONE_TYPES& m_recPhoneType;
};