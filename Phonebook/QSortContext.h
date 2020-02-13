#pragma once

class CQSortContext
{
public:
	CQSortContext(const CListCtrl& oListCtrl, bool bAsc, int nColumn);

	// Methods
	// ----------------
public:
	const CListCtrl& GetListControl();
	const bool GetOrder();
	const int GetColumn();

private:
	const CListCtrl& m_oListCtrl;
	bool m_bAsc;
	int m_nColumn;
};