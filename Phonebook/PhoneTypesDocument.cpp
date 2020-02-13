// PhoneTypesDocument.cpp : implementation of the CPhoneTypesDocument class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "PhoneTypesDocument.h"
#include "UpdateHint.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhoneTypesDocument

IMPLEMENT_DYNCREATE(CPhoneTypesDocument, CDocument)

BEGIN_MESSAGE_MAP(CPhoneTypesDocument, CDocument)
END_MESSAGE_MAP()


// CPhoneTypesDocument construction/destruction

CPhoneTypesDocument::CPhoneTypesDocument() noexcept
{
}

CPhoneTypesDocument::~CPhoneTypesDocument()
{
}

// Methods
// ----------------

bool CPhoneTypesDocument::SelectAll()
{
	CPhoneTypesData oPhoneTypesData;
	if (!oPhoneTypesData.SelectAll(m_oPhoneTypesArray))
		return false;

	return true;
}

bool CPhoneTypesDocument::SelectWhereID(const long lID, PHONE_TYPES& recPhoneType)
{
	CPhoneTypesData oPhoneTypesData;
	if (!oPhoneTypesData.SelectWhereID(lID, recPhoneType))
		return false;

	return true;
}

bool CPhoneTypesDocument::UpdateWhereID(const long lID, const PHONE_TYPES& recPhoneType)
{
	CPhoneTypesData oPhoneTypesData;
	if (!oPhoneTypesData.UpdateWhereID(lID, recPhoneType))
		return false;

	for (int i = 0; i < m_oPhoneTypesArray.GetCount(); i++)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(i);
		if (!pPhoneType)
			return false;

		if (pPhoneType->lID == lID)
		{
			*pPhoneType = recPhoneType;
			break;
		}
	}

	CPhoneTypesUpdateHint oHint( recPhoneType );

	UpdateAllViews(NULL, ViewModify, &oHint);

	return true;
}

bool CPhoneTypesDocument::Insert(PHONE_TYPES& recPhoneType)
{
	CPhoneTypesData oPhoneTypesData;
	if (!oPhoneTypesData.Insert(recPhoneType))
		return false;

	m_oPhoneTypesArray.Add(new PHONE_TYPES(recPhoneType));

	CPhoneTypesUpdateHint oHint( recPhoneType );

	UpdateAllViews(NULL, ViewInsert, &oHint);

	return true;
}

bool CPhoneTypesDocument::DeleteWhereID(const long lID)
{
	CPhoneTypesData oPhoneTypesData;
	if (!oPhoneTypesData.DeleteWhereID(lID))
		return false;

	for (int i = 0; i < m_oPhoneTypesArray.GetCount(); i++)
	{
		if (m_oPhoneTypesArray.GetAt(i)->lID == lID)
		{
			m_oPhoneTypesArray.DeleteElementAt(i);
			break;
		}
	}

	
	PHONE_TYPES recPhoneType( lID );
	CPhoneTypesUpdateHint oHint(recPhoneType);
	UpdateAllViews(NULL, ViewDelete, &oHint);

	return true;
}

const CPhoneTypesArray& CPhoneTypesDocument::GetPhoneTypesArray() const
{
	return m_oPhoneTypesArray;
}

int ComparePhoneTypes(void* pQSortContext, const void* a, const void* b)
{
	if ( !pQSortContext || !a || !b )
		return 0;

	CQSortContext& oQSortContext = *(CQSortContext*)pQSortContext;

	PHONE_TYPES recPhoneType1 = **(PHONE_TYPES**)a;
	PHONE_TYPES recPhoneType2 = **(PHONE_TYPES**)b;

	int nRow1 = -1;
	int nRow2 = -1;

	const CListCtrl& oListCtrl = oQSortContext.GetListControl();
	for (int i = 0; i < oListCtrl.GetItemCount(); i++)
	{
		if (recPhoneType1.lID == oListCtrl.GetItemData(i))
		{
			nRow1 = i;
		}

		if (recPhoneType2.lID == oListCtrl.GetItemData(i))
		{
			nRow2 = i;
		}
	}

	CString strItem1 = oListCtrl.GetItemText(nRow1, oQSortContext.GetColumn());
	CString strItem2 = oListCtrl.GetItemText(nRow2, oQSortContext.GetColumn());

	bool nAsc = oQSortContext.GetOrder();
	if (nAsc)
		return strItem1.CompareNoCase(strItem2);
	else
		return -strItem1.CompareNoCase(strItem2);
}

void CPhoneTypesDocument::SortArray(const CQSortContext& oQSortContext)
{
	qsort_s( m_oPhoneTypesArray.GetData(), m_oPhoneTypesArray.GetCount(), sizeof(PHONE_TYPES*), ComparePhoneTypes, (void*)&oQSortContext );
}

BOOL CPhoneTypesDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!SelectAll())
		return FALSE;

	// (SDI documents will reuse this document)

	return TRUE;
}

// CPhoneTypesDocument serialization

void CPhoneTypesDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void CPhoneTypesDocument::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(lpszTitle);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPhoneTypesDocument::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPhoneTypesDocument::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPhoneTypesDocument::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPhoneTypesDocument diagnostics

#ifdef _DEBUG
void CPhoneTypesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhoneTypesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPhoneTypesDocument commands