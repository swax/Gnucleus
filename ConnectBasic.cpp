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


#include "stdafx.h"
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "AutCore.h"
#include "AutPrefs.h"
#include "AutNetwork.h"
#include "AutShare.h"


#include "ViewConnect.h"

#include "FrameMain.h"

#include "ConnectBasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CConnectBasic, CPropertyPage)

CConnectBasic::CConnectBasic() : CPropertyPage(CConnectBasic::IDD)
{
}

CConnectBasic::CConnectBasic(CViewConnect* pView) : CPropertyPage(CConnectBasic::IDD)
{
	//{{AFX_DATA_INIT(CConnectBasic)
	//}}AFX_DATA_INIT

	m_pDoc       = pView->m_pDoc;
	m_autCore    = m_pDoc->m_autCore;
	m_autNetwork = m_pDoc->m_autNetwork;
	m_autPrefs	 = m_pDoc->m_autPrefs;
	m_autShare   = m_pDoc->m_autShare;

	m_Status   = SOCK_CLOSED;
	m_BarPos   = 0;
	m_HashDots = 0;

	m_Ready = false;
}

CConnectBasic::~CConnectBasic()
{
}

void CConnectBasic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectBasic)
	DDX_Control(pDX, IDC_STATIC_TRANSFERS, m_stcTransfers);
	DDX_Control(pDX, IDC_STATIC_SHARED, m_stcShare);
	DDX_Control(pDX, IDC_STATIC_CHAT, m_stcChat);
	DDX_Control(pDX, IDC_STATIC_SEARCH, m_stcSearch);
	DDX_Control(pDX, IDC_PROGRESS_CONNECT, m_progConnect);
	DDX_Control(pDX, IDC_STATIC_CONNECT, m_stcConnect);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON_TRANSFERS, m_btnTransfers);
	DDX_Control(pDX, IDC_BUTTON_SHARED, m_btnShared);
	DDX_Control(pDX, IDC_BUTTON_CHAT, m_btnChat);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_stcLogo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectBasic, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectBasic)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFERS, OnButtonTransfers)
	ON_BN_CLICKED(IDC_BUTTON_SHARED, OnButtonShared)
	ON_BN_CLICKED(IDC_BUTTON_CHAT, OnButtonChat)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STATIC_TRANSFERS, OnStaticTransfers)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectBasic message handlers

BOOL CConnectBasic::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_btnShared.SetBitmap( ::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SHARE)) );
	m_btnChat.SetBitmap( ::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CHAT)) );
	m_btnTransfers.SetBitmap( ::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TRANSFERS)) );
	m_btnSearch.SetBitmap( ::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEARCH)) );
	
	m_progConnect.SetRange(0, 20);
	
	SetTimer(145, 1000, NULL);

	m_Ready = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectBasic::OnButtonSearch() 
{
	((CFrameMain*) AfxGetApp()->m_pMainWnd)->SearchFromConnect();
}

void CConnectBasic::OnButtonTransfers() 
{
	::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_TRANSFERS, NULL);
}

void CConnectBasic::OnButtonShared() 
{
	::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_SHARE, NULL);
}

void CConnectBasic::OnButtonChat() 
{
	::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_CHAT, NULL);
}

void CConnectBasic::OnTimer(UINT nIDEvent) 
{
	if(m_autCore->IsConnecting())
	{
		int NumConnects = m_autNetwork->GetNormalConnectedCount();

		if(NumConnects)
		{
			int MinConnects = 0;

			if(m_pDoc->m_autPrefs->GetLanMode())
				m_stcConnect.SetWindowText("Connected to the " + m_pDoc->m_autPrefs->GetLanName() + " Network");
			else
				m_stcConnect.SetWindowText("Connected to the Gnutella Network");

			m_Status = SOCK_CONNECTED;

			m_btnSearch.EnableWindow();
			m_stcSearch.EnableWindow();

			if(m_pDoc->m_autPrefs->GetMinConnects() != -1)
				MinConnects = m_pDoc->m_autPrefs->GetMinConnects();
			
			if(m_autNetwork->ClientMode() == CLIENT_LEAF)
				MinConnects = 1;

			if(!MinConnects)
				MinConnects = 10;

			int NewPos = NumConnects * 100 / MinConnects / 5;

			m_progConnect.SetPos(NewPos);
		}
		else
		{
			m_stcConnect.SetWindowText("Connecting...");
			m_Status = SOCK_CONNECTING;

			m_btnSearch.EnableWindow(false);
			m_stcSearch.EnableWindow(false);
		}
	}
	else
	{
		m_stcConnect.SetWindowText("Disconnected");
		m_Status = SOCK_CONNECTING;
		m_progConnect.SetPos(0);

		m_btnSearch.EnableWindow(false);
		m_stcSearch.EnableWindow(false);
	}


	// Display if files are being hashed
	CString ShareText = "Shared Files";

	if(!m_autShare->IsEverythingHashed() && !m_autShare->IsHashingStopped())
	{
		ShareText += ", Hashing";

		for(int i = 0; i < m_HashDots; i++)
			ShareText += ".";

		m_HashDots++;

		if(m_HashDots > 3)
			m_HashDots = 0;
	}
	
	m_stcShare.SetWindowText(ShareText);
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CConnectBasic::OnSockUpdate()
{
	if(m_Status == SOCK_CONNECTING)
	{
		m_BarPos++;

		if(m_BarPos > 20)
			m_BarPos = 1;

		m_progConnect.SetPos(m_BarPos);
	}
}

void CConnectBasic::OnStaticTransfers() 
{
	::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_COMMAND, ID_VIEW_TRANSFERS, NULL);	
}

void CConnectBasic::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	if(!m_Ready)
		return;

	/*int Height, Width, Top, Left;

	CRect rLogo, rStatus, rProgress;
	m_stcLogo.GetWindowRect(rLogo);
	m_stcConnect.GetWindowRect(rStatus);

	Width = rLogo.Width();
	Left  = (cx / 2) - (rLogo.Width() / 2);

	rLogo.left = Left;
	rLogo.right = Left + Width;

	rStatus.left = Left;
	rStatus.right = Left + Width;


	m_stcLogo.MoveWindow(rLogo);
	m_stcConnect.MoveWindow(rStatus);*/
}

