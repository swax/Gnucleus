#if !defined(AFX_WINDOWLISTDLG_H__0B7013C3_9297_45D1_B274_740D6446E388__INCLUDED_)
#define AFX_WINDOWLISTDLG_H__0B7013C3_9297_45D1_B274_740D6446E388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gnucleus.h"

class CWindowListDlg : public CDialogBar
{
public:
	CWindowListDlg();   // standard constructor

	void Activate(CWnd* pWnd);
	void Update(CWnd* pWnd);
	void Close(CWnd* pWnd);

	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);

	void DiscoverWindows();

	enum eIconOrder
	{
		eConnect,
		eShare,
		eTransfers,
		eChat,
		eStatistics,
		eBrowser,
		eSearch
	};


	//{{AFX_DATA(CWindowListDlg)
	enum { IDD = IDD_WINDOW_LIST };
	CTabCtrl	m_ctlWindowList;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CWindowListDlg)
	protected:
	//}}AFX_VIRTUAL


protected:
	int AddNew(CWnd* pWnd);
	int GetIconByWindowType(CWnd* pWnd);
	int MyFind(CWnd* hWnd); 
	int FindSlot(int nIcon);
	CWnd* GetWnd(int nIndex);

	CImageList m_ImageLst;
	unsigned long m_Slots;


	// Generated message map functions
	//{{AFX_MSG(CWindowListDlg)
	afx_msg void OnSelchangeWindowTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWLISTDLG_H__0B7013C3_9297_45D1_B274_740D6446E388__INCLUDED_)
