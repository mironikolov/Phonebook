#pragma once
#include <afx.h>
#include "Structures.h"
#include "Enums.h"

class CUpdateHint : public CObject
{
	// Constants
	// ----------------

	// Constructor / Destructor
	// ----------------
public:
	CUpdateHint( const CITIES& recCity );
	virtual ~CUpdateHint();

	// Methods
	// ----------------
public:
	const CITIES& GetCity();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	const CITIES& m_recCity;
};