// PhoneTypesView.h : interface of the CPhoneTypesView class
//
#pragma once

#include "PhoneTypesDocument.h"
#include "UpdateHint.h"
#include "PhoneTypesDlg.h"
#include "Enums.h"
#include "QSortContext.h"

class CPhoneTypesView : public CListView
{
protected: // create from serialization only
	CPhoneTypesView() noexcept;
	DECLARE_DYNCREATE(CPhoneTypesView)

	// Attributes
public:
	CPhoneTypesDocument* GetDocument() const;

	// Operations
public:

	// Methods
	// ----------------
private:
	void SetRowData(const PHONE_TYPES& recPhoneType, UpdateViewHint eUpdateViewHint);

	void InsertViewColumns();

	void FillListCtrl();

	int GetItemIndex(const PHONE_TYPES& recPhoneType);

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
	virtual ~CPhoneTypesView();
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

#ifndef _DEBUG  // debug version in PhoneTypesView.cpp
inline CPhoneTypesDocument* CPhoneTypesView::GetDocument() const
{
	return reinterpret_cast<CPhoneTypesDocument*>(m_pDocument);
}
#endif