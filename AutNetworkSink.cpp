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
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "AutNetwork.h"

#include "ViewConnect.h"
#include "ViewStatistics.h"

#include "AutNetworkSink.h"


IMPLEMENT_DYNAMIC(CAutNetworkSink, CCmdTarget)
CAutNetworkSink::CAutNetworkSink(CGnucleusDoc* pDoc)
{
	m_pDoc = pDoc;

	EnableAutomation();
}

CAutNetworkSink::~CAutNetworkSink()
{
}


void CAutNetworkSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAutNetworkSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAutNetworkSink, CCmdTarget)
	DISP_FUNCTION_ID(CAutNetworkSink, "OnChange", 1, OnChange, VT_EMPTY, VTS_I4)
	//DISP_FUNCTION_ID(CAutNetworkSink, "OnPacketIncoming", 2, OnPacketIncoming, VT_EMPTY, VTS_I4 VTS_PVARIANT VTS_I4 VTS_I4 VTS_BOOL)
	//DISP_FUNCTION_ID(CAutNetworkSink, "OnPacketOutgoing", 3, OnPacketOutgoing, VT_EMPTY, VTS_I4 VTS_PVARIANT VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CAutNetworkSink, "OnAuthenticate", 4, OnAuthenticate, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CAutNetworkSink, "OnChallenge", 5, OnChallenge, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CAutNetworkSink, "OnPacketIncoming2", 6, OnPacketIncoming2, VT_EMPTY, VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_PVARIANT VTS_I4 VTS_BOOL VTS_I4)
	DISP_FUNCTION_ID(CAutNetworkSink, "OnPacketOutgoing2", 7, OnPacketOutgoing2, VT_EMPTY, VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_PVARIANT VTS_I4 VTS_BOOL)
END_DISPATCH_MAP() 


BEGIN_INTERFACE_MAP(CAutNetworkSink, CCmdTarget)
	INTERFACE_PART(CAutNetworkSink, IID_INetworkEvent, Dispatch)
END_INTERFACE_MAP()


// CAutNetworkSink message handlers

void CAutNetworkSink::OnChange(int NodeID)
{
	if(m_pDoc->m_pViewConnect)
		((CViewConnect*) CWnd::FromHandle(m_pDoc->m_pViewConnect))->OnSockUpdate();

	for(int i = 0; i < m_pDoc->m_pViewStatistics.size(); i++)
		((CViewStatistics*) CWnd::FromHandle(m_pDoc->m_pViewStatistics[i]))->OnSockUpdate();
}

//void CAutNetworkSink::OnPacketIncoming(int NodeID, VARIANT* packet, int size, int ErrorCode, bool Local)
//{
//	if((packet->vt & VT_ARRAY) == 0)
//		return;
//
//	if((packet->vt & VT_UI1) == 0)
//		return;
//
//	SAFEARRAY* psa = packet->parray;
//
//	byte* bArray;
//	SafeArrayAccessData(psa, reinterpret_cast<void**> (&bArray));
//
//	for(int i = 0; i < m_pDoc->m_pViewStatistics.size(); i++)
//		((CViewStatistics*) CWnd::FromHandle(m_pDoc->m_pViewStatistics[i]))->OnPacketIncoming(NodeID, (packet_Header*) bArray, size, ErrorCode, Local);
//
//	SafeArrayUnaccessData(psa);
//	
//}

void CAutNetworkSink::OnPacketIncoming2(int NetworkID, bool TCP, uint32 IP, int Port, VARIANT* packet, int size, bool Local, int ErrorCode)
{
	if((packet->vt & VT_ARRAY) == 0)
		return;

	if((packet->vt & VT_UI1) == 0)
		return;

	SAFEARRAY* psa = packet->parray;

	byte* bArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&bArray));

	NetworkPacket InPacket;
	InPacket.Network      = NetworkID;
	InPacket.TCP          = TCP;
	InPacket.Host.S_addr  = IP;
	InPacket.Port		  = Port;
	InPacket.Incoming	  = true;
	InPacket.Local		  = Local;
	InPacket.Packet		  = bArray;
	InPacket.PacketLength = size;
	InPacket.ErrorCode    = ErrorCode;

	for(int i = 0; i < m_pDoc->m_pViewStatistics.size(); i++)
	{
		((CViewStatistics*) CWnd::FromHandle(m_pDoc->m_pViewStatistics[i]))->OnPacketIncoming(InPacket);
	}

	SafeArrayUnaccessData(psa);
}
//
//void CAutNetworkSink::OnPacketOutgoing(int NodeID, VARIANT* packet, int size, bool Local)
//{
//	
//	if((packet->vt & VT_ARRAY) == 0)
//		return;
//
//	if((packet->vt & VT_UI1) == 0)
//		return;
//
//	SAFEARRAY* psa = packet->parray;
//
//	byte* bArray;
//	SafeArrayAccessData(psa, reinterpret_cast<void**> (&bArray));
//
//
//	for(int i = 0; i < m_pDoc->m_pViewStatistics.size(); i++)
//		((CViewStatistics*) CWnd::FromHandle(m_pDoc->m_pViewStatistics[i]))->OnPacketOutgoing(NodeID, (packet_Header*) bArray, size, Local);
//
//	SafeArrayUnaccessData(psa);
//	
//}

void CAutNetworkSink::OnPacketOutgoing2(int NetworkID, bool TCP, uint32 IP, int Port, VARIANT* packet, int size, bool Local)
{
	if((packet->vt & VT_ARRAY) == 0)
		return;

	if((packet->vt & VT_UI1) == 0)
		return;

	SAFEARRAY* psa = packet->parray;

	byte* bArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&bArray));

	NetworkPacket OutPacket;
	OutPacket.Network      = NetworkID;
	OutPacket.TCP          = TCP;
	OutPacket.Host.S_addr  = IP;
	OutPacket.Port		   = Port;
	OutPacket.Incoming	   = false;
	OutPacket.Local		   = Local;
	OutPacket.Packet	   = bArray;
	OutPacket.PacketLength = size;
	OutPacket.ErrorCode    = 0;

	for(int i = 0; i < m_pDoc->m_pViewStatistics.size(); i++)
		((CViewStatistics*) CWnd::FromHandle(m_pDoc->m_pViewStatistics[i]))->OnPacketOutgoing(OutPacket);

	SafeArrayUnaccessData(psa);
	
}
// Us challenging the remote host
void CAutNetworkSink::OnAuthenticate(int NodeID)
{
	// SAMPLE, MAKE SURE TO DELETE

	/*srand( (unsigned) time(NULL) );

	int nChallenge = rand() % 14 + 0;
	int nAnswer    = 14 - nChallenge;

	m_pDoc->m_autNetwork->SendChallenge(NodeID, DWrdtoStr(nChallenge), DWrdtoStr(nAnswer) );*/
}


// Remote host challenging us
void CAutNetworkSink::OnChallenge(int NodeID, LPCTSTR Challenge)
{
	// SAMPLE, MAKE SURE TO DELETE

	/*int nChallenge = atoi(Challenge);
	int nAnswer    = 14 - nChallenge;

	m_pDoc->m_autNetwork->AnswerChallenge(NodeID, DWrdtoStr(nAnswer) );*/
}

