// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutNetwork wrapper class

class CAutNetwork : public COleDispatchDriver
{
public:
	CAutNetwork(){} // Calls COleDispatchDriver default constructor
	CAutNetwork(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutNetwork(const CAutNetwork& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// INetwork methods
public:
	VARIANT GetNodeIDs()
	{
		VARIANT result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long GetNodeState(long NodeID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NodeID);
		return result;
	}
	long ClientMode()
	{
		long result;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetNodeMode(long NodeID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NodeID);
		return result;
	}
	unsigned long GetNodeIP(long NodeID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, NodeID);
		return result;
	}
	long GetNodePort(long NodeID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NodeID);
		return result;
	}
	unsigned long GetNodeBytesUp(long NodeID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, NodeID);
		return result;
	}
	unsigned long GetNodeBytesDown(long NodeID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, NodeID);
		return result;
	}
	void ConnectNode(LPCTSTR Host, unsigned long Port)
	{
		static BYTE parms[] = VTS_BSTR VTS_UI4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Host, Port);
	}
	void RemoveNode(long NodeID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID);
	}
	long GetNormalConnectedCount()
	{
		long result;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetNodeHandshake(long NodeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, NodeID);
		return result;
	}
	DATE GetNodeConnectTime(long NodeID)
	{
		DATE result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_DATE, (void*)&result, parms, NodeID);
		return result;
	}
	unsigned long GetLocalIP()
	{
		unsigned long result;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	unsigned long GetLocalPort()
	{
		unsigned long result;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	VARIANT GetLanNodeIDs()
	{
		VARIANT result;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void LanModeOn()
	{
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LanModeOff()
	{
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString GetLanNodeName(long LanNodeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, LanNodeID);
		return result;
	}
	long GetLanNodeLeaves(long LanNodeID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, LanNodeID);
		return result;
	}
	void GetNodePacketsPing(long NodeID, long * Good, long * Total)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Good, Total);
	}
	void GetNodePacketsPong(long NodeID, long * Good, long * Total)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Good, Total);
	}
	void GetNodePacketsQuery(long NodeID, long * Good, long * Total)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Good, Total);
	}
	void GetNodePacketsQueryHit(long NodeID, long * Good, long * Total)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Good, Total);
	}
	void GetNodePacketsPush(long NodeID, long * Good, long * Total)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Good, Total);
	}
	void GetNodePacketsTotal(long NodeID, long * Good, long * Total)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Good, Total);
	}
	unsigned long GetNodeBytesDropped(long NodeID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, NodeID);
		return result;
	}
	double GetNodePacketsDown(long NodeID)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_R8, (void*)&result, parms, NodeID);
		return result;
	}
	double GetNodePacketsUp(long NodeID)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_R8, (void*)&result, parms, NodeID);
		return result;
	}
	double GetNodePacketsDropped(long NodeID)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_R8, (void*)&result, parms, NodeID);
		return result;
	}
	long GetLocalSpeed()
	{
		long result;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void ForceUltrapeer(BOOL Enabled)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enabled);
	}
	void JoinLan(LPCTSTR LanName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LanName);
	}
	VARIANT GetChildNodeIDs()
	{
		VARIANT result;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SendChallenge(long NodeID, LPCTSTR Challenge, LPCTSTR Answer)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Challenge, Answer);
	}
	void AnswerChallenge(long NodeID, LPCTSTR Answer)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NodeID, Answer);
	}

	// INetwork properties
public:

};
