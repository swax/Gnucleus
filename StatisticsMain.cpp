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

#include "ViewStatistics.h"
#include "StatisticsMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsMain, CPropertyPage)

CStatisticsMain::CStatisticsMain(CViewStatistics *pView) : CPropertyPage(CStatisticsMain::IDD)
{
	m_pStatView = pView;

	//{{AFX_DATA_INIT(CStatisticsMain)
	//}}AFX_DATA_INIT
}

CStatisticsMain::~CStatisticsMain()
{
}

void CStatisticsMain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsMain)
	DDX_Control(pDX, IDC_STATIC_VERSION, m_stcVersion);
	DDX_Control(pDX, IDC_LIST_SUMMARY, m_lstSummary);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsMain, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsMain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsMain::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Set version text
	CString Title = "Gnucleus version ";
	m_stcVersion.SetWindowText(Title);

	// Setup list box
	CRect rect;
	m_lstSummary.GetWindowRect(&rect);

	int offSet = m_lstSummary.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	m_lstSummary.InsertColumn( 0, "Current", LVCFMT_LEFT,
		(rect.Width() - offSet) * 2/3, 0);
	m_lstSummary.InsertColumn( 1, "Status", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 1);

	// Items to list
	//m_lstSummary.InsertItem(0, "Monitored Memory");		// 3
	m_lstSummary.InsertItem(0, "Bandwidth Usage");		// 2
	m_lstSummary.InsertItem(0, "Network Efficiency");	// 1
	//m_lstSummary.InsertItem(0, "Nodes Sharing");		// 0

	m_lstSummary.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsMain::UpdateSummary()
{
	//m_lstSummary.SetItemText(0, 1, m_Sharing);
	m_lstSummary.SetItemText(0, 1, m_Efficiency);
	m_lstSummary.SetItemText(1, 1, m_Bandwidth + " KB/s");
	//m_lstSummary.SetItemText(3, 1, m_Memory + " KB");
}