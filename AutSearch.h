// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutSearch wrapper class

class CAutSearch : public COleDispatchDriver
{
public:
	CAutSearch(){} // Calls COleDispatchDriver default constructor
	CAutSearch(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutSearch(const CAutSearch& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// ISearch methods
public:
	long StartSearch(LPCTSTR Query)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Query);
		return result;
	}
	void EndSearch(long SearchID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SearchID);
	}
	void RefineSearch(long SearchID, LPCTSTR RefinedQuery)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SearchID, RefinedQuery);
	}
	void SetFiltering(long SearchID, BOOL Enabled)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SearchID, Enabled);
	}
	void FilterSize(long SearchID, long Mode, long Value)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SearchID, Mode, Value);
	}
	void FilterSpeed(long SearchID, long Mode, long Value)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SearchID, Mode, Value);
	}
	void PauseSearch(long SearchID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SearchID);
	}
	unsigned long CountGoodResults(long SearchID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, SearchID);
		return result;
	}
	unsigned long CountTotalResults(long SearchID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, SearchID);
		return result;
	}
	CString GetResultName(long SearchID, unsigned long ResultID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	unsigned long GetResultSize(long SearchID, unsigned long ResultID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	unsigned long GetResultSpeed(long SearchID, unsigned long ResultID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	long GetResultHostCount(long SearchID, unsigned long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	CString GetResultHash(long SearchID, unsigned long ResultID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	VARIANT GetResultIDs(long SearchID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SearchID);
		return result;
	}
	long DownloadResult(long SearchID, unsigned long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	VARIANT GetHostIDs(long SearchID, unsigned long ResultID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	unsigned long GetHostIP(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetHostPort(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetHostSpeed(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetHostDistance(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	BOOL GetHostFirewall(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	BOOL GetHostStable(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	BOOL GetHostBusy(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	CString GetHostVendor(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	VARIANT GetHostMetadata(long SearchID, unsigned long ResultID, unsigned long HostID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetResultState(long SearchID, unsigned long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}

	// ISearch properties
public:

};
