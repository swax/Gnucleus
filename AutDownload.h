// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutDownload wrapper class

class CAutDownload : public COleDispatchDriver
{
public:
	CAutDownload(){} // Calls COleDispatchDriver default constructor
	CAutDownload(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutDownload(const CAutDownload& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDownload methods
public:
	VARIANT GetDownloadIDs()
	{
		VARIANT result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long GetStatus(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	BOOL IsCompleted(long DownloadID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DownloadID);
		return result;
	}
	CString GetName(long DownloadID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetBytesCompleted(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetFileLength(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetSourceCount(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	BOOL IsSearching(long DownloadID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DownloadID);
		return result;
	}
	BOOL IsRetrying(long DownloadID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetCoolingCount(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetActiveSourceCount(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	CString GetReasonClosed(long DownloadID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetBytesPerSec(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetSecETA(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetSourcePos(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	void RemoveCompleted()
	{
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ForceStart(long DownloadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID);
	}
	void Stop(long DownloadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID);
	}
	void Remove(long DownloadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID);
	}
	void RunFile(long DownloadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID);
	}
	void ReSearch(long DownloadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID);
	}
	CString GetHash(long DownloadID, long HashID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID, HashID);
		return result;
	}
	VARIANT GetSourceIDs(long DownloadID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, DownloadID);
		return result;
	}
	unsigned long GetSourceIP(long DownloadID, long SourceID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	long GetSourcePort(long DownloadID, long SourceID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	CString GetSourceName(long DownloadID, long SourceID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	long GetSourceSpeed(long DownloadID, long SourceID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	CString GetSourceStatusStr(long DownloadID, long SourceID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	CString GetSourceVendor(long DownloadID, long SourceID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	CString GetSourceHandshake(long DownloadID, long SourceID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	VARIANT GetChunkIDs(long DownloadID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, DownloadID);
		return result;
	}
	long GetChunkStart(long DownloadID, long ChunkID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, ChunkID);
		return result;
	}
	long GetChunkCompleted(long DownloadID, long ChunkID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, ChunkID);
		return result;
	}
	long GetChunkSize(long DownloadID, long ChunkID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, ChunkID);
		return result;
	}
	long GetChunkFamily(long DownloadID, long ChunkID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, ChunkID);
		return result;
	}
	long GetSourceBytesPerSec(long DownloadID, long SourceID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID, SourceID);
		return result;
	}
	long DownloadFile(LPCTSTR Name, long size, long HashID, LPCTSTR Hash)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Name, size, HashID, Hash);
		return result;
	}
	CString GetFilePath(long DownloadID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID);
		return result;
	}
	void AddSource(long DownloadID, long NetworkID, LPCTSTR URL)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID, NetworkID, URL);
	}
	void Proxy(long DownloadID, BOOL Enabled, LPCTSTR Default)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID, Enabled, Default);
	}
	long GetMetaID(long DownloadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DownloadID);
		return result;
	}
	CString GetAttributeValue(long DownloadID, long AttributeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DownloadID, AttributeID);
		return result;
	}
	void SetAttributeValue(long DownloadID, long AttributeID, LPCTSTR Value)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID, AttributeID, Value);
	}
	void SetMetaID(long DownloadID, long MetaID)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID, MetaID);
	}
	void OverrideDownloadPath(long DownloadID, LPCTSTR Dir)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DownloadID, Dir);
	}
	unsigned __int64 GetFileLength2(long DownloadID)
	{
		unsigned __int64 result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_UI8, (void*)&result, parms, DownloadID);
		return result;
	}
	unsigned __int64 GetBytesCompleted2(long DownloadID)
	{
		unsigned __int64 result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_UI8, (void*)&result, parms, DownloadID);
		return result;
	}
	unsigned __int64 GetChunkStart2(long DownloadID, long ChunkID)
	{
		unsigned __int64 result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_UI8, (void*)&result, parms, DownloadID, ChunkID);
		return result;
	}
	long DownloadFile2(LPCTSTR Name, unsigned __int64 size, long HashID, LPCTSTR Hash)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_UI8 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Name, size, HashID, Hash);
		return result;
	}

	// IDownload properties
public:

};
