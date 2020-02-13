// PersonsView.h : interface of the CPersonsView class
//
#pragma once

#include "PersonsDocument.h"
#include "UpdateHint.h"
#include "PersonsDlg.h"
#include "QSortContext.h"

enum PersonsColumn
{
	ColumnPersonsName = 0,
	ColumnPersonsSurname = 1,
	ColumnPersonsLastName = 2,
	ColumnPersonsUCN = 3,
	ColumnPersonsCity = 4,
	ColumnPersonsAddress = 5
};

class CPersonsView : public CListView
{
protected: // create from serialization only
	CPersonsView() noexcept;
	DECLARE_DYNCREATE(CPersonsView)

	// Attributes
public:
	CPersonsDocument* GetDocument() const;

	// Operations
public:

	// Methods
	// ----------------
private:
	void SetRowText(const PERSONS& recPerson, UpdateViewHint eUpdateViewHint);

	void InsertViewColumns();

	void FillListCtrl();

	int GetItemIndex( const PERSONS& recPerson );

	int GetSelectedItemIndex();

	void ModifyListControlStyles();

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CPersonsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuModify();
	afx_msg void OnMenuInsert();
	afx_msg void OnMenuDelete();
	afx_msg void OpenListCtrlRecord();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in PersonsView.cpp
inline CPersonsDocument* CPersonsView::GetDocument() const
{
	return reinterpret_cast<CPersonsDocument*>(m_pDocument);
}
#endif

