// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutCore wrapper class

class CAutCore : public COleDispatchDriver
{
public:
	CAutCore(){} // Calls COleDispatchDriver default constructor
	CAutCore(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutCore(const CAutCore& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// ICore methods
public:
	void Connect()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Disconnect()
	{
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH GetINetwork()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetICache()
	{
		LPDISPATCH result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetIShare()
	{
		LPDISPATCH result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL IsConnecting()
	{
		BOOL result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetIPrefs()
	{
		LPDISPATCH result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetISearch()
	{
		LPDISPATCH result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetIDownload()
	{
		LPDISPATCH result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetIUpload()
	{
		LPDISPATCH result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	DATE GetUptime()
	{
		DATE result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_DATE, (void*)&result, NULL);
		return result;
	}
	long GetBytesPerSecDown()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetBytesPerSecUp()
	{
		long result;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetCoreVersion()
	{
		CString result;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	VARIANT GetCoreCredits()
	{
		VARIANT result;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	CString GetCoreLicense()
	{
		CString result;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetIUpdate()
	{
		LPDISPATCH result;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetIMeta()
	{
		LPDISPATCH result;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Connect2(long NetworkID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NetworkID);
	}
	void Disconnect2(long NetworkID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NetworkID);
	}
	BOOL IsConnecting2(long NetworkID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, NetworkID);
		return result;
	}
	DATE GetUptime2(long NetworkID)
	{
		DATE result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_DATE, (void*)&result, parms, NetworkID);
		return result;
	}
	LPDISPATCH GetIChat()
	{
		LPDISPATCH result;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// ICore properties
public:
CString GetRunPath()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}
void SetRunPath(CString propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}
CString GetClientName()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}
void SetClientName(CString propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}
CString GetClientVersion()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}
void SetClientVersion(CString propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}
CString GetClientCode()
{
	CString result;
	GetProperty(0x16, VT_BSTR, (void*)&result);
	return result;
}
void SetClientCode(CString propVal)
{
	SetProperty(0x16, VT_BSTR, propVal);
}

};
