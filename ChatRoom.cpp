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

#include "ChatControl.h"
#include "ChatServer.h"
#include "ChatRoom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CChatRoom::CChatRoom(CChatServer *pServer)
{
	m_pServer = pServer;
	m_pChat   = m_pServer->m_pChat;

	m_UserLock = true;

	m_RoomType = 0;
}

CChatRoom::~CChatRoom()
{
	m_UserList.clear();
}

void CChatRoom::WriteToDisplay(ChatLine pLine)
{
	WriteToDisplay(pLine.Text, pLine.Color);
}

void CChatRoom::WriteToDisplay(CString Line, COLORREF Color)
{
	Line.Replace("\r\n", "\n");

	ChatLine Message;
	Message.Text  = Line;
	Message.Color = Color;

	if(m_Display.size() > CHAT_MAXLINES)
		m_Display.pop_front();

	m_Display.push_back(Message);
	m_Pending = Message;

	m_pChat->ChatMessage(CHAT_UPDATE, (LPARAM) this);
}

int CChatRoom::FindUser(CString User)
{
	int pos = -1;

	for(int i = 0; i < m_UserList.size(); i++)
	{
		CString Member = m_UserList[i];
		Member.Remove('@');
		Member.Remove('+');

		if( Member.CompareNoCase( User ) == 0)
			pos = i;
	}

	return pos;
}

void CChatRoom::AddUser(CString User)
{
	m_UserList.push_back(User);
}

void CChatRoom::RemoveUser(CString User)
{
	int pos = FindUser(User);

	if(pos != -1)
	{	
		int i = 0;

		std::vector<CString>::iterator itUser;
		for(itUser = m_UserList.begin(); itUser != m_UserList.end(); itUser++, i++)
			if(pos == i)
			{
				m_UserList.erase(itUser);
				break;
			}
	}
}

void CChatRoom::RenameUser(CString OldName, CString NewName)
{
	int pos = FindUser(OldName);

	if(pos != -1)
	{
		OldName = m_UserList[pos];
		
		if(OldName.Find('@') == 0)
			NewName = '@' + NewName;
		if(OldName.Find('+') == 0)
			NewName = '+' + NewName;

		m_UserList[pos] = NewName;
	}
}

void CChatRoom::Say(CString Message)
{
	if(m_RoomType == ROOM_SERVER)
		return;

	// Send Command
	CString Command = "PRIVMSG " + m_Name + " :" + Message + "\r\n";
	m_pServer->Send(Command, Command.GetLength());

	// Display it
	CString Display = "<" + m_pServer->m_Nick + "> " + Message + "\n";
	WriteToDisplay(Display, COLOR_BLACK);
}

void CChatRoom::ServerCommand(CString Message)
{
	CString Command;
	int nPos = Message.Find(' ');

	if( nPos != -1)
	{
		Command = Message.Left( Message.Find(' '));
	}
	else
		Command = Message;

	if( Command.CompareNoCase("/MSG") == 0)
	{
		// Need to add ':' after nick name for private message to work
		nPos = Message.Find(' ', nPos + 1);
		Message.Insert(nPos + 1, ':');

		Message.Replace(Command, "/PRIVMSG");
	}

	if( Command.CompareNoCase("/QUIT") == 0)
	{
		// Need to add ':' after command
		Message.Insert(nPos + 1, ':');
	}

	if( Command.CompareNoCase("/LEAVE") == 0)
		Message.Replace(Command, "/PART");

	if( Command.CompareNoCase("/NICK") == 0)
		m_pServer->m_Nick = Message.Mid(nPos + 1);

	if( Command.CompareNoCase("/ME") == 0)
	{
		// Action message
		CString Text = Message.Mid(nPos + 1);
		Message = "/PRIVMSG " + m_Name + " :" + (char) 0x01 + "ACTION " + Text + (char) 0x01;

		// Display it
		CString Display = "* " + m_pServer->m_Nick + " " + Text + "\n";
		WriteToDisplay(Display, COLOR_BLACK);
	}

	// /ctcp swabby VERSION
	if( Command.CompareNoCase("/CTCP") == 0)
	{
		CString Text = Message.Mid(nPos + 1);
		CString User, CTCP;

		nPos = Text.Find(' ');
		if(nPos)
		{
			User = Text.Left(nPos);	
			CTCP = Text.Mid(nPos + 1);

			Message = "/PRIVMSG " + User + " :" + (char) 0x01 + CTCP + (char) 0x01;
		}
		else 
			return;
	}

	Message = Message.Mid(1);
	m_pServer->Send(Message + "\r\n", Message.GetLength() + 2);		
}

