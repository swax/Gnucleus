/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2002 John Marshall

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	For support, questions, comments, etc...
	E-Mail:
		swabby@c0re.net

********************************************************************************/


#include "math.h"
#include "stdafx.h"
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "BuildMap.h"
//#include "Evolve.h"

#include "AutCore.h"
#include "AutPrefs.h"
#include "AutCache.h"
#include "AutNetwork.h"
#include "AutShare.h"
#include "AutUpdate.h"

#include "ChatControl.h"
#include "ChatServer.h"
#include "ChatPrefs.h"

#include "PrefsEx.h"
#include "PrefDialog.h"

#include "ViewConnect.h"
#include "ViewShare.h"
#include "ViewSearch.h"
#include "SearchAdvanced.h"
#include "ViewTransfers.h"
#include "ViewStatistics.h"
#include "ViewChat.h"
#include "ViewBrowser.h"

#include "FrameConnect.h"
#include "FrameStatistics.h"
#include "FrameShare.h"
#include "FrameSearch.h"
#include "FrameTransfers.h"
#include "FrameChat.h"
#include "FrameBrowser.h"

#include "SchedulerDlg.h"
#include "WebCacheDlg.h"
#include "SearchMeta.h"

#include "FrameMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CFrameMain, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CFrameMain, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CFrameMain)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_AUTOCONNECT, OnFileAutoconnect)
	ON_COMMAND(ID_VIEW_CONNECTIONS, OnViewConnections)
	ON_COMMAND(ID_FILE_DISCONNECT, OnFileDisconnect)
	ON_COMMAND(ID_VIEW_PREFERENCES, OnViewPreferences)
	ON_COMMAND(ID_VIEW_STATISTICS, OnViewStatistics)
	
	ON_WM_TIMER()
	ON_MESSAGE(WM_TRAYICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(ID_FILE_LOAD_CONFIG, OnFileLoadConfig)
	ON_COMMAND(ID_FILE_LOAD_NODES, OnFileLoadNodes)
	ON_COMMAND(ID_VIEW_SHARE, OnViewShare)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_TRANSFERS, OnViewTransfers)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_COMMAND(ID_HELP_VERSIONCHECK, OnHelpVersioncheck)
	ON_COMMAND(ID_HELP_BUG, OnHelpBug)
	ON_COMMAND(ID_HELP_CHANGELOG, OnHelpChangelog)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_FILE_DISCONNECT, OnUpdateFileDisconnect)
	ON_COMMAND(ID_VIEW_CHAT, OnViewChat)
	ON_COMMAND(ID_FILE_MAP, OnFileMap)
	ON_COMMAND(ID_GOTO_SEARCH, OnGotoSearch)
	ON_COMMAND(ID_VIEW_WINDOWLIST, OnViewWindowlist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WINDOWLIST, OnUpdateViewWindowlist)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_BROWSER, OnViewBrowser)
	ON_COMMAND(ID_TOOLS_BROWSENETWORK, OnToolsBrowsenetwork)
	ON_COMMAND(ID_TOOLS_SCHEDULER, OnToolsScheduler)
	ON_COMMAND(ID_HELP_REPORTGNUCACHE, OnHelpReportgnucache)
	ON_COMMAND(ID_TOOLS_METASEARCH, OnToolsMetaSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,     // status line indicator
	ID_INDICATOR_RECV,
	ID_INDICATOR_SEND
};


CFrameMain::CFrameMain(CGnucleusDoc* pDoc)
{
	m_pDoc   = pDoc;
	m_pApp   = m_pDoc->m_pApp;

	m_autCore	 = m_pDoc->m_autCore;
	m_autPrefs   = m_pDoc->m_autPrefs;
	m_autCache	 = m_pDoc->m_autCache;
	m_autNetwork = m_pDoc->m_autNetwork;
	m_autShare   = m_pDoc->m_autShare;
	
	m_pChat     = m_pDoc->m_pChat;
	m_pPrefsEx  = m_pDoc->m_pPrefsEx;
	m_pBuildDlg = NULL;

	m_GnucInTray  = false;

	memset(&m_tnd, 0, sizeof(m_tnd));
}

CFrameMain::~CFrameMain()
{
	TRACE0("*** CFrameMain Deconstructing\n");

	// m_pDialog will destroy itself
}


int CFrameMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Do this early in the process
	m_suTaskbarRestart = RegisterWindowMessage(_T("TaskbarCreated"));
	ASSERT(m_suTaskbarRestart); // Harmless, but we won't get the messages.


	// Tool bar with icons
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);


	// Search dialog bar
	if (!m_SearchToolbar.Create(this, IDR_MAINFRAME,
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}


	if (!m_wndWindowListDlgBar.Create(this, IDD_WINDOW_LIST,
		CBRS_ALIGN_TOP | CBRS_FLOATING, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create WindowList dialogbar\n");
		return -1;		// fail to create
	}
	

	// Re-bar to hold toolbar with icons and search dialog bar
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_SearchToolbar) ||
		!m_wndReBar.AddBar(&m_wndWindowListDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}


	// Status bar at bottom of scrren
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	else
	{
		m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_RECV, SBPS_NORMAL, 140);
		m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_SEND, SBPS_NORMAL, 125);
	}


	// Tray Icon
	if(m_autPrefs->GetLanMode())
	{
		m_TrayIconOn  = IDR_TRAYON;
		m_TrayIconOff = IDR_TRAYOFF;
	}
	else
	{
		m_TrayIconOn  = IDR_TRAYON_LAN;
		m_TrayIconOff = IDR_TRAYOFF_LAN;
	}

	if (!CreateTrayIcon(this, WM_TRAYICON_NOTIFY, _T("Gnucleus"), AfxGetApp()->LoadIcon(m_TrayIconOff), IDR_TRAY_RCLICK))
	{
		TRACE0("Failed to create system tray icon.\n");
		return -1;      // fail to create
	}


	m_SearchToolbar.EnableSearch(false);

	m_GlobalTimerID  = SetTimer(1,    1000, NULL);
	m_VersionTimerID = SetTimer(2, 3600000, NULL);
	m_ChatTimerID	  = SetTimer(3,   1000, NULL);



	// Load misc window information
	RECT rc;
	rc.left   = AfxGetApp()->GetProfileInt("FrameMain", "Left",   -1);
	rc.top    = AfxGetApp()->GetProfileInt("FrameMain", "Top",    -1);
	rc.right  = AfxGetApp()->GetProfileInt("FrameMain", "Right",  -1);
	rc.bottom = AfxGetApp()->GetProfileInt("FrameMain", "Bottom", -1);

	if (rc.left != -1 && rc.top != -1 && rc.right != -1 && rc.bottom != -1)
	{
		int scr_cx=GetSystemMetrics(SM_CXSCREEN);
		int scr_cy=GetSystemMetrics(SM_CYSCREEN);

		if (rc.left > scr_cx)
			rc.left = -1;
		if (rc.top > scr_cy)
			rc.top = -1;
		if (rc.right < 0)
			rc.right = -1;
		if (rc.bottom < 0)
			rc.bottom = -1;
	
		if (rc.left != -1 && rc.top != -1 && rc.right != -1 && rc.bottom != -1)
			SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
	}
	
	// Determine if list is displayed or not
	if( !AfxGetApp()->GetProfileInt("FrameMain", "WindowList", true) )
		ShowControlBar(&m_wndWindowListDlgBar, false, false);

	// Determine if media is displayed or not
	//if( AfxGetApp()->GetProfileInt("FrameMain", "MediaToolbar", true) )
	//	ShowControlBar(&m_wndMediaToolBar, true, false);

	return 0;
}
 

void CFrameMain::OnDestroy() 
{
	TRACE0("*** CFrameMain::OnDestroy() Called\n");

	KillTimer(m_GlobalTimerID);
	KillTimer(m_VersionTimerID);

	m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);

	// Save misc window information
	if (!IsIconic())
	{
		if(!IsZoomed())
		{
			RECT rc;
			GetWindowRect(&rc);
			
			AfxGetApp()->WriteProfileInt("FrameMain", "Left",	rc.left);
			AfxGetApp()->WriteProfileInt("FrameMain", "Top",	rc.top);
			AfxGetApp()->WriteProfileInt("FrameMain", "Right",	rc.right);
			AfxGetApp()->WriteProfileInt("FrameMain", "Bottom", rc.bottom);
		}

		AfxGetApp()->WriteProfileString("FrameMain", "State", IsZoomed() ? "maximized" : "normal");
	}

	// Actually destroy the window
	CMDIFrameWnd::OnDestroy();
}
  

BOOL CFrameMain::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}


#ifdef _DEBUG
void CFrameMain::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CFrameMain::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


void CFrameMain::OnFileAutoconnect()
{
	OnViewConnections();

	m_autCore->Connect();

	m_pChat->m_AutoConnect = true;
}

void CFrameMain::OnFileDisconnect()
{
	m_autCore->Disconnect();

	m_pChat->m_AutoConnect		= false;


	m_SearchToolbar.EnableSearch(false);

	m_tnd.hIcon = AfxGetApp()->LoadIcon(m_TrayIconOff);
	Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}


void CFrameMain::OnUpdateFileDisconnect(CCmdUI* pCmdUI)
{
	if(m_autCore->IsConnecting())
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CFrameMain::OnFileLoadConfig()
{
	CFileDialog dlgOpenFile(true);

	dlgOpenFile.m_ofn.lpstrTitle = "Load Gnucleus Config File";
	dlgOpenFile.m_ofn.lpstrFilter = "Gnucleus Config\0*.ini\0";

	if( dlgOpenFile.DoModal() == IDOK )
		m_autPrefs->LoadConfig( dlgOpenFile.GetPathName() );
}

void CFrameMain::OnFileLoadNodes()
{
	CFileDialog dlgOpenFile(true);

	dlgOpenFile.m_ofn.lpstrTitle = "Load Gnucleus Cache File";
	dlgOpenFile.m_ofn.lpstrFilter = "Gnucleus Cache\0*.net\0";

	if( dlgOpenFile.DoModal() == IDOK )
		m_autCache->LoadCache(dlgOpenFile.GetPathName());
}

void CFrameMain::OnViewConnections()
{
	if( m_pDoc->m_pViewConnect == NULL)
	{
		CFrameConnect* m_pWindowConnect = (CFrameConnect*) m_pApp->m_pConnectTemplate->CreateNewFrame(m_pDoc, NULL);
		m_pApp->m_pConnectTemplate->InitialUpdateFrame(m_pWindowConnect, m_pDoc);
	}
	else
	{
		FromHandle(m_pDoc->m_pViewConnect)->GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
		FromHandle(m_pDoc->m_pViewConnect)->GetParentFrame()->BringWindowToTop();
	}
}

void CFrameMain::OnViewShare()
{
	if( m_pDoc->m_pViewShare == NULL)
	{
		CFrameShare* m_pWindowShare = (CFrameShare*) m_pApp->m_pShareTemplate->CreateNewFrame(m_pDoc, NULL);
		m_pApp->m_pShareTemplate->InitialUpdateFrame(m_pWindowShare, m_pDoc);
	}
	else
	{
		FromHandle(m_pDoc->m_pViewShare)->GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
		FromHandle(m_pDoc->m_pViewShare)->GetParentFrame()->BringWindowToTop();
	}
}

void CFrameMain::OnViewTransfers()
{
	if( m_pDoc->m_pViewTransfers == NULL)
	{
		CFrameTransfers* m_pWindowTransfers = (CFrameTransfers*) m_pApp->m_pTransfersTemplate->CreateNewFrame(m_pDoc, NULL);
		m_pApp->m_pTransfersTemplate->InitialUpdateFrame(m_pWindowTransfers, m_pDoc);
	}
	else
	{
		FromHandle(m_pDoc->m_pViewTransfers)->GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
		FromHandle(m_pDoc->m_pViewTransfers)->GetParentFrame()->BringWindowToTop();
	}
}

void CFrameMain::OnViewChat()
{
	if( m_pDoc->m_pViewChat == NULL)
	{
		CFrameChat* m_pWindowChat = (CFrameChat*) m_pApp->m_pChatTemplate->CreateNewFrame(m_pDoc, NULL);
		m_pApp->m_pChatTemplate->InitialUpdateFrame(m_pWindowChat, m_pDoc);

		if(m_pChat->m_pPrefs->m_Nick.Find("Gnuwt") != -1)
		{	
			AfxMessageBox("Please change your default name, 'Gnuwt', to something else");
		
			m_pDoc->m_nLastPref = PREF_CHAT;
			AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);		
		}
	}
	else
	{
		FromHandle(m_pDoc->m_pViewChat)->GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
		FromHandle(m_pDoc->m_pViewChat)->GetParentFrame()->BringWindowToTop();
	}
}

void CFrameMain::OnViewStatistics()
{
	CFrameStatistics* m_pWindowStatistics = (CFrameStatistics*) m_pApp->m_pStatisticsTemplate->CreateNewFrame(m_pDoc, NULL);
	m_pApp->m_pStatisticsTemplate->InitialUpdateFrame(m_pWindowStatistics, m_pDoc);
}

void CFrameMain::OnViewPreferences()
{
	CPrefDialog PrefDlg("Preferences");
	PrefDlg.DoModal();
}

void CFrameMain::OnTimer(UINT nIDEvent)
{

	//Check for gnucleus update, called every hour
	if(nIDEvent == m_VersionTimerID)
	{
		// Version check
		//if(m_pPrefs->m_Update == UPDATE_RELEASE || m_pPrefs->m_Update == UPDATE_BETA)
		//	m_pDoc->CheckVersion();

		
		if(COleDateTime::GetCurrentTime() - COleDateTimeSpan(m_autCore->GetUptime()) > COleDateTimeSpan(0, 0, 50, 0))
		{
			// Check if IRC Cache needs more help
			if(m_autNetwork->ClientMode() == CLIENT_ULTRAPEER)
				if(m_pChat->m_ServerList.size() == 0)
					m_pChat->GetConnect();
		}
	}

	// Called every second
	if(nIDEvent == m_GlobalTimerID)
	{
		m_wndStatusBar.SetPaneText(1, "Receive Rate:  " + InsertDecimal((double) m_autCore->GetBytesPerSecDown() / (double) 1024) + " KB/s");
		m_wndStatusBar.SetPaneText(2, "Send Rate:  "    + InsertDecimal((double) m_autCore->GetBytesPerSecUp()  / (double) 1024) + " KB/s");

		if(m_pDoc->m_autShare->IsLoading())
			m_wndStatusBar.SetPaneText(0, CommaIze(DWrdtoStr(m_pDoc->m_autShare->GetFileCount())) + " Files Loaded");
		else
			m_wndStatusBar.SetPaneText(0, "");

			
		int Connected = m_autNetwork->GetNormalConnectedCount();

		// Update tray and search controls accordingly
		if(Connected)
		{
			m_SearchToolbar.EnableSearch(true);
			m_tnd.hIcon = AfxGetApp()->LoadIcon(m_TrayIconOn);
		}
		else
		{
			m_SearchToolbar.EnableSearch(false);
			m_tnd.hIcon = AfxGetApp()->LoadIcon(m_TrayIconOff);
		}

		// If tray is in alter mode, alternate icon
		if(m_pChat->m_NickAlert && m_AlertSignal)
			m_tnd.hIcon = AfxGetApp()->LoadIcon(IDR_TRAY_ALERT);

		m_AlertSignal = !m_AlertSignal;

		
		Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
	}
	
	if(nIDEvent == m_ChatTimerID)
	{
		m_pChat->Timer();
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CFrameMain::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWnd::OnSize(nType, cx, cy);

	if(nType == SIZE_MINIMIZED && m_pPrefsEx->m_TrayOnMin)
	{
		m_GnucInTray = true;

		ShowWindow( SW_HIDE );
		ModifyStyleEx(WS_EX_APPWINDOW, 0);
	}
}

// Tray Functions
BOOL CFrameMain::CreateTrayIcon(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, HICON icon, UINT menuID)
{
    // Make sure we avoid conflict with other messages
    ASSERT(uCallbackMessage >= WM_USER);

    // Tray only supports tooltip text up to 64 characters
    ASSERT(_tcslen(szToolTip) <= 64);

    // load up the NOTIFYICONDATA structure
    m_tnd.cbSize = sizeof(NOTIFYICONDATA);
    m_tnd.hWnd   = pParent->GetSafeHwnd();
    m_tnd.uID    = menuID;
    m_tnd.hIcon  = icon;
    m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_tnd.uCallbackMessage = uCallbackMessage;
    _tcscpy(m_tnd.szTip, szToolTip);

    // Set the tray icon
    return Shell_NotifyIcon(NIM_ADD, &m_tnd);
}


// 2001-05-11 - Scott Kirkwood <nl>
//   Added SetForgroundWindow() before TrackPopupMenu() call as per
//   KB article Q135788.
//
//
LRESULT CFrameMain::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (wParam != m_tnd.uID)
        return NULL;

	CMenu menu;
	CMenu *pMenu = NULL;
	CPoint point;
	int res;


	switch(lParam)
	{
	case WM_RBUTTONUP:

		menu.LoadMenu(IDR_TRAY_RCLICK);
		pMenu = menu.GetSubMenu(0);
		ASSERT(pMenu != NULL);

		// Display and track the popup menu
		GetCursorPos(&point);

		SetForegroundWindow(); // Make ourselves the active window, first

		res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD), point.x, point.y, this);

		PostMessage(WM_NULL, 0, 0); // Send a benine message to ourselves.

		SendMessage(WM_COMMAND, res, NULL);
		break;

	case WM_LBUTTONDBLCLK:

		//if(m_GnucInTray)
		//{
			ShowWindow(SW_RESTORE);

			m_pChat->m_NickAlert  = false;
			m_GnucInTray = false;
		//}

		AfxGetMainWnd()->SetForegroundWindow();
		break;

	case WM_CREATE:
		//AfxMessageBox("Tray created");

		/*if (!CreateTrayIcon(this, WM_TRAYICON_NOTIFY, _T("Gnucleus"), AfxGetApp()->LoadIcon(m_IconTrayOff), IDR_TRAY_RCLICK))
		{
			TRACE0("*** Failed to create system tray icon.\n");
			return -1;      // fail to create
		}*/
		break;
	}

	return NULL;
}

void CFrameMain::OnButtonSearch()
{
	CString search;
	m_SearchToolbar.GetDlgItem(IDC_COMBO_SEARCH)->GetWindowText(search);

	// Check for invalid lengths
	if(search.GetLength() > 255 || search.GetLength() == 0)
		return;


	// Check to make sure search doesnt already exist
	CString Title;
	POSITION pos = m_pDoc->GetFirstViewPosition();

	while (pos != NULL)
	{
		CView* pView = m_pDoc->GetNextView(pos);
		pView->GetParentFrame()->GetWindowText(Title);

		if(Title.Find("Finding - " + search + ",") == 0)
		{
			pView->GetParentFrame()->BringWindowToTop();
			return;
		}
	}

	// Create Window
	CViewSearch* pSearch = CreateSearchWindow();

	// Send Query
	pSearch->m_Search = search;
	pSearch->SendQuery();
	
	// Add to combobox
	CComboBox* pCombo = (CComboBox*) m_SearchToolbar.GetDlgItem(IDC_COMBO_SEARCH);

	if(pCombo->FindStringExact(0, search) == CB_ERR)
	{
		if(pCombo->GetCount() > 9)
			pCombo->DeleteString(9);

		pCombo->InsertString(0, search);
	}
}

CViewSearch* CFrameMain::CreateSearchWindow()
{
	CFrameSearch* m_pWindowSearch = (CFrameSearch*) m_pApp->m_pSearchTemplate->CreateNewFrame(m_pDoc, NULL);
	m_pApp->m_pSearchTemplate->InitialUpdateFrame(m_pWindowSearch, m_pDoc);

	CViewSearch* pSearch = ((CViewSearch*) m_pWindowSearch->GetActiveView());

	return pSearch;
}

BOOL CFrameMain::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 1 && lParam == 0)
		OnButtonSearch();

	return CMDIFrameWnd::OnCommand(wParam, lParam);
}

void CFrameMain::OnHelpVersioncheck()
{
	m_pDoc->m_autUpdate->Check();
	m_pDoc->DisplayEvolver();
}

void CFrameMain::OnHelpChangelog()
{
	OpenBrowser(m_pDoc->m_RunPath + "Docs\\ChangeLog.htm");
}

void CFrameMain::OnHelpBug()
{
	AfxMessageBox("Please be as descriptive as possible with the bug you found.\n\nInclude what you were doing with Gnucleus at the time of the error.\n\nAlso include:\n\tVersion of Gnucleus\n\tVersion of Windows\n\tSpeed of your connection to the internet\n\tIf you are behind a firewall or not\n\nThank you, with this information we can investigate your problem faster.\n\n***Please note, Feature Requests have their own area on SourceForge***");

	OpenBrowser(_T("http://www.c0re.net/gnuforum/index.php"));
}

void CFrameMain::OnClose()
{
	TRACE0("*** CFrameMain::OnClose Called\n");

	if(!m_pDoc->m_ShutDown && m_pPrefsEx->m_TrayOnClose && !m_GnucInTray)
	{
		ShowWindow(SW_MINIMIZE);
		m_GnucInTray = true;

		return;
	}

	CMDIFrameWnd::OnClose();
}


CFrameBrowser* CFrameMain::OpenBrowser(CString Address)
{
	CFrameBrowser* m_pWindowBrowser = NULL;

	try
	{
		if(!m_pDoc)
		{
			ASSERT(m_pDoc);
			return NULL;
		}
		m_pDoc->m_CurrentSite = Address;

		if(!m_pApp || !m_pDoc)
		{
			ASSERT(0);
			return NULL;
		}

	
		m_pWindowBrowser = (CFrameBrowser*) m_pApp->m_pBrowserTemplate->CreateNewFrame(m_pDoc, NULL);
		
		if(!m_pWindowBrowser || !m_pApp->m_pBrowserTemplate)
		{
			ASSERT(m_pWindowBrowser);
			// If it failed, do the default thing.
			::ShellExecute(NULL, "open", Address, NULL, NULL, SW_SHOW);
			return NULL;
		}

		m_pApp->m_pBrowserTemplate->InitialUpdateFrame(m_pWindowBrowser, m_pDoc);
	}
	catch(...)
	{
		// If it failed, do the default thing.
		::ShellExecute(NULL, "open", Address, NULL, NULL, SW_SHOW);
		return NULL;
	}

	return m_pWindowBrowser;
}



void CFrameMain::OnFileMap()
{
	if(m_pBuildDlg)
	{
		m_pBuildDlg->ShowWindow(SW_SHOW);
		m_pBuildDlg->BringWindowToTop();
	}
	else
	{
		m_pBuildDlg = new CBuildMap(NULL, this);
		m_pBuildDlg->Create(IDD_BUILD_MAP);

		m_pBuildDlg->ShowWindow(SW_SHOW);
	}
}


LRESULT CFrameMain::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_suTaskbarRestart)
		VERIFY(CreateTrayIcon(this, WM_TRAYICON_NOTIFY, _T("Gnucleus"), AfxGetApp()->LoadIcon(m_TrayIconOff), IDR_TRAY_RCLICK));

	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}


void CFrameMain::OnGotoSearch()
{
	CWnd* pWnd = m_SearchToolbar.GetDlgItem(IDC_COMBO_SEARCH);
	ASSERT(pWnd);
	if (pWnd)
	{
		pWnd->SetFocus();
	}
}

void CFrameMain::SearchFromConnect()
{
	CWnd* pWnd = m_SearchToolbar.GetDlgItem(IDC_COMBO_SEARCH);

	pWnd->SetFocus();
	pWnd->SetWindowText("*** Enter Search Here ***");
}


void CFrameMain::OnViewWindowlist()
{		
	// From the WinFrm.cpp in MFC sources
	if(m_wndWindowListDlgBar.IsWindowVisible())
	{
		ShowControlBar(&m_wndWindowListDlgBar, FALSE, FALSE);
		AfxGetApp()->WriteProfileInt("FrameMain", "WindowList",	false);
	}
	else
	{
		ShowControlBar(&m_wndWindowListDlgBar, TRUE, FALSE);
		AfxGetApp()->WriteProfileInt("FrameMain", "WindowList",	true);
	}
}


void CFrameMain::OnUpdateViewWindowlist(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndWindowListDlgBar.IsWindowVisible());
}



void CFrameMain::OnViewBrowser() 
{
	OpenBrowser("http://www.gnucleus.net");
}

void CFrameMain::OnToolsBrowsenetwork() 
{
	OpenBrowser("http://127.0.0.1:" + DWrdtoStr(m_autNetwork->GetLocalPort()));
}

void CFrameMain::OnToolsScheduler() 
{
	CSchedulerDlg Scheduler(NULL, m_pDoc);

	Scheduler.DoModal();
}

void CFrameMain::OnHelpReportgnucache() 
{
	CWebCacheDlg CacheDlg(NULL, m_pDoc);

	CacheDlg.DoModal();
}

void CFrameMain::OnToolsMetaSearch()
{
	CSearchMeta SearchDlg(NULL, this);

	SearchDlg.DoModal();
}