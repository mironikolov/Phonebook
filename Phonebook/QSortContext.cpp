#include "stdafx.h"
#include "QSortContext.h"

CQSortContext::CQSortContext(const CListCtrl& oListCtrl, bool bAsc, int nColumn)
	: m_oListCtrl(oListCtrl)
	, m_bAsc(bAsc)
	, m_nColumn(nColumn) 
{
}

const CListCtrl& CQSortContext::GetListControl()
{
	return m_oListCtrl;
}

const bool CQSortContext::GetOrder()
{
	return m_bAsc;
}

const int CQSortContext::GetColumn()
{
	return m_nColumn;
}
