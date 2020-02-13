
// Phonebook.h : main header file for the Phonebook application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPhonebookApp:
// See Phonebook.cpp for the implementation of this class
//

class CPhonebookApp : public CWinApp
{
public:
	CPhonebookApp() noexcept;

	void OpenPersonsDocument();
	void OpenCitiesDocument();
	void OpenPhoneTypesDocument();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPhonebookApp theApp;
