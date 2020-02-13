#pragma once

#include "Structures.h"
#include "Enums.h"
// CCitiesDialog dialog

class CCitiesDlg : public CDialogEx
{	
	// Macros
	// ----------------
	DECLARE_DYNAMIC(CCitiesDlg)
	DECLARE_MESSAGE_MAP()

	// Constants
	// ----------------
private:
	enum { IDD = IDD_CITIES};

	// Constructor / Destructor
	// ----------------
public:
	CCitiesDlg ( CITIES& recCity, const DialogModes& eMode, CWnd* pParent = nullptr );
	virtual ~CCitiesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CITIES };
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
	CITIES& m_recCity;
	const DialogModes& m_eMode;

private:
	CEdit m_edbName;
	CEdit m_edbDistrict;
	CButton m_btnOK;
};

