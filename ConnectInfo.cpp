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
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "ConnectInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CConnectInfo::CConnectInfo(CWnd* pParent /*=NULL*/, CGnucleusDoc* pDoc)
	: CDialog(CConnectInfo::IDD, pParent)
{
	m_pDoc = pDoc;
	hAsync = 0;

	//{{AFX_DATA_INIT(CConnectInfo)
	//}}AFX_DATA_INIT
}

CConnectInfo::~CConnectInfo()
{
	if(hAsync)
		::WSACancelAsyncRequest(hAsync);
}

void CConnectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectInfo)
	DDX_Control(pDX, IDC_EDIT_HOSTNAME, m_ebHostName);
	DDX_Control(pDX, IDC_EDIT_HOST, m_ebHost);
	DDX_Control(pDX, IDC_STATIC_TIME, m_stcTime);
	DDX_Control(pDX, IDC_EDIT_HANDSHAKE, m_ebHandshake);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectInfo, CDialog)
	//{{AFX_MSG_MAP(CConnectInfo)
	ON_MESSAGE(CONNECT_INFO, OnAsyncGetHostByAddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CConnectInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Handshake.Replace("\n\n", "\r\n\r\n");

	m_ebHost.SetWindowText(m_Host + ":" + DWrdtoStr(m_Port));

	CString ConnectTime = m_Time.Format("%I:%M%p %d%b");
	ConnectTime.MakeUpper();

	m_stcTime.SetWindowText(ConnectTime);
	m_ebHandshake.SetWindowText(m_Handshake);
	
	//Makslane: start asynchronous host name resolving
	IP HostIP = StrtoIP(m_Host);
	
	hAsync = ::WSAAsyncGetHostByAddr(m_hWnd,
		CONNECT_INFO,
		(char*) &HostIP.S_addr,
		sizeof(HostIP.S_addr),
		PF_INET,
		bufHost,
		MAXGETHOSTSTRUCT);

	if(hAsync)
		m_ebHostName.SetWindowText(_T("Resolving host name..."));
	else
		m_ebHostName.SetWindowText(m_Host);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CConnectInfo::OnAsyncGetHostByAddr(WPARAM wParam, LPARAM lParam)
{
	hAsync = 0;

	int nWSAError = WSAGETASYNCERROR(lParam);
	
	if(!nWSAError)
		m_ebHostName.SetWindowText(((LPHOSTENT)bufHost)->h_name);
	else
		m_ebHostName.SetWindowText("Could not resolve host");
	
	return NULL;
}
