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

#include "ViewStatistics.h"
#include "StatisticsMain.h"
#include "StatisticsMemory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsMemory, CPropertyPage)

CStatisticsMemory::CStatisticsMemory(CViewStatistics *pView) : CPropertyPage(CStatisticsMemory::IDD)
{
	m_pStatView = pView;

	if(pView)
		m_pDoc = pView->m_pDoc;

	//{{AFX_DATA_INIT(CStatisticsMemory)
	//}}AFX_DATA_INIT
}

CStatisticsMemory::~CStatisticsMemory()
{
}

void CStatisticsMemory::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsMemory)
	DDX_Control(pDX, IDC_STATIC_TEMP9, m_stcTemp9);
	DDX_Control(pDX, IDC_STATIC_TEMP8, m_stcTemp8);
	DDX_Control(pDX, IDC_STATIC_TEMP7, m_stcTemp7);
	DDX_Control(pDX, IDC_STATIC_TEMP6, m_stcTemp6);
	DDX_Control(pDX, IDC_STATIC_TEMP5, m_stcTemp5);
	DDX_Control(pDX, IDC_STATIC_TEMP4, m_stcTemp4);
	DDX_Control(pDX, IDC_STATIC_TEMP3, m_stcTemp3);
	DDX_Control(pDX, IDC_STATIC_TEMP2, m_stcTemp2);
	DDX_Control(pDX, IDC_STATIC_TEMP1, m_stcTemp1);
	DDX_Control(pDX, IDC_LIST_MEMORYZ, m_lstMemory);
	DDX_Control(pDX, IDC_EDIT_HASH, m_ebHash);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsMemory, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsMemory)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsMemory::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Setup list box
	CRect rect;
	m_lstMemory.GetWindowRect(&rect);

	int offSet = m_lstMemory.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	m_lstMemory.InsertColumn( 0, "Structure", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/3, 0);
	m_lstMemory.InsertColumn( 1, "Size", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 1);
	m_lstMemory.InsertColumn( 2, "Memory", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/3, 2);

	m_lstMemory.InsertItem(0, "Total");
	m_lstMemory.InsertItem(0, "");

	// Items to list
	m_lstMemory.InsertItem(0, "Error Log");			// 8
	m_lstMemory.InsertItem(0, "Share Table");		// 7
	m_lstMemory.InsertItem(0, "Hash Tables");		// 6
	m_lstMemory.InsertItem(0, "Misc Sockets");		// 5
	m_lstMemory.InsertItem(0, "Upload Sockets");	// 4
	m_lstMemory.InsertItem(0, "Download Sockets");	// 3
	m_lstMemory.InsertItem(0, "Node Sockets");		// 2
	m_lstMemory.InsertItem(0, "LAN Host List");		// 1
	m_lstMemory.InsertItem(0, "Cache List");		// 0

	m_lstMemory.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Start up Timer
	m_nTimerID = SetTimer(1, 1000, NULL);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsMemory::OnTimer(UINT nIDEvent) 
{
	//if(m_nTimerID == nIDEvent && !m_pStatView->m_Paused)
	//{
	//	int TotalSize = 0;

	//	// Cache
	//	int Size = m_pDoc->m_pCache->m_PermNodes.size() +  m_pDoc->m_pCache->m_RealNodes.size();
	//	m_lstMemory.SetItemText(0, 1, CommaIze( DWrdtoStr(Size)));
	//	m_lstMemory.SetItemText(0, 2, CommaIze( DWrdtoStr((Size * sizeof(Node)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(Node);

	//	// LAN Host List
	//	Size = m_pDoc->m_pComm->m_LANSock->m_HostList.size();
	//	m_lstMemory.SetItemText(1, 1, CommaIze( DWrdtoStr(Size)));
	//	m_lstMemory.SetItemText(1, 2, CommaIze( DWrdtoStr((Size * sizeof(Node)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(Node);

	//	// Nodes
	//	Size         = m_pDoc->m_pComm->m_NodeList.size();
	//	int NodeSize = Size;
	//	
	//	m_lstMemory.SetItemText(2, 1, CommaIze( DWrdtoStr(Size)));
	//	m_lstMemory.SetItemText(2, 2, CommaIze( DWrdtoStr((Size * sizeof(CGnuNode)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(CGnuNode);

	//	// Downloads
	//	Size = m_pDoc->m_pComm->m_DownloadList.size();
	//	
	//	m_lstMemory.SetItemText(3, 1, CommaIze( DWrdtoStr(Size)));
	//	m_lstMemory.SetItemText(3, 2, CommaIze( DWrdtoStr((Size * sizeof(CGnuDownload)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(CGnuDownload);

	//	// Uploads
	//	Size = m_pDoc->m_pComm->m_UploadList.size();
	//	
	//	m_lstMemory.SetItemText(4, 1, CommaIze( DWrdtoStr(Size)));
	//	m_lstMemory.SetItemText(4, 2, CommaIze( DWrdtoStr((Size * sizeof(CGnuUploadShell)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(CGnuUploadShell);

	//	// Misc Sock
	//	Size = m_pDoc->m_pComm->m_SockList.size();

	//	m_lstMemory.SetItemText(5, 1, CommaIze( DWrdtoStr(Size)));
	//	m_lstMemory.SetItemText(5, 2, CommaIze( DWrdtoStr((Size * sizeof(CGnuSock)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(CGnuSock);

	//	// Hash Tables - 2 Tables of CACHE_SIZE per hash, 3 hashes per connect
	//	Size = PACKETCACHE_SIZE * 2 * 3 * NodeSize;
	//	m_lstMemory.SetItemText(6, 1, CommaIze( DWrdtoStr( Size)));
	//	m_lstMemory.SetItemText(6, 2, CommaIze( DWrdtoStr((Size * sizeof(key_Value)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(key_Value);

	//	// Share Table
	//	Size = m_pDoc->m_pShare->m_SharedFiles.size();
	//	m_lstMemory.SetItemText(7, 1, CommaIze( DWrdtoStr( Size)));
	//	m_lstMemory.SetItemText(7, 2, CommaIze( DWrdtoStr((Size * 50) / 1024)) + " KB");
	//	TotalSize += Size * 50;

	//	// Share Table
	//	Size = m_pDoc->m_ErrorList.size();
	//	m_lstMemory.SetItemText(8, 1, CommaIze( DWrdtoStr( Size)));
	//	m_lstMemory.SetItemText(8, 2, CommaIze( DWrdtoStr((Size * sizeof(ErrorInfo)) / 1024)) + " KB");
	//	TotalSize += Size * sizeof(ErrorInfo);

	//	// Total Size
	//	CString Memory = CommaIze( DWrdtoStr(TotalSize / 1024));
	//	m_lstMemory.SetItemText(10, 2, Memory + " KB");

	//	// Update Summary
	//	m_pStatView->m_TabMain->m_Memory = Memory;
	//	m_pStatView->m_TabMain->UpdateSummary();	


	//	// Hash Time Span
	//	CString Refresh =  DWrdtoStr(m_pDoc->m_pComm->m_TableRouting.HashTimeSpan.GetMinutes()) + "m ";
	//			Refresh += DWrdtoStr(m_pDoc->m_pComm->m_TableRouting.HashTimeSpan.GetSeconds()) + "s";
	//
	//	m_ebHash.SetWindowText(Refresh);

	//	CGnuShare* pShare = m_pDoc->m_pShare;
	//	CGnuWordHash* pTable = pShare->m_pWordTable;

	//	m_stcTemp1.SetWindowText("Table Size: " + CommaIze(DWrdtoStr(pTable->m_TableSize)));
	//	
	//	m_stcTemp3.SetWindowText("Words Hashed: " + CommaIze(DWrdtoStr(pTable->m_HashedWords)));
	//	m_stcTemp4.SetWindowText("Largest Rehash: " + CommaIze(DWrdtoStr(pTable->m_LargestRehash)));
	//	
	//	m_stcTemp6.SetWindowText("Unique Slots: " + CommaIze(DWrdtoStr(pTable->m_UniqueSlots)));
	//	m_stcTemp7.SetWindowText("Remote Slots: " + CommaIze(DWrdtoStr(pTable->m_RemoteSlots)));
	//	

	//	/*
	//	m_stcTemp1.SetWindowText("Freq: " + CommaIze(DWrdtoStr( pShare->m_Freq)) + " cycs/sec");
	//	m_stcTemp3.SetWindowText("Avg Thread Usage/Sec: " + InsertDec( pShare->m_UsagePerSecTotal / 60.0, 5) + " sec");
	//	m_stcTemp4.SetWindowText("Avg Queries/Sec: " + InsertDec( pShare->m_QueriesPerSecTotal / 60.0, 5));

	//	if(pShare->m_QueriesPerSecTotal)
	//		m_stcTemp5.SetWindowText("Avg Ticks/Query: " + InsertDec( pShare->m_UsagePerSecTotal / pShare->m_QueriesPerSecTotal, 5) + " sec");
	//
	//	m_stcTemp7.SetWindowText("Queries last sec: " + CommaIze(DWrdtoStr(pShare->m_QueryAmount)));
	//	m_stcTemp8.SetWindowText("Query Backlog: " + CommaIze(DWrdtoStr(pShare->m_QueryQueue)));

	//	if(pShare->m_MaxFileIndexSize)
	//		m_stcTemp9.SetWindowText("Est. Max Index Size: " + CommaIze( DWrdtoStr(pShare->m_MaxFileIndexTotal / pShare->m_MaxFileIndexSize)));
	//	*/
	// }

	CPropertyPage::OnTimer(nIDEvent);
}

CString CStatisticsMemory::InsertDec(double dNumber, int place)
{
	int    decimal, sign;

	CString strNumber(_fcvt( dNumber, place, &decimal, &sign));
	

	if(decimal > 0)
	{
		strNumber.Insert(decimal, '.');
	}
	else
	{
		CString Padding = "0.";

		while(decimal < 0)
		{
			Padding += "0";
			decimal++;
		}

		strNumber = Padding + strNumber; 
	}

	return strNumber;
}
