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
	license your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/


#include "stdafx.h"
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "AutPrefs.h"
#include "AutCache.h"

#include "PrefsEx.h"
#include "PrefLocal.h"
#include "PrefLocalNetwork.h"
#include "PrefLocalFirewall.h"
#include "PrefConnect.h"
#include "PrefConnectServers.h"
#include "PrefConnectScreen.h"
#include "PrefSearch.h"
#include "PrefSearchBlock.h"
#include "PrefSearchScreen.h"
#include "PrefShare.h"
#include "PrefTransfer.h"
#include "PrefBandwidth.h"
#include "PrefChat.h"
#include "PrefPlugins.h"

#include "PrefDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPrefDialog, CPropertySheet)

CPrefDialog::CPrefDialog(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pDoc		= NULL;

	m_TabLocal			= new CPrefLocal;
	m_TabLocalNetwork	= new CPrefLocalNetwork;
	m_TabLocalFirewall	= new CPrefLocalFirewall;
	m_TabConnect		= new CPrefConnect;
	m_TabConnectServers	= new CPrefConnectServers;
	m_TabConnectScreen	= new CPrefConnectScreen;
	m_TabSearch			= new CPrefSearch;
	m_TabSearchScreen	= new CPrefSearchScreen;
	m_TabSearchBlock	= new CPrefSearchBlock;
	m_TabShare			= new CPrefShare;
	m_TabTransfer		= new CPrefTransfer;		
	m_TabBandwidth		= new CPrefBandwidth;
	m_TabChat			= new CPrefChat;
	m_TabPlugins		= new CPrefPlugins;

	AddPages();
}

CPrefDialog::CPrefDialog(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pDoc = NULL;

	m_TabLocal			= new CPrefLocal;
	m_TabLocalNetwork	= new CPrefLocalNetwork;
	m_TabLocalFirewall	= new CPrefLocalFirewall;
	m_TabConnect		= new CPrefConnect;
	m_TabConnectServers	= new CPrefConnectServers;
	m_TabConnectScreen	= new CPrefConnectScreen;
	m_TabSearch			= new CPrefSearch;
	m_TabSearchScreen	= new CPrefSearchScreen;
	m_TabSearchBlock	= new CPrefSearchBlock;
	m_TabShare			= new CPrefShare;
	m_TabTransfer		= new CPrefTransfer;		
	m_TabBandwidth		= new CPrefBandwidth;
	m_TabChat			= new CPrefChat;
	m_TabPlugins		= new CPrefPlugins;

	AddPages();
}

CPrefDialog::~CPrefDialog()
{
	// Save Configs
	m_pDoc->m_autPrefs->SaveConfig(m_pDoc->m_RunPath  + "Data\\GnuConfig.ini");
	m_pDoc->m_autPrefs->SaveBlocked(m_pDoc->m_RunPath  + "Data\\GnuBlocked.net");

	// Save node caches
	m_pDoc->m_autCache->SaveCache(m_pDoc->m_RunPath + "Data\\GnuCache.net");

	// Delete tabs
	delete m_TabLocal;		
	delete m_TabLocalNetwork;
	delete m_TabLocalFirewall;	
	delete m_TabConnect	;	
	delete m_TabConnectServers;
	delete m_TabConnectScreen;	
	delete m_TabSearch;	
	delete m_TabSearchScreen;	
	delete m_TabSearchBlock;
	delete m_TabShare;			
	delete m_TabTransfer;		
	delete m_TabBandwidth;	
	delete m_TabChat;
	delete m_TabPlugins;
}


BEGIN_MESSAGE_MAP(CPrefDialog, CPropertySheet)
	//{{AFX_MSG_MAP(CPrefDialog)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_PREF_TREE, OnSelchanged)
	ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP()


void CPrefDialog::AddPages()
{
	// Don't load the help button, maybe later we can put help in
	m_hIcon = AfxGetApp()->LoadIcon(IDR_SETTINGS);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags &= ~PSH_HASHELP;

	m_pDoc   = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	
	// Add Tabs
	AddPage(m_TabLocal);
	AddPage(m_TabLocalNetwork);
	AddPage(m_TabLocalFirewall);	
	AddPage(m_TabConnect);	
	AddPage(m_TabConnectServers);
	AddPage(m_TabConnectScreen);	
	AddPage(m_TabSearch);	
	AddPage(m_TabSearchScreen);	
	AddPage(m_TabSearchBlock);
	AddPage(m_TabShare);			
	AddPage(m_TabTransfer);		
	AddPage(m_TabBandwidth);
	AddPage(m_TabChat);
	//AddPage(m_TabPlugins);
}

BOOL CPrefDialog::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// Disable the tabs, so we can use the tree control.
	GetTabControl()->ShowWindow(SW_HIDE);
	
	// Setup the tree control dynamicly	
	CRect rectWnd, rectTree;
	GetWindowRect(rectWnd);
	ScreenToClient(rectWnd);

	rectTree.SetRect(PREF_TREE_MARGIN_LEFT, PREF_TREE_MARGIN_TOP, 
		PREF_TREE_MARGIN_LEFT + PREF_TREE_WIDTH, rectWnd.bottom - PREF_TREE_MARGIN_BOTTOM - 13);

	BOOL bCreateResult = m_Tree.Create( WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER |
		TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
		rectTree, this , IDC_PREF_TREE);

	//	m_Tree.ModifyStyleEx(0, WS_EX_WINDOWEDGE);
	
	// Flesh out the tree control.
	HTREEITEM Parent, Root, Select;

	Root   = m_Tree.InsertItem("Local"           );
			 m_Tree.InsertItem("Network",  Root  );
			 m_Tree.InsertItem("Firewall", Root  );
			 m_Tree.Expand(Root, TVE_EXPAND	     );
	
	Parent = m_Tree.InsertItem("Connect"         );
			 m_Tree.InsertItem("Servers", Parent );
			 m_Tree.InsertItem("Filter",  Parent );
			 m_Tree.Expand(Parent, TVE_EXPAND	 );
	
	Parent = m_Tree.InsertItem("Search"          );
			 m_Tree.InsertItem("Filter", Parent  );
			 m_Tree.InsertItem("Block",  Parent );
			 m_Tree.Expand(Parent, TVE_EXPAND	 );

			 m_Tree.InsertItem("Share"           );
			 m_Tree.InsertItem("Transfer"        );
			 m_Tree.InsertItem("Bandwidth"       );
			 m_Tree.InsertItem("Chat"			 );
			 //m_Tree.InsertItem("Plug-ins"		 );

	Select = Root;
	for(int i = 1; i < m_pDoc->m_nLastPref; i++)
		Select = m_Tree.GetNextItem(Select, TVGN_NEXTVISIBLE);

	if(Select)
		m_Tree.SelectItem(Select); // select the first item

	//save rect for property pages
	GetActivePage()->GetWindowRect(m_rectPage);
	ScreenToClient(m_rectPage);

	return bResult;
}

int CPrefDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectWnd;
	GetWindowRect(rectWnd);

	// add space for the tree control to the width of the dialog box.
	rectWnd.right += PREF_TREE_WIDTH + PREF_TREE_MARGIN_LEFT + PREF_TREE_MARGIN_RIGHT;

	MoveWindow(rectWnd);
	CenterWindow(NULL);
	
	return 0;
}

afx_msg void CPrefDialog::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	HTREEITEM Item = m_Tree.GetSelectedItem();

	if(Item)
	{
		CString Selected = m_Tree.GetItemText(Item);

		if(Selected == "Local")
			SetActivePage(m_TabLocal);
		if(Selected == "Network")
			SetActivePage(m_TabLocalNetwork);
		if(Selected == "Firewall")
			SetActivePage(m_TabLocalFirewall);
		if(Selected == "Connect")
			SetActivePage(m_TabConnect);
		if(Selected == "Servers")
			SetActivePage(m_TabConnectServers);
		if(Selected == "Filter" && m_Tree.GetItemText( m_Tree.GetParentItem(Item)) == "Connect")
		{
			Selected = m_Tree.GetItemText( m_Tree.GetParentItem(Item)) + " " + Selected;
			SetActivePage(m_TabConnectScreen);
		}
		if(Selected == "Search")
			SetActivePage(m_TabSearch);
		if(Selected == "Filter" && m_Tree.GetItemText( m_Tree.GetParentItem(Item)) == "Search")
		{
			Selected = m_Tree.GetItemText( m_Tree.GetParentItem(Item)) + " " + Selected;
			SetActivePage(m_TabSearchScreen);
		}
		if(Selected == "Block")
			SetActivePage(m_TabSearchBlock);
		if(Selected == "Share")
			SetActivePage(m_TabShare);
		if(Selected == "Transfer")
			SetActivePage(m_TabTransfer);
		if(Selected == "Bandwidth")
			SetActivePage(m_TabBandwidth);
		if(Selected == "Chat")
			SetActivePage(m_TabChat);
		if(Selected == "Plug-ins")
			SetActivePage(m_TabPlugins);

		// Set the title
		SetTitle((LPCTSTR)Selected, PSH_PROPTITLE);
	}

	
	// Move window around because of tree control and no tabs.
	CRect rectWnd;
	GetActivePage()->GetWindowRect(rectWnd);
	ScreenToClient(rectWnd);

	// Move over and up
	rectWnd.OffsetRect( PREF_TREE_MARGIN_LEFT + PREF_TREE_WIDTH + PREF_TREE_MARGIN_RIGHT,
		-PREF_TAB_OFFSET_UP);
	GetActivePage()->MoveWindow(rectWnd);

	// Save last selected
	HTREEITEM Selected = m_Tree.GetSelectedItem(),
			  Compare  = m_Tree.GetRootItem();
	
	int PrefPos = 1;
	while(Compare != Selected)
	{
		Compare = m_Tree.GetNextItem(Compare, TVGN_NEXTVISIBLE);
		PrefPos++;
	}
	
	m_pDoc->m_nLastPref = PrefPos;
}

// A unicode NT fix
afx_msg LRESULT CPrefDialog::OnNotifyFormat(WPARAM wParam,LPARAM lParam)
{
	#ifdef _UNICODE
		return NFR_UNICODE;
	#else
		return NFR_ANSI;
	#endif
}

afx_msg void CPrefDialog::OnApplyNow()
{
	Default();

	GetActivePage()->MoveWindow(m_rectPage);

	// Save config
	m_pDoc->m_pPrefsEx->SavePrefsEx(m_pDoc->m_RunPath + "Data\\GnuConfigEx.ini");

	m_pDoc->m_autPrefs->SaveConfig(m_pDoc->m_RunPath  + "Data\\GnuConfig.ini");
	m_pDoc->m_autPrefs->SaveBlocked(m_pDoc->m_RunPath  + "Data\\GnuBlocked.net");

	// Save node caches
	m_pDoc->m_autCache->SaveCache(m_pDoc->m_RunPath + "Data\\GnuCache.net");}
