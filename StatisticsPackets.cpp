/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2004 John Marshall

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
#include "AutNetwork.h"

#include "ViewStatistics.h"
#include "StatisticsMain.h"
#include "StatisticsPackets.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsPackets, CPropertyPage)

CStatisticsPackets::CStatisticsPackets(CViewStatistics *pView) : CPropertyPage(CStatisticsPackets::IDD)
{
	m_pStatView = pView;
	m_autNetwork = pView->m_autNetwork;

	//{{AFX_DATA_INIT(CStatisticsPackets)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStatisticsPackets::~CStatisticsPackets()
{
}

void CStatisticsPackets::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsPackets)
	DDX_Control(pDX, IDC_LIST_DATA, m_lstData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsPackets, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsPackets)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsPackets::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// ListBox - Node
	CRect rect;
	m_lstData.GetWindowRect(&rect);

	int offSet = m_lstData.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;


	m_lstData.InsertColumn( 0, "Packet", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/3, 0);
	m_lstData.InsertColumn( 1, "Traffic", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 1);
	m_lstData.InsertColumn( 2, "Efficiency", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 2);

	m_lstData.InsertItem(0, "Total");
	m_lstData.InsertItem(0, "");

	m_lstData.InsertItem(0, "Push");
	m_lstData.InsertItem(0, "Query Hit");
	m_lstData.InsertItem(0, "Query");
	m_lstData.InsertItem(0, "Pong");
	m_lstData.InsertItem(0, "Ping");

	m_lstData.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Update Distribution Listbox
	UpdateData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsPackets::UpdateData()
{
	long  PingsTotalSum = 0, PongsTotalSum = 0, QueriesTotalSum = 0,
		   QueryHitsTotalSum = 0, PushesTotalSum = 0, PacketTotalSum = 0;

	long PingsGoodSum = 0, PongsGoodSum = 0, QueriesGoodSum = 0,
		   QueryHitsGoodSum = 0, PushesGoodSum = 0, PacketsGoodSum = 0;	

	// Update data variables
	POSITION pos = m_pStatView->m_lstNodes.GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_pStatView->m_lstNodes.GetNextSelectedItem(pos);
		
		int NodeID = m_pStatView->m_lstNodes.GetItemData(nItem);

		long  PingsTotal = 0, PongsTotal = 0, QueriesTotal = 0,
		   QueryHitsTotal = 0, PushesTotal = 0, PacketTotal = 0;

		long PingsGood = 0, PongsGood = 0, QueriesGood = 0,
		   QueryHitsGood = 0, PushesGood = 0, PacketsGood = 0;	

		m_autNetwork->GetNodePacketsPing(NodeID, &PingsGood, &PingsTotal);
		m_autNetwork->GetNodePacketsPong(NodeID, &PongsGood, &PongsTotal);
		m_autNetwork->GetNodePacketsQuery(NodeID, &QueriesGood, &QueriesTotal);
		m_autNetwork->GetNodePacketsQueryHit(NodeID, &QueryHitsGood, &QueryHitsTotal);
		m_autNetwork->GetNodePacketsPush(NodeID, &PushesGood, &PushesTotal);
		m_autNetwork->GetNodePacketsTotal(NodeID, &PacketsGood, &PacketTotal);

		PingsTotalSum	+= PingsTotal; 
		PongsTotalSum	+= PongsTotal;
		QueriesTotalSum +=QueriesTotal;
		QueryHitsTotalSum += QueryHitsTotal; 
		PushesTotalSum	+= PushesTotal; 
		PacketTotalSum	+= PacketTotal;

		PingsGoodSum	+= PingsGood;
		PongsGoodSum	+= PongsGood; 
		QueriesGoodSum	+= QueriesGood;
		QueryHitsGoodSum += QueryHitsGood;
		PushesGoodSum	+= PushesGood;
		PacketsGoodSum	+= PacketsGood;	
	}


	// Traffic
	m_lstData.SetItemText(0, 1, GetPercentage(PacketTotalSum, PingsTotalSum));
	m_lstData.SetItemText(1, 1, GetPercentage(PacketTotalSum, PongsTotalSum));
	m_lstData.SetItemText(2, 1, GetPercentage(PacketTotalSum, QueriesTotalSum));
	m_lstData.SetItemText(3, 1, GetPercentage(PacketTotalSum, QueryHitsTotalSum));
	m_lstData.SetItemText(4, 1, GetPercentage(PacketTotalSum, PushesTotalSum));

	// Efficeincy
	m_lstData.SetItemText(0, 2, GetPercentage(PingsTotalSum, PingsGoodSum));
	m_lstData.SetItemText(1, 2, GetPercentage(PongsTotalSum, PongsGoodSum));
	m_lstData.SetItemText(2, 2, GetPercentage(QueriesTotalSum, QueriesGoodSum));
	m_lstData.SetItemText(3, 2, GetPercentage(QueryHitsTotalSum, QueryHitsGoodSum));
	m_lstData.SetItemText(4, 2, GetPercentage(PushesTotalSum, PushesGoodSum));

	// Total
	CString Efficiency  = GetPercentage(PacketTotalSum, PacketsGoodSum);
	m_lstData.SetItemText(6, 2, Efficiency);

	// Update Summary
	m_pStatView->m_TabMain->m_Efficiency  = Efficiency;
	m_pStatView->m_TabMain->UpdateSummary();
}

