// PersonsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Phonebook.h"
#include "PersonsDlg.h"
#include "afxdialogex.h"

#define PHONE_NUMBERS_NUMBER_COLUMN _T("Номер")
#define PHONE_NUMBERS_TYPE_COLUMN _T("Тип")
#define PHONE_NUMBERS_COLUMN_WIDTH 250

enum NumbersListControlColumns {
	NumbersListControlColumnNumber=0,
	NumbersListControlColumnType
};

// CPersonsDlg dialog

IMPLEMENT_DYNAMIC(CPersonsDlg, CDialogEx)

CPersonsDlg::CPersonsDlg( CPersonWithNumbers& oPersonWithNumbers, const CCitiesMap& oCitiesMap, 
						const CPhoneTypesMap& oPhoneTypesMap, const DialogModes& eMode, CWnd* pParent )
	: CDialogEx(IDD_PERSONS, pParent)
	, m_oPersonWithNumbers( oPersonWithNumbers )
	, m_oCitiesMap( oCitiesMap )
	, m_oPhoneTypesMap(oPhoneTypesMap)
	, m_eMode( eMode )
{
}

CPersonsDlg::~CPersonsDlg()
{
}

void CPersonsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_PERSONS_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_PERSONS_SURNAME, m_edbSurname);
	DDX_Control(pDX, IDC_EDB_PERSONS_LAST_NAME, m_edbLastName);
	DDX_Control(pDX, IDC_EDB_PERSONS_UCN, m_edbUCN);
	DDX_Control(pDX, IDC_CMB_PERSONS_CITY, m_cmbCity);
	DDX_Control(pDX, IDC_EDB_PERSONS_ADDRESS, m_edbAddress);
	DDX_Control(pDX, IDC_LSC_PERSONS_NUMBERS, m_lscNumbers);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_PERSONS_NUMBER_ADD, m_btnPersonsNumberAdd);
	DDX_Control(pDX, IDC_BTN_PERSONS_NUMBER_DELETE, m_btnPersonsNumberDel);
}


BEGIN_MESSAGE_MAP(CPersonsDlg, CDialogEx)
	ON_BN_CLICKED( IDC_BTN_PERSONS_NUMBER_ADD, &CPersonsDlg::AddPhoneNumber )
	ON_BN_CLICKED( IDC_BTN_PERSONS_NUMBER_DELETE, &CPersonsDlg::DeletePhoneNumber )
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID_MENU_MODIFY, &CPersonsDlg::ModifyPhoneNumbers)
	ON_COMMAND(ID_MENU_INSERT, &CPersonsDlg::AddPhoneNumber)
	ON_COMMAND(ID_MENU_DELETE, &CPersonsDlg::DeletePhoneNumber)
	ON_COMMAND(ID_MENU_VIEW, &CPersonsDlg::ViewPhoneNumbers)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LSC_PERSONS_NUMBERS, &CPersonsDlg::OnItemactivateLscPersonsNumbers)
	ON_NOTIFY(NM_CLICK, IDC_LSC_PERSONS_NUMBERS, &CPersonsDlg::OnClickLscPersonsNumbers)
END_MESSAGE_MAP()


// CPersonsDlg message handlers

// Methods
// ----------------
BOOL CPersonsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDialogText();

	SetDialogMode();

	SetLimitText();

	return TRUE;  
}

void CPersonsDlg::SetDialogText()
{
	PERSONS& recPerson = m_oPersonWithNumbers.GetPerson();

	CString strName = recPerson.szName;
	strName.Trim();
	m_edbName.SetWindowTextW(strName);

	CString strSurname = recPerson.szSurname;
	strSurname.Trim();
	m_edbSurname.SetWindowTextW( strSurname );

	CString strLastName = recPerson.szLastName;
	strLastName.Trim();
	m_edbLastName.SetWindowTextW( strLastName );

	CString strUCN = recPerson.szUCN;
	strUCN.Trim();
	m_edbUCN.SetWindowTextW( strUCN );

	if (m_eMode == DialogModeView)
	{
		CITIES* pCity = nullptr;
		if (!m_oCitiesMap.Lookup(recPerson.lCityID, pCity))
			return;

		if ( pCity == nullptr )
			return;

		m_cmbCity.AddString( pCity->szName );
	}
	else
		FillComboBox( recPerson );
	
	CString strAddress = recPerson.szAddress;
	strAddress.Trim();
	m_edbAddress.SetWindowTextW( strAddress );

	SetPhoneNumbersListControl();
}

void CPersonsDlg::GetDialogText()
{
	CString strName = NULL;
	CString strSurname = NULL;
	CString strLastName = NULL;
	CString strUCN = NULL;
	CString strAddress = NULL;

	m_edbName.GetWindowTextW( strName );
	m_edbSurname.GetWindowTextW( strSurname );
	m_edbLastName.GetWindowTextW( strLastName );
	m_edbUCN.GetWindowTextW( strUCN );
	m_edbAddress.GetWindowTextW( strAddress );

	PERSONS& recPerson = m_oPersonWithNumbers.GetPerson();
	_tcscpy_s( recPerson.szName, _countof( recPerson.szName ), strName );
	_tcscpy_s( recPerson.szSurname, _countof( recPerson.szSurname ), strSurname );
	_tcscpy_s( recPerson.szLastName, _countof( recPerson.szLastName ), strLastName );
	_tcscpy_s( recPerson.szUCN, _countof( recPerson.szUCN ), strUCN );
	_tcscpy_s( recPerson.szAddress, _countof( recPerson.szAddress ), strAddress );

	int nIndex = m_cmbCity.GetCurSel();
	if (nIndex == -1)
	{
		MessageBox(_T("Няма избран град"), NULL, MB_ICONWARNING);
		return;
	}

	recPerson.lCityID = (long)m_cmbCity.GetItemData( nIndex );
}

bool CPersonsDlg::ValidateData()
{
	CString strName = NULL;
	CString strSurname = NULL;
	CString strLastName = NULL;
	CString strUCN = NULL;
	CString strAddress = NULL;

	m_edbName.GetWindowTextW( strName );
	m_edbSurname.GetWindowTextW( strSurname );
	m_edbLastName.GetWindowTextW( strLastName );
	m_edbUCN.GetWindowTextW( strUCN );
	m_edbAddress.GetWindowTextW( strAddress );

	int nIndex = m_cmbCity.GetCurSel();
	if (nIndex == -1)
	{
		m_cmbCity.SetFocus();
		MessageBox(_T("Изберете град"), NULL, MB_ICONWARNING);
		return false;
	}

	strName.Trim();
	if (strName.IsEmpty())
	{
		m_edbName.SetFocus();
		MessageBox(_T("Въведете име"), NULL, MB_ICONWARNING);
		return false;
	}
	
	strSurname.Trim();
	if (strSurname.IsEmpty())
	{
		m_edbSurname.SetFocus();
		MessageBox(_T("Въведете презиме"), NULL, MB_ICONWARNING);
		return false;
	}

	strLastName.Trim();
	if (strLastName.IsEmpty())
	{
		m_edbLastName.SetFocus();
		MessageBox(_T("Въведете фамилия"), NULL, MB_ICONWARNING);
		return false;
	}

	strUCN.Trim();
	if (strUCN.SpanIncluding(_T("0123456789")) != strUCN)
	{
		m_edbUCN.SetFocus();
		MessageBox(_T("Грешно ЕГН"), NULL, MB_ICONWARNING);
		return false;
	}
	if (strUCN.IsEmpty())
	{
		m_edbUCN.SetFocus();
		MessageBox(_T("Въведете ЕГН"), NULL, MB_ICONWARNING);
		return false;
	}

	strAddress.Trim();
	if (strAddress.IsEmpty())
	{
		m_edbAddress.SetFocus();
		MessageBox(_T("Въведете адрес"), NULL, MB_ICONWARNING);
		return false;
	}

	if (m_lscNumbers.GetItemCount() == 0)
	{
		MessageBox(_T("Въведете номера"), NULL, MB_ICONWARNING);
		return false;
	}
		
	return true;
}

void CPersonsDlg::SetViewModeRestrictions()
{
	m_edbName.SetReadOnly();
	m_edbSurname.SetReadOnly();
	m_edbLastName.SetReadOnly();
	m_edbUCN.SetReadOnly();
	m_edbAddress.SetReadOnly();
	m_cmbCity.SetCurSel( 0 );
	m_cmbCity.EnableWindow( FALSE );
	m_lscNumbers.EnableWindow( FALSE );
	m_btnOK.EnableWindow( FALSE );
	m_btnPersonsNumberAdd.EnableWindow( FALSE );
	m_btnPersonsNumberDel.EnableWindow( FALSE );
}

void CPersonsDlg::SetDialogMode()
{
	switch (m_eMode)
	{
	case DialogModeModify:
		SetWindowText( _T( "Редакция на потребител" ) );
		break;

	case DialogModeView:
		SetWindowText( _T( "Преглед на потребител" ) );
		SetViewModeRestrictions();
		break;

	case DialogModeInsert:
		SetWindowText( _T( "Добавяне на потребител" ) );
		break;
	}
}

void CPersonsDlg::SetLimitText()
{
	m_edbName.SetLimitText( PERSONS_NAME_SIZE );
	m_edbSurname.SetLimitText( PERSONS_SURNAME_SIZE );
	m_edbLastName.SetLimitText( PERSONS_LASTNAME_SIZE );
	m_edbUCN.SetLimitText( PERSONS_UCN_SIZE );
	m_edbAddress.SetLimitText( PERSONS_ADDRESS_SIZE );
}

void CPersonsDlg::SetPhoneNumbersListControl()
{
	m_lscNumbers.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	m_lscNumbers.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_lscNumbers.InsertColumn(NumbersListControlColumnNumber, PHONE_NUMBERS_NUMBER_COLUMN, LVCFMT_LEFT, PHONE_NUMBERS_COLUMN_WIDTH);
	m_lscNumbers.InsertColumn(NumbersListControlColumnType, PHONE_NUMBERS_TYPE_COLUMN, LVCFMT_LEFT, PHONE_NUMBERS_COLUMN_WIDTH);

	const CPhoneNumbersArray& oPhoneNumbersArray = m_oPersonWithNumbers.GetNumbers();
	for (int i = 0; i < oPhoneNumbersArray.GetCount(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oPhoneNumbersArray.GetAt(i);
		if (!pPhoneNumber)
			return;

		SetItemText(*pPhoneNumber, DialogModeInsert, i);
	}
}

void CPersonsDlg::SetItemText( const PHONE_NUMBERS& recPhoneNumber, DialogModes eModes, int nArrayIndex)
{
	int nItemIndex = -1;
	if (eModes == DialogModeInsert)
	{
		nItemIndex = m_lscNumbers.GetItemCount();
		m_lscNumbers.InsertItem(nItemIndex, _T(""), NULL);
		m_lscNumbers.SetItemData(nItemIndex, nArrayIndex);
	}
	else
		nItemIndex = GetItemIndex( nArrayIndex );

	if (nItemIndex == -1)
	{
		MessageBox(_T("Невалидна селекция"), NULL, MB_ICONWARNING);
		return;
	}

	m_lscNumbers.SetItemText( nItemIndex, NumbersListControlColumnNumber, recPhoneNumber.szNumber );

	PHONE_TYPES* pPhoneType = nullptr;
	if (!m_oPhoneTypesMap.Lookup(recPhoneNumber.lPhoneTypeID, pPhoneType))
		return;

	if ( pPhoneType == nullptr )
		return;

	m_lscNumbers.SetItemText( nItemIndex, NumbersListControlColumnType, pPhoneType->szType );
}

int CPersonsDlg::GetItemIndex( int nArrayIndex )
{
	for (int i = 0; i < m_lscNumbers.GetItemCount(); i++)
	{
		if ( nArrayIndex == m_lscNumbers.GetItemData(i))
		{
			return i;
		}
	}
	return -1;
}

void CPersonsDlg::ModifyPhoneNumbers()
{
	int nArrayIndex = GetSelectedPhoneNumberArrayIndex();
	if ( nArrayIndex == -1 )
		return;

	CPhoneNumbersArray& oPhoneNumbersArray = m_oPersonWithNumbers.GetNumbers();

	PHONE_NUMBERS* pPhoneNumber = oPhoneNumbersArray.GetAt( nArrayIndex );
	if ( !pPhoneNumber )
		return;

	CPhoneNumbersDlg oPhoneNumbersDlg( *pPhoneNumber, m_oPhoneTypesMap, DialogModeModify);
	if (oPhoneNumbersDlg.DoModal() != IDOK)
		return;

	SetItemText( *pPhoneNumber,DialogModeModify , nArrayIndex);
}

void CPersonsDlg::ViewPhoneNumbers()
{
	int nArrayIndex = GetSelectedPhoneNumberArrayIndex();
	if (nArrayIndex == -1)
		return;

	CPhoneNumbersArray& oPhoneNumbersArray = m_oPersonWithNumbers.GetNumbers();

	PHONE_NUMBERS* pPhoneNumber = oPhoneNumbersArray.GetAt( nArrayIndex );
	if ( !pPhoneNumber )
		return;

	CPhoneNumbersDlg oPhoneNumbersDlg( *pPhoneNumber, m_oPhoneTypesMap, DialogModeView );
	if (oPhoneNumbersDlg.DoModal() != IDOK)
		return;

	SetItemText( *pPhoneNumber, DialogModeView, nArrayIndex );
}

int CPersonsDlg::GetSelectedPhoneNumberArrayIndex()
{
	int nIndex = GetSelectedItemIndex();
	if (nIndex == -1)
		return -1;

	int nArrayIndex = (int)m_lscNumbers.GetItemData(nIndex);
	return nArrayIndex;
}

void CPersonsDlg::AddPhoneNumber()
{
	PHONE_NUMBERS recPhoneNumber;

	CPhoneNumbersDlg oPhoneNumbersDlg( recPhoneNumber, m_oPhoneTypesMap, DialogModeInsert );
	if (oPhoneNumbersDlg.DoModal() != IDOK)
		return;

	int nIndex = (int)m_oPersonWithNumbers.GetNumbers().Add( new PHONE_NUMBERS( recPhoneNumber ) );

	SetItemText( recPhoneNumber, DialogModeInsert, nIndex );
}

void CPersonsDlg::DeletePhoneNumber()
{
	int nMessage = MessageBox(_T("Сигурни ли сте, че искате да изтриете този запис?"), NULL, MB_YESNO);
	if (nMessage != IDYES)
		return;

	CPhoneNumbersArray& oPhoneNumbersArray = m_oPersonWithNumbers.GetNumbers();

	int nArrayIndex = GetSelectedPhoneNumberArrayIndex();
	if ( nArrayIndex == -1 )
		return;

	oPhoneNumbersArray.DeleteElementAt( nArrayIndex );

	int nIndex = GetSelectedItemIndex();
	if ( nIndex == -1 )
		return ;

	m_lscNumbers.DeleteItem( nIndex );
}

int CPersonsDlg::GetSelectedItemIndex()
{
	POSITION oPos = m_lscNumbers.GetFirstSelectedItemPosition();
	int nIndex = m_lscNumbers.GetNextSelectedItem(oPos);
	return nIndex;
}

void CPersonsDlg::FillComboBox( const PERSONS& recPerson )
{
	POSITION pos = m_oCitiesMap.GetStartPosition();
	while (pos != NULL)
	{
		long lKey = 0;
		CITIES* pCity = nullptr;
		m_oCitiesMap.GetNextAssoc( pos, lKey, pCity );

		int pos = m_cmbCity.AddString( pCity->szName );
		m_cmbCity.SetItemData( pos, pCity->lID );

		if ( recPerson.lCityID == pCity->lID )
		{
			m_cmbCity.SetCurSel( pos );
		}
	}
}

void CPersonsDlg::OnOK()
{
	if (!ValidateData())
		return;

	GetDialogText();

	CDialogEx::OnOK();
}


void CPersonsDlg::OnClickedBtnPersonsNumberAdd()
{
	AddPhoneNumber();
}

void CPersonsDlg::OnClickedBtnPersonsNumberDelete()
{
	DeletePhoneNumber();
}


void CPersonsDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CRect оRect;
	m_lscNumbers.GetWindowRect(оRect);

	CMenu oMainMenu;
	oMainMenu.LoadMenuW(IDR_CONTEXTMENU);
	CMenu* pSubMenu = oMainMenu.GetSubMenu(0);
	if ( !pSubMenu )
		return;

	int nIndex = GetSelectedItemIndex();
	if (nIndex == -1)
	{
		if (m_eMode == DialogModeView)
			oMainMenu.EnableMenuItem( ID_MENU_INSERT, MF_GRAYED );

		oMainMenu.EnableMenuItem(ID_MENU_MODIFY, MF_GRAYED);
		oMainMenu.EnableMenuItem(ID_MENU_VIEW, MF_GRAYED);
		oMainMenu.EnableMenuItem(ID_MENU_DELETE, MF_GRAYED);
	}

	if( оRect.PtInRect( point ) )
		(*pSubMenu).TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
}

void CPersonsDlg::OnItemactivateLscPersonsNumbers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	ViewPhoneNumbers();
	
	*pResult = 0;
}

void CPersonsDlg::OnClickLscPersonsNumbers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nIndex = GetSelectedItemIndex();

	if (nIndex!=-1)
		m_btnPersonsNumberDel.EnableWindow(TRUE);
	else
		m_btnPersonsNumberDel.EnableWindow(FALSE);

	*pResult = 0;
}
