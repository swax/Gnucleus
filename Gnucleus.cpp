/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2004 John Marshall

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


#include "stdafx.h"
#include "GnucleusDoc.h"
#include "AutPrefs.h"
#include "AutCore.h"
#include "AutDownload.h"

#include "FrameMain.h"
#include "FrameConnect.h"
#include "FrameStatistics.h"
#include "FrameShare.h"
#include "FrameSearch.h"
#include "FrameTransfers.h"
#include "FrameChat.h"
#include "FrameBrowser.h"

#include "ViewConnect.h"
#include "ViewStatistics.h"
#include "ViewShare.h"
#include "ViewSearch.h"
#include "ViewTransfers.h"
#include "ViewChat.h"
#include "ViewBrowser.h"
#include "NetSetup.h"

#include "Gnucleus.h"
#include "afxwin.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CGnucleusApp, CWinApp)
	//{{AFX_MSG_MAP(CGnucleusApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()


CGnucleusApp::CGnucleusApp()
{
	m_pDoc = NULL;

	m_pConnectTemplate    = NULL;
	m_pStatisticsTemplate = NULL;
	m_pShareTemplate	  = NULL;
	m_pTransfersTemplate  = NULL;
	m_pSearchTemplate     = NULL; 
	m_pChatTemplate       = NULL;
	m_pBrowserTemplate	  = NULL;

	m_Min = false;
}

CGnucleusApp::~CGnucleusApp()
{
	TRACE0("*** CGnucleusApp Deconstructing\n");
}

// The one and only CGnucleusApp object
CGnucleusApp theApp;



BOOL CGnucleusApp::InitInstance()
{	
	// Make sure this thread runs on the first CPU
	AssignThreadToCPU(AfxGetThread(), CPU_0);

	
	// Enable tracing if we are in debug mode
	#ifdef _DEBUG
		::afxTraceEnabled = true;
	#endif


	// Winsock Init
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return false;
	}


	// RichEdit Init
	if(!AfxInitRichEdit())
	{
		AfxMessageBox("Could not initialize RTF Control");
		return false;
	}
	

	// Enable containing controls, activex stuff
	AfxEnableControlContainer();


	// Enabe the use of OLE/COM objects
	if (!AfxOleInit())
	{
		AfxMessageBox("OLE Init Failed");
		return false;
	}


	// Depreciated Funtions in MFC 5.0
	#if _MSC_VER < 1300
		#ifdef _AFXDLL
			Enable3dControls();			// MFC in a shared DLL
		#else
			Enable3dControlsStatic();	// MFC static
		#endif
	#endif

	
	// Seed random generator
	srand( (unsigned)time(NULL) );


	// Process CommandLine
	CMyCmdLineInfo cmdinfo;
	cmdinfo.m_App = this;
	cmdinfo.m_Minimize = false;

	ParseCommandLine(cmdinfo);
	m_Min = cmdinfo.m_Minimize;


	// Load view templates and main document object
	LoadTemplates();
	
	m_pDoc = new CGnucleusDoc(this);


	// Check if we can run multiple instances
	if(InstanceRunning())
	{
		ReleaseMutex(m_hMutex);
		
		if(AfxMessageBox("Gnucleus is already running, click Yes to close this instance,\nthen check the tray for Gnucleus already running", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			return false;
	}


	// Init the main window
	CFrameMain* pMainFrame = new CFrameMain(m_pDoc);
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		AfxMessageBox("Could not initialize CMainFrame");
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	


	// Hide window at start up
	if(cmdinfo.m_Minimize)
		pMainFrame->ShowWindow(SW_MINIMIZE);
	else
	{
		CString state = AfxGetApp()->GetProfileString("FrameMain", "State", "maximized");
		pMainFrame->ShowWindow(state == "maximized" ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
	}


	// Size child windows
	SendMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_CONNECTIONS, NULL);

	if(!m_Min)
		SendMessage(m_pMainWnd->m_hWnd, WM_COMMAND, ID_WINDOW_CASCADE, NULL);


	return true;
}

bool CGnucleusApp::InstanceRunning()
{
	m_hMutex = CreateMutex(NULL, false, "Gnucleus_Mutex");
	
	if(m_hMutex)
	{
		DWORD dwWait = WaitForSingleObject(m_hMutex, 10);

		switch(dwWait)
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			return true;
		case WAIT_FAILED:
			break;
		}
	}

	return false;
}

void CGnucleusApp::LoadTemplates()
{
	// Connect window
	m_pConnectTemplate = new CMultiDocTemplate(
		IDR_CONNECT,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameConnect), 
		RUNTIME_CLASS(CViewConnect));
	AddDocTemplate(m_pConnectTemplate);

	// Statistics window
	m_pStatisticsTemplate = new CMultiDocTemplate(
		IDR_STATISTICS,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameStatistics),	
		RUNTIME_CLASS(CViewStatistics));	
	AddDocTemplate(m_pStatisticsTemplate);

	// Share window
	m_pShareTemplate = new CMultiDocTemplate(
		IDR_SHARE,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameShare),
		RUNTIME_CLASS(CViewShare));
	AddDocTemplate(m_pShareTemplate);

	// Transfers window
	m_pTransfersTemplate = new CMultiDocTemplate(
		IDR_TRANSFERS,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameTransfers),
		RUNTIME_CLASS(CViewTransfers));
	AddDocTemplate(m_pTransfersTemplate);

	// Search window
	m_pSearchTemplate = new CMultiDocTemplate(
		IDR_SEARCH,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameSearch),
		RUNTIME_CLASS(CViewSearch));
	AddDocTemplate(m_pSearchTemplate);

	// Chat window
	m_pChatTemplate = new CMultiDocTemplate(
		IDR_CHAT,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameChat),
		RUNTIME_CLASS(CViewChat));
	AddDocTemplate(m_pChatTemplate);

	// Browser window
	m_pBrowserTemplate = new CMultiDocTemplate(
		IDR_BROWSER,
		RUNTIME_CLASS(CGnucleusDoc),
		RUNTIME_CLASS(CFrameBrowser),
		RUNTIME_CLASS(CViewBrowser));
	AddDocTemplate(m_pBrowserTemplate);
}

// Command line processing
void CMyCmdLineInfo::ParseParam(LPCSTR pszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag && stricmp(pszParam, "min") == 0)
		m_Minimize = true;
	else 
		CCommandLineInfo::ParseParam (pszParam, bFlag, bLast);
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	CString m_AboutDNA;
	CString m_LicenseDNA;


	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CButton	m_stcVersionFrame;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CStatic m_stcVersion;
	CStatic m_stcLicenseDNA;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_VERSION, m_stcVersionFrame);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_GNUCDNA, m_stcVersion);
	DDX_Control(pDX, IDC_STATIC_DNALICENSE, m_stcLicenseDNA);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// App command to run the dialog
void CGnucleusApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

	aboutDlg.m_AboutDNA   = "Gnucleus uses GnucDNA version " + m_pDoc->m_autCore->GetCoreVersion();
	aboutDlg.m_LicenseDNA = m_pDoc->m_autCore->GetCoreLicense();

	
	// Test download from web
	//int DownloadID = m_pDoc->m_autDownload->DownloadFile("Picture2.jpg", 282152, HASH_SHA1, "3IM67PAFGVWTQZEAGTWKAEM3SYBVG4OF");
	//m_pDoc->m_autDownload->AddSource(DownloadID, NETWORK_WEB, "http://www.gnucleus.com/GnucDNA/picture2.jpg");
	//m_pDoc->m_autDownload->Proxy(DownloadID, true, "66.123.218.124:80");

	aboutDlg.DoModal();
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CGnucleusDoc* pDoc = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;

	CString Title = "Gnucleus ";
	Title += pDoc->m_GnuVersion;

	m_stcVersionFrame.SetWindowText(Title);
	

	m_stcVersion.SetWindowText(m_AboutDNA);
	m_stcLicenseDNA.SetWindowText(m_LicenseDNA);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGnucleusApp::PreTranslateMessage(MSG* pMsg) 
{
	
	return CWinApp::PreTranslateMessage(pMsg);
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

