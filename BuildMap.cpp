/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2003 John Marshall

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
#include "FrameMain.h"

#include "BuildMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuildMap::CBuildMap(CWnd* pParent /*=NULL*/, CFrameMain* pFrame /*=NULL*/)
	: CDialog(CBuildMap::IDD, pParent)
{
	m_PongsTimerID = 0;
	m_Building     = false;

	m_pFrame	   = pFrame;
	m_pDoc		   = pFrame->m_pDoc;

	//{{AFX_DATA_INIT(CBuildMap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBuildMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuildMap)
	DDX_Control(pDX, IDC_CHECK_SHAPENODES, m_chkShapeNodes);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_ebWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_ebHeight);
	DDX_Control(pDX, IDC_BUTTON_BUILD, m_btnBuild);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_EDIT_HOPS, m_ebHops);
	DDX_Control(pDX, IDC_CHECK_SIZENODES, m_chkSizeNodes);
	DDX_Control(pDX, IDC_STATIC_NODES, m_stcNodes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBuildMap, CDialog)
	//{{AFX_MSG_MAP(CBuildMap)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBuildMap::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ebHops.SetWindowText("4");
	m_chkSizeNodes.SetCheck(true);

	m_ebHeight.SetWindowText("10");
	m_ebWidth.SetWindowText("10");

	m_PongsTimerID = SetTimer(1, 1000, NULL);

	//m_pDoc->m_LocalMapping = true;
	
	return true;  
}


void CBuildMap::OnButtonRefresh() 
{
	//m_pComm->MapTable.clear(); 

	//// Insert ourselves
	//MapNode Localhost;
	//Localhost.MapID		  = 0;
	//Localhost.Host		  = m_pDoc->m_pPrefs->m_ForcedHost.S_addr ? m_pDoc->m_pPrefs->m_ForcedHost : m_pDoc->m_pPrefs->m_LocalHost;
	//Localhost.Port        = (WORD) m_pDoc->m_pPrefs->m_ForcedPort ? m_pDoc->m_pPrefs->m_ForcedPort : m_pDoc->m_pPrefs->m_LocalPort;
	//m_pComm->MapTable.push_back(Localhost);

	//// Activate mapping, start trapping pongs
	//m_pDoc->m_LocalMapping = true;

	//for(int i = 0; i < m_pComm->m_NodeList.size(); i++)
	//	m_pComm->m_NodeList[i]->Refresh();
}

void CBuildMap::OnTimer(UINT nIDEvent) 
{
	if(m_PongsTimerID == nIDEvent)
	{
		//m_stcNodes.SetWindowText( CommaIze( DWrdtoStr( m_pComm->MapTable.size())));

		if(m_Building)
		{
			bool Finished = false;

			CFile NetMap;

			if( NetMap.Open((m_pDoc->m_RunPath + "Graphing\\graph.gif"), CFile::modeRead, NULL))
			{
				Finished = true;

				CString Path = NetMap.GetFilePath();
				NetMap.Close();

				m_pFrame->OpenBrowser(Path);
			}
			else
			{
				CString Dots;
				m_btnBuild.GetWindowText(Dots);
				
				if(Dots.Find("...") != -1)
					m_btnBuild.SetWindowText("Building");
				else
					m_btnBuild.SetWindowText(Dots + ".");


				m_TimeBuilding++;

				if(m_TimeBuilding == 60)
					AfxMessageBox("This map has been building for a minute,\r\n your map is probably too big to be created.\r\n\r\nEither continue waiting or use CTRL-ALT-DEL\r\nand Select End Task, Neato.\r\n\r\nLess intensive settings enable the map to be built faster.");
				// Expand this so gnucleus ends Neato for the user
				// FindWindow("tty", "Neato");  to get the window
			}

			if(Finished)
			{
				m_Building = false;

				m_btnRefresh.EnableWindow(true);
				m_btnBuild.EnableWindow(true);

				m_btnBuild.SetWindowText("Build Map");
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CBuildMap::OnButtonBuild() 
{
	CreateDirectory(m_pDoc->m_RunPath + "Graphing\\", NULL);
	CFile MapFile;
	
	if(!MapFile.Open(m_pDoc->m_RunPath + "Graphing\\graph.txt", CFile::modeCreate | CFile::modeWrite))
		return;

	CString strHops, strHeight, strWidth;

	// Get hops pref
	m_ebHops.GetWindowText(strHops);
	int MaxHops = 3;
	if(strHops != "")
		MaxHops = atoi(strHops);

	// Get height pref
	m_ebHeight.GetWindowText(strHeight);
	int MapHeight = 3;
	if(strHeight != "")
		MapHeight = atoi(strHeight);

	// Get width pref
	m_ebWidth.GetWindowText(strWidth);
	int MapWidth = 3;
	if(strWidth != "")
		MapWidth = atoi(strWidth);

	// Graph header, default settings here
	CString Header = "graph GnutellaNet \r\n";
			Header += "{ \r\n";
			Header += "\t size=\"" + DWrdtoStr(MapHeight) + "," + DWrdtoStr(MapWidth) + "\" \r\n";
			Header += "\t graph[center=\"" + InsertDecimal((double)MapHeight / 2) + "," + InsertDecimal((double)MapWidth / 2) + "\"] \r\n";
			Header += "\t node [shape=circle,width=.2,height=.2,label=\"\"] \r\n";
	

	// Put ourselves at the center of the graph
	/*CString DotSize = GetDotSize(m_pDoc->m_pShare->m_TotalLocalFiles);
	CString Size    = "width=."    + DotSize;
		    Size   += ", height=." + DotSize;

	Header += "\r\n\t 0 [label=\"You\", fontsize=10, style=filled, color=red, " + Size + "] \r\n\r\n";
	MapFile.Write(Header, Header.GetLength());*/

	m_DetailList.clear();

	//// Map all the branches of nodes from us
	//for(int i = 0; i < m_pComm->m_NodeList.size(); i++)	
	//{
	//	CGnuNode* p = m_pComm->m_NodeList[i];

	//	if(m_pComm->m_NodeList[i]->m_Status != SOCK_CONNECTED)
	//		continue;

	//	// Maps the node directly connected
	//	for(int j = 0; j < p->LocalMap[0].size(); j++)
	//	{
	//		CString NextNode = "\t " + DWrdtoStr(0) + " -- ";
	//				NextNode += DWrdtoStr(p->LocalMap[0][j]) + " [" + GetMapColor(1) + "]\r\n";

	//		MapFile.Write(NextNode, NextNode.GetLength());
	//	}

	//	// Maps node connections
	//	for(j = 0; j < p->MapLinkList.size(); j++)
	//	{
	//		bool Insert = false;

	//		if(p->MapLinkList[j].Hops <= MaxHops)
	//			Insert = true;

	//		if(Insert)
	//		{
	//			AddDetail(p->MapLinkList[j].ParentID);
	//			AddDetail(p->MapLinkList[j].ID);

	//			CString NodeLink = "\t " + DWrdtoStr( p->MapLinkList[j].ParentID );
	//					NodeLink += " -- ";
	//					NodeLink += DWrdtoStr(p->MapLinkList[j].ID) + " [" + GetMapColor(p->MapLinkList[j].Hops) + "]\r\n";
	//			
	//			MapFile.Write(NodeLink, NodeLink.GetLength());
	//		}
	//	}

	//	MapFile.Write("\r\n\r\n", 4);
	//}

	// Write each node's details
	//m_DetailList.sort();
	//
	//std::list<DWORD>::iterator itNode;
	//for(itNode = m_DetailList.begin(); itNode != m_DetailList.end(); itNode++)
	//	for(i = 0; i < m_pComm->MapTable.size(); i++)
	//		if(*itNode == m_pComm->MapTable[i].MapID)
	//		{
	//			// Get size of node
	//			CString Size;
	//			if(m_chkSizeNodes.GetCheck())
	//			{
	//				DotSize = GetDotSize(m_pComm->MapTable[i].FileSize);
	//				Size  =  ",width=."  + DotSize;
	//				Size +=  ",height=." + DotSize;
	//			}

	//			// Get shape of node
	//			CString Shape = "circle";
	//			if(m_chkShapeNodes.GetCheck())
	//			{
	//				if(strcmp(m_pComm->MapTable[i].Client, "BEAR") == 0) 
	//					Shape = "box";
	//				
	//				else if(strcmp(m_pComm->MapTable[i].Client, "LIME") == 0|| strcmp(m_pComm->MapTable[i].Client, "SNUT") == 0) 
	//					Shape = "diamond";

	//				else if(strcmp(m_pComm->MapTable[i].Client, "GNUC") == 0) 
	//					Shape = "triangle";

	//				else
	//					Shape = "circle";
	//			}

	//			// Write node's details
	//			CString Details = "\t " + DWrdtoStr(m_pComm->MapTable[i].MapID) + " [shape=" + Shape + Size + "]\r\n";
	//			MapFile.Write(Details, Details.GetLength());
	//		}

	MapFile.Write("\r\n\r\n", 4);
	
	Header = "\r\n} \r\n";
	MapFile.Write((LPCTSTR) Header, Header.GetLength());
	
	MapFile.Close();

	DeleteFile(m_pDoc->m_RunPath + "Graphing\\graph.gif");

	if( (int) ShellExecute(NULL, "open", "neato.exe", "-Tgif graph.txt -o graph.gif", m_pDoc->m_RunPath + "Graphing\\", SW_HIDE) > 32)
	{
		m_Building     = true;
		m_TimeBuilding = 0;

		m_btnRefresh.EnableWindow(false);
		m_btnBuild.EnableWindow(false);

		m_btnBuild.SetWindowText("Building");
	}
	else
		AfxMessageBox("Error Launching Graphing Software");
}

void CBuildMap::AddDetail(DWORD NodeID)
{
	std::list<DWORD>::iterator itNode;
	for(itNode = m_DetailList.begin(); itNode != m_DetailList.end(); itNode++)
		if( *itNode == NodeID )
			return;

	m_DetailList.push_back(NodeID);
}

CString CBuildMap::GetDotSize(DWORD dwSize)
{
	// Get node size, * 100 to make up for decimal point
	if(dwSize > 3000000)
		dwSize = 3000000;

	dwSize = dwSize / 25000 + 100;

	return DWrdtoStr(dwSize);
}

void CBuildMap::PostNcDestroy() 
{
	delete this;

	CDialog::PostNcDestroy();
}



void CBuildMap::OnButtonRead() 
{
	ShellExecute(NULL, "open", m_pDoc->m_RunPath + "Graphing\\ATT_Binary_License.txt", NULL, NULL, SW_SHOW);
}

void CBuildMap::OnDestroy() 
{
	CDialog::OnDestroy();
	
	
	
}

void CBuildMap::OnClose() 
{
	m_pFrame->m_pBuildDlg = NULL;
	
	//m_pDoc->m_LocalMapping = false;

	//// Clear now, could be useful later though
	//m_pComm->MapTable.clear();

	//for(int i = 0; i < m_pComm->m_NodeList.size(); i++)
	//{
	//	m_pComm->m_NodeList[i]->MapLinkList.clear();

	//	for(int j = 0; j < MAX_TTL; j++)
	//		m_pComm->m_NodeList[i]->LocalMap[j].clear();
	//}


	CDialog::OnClose();
}
