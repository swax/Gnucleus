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
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "ChatControl.h"
#include "ChatRoom.h"
#include "ChatServer.h"
#include "ChatIdentd.h"
#include "ChatPrefs.h"

#include "ViewChat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewChat

IMPLEMENT_DYNCREATE(CViewChat, CFormView)

CViewChat::CViewChat()
	: CFormView(CViewChat::IDD)
{
	m_BackLogPos = 0;
	m_BackLog.push_back("");

	//{{AFX_DATA_INIT(CViewChat)
	//}}AFX_DATA_INIT
}

CViewChat::~CViewChat()
{
}

void CViewChat::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewChat)
	DDX_Control(pDX, IDC_BUTTON_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_LIST_USERS, m_lstUsers);
	DDX_Control(pDX, IDC_STATIC_CONNECT, m_stcConnect);
	DDX_Control(pDX, IDC_STATIC_CHANNEL, m_stcChannel);
	DDX_Control(pDX, IDC_STATIC_USER, m_stcUser);
	DDX_Control(pDX, IDC_TREE_CONNECTS, m_treeConnections);
	DDX_Control(pDX, IDC_RICHEDIT_CHAT, m_rebChat);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_ebInput);
	DDX_Control(pDX, IDC_BUTTON_CONFIGURE, m_btnConfigure);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewChat, CFormView)
	//{{AFX_MSG_MAP(CViewChat)
	ON_WM_DESTROY()
	ON_MESSAGE(CHAT_UPDATE, OnChatUpdate)
	ON_MESSAGE(CHAT_SELECT, OnChatSelect)
	ON_MESSAGE(CHAT_USER,   OnChatUser)
	ON_MESSAGE(CHAT_TOPIC,   OnChatTopic)
	ON_MESSAGE(CHAT_NOTICE,   OnChatNotice)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONNECTS, OnSelchangedTreeConnects)
	ON_BN_CLICKED(IDC_BUTTON_CONFIGURE, OnButtonConfigure)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CONNECTS, OnRclickTreeConnects)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USERS, OnRclickListUsers)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USERS, OnDblclkListUsers)
	ON_WM_CREATE()
	ON_COMMAND(ID_CHAT_CONNECT, OnChatConnect)
	ON_COMMAND(ID_CHAT_DISCONNECT, OnChatDisconnect)
	ON_COMMAND(ID_CHAT_HELP, OnChatHelp)
	ON_COMMAND(ID_CHAT_PREFS, OnChatPrefs)
	ON_UPDATE_COMMAND_UI(ID_CHAT_DISCONNECT, OnUpdateChatDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewChat diagnostics

#ifdef _DEBUG
void CViewChat::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewChat::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewChat message handlers

void CViewChat::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_pDoc  = (CGnucleusDoc*) GetDocument();
	m_pDoc->m_pViewChat = GetSafeHwnd();
	m_pChat = m_pDoc->m_pChat;

	m_rebChat.SetTargetDevice(NULL, 0);

	// Setup tree
	LoadTree();

	// Setup user list
	int offSet = m_lstUsers.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;
	CRect rect;
	m_lstUsers.GetWindowRect(&rect);
	m_lstUsers.InsertColumn( 0, "Users", LVCFMT_LEFT, (rect.Width() - offSet), 0);
	m_lstUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	
	
	// Resize stuff
	GetParentFrame()->GetClientRect(&rect);
//	OnSize(SIZE_RESTORED, rect.right - 4, rect.bottom - 4);
	PostMessage(WM_SIZE, SIZE_RESTORED, MAKELONG(rect.right - 4, rect.bottom - 4));
	
	// Try connecting again
	m_pChat->m_InetTested  = false;
	m_pChat->m_AutoConnect = true;

	// Join chat channel
	for(int i = 0; i < m_pChat->m_ServerList.size(); i++)
	{
		if(m_pChat->m_ServerList[i]->m_Status == SOCK_CONNECTED)
		{
			CString Join = "JOIN #gnutella/chat\r\n";
			m_pChat->m_ServerList[i]->Send(Join, Join.GetLength());

			if(m_pChat->m_pPrefs->m_PrefChannel != "")
			{
				Join = "JOIN #gnutella/chat/" + m_pChat->m_pPrefs->m_PrefChannel + "\r\n";
				m_pChat->m_ServerList[i]->Send(Join, Join.GetLength());
			}
		}
	}

	// Left Column
	m_DlgResizer.SizeItem(IDC_TREE_CONNECTS,  CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_CONFIGURE, CDlgResizer::Down);

	// Middle Column
	m_DlgResizer.SizeItem(IDC_STATIC_CHANNEL, CDlgResizer::Right);
	m_DlgResizer.SizeItem(IDC_RICHEDIT_CHAT, CDlgResizer::RightAndDown);
	m_DlgResizer.MoveSizeItem(IDC_EDIT_INPUT, CDlgResizer::Down, CDlgResizer::Right);

	// Right Column
	m_DlgResizer.MoveItem(IDC_STATIC_USER, CDlgResizer::Right);
	m_DlgResizer.MoveSizeItem(IDC_LIST_USERS, CDlgResizer::Right, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_HELP, CDlgResizer::RightAndDown);		

	m_DlgResizer.Done();
}

void CViewChat::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// Part chat channel
	for(int i = 0; i < m_pChat->m_ServerList.size(); i++)
		if(m_pChat->m_ServerList[i]->m_Status == SOCK_CONNECTED)
		{
			CString Part = "PART #gnutella/chat\r\n";
			m_pChat->m_ServerList[i]->Send(Part, Part.GetLength());

			if(m_pChat->m_pPrefs->m_PrefChannel != "")
			{
				Part = "PART #gnutella/chat/" + m_pChat->m_pPrefs->m_PrefChannel + "\r\n";
				m_pChat->m_ServerList[i]->Send(Part, Part.GetLength());
			}
		}

	m_pDoc->m_pViewChat = NULL;
}

BOOL CViewChat::PreTranslateMessage(MSG* pMsg) 
{
	CChatRoom* pRoom = NULL;
	HTREEITEM hItem = m_treeConnections.GetSelectedItem();
	
	if(hItem)
		pRoom = GetTreeData(hItem);

	switch (pMsg->message)
    {
    case WM_KEYDOWN:

		if(GetFocus() == GetDlgItem(IDC_EDIT_INPUT))
		{
			switch (pMsg->wParam)
			{
			case VK_UP:
				if(m_BackLogPos > 0)
				{
					m_ebInput.GetWindowText(m_BackLog[m_BackLogPos]);
					m_BackLogPos--;
					m_ebInput.SetWindowText( m_BackLog[m_BackLogPos] );
				}
				break;
			case VK_DOWN:
				if(m_BackLogPos < m_BackLog.size() - 1)
				{
					m_ebInput.GetWindowText(m_BackLog[m_BackLogPos]);
					m_BackLogPos++;
					m_ebInput.SetWindowText( m_BackLog[m_BackLogPos] );
				}
				break;
			case VK_RETURN:
				RelayCommand();
				break;
			}
		}

		if(pRoom)
			switch(pMsg->wParam)
			{
				case VK_F1:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f1", pRoom);
					break;
				case VK_F2:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f2", pRoom);
					break;
				case VK_F3:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f3", pRoom);
					break;
				case VK_F4:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f4", pRoom);
					break;
				case VK_F5:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f5", pRoom);
					break;
				case VK_F6:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f6", pRoom);
					break;
				case VK_F7:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f7", pRoom);
					break;
				case VK_F8:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f8", pRoom);
					break;
				case VK_F9:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f9", pRoom);
					break;
				case VK_F10:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f10", pRoom);
					break;
				case VK_F11:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f11", pRoom);
					break;
				case VK_F12:
					pRoom->m_pServer->AnalyzeAndSendCommand("/f12", pRoom);
					break;
			}

        break;
	}
    
	return CFormView::PreTranslateMessage(pMsg);
}

// probably should be process
void CViewChat::RelayCommand()
{	
	CString Input;

	// Get input
	m_ebInput.GetWindowText(Input);
	
	// Back logs so user can use arrow for prev. commands
	m_BackLog[m_BackLogPos] = Input;

	if(m_BackLogPos < m_BackLog.size() - 1)
	{
		int i = 0;
		std::vector<CString>::iterator itLine;
		for(itLine = m_BackLog.begin(); itLine != m_BackLog.end(); itLine++, i++)
			if(i == m_BackLogPos + 1)
			{
				m_BackLog.erase(itLine, m_BackLog.end());
				break;
			}
	}

	if(m_BackLog.size() > 200)
		m_BackLog.erase(m_BackLog.begin());
	else
		m_BackLogPos++;
	
	m_BackLog.push_back("");
	m_ebInput.SetWindowText("");	

	if( ClientCommand(Input))
		return;	


	HTREEITEM hItem = m_treeConnections.GetSelectedItem();
	if(hItem)
	{
		CChatRoom* pRoom = GetTreeData(hItem);
		if(!pRoom)
			return;
		
		// Check raw mode
		if(pRoom->m_RoomType == ROOM_RAW)
		{
			pRoom->m_pServer->Send(Input + "\r\n", Input.GetLength() + 2);
			return;
		}

		// Parse user input
		if( Input.GetLength() )
		{
			if(Input[0] == '/')
				pRoom->m_pServer->AnalyzeAndSendCommand(Input, pRoom);
				//pRoom->ServerCommand(Input);

			else
				pRoom->Say(Input);
		}
	}
}

bool CViewChat::ClientCommand(CString command)
{
	command.MakeLower();

	// Delete
	if( command.Find("/wtf?") == 0)
		for(int i = 0; i < m_pChat->m_ServerList.size(); i++)
		{
			CString Info = m_pChat->m_ServerList[i]->m_Host;
			Info += " - ";
			Info += DWrdtoStr(m_pChat->m_ServerList[i]->GnuUsers) + " Users";

			AfxMessageBox(Info);
		}

	// Server command
	if( command.Find("/server") == 0)
	{
		CString server;
		UINT	port = 6667;

		int pos = command.Find(" ") + 1;
		int mid = command.Find(":");
		
		if(mid == -1)
			server = command.Mid(pos);
		else
		{
			server = command.Mid(pos, mid - pos);
			port   = atoi( (LPCTSTR) command.Mid(mid + 1));
		}

		m_pChat->AddServer(server, port);
		
		return true;
	}

	if( command.Find("/disconnect") == 0)
	{
		HTREEITEM hItem = m_treeConnections.GetSelectedItem();
		if(hItem)
		{
			CChatRoom* pRoom = GetTreeData(hItem);
			if(!pRoom)
				return true;

			pRoom->m_pServer->Close("Disconnected");
		}
	}

	return false;
}

LRESULT CViewChat::OnChatUpdate(WPARAM pRoom, LPARAM lParam)
{
	if(!pRoom)
	{
		LoadTree();
		return NULL;
	}
	
	HTREEITEM Selected = m_treeConnections.GetNextItem(TVI_ROOT, TVGN_CARET);

	if(Selected && GetTreeData(Selected) == (CChatRoom*) pRoom)
	{
		int nScrollLimit = m_rebChat.GetScrollLimit(SB_VERT),
			nScrollPos   = m_rebChat.GetScrollPos(SB_VERT);

		int size = m_rebChat.GetTextLength();
		m_rebChat.SetSel(size, size);

		// Set color
		CHARFORMAT cf;
		cf.dwMask = CFM_COLOR | CFM_BOLD;
		cf.dwEffects = CFE_BOLD;
		cf.crTextColor = ((CChatRoom*) pRoom)->m_Pending.Color;
		cf.cbSize = sizeof(cf);
		m_rebChat.SetSelectionCharFormat(cf);

		// Insert text
		EDITSTREAM es;
		es.dwCookie = (DWORD) pRoom;
		es.pfnCallback = ChatStreamInCallback; 
		m_rebChat.StreamIn(SF_TEXT | SFF_SELECTION, es);

		if(nScrollPos >= nScrollLimit - 1)
			ScrollChattoBottom();
	}
	else
	{
		// Find item and put a * in front of it, symbolize a change
		HTREEITEM item = m_treeConnections.GetRootItem();

		while(item)
		{
			if( m_treeConnections.GetItemData(item) == pRoom)
			{
				CString store = m_treeConnections.GetItemText(item);

				if(store.Find("*") != 0)
					store = "* " + store;

				m_treeConnections.SetItemText(item, store);
			}

			item = m_treeConnections.GetNextVisibleItem(item);
		}
	}

	return NULL;
}

LRESULT CViewChat::OnChatSelect(WPARAM pRoom, LPARAM lParam)
{
	// Switch to view of room pointed to
	HTREEITEM item = m_treeConnections.GetRootItem();

	while(item)
	{
		if( GetTreeData(item) == (CChatRoom*) pRoom)
		{
			m_treeConnections.Select(item, TVGN_CARET);

			UpdateChat();
			UpdateUsers(NULL);
			UpdateTopic((CChatRoom*) pRoom);
		}

		item = m_treeConnections.GetNextVisibleItem(item);
	}

	return NULL;
}


LRESULT CViewChat::OnChatUser(WPARAM pRoom, LPARAM pUser)
{
	HTREEITEM Selected = m_treeConnections.GetNextItem(TVI_ROOT, TVGN_CARET);;
	

	if( Selected && m_treeConnections.GetItemData(Selected) == pRoom)
		UpdateUsers((CString*) pUser);

	return NULL;
}

LRESULT CViewChat::OnChatTopic(WPARAM pRoom, LPARAM lParam)
{
	HTREEITEM Selected = m_treeConnections.GetNextItem(TVI_ROOT, TVGN_CARET);;
	
	if( Selected && m_treeConnections.GetItemData(Selected) == pRoom)
		UpdateTopic( (CChatRoom*) pRoom);


	return NULL;
}

LRESULT CViewChat::OnChatNotice(WPARAM pRoom, LPARAM pChatLine)
{
	HTREEITEM Selected = m_treeConnections.GetNextItem(TVI_ROOT, TVGN_CARET);;
	
	if(!Selected)
		return NULL;
	
	CChatRoom* pModified = (CChatRoom*) pRoom;
	CChatRoom* pCurrent  = GetTreeData(Selected);
	if(!pCurrent)
		return NULL;	

	if(pCurrent->m_RoomType != ROOM_SERVER)
		if(pModified->m_pServer == pCurrent->m_pServer)
		{
			pModified->m_Pending.Text.Remove('\n');
			pModified->m_Pending.Text += "\n";

			pCurrent->WriteToDisplay(pModified->m_Pending);
		}
			

	// If current view is connected to same server, and not the server

	// Write to current display the string

	return NULL;
}

static DWORD CALLBACK 
ChatStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString* pstr = &((CChatRoom*) dwCookie)->m_Pending.Text;

	// Thank you, Developer.com
	if( pstr->GetLength() < cb )
	{
		*pcb = pstr->GetLength();
		memcpy(pbBuff, (LPCSTR)*pstr, *pcb );
		pstr->Empty();
	}
	else
	{
		*pcb = cb;
		memcpy(pbBuff, (LPCSTR)*pstr, *pcb );
		*pstr = pstr->Right( pstr->GetLength() - cb );
	}

	return 0;
}


void CViewChat::LoadTree()
{
	m_treeConnections.SetRedraw(false);

	int ConnectNum = 0;

	HTREEITEM Item, Parent;
	HTREEITEM Selected = m_treeConnections.GetSelectedItem();

	// Reset Content
	m_treeConnections.DeleteAllItems();
	m_rebChat.SetWindowText("");
	m_lstUsers.DeleteAllItems();

	m_stcChannel.SetWindowText("");
	m_stcUser.SetWindowText("0 Users");

	// Load tree
	for(int i = 0; i < m_pChat->m_ServerList.size(); i++)
	{
		ConnectNum++;

		// Add server as parent node
		CChatServer* p = m_pChat->m_ServerList[i];
	
		Item = m_treeConnections.InsertItem(p->m_Host, NULL);
		m_treeConnections.SetItemData(Item, (DWORD) p->m_Console);

		Parent = Item;

		// Add rooms below it
		for(int j = 0; j < p->m_RoomList.size(); j++)
		{
			Item = m_treeConnections.InsertItem(p->m_RoomList[j]->m_Name, Parent);
			m_treeConnections.SetItemData(Item, (DWORD) p->m_RoomList[j]);
		}

		// Add private messages
		for(j = 0; j < p->m_PrivateList.size(); j++)
		{
			Item = m_treeConnections.InsertItem(p->m_PrivateList[j]->m_Name, Parent);
			m_treeConnections.SetItemData(Item, (DWORD) p->m_PrivateList[j]);
		}

		m_treeConnections.Expand(Parent, TVE_EXPAND);
	}

	m_treeConnections.SetRedraw(true);
	
	CString plural = (ConnectNum == 1) ? " Connection" : " Connections";
	m_stcConnect.SetWindowText( DWrdtoStr(ConnectNum) + plural);

	if(!Selected)
		Selected = m_treeConnections.GetRootItem();
	m_treeConnections.Select(Selected, TVGN_CARET);

	UpdateChat();
}

void CViewChat::OnSelchangedTreeConnects(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// If astrik set take it back
	HTREEITEM Selected = m_treeConnections.GetSelectedItem();

	CString store = m_treeConnections.GetItemText(Selected);
	
	if(store.Find("* ") == 0)
		store = store.Mid(2);

	UpdateChat();
	UpdateUsers(NULL);

	// Set title
	m_treeConnections.SetItemText(Selected, store);
	UpdateTopic(GetTreeData(Selected));

	*pResult = 0;
}

void CViewChat::UpdateChat()
{
	HTREEITEM hItem = m_treeConnections.GetSelectedItem();

	if(!hItem)
		return;

	CChatRoom* pRoom = GetTreeData(hItem);
		if(!pRoom)
			return;

	int size = 0;

	m_rebChat.SetRedraw(false);

	m_rebChat.SetWindowText("");

	// Insert text into box
	std::list<ChatLine>::iterator itLine;
	for(itLine = pRoom->m_Display.begin(); itLine != pRoom->m_Display.end(); itLine++)
	{
		pRoom->m_Pending = *itLine;

		size = m_rebChat.GetTextLength();
		m_rebChat.SetSel(size, size);

		// Set color
		CHARFORMAT cf;
		cf.dwMask = CFM_COLOR | CFM_BOLD;
		cf.dwEffects = CFE_BOLD;
		cf.crTextColor = ((CChatRoom*) pRoom)->m_Pending.Color;
		cf.cbSize = sizeof(cf);
		m_rebChat.SetSelectionCharFormat(cf);

		// Insert text
		EDITSTREAM es;
		es.dwCookie = (DWORD) pRoom;
		es.pfnCallback = ChatStreamInCallback; 
		m_rebChat.StreamIn(SF_TEXT | SFF_SELECTION, es);
	}

	ScrollChattoBottom();

	m_rebChat.SetRedraw(true);
	m_rebChat.InvalidateRect(NULL);
}

void CViewChat::UpdateUsers(CString* pUser)
{
	HTREEITEM hItem = m_treeConnections.GetSelectedItem();

	if(!hItem)
		return;

	CChatRoom* pRoom = GetTreeData(hItem);
		if(!pRoom)
			return;

	if(!pRoom)
		return;

	// Update user list
	int pos = -1;
	CString plural;

	if(pUser)
	{
		char mod = pUser->GetAt(0);

		switch(mod)
		{
		case '+':
			m_lstUsers.InsertItem(0, pUser->Mid(1) );

			plural = (pRoom->m_UserList.size() == 1) ? " User" : " Users";
			m_stcUser.SetWindowText( DWrdtoStr(pRoom->m_UserList.size()) + plural);
	
			break;
		case '-':
			pos = pRoom->FindUser( pUser->Mid(1));

			if(pos != -1)
			{
				LVFINDINFO info;
				info.flags = LVFI_PARTIAL|LVFI_STRING;
				info.psz = pRoom->m_UserList[pos];

				int index = m_lstUsers.FindItem(&info);
				if(index != -1)
					m_lstUsers.DeleteItem(index);
			}

			plural = (pRoom->m_UserList.size() == 1) ? " User" : " Users";
			m_stcUser.SetWindowText( DWrdtoStr(pRoom->m_UserList.size() - 1) + plural);
		
			break;
		case '*':
			int mid = pUser->Find('/');
			
			CString OldName = pUser->Mid(1, mid - 1),
					NewName = pUser->Mid(mid + 1);

			pos		= pRoom->FindUser(OldName);

			if(pos != -1)
			{
				OldName = pRoom->m_UserList[pos];

				LVFINDINFO info;
				info.flags = LVFI_PARTIAL|LVFI_STRING;
				info.psz = OldName;

				int index = m_lstUsers.FindItem(&info);
				if(index != -1)
					m_lstUsers.DeleteItem(index);
			}
			
			if(OldName.Find('@') == 0)
				NewName = '@' + NewName;
			if(OldName.Find('+') == 0)
				NewName = '+' + NewName;

			m_lstUsers.InsertItem(0, NewName);

			break;
		}
	}

	// Reload  user list
	else
	{
		m_lstUsers.DeleteAllItems();

		for(int i = 0; i < pRoom->m_UserList.size(); i++)
			m_lstUsers.InsertItem(0, pRoom->m_UserList[i] );

		CString plural = (pRoom->m_UserList.size() == 1) ? " User" : " Users";
		m_stcUser.SetWindowText( DWrdtoStr(pRoom->m_UserList.size()) + plural);
	}
}


void CViewChat::UpdateTopic(CChatRoom* pRoom)
{
	if(pRoom)
	{
		CString Topic = pRoom->m_Name;

		if(pRoom->m_Topic != "")
			Topic = Topic + " - " + pRoom->m_Topic;

		m_stcChannel.SetWindowText(Topic);
	}
}


void CViewChat::ScrollChattoBottom()
{
	int nLimit = m_rebChat.GetScrollLimit(SB_VERT);
	
	m_rebChat.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, nLimit), NULL);
	m_rebChat.SetScrollPos(SB_VERT, nLimit);
}

void CViewChat::OnButtonConfigure() 
{
	// Dispaly Preferences, transfer tab
	m_pDoc->m_nLastPref = PREF_CHAT;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);

}



void CViewChat::OnRclickTreeConnects(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint point;
    GetCursorPos(&point);

	CPoint ClientPoint = point;
	m_treeConnections.ScreenToClient(&ClientPoint);

	HTREEITEM nItem = m_treeConnections.HitTest(ClientPoint);

	if (!nItem)
		return;


    CMenu menu;
    menu.LoadMenu(IDR_CHAT_CONNECT_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);
	
	switch(res)
	{
	case ID_CHAT_LSTCONNECTED_CLOSE:

		CChatRoom* pRoom = GetTreeData(nItem);
		if(!pRoom)
			return;

		switch(pRoom->m_RoomType)
		{
		case ROOM_SERVER:
			m_pChat->RemoveServer(pRoom->m_pServer);
			break;
		case ROOM_CHANNEL:
			pRoom->ServerCommand("/PART " + pRoom->m_Name);
			pRoom->m_pServer->RemoveRoom(pRoom);
			break;
		case ROOM_PRIVATE:
			pRoom->m_pServer->RemoveRoom(pRoom);
			break;
		}
		break;
	}

	*pResult = 0;
}

void CViewChat::OnDblclkListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{

	HTREEITEM nItem = m_treeConnections.GetSelectedItem();
	
	if(!nItem)
		return;

	CChatRoom* pSelRoom = GetTreeData(nItem);
	if(!pSelRoom)
		return;

	CChatServer* pServer = pSelRoom->m_pServer;
	
	int nSel = m_lstUsers.GetSelectionMark();
	if(nSel < 0 || nSel >= m_lstUsers.GetItemCount())
		return;

	CString User = GetUserFromList(nSel);

	// Create Room
	CChatRoom* pRoom = pServer->GetPrivate(User);

	if(!pRoom)
	{
		CChatRoom* pNewRoom  = new CChatRoom(pServer);
		pNewRoom->m_Name     = User;
		pNewRoom->m_RoomType = ROOM_PRIVATE;
		pServer->m_PrivateList.push_back(pNewRoom);

		pNewRoom->AddUser(pServer->m_Nick);
		pNewRoom->AddUser(User);

		pRoom = pNewRoom;
	}
	
	m_pChat->ChatMessage(CHAT_UPDATE, NULL);
	m_pChat->ChatMessage(CHAT_SELECT, (DWORD) pRoom);

	*pResult = 0;
}

void CViewChat::OnRclickListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	HTREEITEM nTreeItem = m_treeConnections.GetSelectedItem();
	if(!nTreeItem)
		return;

	int nUserItem = m_lstUsers.GetSelectionMark();
	if (nUserItem < 0 || nUserItem >= m_lstUsers.GetItemCount())
		return;

    CMenu menu;
    menu.LoadMenu(IDR_CHAT_USERS_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstUsers.GetFirstSelectedItemPosition();

	CChatRoom* pSelRoom = GetTreeData(nTreeItem);
	if(!pSelRoom)
		return;

	CChatServer* pServer = pSelRoom->m_pServer;
	CChatRoom*   pRoom   = NULL;

	CString		 User;
	int			 nItem;

	switch(res)
	{
	case ID_CHAT_WHOIS:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			pRoom = GetTreeData(nTreeItem);
			if(pRoom)
				pRoom->ServerCommand("/WHOIS " + GetUserFromList(nItem));
		}
		break;	

	case ID_CHAT_PRIVATE:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			User = GetUserFromList(nItem);

			// Create Room
			pRoom = pServer->GetPrivate(User);

			if(!pRoom)
			{
				CChatRoom* pNewRoom  = new CChatRoom(pServer);
				pNewRoom->m_Name     = User;
				pNewRoom->m_RoomType = ROOM_PRIVATE;
				pServer->m_PrivateList.push_back(pNewRoom);

				pNewRoom->AddUser(pServer->m_Nick);
				pNewRoom->AddUser(User);

				pRoom = pNewRoom;
			}
			
			m_pChat->ChatMessage(CHAT_UPDATE, NULL);
			m_pChat->ChatMessage(CHAT_SELECT, (DWORD) pRoom);
		}
		break;

	case ID_CHAT_VERSION:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			pRoom = GetTreeData(nTreeItem);
			if(pRoom)
				pRoom->ServerCommand("/PRIVMSG " + GetUserFromList(nItem) + " :" + (char) 0x01 + "VERSION" + (char) 0x01);
		}
		break;

	case ID_CHAT_CONTROL_OP:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			pRoom = GetTreeData(nTreeItem);
			if(pRoom)
				pRoom->ServerCommand("/MODE " + pRoom->m_Name + " +o " + GetUserFromList(nItem));
		}
		break;

	case ID_CHAT_CONTROL_DEOP:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			pRoom = GetTreeData(nTreeItem);
			if(pRoom)
				pRoom->ServerCommand("/MODE " + pRoom->m_Name + " -o " + GetUserFromList(nItem));
		}
		break;

	case ID_CHAT_CONTROL_KICK:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			pRoom = GetTreeData(nTreeItem);
			if(pRoom)
				pRoom->ServerCommand("/KICK " + pRoom->m_Name + " " + GetUserFromList(nItem));
		}
		break;

	case ID_CHAT_CONTROL_BAN:
		while(pos)
		{
			nItem = m_lstUsers.GetNextSelectedItem(pos);
			if (nItem < 0 || nItem >= m_lstUsers.GetItemCount())
				return;

			pRoom = GetTreeData(nTreeItem);
			if(pRoom)
				pRoom->ServerCommand("/MODE " + pRoom->m_Name + " +b " + GetUserFromList(nItem));
		}
		break;
	}
	*pResult = 0;
}

CString CViewChat::GetUserFromList(int index)
{
	if(index < 0 || index >= m_lstUsers.GetItemCount())
		return "";

	CString User = m_lstUsers.GetItemText(index, 0);
	User.Remove('@');
	User.Remove('+');

	return User;
}

CChatRoom* CViewChat::GetTreeData(HTREEITEM item)
{
	CChatRoom* pTestRoom = (CChatRoom*) m_treeConnections.GetItemData(item);

	for(int i = 0; i < m_pChat->m_ServerList.size(); i++)
	{
		CChatServer* p = m_pChat->m_ServerList[i];

		if(pTestRoom == p->m_Console)
			return p->m_Console;
		
		for(int j = 0; j < p->m_RoomList.size(); j++)
			if(pTestRoom == p->m_RoomList[j])
				return p->m_RoomList[j];

		for(j = 0; j < p->m_PrivateList.size(); j++)
			if(pTestRoom == p->m_PrivateList[j])
				return p->m_PrivateList[j];
	}
	
	return NULL;
}

void CViewChat::OnButtonHelp() 
{
	ShellExecute(NULL, "open", m_pDoc->m_RunPath + "Docs/Chat.txt", NULL, NULL, SW_SHOWNORMAL);		
}


int CViewChat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	if (!m_wndToolBar.CreateEx(this->GetParentFrame()) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_CHAT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	m_DlgResizer.InitResizer(this);
	
	return 0;
}

void CViewChat::OnChatConnect() 
{
	m_pChat->m_AutoConnect = true;
}

void CViewChat::OnChatDisconnect() 
{
	m_pChat->m_AutoConnect = false;

	while(m_pChat->m_ServerList.size())
		m_pChat->RemoveServer(m_pChat->m_ServerList[0]);

	while(m_pChat->m_IdentdList.size())
	{
		delete m_pChat->m_IdentdList.back();
		m_pChat->m_IdentdList.pop_back();
	}
}

void CViewChat::OnUpdateChatDisconnect(CCmdUI* pCmdUI) 
{
	if(m_pChat->m_AutoConnect)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CViewChat::OnChatPrefs() 
{
	m_pDoc->m_nLastPref = PREF_CHAT;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);
}

/*
void CViewChat::OnChatColors() 
{
	
}*/


void CViewChat::OnChatHelp() 
{
	ShellExecute(NULL, "open", m_pDoc->m_RunPath + "Docs/Chat.txt", NULL, NULL, SW_SHOWNORMAL);			
}



/*
void CViewChat::OnChatAlias() 
{
	// TODO: Add your command handler code here
	
}

void CViewChat::OnChatRemote() 
{
	// TODO: Add your command handler code here
	
}
*/

BOOL CViewChat::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CFormView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}
