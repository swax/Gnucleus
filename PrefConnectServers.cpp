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
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "AutPrefs.h"
#include "AutCache.h"

#include "PrefConnectServers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefConnectServers, CPropertyPage)

CPrefConnectServers::CPrefConnectServers() : CPropertyPage(CPrefConnectServers::IDD)
{
	//{{AFX_DATA_INIT(CPrefConnectServers)
	//}}AFX_DATA_INIT

	m_pDoc		= NULL;
	m_autPrefs  = NULL;
}

CPrefConnectServers::~CPrefConnectServers()
{
}

void CPrefConnectServers::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefConnectServers)
	DDX_Control(pDX, IDC_LIST_SERVERS, m_lstServers);
	DDX_Control(pDX, IDC_EDIT_PORT, m_ebPort);
	DDX_Control(pDX, IDC_EDIT_HOST, m_ebHost);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefConnectServers, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefConnectServers)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVERS, OnRclickListServers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefConnectServers::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;

	// Setup listbox
	int offSet = m_lstServers.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstServers.GetWindowRect(&rect);

	m_lstServers.InsertColumn( 0, "Host", LVCFMT_LEFT,
		(rect.Width() - offSet) * 3/4, 0);
	m_lstServers.InsertColumn( 1, "Port", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/4, 1);

	m_lstServers.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Copy vector to host servers array
	VARIANT var = m_autPrefs->GetHostServers();
	SAFEARRAY* psa = var.parray;

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> HostServers;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 HostServers.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	// Load list box
	for(int i = 0; i < HostServers.size(); i++)
	{
		int pos    = m_lstServers.GetItemCount();
		int colpos = HostServers[i].Find(":");

		if(colpos > 0)
		{
			m_lstServers.InsertItem(pos, HostServers[i].Left(colpos));
			m_lstServers.SetItemText(pos, 1, HostServers[i].Mid(colpos + 1));
		}
	}
	
	// Set default port
	m_ebPort.SetWindowText("6346");	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefConnectServers::OnButtonAdd() 
{
	int pos = -1;
	CString Host, Port;

	m_ebHost.GetWindowText(Host);
	m_ebPort.GetWindowText(Port);

	if(Host == "" || Port == "")
		return;

	// Accept a Host:Port format.
	if( (pos = Host.Find(":", 0)) != -1)
	{
		Port = Host.Mid(pos + 1, Host.GetLength() - pos);
		Host = Host.Mid(0, pos);

		m_ebHost.SetWindowText(Host);
		m_ebPort.SetWindowText(Port);
	}

	pos = m_lstServers.GetItemCount();
	m_lstServers.InsertItem(pos, Host);
	m_lstServers.SetItemText(pos, 1, Port);

	SetModified();
}

void CPrefConnectServers::OnButtonRemove() 
{
	int      nItem;
	POSITION pos = m_lstServers.GetFirstSelectedItemPosition();

	while(pos)
	{
		pos   = m_lstServers.GetFirstSelectedItemPosition();
		nItem = m_lstServers.GetNextSelectedItem(pos);

		m_lstServers.DeleteItem(nItem);
	}

	SetModified();
}

BOOL CPrefConnectServers::OnApply() 
{
	std::vector<CString> HostServers;
	for(int i = 0; i < m_lstServers.GetItemCount(); i++)
	{
		CString HostPort = m_lstServers.GetItemText(i, 0);
		HostPort  += ":" + m_lstServers.GetItemText(i, 1);

		m_pDoc->m_autCache->AddNode(HostPort, false);

		HostServers.push_back(HostPort);
	}

	VARIANT vaNodes;
	VariantInit(&vaNodes);

	vaNodes.vt = VT_ARRAY | VT_BSTR;

	SAFEARRAYBOUND bounds = {HostServers.size(), 0};
	SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	for(int i = 0; i < HostServers.size(); i++)
		strArray[i] = HostServers[i].AllocSysString();

	SafeArrayUnaccessData(psa);
	vaNodes.parray = psa;

	m_autPrefs->SetHostServers(&vaNodes);
	
	VariantClear(&vaNodes);

	return CPropertyPage::OnApply();
}

void CPrefConnectServers::OnRclickListServers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstServers.GetSelectionMark();
	if (0 > nItem || nItem > m_lstServers.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_SERVERCONN_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstServers.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_SERVERCONN_REMOVE:
		while(pos)
		{
			pos   = m_lstServers.GetFirstSelectedItemPosition();
			nItem = m_lstServers.GetNextSelectedItem(pos);

			m_lstServers.DeleteItem(nItem);
		}

		SetModified();
		break;
	}

	*pResult = 0;
}
