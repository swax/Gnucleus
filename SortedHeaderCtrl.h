#if !defined(AFX_SORTEDHEADERCTRL_H__3D138C13_BE82_11D1_B971_ACBC3F000000__INCLUDED_)
#define AFX_SORTEDHEADERCTRL_H__3D138C13_BE82_11D1_B971_ACBC3F000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSortedHeaderCtrl : public CHeaderCtrl
{
public:
	CSortedHeaderCtrl();
	virtual ~CSortedHeaderCtrl();

	int SetSortImage(int nCol, BOOL bAsc);
	BOOL IsAscSorted();
	int GetSortedColumn();
	static const int NOT_SORTED;
	void SetOwnerDraw(int nCol);


protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	int m_nSortCol;
	BOOL m_bSortAsc;


	//{{AFX_VIRTUAL(CSortedHeaderCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSortedHeaderCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTEDHEADERCTRL_H__3D138C13_BE82_11D1_B971_ACBC3F000000__INCLUDED_)
