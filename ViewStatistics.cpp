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

#include "AutNetwork.h"

#include "ConnectInfo.h"
#include "StatisticsMain.h"
#include "StatisticsDistribution.h"
#include "StatisticsPackets.h"
#include "StatisticsBandwidth.h"
#include "StatisticsLog.h"
#include "StatisticsMemory.h"
#include "StatisticsError.h"

#include "ViewStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewStatistics, CFormView)

CViewStatistics::CViewStatistics()
	: CFormView(CViewStatistics::IDD)
{
	//{{AFX_DATA_INIT(CViewStatistics)
	//}}AFX_DATA_INIT

	m_pDoc  = NULL;
	m_autNetwork = NULL;

	m_Paused = false;
}

CViewStatistics::~CViewStatistics()
{
	delete m_TabMain;
	delete m_TabDistribution;
	delete m_TabPackets;
	delete m_TabBandwidth;
	delete m_TabLog;
	delete m_TabMemory;
	delete m_TabError;
}

void CViewStatistics::OnDestroy() 
{
	CFormView::OnDestroy();
	
	std::vector<HWND>::iterator itView;
	for(itView = m_pDoc->m_pViewStatistics.begin(); itView != m_pDoc->m_pViewStatistics.end(); itView++)
		if(*itView == GetSafeHwnd())
			break;

	m_pDoc->m_pViewStatistics.erase(itView);
}

void CViewStatistics::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewStatistics)
	DDX_Control(pDX, IDC_BUTTON_PAUSEGO, m_btnPauseGo);
	DDX_Control(pDX, IDC_PSHEET_AREA, m_tabSheet);
	DDX_Control(pDX, IDC_LIST_NODES, m_lstNodes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewStatistics, CFormView)
	//{{AFX_MSG_MAP(CViewStatistics)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PAUSEGO, OnButtonPauseGo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NODES, OnItemchangedListNodes)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_NODES, OnRclickListNodes)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_NODES, OnDblclkListNodes)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStatistics diagnostics

#ifdef _DEBUG
void CViewStatistics::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewStatistics::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CViewStatistics::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_pDoc       = (CGnucleusDoc*) GetDocument();
	m_autNetwork = m_pDoc->m_autNetwork;

	// Property Sheets
	m_TabMain		  = new CStatisticsMain(this);
	m_TabDistribution = new CStatisticsDistribution(this);
	m_TabPackets	  = new CStatisticsPackets(this);
	m_TabBandwidth	  = new CStatisticsBandwidth(this);
	m_TabLog		  = new CStatisticsLog;
	m_TabMemory		  = new CStatisticsMemory(this);
	m_TabError		  = new CStatisticsError(this);

	m_pSheet.AddPage(m_TabMain);
	//m_pSheet.AddPage(m_TabDistribution);
	m_pSheet.AddPage(m_TabPackets);
	m_pSheet.AddPage(m_TabBandwidth);
	m_pSheet.AddPage(m_TabLog);
	//m_pSheet.AddPage(m_TabMemory);
	//m_pSheet.AddPage(m_TabError);

	CreatePropSheet(this, &m_pSheet);
	SizePropSheet(this, &m_pSheet, IDC_PSHEET_AREA);

	for(int i = 0; i < m_pSheet.GetPageCount(); ++i)
		m_pSheet.SetActivePage(i);
	m_pSheet.SetActivePage(0);

	// Set Title
	//m_TimeStarted = CTime::GetCurrentTime();
	//GetParentFrame()->SetWindowText("Statistics - " + m_TimeStarted.Format("%I:%M %p"));

	// Connected Nodes listbox
	int offSet = m_lstNodes.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstNodes.GetWindowRect(&rect);

	m_lstNodes.InsertColumn( 0, "Node", LVCFMT_LEFT,  rect.Width() - offSet, 0);
	m_lstNodes.InsertColumn( 1, "Type", LVCFMT_LEFT, (rect.Width() - offSet) * 0.5, 1);

	m_lstNodes.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	
	// Size all the controls in proportion to the size
	RECT rWin;
	GetClientRect(&rWin);
	OnSize(SIZE_RESTORED, rWin.right, rWin.bottom);

	m_pDoc->m_pViewStatistics.push_back( GetSafeHwnd() );

	OnSockUpdate();
	m_TabDistribution->UpdateDistribution();
	m_TabPackets->UpdateData();
	m_TabBandwidth->UpdateBandwidth();
}

void CViewStatistics::OnSockUpdate()
{
	// Save selected nodes
	std::vector<int> ListSelected;
	POSITION lstPos = m_lstNodes.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstNodes.GetNextSelectedItem(lstPos);

		m_lstNodes.SetItem(nItem, 0, LVIF_STATE, NULL, 0, 0, LVIS_SELECTED, 0);
		if(nItem < m_lstNodes.GetItemCount())
			ListSelected.push_back(m_lstNodes.GetItemData(nItem));
	}

	CString HostPort, NodeType;
	int itemCount = m_lstNodes.GetItemCount();
	int Hosts	  = 0;
	int Children  = 0;
	int pos = 0;


	// Get current node IDs and put them into a vector
	_variant_t var = m_autNetwork->GetNodeIDs();
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> NodeIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 NodeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);


	// Get child IDs and put them into a vector
	var.Clear();
	var = m_autNetwork->GetChildNodeIDs();
	psa = var.parray;

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
				m_lstNodes.InsertItem(pos, HostPort);
			else
				m_lstNodes.SetItemText(pos, 0, HostPort);

			// Type 
			if(Mode == 1)
				NodeType = "Local Child";

			else if(m_autNetwork->GetNodeMode(NodeID) == CLIENT_ULTRAPEER)
				NodeType = "Ultrapeer";

			else if(m_autNetwork->GetNodeMode(NodeID) == CLIENT_LEAF)
				NodeType = "Child Node";

			else
				NodeType = "Normal Node";

			m_lstNodes.SetItemText(pos, 1, NodeType);

			m_lstNodes.SetItemData(pos, (DWORD) NodeID);
			

			// Re-select previously selected item
			for (int sel = 0; sel < ListSelected.size(); sel++)
				if(ListSelected[sel] == NodeID)
					m_lstNodes.SetItem(pos, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
		
			pos++;
		}

		if(Mode == 1 && Children == 0)
			break;

		// Add space between
		if(itemCount <= pos)
			m_lstNodes.InsertItem(pos, "");
		else
			m_lstNodes.SetItemText(pos, 0, "");
		m_lstNodes.SetItemText(pos, 1, "");
		m_lstNodes.SetItemData(pos, NULL);
		pos++;
		

		if(m_autNetwork->ClientMode() == CLIENT_LEAF)
			break;
	}

	// Delete extra lines
	for (int del = pos; del < itemCount; del++)
		m_lstNodes.DeleteItem(pos);
}

void CViewStatistics::OnPacketIncoming(int NodeID, packet_Header* packet, int size, int ErrorCode, bool Local)
{
	if(!m_Paused && SockSelected(NodeID))
	{
		// Update views
		if(Local)
		{
			m_TabDistribution->PacketIncoming(packet);
			m_TabLog->PacketIncoming(packet);
		}

		else if(ErrorCode == 0)
		{
			m_TabLog->PacketGood(packet);
		}

		else
			m_TabLog->PacketBad(packet, ErrorCode);


		m_TabPackets->UpdateData();
	}
}

void CViewStatistics::OnPacketOutgoing(int NodeID, packet_Header* packet, int size, bool Local)
{
	if(!m_Paused && SockSelected(NodeID))
	{
		if(Local)
			m_TabLog->PacketOutgoingLocal(packet);
		else
			m_TabLog->PacketOutgoing(packet);
	}
}

bool CViewStatistics::SockSelected(int NodeID)
{
	POSITION pos = m_lstNodes.GetFirstSelectedItemPosition();
	
	if(pos != NULL)
	   while (pos)
	   {
		  int nItem = m_lstNodes.GetNextSelectedItem(pos);
		  
		  if(m_lstNodes.GetItemData(nItem) == NodeID)
			  return true;
	   }

	return false;
}

void CViewStatistics::OnButtonPauseGo() 
{
	CString Status;
	m_btnPauseGo.GetWindowText(Status);

	if(Status == "Pause")
	{
		m_Paused = true;
		Status = "Go";
		m_btnPauseGo.SetWindowText(Status);
		
		GetParentFrame()->SetWindowText("Statistics - Paused");

	}
	else
	{
		m_Paused = false;
		Status = "Pause";
		m_btnPauseGo.SetWindowText(Status);

		m_TimeStarted = CTime::GetCurrentTime();
		GetParentFrame()->SetWindowText("Statistics - " + m_TimeStarted.Format("%I:%M %p"));

		m_TabDistribution->UpdateDistribution();
		m_TabPackets->UpdateData();
		m_TabBandwidth->UpdateBandwidth();
	}
}

void CViewStatistics::OnSize(UINT nType, int cx, int cy) 
// I HATE THIS CODE
{
	if( cx < 300 || cy < 200)
		return;			

	CFormView::OnSize(nType, cx, cy);
	
	if(m_lstNodes.m_hWnd != NULL)
	{
		int top_tabSheet, left_tabSheet,
			top_btnPause, left_btnPause, height_btnPause, width_btnPause,
			top_lstNodes, left_lstNodes, height_lstNodes, width_lstNodes;

		RECT rWnd, rNodes, rPause, rTabs;

		GetWindowRect(&rWnd);
		m_lstNodes.GetWindowRect(&rNodes);
		m_btnPauseGo.GetWindowRect(&rPause);
		m_tabSheet.GetWindowRect(&rTabs);

		top_btnPause    = rPause.top    - rWnd.top;
		left_btnPause   = rPause.left   - rWnd.left; 
		width_btnPause  = rPause.right  - rPause.left;
		height_btnPause = rPause.bottom - rPause.top;

		top_lstNodes    = rNodes.top    - rWnd.top;
		left_lstNodes   = rNodes.left   - rWnd.left; 
		width_lstNodes  = rNodes.right  - rNodes.left;
		height_lstNodes = rNodes.bottom - rNodes.top;

		top_tabSheet  = rTabs.top  - rWnd.top;
		left_tabSheet = rTabs.left - rWnd.left;
		
		m_btnPauseGo.MoveWindow(left_btnPause, cy - 12 - height_btnPause, width_btnPause, height_btnPause);
		m_lstNodes.MoveWindow(left_lstNodes, top_lstNodes, width_lstNodes, (cy - 12 - height_btnPause) - 12 - top_lstNodes);

		m_tabSheet.MoveWindow(left_tabSheet, top_tabSheet, cx - left_tabSheet - 5, cy - top_tabSheet - 5);
		SizePropSheet(this, &m_pSheet, IDC_PSHEET_AREA);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Property Sheet 

BOOL CViewStatistics::CreatePropSheet(CWnd* pParent, CPropertySheet* pPSheet)
{
	ASSERT_VALID( pParent );
	ASSERT_VALID( pPSheet );

	if( !pPSheet->Create( pParent, WS_CHILD | WS_VISIBLE, 0))
		return false;

	pPSheet->ModifyStyle(0, WS_TABSTOP);
	pPSheet->ModifyStyle(0, WS_EX_CONTROLPARENT);

	pParent->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return true;
}

void CViewStatistics::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, UINT nSheetID)
{
	ASSERT_VALID( pParent );
	ASSERT_VALID( pPSheet );

	CRect rcNewPos;
	CWnd* pWndNewArea = pParent->GetDlgItem(nSheetID);

	if(pWndNewArea == NULL)
		return;

	pWndNewArea->GetWindowRect(&rcNewPos);
	pParent->ScreenToClient(&rcNewPos);

	SizePropSheet(pParent, pPSheet, rcNewPos);
}

void CViewStatistics::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, CRect rcNewPos)
{
	ASSERT_VALID( pParent );
	ASSERT_VALID( pPSheet );

	CTabCtrl* pTabCtrl = pPSheet->GetTabControl();
	ASSERT(pTabCtrl != NULL);

	CRect rcTabCtrl;
	pTabCtrl->GetWindowRect(&rcTabCtrl);
	pParent->ScreenToClient(&rcTabCtrl);

	CRect rcPSheet;
	pPSheet->GetWindowRect(&rcPSheet);
	pParent->ScreenToClient(&rcPSheet);

	int dcx = rcPSheet.Width() - rcTabCtrl.Width();
	int dcy = rcPSheet.Height() - rcTabCtrl.Height();

	pPSheet->MoveWindow(rcNewPos.left, rcNewPos.top, rcNewPos.Width(), rcNewPos.Height());

	pTabCtrl->SetWindowPos(NULL, 0, 0, rcNewPos.Width() - dcx, rcNewPos.Height() - dcy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

	int nCurrentPage = pPSheet->GetActiveIndex();
	//for(int i = 0; i < pPSheet->GetPageCount(); ++i)
	//	pPSheet->SetActivePage(i);

	pPSheet->SetActivePage(nCurrentPage);
}

/////////////////////////////////////////////////////////////////////////

void CViewStatistics::SelectNode(int NodeID)
{
	for (int i = 0; i < m_lstNodes.GetItemCount(); i++)
		if(m_lstNodes.GetItemData(i) == NodeID)
			m_lstNodes.SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);

	if(!m_Paused)
	{
		m_TabDistribution->UpdateDistribution();
		m_TabPackets->UpdateData();
		m_TabBandwidth->UpdateBandwidth();
	}
}

CGnucleusDoc* CViewStatistics::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGnucleusDoc)));
	return (CGnucleusDoc*)m_pDocument;
}

void CViewStatistics::OnItemchangedListNodes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(!m_Paused)
	{
		//m_TabDistribution->UpdateDistribution();
		m_TabPackets->UpdateData();
		m_TabBandwidth->UpdateBandwidth();
	}
	
	*pResult = 0;
}

void CViewStatistics::OnRclickListNodes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstNodes.GetSelectionMark();
	if (0 > nItem || nItem >= m_lstNodes.GetItemCount())
		return;

    CMenu menu;
    menu.LoadMenu(IDR_STATISTICS_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
       point.x, point.y, this);

	POSITION pos = m_lstNodes.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_LSTNODES_REFRESH:
		while(pos)
		{
			pos = m_lstNodes.GetFirstSelectedItemPosition();
			nItem = m_lstNodes.GetNextSelectedItem(pos);

			if (0 <= nItem && nItem < m_lstNodes.GetItemCount())
				if(m_lstNodes.GetItemData(nItem))
				{
					// (m_lstNodes.GetItemData(nItem))->Refresh();
				}
		}
		break;
	case ID_LSTNODES_INFO:
		if(pos)
		{
			nItem = m_lstNodes.GetSelectionMark();

			if (0 <= nItem && nItem < m_lstNodes.GetItemCount())
				ExtendedInfo(m_lstNodes.GetItemData(nItem));
		}
		break;
	case ID_LSTNODES_DISCONNECT:
		while(pos)
		{
			pos = m_lstNodes.GetFirstSelectedItemPosition();
			nItem = m_lstNodes.GetNextSelectedItem(pos);

			if (0 <= nItem && nItem < m_lstNodes.GetItemCount())
				m_autNetwork->RemoveNode(m_lstNodes.GetItemData(nItem));
		}
		break;
	}
		
	*pResult = 0;
}

void CViewStatistics::OnDblclkListNodes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!

	int nItem = m_lstNodes.GetSelectionMark();

	if(nItem >= 0)
		ExtendedInfo(m_lstNodes.GetItemData(nItem));

	*pResult = 0;
}

void CViewStatistics::ExtendedInfo(int NodeID)
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



//void CViewStatistics::OnTimer(UINT nIDEvent)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CFormView::OnTimer(nIDEvent);
//}

BOOL CViewStatistics::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CFormView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}
