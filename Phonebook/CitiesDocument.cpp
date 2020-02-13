
// CitiesDocument.cpp : implementation of the CCitiesDocument class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "CitiesDocument.h"
#include "UpdateHint.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum CitiesColumns
{
	CitiesColumnName = 0,
	CitiesColumnDistrict = 1
};

// CCitiesDocument

IMPLEMENT_DYNCREATE(CCitiesDocument, CDocument)

BEGIN_MESSAGE_MAP(CCitiesDocument, CDocument)
END_MESSAGE_MAP()


// CCitiesDocument construction/destruction

CCitiesDocument::CCitiesDocument() noexcept
{
}

CCitiesDocument::~CCitiesDocument()
{
}

// Methods
// ----------------

bool CCitiesDocument::SelectAll()
{
	CCitiesData oCitiesData;
	if ( !oCitiesData.SelectAll( m_oCitiesArray, m_oPersonsArray ) )
		return false;

	return true;
}

bool CCitiesDocument::SelectWhereID( const long lID, CITIES& recCity )
{
	CCitiesData oCitiesData;
	if ( !oCitiesData.SelectWhereID( lID, recCity ) )
		return false;

	return true;
}

bool CCitiesDocument::UpdateWhereID( const long lID, const CITIES& recCity )
{
	CCitiesData oCitiesData;
	if ( !oCitiesData.UpdateWhereID( lID, recCity ) )
		return false;

	for (int i = 0; i < m_oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = m_oCitiesArray.GetAt(i);
		if (!pCity)
			return false;

		if ( pCity->lID == lID)
		{
			*pCity = recCity;
			break;
		}
	}

	CUpdateHint oHint( recCity );
	UpdateAllViews( NULL, ViewModify, &oHint );

	return true;
}

bool CCitiesDocument::Insert( CITIES& recCity )
{
	CCitiesData oCitiesData;
	if ( !oCitiesData.Insert( recCity ) )
		return false;

	m_oCitiesArray.Add( new CITIES( recCity ) );

	CUpdateHint oHint( recCity );

	UpdateAllViews(NULL , ViewInsert, &oHint);

	return true;
}

bool CCitiesDocument::DeleteWhereID( const long lID )
{
	CCitiesData oCitiesData;
	if ( !oCitiesData.DeleteWhereID( lID ) )
		return false;

	for (int i = 0; i < m_oCitiesArray.GetCount(); i++)
	{
		if ( m_oCitiesArray.GetAt(i)->lID == lID )
		{
			m_oCitiesArray.DeleteElementAt( i );
			break;
		}
	}

	CITIES recCity( lID );
	CUpdateHint oHint( recCity );
	UpdateAllViews( NULL, ViewDelete, &oHint);

	return true;
}

const CCitiesArray& CCitiesDocument::GetCitiesArray() const
{
	return m_oCitiesArray;
}

int CompareCities( void* pCompare,const void* a, const void* b)
{
	if ( !pCompare || !a || !b )
		return 0;

	CQSortContext& oQSortContext = *(CQSortContext*)pCompare;

	CITIES recCity1 = **(CITIES**)a;
	CITIES recCity2 = **(CITIES**)b;

	int nRow1 = -1;
	int nRow2 = -1;

	const CListCtrl& oListCtrl = oQSortContext.GetListControl();
	for (int i = 0; i < oListCtrl.GetItemCount(); i++)
	{
		if (recCity1.lID == oListCtrl.GetItemData(i))
		{
			nRow1 = i;
		}

		if (recCity2.lID == oListCtrl.GetItemData(i))
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

void CCitiesDocument::SortArray(const CQSortContext& oQSortContext)
{
	qsort_s( m_oCitiesArray.GetData(), m_oCitiesArray.GetCount(), sizeof(CITIES*), CompareCities, (void*)&oQSortContext);
}



BOOL CCitiesDocument::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() )
		return FALSE;

	if ( !SelectAll() )
		return FALSE;

	// (SDI documents will reuse this document)

	return TRUE;
}

// CCitiesDocument serialization

void CCitiesDocument::Serialize(CArchive& ar)
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

void CCitiesDocument::SetTitle(LPCTSTR lpszTitle)
{
	CDocument::SetTitle(lpszTitle);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCitiesDocument::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCitiesDocument::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCitiesDocument::SetSearchContent(const CString& value)
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

// CCitiesDocument diagnostics

#ifdef _DEBUG
void CCitiesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCitiesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCitiesDocument commands
