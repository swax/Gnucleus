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


#include "StdAfx.h"

#include "ChatControl.h"

#include "ChatPrefs.h"

CChatPrefs::CChatPrefs(CChatControl* pChat)
{
	m_pChat = pChat;

	m_Nick				= GetNick();
	m_AltNick			= GetNick();
	m_PrefNet			= "GnuDefault";
	m_DisableIdent		= false;

	m_InternalIRC       = false;
}

CChatPrefs::~CChatPrefs(void)
{
}

void CChatPrefs::LoadServers(CString ServerFile)
{
	char buffer[256];

	GetPrivateProfileString("LocalNetwork", "InternalIRC",		"0",	buffer, 256, ServerFile);
	m_InternalIRC = (0 != atoi(buffer));
	GetPrivateProfileString("LocalNetwork",	"InternalIRCAddr",	"",		buffer, 256, ServerFile);
	m_InternalIRCAddr = buffer;

	GetPrivateProfileString("Chat",	"Nick",    GetNick(),	buffer, 256, ServerFile);
	m_Nick = buffer;
	GetPrivateProfileString("Chat",	"AltNick", GetNick(),	buffer, 256, ServerFile);
	m_AltNick = buffer;
	GetPrivateProfileString("Chat",	"DisableIdent",			"0",	buffer, 256, ServerFile);
	m_DisableIdent = (0 != atoi(buffer));

	m_Nick.Replace("Gnuke", "gnclr");
	m_AltNick.MakeLower();
	m_AltNick.Replace("Gnuke", "gnclr");


	// IRC Servers
	GetPrivateProfileString("Chat Servers",	"Preferred Net", "GnuDefault",	buffer, 256, ServerFile);
	m_PrefNet = buffer;
	GetPrivateProfileString("Chat Servers",	"Preferred Channel", "",	buffer, 256, ServerFile);
	m_PrefChannel = buffer;

	if(m_PrefNet == "EFnet")
		m_PrefNet = "GnuDefault";


	CString KeyName;

	for(int i = 1; ; i++)
	{
		KeyName.Format("n%ld", i);

		if(GetPrivateProfileString( "Chat Servers", KeyName, "",	buffer, 256, ServerFile) > 0)
		{
			CString Parse(buffer);

			int pos = Parse.Find(":"),
				endpos = Parse.Find(";");
			

			if(pos != -1)
			{
				CString Network = Parse.Left(pos);

				pos++;
				CString Server  = Parse.Mid(pos, endpos - pos);

				AddServer(Network, Server);
			}
		}
		else
			break;
	}

}

void CChatPrefs::AddServer(CString Network, CString Server)
{
	ChatNetwork myNet;
	myNet.Name = Network;
	

	bool NetworkFound = false;

	for(int i = 0; i < m_ChatNets.size(); i++)
		if(m_ChatNets[i].Name == Network)
		{
			NetworkFound     = true;
			bool ServerFound = false;

			for(int j = 0; j < m_ChatNets[i].Servers.size(); j++)
				if(m_ChatNets[i].Servers[j] == Server)
					ServerFound = true;

			if(!ServerFound)
				m_ChatNets[i].Servers.push_back(Server);
		}

	if(!NetworkFound)
	{
		myNet.Servers.push_back(Server);
		m_ChatNets.push_back(myNet);
	}
}

void CChatPrefs::SaveServers(CString ServerFile)
{
	WritePrivateProfileString("LocalNetwork", "InternalIRC",		DWrdtoStr(m_InternalIRC),	ServerFile);
	WritePrivateProfileString("LocalNetwork", "InternalIRCAddr",	m_InternalIRCAddr,			ServerFile);

	WritePrivateProfileString("Chat",	"Nick",			m_Nick,			ServerFile);
	WritePrivateProfileString("Chat",	"AltNick",		m_AltNick,		ServerFile);
	WritePrivateProfileString("Chat",	"DisableIdent",	DWrdtoStr(m_DisableIdent),		ServerFile);


	// IRC Servers
	WritePrivateProfileString("Chat Servers",	"Preferred Net",		m_PrefNet,		ServerFile);
	WritePrivateProfileString("Chat Servers",	"Preferred Channel",	m_PrefChannel,	ServerFile);
	
	CString KeyName;

	int Item = 1;
	for (int i = 0; i < m_ChatNets.size(); i++)
		for(int j = 0; j < m_ChatNets[i].Servers.size(); j++)
		{
			KeyName.Format("n%ld", Item); 

			CString Line = m_ChatNets[i].Name + ":" + m_ChatNets[i].Servers[j] + ";";

			WritePrivateProfileString ("Chat Servers", KeyName, Line, ServerFile);
		
			Item++;
		}
}

void CChatPrefs::LoadChatAliases(CString ConfigFile)
{
	CFile File;

	
	if(File.Open(ConfigFile, CFile::modeRead))
	{
		char* buffer;
		buffer = new char[File.GetLength() + 1];

		UINT byteread = File.Read(buffer, File.GetLength());
		buffer[File.GetLength()] = '\0';

		CString Data = buffer;
		
		// Parse the data
		CString ParseList;
		
		while (Data.Find("\r\n") != -1)
		{
			int pos = Data.Find("\r\n");
			
			CString LineText;
			LineText = Data.Left(pos + 2);
			Data = Data.Right(Data.GetLength() - pos - 2);
		
			int epos = LineText.Find('=');
			if (epos != -1)
			{
				LineText = LineText.Right(LineText.GetLength() - epos - 1);
				ParseList += LineText;
			}
		}
		
		// Put list of aliases in map
		m_pChat->m_AliasMap.RemoveAll();
	
		int pos  = ParseList.Find("\r\n");
		int lpos = 0;
		CString command;

		while (pos != -1)
		{
			command = ParseList.Left(pos);
			ParseList = ParseList.Right(ParseList.GetLength() - pos - 2);
			pos = ParseList.Find("\r\n");		

			if (command.Find(' ') != -1)
				m_pChat->m_AliasMap.SetAt(command.Left(command.Find(' ')), command.Right(command.GetLength() - command.Find(' ') - 1));
		}
		
		delete [] buffer;
		File.Abort();
	}
}

void CChatPrefs::SaveChatAliases(CString ConfigFile)
{

}

void CChatPrefs::LoadChatScripts(CString ConfigFile)
{
	CFile File;

	if(File.Open(ConfigFile, CFile::modeRead))
	{
		
		char* buffer;
		buffer = new char[File.GetLength() + 1];

		UINT byteread = File.Read(buffer, File.GetLength());
		buffer[File.GetLength()] = '\0';

		CString Data = buffer;
		while (Data.Find("\r\n") != -1)
		{
			int pos = Data.Find("\r\n");
			CString line = Data.Left(pos);
			
			if(!line.IsEmpty())
				m_pChat->m_RemoteScript.Add(line);
			
			Data = Data.Right(Data.GetLength() - pos - 2);
		}
	
		delete [] buffer;
		File.Abort();

	}
}

void CChatPrefs::SaveChatScripts(CString ConfigFile)
{

}

void CChatPrefs::LoadChatHotlist(CString ConfigFile)
{
	/*TCHAR buff[10];
	GetPrivateProfileString("notifycounter", "KeyCounter","0",buff, 10, ConfigFile);

	CString szBuf = buff;
	
	int key = atoi(buff);

	for (int i = 0; i < key; i++)
	{
		TCHAR str[100];
		CString szKey;
		szKey.Format("nick%d",i);

		GetPrivateProfileString("notify", szKey, "0", str, 100, ConfigFile);
		CString nick = str;
		if (nick != "0")
		{
			NickIsOn* Nick = new NickIsOn;
			Nick->szNick = nick;
			Nick->bStatus = 0;
			m_HotList.Add(Nick);
		}
	}*/
}

void CChatPrefs::SaveChatHotlist(CString ConfigFile)
{
	/*WritePrivateProfileString("notify", NULL, NULL, ConfigFile);
  
	for (int i = 0; i < m_HotList.GetSize(); i++)
	{
		CString szKey;
		szKey.Format("nick%d", i);
		NickIsOn* Nick = m_HotList.GetAt(i);
		WritePrivateProfileString("notify", szKey, Nick->szNick, ConfigFile);
		delete Nick;
  	}

	m_HotList.RemoveAll();
	CString szCounter="";
	szCounter.Format("%d", i);
	WritePrivateProfileString("notifycounter", "KeyCounter", szCounter, ConfigFile);*/
}

CString CChatPrefs::GetNick()
{
	CString Nick = "Gnuwt";
		
	Nick += DWrdtoStr( (rand() % 10000 + 0));

	return Nick;
}