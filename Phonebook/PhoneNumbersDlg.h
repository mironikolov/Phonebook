#pragma once

#include "Structures.h"
#include "Enums.h"
// CPhoneNumbersDlg dialog

class CPhoneNumbersDlg : public CDialogEx
{
	// Macros
	// ----------------
	DECLARE_DYNAMIC(CPhoneNumbersDlg)
	DECLARE_MESSAGE_MAP()

	// Constants
	// ----------------
private:
	enum { IDD = IDD_PHONE_NUMBERS };

	// Constructor / Destructor
	// ----------------
public:
	CPhoneNumbersDlg( PHONE_NUMBERS& recPhoneNumber, const CPhoneTypesMap& oPhoneTypesMap
					, const DialogModes& eMode, CWnd* pParent = nullptr);
	virtual ~CPhoneNumbersDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_NUMBERS };
#endif

	// MFC Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Overrides
	// ----------------
public:
	virtual BOOL OnInitDialog() override;

	virtual void OnOK() override;

	// Methods
	// ----------------

private:
	void SetDialogText();
	void GetDialogText();
	bool ValidateData();
	void SetViewModeRestrictions();
	void SetDialogMode();
	void SetLimitText();

	// Members
	// ----------------
private:
	PHONE_NUMBERS& m_recPhoneNumber;
	const DialogModes& m_eMode;
	const CPhoneTypesMap& m_oPhoneTypesMap;
private:
	CEdit m_edbPhoneNumber;
	CComboBox m_cmbPhoneType;
	CButton m_btnOK;
};
