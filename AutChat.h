// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutChat wrapper class

class CAutChat : public COleDispatchDriver
{
public:
	CAutChat(){} // Calls COleDispatchDriver default constructor
	CAutChat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutChat(const CAutChat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IChat methods
public:
	void SendDirectMessage(LPCTSTR Address, LPCTSTR Message)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Address, Message);
	}

	// IChat properties
public:

};
