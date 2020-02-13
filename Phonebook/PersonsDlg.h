#pragma once
#include "Structures.h"
#include "PersonWithNumbers.h"
#include <tchar.h>
#include "PhoneNumbersDlg.h"
#include "Enums.h"

// CPersonsDlg dialog

class CPersonsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonsDlg)

	// Constructor / Destructor
	// ----------------
public:
	CPersonsDlg( CPersonWithNumbers& oPersonWithNumbers,
		const CCitiesMap& m_oCitiesMap,
		const CPhoneTypesMap& oPhoneTypesMap,
		const DialogModes& eMode, CWnd* pParent = nullptr );
	virtual ~CPersonsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSONS };
#endif


	// Overrides
	// ----------------
public:
	virtual BOOL OnInitDialog() override;

	virtual void OnOK() override;

	// Methods
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void SetDialogText();

	void GetDialogText();

	bool ValidateData();

	void SetViewModeRestrictions();

	void SetDialogMode();

	void SetLimitText();

	void SetPhoneNumbersListControl();

	void SetItemText( const PHONE_NUMBERS& recPhoneNumber, DialogModes eModes, int nArrayIndex);

	int GetItemIndex( int nArrayIndex );

	void ModifyPhoneNumbers();

	void ViewPhoneNumbers();

	int GetSelectedPhoneNumberArrayIndex();

	void AddPhoneNumber();

	void DeletePhoneNumber();

	int GetSelectedItemIndex();

	void FillComboBox( const PERSONS& recPerson );

	// Members
	// ----------------
private:
	CPersonWithNumbers& m_oPersonWithNumbers;
	const DialogModes& m_eMode;
	const CCitiesMap& m_oCitiesMap;
	const CPhoneTypesMap& m_oPhoneTypesMap;

	CEdit m_edbName;
	CEdit m_edbSurname;
	CEdit m_edbLastName;
	CEdit m_edbUCN;

	CComboBox m_cmbCity;
	CEdit m_edbAddress;
	CListCtrl m_lscNumbers;
	CButton m_btnOK;

	CButton m_btnPersonsNumberAdd;
	CButton m_btnPersonsNumberDel;

public:
	afx_msg void OnClickedBtnPersonsNumberAdd();
	afx_msg void OnClickedBtnPersonsNumberDelete();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnItemactivateLscPersonsNumbers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickLscPersonsNumbers(NMHDR *pNMHDR, LRESULT *pResult);
};
