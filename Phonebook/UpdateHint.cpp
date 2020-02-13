#include "stdafx.h"
#include "UpdateHint.h"


/////////////////////////////////////////////////////////////////////////////
// CSampleClass

// Constructor / Destructor
// ----------------

CUpdateHint::CUpdateHint( const CITIES& recCity )
	: m_recCity( recCity )
{
}

CUpdateHint::~CUpdateHint()
{
}

// Methods
// ----------------
const CITIES& CUpdateHint::GetCity()
{
	return m_recCity;
}


// Overrides
// ----------------