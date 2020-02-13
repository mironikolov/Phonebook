// PersonsView.cpp : implementation of the CPersonsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "PersonsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PERSONS_NAME _T( "Име" )
#define PERSONS_SURNAME _T( "Презиме" )
#define PERSONS_LAST_NAME _T( "Фамилия" )
#define PERSONS_UCN _T( "ЕГН" )
#define PERSONS_CITY _T( "Град" )
#define PERSONS_ADDRESS _T( "Адрес" )
#define LIST_COLUMN_WIDTH 300

// CCitiesView

IMPLEMENT_DYNCREATE(CPersonsView, CListView)

BEGIN_MESSAGE_MAP( CPersonsView, CListView )
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, &CListView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview )

	ON_COMMAND( ID_MENU_MODIFY, &CPersonsView::OnMenuModify )
	ON_COMMAND( ID_MENU_INSERT, &CPersonsView::OnMenuInsert )
	ON_COMMAND( ID_MENU_DELETE, &CPersonsView::OnMenuDelete )
	ON_COMMAND( ID_MENU_VIEW, &CPersonsView::OpenListCtrlRecord )

	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT( LVN_ITEMACTIVATE, &CPersonsView::OnLvnItemActivate )
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CPersonsView::OnLvnColumnclick)
END_MESSAGE_MAP()

// CCitiesView construction/destruction

CPersonsView::CPersonsView() noexcept
{
}

CPersonsView::~CPersonsView()
{
}

// Methods
// ----------------
BOOL CPersonsView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CListView::PreCreateWindow(cs);
}


void CPersonsView::SetRowText(const PERSONS& recPerson, UpdateViewHint eUpdateViewHint)
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nItemCount = -1;

	if (eUpdateViewHint == ViewInsert)
	{
		nItemCount = oListCtrl.GetItemCount();
		oListCtrl.InsertItem( nItemCount, _T(""), NULL );
		oListCtrl.SetItemData( nItemCount, recPerson.lID );
	}
	else
		nItemCount = GetItemIndex( recPerson );

	if ( nItemCount == -1 )
		return;

	oListCtrl.SetItemText( nItemCount, ColumnPersonsName, recPerson.szName );
	oListCtrl.SetItemText( nItemCount, ColumnPersonsSurname, recPerson.szSurname );
	oListCtrl.SetItemText( nItemCount, ColumnPersonsLastName, recPerson.szLastName );
	oListCtrl.SetItemText( nItemCount, ColumnPersonsUCN, recPerson.szUCN );
	oListCtrl.SetItemText( nItemCount, ColumnPersonsAddress, recPerson.szAddress );
	
	CITIES* pCity = nullptr;
	if(!GetDocument()->GetCitiesMap().Lookup( recPerson.lCityID, pCity ))
		return;

	if ( pCity == nullptr )
		return;

	oListCtrl.SetItemText( nItemCount, ColumnPersonsCity, pCity->szName );
}

void CPersonsView::InsertViewColumns()
{
	CListCtrl& oListCtrl = GetListCtrl();
	oListCtrl.InsertColumn( ColumnPersonsName, PERSONS_NAME, LVCFMT_LEFT, LIST_COLUMN_WIDTH );
	oListCtrl.InsertColumn( ColumnPersonsSurname, PERSONS_SURNAME, LVCFMT_LEFT, LIST_COLUMN_WIDTH );
	oListCtrl.InsertColumn( ColumnPersonsLastName, PERSONS_LAST_NAME, LVCFMT_LEFT, LIST_COLUMN_WIDTH );
	oListCtrl.InsertColumn( ColumnPersonsUCN, PERSONS_UCN, LVCFMT_LEFT, LIST_COLUMN_WIDTH );
	oListCtrl.InsertColumn( ColumnPersonsCity, PERSONS_CITY, LVCFMT_LEFT, LIST_COLUMN_WIDTH );
	oListCtrl.InsertColumn( ColumnPersonsAddress, PERSONS_ADDRESS, LVCFMT_LEFT, LIST_COLUMN_WIDTH );
}

void CPersonsView::FillListCtrl()
{
	const CPersonsArray& oPersonsArray = GetDocument()->GetPersonsArray();
	for (int i = 0; i < oPersonsArray.GetCount(); i++)
	{
		PERSONS* pPerson = oPersonsArray.GetAt(i);
		if (!pPerson)
			return;

		SetRowText(*pPerson, ViewInsert);
	}
}

void CPersonsView::OnDraw(CDC* /*pDC*/)
{
	CPersonsDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

}

void CPersonsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	ModifyListControlStyles();

	InsertViewColumns();

	FillListCtrl();
}


// CCitiesView printing

BOOL CPersonsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPersonsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPersonsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CCitiesView diagnostics

#ifdef _DEBUG
void CPersonsView::AssertValid() const
{
	CListView::AssertValid();
}

void CPersonsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPersonsDocument* CPersonsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPersonsDocument)));
	return (CPersonsDocument*)m_pDocument;
}
#endif //_DEBUG


// CCitiesView message handlers

void CPersonsView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu oMainMenu;
	oMainMenu.LoadMenuW(IDR_CONTEXTMENU);
	CMenu* pSubMenu = oMainMenu.GetSubMenu(0);
	if (!pSubMenu)
		return;

	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();

	if ( nIndex == -1 )
	{
		oMainMenu.EnableMenuItem( ID_MENU_MODIFY, MF_GRAYED );
		oMainMenu.EnableMenuItem( ID_MENU_VIEW, MF_GRAYED );
		oMainMenu.EnableMenuItem( ID_MENU_DELETE, MF_GRAYED );
	}

	(*pSubMenu).TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CPersonsView::OpenListCtrlRecord()
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if (nIndex == -1)
		return;

	long lID = (long)oListCtrl.GetItemData( nIndex );

	CPersonWithNumbers oPersonWithNumbers;

	if ( !GetDocument()->SelectWhereID( lID, oPersonWithNumbers ) )
		return;

	const CPhoneTypesMap& oPhoneTypesMap = GetDocument()->GetPhoneTypesMap();

	CPersonsDlg oPersonsDlg( oPersonWithNumbers, GetDocument()->GetCitiesMap(), oPhoneTypesMap, DialogModeView);
	oPersonsDlg.DoModal();
}

void CPersonsView::OnMenuModify()
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();

	if ( nIndex == -1 )
		return;

	long lID = (long)oListCtrl.GetItemData( nIndex );

	CPersonWithNumbers oPersonWithNumbers;

	if ( !GetDocument()->SelectWhereID( lID, oPersonWithNumbers ) )
		return;

	const CCitiesMap& oCitiesMap = GetDocument()->GetCitiesMap();
	const CPhoneTypesMap& oPhoneTypesMap = GetDocument()->GetPhoneTypesMap();

	CPersonsDlg oPersonsDlg( oPersonWithNumbers, oCitiesMap, oPhoneTypesMap, DialogModeModify);
	if (oPersonsDlg .DoModal() != IDOK )
		return;

	if (!GetDocument()->UpdateWhereID( oPersonWithNumbers.GetPerson().lID, oPersonWithNumbers))
		return;
}

void CPersonsView::OnMenuInsert()
{
	CPersonWithNumbers oPersonWithNumbers;

	const CPhoneTypesMap& oPhoneTypesMap = GetDocument()->GetPhoneTypesMap();
	const CCitiesMap& oCitiesMap = GetDocument()->GetCitiesMap();
	CPersonsDlg oPersonsDlg( oPersonWithNumbers, oCitiesMap, oPhoneTypesMap, DialogModeInsert);

	if ( oPersonsDlg.DoModal() != IDOK )
		return;

	if (!GetDocument()->Insert( oPersonWithNumbers ))
		return;
}

void CPersonsView::OnMenuDelete()
{
	int nMessage = MessageBox(_T("Сигурни ли сте, че искате да изтриете този запис?"), NULL, MB_YESNO);
	if (nMessage != IDYES)
		return;

	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if ( nIndex == -1 )
		return;

	long lID = (long)oListCtrl.GetItemData( nIndex );

	if (!GetDocument()->DeleteWhereID(lID))
		return;
}

int CPersonsView::GetItemIndex(const PERSONS& recPerson)
{
	CListCtrl& oListCtrl = GetListCtrl();

	for (int i = 0; i < oListCtrl.GetItemCount(); i++)
	{
		if (recPerson.lID == oListCtrl.GetItemData(i))
		{
			return i;
		}
	}

	return -1;
}

int CPersonsView::GetSelectedItemIndex()
{
	CListCtrl& oListCtrl = GetListCtrl();

	POSITION oPos = oListCtrl.GetFirstSelectedItemPosition();
	int nIndex = oListCtrl.GetNextSelectedItem(oPos);
	return nIndex;
}

void CPersonsView::ModifyListControlStyles()
{
	CListCtrl& oListCtrl = GetListCtrl();
	oListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	oListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
}

void CPersonsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListCtrl& oListCtrl = GetListCtrl();

	CPersonWithNumbers* pUpdateHint = static_cast<CPersonWithNumbers*>(pHint);
	if (!pUpdateHint)
		return;

	PERSONS recPerson = pUpdateHint->GetPerson();

	switch (lHint)
	{
	case ViewModify:
		SetRowText(recPerson, ViewModify);
		break;

	case ViewInsert:
		SetRowText(recPerson, ViewInsert);
		break;

	case ViewDelete:
		{
		int nIndex = GetItemIndex( recPerson );
		if ( nIndex == -1 )
			return;

		oListCtrl.DeleteItem( nIndex );
		}

		break;
	}

}

void CPersonsView::OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OpenListCtrlRecord();
	*pResult = 0;
}

void CPersonsView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	static int nLastColumn = -1;

	static bool bAsc = true;

	if (nLastColumn != pNMLV->iSubItem)
	{
		bAsc = true;
		nLastColumn = pNMLV->iSubItem;
	}
	else
		bAsc = !bAsc;

	CListCtrl& oListCtrl = GetListCtrl();

	CQSortContext oQSortContext(oListCtrl, bAsc, pNMLV->iSubItem);

	GetDocument()->SortArray(oQSortContext);

	oListCtrl.DeleteAllItems();

	FillListCtrl();

	*pResult = 0;
}

