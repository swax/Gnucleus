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
#include "AutCache.h"

#include "FrameStatistics.h"
#include "ViewStatistics.h"

#include "ViewConnect.h"
#include "ConnectInfo.h"
#include "ConnectAdvanced.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CConnectAdvanced, CPropertyPage)

CConnectAdvanced::CConnectAdvanced() : CPropertyPage(CConnectAdvanced::IDD)
{
	m_pDoc		 = NULL;
	m_autNetwork = NULL;

	m_SetActive = false;
}

CConnectAdvanced::CConnectAdvanced(CViewConnect* pView) : CPropertyPage(CConnectAdvanced::IDD)
{
	m_pDoc		 = pView->m_pDoc;
	m_autNetwork = m_pDoc->m_autNetwork;


	//{{AFX_DATA_INIT(CConnectAdvanced)
	//}}AFX_DATA_INIT
}

CConnectAdvanced::~CConnectAdvanced()
{
}

void CConnectAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectAdvanced)
	DDX_Control(pDX, IDC_LST_KNOWN, m_lstCached);
	DDX_Control(pDX, IDC_STATIC_PORT, m_stcPort);
	DDX_Control(pDX, IDC_STATIC_HOST, m_stcHost);
	DDX_Control(pDX, IDC_STATIC_EXIST, m_stcExist);
	DDX_Control(pDX, IDC_STATIC_CONNECT, m_stcConnect);
	DDX_Control(pDX, IDC_STATIC_ADD, m_stcAdd);
	DDX_Control(pDX, IDC_LST_CONNECTED, m_lstConnected);
	DDX_Control(pDX, IDC_EDIT_PORT, m_ebPort);
	DDX_Control(pDX, IDC_EDIT_HOST, m_ebHost);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectAdvanced, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectAdvanced)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_NOTIFY(NM_RCLICK, IDC_LST_KNOWN, OnRclickLstKnown)
	ON_NOTIFY(NM_RCLICK, IDC_LST_CONNECTED, OnRclickLstConnected)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_CONNECTED, OnDblclkLstConnected)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LST_CONNECTED, OnKeydownLstConnected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CConnectAdvanced::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	// Connected Nodes listbox
	CRect rect;
	m_lstConnected.GetWindowRect(&rect);
	
	int offSet = m_lstConnected.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;
	int nWidth = rect.Width();

	m_lstConnected.InsertColumn( 0, "Node",		  LVCFMT_LEFT,	nWidth * 5/16, 0);
	m_lstConnected.InsertColumn( 1, "Type",		  LVCFMT_LEFT,	nWidth * 5/16, 1);
	m_lstConnected.InsertColumn( 2, "Bandwidth",  LVCFMT_RIGHT, nWidth * 3/16, 2);
	m_lstConnected.InsertColumn( 3, "Efficiency", LVCFMT_RIGHT, nWidth * 3/16, 3);

	m_lstConnected.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	
	// Host Cache Listbox
	m_lstCached.GetWindowRect(&rect);
	offSet = m_lstCached.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;
	nWidth = rect.Width() - offSet;
	
	m_lstCached.InsertColumn(0, "Node",	 LVCFMT_LEFT, nWidth * 5/10, 0);
	m_lstCached.InsertColumn(1, "Status", LVCFMT_LEFT, nWidth * 5/10, 1);

	m_lstCached.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	DWORD CacheSize = m_pDoc->m_autCache->GetNodeCacheMaxSize() + m_pDoc->m_autCache->GetUltraNodeCacheMaxSize();
	
	if(CacheSize >= NODECACHE_SIZE)
		m_lstCached.InsertItem(0, "Cache Full");
	else if(CacheSize == 0)
		m_lstCached.InsertItem(0, "Cache Empty");
	else
		m_lstCached.InsertItem(0, "Cache at " + GetPercentage(NODECACHE_SIZE, CacheSize));
	m_lstCached.InsertItem(0, "");

	OnSockUpdate();


	m_DlgResizer.InitResizer(this);

	// List box on left
	m_DlgResizer.ListCtrlItem(IDC_LST_KNOWN);
	m_DlgResizer.SizeItem(IDC_LST_KNOWN, CDlgResizer::Down);

	m_DlgResizer.ListCtrlItem(IDC_LST_CONNECTED);
	m_DlgResizer.SizeItem(IDC_LST_CONNECTED, CDlgResizer::RightAndDown);

	// Static box on LHS
	m_DlgResizer.MoveItem(IDC_STATIC_ADD, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_STATIC_HOST, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_STATIC_PORT, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_EDIT_HOST, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_EDIT_PORT, CDlgResizer::Down);

	// Buttons
	m_DlgResizer.MoveItem(IDC_BUTTON_ADD, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_REMOVE, CDlgResizer::Down);
	m_DlgResizer.Done();


	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectAdvanced::OnTimer(UINT nIDEvent) 
{
	UpdateCacheView();
	UpdateConnectView();
		
	CPropertyPage::OnTimer(nIDEvent);
}

void CConnectAdvanced::OnSockUpdate()
{
	UpdateCacheView();
	UpdateConnectView();
}


void CConnectAdvanced::OnNodeCacheAdd()
{
	UpdateCacheView();
}

void CConnectAdvanced::UpdateConnectView()
{
	if(!m_lstConnected.IsWindowVisible() && !m_SetActive)
		return;

	m_SetActive = false;

	// Save selected nodes
	std::vector<int> ListSelected;
	POSITION lstPos = m_lstConnected.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstConnected.GetNextSelectedItem(lstPos);

		m_lstConnected.SetItem(nItem, 0, LVIF_STATE, NULL, 0, 0, LVIS_SELECTED, 0);
		if(nItem < m_lstConnected.GetItemCount() - 2)
			ListSelected.push_back(m_lstConnected.GetItemData(nItem));
	}

	CString HostPort, NodeType;
	int itemCount = m_lstConnected.GetItemCount();
	int Hosts	  = 0;
	int Children  = 0;
	int pos = 0;


	// Get current node IDs and put them into a vector
	_variant_t varNode = m_autNetwork->GetNodeIDs();
	SAFEARRAY* psa = varNode.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> NodeIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 NodeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);


	// Get child IDs and put them into a vector
	_variant_t varChild = m_autNetwork->GetChildNodeIDs();
	psa = varChild.parray;

	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> ChildIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 ChildIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);


	// Mode 0 - Normal nodes
	// Mode 1 - Child nodes
	for(int Mode = 0; Mode < 2; Mode++)
	{
		std::vector<int>* NodeList = NULL;

		if(Mode == 0)
			NodeList = &NodeIDs;
		else
			NodeList = &ChildIDs;


		for(i = 0; i < NodeList->size(); i++)
		{
			int NodeID = NodeList->at(i);


			if(m_autNetwork->GetNodeState(NodeID) != SOCK_CONNECTED)
				continue;
			

			// Count Hosts
			Hosts++;
			
			if(Mode == 1)
				Children++;
			
			
			IP NodeIP;
			NodeIP.S_addr = m_autNetwork->GetNodeIP(NodeID);

			HostPort = IPtoStr(NodeIP) + ":" + DWrdtoStr(m_autNetwork->GetNodePort(NodeID));

			if(itemCount <= pos)
				m_lstConnected.InsertItem(pos, HostPort);
			else
				m_lstConnected.SetItemText(pos, 0, HostPort);

			// Type 
			if(Mode == 1)
				NodeType = "Local Child";

			else if(m_autNetwork->GetNodeMode(NodeID) == CLIENT_ULTRAPEER)
				NodeType = "Ultrapeer";

			else if(m_autNetwork->GetNodeMode(NodeID) == CLIENT_LEAF)
				NodeType = "Child Node";

			else
				NodeType = "Normal Node";

			m_lstConnected.SetItemText(pos, 1, NodeType);

			// Bandwidth
			m_lstConnected.SetItemText(pos, 2, GetBandwidth(NodeID) + " KB/s");
			
			//Efficiency
			m_lstConnected.SetItemText(pos, 3, GetEfficiency(NodeID));

			m_lstConnected.SetItemData(pos, (DWORD) NodeID);
			

			// Re-select previously selected item
			for (int sel = 0; sel < ListSelected.size(); sel++)
				if(ListSelected[sel] == NodeID)
					m_lstConnected.SetItem(pos, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
		
			pos++;
		}

		if(Mode == 1 && Children == 0)
			break;

		// Add space between
		if(itemCount <= pos)
			m_lstConnected.InsertItem(pos, "");
		else
			m_lstConnected.SetItemText(pos, 0, "");
		m_lstConnected.SetItemText(pos, 1, "");
		m_lstConnected.SetItemText(pos, 2, "");
		m_lstConnected.SetItemText(pos, 3, "");
		m_lstConnected.SetItemData(pos, NULL);
		pos++;
		

		if(m_autNetwork->ClientMode() == CLIENT_LEAF)
			break;

	}

	// Add host count
	CString HostCount = DWrdtoStr(Hosts);;

	if(Hosts)
		HostCount += (Hosts == 1) ? " Connection" : " Connections";
	else
		HostCount = "";


	if(itemCount <= pos)
		m_lstConnected.InsertItem(pos, HostCount);
	else
		m_lstConnected.SetItemText(pos, 0, HostCount);

	m_lstConnected.SetItemText(pos, 1, "");
	m_lstConnected.SetItemText(pos, 2, "");
	m_lstConnected.SetItemText(pos, 3, "");
	m_lstConnected.SetItemData(pos, NULL);
	pos++;
	

	// Delete extra lines
	for (int del = pos; del < itemCount; del++)
		m_lstConnected.DeleteItem(pos);
}

void CConnectAdvanced::UpdateCacheView()
{
	if(!m_lstConnected.IsWindowVisible() && !m_SetActive)
		return;

	m_SetActive = false;


	// Save selected nodes
	std::vector<int> ListSelected;
	POSITION lstPos = m_lstCached.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstCached.GetNextSelectedItem(lstPos);

		m_lstCached.SetItem(nItem, 0, LVIF_STATE, NULL, 0, 0, LVIS_SELECTED, 0);
		if(nItem < m_lstCached.GetItemCount() - 2)
			ListSelected.push_back( m_lstCached.GetItemData(nItem));
	}

	CString HostPort;
	int itemCount = m_lstCached.GetItemCount();


	// Get current node IDs and put them into a vector
	_variant_t varNode = m_autNetwork->GetNodeIDs();
	SAFEARRAY* psa = varNode.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> NodeIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 NodeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);

	// Get child IDs and put them into a vector
	_variant_t varChild = m_autNetwork->GetChildNodeIDs();
	psa = varChild.parray;

	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 NodeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);


	// Add nodes to list box
	for(int i = 0, pos = 0; i < NodeIDs.size(); i++)
	{
		if(m_autNetwork->GetNodeState(NodeIDs[i]) == SOCK_CONNECTED)
			continue;

		IP NodeIP;
		NodeIP.S_addr = m_autNetwork->GetNodeIP(NodeIDs[i]);

		HostPort = IPtoStr(NodeIP) + ":" + DWrdtoStr(m_autNetwork->GetNodePort(NodeIDs[i]));

		if(itemCount <= pos)
			m_lstCached.InsertItem(pos, HostPort);
		else
			m_lstCached.SetItemText(pos, 0, HostPort);

		m_lstCached.SetItemText(pos, 1, "");
		m_lstCached.SetItemData(pos, NodeIDs[i]);

		// Re-select previously selected item
		for (int sel = 0; sel < ListSelected.size(); sel++)
			if(ListSelected[sel] == NodeIDs[i])
				m_lstCached.SetItem(pos, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
	
		pos++;
	}

	// Add space between
	
	if(itemCount <= pos)
		m_lstCached.InsertItem(pos, "");
	else
		m_lstCached.SetItemText(pos, 0, "");

	m_lstCached.SetItemText(pos, 1, "");
	m_lstCached.SetItemData(pos, NULL);
	pos++;

	// Add cache size
	int CacheSize = m_pDoc->m_autCache->GetNodeCacheMaxSize() + m_pDoc->m_autCache->GetUltraNodeCacheMaxSize();

	CString CacheText;

	if(CacheSize >= NODECACHE_SIZE)
		CacheText = "Cache Full";
	else if(CacheSize == 0)
		CacheText = "Cache Empty";
	else
		CacheText = "Cache at " + GetPercentage(NODECACHE_SIZE * 2, CacheSize);


	if(itemCount <= pos)
		m_lstCached.InsertItem(pos, CacheText);
	else
		m_lstCached.SetItemText(pos, 0, CacheText);
	
	m_lstCached.SetItemText(pos, 1, "");
	m_lstCached.SetItemData(pos, NULL);
	pos++;

	// Delete extra lines
	for (int del = pos; del < itemCount; del++)
		m_lstCached.DeleteItem(pos);
}

CString CConnectAdvanced::GetBandwidth(int NodeID) 
{
	return InsertDecimal((double) (m_autNetwork->GetNodeBytesDown(NodeID) + m_autNetwork->GetNodeBytesUp(NodeID)) / (double) 1024);
}

CString CConnectAdvanced::GetEfficiency(int NodeID) 
{
	long Good, Total;
	m_autNetwork->GetNodePacketsTotal(NodeID, &Good, &Total);

	return GetPercentage(Total, Good);
}

void CConnectAdvanced::OnButtonAdd() 
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

	m_autNetwork->ConnectNode(Host, atoi(Port));

	m_pDoc->m_autCore->Connect();
}

void CConnectAdvanced::OnButtonRemove() 
{
	int      nItem;

	POSITION pos = m_lstConnected.GetFirstSelectedItemPosition();
	while(pos)
	{
		pos   = m_lstConnected.GetFirstSelectedItemPosition();
		nItem = m_lstConnected.GetNextSelectedItem(pos);

		m_autNetwork->RemoveNode( m_lstConnected.GetItemData(nItem) );
	}

	pos = m_lstCached.GetFirstSelectedItemPosition();
	while(pos)
	{
		pos = m_lstCached.GetFirstSelectedItemPosition();
		nItem = m_lstCached.GetNextSelectedItem(pos);

		m_autNetwork->RemoveNode( m_lstCached.GetItemData(nItem) );
	}

	OnSockUpdate();
}

void CConnectAdvanced::OnRclickLstKnown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CConnectAdvanced::OnRclickLstConnected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstConnected.GetSelectionMark();
	if (nItem < 0 || nItem > m_lstConnected.GetItemCount() - 3)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_CONNECT_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstConnected.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_LSTCONNECT_INFO:
		nItem = m_lstConnected.GetNextSelectedItem(pos);
		if (nItem >= 0 && nItem < m_lstConnected.GetItemCount() - 2)
			ExtendedInfo( m_lstConnected.GetItemData(nItem));

		break;
	case ID_LSTCONNECT_LOG:
		if(pos)
		{
			CGnucleusApp* App = (CGnucleusApp*) AfxGetApp();
	
			CFrameStatistics* m_pWindowStatistics = (CFrameStatistics*) App->m_pStatisticsTemplate->CreateNewFrame(m_pDoc, NULL);
			App->m_pStatisticsTemplate->InitialUpdateFrame(m_pWindowStatistics,m_pDoc);	

			while(pos)
			{
				int nItem = m_lstConnected.GetNextSelectedItem(pos);
			
				CViewStatistics* ViewStat = (CViewStatistics*) m_pWindowStatistics->GetActiveView();
				
				if(nItem >= 0)
					ViewStat->SelectNode( m_lstConnected.GetItemData(nItem) );
			}
		}
		break;

	/*case ID_LSTCONNECT_REFRESH:
		while(pos)
		{
			int nItem = m_lstConnected.GetNextSelectedItem(pos);

			if(nItem >= 0)
				((CGnuNode*) m_lstConnected.GetItemData(nItem))->Refresh();
		}
		break;*/

	case ID_LSTCONNECT_DISCONNECT:
		OnButtonRemove();
		break;
	}

	*pResult = 0;
}

void CConnectAdvanced::OnDblclkLstConnected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstConnected.GetSelectionMark();
	if (nItem < 0 || nItem > m_lstConnected.GetItemCount() - 3)
		return;
	
	POSITION pos = m_lstConnected.GetFirstSelectedItemPosition();
	

	if(pos)
	{
		CGnucleusApp* App = (CGnucleusApp*) AfxGetApp();

		CFrameStatistics* m_pWindowStatistics = (CFrameStatistics*) App->m_pStatisticsTemplate->CreateNewFrame(m_pDoc, NULL);
		App->m_pStatisticsTemplate->InitialUpdateFrame(m_pWindowStatistics, m_pDoc);	

		while(pos)
		{
			int nItem = m_lstConnected.GetNextSelectedItem(pos);
		
			CViewStatistics* ViewStat = (CViewStatistics*) m_pWindowStatistics->GetActiveView();
			ViewStat->SelectNode( m_lstConnected.GetItemData(nItem) );
		}
	}

	*pResult = 0;
}

void CConnectAdvanced::ExtendedInfo(int NodeID)
{
	CConnectInfo InfoDlg(NULL, m_pDoc);

	IP NodeIP;
	NodeIP.S_addr = m_autNetwork->GetNodeIP(NodeID);
	
	InfoDlg.m_Host		= IPtoStr(NodeIP);
	InfoDlg.m_Port	    = m_autNetwork->GetNodePort(NodeID);
	InfoDlg.m_Time		= m_autNetwork->GetNodeConnectTime(NodeID);
	InfoDlg.m_Handshake = m_autNetwork->GetNodeHandshake(NodeID);

	InfoDlg.DoModal();
}

void CConnectAdvanced::OnKeydownLstConnected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	// E key
	if(pLVKeyDow->wVKey == 69)
	{
		POSITION pos = m_lstConnected.GetFirstSelectedItemPosition();

		int nItem = m_lstConnected.GetNextSelectedItem(pos);

		if (nItem >= 0 && nItem < m_lstConnected.GetItemCount() - 2)
			ExtendedInfo( m_lstConnected.GetItemData(nItem));
	}

	if(pLVKeyDow->wVKey == VK_DELETE)
	{
		OnButtonRemove();
	}

	*pResult = 0;
}

BOOL CConnectAdvanced::OnSetActive() 
{
	m_SetActive = true;

	OnSockUpdate();

	return CPropertyPage::OnSetActive();
}
