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

#include "ViewStatistics.h"
#include "StatisticsMain.h"
#include "StatisticsDistribution.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsDistribution, CPropertyPage)

CStatisticsDistribution::CStatisticsDistribution(CViewStatistics *pView) : CPropertyPage(CStatisticsDistribution::IDD)
{
	m_pStatView = pView;

	for(int i = 0; i < MAX_TTL; i++)
	{
		m_colFriends[i] = 0;
		m_colLibrary[i] = 0;
	}

	dwTotalFriends = 0;
	dwTotalLibrary = 0;
	dwTotalSharing = 0;

	//{{AFX_DATA_INIT(CStatisticsDistribution)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStatisticsDistribution::~CStatisticsDistribution()
{
}

void CStatisticsDistribution::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsDistribution)
	DDX_Control(pDX, IDC_STATIC_SHARE, m_stcShare);
	DDX_Control(pDX, IDC_LIST_DIST, m_lstDist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsDistribution, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsDistribution)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsDistribution::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	int offSet = m_lstDist.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;
	CRect rect;
	m_lstDist.GetWindowRect(&rect);

	m_lstDist.InsertColumn( 0, "Hops", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 2/8, 0);
	m_lstDist.InsertColumn( 1, "Friends", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 2/8, 1);
	m_lstDist.InsertColumn( 2, "Library", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 4/8, 2);

	m_lstDist.InsertItem(0, "Total");
	m_lstDist.InsertItem(0, "");

	for(int i = MAX_TTL; i > 0; i--)
		m_lstDist.InsertItem(0, DWrdtoStr(i));

	m_lstDist.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	UpdateDistribution();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsDistribution::UpdateDistribution()
{
	//for(int i = 0; i < MAX_TTL; i++)
	//{
	//	m_colFriends[i] = 0;
	//	m_colLibrary[i] = 0;
	//}
	//dwTotalFriends = 0;
	//dwTotalLibrary = 0;
	//dwTotalSharing = 0;

	//// Update distribution variables
	//POSITION pos = m_pStatView->m_lstNodes.GetFirstSelectedItemPosition();
	//while(pos)
	//{
	//	int nItem = m_pStatView->m_lstNodes.GetNextSelectedItem(pos);
	//	
	//	int NodeID = m_pStatView->m_lstNodes.GetItemData(nItem);

	//	
	//	/*
	//	for(int i = 0; i < MAX_TTL; i++)
	//	{
	//		m_colFriends[i] += pNode->m_dwFriends[i];
	//		m_colLibrary[i] += pNode->m_dwLibrary[i];
	//	}

	//	dwTotalSharing += pNode->m_dwSharingTotal;
	//	*/
	//	
	//}

	//// Update Listbox - Items
	//for(i = 0; i < MAX_TTL; i++)
	//{
	//	m_lstDist.SetItemText(i, 1, CommaIze( DWrdtoStr(m_colFriends[i]) ));
	//	m_lstDist.SetItemText(i, 2, CommaIze( DWrdtoStr(m_colLibrary[i]/1024)) + " MB");
	//}

	//// Update Listbox - Total
	//for(i = 0; i < MAX_TTL; i++)
	//{
	//	dwTotalFriends += m_colFriends[i];
	//	dwTotalLibrary += m_colLibrary[i];
	//}
	//m_lstDist.SetItemText(MAX_TTL + 1, 1, CommaIze( DWrdtoStr(dwTotalFriends) ));
	//m_lstDist.SetItemText(MAX_TTL + 1, 2, CommaIze( DWrdtoStr(dwTotalLibrary/1024)) + " MB");

	//// Sharing percentage
	//CString Sharing = GetPercentage((double) dwTotalFriends, (double) dwTotalSharing);
	//m_stcShare.SetWindowText(Sharing);

	//// Update Summary
	//m_pStatView->m_TabMain->m_Sharing = Sharing;
	//m_pStatView->m_TabMain->UpdateSummary();
}

void CStatisticsDistribution::OnButtonRefresh() 
{
	POSITION pos = m_pStatView->m_lstNodes.GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_pStatView->m_lstNodes.GetNextSelectedItem(pos);
		
		int NodeID = m_pStatView->m_lstNodes.GetItemData(nItem);

		//pNode->Refresh();
	}

	UpdateDistribution();
}

void CStatisticsDistribution::PacketIncoming(packet_Header* packet)
{
	//if( packet->Function == 0x01 )
	//{
	//	packet_Pong* Pong = (packet_Pong*) packet;
	//	int nHops = Pong->Header.Hops - 1;
	//	
	//	m_colFriends[nHops]++;
	//	dwTotalFriends++;

	//	DWORD dwLibrary = MAX_SHARE < Pong->FileSize ? MAX_SHARE : Pong->FileSize;
	//	if(dwLibrary)
	//	{
	//		m_colLibrary[nHops] += dwLibrary;
	//		dwTotalLibrary += dwLibrary;
	//		dwTotalSharing++;
	//	}

	//	// Item
	//	m_lstDist.SetItemText(nHops, 1, CommaIze( DWrdtoStr(m_colFriends[nHops]) ));
	//	m_lstDist.SetItemText(nHops, 2, CommaIze( DWrdtoStr(m_colLibrary[nHops]/1024)) + " MB");

	//	// Total
	//	m_lstDist.SetItemText(MAX_TTL + 1, 1, CommaIze( DWrdtoStr(dwTotalFriends) ));
	//	m_lstDist.SetItemText(MAX_TTL + 1, 2, CommaIze( DWrdtoStr(dwTotalLibrary/1024)) + " MB");
	//
	//	// Sharing percentage
	//	CString Sharing = GetPercentage((double) dwTotalFriends, (double) dwTotalSharing);
	//	m_stcShare.SetWindowText(Sharing);

	//	// Update Summary
	//	m_pStatView->m_TabMain->m_Sharing = Sharing;
	//	m_pStatView->m_TabMain->UpdateSummary();
	//}


}
