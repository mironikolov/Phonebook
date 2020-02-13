
// PhoneTypesView.cpp : implementation of the CPhoneTypesView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "PhoneTypesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PHONE_TYPES_TYPE _T( "Тип" )
#define COLUMN_TYPE 0
#define COLUMN_WIDTH 200

// CPhoneTypesView

IMPLEMENT_DYNCREATE(CPhoneTypesView, CListView)

BEGIN_MESSAGE_MAP(CPhoneTypesView, CListView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)

	ON_COMMAND(ID_MENU_MODIFY, &CPhoneTypesView::OnMenuModify)
	ON_COMMAND(ID_MENU_INSERT, &CPhoneTypesView::OnMenuInsert)
	ON_COMMAND(ID_MENU_DELETE, &CPhoneTypesView::OnMenuDelete)
	ON_COMMAND(ID_MENU_VIEW, &CPhoneTypesView::OpenListCtrlRecord)

	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CPhoneTypesView::OnLvnItemActivate)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CPhoneTypesView::OnLvnColumnclick)
END_MESSAGE_MAP()

// CPhoneTypesView construction/destruction

CPhoneTypesView::CPhoneTypesView() noexcept
{
}

CPhoneTypesView::~CPhoneTypesView()
{
}

// Methods
// ----------------
BOOL CPhoneTypesView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CListView::PreCreateWindow(cs);
}


void CPhoneTypesView::SetRowData(const PHONE_TYPES& recPhoneType, UpdateViewHint eUpdateViewHint)
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nItemIndex = -1;

	if (eUpdateViewHint == ViewInsert)
	{
		nItemIndex = oListCtrl.GetItemCount();
		oListCtrl.InsertItem(nItemIndex, _T(""), NULL);
		oListCtrl.SetItemData(nItemIndex, recPhoneType.lID);
	}
	else
		nItemIndex = GetItemIndex(recPhoneType);

	if (nItemIndex == -1)
		return;

	oListCtrl.SetItemText(nItemIndex, COLUMN_TYPE, recPhoneType.szType);
}

void CPhoneTypesView::InsertViewColumns()
{
	CListCtrl& oListCtrl = GetListCtrl();
	oListCtrl.InsertColumn(COLUMN_TYPE, PHONE_TYPES_TYPE, LVCFMT_LEFT, COLUMN_WIDTH);
}

void CPhoneTypesView::FillListCtrl()
{
	const CPhoneTypesArray& oPhoneTypesArray = GetDocument()->GetPhoneTypesArray();
	for (int i = 0; i < oPhoneTypesArray.GetCount(); i++)
	{
		PHONE_TYPES* pPhoneType = oPhoneTypesArray.GetAt(i);
		if (!pPhoneType)
			return;

		SetRowData(*pPhoneType, ViewInsert);
	}
}

void CPhoneTypesView::OnDraw(CDC* /*pDC*/)
{
	CPhoneTypesDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

}

void CPhoneTypesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	ModifyListControlStyles();

	InsertViewColumns();

	FillListCtrl();
}


// CPhoneTypesView printing

BOOL CPhoneTypesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPhoneTypesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPhoneTypesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// CPhoneTypesView diagnostics

#ifdef _DEBUG
void CPhoneTypesView::AssertValid() const
{
	CListView::AssertValid();
}

void CPhoneTypesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPhoneTypesDocument* CPhoneTypesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneTypesDocument)));
	return (CPhoneTypesDocument*)m_pDocument;
}
#endif //_DEBUG

// CPhoneTypesView message handlers
void CPhoneTypesView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu oMainMenu;
	oMainMenu.LoadMenuW(IDR_CONTEXTMENU);
	CMenu* pSubMenu = oMainMenu.GetSubMenu(0);
	if (!pSubMenu)
		return;

	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();

	if (nIndex == -1)
	{
		oMainMenu.EnableMenuItem(ID_MENU_MODIFY, MF_GRAYED);
		oMainMenu.EnableMenuItem(ID_MENU_VIEW, MF_GRAYED);
		oMainMenu.EnableMenuItem(ID_MENU_DELETE, MF_GRAYED);
	}

	(*pSubMenu).TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CPhoneTypesView::OpenListCtrlRecord()
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();

	if (nIndex == -1)
		return;

	long lID = (long)oListCtrl.GetItemData(nIndex);

	PHONE_TYPES recPhoneType;
	if (!GetDocument()->SelectWhereID(lID, recPhoneType))
		return;

	CPhoneTypesDlg oPhoneTypesDlg(recPhoneType, DialogModeView);
	oPhoneTypesDlg.DoModal();
}

void CPhoneTypesView::OnMenuModify()
{
	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();

	if (nIndex == -1)
		return;

	long lID = (long)oListCtrl.GetItemData(nIndex);

	PHONE_TYPES recPhoneType;
	if (!GetDocument()->SelectWhereID(lID, recPhoneType))
		return;

	CPhoneTypesDlg oPhoneTypesDlg(recPhoneType, DialogModeModify);
	if (oPhoneTypesDlg.DoModal() != IDOK)
		return;

	if (!GetDocument()->UpdateWhereID(recPhoneType.lID, recPhoneType))
		return;
}

void CPhoneTypesView::OnMenuInsert()
{
	PHONE_TYPES recPhoneType;
	CPhoneTypesDlg oPhoneTypesDlg(recPhoneType, DialogModeInsert);

	if (oPhoneTypesDlg.DoModal() != IDOK)
		return;

	if (!GetDocument()->Insert(recPhoneType))
		return;
}

void CPhoneTypesView::OnMenuDelete()
{
	int nMessage = MessageBox(_T("Сигурни ли сте, че искате да изтриете този запис?"), NULL, MB_YESNO);
	if (nMessage != IDYES)
		return;

	CListCtrl& oListCtrl = GetListCtrl();

	int nIndex = GetSelectedItemIndex();
	if (nIndex == -1)
		return;

	long lID = (long)oListCtrl.GetItemData(nIndex);

	if (!GetDocument()->DeleteWhereID(lID))
		return;
}

int CPhoneTypesView::GetItemIndex(const PHONE_TYPES& recPhoneType)
{
	CListCtrl& oListCtrl = GetListCtrl();

	for (int i = 0; i < oListCtrl.GetItemCount(); i++)
	{
		if (recPhoneType.lID == oListCtrl.GetItemData(i))
		{
			return i;
		}
	}

	return -1;
}

int CPhoneTypesView::GetSelectedItemIndex()
{
	CListCtrl& oListCtrl = GetListCtrl();

	POSITION oPos = oListCtrl.GetFirstSelectedItemPosition();
	int nIndex = oListCtrl.GetNextSelectedItem(oPos);
	return nIndex;
}

void CPhoneTypesView::ModifyListControlStyles()
{
	CListCtrl& oListCtrl = GetListCtrl();
	ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	oListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void CPhoneTypesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CListCtrl& oListCtrl = GetListCtrl();

	CPhoneTypesUpdateHint* pPhoneTypesUpdateHint = static_cast<CPhoneTypesUpdateHint*>(pHint);
	if (!pPhoneTypesUpdateHint)
		return;

	const PHONE_TYPES& recPhoneType = pPhoneTypesUpdateHint->GetPhoneType();

	switch (lHint)
	{
	case ViewModify:
		SetRowData(recPhoneType, ViewModify);
		break;

	case ViewInsert:
		SetRowData(recPhoneType, ViewInsert);
		break;

	case ViewDelete:
	{
		int nIndex = GetItemIndex(recPhoneType);
		if (nIndex == -1)
			return;

		oListCtrl.DeleteItem(nIndex);
	}

	break;
	}
}

void CPhoneTypesView::OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OpenListCtrlRecord();
	*pResult = 0;
}

void CPhoneTypesView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
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

	CQSortContext oCompare(oListCtrl, bAsc, pNMLV->iSubItem);

	GetDocument()->SortArray(oCompare);

	oListCtrl.DeleteAllItems();

	FillListCtrl();

	*pResult = 0;
}