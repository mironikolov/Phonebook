
// PersonsDocument.cpp : implementation of the CPersonsDocument class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Phonebook.h"
#endif

#include "PersonsDocument.h"
#include "UpdateHint.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum PersonColumns
{
	PersonColumnName = 0,
	PersonColumnSurname = 1,
	PersonColumnLastName = 2,
	PersonColumnAddress = 5
};

// CCitiesDocument

IMPLEMENT_DYNCREATE(CPersonsDocument, CDocument)

BEGIN_MESSAGE_MAP(CPersonsDocument, CDocument)
END_MESSAGE_MAP()


// CCitiesDocument construction/destruction

CPersonsDocument::CPersonsDocument() noexcept
{
}

CPersonsDocument::~CPersonsDocument()
{
}

// Methods
// ----------------

bool CPersonsDocument::SelectAll()
{
	CCitiesArray oCitiesArray;

	CPhoneTypesArray recPhoneTypesArray;

	CPersonsData oPersonsData;
	if ( !oPersonsData.SelectAll( m_oPersonsArray, oCitiesArray, recPhoneTypesArray) )
		return false;

	for (int i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = new CITIES();
		*pCity = *oCitiesArray.GetAt(i);
		m_oCitiesMap.SetAt( pCity->lID , pCity );
	}

	for (int i = 0; i < recPhoneTypesArray.GetCount(); i++)
	{
		PHONE_TYPES* pPhoneType = recPhoneTypesArray.GetAt(i);
		if (!pPhoneType)
			return false;

		m_oPhoneTypesMap.SetAt( pPhoneType->lID, new PHONE_TYPES( *pPhoneType ) );
	}

	return true;
}

bool CPersonsDocument::SelectWhereID(const long lID, CPersonWithNumbers& oPersonWithNumbers)
{
	CPersonsData oPersonsData;
	if (!oPersonsData.SelectWhereID(lID, oPersonWithNumbers))
		return false;

	return true;
}

bool CPersonsDocument::UpdateWhereID(const long lID, CPersonWithNumbers& oPersonWithNumbers)
{

	CPersonsData oPersonsData;
	if (!oPersonsData.UpdateWhereID(lID, oPersonWithNumbers))
		return false;

	for (int i = 0; i < m_oPersonsArray.GetCount(); i++)
	{
		PERSONS* recPerson = m_oPersonsArray.GetAt(i);
		if (!recPerson)
			return false;

		if (recPerson->lID == lID)
		{
			*recPerson = oPersonWithNumbers.GetPerson();
			break;
		}
	}

	UpdateAllViews(NULL, ViewModify, &oPersonWithNumbers);

	return true;
}

bool CPersonsDocument::Insert( CPersonWithNumbers& oPersonWithNumbers )
{
	CPersonsData oPersonsData;
	if ( !oPersonsData.Insert( oPersonWithNumbers ) )
		return false;

	m_oPersonsArray.Add( new PERSONS( oPersonWithNumbers.GetPerson() ) );

	UpdateAllViews(NULL, ViewInsert, &oPersonWithNumbers);

	return true;
}

bool CPersonsDocument::DeleteWhereID(const long lID)
{
	CPersonsData oPersonsData;
	if (!oPersonsData.DeleteWhereID(lID))
		return false;

	for (int i = 0; i < m_oPersonsArray.GetCount(); i++)
	{
		if (m_oPersonsArray.GetAt(i)->lID == lID)
		{
			m_oPersonsArray.DeleteElementAt(i);
			break;
		}
	}

	CPersonWithNumbers oPersonWithNumbers;
	oPersonWithNumbers.GetPerson().lID = lID;
	UpdateAllViews(NULL, ViewDelete, &oPersonWithNumbers);

	return true;
}

const CPersonsArray& CPersonsDocument::GetPersonsArray() const
{
	return m_oPersonsArray;
}

const CCitiesMap& CPersonsDocument::GetCitiesMap() const
{
	return m_oCitiesMap;
}

const CPhoneTypesMap& CPersonsDocument::GetPhoneTypesMap() const
{
	return m_oPhoneTypesMap;
}

int ComparePersons(void* pQSortContext, const void* a, const void* b)
{
	if (!pQSortContext || !a || !b)
		return 0;

	CQSortContext& oQSortContext = *(CQSortContext*)pQSortContext;
	
	PERSONS recPerson1 = **(PERSONS**)a;
	PERSONS recPerson2 = **(PERSONS**)b;

	int nRow1 = -1;
	int nRow2 = -1;

	const CListCtrl& oListCtrl = oQSortContext.GetListControl();
	for (int i = 0; i < oListCtrl.GetItemCount(); i++)
	{
		if (recPerson1.lID == oListCtrl.GetItemData(i))
		{
			nRow1 = i;
		}

		if (recPerson2.lID == oListCtrl.GetItemData(i))
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

void CPersonsDocument::SortArray(const CQSortContext& oQSortContext)
{
	qsort_s( m_oPersonsArray.GetData(), m_oPersonsArray.GetCount(), sizeof(PERSONS*), ComparePersons, (void*)&oQSortContext );
}

BOOL CPersonsDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (!SelectAll())
		return FALSE;

	// (SDI documents will reuse this document)

	return TRUE;
}

// CCitiesDocument serialization

void CPersonsDocument::Serialize(CArchive& ar)
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

void CPersonsDocument::SetTitle(LPCTSTR lpszTitle)
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
void CPersonsDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CPersonsDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCitiesDocument commands
