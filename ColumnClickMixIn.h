#if !defined(AFX_COLUMNCLICKMIXIN_H__F8AA6320_C9C6_11D3_9E3A_0040055DBD34__INCLUDED_)
#define AFX_COLUMNCLICKMIXIN_H__F8AA6320_C9C6_11D3_9E3A_0040055DBD34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SortedHeaderCtrl.h"

////////////////////////////////////////////////////////////////////////////////
// @class   MixIn class for handling sorting in a list view
//  To use this class you need to call PreSubclassWindow in your PreSubclassWindow
//  function.  The default sort routine probably dosn't do what you want, override it
//  with your own version.  You have a choice of making one sort callback per column
//  or one for the whole list, or something in-between.
//  You sort callback should return a comparison assuming ascending, this will be modified
//  if the sort is descending.
// @comm    
// @devnote 
// 
// 2000-01-13 Scott Kirkwood <nl>
//    Created <nl>
// 
// @base    
// 
template <class CParent, class ParamType = LPARAM>
class CColumnClickMixIn  
{
public:
	CColumnClickMixIn();
	virtual ~CColumnClickMixIn();

	enum { k_nMaxColumns = 40 };

	typedef int (CALLBACK *SortCallbackType)(ParamType nIndex1, ParamType nIndex2, CParent* pThis);
	
	LRESULT HandleColumnClick(NMHDR* pNMHDR);
	void SetSortCallback(SortCallbackType pSortFunction, int nColumn = -1);

	const CListCtrl* GetParent() const       { return m_pParent; } 
	void SetParent(CListCtrl* pParent) { m_pParent = pParent; }

	int GetSortWhichColumn() const { return m_nSortWhichColumn; } 

	bool GetSortAscending() const { return m_bSortAscending; } 

	void PreSubclassWindow(CParent* pParent);

	void SetSortColumn(int nCol);
protected:
	static int CALLBACK DefaultSortCallback(LPARAM nIndex1, LPARAM nIndex2, CColumnClickMixIn* pThis);
	static int CALLBACK WrapperSortCallback(LPARAM nIndex1, LPARAM nIndex2, LPARAM nData);
private:
	// @cmember Which column is currently sorted?
	int m_nSortWhichColumn;

	// @cmember Sort ascending or descending?
	bool m_bSortAscending;

	// @cmember Pointer to users sort function
	SortCallbackType m_pSortFunction[k_nMaxColumns];

	// @cmember Pointer back to our parent
	CParent* m_pParent;

	CSortedHeaderCtrl m_HeaderCtrl;
};


////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @comm    
// 
// @devnote 
// 
// 2000-01-13 - Scott Kirkwood <nl>
//   Created <nl>
// 
template <class CParent, class ParamType>
CColumnClickMixIn<CParent, ParamType>::CColumnClickMixIn()
	: m_pParent(0)
{
	m_nSortWhichColumn = 0; // first column
	m_bSortAscending = false;

	for (int i = 0; i < k_nMaxColumns; ++i)
	{
		m_pSortFunction[i] = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @comm    
// 
// @devnote 
// 
// 2000-01-13 - Scott Kirkwood <nl>
//   Created <nl>
// 
template <class CParent, class ParamType>
CColumnClickMixIn<CParent, ParamType>::~CColumnClickMixIn()
{

}

////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @parm    
// @rdesc   
// 
// @comm    
// 
// @devnote 
// 
// 2000-01-13 - Scott Kirkwood <nl>
//   Created <nl>
// 
template <class CParent, class ParamType>
LRESULT CColumnClickMixIn<CParent, ParamType>::HandleColumnClick(NMHDR* pNMHDR)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nCol = pNMListView->iSubItem;

	// If the user clicked on the same column more than once, 
	// change sort direction
	if (nCol == m_nSortWhichColumn)
	{
		// Toggle the sort on this column
		m_bSortAscending = !m_bSortAscending;
	}
	else
	{
		m_bSortAscending = true;
	}

	// First set the sort order and column and draw the header
	// This must be called before SortItems to set sort order and column 
	if ((m_pParent->GetStyle() & LVS_TYPEMASK) == LVS_REPORT) 
	{
		m_HeaderCtrl.SetSortImage(nCol, m_bSortAscending);
	}

	m_nSortWhichColumn = nCol;

	m_pParent->SortItems(WrapperSortCallback, (LPARAM)this);	// Sort the column.

	return 0; // SUCCESS
}


////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @comm    
// 
// @devnote 
// 
// 2000-03-26 - Scott Kirkwood <nl>
//   Created <nl>
// 
template <class CParent, class ParamType>
void CColumnClickMixIn<CParent, ParamType>::PreSubclassWindow(CParent* pParent) 
{
	m_pParent = pParent;

	//Item 0 is the Header Control		
	VERIFY(m_HeaderCtrl.SubclassWindow(::GetDlgItem(m_pParent->m_hWnd,0)));
}

////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @comm    
// 
// @devnote 
// 
// 2000-03-26 - Scott Kirkwood <nl>
//   Created <nl>
// 
template <class CParent, class ParamType>
int CALLBACK CColumnClickMixIn<CParent, ParamType>::WrapperSortCallback(LPARAM nIndex1, LPARAM nIndex2, LPARAM nData)
{
	CColumnClickMixIn* pThis = (CColumnClickMixIn*)nData;
	long iResult = 0;

	SortCallbackType pSortFunc = pThis->m_pSortFunction[pThis->m_nSortWhichColumn];
	if (pSortFunc == NULL)
	{
		iResult = DefaultSortCallback(nIndex1, nIndex2, pThis);
	}
	else
	{
		iResult = pSortFunc((ParamType)nIndex1, (ParamType)nIndex2, (CParent*)pThis);
	}

	return pThis->GetSortAscending() ? iResult : -iResult;
}

template <class CParent, class ParamType>
int CALLBACK CColumnClickMixIn<CParent, ParamType>::DefaultSortCallback(LPARAM nIndex1, LPARAM nIndex2, CColumnClickMixIn* /*pThis*/)
{
	return nIndex1 - nIndex2;
}

////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @parm    Sort function to use, can be one per column
// @parm    -1 indicates that all columns will have this one sort function
// @comm    
// 
// @devnote 
// 
// 2000-04-13 - Scott Kirkwood <nl>
//   Created <nl>
// 
template <class CParent, class ParamType>
void CColumnClickMixIn<CParent, ParamType>::SetSortCallback(SortCallbackType pSortFunction, int nColumn /* = -1 */)
{
	if (nColumn == -1)
	{
		for (int i = 0; i < k_nMaxColumns; ++i)
		{
			m_pSortFunction[i] = pSortFunction;
		}
	}
	else
	{
		m_pSortFunction[nColumn] = pSortFunction;
	}
}

template <class CParent, class ParamType>
void CColumnClickMixIn<CParent, ParamType>::SetSortColumn(int nColumn /* = -1 */)
{
	NM_LISTVIEW NMListView;
	memset(&NMListView, 0, sizeof(NMListView));
	NMListView.iSubItem = nColumn;

	HandleColumnClick((NMHDR*)&NMListView);
}


#endif // !defined(AFX_COLUMNCLICKMIXIN_H__F8AA6320_C9C6_11D3_9E3A_0040055DBD34__INCLUDED_)
