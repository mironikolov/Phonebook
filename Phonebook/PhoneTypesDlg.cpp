// PhoneTypesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Phonebook.h"
#include "PhoneTypesDlg.h"
#include "afxdialogex.h"


// CPhoneTypesDlg dialog

IMPLEMENT_DYNAMIC(CPhoneTypesDlg, CDialogEx)

CPhoneTypesDlg::CPhoneTypesDlg( PHONE_TYPES& recPhoneType, const DialogModes& eMode, CWnd* pParent )
	: CDialogEx(IDD_PHONE_TYPES, pParent)
	, m_recPhoneType(recPhoneType)
	, m_eMode(eMode)
{

}

CPhoneTypesDlg::~CPhoneTypesDlg()
{
}

void CPhoneTypesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_PHONE_TYPES_TYPE, m_edbType);
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CPhoneTypesDlg, CDialogEx)
END_MESSAGE_MAP()


// CPhoneTypesDlg message handlers

// Methods
// ----------------

void CPhoneTypesDlg::OnOK()
{
	if (!ValidateData())
		return;

	GetDialogText();

	CDialogEx::OnOK();
}

void CPhoneTypesDlg::SetDialogText()
{
	CString strType = m_recPhoneType.szType;
	strType.Trim();

	m_edbType.SetWindowTextW(strType);
}

void CPhoneTypesDlg::GetDialogText()
{
	CString strType;

	m_edbType.GetWindowTextW(strType);

	_tcscpy_s(m_recPhoneType.szType, _countof(m_recPhoneType.szType), strType);
}

BOOL CPhoneTypesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDialogText();

	SetDialogMode();

	return TRUE;
}

bool CPhoneTypesDlg::ValidateData()
{
	CString strType;
	m_edbType.GetWindowTextW(strType);
	strType.Trim();

	if (strType.IsEmpty())
	{
		MessageBox(_T("Въведете тип"), NULL, MB_ICONWARNING);
		return false;
	}

	return true;
}

void CPhoneTypesDlg::SetViewModeRestrictions()
{
	m_btnOK.EnableWindow(FALSE);
	m_edbType.SetReadOnly();
}

void CPhoneTypesDlg::SetDialogMode()
{
	SetLimitText();

	switch (m_eMode)
	{
	case DialogModeModify:
		SetWindowText(_T("Редакция на тип"));
		break;

	case DialogModeView:
		SetWindowText(_T("Преглед на тип"));
		SetViewModeRestrictions();
		break;

	case DialogModeInsert:
		SetWindowText(_T("Добавяне на тип"));
		break;
	}
}

void CPhoneTypesDlg::SetLimitText()
{
	m_edbType.SetLimitText( PHONE_TYPES_TYPE_SIZE );
}
