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

#include "ChatPrefs.h"
#include "ChatControl.h"

#include "ChatIdentd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CChatIdentd::CChatIdentd(CChatControl* pChat)
{
	m_pChat = pChat;
	
	m_DataSending = false;

	m_SecsAlive = 0;
}

CChatIdentd::~CChatIdentd()
{
	// Flush receive buffer
	byte pBuff[4096];
	while(Receive(pBuff, 4096) > 0)
		;

	if(m_hSocket != INVALID_SOCKET)
		AsyncSelect(0);
}


#if 0
BEGIN_MESSAGE_MAP(CChatIdentd, CAsyncSocket)
	//{{AFX_MSG_MAP(CChatIdentd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


/////////////////////////////////////////////////////////////////////////////
// CChatIdentd member functions

void CChatIdentd::OnReceive(int nErrorCode) 
{
	// Receive Data
	DWORD BuffLength = Receive(m_pBuff, 4096);

	// Check for errors
	switch (BuffLength)
	{
	case 0:
		Close();
		return;
		break;
	case SOCKET_ERROR:
		Close();
		return;
		break;
	}

	m_pBuff[BuffLength]  = '\0';
	CString Header(m_pBuff);

	// We are assuming here that the server is sending valid stuff

	Header = Header + " : USERID : UNIX :" + m_pChat->m_pPrefs->m_Nick + "\r\n";

	// Server -> 1025, 6667
	// Client -> 1025 , 6667 : USERID : UNIX :gnucleus

	Send(Header, Header.GetLength());
	
	CAsyncSocket::OnReceive(nErrorCode);
}

int CChatIdentd::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	int Command = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
	
	if(Command != SOCKET_ERROR)
		m_DataSending = true;

	return Command;
}

void CChatIdentd::OnSend(int nErrorCode) 
{
	m_DataSending = false;
	
	CAsyncSocket::OnSend(nErrorCode);
}

void CChatIdentd::Close() 
{
	if(m_hSocket != INVALID_SOCKET)
	{
		AsyncSelect(0);
		ShutDown(1);

		CAsyncSocket::Close();
	}
}