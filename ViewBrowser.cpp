/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2003 John Marshall

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
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "ViewBrowser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewBrowser, CHtmlView)

CViewBrowser::CViewBrowser()
{
	//{{AFX_DATA_INIT(CViewBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CViewBrowser::~CViewBrowser()
{
}

void CViewBrowser::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewBrowser)
	DDX_Control(pDX, IDC_EDIT_URL, m_ebUrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewBrowser, CHtmlView)
	//{{AFX_MSG_MAP(CViewBrowser)
	ON_WM_CREATE()
	ON_COMMAND(ID_SURF_BACK, OnSurfBack)
	ON_COMMAND(ID_SURF_FORWARD, OnSurfForward)
	ON_COMMAND(ID_SURF_HOME, OnSurfHome)
	ON_COMMAND(ID_SURF_REFRESH, OnSurfRefresh)
	ON_COMMAND(ID_SURF_STOP, OnSurfStop)
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CViewBrowser::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CViewBrowser::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


int CViewBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this->GetParentFrame()) ||
		!m_wndToolBar.LoadToolBar(IDR_SURF))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_AddressBar.Create(this->GetParentFrame(), IDD_BROWSER_URL,
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create AddressBar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this->GetParentFrame()) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_AddressBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	return 0;
}


void CViewBrowser::OnInitialUpdate() 
{
	m_pDoc = (CGnucleusDoc*) GetDocument();

	Navigate2(m_pDoc->m_CurrentSite, NULL, NULL);	

	m_AddressBar.GetDlgItem(IDC_EDIT_URL)->SetWindowText(m_pDoc->m_CurrentSite);
}


void CViewBrowser::OnSurfBack() 
{
	GoBack();
}

void CViewBrowser::OnSurfForward() 
{
	GoForward();
}

void CViewBrowser::OnSurfHome() 
{
	Navigate("http://www.gnucleus.net");

	m_AddressBar.GetDlgItem(IDC_EDIT_URL)->SetWindowText("http://www.gnucleus.net");
}

void CViewBrowser::OnSurfRefresh() 
{
	Refresh();
}

void CViewBrowser::OnSurfStop() 
{
	Stop();
}

void CViewBrowser::OnButtonGo() 
{
	CString url;
	m_AddressBar.GetDlgItem(IDC_EDIT_URL)->GetWindowText(url);

	Navigate(url);
}


void CViewBrowser::OnSize(UINT nType, int cx, int cy) 
{
	CHtmlView::OnSize(nType, cx, cy);
	
	CRect winRect;
	m_AddressBar.GetWindowRect(&winRect);

	CRect ebRect;
	m_AddressBar.GetDlgItem(IDC_EDIT_URL)->GetWindowRect(&ebRect);

	CRect btnRect;
	m_AddressBar.GetDlgItem(IDC_BUTTON_GO)->GetWindowRect(&btnRect);

	m_AddressBar.GetDlgItem(IDC_BUTTON_GO)->MoveWindow(winRect.Width() - btnRect.Width() - 16, btnRect.top - winRect.top ,btnRect.Width(), btnRect.Height());
	m_AddressBar.GetDlgItem(IDC_EDIT_URL)->MoveWindow(ebRect.left - winRect.left, ebRect.top - winRect.top, (winRect.right - ebRect.left) - btnRect.Width() - 24, ebRect.Height());
}



void CViewBrowser::CommandSent()
{
	OnButtonGo();
}

BOOL CViewBrowser::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CHtmlView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}
