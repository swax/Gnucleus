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
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "ChatControl.h"
#include "ChatServer.h"
#include "ChatPrefs.h"

#include "PrefChat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefChat, CPropertyPage)

CPrefChat::CPrefChat() : CPropertyPage(CPrefChat::IDD)
{
	m_pDoc      = NULL;
	m_chatPrefs = NULL;

	//{{AFX_DATA_INIT(CPrefChat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPrefChat::~CPrefChat()
{
}

void CPrefChat::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefChat)
	DDX_Control(pDX, IDC_STATIC_SERVERS, m_stcServers);
	DDX_Control(pDX, IDC_LIST_SERVERS, m_lstServers);
	DDX_Control(pDX, IDC_EDIT_SERVER, m_ebServer);
	DDX_Control(pDX, IDC_EDIT_PREFCHAN, m_ebPrefChannel);
	DDX_Control(pDX, IDC_COMBO_NETWORK, m_cmbNetwork);
	DDX_Control(pDX, IDC_CHECK_PREFCHAN, m_chkPrefChannel);
	DDX_Control(pDX, IDC_CHECK_DEFNETWORK, m_chkPrefNet);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_EDIT_NICK, m_ebNick);
	DDX_Control(pDX, IDC_EDIT_ALTNICK, m_ebAltNick);
	DDX_Control(pDX, IDC_CHECK_IDENT_DISABLE, m_chkIdentDisable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefChat, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefChat)
	ON_EN_CHANGE(IDC_EDIT_NICK, OnChangeEditNick)
	ON_EN_CHANGE(IDC_EDIT_ALTNICK, OnChangeEditAltnick)
	ON_BN_CLICKED(IDC_CHECK_PREFCHAN, OnCheckPrefchan)
	ON_EN_CHANGE(IDC_EDIT_PREFCHAN, OnChangeEditPrefchan)
	ON_CBN_SELCHANGE(IDC_COMBO_NETWORK, OnSelchangeComboNetwork)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_CHECK_DEFNETWORK, OnCheckDefnetwork)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVERS, OnRclickListServers)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_IDENT_DISABLE, OnBnClickedCheckIdentDisable)
END_MESSAGE_MAP()


BOOL CPrefChat::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc      = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_chatPrefs = m_pDoc->m_pChat->m_pPrefs;
	
	
	// Nicknames
	m_ebNick.SetWindowText(m_chatPrefs->m_Nick);
	m_ebAltNick.SetWindowText(m_chatPrefs->m_AltNick);

	// Load combo box
	for(int i = 0; i < m_chatPrefs->m_ChatNets.size(); i++)
		m_cmbNetwork.InsertString(0, m_chatPrefs->m_ChatNets[i].Name);

	m_cmbNetwork.SelectString(-1, m_chatPrefs->m_PrefNet);

	// Server listbox
	int offSet = m_lstServers.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;
	CRect rect;
	m_lstServers.GetWindowRect(&rect);
	m_lstServers.InsertColumn( 0, "Server", LVCFMT_LEFT, (rect.Width() - offSet), 0);
	m_lstServers.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	OnSelchangeComboNetwork();

	// Preferred Channel
	if(m_chatPrefs->m_PrefChannel == "")
		m_ebPrefChannel.EnableWindow(false);
	else
	{
		m_chkPrefChannel.SetCheck(true);
		m_ebPrefChannel.SetWindowText(m_chatPrefs->m_PrefChannel);
	}

	if(m_chatPrefs->m_DisableIdent)
		m_chkIdentDisable.SetCheck(1);

	return TRUE;  
}

void CPrefChat::OnChangeEditNick() 
{
	SetModified();
}

void CPrefChat::OnChangeEditAltnick() 
{
	SetModified();
}

void CPrefChat::OnSelchangeComboNetwork() 
{
	int nIndex = m_cmbNetwork.GetCurSel();

	if (nIndex != LB_ERR)
	{
		CString SelNetwork;
		m_cmbNetwork.GetLBText(nIndex, SelNetwork);

		m_lstServers.DeleteAllItems();

		for(int i = 0; i < m_chatPrefs->m_ChatNets.size(); i++)		
			if(m_chatPrefs->m_ChatNets[i].Name == SelNetwork)
			{
				int Count = 0;
				
				for(int j = 0; j < m_chatPrefs->m_ChatNets[i].Servers.size(); j++)
				{
					m_lstServers.InsertItem(0, m_chatPrefs->m_ChatNets[i].Servers[j]);
					
					Count++;
				}

				m_stcServers.SetWindowText( DWrdtoStr(Count) + " Servers");
			}

		if(SelNetwork == m_chatPrefs->m_PrefNet)
			m_chkPrefNet.SetCheck(true);
		else
			m_chkPrefNet.SetCheck(false);
	}
}

void CPrefChat::OnCheckDefnetwork() 
{
	int nIndex = m_cmbNetwork.GetCurSel();

	if (nIndex != LB_ERR)
	{
		CString SelNetwork;
		m_cmbNetwork.GetLBText(nIndex, SelNetwork);

		m_chatPrefs->m_PrefNet = SelNetwork;

		m_chkPrefNet.SetCheck(true);
	}
}

void CPrefChat::OnCheckPrefchan() 
{
	if(m_chkPrefChannel.GetCheck())
		m_ebPrefChannel.EnableWindow(true);
	else
		m_ebPrefChannel.EnableWindow(false);

	SetModified();
}

void CPrefChat::OnChangeEditPrefchan() 
{
	SetModified();
}

void CPrefChat::OnButtonAdd() 
{
	int nIndex = m_cmbNetwork.GetCurSel();

	if (nIndex != LB_ERR)
	{
		CString SelNetwork;
		m_cmbNetwork.GetLBText(nIndex, SelNetwork);

		CString NewServer;

		m_ebServer.GetWindowText(NewServer);

		if(NewServer != "")
		{
			if(NewServer.Find(":") == -1)
			{
				AfxMessageBox("Please include a port with ':' following the server name");
				return;
			}

			for(int i = 0; i < m_chatPrefs->m_ChatNets.size(); i++)		
				if(m_chatPrefs->m_ChatNets[i].Name == SelNetwork)
					m_chatPrefs->m_ChatNets[i].Servers.push_back(NewServer);
		}
	
		m_ebServer.SetWindowText("");
		OnSelchangeComboNetwork();
	}

	SetModified();
}

void CPrefChat::OnRclickListServers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstServers.GetSelectionMark();
	if (0 > nItem || nItem > m_lstServers.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_PREFCHAT_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD), point.x, point.y, this);

	POSITION pos = m_lstServers.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_PREFCHAT_REMOVE:
		// Remove servers from pref list
		int nIndex = m_cmbNetwork.GetCurSel();

		if (nIndex != LB_ERR)
		{
			CString SelNetwork;
			m_cmbNetwork.GetLBText(nIndex, SelNetwork);

			for(int i = 0; i < m_chatPrefs->m_ChatNets.size(); i++)		
				if(m_chatPrefs->m_ChatNets[i].Name == SelNetwork)
					while(pos)
					{		
						nItem = m_lstServers.GetNextSelectedItem(pos);

						CString DelServer = m_lstServers.GetItemText(nItem, 0);

						std::vector<CString>::iterator itServer;
						for(itServer = m_chatPrefs->m_ChatNets[i].Servers.begin(); itServer != m_chatPrefs->m_ChatNets[i].Servers.end(); itServer++)
							if(DelServer == (*itServer))
							{
								m_chatPrefs->m_ChatNets[i].Servers.erase(itServer);
								break;
							}
				
					}			
		}

		// Reload lists
		OnSelchangeComboNetwork();
		break;
	}


	*pResult = 0;
}

void CPrefChat::OnBnClickedCheckIdentDisable()
{
	SetModified();
}

BOOL CPrefChat::OnApply() 
{
	CString store;

	// Nick settings
	m_ebNick.GetWindowText(store);
	if(store != "")
	{	
		if(store != m_chatPrefs->m_Nick)
			for(int i = 0; i < m_pDoc->m_pChat->m_ServerList.size(); i++)	
				if(m_pDoc->m_pChat->m_ServerList[i]->m_Status == SOCK_CONNECTED)
				{
					CString Command = "NICK " + store + "\r\n";
					m_pDoc->m_pChat->m_ServerList[i]->Send(Command, Command.GetLength());
					m_pDoc->m_pChat->m_ServerList[i]->m_Nick = store;
				}

		m_chatPrefs->m_Nick = store;
	}

	m_ebAltNick.GetWindowText(store);	
	if(store != "")
		m_chatPrefs->m_AltNick = store;


	// Preferred channel
	if(m_chkPrefChannel.GetCheck())
	{
		m_ebPrefChannel.GetWindowText(store);

		if(store == "")
			m_chatPrefs->m_PrefChannel = "";
		else
			m_chatPrefs->m_PrefChannel = store;
	}
		
	else
		m_chatPrefs->m_PrefChannel = "";

	// Ident server
	if(m_chkIdentDisable.GetCheck())
		m_chatPrefs->m_DisableIdent = true;
	else
		m_chatPrefs->m_DisableIdent = false;

	
	return CPropertyPage::OnApply();
}






