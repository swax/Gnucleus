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

#include "AutNetwork.h"
#include "AutCache.h"

#include "ChatControl.h"
#include "ChatRoom.h"
#include "ChatServer.h"
#include "ChatPrefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CChatServer::CChatServer(CChatControl* pChat, CString Host, UINT Port)
{
	m_pChat  = pChat;
	m_Host   = Host;
	m_Port   = Port;

	m_pPrefs   = m_pChat->m_pPrefs;
	m_autCache = m_pChat->m_pDoc->m_autCache;
	

	m_CacheNum	  = 1;  // Change on beta
	m_DataSending = false;
	m_Status      = SOCK_CONNECTING;
	m_SecsDead    = 0;

	m_dwExtraLength = 0;

	// Check for default Chat names and change
	m_pChat->CheckNicks();	

	m_Nick = m_pPrefs->m_Nick;
	m_LoggedIn = false;

	// Setup server display
	m_Console			   = new CChatRoom(this);
	m_Console->m_Name      = m_Host;
	m_Console->m_RoomType  = ROOM_SERVER;

	GnuUsers = 0;

	// Delete Later
	CChatRoom* pDebug  = new CChatRoom(this);
	pDebug->m_Name     = "(raw irc)";
	pDebug->m_RoomType = ROOM_RAW;
	m_RoomList.push_back(pDebug);
}

CChatServer::~CChatServer()
{
	m_Status = SOCK_CLOSED;

	// Flush receive buffer
	byte pBuff[4096];
	while(Receive(pBuff, 4096) > 0)
		;

	if(m_hSocket != INVALID_SOCKET)
		AsyncSelect(0);

	delete m_Console;

	for(int i = 0; i < m_RoomList.size(); i++)
	{
		delete m_RoomList[i];
		m_RoomList[i] = NULL;
	}
	m_RoomList.clear();

	for(i = 0; i < m_PrivateList.size(); i++)
	{
		delete m_PrivateList[i];
		m_PrivateList[i] = NULL;
	}

	m_PrivateList.clear();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CChatServer, CAsyncSocket)
	//{{AFX_MSG_MAP(CChatServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CChatServer member functions

void CChatServer::OnConnect(int nErrorCode) 
{
	if(nErrorCode)
	{
		return;
	}

	m_Status = SOCK_CONNECTED;

	//UINT nPort = m_pComm->m_LocalPort;

	CString NickCommand = "NICK " + m_Nick + "\r\n",
		UserCommand = "USER " + m_pPrefs->m_Nick + " 0 * :Gnuke\r\n";
		//	UserCommand = "USER gnucleus 0 * :Gnuke\r\n";

	Send(NickCommand, NickCommand.GetLength());
	Send(UserCommand, UserCommand.GetLength());

	// Re-Join rooms
	for(int i = 1; i < m_RoomList.size(); i++)
	{
		CString JoinCommand = "JOIN " + m_RoomList[i]->m_Name + "\r\n";
		Send(JoinCommand, JoinCommand.GetLength());
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CChatServer::OnReceive(int nErrorCode) 
{
	int dwOffset = 0, dwBuffLength = 0;

	// Add any unprocessed bytes from last receive
	if(m_dwExtraLength)
	{
		ASSERT(m_dwExtraLength);
		memcpy(m_pBuff, m_pExtra, m_dwExtraLength);
		dwOffset = m_dwExtraLength;
		m_dwExtraLength = 0;
	}

	dwBuffLength = Receive(&m_pBuff[dwOffset], 4096 - dwOffset);

	// Check for errors
	if(!dwBuffLength || dwBuffLength == SOCKET_ERROR)
	{
		Close("Socket Error");
		return;
	}

	dwBuffLength += dwOffset;

	m_dwExtraLength = Breakup(m_pBuff, dwBuffLength);

	if (0 != m_dwExtraLength)
	{
		if(4096 > m_dwExtraLength)
			memcpy(m_pExtra, &m_pBuff[dwBuffLength - m_dwExtraLength], m_dwExtraLength);
		else
			return; // Very bad, dropping packets
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

DWORD CChatServer::Breakup(byte* bundle, DWORD length)
{
	int frontPos = 0, endPos = -1;

	
	CString RawData( (char*) bundle, length);

	endPos = RawData.Find("\r\n", frontPos);

	while(endPos > -1)
	{
		Process( RawData.Mid(frontPos, endPos + 2 - frontPos) );

		frontPos = endPos + 2;
		endPos  = RawData.Find("\r\n", frontPos);
	}

	return length - frontPos;
}

void CChatServer::Process(CString Message)
{
	int nPos = Message.Find(" ");
	CString Prefix = Message.Left(nPos);

	if(!Prefix.GetLength())
		return;

	// Direct Message
	if(Prefix[0] != ':')
	{
		if(Prefix.CompareNoCase("NOTICE") == 0)
			Receive_Notice(Message);

		if(Prefix.CompareNoCase("ERROR") == 0)
			Receive_Error(Message);
	
		if(Prefix.CompareNoCase("PING") == 0)
			Receive_Ping(Message);
	}

	// IRC prefix ':' message (most messages)
	else
	{
		// Prefix is either a server or user
		Prefix = Prefix.Mid(1);

		// Get Command
		nPos++;
		int nBackPos = Message.Find(" ", nPos);
		CString Command = Message.Mid(nPos, nBackPos - nPos); 

		// User message
		if( Prefix.Find('!') > 0)
		{
			if(Command.CompareNoCase("INVITE") == 0)
				Receive_Invite(Message, 0);

			if(Command.CompareNoCase("JOIN") == 0)
				Receive_Join(Message, 0);

			if(Command.CompareNoCase("KICK") == 0)
				Receive_Kick(Message);

			if(Command.CompareNoCase("MODE") == 0)
				Receive_Mode(Message, 0);

			if(Command.CompareNoCase("NICK") == 0)
				Receive_Nick(Prefix, Message.Mid(nPos + 6) );

			if(Command.CompareNoCase("NOTICE") == 0)
				Receive_Notice(Message);

			if(Command.CompareNoCase("PART") == 0)
				Receive_Part(Message);

			if(Command.CompareNoCase("PRIVMSG") == 0)
				Receive_PrivMsg(Message);

			if(Command.CompareNoCase("QUIT") == 0)
				Receive_Quit(Message);

			if(Command.CompareNoCase("TOPIC") == 0)
				Receive_Topic(Message, 0);
		}

		// Server message
		else
		{
			int code = atoi(Command);

			// Numeric Command
			if(code)
			{
				// DELETE
				if(code == RPL_WHOREPLY)
					GnuUsers++;

				if(code == RPL_WELCOME  || 
				   code == RPL_YOURHOST || 
				   code == RPL_CREATED  ||
				   code == RPL_MYINFO     )
					Receive_Welcome( Message.Mid(nPos), code);

				if(code == RPL_MOTDSTART || 
				   code == RPL_MOTD		 || 
				   code == RPL_ENDOFMOTD  )
					Receive_MOTD( Message.Mid(nPos), code);

				if(code == RPL_LUSERCLIENT   || 
				   code == RPL_LUSEROP		 || 
				   code == RPL_LUSERCHANNELS ||
				   code == RPL_LUSERME			)
					Receive_ServerStats( Message.Mid(nPos), code);

				if(code == RPL_LOCALUSERS  || 
				   code == RPL_GLOBALUSERS || 
				   code == RPL_STATSDLINE  )
					Receive_ServerCount( Message.Mid(nPos), code);

				if(code == RPL_WHOISUSER     || 
				   code == RPL_WHOISCHANNELS || 
				   code == RPL_WHOISSERVER   ||
				   code == RPL_WHOISIDLE	 ||
				   code == RPL_ENDOFWHOIS			)
					Receive_Whois( Message.Mid(nPos), code);

				if(code == RPL_NAMREPLY || code == RPL_ENDOFNAMES)
					Receive_Join(Message, code);

				if(code == RPL_CHANNELMODEIS)
					Receive_Mode(Message, code);

				if(code == RPL_TOPIC || code == RPL_NOTOPIC)
					Receive_Topic(Message, code);

				if(code == RPL_INVITING)
					Receive_Invite(Message, code);

				if(code == RPL_AWAY  || 
				   code == RPL_UNAWAY || 
				   code == RPL_NOWAWAY  )
					Receive_Away( Message.Mid(1), code);

				
				// Error Messages
				if( 400 <= code && code < 505)
					Receive_ErrorCode(Message.Mid(nBackPos + 1), code);
			}
			
			// String Command
			else
			{
				if(Command.CompareNoCase("NOTICE") == 0)
					Receive_Notice( Message.Mid(nPos) );
			}
		}
	}

	if(m_RoomList.size())
		m_RoomList[0]->WriteToDisplay(Message, COLOR_BLACK);
}

void CChatServer::Receive_Welcome(CString Welcome, int code)
{
	m_pChat->StopIdentd();

	// Add this server to the top of the try list
	for(int i = 0; i < m_pPrefs->m_ChatNets.size(); i++)		
		for(int j = 0; j < m_pPrefs->m_ChatNets[i].Servers.size(); j++)
			if(m_pPrefs->m_ChatNets[i].Servers[j].Find(m_Host) != -1)
			{
				CString Temp = m_pPrefs->m_ChatNets[i].Servers[j];
		
				int pos = 0;
				std::vector<CString>::iterator itHost;
				for(itHost = m_pPrefs->m_ChatNets[i].Servers.begin(); itHost != m_pPrefs->m_ChatNets[i].Servers.end(); itHost++, pos++)
					if(pos == j)
					{
 						m_pPrefs->m_ChatNets[i].Servers.erase(itHost);
						break;
					}

				m_pPrefs->m_ChatNets[i].Servers.insert(m_pPrefs->m_ChatNets[i].Servers.begin(), Temp);
				
				break;
			}
			
	if(code < RPL_MYINFO)
	{
		m_LoggedIn = true;
		Welcome = Welcome.Mid( Welcome.Find(':') + 1);
	}

	// Code 004 has no ':'
	else
	{
		int pos = Welcome.Find(" ");
		pos		= Welcome.Find(" ", pos + 1);
		Welcome = Welcome.Mid( pos + 1);
	}

	if(code == RPL_WELCOME)
	{
		Welcome = "\n" + Welcome;

		// Set invisible
		CString Mode = "MODE " + m_Nick + " +i\r\n";
		Send(Mode, Mode.GetLength());

		// Connected to server now so join channels
		CString Join;

		if(m_Host != "irc.openprojects.net")
		{
			Join = "JOIN #gnutella/cache." + DWrdtoStr(m_CacheNum) + "\r\n";
			Send(Join, Join.GetLength());


			if(m_pPrefs->m_PrefChannel != "")
			{
				Join = "JOIN #gnutella/cache/" + m_pPrefs->m_PrefChannel + "\r\n";
				Send(Join, Join.GetLength());
			}
		}

		if(m_pChat->m_pDoc->m_pViewChat)
		{
			if(m_Host != "irc.openprojects.net")
				Join = "JOIN #gnutella/chat\r\n";
			else
				Join = "JOIN #gnucleus\r\n";

			Send(Join, Join.GetLength());


			if(m_pPrefs->m_PrefChannel != "")
			{
				Join = "JOIN #gnutella/chat/" + m_pPrefs->m_PrefChannel + "\r\n";
				Send(Join, Join.GetLength());
			}
		}
	}

	m_Console->WriteToDisplay(Welcome, COLOR_BLACK);
}

void CChatServer::Receive_ServerStats(CString Stats, int code)
{
	if(code == RPL_LUSERCLIENT || code == RPL_LUSERME)
		Stats = Stats.Mid( Stats.Find(':') + 1);

	// These codes have no ':'
	else
	{
		int pos = Stats.Find(" ");
		pos		= Stats.Find(" ", pos + 1);
		Stats   = Stats.Mid( pos + 1);
		Stats.Remove(':');
	}

	if(code == RPL_LUSERCLIENT)
		Stats = "\n" + Stats;

	m_Console->WriteToDisplay(Stats, COLOR_BLACK);
}


void CChatServer::Receive_ServerCount(CString Count, int code)
{
	Count = Count.Mid( Count.Find(':') + 1);

	if(code == RPL_LOCALUSERS)
		Count = "\n" + Count;

	m_Console->WriteToDisplay(Count, COLOR_BLACK);
}


void CChatServer::Receive_MOTD(CString MOTD, int code)
{
	switch(code)
	{
	case RPL_MOTDSTART:
		MOTD = "\n" + MOTD.Mid( MOTD.Find(':') + 1);
		break;
	case RPL_MOTD:
		MOTD = MOTD.Mid( MOTD.Find(':') + 1);
		break;
	case RPL_ENDOFMOTD:
		MOTD = MOTD.Mid( MOTD.Find(':') + 1);
		break;
	}

	m_Console->WriteToDisplay(MOTD, COLOR_BLACK);
}


void CChatServer::Receive_Ping(CString Ping)
{
	m_Console->WriteToDisplay("\n\n" + Ping, COLOR_GREEN);

	CString Pong = "PONG" + Ping.Mid(4) + "\r\n";
	Send(Pong, Pong.GetLength());

	Pong.Remove('\r');
	Pong.Remove('\n');

	m_Console->WriteToDisplay(Pong, COLOR_GREEN);
}


void CChatServer::Receive_Notice(CString Notice)
{
	Notice.Remove('\r');
	Notice.Remove('\n');
	Notice.Remove(0x01);
		
	int nPos = Notice.Find(" ");
	CString Prefix = Notice.Left(nPos);
	Notice = Notice.Mid( Notice.Find(':', nPos) + 1);

	nPos = Notice.Find("open(");

	// Check for 'special' notice
	if(nPos != -1)
	{
		int frontPos = nPos + 5;
		int backPos  = Notice.Find(":", nPos);
		
		CString HostPort = Notice.Mid(frontPos, backPos - frontPos);

		frontPos = backPos + 1;
		backPos  = Notice.Find(":", frontPos);
		HostPort += ":" + atoi( Notice.Mid(frontPos, backPos - frontPos));

		if(Notice.Find("super") != -1)
			m_autCache->AddNode(HostPort, true);
		else
			m_autCache->AddNode(HostPort, false);

		return;
	}

	// User Notice
	if(Prefix.Find('!') != -1)
	{
		CString Nick = Prefix.Mid(1, Prefix.Find('!') - 1);

		Notice = "-" + Nick + "- " + Notice;
	}

	m_Console->WriteToDisplay("\n" + Notice, COLOR_BROWN);
	m_pChat->ChatMessage(CHAT_NOTICE, (DWORD) m_Console);
}


void CChatServer::Receive_Error(CString Error)
{
	Error = Error.Mid( Error.Find(':') + 1);

	for(int i = 0; i < m_RoomList.size(); i++)
		if(m_RoomList[i])
			m_RoomList[i]->WriteToDisplay(Error + "\n", COLOR_RED);

	m_Console->WriteToDisplay("\n" + Error, COLOR_RED);
}


void CChatServer::Receive_ErrorCode(CString Error, int code)
{
	// While logging in, nick may conflict
	if(code == ERR_NICKNAMEINUSE)
		if(!m_LoggedIn)
		{
			if(Error.Find("*") == 0 && Error.Find(m_pPrefs->m_Nick) != -1)
				m_Nick = m_pPrefs->m_AltNick;
			else
			{
				CString RandEnd( DWrdtoStr(rand() % 999 + 0));
				
				int cut = m_pPrefs->m_Nick.GetLength();
				if(cut > 6)
				{
					cut = 7;
					m_pPrefs->m_Nick = m_pPrefs->m_Nick.Left(cut);
				}

				m_pPrefs->m_Nick += RandEnd;
				m_Nick = m_pPrefs->m_Nick;
			}

			CString Change = "NICK " + m_Nick + "\r\n";
			Send(Change, Change.GetLength());
		}
		else
			m_Nick = Error.Left(Error.Find(' '));

	Error.Replace(m_Nick + " ", "");


	// If unable to join cache channel, join the next cache channel  up
	if(code == ERR_CHANNELISFULL	|| code == ERR_UNKNOWNMODE		||
	   code == ERR_INVITEONLYCHAN	|| code == ERR_BANNEDFROMCHAN	||
	   code == ERR_BADCHANNELKEY	|| code == ERR_BADCHANMASK)	
	{
		if(Error.Find("#gnutella/cache.") > -1)
		{
			m_CacheNum++;

			CString Join = "JOIN #gnutella/cache." + DWrdtoStr(m_CacheNum) + "\r\n";
			Send(Join, Join.GetLength());
		}
	}

	// If cache channel has been moderated, leave cache channel sendrequest() rejoins new channel
	if(code == ERR_CANNOTSENDTOCHAN)
		if(Error.Find("#gnutella/cache." + DWrdtoStr(m_CacheNum)) > -1)
		{
			CString Part = "PART #gnutella/cache." + DWrdtoStr(m_CacheNum) + "\r\n";
			Send(Part, Part.GetLength());
			
			m_CacheNum++;
			CString Join = "JOIN #gnutella/cache." + DWrdtoStr(m_CacheNum) + "\r\n";
			Send(Join, Join.GetLength());
		}



	// Break apart error and display 
	int EndParam = Error.Find(':');
	int EndPos   = Error.Find(' ');

	CString Token1, Token2, Message = "! ";
	
	if(EndPos < EndParam)
	{
		Token1 = Error.Left(EndPos);
		Message += Token1 + " ";
	}

	int FrontPos = EndPos + 1;
	EndPos       = Error.Find(' ', EndPos + 1);

	if(EndPos < EndParam)
	{
		Token2 = Error.Mid(FrontPos, EndPos - FrontPos);
		Message += Token2 + " ";
	}

	Message += Error.Mid(EndParam + 1);
	Message.Remove('\n');
	Message.Remove('\r');

	m_Console->WriteToDisplay("\n" + Message, COLOR_BROWN);
	m_pChat->ChatMessage(CHAT_NOTICE, (DWORD) m_Console);
}


void CChatServer::Receive_Whois(CString Whois, int code)
{
	int pos = Whois.Find(" ");
	pos		= Whois.Find(" ", pos + 1);
	Whois   = Whois.Mid( pos + 1);
	Whois.Remove(':');


	if(code == RPL_WHOISUSER)
	{
		pos = Whois.Find(" ");
		Whois.Insert(pos, " is");

		pos = Whois.Find(" ", pos + 4);
		Whois.SetAt(pos, '@');

		Whois = "\n" + Whois;
	}

	if(code == RPL_WHOISCHANNELS)
	{
		pos = Whois.Find(" ");
		Whois.Insert(pos, " on");
	}

	if(code == RPL_WHOISSERVER)
	{
		pos = Whois.Find(" ");
		Whois.Insert(pos, " using");
	}

	if(code == RPL_WHOISIDLE)
	{
		pos = Whois.Find(" ");
		Whois.Insert(pos, " has been ");
	}

	m_Console->WriteToDisplay(Whois, COLOR_BLACK);
}


void CChatServer::Receive_Nick(CString OldName, CString NewName)
{
	NewName.Remove('\n');
	NewName.Remove('\r');

	int pos = OldName.Find("!");
	OldName = OldName.Mid(0, pos);
	CString ChangeNick = " >> " + OldName + " is now " + NewName + "\n";

	// If it is us
	if(NewName == m_Nick)
	{
		m_Nick = NewName;
		m_Console->WriteToDisplay(ChangeNick, COLOR_GREEN);
	}

	for(int i = 0; i < m_RoomList.size(); i++)
		if( pos=m_RoomList[i]->FindUser( OldName ) != -1)
		{
			m_RoomList[i]->WriteToDisplay(ChangeNick, COLOR_GREEN);

			CString Nick = "*" + OldName + "/" + NewName;
			m_pChat->ChatMessage(CHAT_USER, (DWORD) m_RoomList[i], (DWORD) &Nick);

			m_RoomList[i]->RenameUser(OldName, NewName);
		}

	for(i = 0; i < m_PrivateList.size(); i++)
		if( pos=m_PrivateList[i]->FindUser( OldName ) != -1)
		{
			m_PrivateList[i]->WriteToDisplay(ChangeNick, COLOR_GREEN);

			CString Nick = "*" + OldName + "/" + NewName;
			m_pChat->ChatMessage(CHAT_USER, (DWORD) m_PrivateList[i], (DWORD) &Nick);

			m_PrivateList[i]->RenameUser(OldName, NewName);
		}
}


void CChatServer::Receive_Join(CString Message, int code)
{
	// User joining channel
	if(!code)
	{
		// Get Nick and Channel of user joining
		CString Nick	= Message.Mid(1, Message.Find('!') - 1);
		CString Channel = Message.Mid( Message.Find('#'));
		Channel			= Channel.Left( Channel.Find('\r'));

		// If it is us
		if( m_Nick.CompareNoCase(Nick) == 0)
		{
			// Add room
			if( GetChannel(Channel) )
				return;
			else
				CreateRoom(Channel);

			// Join ultrapeer channel
			if(Channel == "#gnutella/cache." + DWrdtoStr(m_CacheNum))
				if(m_pChat->m_autNetwork->ClientMode() == CLIENT_ULTRAPEER)
				{
					CString Join = "JOIN #gnutella/ultracache." + DWrdtoStr(m_CacheNum) + "\r\n";
					Send(Join, Join.GetLength());

					m_pChat->m_TryUltraCache  = 15;
					m_pChat->m_UltraCacheMode = false;
				}
		}

		// If it is someone else
		else
		{
			CChatRoom* pRoom = GetChannel(Channel);

			if(pRoom)
			{
				pRoom->AddUser(Nick);
				pRoom->WriteToDisplay(" >> " + Nick + " has joined " + Channel + "\n", COLOR_GREEN);
				
				EventHandler("JOIN", Nick, Channel, pRoom, Message);
				
				Nick = "+" + Nick;
				m_pChat->ChatMessage(CHAT_USER, (DWORD) pRoom, (DWORD) &Nick);
			}
			else
				CreateRoom(Channel);
		}
	}

	// List of users in channel
	if(code == RPL_NAMREPLY)
	{
		// Get the channel name
		int pos		= Message.Find('#');
		int backPos = Message.Find(' ', pos);

		CString Channel = Message.Mid(pos, backPos - pos);

		CChatRoom* pRoom = GetChannel(Channel);

		if(pRoom)
		{
			// Update user list
			if(pRoom->m_UserLock)
			{
				pRoom->m_UserList.clear();
				pRoom->m_UserLock = false;
			}

			pos		= Message.Find(":", backPos) + 1;
			int end = Message.Find('\r');

			// Extract names from message and insert into list
			while(pos < end)
			{
				backPos = Message.Find(" ", pos);
		
				if(backPos == -1)
					backPos = end;

				CString User = Message.Mid(pos, backPos - pos);

				pRoom->AddUser(User);

				pos = backPos + 1;
			}

			m_pChat->ChatMessage(CHAT_USER, (DWORD) pRoom);	
		}
		else
			CreateRoom(Channel);
	}
	
	// End of user list
	if(code == RPL_ENDOFNAMES)
	{
		// Get the channel name
		int pos			= Message.Find('#');
		int backPos		= Message.Find(' ', pos);
		CString Channel = Message.Mid(pos, backPos - pos);
		
		CChatRoom* pRoom = GetChannel(Channel);

		if(pRoom)
		{
			pRoom->m_UserLock = true;

			// If this is the ultrapeer room and there are less than 10 users, stay
			if(m_pChat->m_TryUltraCache > 0)
				if(Channel == "#gnutella/ultracache." + DWrdtoStr(m_CacheNum))
					if(pRoom->m_UserList.size() > 10)
					{
						if(m_pChat->m_pDoc->m_pViewChat == NULL)
							Close("Too many ultrapeers");

						m_pChat->m_TryUltraCache  = 0;
						m_pChat->m_UltraCacheMode = false;
					}
					else
					{
						m_pChat->m_TryUltraCache  = 0;
						m_pChat->m_UltraCacheMode = true;
					}

		}
		else
			CreateRoom(Channel);

		CString ChanMode = "MODE " + Channel + "\r\n";
		Send(ChanMode, ChanMode.GetLength());
	}
}

void CChatServer::Receive_Mode(CString Message, int code)
{
	if(!code)
	{
		CString Channel;
		CString Nick = Message.Mid(1, Message.Find('!') - 1);

		CChatRoom* pRoom = NULL;

		int pos		= Message.Find('#', Message.Find("MODE"));
		int backPos = 0;

		if(pos != -1)
		{	
			backPos		= Message.Find(' ', pos);
			Channel = Message.Mid(pos, backPos - pos);
			pRoom = GetChannel(Channel);
		}
		else
			pRoom = NULL;

		if(pRoom)
		{
			Message = Message.Mid(backPos + 1);
			Message.Remove('\n');
			Message.Remove('\r');

			pRoom->WriteToDisplay(" >> " + Nick + " changed mode to " + Message + "\n", COLOR_ORANGE);

			// Too lazy to parse, todo: add more code here
			CString Request = "NAMES " + pRoom->m_Name + "\r\n";
			Send(Request, Request.GetLength());

			// at end of names function, mode gets sent to detect any moderation
		}
	}

	if(code == RPL_CHANNELMODEIS)
	{
		// Get the channel name
		int pos			= Message.Find('#');
		int backPos		= Message.Find(' ', pos);
		CString Channel = Message.Mid(pos, backPos - pos);

		CChatRoom* pRoom = GetChannel(Channel);

		if(pRoom)
		{
			Message = Message.Mid(backPos + 1);
			Message.Remove('\n');
			Message.Remove('\r');

			pRoom->WriteToDisplay(" >> Mode set to " + Message + "\n", COLOR_ORANGE);

			// If mode of cache channel is moderated, leave
			if(Channel.Find("#gnutella/cache." + DWrdtoStr(m_CacheNum)) != -1)
				if(Message.Find("+") != -1 && Message.Find("m") != -1)
				{
					CString Part = "PART #gnutella/cache." + DWrdtoStr(m_CacheNum) + "\r\n";
					Send(Part, Part.GetLength());
					RemoveRoom(pRoom);
					
					m_CacheNum++;
					CString Join = "JOIN #gnutella/cache." + DWrdtoStr(m_CacheNum) + "\r\n";
					Send(Join, Join.GetLength());
				}
		}
	}
}

void CChatServer::Receive_Topic(CString Message, int code)
{
	if(!code)
	{
		CString Nick = Message.Mid(1, Message.Find('!') - 1);

		int pos			= Message.Find('#', Message.Find("TOPIC"));
		int backPos		= Message.Find(' ', pos);
		CString Channel = Message.Mid(pos, backPos - pos);

		CChatRoom* pRoom = GetChannel(Channel);

		if(pRoom)
		{
			CString Topic = Message.Mid( Message.Find(':', backPos) + 1);
			Topic.Remove('\n');
			Topic.Remove('\r');

			pRoom->m_Topic = Topic;
			pRoom->WriteToDisplay(" >> " + Nick + " has changed topic to \"" + Topic + "\"\n", COLOR_ORANGE);
		
			m_pChat->ChatMessage(CHAT_TOPIC, (DWORD) pRoom);
		}
	}

	if(code)
	{
		// Get the channel name
		int pos			= Message.Find('#');
		int backPos		= Message.Find(' ', pos);
		CString Channel = Message.Mid(pos, backPos - pos);

		CChatRoom* pRoom = GetChannel(Channel);

		if(pRoom && code == RPL_TOPIC)
		{
			CString Topic = Message.Mid( Message.Find(':', backPos) + 1);
			Topic.Remove('\n');
			Topic.Remove('\r');

			pRoom->m_Topic = Topic;
			pRoom->WriteToDisplay(" >> Topic set to \"" + Topic + "\"\n", COLOR_ORANGE);
			
			m_pChat->ChatMessage(CHAT_TOPIC, (DWORD) pRoom);
		}

		if(pRoom && code == RPL_NOTOPIC)
		{
			pRoom->m_Topic = "";
			pRoom->WriteToDisplay(" >> Topic not set\n", COLOR_ORANGE);
			
			m_pChat->ChatMessage(CHAT_TOPIC, (DWORD) pRoom);
		}
	}
}


void CChatServer::Receive_Invite(CString Message, int code)
{
	if(!code)
	{
		CString Nick = Message.Mid(1, Message.Find('!') - 1);

		int pos			= Message.Find('#', Message.Find("INVITE"));
		int backPos		= Message.Find('\r', pos);
		CString Channel = Message.Mid(pos, backPos - pos);

		for(int i = 0; i < m_RoomList.size(); i++)
			m_RoomList[i]->WriteToDisplay(" >> " + Nick + " has invited you to " + Channel + "\n", COLOR_GREEN);
	
		m_Console->WriteToDisplay("\n >> " + Nick + " has invited you to " + Channel, COLOR_GREEN);
	}

	if(code)
	{
		int pos = Message.Find('#');
		int backPos = Message.Find(' ', pos);
		CString Channel = Message.Mid(pos, backPos - pos);

		for(int i = 0; i < m_RoomList.size(); i++)
			m_RoomList[i]->WriteToDisplay(" >> You have been invited you to " + Channel + "\n", COLOR_GREEN);
	
		m_Console->WriteToDisplay("\n >> You have been invited you to " + Channel, COLOR_GREEN);
	}
}

void CChatServer::Receive_Away(CString Message, int code)
{
	if(code == RPL_AWAY)
	{
		int pos		 = Message.Find(' ', Message.Find( DWrdtoStr(RPL_AWAY) )) + 1;
		int backPos  = Message.Find(':', pos) - 1;
		CString Nick = Message.Mid(pos, backPos - pos);

		for(int i = 0; i < m_RoomList.size(); i++)
			if( m_RoomList[i]->FindUser( Nick )  != -1)
				m_RoomList[i]->WriteToDisplay(" >> " + Nick + " is away\n", COLOR_GREEN);
	
		m_Console->WriteToDisplay(" is Away\n", COLOR_BLACK);
	}
	else
	{
		CString Text;

		if(code == RPL_UNAWAY)
			Text = " >> You are no longer marked as being away";
		if(code == RPL_NOWAWAY)
			Text = " >> You have been marked as being away";

		for(int i = 0; i < m_RoomList.size(); i++)
			m_RoomList[i]->WriteToDisplay(Text + "\n", COLOR_GREEN);
	
		m_Console->WriteToDisplay("\n" + Text, COLOR_GREEN);
	
	}
}


void CChatServer::Receive_Part(CString Message)
{
	// Get Nick and Channel of user joining
	CString Nick	= Message.Mid(1, Message.Find('!') - 1);
	CString Channel = Message.Mid( Message.Find('#'));
	Channel = Channel.Left( Channel.Find('\r'));

	// If it is us
	if( m_Nick.CompareNoCase(Nick) == 0)
	{
		RemoveRoom( GetChannel(Channel) );

		m_Console->WriteToDisplay("\n >> Left " + Channel, COLOR_BLUE);
	}

	// If it is someone else
	else
	{
		CChatRoom* pRoom = GetChannel(Channel);

		if(pRoom)
		{
			pRoom->WriteToDisplay(" >> " + Nick + " has left " + Channel + "\n", COLOR_BLUE);

			EventHandler("PART", Nick, Channel, pRoom, Message);

			CString NickTag = "-" + Nick;
			m_pChat->ChatMessage(CHAT_USER, (DWORD) pRoom, (DWORD) &NickTag);

			pRoom->RemoveUser(Nick);
		}
	}
}


void CChatServer::Receive_Kick(CString Message)
{
	// Who's kicking
	CString Nick = Message.Mid(1, Message.Find('!') - 1);
	
	// What channel
	int pos			= Message.Find('#', Message.Find("KICK"));
	int backPos		= Message.Find(' ', pos);
	CString Channel = Message.Mid(pos, backPos - pos);

	// Who's being kicked
	pos			 = backPos + 1;
	backPos		 = Message.Find(' ', pos);
	CString User = Message.Mid(pos, backPos - pos);

	// Reason why
	Message = Message.Mid( Message.Find(':', backPos) + 1);
	Message.Remove('\r');
	Message.Remove('\n');

	CChatRoom* pRoom = GetChannel(Channel);

	// If it us
	if(pRoom && User == m_Nick)
	{

		RemoveRoom(pRoom);

		m_Console->WriteToDisplay("\n >> Kicked from " + Channel + " by " + Nick + " (" + Message + ")", COLOR_BLUE);
	}

	// If it is someone else
	else if(pRoom)
	{
		pRoom->WriteToDisplay(" >> " + User + " kicked from " + Channel + " by " + Nick  + " (" + Message + ")\n", COLOR_BLUE);

		CString UserTag = "-" + User;
		m_pChat->ChatMessage(CHAT_USER, (DWORD) pRoom, (DWORD) &UserTag);
	
		EventHandler("KICK", Nick, Channel, pRoom, Message);

		pRoom->RemoveUser(User);
	}
}


void CChatServer::Receive_Quit(CString Message)
{
	CString Nick = Message.Mid(1, Message.Find('!') - 1);

	Message = Message.Mid(Message.Find(':', 1) + 1);
	Message.Remove('\n');
	Message.Remove('\r');

	for(int i = 0; i < m_RoomList.size(); i++)
		if( m_RoomList[i]->FindUser( Nick ) != -1)
		{
			m_RoomList[i]->WriteToDisplay(" >> " + Nick + " has quit (" + Message + ")\n", COLOR_BLUE);

			EventHandler("QUIT", Nick, "", m_RoomList[i], Message);

			CString NickTag = "-" + Nick;
			m_pChat->ChatMessage(CHAT_USER, (DWORD) m_RoomList[i], (DWORD) &NickTag);
		
			m_RoomList[i]->RemoveUser(Nick);
		}
}


void CChatServer::Receive_PrivMsg(CString Message)
{
	CString Sender = Message.Mid(1, Message.Find('!') - 1);

	int pos        = Message.Find("PRIVMSG") + 8;
	int backPos    = Message.Find(' ', pos + 1);
	CString Target = Message.Mid(pos, backPos - pos);

	Message = Message.Mid(Message.Find(':', backPos) + 1);
	Message.Remove('\n');
	Message.Remove('\r');

	if(Message.Find(m_Nick) != -1)
		if(m_pChat->m_pDoc->m_pViewChat)
			if( !CWnd::FromHandle(m_pChat->m_pDoc->m_pViewChat)->IsWindowVisible() )
				m_pChat->m_NickAlert = true;

	CChatRoom* pRoom = GetChannel(Target);
	
	CString CTCP = (char) 0x01;
	CTCP += "VERSION";
	CTCP += (char) 0x01;

	// Check for CTCP Version
	if(Message == CTCP)
	{
		CTCP = "NOTICE " + Sender + " :" + (char) 0x01 + "VERSION Gnucleus ";
		CTCP = CTCP + GNUCLEUS_VERSION + (char) 0x01 + "\r\n";

		Send(CTCP, CTCP.GetLength());
		return;
	}

	/* Check for CTCP Port // No longer needed
	CTCP.Replace("VERSION", "PORT");
	if(Message == CTCP)
	{
		UINT nPort = m_pComm->m_LocalPort;

		CTCP = "NOTICE " + Sender + " :" + (char) 0x01 + "PORT ";
		CTCP = CTCP + DWrdtoStr(nPort) + (char) 0x01 + "\r\n";

		Send(CTCP, CTCP.GetLength());
		return;
	}*/

	// Message to a channel
	if(pRoom)
	{
		if(Message.GetLength())
		{
			if(Message[0] == 0x01)
			{
				Message = Message.Mid( Message.Find("ACTION") + 7);
				Message.Remove(0x01);

				pRoom->WriteToDisplay("* " + Sender + " " + Message + "\n", COLOR_BLACK);

				EventHandler("TEXT",Sender, Target, pRoom, Message);
			}
			else
				pRoom->WriteToDisplay("<" + Sender + "> " + Message + "\n", COLOR_BLACK);
		}
	}

	// Message to user
	if(Target == m_Nick)
	{
		// If room exists
		pRoom = GetPrivate(Sender);

		if(pRoom)
		{
			pRoom->WriteToDisplay("<" + Sender + "> " + Message + "\n", COLOR_BLACK);
			EventHandler("TEXT",Sender, Target, pRoom, Message);
		}

		// Create Room
		else
		{
			CChatRoom* pRoom  = new CChatRoom(this);
			pRoom->m_Name     = Sender;
			pRoom->m_RoomType = ROOM_PRIVATE;
			m_PrivateList.push_back(pRoom);

			pRoom->AddUser(m_Nick);
			pRoom->AddUser(Sender);

			m_pChat->ChatMessage(CHAT_UPDATE, NULL);
			m_pChat->ChatMessage(CHAT_SELECT, (DWORD) pRoom);

			pRoom->WriteToDisplay("<" + Sender + "> " + Message + "\n", COLOR_BLACK);
			EventHandler("TEXT", Sender, Target, pRoom, Message);
		}
	}
}


CChatRoom* CChatServer::GetChannel(CString Channel)
{
	CChatRoom* pRoom = NULL;

	for(int i = 0; i < m_RoomList.size(); i++)
		if(m_RoomList[i]->m_Name.CompareNoCase(Channel) == 0)
			pRoom = m_RoomList[i];

	return pRoom;
}


CChatRoom* CChatServer::GetPrivate(CString Sender)
{
	CChatRoom* pRoom = NULL;

	for(int i = 0; i < m_PrivateList.size(); i++)
		if(m_PrivateList[i]->m_Name.CompareNoCase(Sender) == 0)
			pRoom = m_PrivateList[i];

	return pRoom;
}

CChatRoom* CChatServer::CreateRoom(CString Name)
{
	CChatRoom* pRoom  = new CChatRoom(this);
	pRoom->m_Name     = Name;
	pRoom->m_RoomType = ROOM_CHANNEL;
	m_RoomList.push_back(pRoom);

	m_pChat->ChatMessage(CHAT_UPDATE, NULL);
	
	CString lowName = Name;
	lowName.MakeLower();


	if(Name.Find("#gnutella/cache") == 0 || Name.Find("#gnutella/ultracache") == 0)
	{
		for(int i = 0; i < m_RoomList.size(); i++)
			if(m_RoomList[i]->m_Name == "#gnutella/chat")
				m_pChat->ChatMessage(CHAT_SELECT, (DWORD) m_RoomList[i]);

	}
	else
		m_pChat->ChatMessage(CHAT_SELECT, (DWORD) pRoom);


	return pRoom;
}

void CChatServer::RemoveRoom(CChatRoom* pRoom)
{
	if(pRoom)
	{
		std::vector<CChatRoom*>::iterator itRoom;

		// Scan Channels
		for(itRoom = m_RoomList.begin(); itRoom != m_RoomList.end(); itRoom++)
			if(*itRoom == pRoom)
			{
				delete *itRoom;
				*itRoom = NULL;
				m_RoomList.erase(itRoom);

				itRoom--;
			}

		// Scan Private Messages
		for(itRoom = m_PrivateList.begin(); itRoom != m_PrivateList.end(); itRoom++)
			if(*itRoom == pRoom)
			{
				delete *itRoom;
				*itRoom = NULL;
				m_PrivateList.erase(itRoom);

				itRoom--;
			}

		// First clear channel, then select it
		m_pChat->ChatMessage(CHAT_UPDATE, NULL);
		m_pChat->ChatMessage(CHAT_UPDATE, NULL);
	}
}

int CChatServer::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	int Command = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
	
	if(Command != SOCKET_ERROR)
		m_DataSending = true;

	return Command;
}

void CChatServer::OnSend(int nErrorCode) 
{
	m_DataSending = false;
	
	CAsyncSocket::OnSend(nErrorCode);
}


void CChatServer::OnClose(int nErrorCode) 
{
	m_Status = SOCK_CLOSED;

	CString Error = "*** Disconnected";

	for(int i = 0; i < m_RoomList.size(); i++)
		if(m_RoomList[i])
			m_RoomList[i]->WriteToDisplay(Error + "\n", COLOR_RED);

	m_Console->WriteToDisplay("\n" + Error, COLOR_RED);

	CAsyncSocket::OnClose(nErrorCode);
}

void CChatServer::Close(CString Error) 
{
	// Send quit command to IRC server
	if(m_Status == SOCK_CONNECTED)
	{
		CString Reason;

		if(m_pChat->m_pDoc->m_ShutDown)
			Reason = "evolving";
		else
			Reason = "gnuked";

		CString Quit = "QUIT :" + Reason + "\r\n";
		Send(Quit, Quit.GetLength());
	}

	// Shutdown socket
	m_Status = SOCK_CLOSED;

	if(m_hSocket != INVALID_SOCKET)
	{
		AsyncSelect(0);
		ShutDown(1);

		CAsyncSocket::Close();
	}

	Error = "*** " + Error;

	for(int i = 0; i < m_RoomList.size(); i++)
		if(m_RoomList[i])
			m_RoomList[i]->WriteToDisplay(Error + "\n", COLOR_RED);
	m_Console->WriteToDisplay("\n" + Error, COLOR_RED);
}


void CChatServer::EventHandler(CString eventName, CString sender, CString channel, CChatRoom* pChannel, CString message)
{
	CString event_level;
	CString event;
	CString text;
	CString where;
	CString triggered_command;
		

	int size = m_pChat->m_RemoteScript.GetSize();

	for (int i = 0; i < size; i++)
	{
		CString event_string = m_pChat->m_RemoteScript.GetAt(i);
		
		int pos		 = event_string.Find(':');
		event		 = event_string.Left(pos);
		event_string = event_string.Right(event_string.GetLength() - pos -1);


		// For text
		if (event == eventName && eventName == "TEXT")
		{
			pos  = event_string.Find(':');
			text = event_string.Left(pos);
			event_string = event_string.Right(event_string.GetLength() - pos -1);
			
			pos   = event_string.Find(':');
			where = event_string.Left(pos);
			event_string = event_string.Right(event_string.GetLength() - pos -1);

			triggered_command = event_string;

			triggered_command.Replace("$chan", channel);
			
			bool Flag = false;
			if (!text.IsEmpty())
			{
				if (text[0] == '*' && text[text.GetLength() - 1] == '*')
				{
					text.Remove('*');
					text.Remove('*');
					
					if(message.Find(text) != -1)
						Flag = true;
				}
				else if (text[0] == '*')
				{
					text.Remove('*');
					if (message.Right(text.GetLength()) == text)
						Flag = true;
				}
				else if (text[text.GetLength() - 1] == '*')
				{
					text.Remove('*');
					if (message.Left(text.GetLength()) == text)
						Flag = true;
				}
				if (Flag)
				{
					if (where == '#')
						AnalyzeAndSendCommand(triggered_command, pChannel, sender);
					else if(where == channel)
						AnalyzeAndSendCommand(triggered_command, pChannel, sender);
				}
			}
		}

		// For join
		if (event == eventName && eventName == "JOIN")
		{
			pos   = event_string.Find(':');
			where = event_string.Left(pos);
			event_string = event_string.Right(event_string.GetLength() - pos -1);

			triggered_command = event_string;
			
			triggered_command.Replace("$chan", channel);
			
			if (where == '#')
				AnalyzeAndSendCommand(triggered_command, pChannel, sender);
			else if(where == channel)
				AnalyzeAndSendCommand(triggered_command, pChannel, sender);
		}		
		
		// For part
		if (event == eventName && eventName == "PART")
		{
			pos   = event_string.Find(':');
			where = event_string.Left(pos);
			event_string = event_string.Right(event_string.GetLength() - pos -1);

			triggered_command = event_string;
			
			triggered_command.Replace("$chan", channel);
			
			if (where == '#')
				AnalyzeAndSendCommand(triggered_command, pChannel, sender);
			else if(where == channel)
				AnalyzeAndSendCommand(triggered_command, pChannel, sender);
		}

		// For kick
		if (event == eventName && eventName == "KICK")
		{
			pos   = event_string.Find(':');
			where = event_string.Left(pos);
			event_string = event_string.Right(event_string.GetLength() - pos -1);

			triggered_command = event_string;
			
			triggered_command.Replace("$chan", channel);
			
			if (where == '#')
				AnalyzeAndSendCommand(triggered_command, pChannel, sender);
			else if(where == channel)
				AnalyzeAndSendCommand(triggered_command, pChannel, sender);
		}

		// For quit
		if (event == eventName && eventName == "QUIT")
		{
			triggered_command = event_string;
			triggered_command.Replace("$chan", channel);
			AnalyzeAndSendCommand(triggered_command, pChannel, sender);
		}
	}
}


void CChatServer::AnalyzeAndSendCommand(CString Command, CChatRoom* pRoom, CString Who)
{
	CStringArray parameterArray;
	
	CString Message = Command;
	CString Input   = Command;
	CString Sender  = Who;

	CString aliasesCommand;
	CString value;
	CString command;
	CString temp;
	CString key;
	


	/*CString* pSender = (CString*)lParam;
	CString* pString = (CString*)wParam;
	if (pString)
		Message = *pString;
		Input = *pString;

	if (pSender)
		Sender=*pSender;*/


	int pos = Message.Find(' ');

	if (pos != -1)
	{
		key = Message.Left(Message.Find(' '));
		
		Message = Message.Right(Message.GetLength() - Message.Find(' ') - 1);

		while (Message.Find(' ')!= -1)
		{
			int pos = Message.Find(' ');
			CString msg = Message.Left(pos);
			if (!msg.IsEmpty())
				parameterArray.Add(msg);

			Message = Message.Right(Message.GetLength() - pos -1);
		}
			
		if (!Message.IsEmpty())
			parameterArray.Add(Message);
	}
	else
		key = Message;
	


	if (m_pChat->m_AliasMap.Lookup(key, aliasesCommand))
	{			
		do
		{
			int pos = aliasesCommand.Find(" | ");

			if (pos != -1)
			{
				value          = aliasesCommand.Left(pos);
				aliasesCommand = aliasesCommand.Right(aliasesCommand.GetLength() - pos - 3);
			}
			else
			{
				value          = aliasesCommand;
				aliasesCommand = "";
			}
			
			if (value.Find('$') != -1)
			{
				pos     = value.Find(' ');
				command = value.Left(pos);
				value   = value.Right(value.GetLength() - pos - 1);
			}
			else
			{
				command = value;
				value   = "";
			}


			
			if (!value.IsEmpty())
			{
				do
				{
					CString str = "";
				
					command += ' ';
					
					if(value.Find(' ') != -1)
					{
						pos   = value.Find(' ');
						temp  = value.Left(pos);
						value = value.Right(value.GetLength() - pos -1);
					}
					else
					{
						temp  = value;
						value = "";
					}


					if(temp.Mid(0, 1) == '#')
					{
						//str = '#';
						temp.Remove('#');
					}
					

					if(temp.Find('$') == -1)
					{
						command += str + temp;
					}
					else if (temp.Mid(0,2) == "$$")
					{
						CString parameter;
						temp.Remove('$');
						temp.Remove('$');

						CString aliasesString = ParseAliasesString(parameterArray, temp);
						
						if (aliasesString.IsEmpty())
							command = "";
						else
						{
							str += aliasesString;
						}
					}
					else if (temp.Mid(0,1) == '$')
					{
						if (temp.CompareNoCase("$nick") == 0)
						{
							str += Sender;
							temp = "";
						}
					
						if (temp.CompareNoCase("$chan") == 0)
						{
							str += pRoom->m_Name;
							temp = "";
						}
						
						if (temp.CompareNoCase("$me") == 0)
						{
							str += pRoom->m_pServer->m_Nick;
							temp = "";
						}

						temp.Remove('$');
						CString aliasesString = ParseAliasesString(parameterArray, temp);
						
						str += aliasesString;
					}

					command += str; 

				} while(!value.IsEmpty());
			}


			if (!command.IsEmpty())
				if (command[command.GetLength()-1] == '#')
				{
					command.Remove('#');
					command += pRoom->m_Name;
				}


			//TRACE("command is '%s'\n", command);
			pRoom->ServerCommand(command);
		

		} while(!aliasesCommand.IsEmpty());
	}
	else
	{
		do	
		{
			int pos = Input.Find(" | ");
			if (pos != -1)
			{
				command = Input.Left(pos);
				Input   = Input.Right(Input.GetLength() - pos - 3);
			}
			else
			{
				command = Input;
				Input   = "";
			}

			command.Replace("$me", pRoom->m_pServer->m_Nick);
			command.Replace("$nick", Sender);
			command.Replace("$chan", pRoom->m_Name);

			pRoom->ServerCommand(command);

		} while(!Input.IsEmpty());
	}
}	


CString CChatServer::ParseAliasesString(CStringArray& paramArray, CString aliasStr)
{
	CString str;
	int pos;


	pos = aliasStr.Find('-');

	if (pos != -1)
	{
		int param1, param2; 
		CString op;

		param1   = atoi(aliasStr.Left(pos))-1;
		aliasStr = aliasStr.Right(aliasStr.GetLength() - pos);
		op = aliasStr[0];
		aliasStr.Remove('-');

		if(!aliasStr.IsEmpty())
		{
			param2 = atoi(aliasStr);
			if (param1 >= 0 && param2 < paramArray.GetSize())
			{
				for (int i = param1; i < param2; i++)
				{
					str += paramArray.GetAt(i) + ' ';
					aliasStr = "";
				}
			}
		}
		else
		{
			for (int i = param1; i < paramArray.GetSize(); i++)
			{
				str += paramArray.GetAt(i) + ' ';
				aliasStr="";
			}
		}
	}
	else
	{
		int index = atoi(aliasStr) - 1;
		if (index < paramArray.GetSize() && index >= 0)
			 str += paramArray.GetAt(index);;
	}

	return str;
}