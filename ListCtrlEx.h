#if !defined(AFX_LISTCTRLEX_H__106DE3FF_22C3_49A2_9EDD_44B87D1DC45E__INCLUDED_)
#define AFX_LISTCTRLEX_H__106DE3FF_22C3_49A2_9EDD_44B87D1DC45E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ColumnClickMixIn.h"


class CListCtrlEx : public CListCtrl,
	public CColumnClickMixIn<CListCtrlEx>
{
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

	void SetSortColumn(int nColumn)
	{
		CColumnClickMixIn<CListCtrlEx>::SetSortColumn(nColumn);
	}

	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class CDrawItemIter;


class CDrawItemHelper
{
public:
	CDrawItemHelper(LPDRAWITEMSTRUCT lpDrawItem);
	~CDrawItemHelper();

protected:
	friend class CDrawItemIter;
	void Setup();
	void TearDown();

	LPDRAWITEMSTRUCT m_lpDrawItem;
	LV_ITEM m_lvi;
	int m_cxImage;
	int m_cyImage;
	RECT m_rcClip;
	int m_iColumn;
	UINT m_uiFlags;
};


class CDrawItemIter
{
public:
	CDrawItemIter(CDrawItemHelper& drawItemHelper, CObject *pParent=NULL);
	void Begin()
	{
		m_nColumn = 0;
	}
	operator bool() const { return m_nColumn < m_nNumColumns; }
	void operator ++();
	int GetColumnNum() const { return m_nColumn; }

	void DrawCell();
	void DrawIcons();
	const CRect& GetCurRect() const { return m_rcCol; }

protected:
	CDrawItemHelper& m_DrawItemHelper;
	int m_nNumColumns;
	int m_nColumn;
	LV_COLUMN m_lvc;
	LV_ITEM& m_lvi;
	CRect m_rcCol;
	int m_nOffset;

	HDC m_hDc;
	HWND m_hWnd;
	CBrush m_Brush;

	CObject *pParent; //Makslane: object parent
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__106DE3FF_22C3_49A2_9EDD_44B87D1DC45E__INCLUDED_)
