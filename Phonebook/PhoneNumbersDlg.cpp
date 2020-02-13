// PhoneNumbersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Phonebook.h"
#include "PhoneNumbersDlg.h"
#include "afxdialogex.h"

// CPhoneNumbersDlg dialog

IMPLEMENT_DYNAMIC(CPhoneNumbersDlg, CDialogEx)

// Constructor / Destructor
// ----------------
CPhoneNumbersDlg::CPhoneNumbersDlg( PHONE_NUMBERS& recPhoneNumber, const CPhoneTypesMap& oPhoneTypesMap,
									const DialogModes& eMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHONE_NUMBERS, pParent)
	,m_recPhoneNumber(recPhoneNumber)
	,m_eMode( eMode )
	, m_oPhoneTypesMap( oPhoneTypesMap )
{

}

CPhoneNumbersDlg::~CPhoneNumbersDlg()
{
}

// Methods
// ----------------
void CPhoneNumbersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_PHONE_NUMBER, m_edbPhoneNumber);
	DDX_Control(pDX, IDC_CMB_PHONE_TYPE, m_cmbPhoneType);
	DDX_Control(pDX, IDOK, m_btnOK);
}

void CPhoneNumbersDlg::SetDialogText()
{
	POSITION pos = m_oPhoneTypesMap.GetStartPosition();
	while (pos != NULL)
	{
		long lKey;
		PHONE_TYPES* pPhoneType = nullptr;
		m_oPhoneTypesMap.GetNextAssoc(pos, lKey, pPhoneType);

		int pos = m_cmbPhoneType.AddString(pPhoneType->szType);
		m_cmbPhoneType.SetItemData(pos, pPhoneType->lID);

		if (m_recPhoneNumber.lPhoneTypeID == pPhoneType->lID)
			m_cmbPhoneType.SetCurSel(pos);
	}

	CString strPhoneNumber = m_recPhoneNumber.szNumber;
	strPhoneNumber.Trim();
	m_edbPhoneNumber.SetWindowTextW(strPhoneNumber);

}

void CPhoneNumbersDlg::GetDialogText()
{
	CString strPhoneNumber;

	m_edbPhoneNumber.GetWindowTextW( strPhoneNumber );

	_tcscpy_s( m_recPhoneNumber.szNumber, _countof( m_recPhoneNumber.szNumber ), strPhoneNumber);

	int pos = m_cmbPhoneType.GetCurSel();
	m_recPhoneNumber.lPhoneTypeID = (long)m_cmbPhoneType.GetItemData( pos );

}

bool CPhoneNumbersDlg::ValidateData()
{
	CString strPhoneNumber;
	m_edbPhoneNumber.GetWindowTextW(strPhoneNumber);
	strPhoneNumber.Trim();

	if ( strPhoneNumber.IsEmpty() )
	{
		MessageBox( _T("Въведете телефонен номер"), NULL, MB_ICONWARNING );
		return false;
	}

	int nIndex = m_cmbPhoneType.GetCurSel();
	if (nIndex == -1)
	{
		MessageBox(_T("Изберете тип телефон"), NULL, MB_ICONWARNING);
		return false;
	}

	return true;
}

void CPhoneNumbersDlg::SetViewModeRestrictions()
{
	m_edbPhoneNumber.EnableWindow( FALSE );
	m_cmbPhoneType.EnableWindow( FALSE );
	m_btnOK.EnableWindow( FALSE );
}

void CPhoneNumbersDlg::SetDialogMode()
{
	switch (m_eMode)
	{
	case DialogModeModify:
		SetWindowText(_T("Редакция на номер"));
		break;

	case DialogModeView:
		SetWindowText(_T("Преглед на номер"));
		SetViewModeRestrictions();
		break;

	case DialogModeInsert:
		SetWindowText(_T("Добавяне на номер"));
		break;
	}
}

void CPhoneNumbersDlg::SetLimitText()
{
	m_edbPhoneNumber.SetLimitText(PHONE_NUMBERS_NUMBER_SIZE);
}

// Overrides
// ----------------
BOOL CPhoneNumbersDlg::OnInitDialog()//TODO
{
	CDialogEx::OnInitDialog();
	
	SetDialogMode();

	SetDialogText();

	SetLimitText();

	return 0;
}

void CPhoneNumbersDlg::OnOK()
{
	if (!ValidateData())
		return;

	GetDialogText();

	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(CPhoneNumbersDlg, CDialogEx)
END_MESSAGE_MAP()


// CPhoneNumbersDlg message handlers
