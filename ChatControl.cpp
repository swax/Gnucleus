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

#include "AutPrefs.h"
#include "AutNetwork.h"

#include "ChatServer.h"
#include "ChatRoom.h"
#include "ChatIdentd.h"
#include "ChatPrefs.h"

#include "ChatControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT TestInetThread(LPVOID pVoidChat);


CChatControl::CChatControl(CGnucleusDoc* pDoc)
{
	m_pDoc        = pDoc;
	m_autNetwork  = pDoc->m_autNetwork;
	m_autPrefs    = pDoc->m_autPrefs;

	m_pPrefs   = new CChatPrefs(this);

	m_IdentdStatus = SOCK_CLOSED;

	m_InetTested  = false;
	m_AutoConnect = false;
	
	m_TryUltraCache  = 0;
	m_UltraCacheMode = false;

	m_SecsRequest = 0;

	m_NickAlert = false;

	// Load chat prefs
	m_pPrefs->LoadServers(m_pDoc->m_RunPath + "ChatServers.ini");
	m_pPrefs->LoadChatAliases(m_pDoc->m_RunPath + "ChatAliases.ini");
	m_pPrefs->LoadChatScripts(m_pDoc->m_RunPath + "ChatScripts.ini");
	m_pPrefs->LoadChatHotlist(m_pDoc->m_RunPath + "ChatHotlist.ini");
}

CChatControl::~CChatControl()
{	
	// Save chat prefs
	m_pPrefs->SaveServers(m_pDoc->m_RunPath  + "ChatServers.ini");
	m_pPrefs->SaveChatAliases(m_pDoc->m_RunPath + "ChatAliases.ini");
	m_pPrefs->SaveChatScripts(m_pDoc->m_RunPath + "ChatScripts.ini");
	m_pPrefs->SaveChatHotlist(m_pDoc->m_RunPath + "ChatHotlist.ini");

	delete m_pPrefs;
	m_pPrefs = NULL;


	if(m_hSocket != INVALID_SOCKET)
		AsyncSelect(0);

	TRACE0("*** CChatControl Deconstructing\n");

	for(int i = 0; i < m_ServerList.size(); i++)
	{
		delete m_ServerList[i];
		m_ServerList[i] = NULL;
	}

	while(m_IdentdList.size())
	{
		delete m_IdentdList.back();
		m_IdentdList.pop_back();
	}

	m_ServerList.clear();
}


#if 0
BEGIN_MESSAGE_MAP(CChatControl, CAsyncSocket)
	//{{AFX_MSG_MAP(CChatControl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


bool CChatControl::StartIdentd()
{
	if(m_IdentdStatus == SOCK_CONNECTED)
		return true;
			
	if(Create(113))
	{
		if(Listen())
		{
			m_IdentdStatus = SOCK_CONNECTED;
			return true;
		}
		//else
		//	m_pDoc->LogError("Identd Listen Error: " + DWrdtoStr(GetLastError()));
	}
	//else
	//	m_pDoc->LogError("Identd Create Error: " + DWrdtoStr(GetLastError()));

	return false;
}

void CChatControl::StopIdentd()
{
	Close();

	while(m_IdentdList.size())
	{
		delete m_IdentdList.back();
		m_IdentdList.pop_back();
	}

	m_IdentdStatus = SOCK_CLOSED;
}

void CChatControl::GetConnect()
{
	// Test for internet connection 
	if(!m_autPrefs->GetLanMode() && !m_InetTested)
	{
		CWinThread* pThread = AfxBeginThread(TestInetThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		AssignThreadToCPU(pThread, CPU_0);
		pThread->ResumeThread();

		return;
	}

	if(m_autPrefs->GetLanMode())
	{
		if(m_pPrefs->m_InternalIRC)
		{
			CString Host = m_pPrefs->m_InternalIRCAddr;

			int pos = Host.Find(":");

			if(pos != -1)
				AddServer( Host.Left(Host.Find(":")), atoi(Host.Mid(Host.Find(":") + 1)));
			else
				AddServer(m_pPrefs->m_InternalIRCAddr, 6667);
		}

		return;
	}

	// If its for chat
	if(m_pDoc->m_pViewChat)
		if(m_pPrefs->m_PrefNet == "GnuDefault")
		{
			AddServer("irc.openprojects.net", 6667);
			return;
		}
			

	// Connect to random server on default network
	for(int i = 0; i < m_pPrefs->m_ChatNets.size(); i++)		
		if(m_pPrefs->m_ChatNets[i].Name == m_pPrefs->m_PrefNet)
			if(m_pPrefs->m_ChatNets[i].Servers.size())
			{
				int servpos = rand() % m_pPrefs->m_ChatNets[i].Servers.size() + 0;
				CString Parse = m_pPrefs->m_ChatNets[i].Servers[servpos];
				
				// Take first server from list, and move it to the back
				//pPrefs->m_ChatNets[i].Servers.erase( pPrefs->m_ChatNets[i].Servers.begin() );
				//pPrefs->m_ChatNets[i].Servers.push_back(Parse);
		
				int colon = Parse.Find( _T(":") );

				if(colon != -1)
				{
					std::vector<UINT> PortList;

					CString Server   = Parse.Left(colon);
					CString PortString = Parse.Mid(colon + 1) + ",";
					
					int pos = 0;
					while(PortString.Find( _T(","), pos) != -1)
					{
						UINT port = atoi(PortString.Mid(pos, PortString.Find( _T(","), pos) - pos));
						PortList.push_back(port);

						pos = PortString.Find( _T(","), pos) + 1;
					}

					if(PortList.size())
					{	
						int portpos = rand() % PortList.size() + 0;
						AddServer(Server, PortList[portpos]);

						return;
					}
				}
			}
}

UINT TestInetThread(LPVOID pVoidChat)
{	
	CChatControl* pChat  = (CChatControl*) pVoidChat;
	CChatPrefs*  pPrefs =  pChat->m_pPrefs;

	TRACE0("*** Internet Test Thread Started\n");

	srand( (unsigned)time(NULL) );
	bool Found = false;

	// Get Server from preferred network to test
	for(int i = 0; i < pPrefs->m_ChatNets.size(); i++)		
		if(pPrefs->m_ChatNets[i].Name == pPrefs->m_PrefNet)
		{
			Found = true;
			int nPos = rand() % pPrefs->m_ChatNets[i].Servers.size() + 0;
		
			CString Server = pPrefs->m_ChatNets[i].Servers[nPos];
			int colon = Server.Find( _T(":"));

			if(colon != -1)
				Server = Server.Left(colon);

			LPHOSTENT lphost;
			lphost = gethostbyname(Server);
			if (!lphost)
				pChat->m_AutoConnect = false;
		}

	if(!Found)
		pChat->m_AutoConnect = false;

	pChat->m_InetTested = true;

	TRACE0("*** Internet Test Thread Ended\n");
	return 0;
}

void CChatControl::AddServer(CString Host, UINT Port)
{
	// If server already exists
	for(int i = 0; i < m_ServerList.size(); i++)
	{
		CChatServer* pServer = m_ServerList[i];

		if(pServer->m_Host == Host)
		{
			if(pServer->m_Status == SOCK_CLOSED)
			{
				if(pServer->m_hSocket == INVALID_SOCKET)
					pServer->Create();
						//m_pDoc->LogError("Chat Create Error: " + DWrdtoStr(pServer->GetLastError()));
				
				pServer->Connect(Host, Port);
			}

			ChatMessage(CHAT_UPDATE, NULL);
			ChatMessage(CHAT_SELECT, (DWORD) pServer->m_Console);
		
			return;
		}
	}

	if(!m_pPrefs->m_DisableIdent)
		StartIdentd();
	
	CChatServer* Server = new CChatServer(this, Host, Port);

	//Attempt to connect to node
	Server->Create();
		//m_pDoc->LogError("Chat Create Error: " + DWrdtoStr(Server->GetLastError()));

	if( !Server->Connect(Host, Port) )
		if (Server->GetLastError() != WSAEWOULDBLOCK)
		{
			delete Server;
			return;
		}

	// Add node to list
	
	m_ServerList.push_back(Server);


	ChatMessage(CHAT_UPDATE, NULL);
}

void CChatControl::RemoveServer(CChatServer* pServer)
{	
	if(pServer)
	{
		std::vector<CChatServer*>::iterator itServer;
		for(itServer = m_ServerList.begin(); itServer != m_ServerList.end(); itServer++)
			if(*itServer == pServer)
			{
				delete *itServer;
				m_ServerList.erase(itServer);
				
				itServer--;
			}

		// First clear channel, then select it
		ChatMessage(CHAT_UPDATE, NULL);
		ChatMessage(CHAT_UPDATE, NULL);
	}
}

bool CChatControl::SendRequest()
{
	bool Sent = false;

	if(m_autNetwork->GetLocalIP() == StrtoIP("127.0.0.1").S_addr)
		return false;

	if(m_autNetwork->GetLocalPort() == 0)
		return false;

	for(int i = 0; i < m_ServerList.size(); i++)
		if(m_ServerList[i]->m_Status == SOCK_CONNECTED)
		{
			bool CacheFound = false;
			
			for(int j = 0; j < m_ServerList[i]->m_RoomList.size(); j++)
			{
				CString Channel = m_ServerList[i]->m_RoomList[j]->m_Name;

				if(Channel == "#gnutella/cache." + DWrdtoStr(m_ServerList[i]->m_CacheNum) ||
				   Channel == "#gnutella/cache/" + m_pPrefs->m_PrefChannel)
				{
					IP   Host;
					Host.S_addr = m_autNetwork->GetLocalIP();
					UINT Port   = m_autNetwork->GetLocalPort();

					CString Notify = "NOTICE " + Channel + " :open(";
					Notify += IPtoStr(Host) + ":";
					Notify += DWrdtoStr(Port) + ":";

					if(m_autNetwork->ClientMode() == CLIENT_ULTRAPEER)
						Notify += "super";
					else
						Notify += "normal";

					Notify += ":)\r\n";

					m_ServerList[i]->Send(Notify, Notify.GetLength());

					CacheFound  = true;
					Sent		= true;
				}
			}

			if(!CacheFound)
			{
				CString Join = "JOIN #gnutella/cache." + DWrdtoStr(m_ServerList[i]->m_CacheNum) + "\r\n";
				m_ServerList[i]->Send(Join, Join.GetLength());
			}
		}

	return Sent;
}		

void CChatControl::ChatMessage(UINT dwMessID, WPARAM room, LPARAM user)
{
	if(m_pDoc->m_pViewChat)
		CWnd::FromHandle(m_pDoc->m_pViewChat)->SendMessage(dwMessID, room, user);
}

void CChatControl::OnAccept(int nErrorCode) 
{
	if(nErrorCode)
	{
		return;
	};

	CChatIdentd* Ident = new CChatIdentd(this);
	int Safe = Accept(*Ident);

	if(Safe)
	{
		m_IdentdList.push_back(Ident);
	}
	else
	{
		delete Ident;
	}

	/*
	CAsyncSocket New;
	int Safe = Accept(New);

	if(Safe)
	{	
		SOCKET IdentHandle = New.Detach();

		m_PendingList.push_back(IdentHandle);
	}
	*/

	//else
	//	m_pDoc->LogError("Chat Accept Error: " + DWrdtoStr(GetLastError()));

	CAsyncSocket::OnAccept(nErrorCode);
}

void CChatControl::CheckNicks()
{
	// If user hasnt changed the default names, set it as a random string from the share
	/*if(m_pPrefs->m_Nick.Find("Gnuwt") == 0)
		m_pPrefs->m_Nick = m_pDoc->m_pShare->GetRandString(m_pPrefs->m_Nick);


	if(m_pPrefs->m_AltNick.Find("Gnuwt") == 0)
		m_pPrefs->m_AltNick = m_pDoc->m_pShare->GetRandString(m_pPrefs->m_AltNick);*/
}

void CChatControl::Timer()
{
	int ChatConnects = 0;

	
	if(m_TryUltraCache > 0)
		m_TryUltraCache--;

	if(m_ServerList.size() == 0)
		m_UltraCacheMode = false;


	// Manage connected chat servers
	std::vector<CChatServer*>::iterator itServer;
	for(itServer = m_ServerList.begin(); itServer != m_ServerList.end(); itServer++)
	{
		if((*itServer)->m_Status == SOCK_CONNECTED)
		{
			ChatConnects++;
		}
		else
		{
			if((*itServer)->m_SecsDead > 5)
			{
				RemoveServer(*itServer);
				itServer--;
			}
			else
				(*itServer)->m_SecsDead++;
		}
	}



	// Send a connect request if we're connected and in the cache channel
	if(ChatConnects)
		if(m_UltraCacheMode)
		{
			if(m_SecsRequest > 50)
				m_SecsRequest = SendRequest() ? 0 : 50;
			else
				m_SecsRequest++;
		}


	// Chat window open
	if(m_pDoc->m_pViewChat)
	{
		if(m_AutoConnect)
		{
			bool ChatConnect = false;

			for(int i = 0; i < m_ServerList.size(); i++)
				if(m_ServerList[i]->m_Host == "irc.openprojects.net")
					ChatConnect = true;

			if(!ChatConnect)
				GetConnect();
		}
	}

	// Chat window not open
	else if(m_AutoConnect)
	{
		if(ChatConnects)
		{
			if(!m_UltraCacheMode && m_TryUltraCache == 0)
				if(m_autNetwork->ClientMode() == CLIENT_LEAF || m_autPrefs->GetMinConnects() != -1 && m_autNetwork->GetNormalConnectedCount() >= m_autPrefs->GetMinConnects())
					RemoveServer( m_ServerList[0] );
		}
		else
		{
			if(m_autNetwork->GetNormalConnectedCount() == 0)
				GetConnect();

			m_UltraCacheMode = false;
			m_TryUltraCache  = 0;
		}
	}

	// Clean Chat Sockets
	std::vector<CChatServer*>::iterator itChat;
	for(itChat = m_ServerList.begin(); itChat != m_ServerList.end(); itChat++)
		if((*itChat)->m_Status == SOCK_CLOSED)
		{
			delete *itChat;
			m_ServerList.erase(itChat);

			itChat--;
		}

	// Remove old ident connects
	std::vector<CChatIdentd*>::iterator itIdent;
	for(itIdent = m_IdentdList.begin(); itIdent != m_IdentdList.end(); itIdent++)
	{
		(*itIdent)->m_SecsAlive++;

		if((*itIdent)->m_SecsAlive >= 10)
		{
			delete *itIdent;
			m_IdentdList.erase(itIdent);

			itIdent--;
		}
	}

}