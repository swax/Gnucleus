#if !defined(AFX_PREFDIALOG_H__A6C36B20_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFDIALOG_H__A6C36B20_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Define the size of the tree control
#define PREF_TREE_WIDTH			100
#define PREF_TREE_MARGIN_TOP	20
#define PREF_TREE_MARGIN_LEFT	15
#define PREF_TREE_MARGIN_BOTTOM	60
#define PREF_TREE_MARGIN_RIGHT	5
#define PREF_TAB_OFFSET_UP		20


class CGnucleusDoc;
class CPrefLocal;
class CPrefLocalNetwork;
class CPrefLocalFirewall;
class CPrefConnect;
class CPrefConnectServers;
class CPrefConnectScreen;
class CPrefSearch;
class CPrefSearchBlock;
class CPrefSearchScreen;
class CPrefShare;
class CPrefTransfer;
class CPrefBandwidth;
class CPrefChat;
class CPrefPlugins;


class CPrefDialog : public CPropertySheet
{
	DECLARE_DYNAMIC(CPrefDialog)

public:
	CPrefDialog(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPrefDialog(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPrefDialog();

	CPrefLocal*			 m_TabLocal;
	CPrefLocalNetwork*	 m_TabLocalNetwork;
	CPrefLocalFirewall*	 m_TabLocalFirewall;
	CPrefConnectServers* m_TabConnectServers;
	CPrefConnectScreen*	 m_TabConnectScreen;
	CPrefSearch*		 m_TabSearch;
	CPrefSearchScreen*	 m_TabSearchScreen;
	CPrefSearchBlock*	 m_TabSearchBlock;
	CPrefShare*			 m_TabShare;
	CPrefTransfer*		 m_TabTransfer;
	CPrefBandwidth*		 m_TabBandwidth;
	CPrefChat*			 m_TabChat;
	CPrefPlugins*		 m_TabPlugins;


	//{{AFX_VIRTUAL(CPrefDialog)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


protected:
	void AddPages();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);


	CGnucleusDoc*		 m_pDoc;


	//{{AFX_MSG(CPrefDialog)
	afx_msg LRESULT OnNotifyFormat(WPARAM wParam,LPARAM lParam);
	afx_msg void OnApplyNow();

	HICON		   m_hIcon;
	CTreeCtrl	   m_Tree;
	CRect		   m_rectPage;

	CPrefConnect*		m_TabConnect;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFDIALOG_H__A6C36B20_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
