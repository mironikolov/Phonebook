#pragma once
#include "Structures.h"
#include "Enums.h"

// CPhoneTypesDlg dialog

class CPhoneTypesDlg : public CDialogEx
{
	// Macros
	// ----------------
	DECLARE_DYNAMIC(CPhoneTypesDlg)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesDlg( PHONE_TYPES& recPhoneType, const DialogModes& eMode, CWnd* pParent = nullptr );   // standard constructor
	virtual ~CPhoneTypesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_TYPES };
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
	PHONE_TYPES& m_recPhoneType;
	const DialogModes& m_eMode;

private:
	CEdit m_edbType;
	CButton m_btnOK;
};

