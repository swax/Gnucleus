/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
	Copyright (C) 2000-2005 John Marshall Group

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	By contributing code you grant John Marshall Group an unlimited, non-exclusive
	license to your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/


#include "stdafx.h"
#include "gnucleus.h"
#include "GnucleusDoc.h"
#include "AutNetwork.h"

#include "ViewStatistics.h"
#include "StatisticsMain.h"
#include "StatisticsBandwidth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsBandwidth, CPropertyPage)

CStatisticsBandwidth::CStatisticsBandwidth(CViewStatistics *pView) : CPropertyPage(CStatisticsBandwidth::IDD)
{
	m_pStatView  = pView;
	m_autNetwork = pView->m_autNetwork;

	//{{AFX_DATA_INIT(CStatisticsBandwidth)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStatisticsBandwidth::~CStatisticsBandwidth()
{
}

void CStatisticsBandwidth::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsBandwidth)
	DDX_Control(pDX, IDC_LIST_BANDWIDTH, m_lstBandwidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsBandwidth, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsBandwidth)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsBandwidth::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Setup list box
	CRect rect;
	m_lstBandwidth.GetWindowRect(&rect);

	int offSet = m_lstBandwidth.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	m_lstBandwidth.InsertColumn( 0, "Direction", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/3, 0);
	m_lstBandwidth.InsertColumn( 1, "KB/s", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 1);
	m_lstBandwidth.InsertColumn( 2, "Packets", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 2);

	m_lstBandwidth.InsertItem(0, "Dropped");
	m_lstBandwidth.InsertItem(0, "Total");
	m_lstBandwidth.InsertItem(0, "");

	m_lstBandwidth.InsertItem(0, "Out");
	m_lstBandwidth.InsertItem(0, "In");

	m_lstBandwidth.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Start up Timer
	m_nTimerID = SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsBandwidth::UpdateBandwidth()
{
	double PacketsRcvd = 0, PacketsSent = 0, PacketsTotal = 0, PacketsDropped = 0;
	long	BytesRcvd  = 0, BytesSent   = 0, BytesTotal   = 0, BytesDropped = 0;

	POSITION pos = m_pStatView->m_lstNodes.GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_pStatView->m_lstNodes.GetNextSelectedItem(pos);
		
		int NodeID = m_pStatView->m_lstNodes.GetItemData(nItem);
		
		BytesRcvd    += m_autNetwork->GetNodeBytesDown(NodeID);
		BytesSent    += m_autNetwork->GetNodeBytesUp(NodeID);
		BytesTotal   += BytesRcvd + BytesSent;
		BytesDropped += m_autNetwork->GetNodeBytesDropped(NodeID);

		PacketsRcvd    += m_autNetwork->GetNodePacketsDown(NodeID);
		PacketsSent    += m_autNetwork->GetNodePacketsUp(NodeID);
		PacketsTotal   += PacketsRcvd + PacketsSent;
		PacketsDropped += m_autNetwork->GetNodePacketsDropped(NodeID);	
	}
	
	m_lstBandwidth.SetItemText(0, 1, InsertDecimal( (double) BytesRcvd  / (double) 1024));
	m_lstBandwidth.SetItemText(1, 1, InsertDecimal( (double) BytesSent  / (double) 1024));
	m_lstBandwidth.SetItemText(3, 1, InsertDecimal((double) BytesTotal / (double) 1024));
	m_lstBandwidth.SetItemText(4, 1, InsertDecimal( (double) BytesDropped  / (double) 1024));

	m_lstBandwidth.SetItemText(0, 2, InsertDecimal( PacketsRcvd ));
	m_lstBandwidth.SetItemText(1, 2, InsertDecimal( PacketsSent ));
	m_lstBandwidth.SetItemText(3, 2, InsertDecimal( PacketsTotal));
	m_lstBandwidth.SetItemText(4, 2, InsertDecimal( PacketsDropped));


	// Update Summary
	m_pStatView->m_TabMain->m_Bandwidth = InsertDecimal( (double) BytesTotal / (double) 1024);
	m_pStatView->m_TabMain->UpdateSummary();
}

void CStatisticsBandwidth::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_nTimerID && !m_pStatView->m_Paused)
		UpdateBandwidth();
	
	CPropertyPage::OnTimer(nIDEvent);
}
