#pragma once

class CGnucleusApp;
class CGnucleusDoc;

class CAutCore;
class CAutPrefs;
class CAutCache;
class CAutNetwork;
class CAutShare;

class CFrameConnect;
class CFrameBrowser;
class CViewSearch;

class CBuildMap;
class CChatControl;
class CPrefsEx;


#include "WindowListDlg.h"
#include "SearchToolbar.h"

class CFrameMain : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CFrameMain)

public:
	CFrameMain(CGnucleusDoc* pDoc);
	virtual ~CFrameMain();

	CFrameBrowser* OpenBrowser(CString);

	void SearchFromConnect();
	CViewSearch* CreateSearchWindow();


	CGnucleusApp* m_pApp;
	CGnucleusDoc* m_pDoc;

	CAutCore*     m_autCore;
	CAutPrefs*	  m_autPrefs;
	CAutCache*	  m_autCache;
	CAutNetwork*  m_autNetwork;
	CAutShare*	  m_autShare;

	CWindowListDlg  m_wndWindowListDlgBar; 
	CSearchToolbar  m_SearchToolbar;

	CBuildMap*	  m_pBuildDlg;
	CChatControl* m_pChat;
	CPrefsEx*     m_pPrefsEx;

	//{{AFX_VIRTUAL(CFrameMain)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif


protected:
	BOOL CreateTrayIcon(CWnd*, UINT, LPCTSTR, HICON, UINT);

	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;

	int	m_GlobalTimerID;
	int m_VersionTimerID;
	int m_ChatTimerID;

	NOTIFYICONDATA    m_tnd;
	int				  m_TrayIconOn;
	int				  m_TrayIconOff;

	bool	m_GnucInTray;
	bool	m_AlertSignal;

	UINT              m_suTaskbarRestart; // Registered message ID for "TaskbarCreated"
	
	
	//{{AFX_MSG(CFrameMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileAutoconnect();
	afx_msg void OnViewConnections();
	afx_msg void OnFileDisconnect();
	afx_msg void OnViewPreferences();
	afx_msg void OnViewStatistics();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnTrayNotification(WPARAM, LPARAM);
	afx_msg void OnFileLoadConfig();
	afx_msg void OnFileLoadNodes();
	afx_msg void OnViewShare();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewTransfers();
	afx_msg void OnButtonSearch();
	afx_msg void OnHelpVersioncheck();
	afx_msg void OnHelpBug();
	afx_msg void OnHelpChangelog();
	afx_msg void OnClose();
	afx_msg void OnUpdateFileDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnViewChat();
	afx_msg void OnFileMap();
	afx_msg void OnGotoSearch();
	afx_msg void OnViewWindowlist();
	afx_msg void OnUpdateViewWindowlist(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewBrowser();
	afx_msg void OnToolsBrowsenetwork();
	afx_msg void OnToolsScheduler();
	afx_msg void OnHelpReportgnucache();
	afx_msg void OnToolsMetaSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

