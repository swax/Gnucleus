#if !defined(AFX_BUILDMAP_H__689D1BC0_448A_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_BUILDMAP_H__689D1BC0_448A_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CFrameMain;


class CBuildMap : public CDialog
{
public:
	CBuildMap(CWnd* pParent = NULL, CFrameMain* pFrame = NULL);   // standard constructor

	CString GetDotSize(DWORD);

	CGnucleusDoc* m_pDoc;
	CFrameMain*	  m_pFrame;


	//{{AFX_DATA(CBuildMap)
	enum { IDD = IDD_BUILD_MAP };
	CButton	m_chkShapeNodes;
	CEdit	m_ebWidth;
	CEdit	m_ebHeight;
	CButton	m_btnBuild;
	CButton	m_btnRefresh;
	CEdit	m_ebHops;
	CButton	m_chkSizeNodes;
	CStatic	m_stcNodes;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBuildMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:
	void AddDetail(DWORD);

	UINT m_PongsTimerID;

	bool m_Building;
	int  m_TimeBuilding;

	std::list<DWORD> m_DetailList;


	// Generated message map functions
	//{{AFX_MSG(CBuildMap)
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonBuild();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRead();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDMAP_H__689D1BC0_448A_11D5_ACF2_00A0CC533D52__INCLUDED_)
