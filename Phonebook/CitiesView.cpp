
// CitiesView.cpp : implementation of the CCitiesView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "CitiesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CITIES_NAME _T( "Град" )
#define CITIES_DISTRICT _T( "Регион" )
#define CITIES_ID _T( "ID" )
#define LIST_COLUMN_WIDTH 400



// CCitiesView

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, &CListView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview )

	ON_COMMAND( ID_MENU_MODIFY, &CCitiesView::OnMenuModify )
	ON_COMMAND( ID_MENU_INSERT, &CCitiesView::OnMenuInsert )
	ON_COMMAND( ID_MENU_DELETE, &CCitiesView::OnMenuDelete )
	ON_COMMAND( ID_MENU_VIEW, &CCitiesView::OpenListCtrlRecord )

	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CCitiesView::OnLvnItemActivate)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CCitiesView::OnLvnColumnclick)
END_MESSAGE_MAP()

// CCitiesView construction/destruction

CCitiesView::CCitiesView() noexcept
{
}

CCitiesView::~CCitiesView()
{
}

// Methods
// ----------------
BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CListView::PreCreateWindow(cs);
}


void CCitiesView::SetRowData( const CITIES& recCity, UpdateViewHint eUpdateViewHint )
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nItemIndex = -1;

	if (eUpdateViewHint == ViewInsert)
	{
		nItemIndex = oListCtrl.GetItemCount();
		oListCtrl.InsertItem( nItemIndex, _T(""), NULL );
		oListCtrl.SetItemData( nItemIndex, recCity.lID );
	}
	else
		nItemIndex = GetItemIndex( recCity );

	if ( nItemIndex == -1 )
		return;

	oListCtrl.SetItemText( nItemIndex, ColumnName, recCity.szName );
	oListCtrl.SetItemText( nItemIndex, ColumnDistrict, recCity.szDistrict );
}

void CCitiesView::InsertViewColumns()
{
	CListCtrl& oListCtrl = GetListCtrl();
	oListCtrl.InsertColumn(ColumnName, CITIES_NAME, LVCFMT_LEFT, LIST_COLUMN_WIDTH);
	oListCtrl.InsertColumn(ColumnDistrict, CITIES_DISTRICT, LVCFMT_LEFT, LIST_COLUMN_WIDTH);
}

void CCitiesView::FillListCtrl() 
{
	const CCitiesArray& oCitiesArray = GetDocument()->GetCitiesArray();
	for (int i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = oCitiesArray.GetAt(i);
		if (!pCity)
			return;

		SetRowData(*pCity, ViewInsert);
	}
}

void CCitiesView::OnDraw(CDC* /*pDC*/)
{
	CCitiesDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

}

void CCitiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	ModifyListControlStyles();

	InsertViewColumns();

	FillListCtrl();
}


// CCitiesView printing

BOOL CCitiesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCitiesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CCitiesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CCitiesView diagnostics

#ifdef _DEBUG
void CCitiesView::AssertValid() const
{
	CListView::AssertValid();
}

void CCitiesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCitiesDocument* CCitiesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDocument)));
	return (CCitiesDocument*)m_pDocument;
}
#endif //_DEBUG

// CCitiesView message handlers
void CCitiesView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu oMainMenu;
	oMainMenu.LoadMenuW( IDR_CONTEXTMENU );
	CMenu* pSubMenu = oMainMenu.GetSubMenu( 0 );
	if (!pSubMenu)
		return;
	
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if (nIndex == -1)
	{
		oMainMenu.EnableMenuItem( ID_MENU_MODIFY, MF_GRAYED );
		oMainMenu.EnableMenuItem( ID_MENU_VIEW, MF_GRAYED );
		oMainMenu.EnableMenuItem( ID_MENU_DELETE, MF_GRAYED );
	}

	(*pSubMenu).TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CCitiesView::OpenListCtrlRecord() 
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if ( nIndex == -1 )
		return;

	long lID = (long)oListCtrl.GetItemData( nIndex );

	CITIES recCity;
	if (!GetDocument()->SelectWhereID(lID, recCity))
		return;

	CCitiesDlg oCitiesDlg(recCity, DialogModeView );
	oCitiesDlg.DoModal();
}

void CCitiesView::OnMenuModify()
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if ( nIndex == -1 )
		return;

	long lID = (long)oListCtrl.GetItemData( nIndex );

	CITIES recCity;
	if ( !GetDocument()->SelectWhereID( lID, recCity ) )
		return;

	CCitiesDlg oCitiesDlg( recCity, DialogModeModify );
	if (oCitiesDlg.DoModal() != IDOK)
		return;

	if( !GetDocument()->UpdateWhereID( recCity.lID, recCity ) )
		return;
}

void CCitiesView::OnMenuInsert()
{
	CITIES recCity;
	CCitiesDlg oCitiesDlg( recCity, DialogModeInsert );

	if ( oCitiesDlg.DoModal() != IDOK )
		return;

	if ( !GetDocument()->Insert( recCity ) )
		return;
}

void CCitiesView::OnMenuDelete()
{
	int nMessage = MessageBox( _T( "Сигурни ли сте, че искате да изтриете този запис?" ), NULL, MB_YESNO );
	if ( nMessage != IDYES )
		return;

	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if ( nIndex == -1 )
		return;

	long lID = (long)oListCtrl.GetItemData( nIndex );

	if ( !GetDocument()->DeleteWhereID( lID ) )
		return;
}

int CCitiesView::GetItemIndex( const CITIES& recCity )
{
	CListCtrl& oListCtrl = GetListCtrl();

	for (int i = 0; i < oListCtrl.GetItemCount(); i++)
	{
		if ( recCity.lID == oListCtrl.GetItemData(i) )
		{
			return i;
		}
	}

	return -1;
}

int CCitiesView::GetSelectedItemIndex()
{
	CListCtrl& oListCtrl = GetListCtrl();

	POSITION oPos = oListCtrl.GetFirstSelectedItemPosition();
	int nIndex = oListCtrl.GetNextSelectedItem(oPos);
	return nIndex;
}

void CCitiesView::ModifyListControlStyles()
{
	CListCtrl& oListCtrl = GetListCtrl();
	ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	oListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void CCitiesView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
	CListCtrl& oListCtrl = GetListCtrl();

	CUpdateHint* pUpdateHint = static_cast<CUpdateHint*>( pHint );
	if (!pUpdateHint)
		return;

	const CITIES& recCity = pUpdateHint->GetCity();

	switch (lHint)
	{
	case ViewModify:
		SetRowData(recCity, ViewModify);
		break;

	case ViewInsert:
		SetRowData(recCity, ViewInsert);
		break;

	case ViewDelete:
		{
			int nIndex = GetItemIndex( recCity );
			if (nIndex == -1)
				return;

			oListCtrl.DeleteItem(nIndex);
		}

		break;
	}
}

void CCitiesView::OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OpenListCtrlRecord();
	*pResult = 0;
}

void CCitiesView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
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