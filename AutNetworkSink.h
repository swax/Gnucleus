#pragma once


// {7468213E-010E-4ec6-A17D-642E909BA7EC}
static const IID IID_INetworkEvent =
{ 0x7468213e, 0x10e, 0x4ec6, { 0xa1, 0x7d, 0x64, 0x2e, 0x90, 0x9b, 0xa7, 0xec } };


class CGnucleusDoc;

class CAutNetworkSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CAutNetworkSink)

public:
	CAutNetworkSink(CGnucleusDoc* pDoc);
	virtual ~CAutNetworkSink();

	void OnChange(int NodeID);
	//void OnPacketIncoming(int NodeID, VARIANT* packet, int size, int ErrorCode, bool Local);
	void OnPacketIncoming2(int NetworkID, bool TCP, uint32 IP, int Port, VARIANT* packet, int size, bool Local, int ErrorCode);
	//void OnPacketOutgoing(int NodeID, VARIANT* packet, int size, bool Local);
	void OnPacketOutgoing2(int NetworkID, bool TCP, uint32 IP, int Port, VARIANT* packet, int size, bool Local);
	void OnAuthenticate(int NodeID);
	void OnChallenge(int NodeID, LPCTSTR Challenge);

	virtual void OnFinalRelease();

	CGnucleusDoc* m_pDoc;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

};

struct NetworkPacket
{
	int  Network;
	bool TCP;

	IP   Host;
	int  Port;
		
	bool Incoming;
	bool Local;
	int  ErrorCode;
	
	byte* Packet;
	int   PacketLength;

	NetworkPacket()
	{
		Network = 0;
		TCP		= true;

		Host.S_addr = 0;
		Port        = 0;
		
		Incoming  = false;
		Local     = false;
		ErrorCode = 0;
	
		Packet       = NULL;
		PacketLength = 0;
	};
};


