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
	long CountGoodResults(long SearchID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID);
		return result;
	}
	long CountTotalResults(long SearchID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID);
		return result;
	}
	CString GetResultName(long SearchID, long ResultID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	long GetResultSize(long SearchID, long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	long GetResultSpeed(long SearchID, long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	long GetResultHostCount(long SearchID, long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	CString GetResultHash(long SearchID, long ResultID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
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
	long DownloadResult(long SearchID, long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	VARIANT GetHostIDs(long SearchID, long ResultID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	long GetHostIP(long SearchID, long ResultID, long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetHostPort(long SearchID, long ResultID, long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetHostSpeed(long SearchID, long ResultID, long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetHostDistance(long SearchID, long ResultID, long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	BOOL GetHostFirewall(long SearchID, long ResultID, long HostID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	BOOL GetHostStable(long SearchID, long ResultID, long HostID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	BOOL GetHostBusy(long SearchID, long ResultID, long HostID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	CString GetHostVendor(long SearchID, long ResultID, long HostID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	VARIANT GetHostExtended(long SearchID, long ResultID, long HostID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	long GetResultState(long SearchID, long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	long StartMetaSearch(LPCTSTR Query, long MetaID, VARIANT * AttributeList)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Query, MetaID, AttributeList);
		return result;
	}
	long StartHashSearch(LPCTSTR Query, LPCTSTR Hash)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Query, Hash);
		return result;
	}
	long GetResultMetaID(long SearchID, long ResultID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID);
		return result;
	}
	CString GetResultAttributeValue(long SearchID, long ResultID, long AttributeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID, AttributeID);
		return result;
	}
	long GetHostMetaID(long SearchID, long ResultID, long HostID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SearchID, ResultID, HostID);
		return result;
	}
	CString GetHostAttributeValue(long SearchID, long ResultID, long HostID, long AttributeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, SearchID, ResultID, HostID, AttributeID);
		return result;
	}

	// ISearch properties
public:

};
