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
	license to your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/

#include "stdafx.h"
#include "Gnucleus.h"
#include "AutChatSink.h"


// CAutChatSink

IMPLEMENT_DYNAMIC(CAutChatSink, CCmdTarget)
CAutChatSink::CAutChatSink(CGnucleusDoc* pDoc)
{
	m_pDoc = pDoc;

	EnableAutomation();
}

CAutChatSink::~CAutChatSink()
{
}

void CAutChatSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAutChatSink, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAutChatSink, CCmdTarget)
	DISP_FUNCTION_ID(CAutChatSink, "OnRecvPrivateMessage", 1, OnRecvPrivateMessage, VT_EMPTY, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CAutChatSink, CCmdTarget)
	INTERFACE_PART(CAutChatSink, IID_IChatEvent, Dispatch)
END_INTERFACE_MAP()

void CAutChatSink::OnRecvPrivateMessage(LPCTSTR Address, LPCTSTR Message)
{
	AfxMessageBox(Message);
}
