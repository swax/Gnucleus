// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutCache wrapper class

class CAutCache : public COleDispatchDriver
{
public:
	CAutCache(){} // Calls COleDispatchDriver default constructor
	CAutCache(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutCache(const CAutCache& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// ICache methods
public:
	void LoadCache(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void LoadUltraCache(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void LoadWebCache(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void AddWebCache(LPCTSTR WebAddress)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, WebAddress);
	}
	void SaveCache(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void SaveUltraCache(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void SaveWebCache(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	long GetNodeCacheSize()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetNodeCacheMaxSize()
	{
		long result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetUltraNodeCacheSize()
	{
		long result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetUltraNodeCacheMaxSize()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetWebCacheSize()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetWebCacheMaxSize()
	{
		long result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void AddNode(LPCTSTR HostPort, BOOL SuperNode)
	{
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, HostPort, SuperNode);
	}
	void SeedWebCache(LPCTSTR WebAddress)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, WebAddress);
	}
	void TryWebCache(LPCTSTR Network)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Network);
	}

	// ICache properties
public:

};
