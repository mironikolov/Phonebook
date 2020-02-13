// CCitiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Phonebook.h"
#include "CitiesDlg.h"
#include "afxdialogex.h"

// CCitiesDialog dialog

IMPLEMENT_DYNAMIC(CCitiesDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CCitiesDlg, CDialog)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------

CCitiesDlg::CCitiesDlg( CITIES& recCity, const DialogModes& eMode, CWnd* pParent )
	: CDialogEx(IDD_CITIES, pParent)
	, m_recCity( recCity )
	, m_eMode( eMode )
{
}

CCitiesDlg::~CCitiesDlg()
{
}

// MFC Overrides
// ----------------
void CCitiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_CITIES_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_CITIES_DISTRICT, m_edbDistrict);
	DDX_Control(pDX, IDOK, m_btnOK);
}

// MFC Message Handlers
// ----------------

// Methods
// ----------------

void CCitiesDlg::OnOK()
{
	if (!ValidateData())
		return;

	GetDialogText();

	CDialogEx::OnOK();
}

void CCitiesDlg::SetDialogText()
{
	CString strName = m_recCity.szName;
	strName.Trim();

	CString strDistrict = m_recCity.szDistrict;
	strDistrict.Trim();

	m_edbName.SetWindowTextW( strName );
	m_edbDistrict.SetWindowTextW( strDistrict );
}

void CCitiesDlg::GetDialogText( )
{
	CString strName;
	CString strDistrict;

	m_edbName.GetWindowTextW( strName );
	m_edbDistrict.GetWindowTextW( strDistrict );

	_tcscpy_s(m_recCity.szName, _countof(m_recCity.szName), strName);
	_tcscpy_s(m_recCity.szDistrict, _countof(m_recCity.szDistrict), strDistrict);
}

BOOL CCitiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDialogText();

	SetDialogMode();

	return TRUE;
}

bool CCitiesDlg::ValidateData()
{
	CString strName;
	m_edbName.GetWindowTextW( strName );
	strName.Trim();

	CString strDistrict = m_recCity.szDistrict;
	m_edbDistrict.GetWindowTextW( strDistrict );
	strDistrict.Trim();

	if ( strName.IsEmpty() )
	{
		MessageBox( _T( "Въведете име на град" ), NULL, MB_ICONWARNING );
		return false;
	}

	if ( strDistrict.IsEmpty() )
	{
		MessageBox( _T( "Въведете име на регион" ), NULL, MB_ICONWARNING );
		return false;
	}

	return true;
}

void CCitiesDlg::SetViewModeRestrictions()
{
	m_btnOK.EnableWindow(FALSE);
	m_edbName.SetReadOnly();
	m_edbDistrict.SetReadOnly();
}

void CCitiesDlg::SetDialogMode()
{
	SetLimitText();

	switch (m_eMode)
	{
	case DialogModeModify:
		SetWindowText(_T("Редакция на град"));
		break;

	case DialogModeView:
		SetWindowText(_T("Преглед на град"));
		SetViewModeRestrictions();
		break;

	case DialogModeInsert:
		SetWindowText(_T("Добавяне на град"));
		break;
	}
}

void CCitiesDlg::SetLimitText()
{
	m_edbName.SetLimitText(CITIES_NAME_SIZE);
	m_edbDistrict.SetLimitText(CITIES_DISTRICT_SIZE);
}