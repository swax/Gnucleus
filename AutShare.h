// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutShare wrapper class

class CAutShare : public COleDispatchDriver
{
public:
	CAutShare(){} // Calls COleDispatchDriver default constructor
	CAutShare(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutShare(const CAutShare& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IShare methods
public:
	VARIANT GetFileIDs()
	{
		VARIANT result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long GetFileIndex(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	CString GetFileDir(long FileID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID);
		return result;
	}
	CString GetFileName(long FileID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID);
		return result;
	}
	long GetFileSize(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	long GetFileMatches(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	long GetFileUploads(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	void StartHashing()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void StopHashing()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL IsEverythingHashed()
	{
		BOOL result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL IsHashingStopped()
	{
		BOOL result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString GetFileHash(long FileID, long HashID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID, HashID);
		return result;
	}
	void StopSharingFile(long FileID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileID);
	}
	VARIANT GetFileKeywords(long FileID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, FileID);
		return result;
	}
	VARIANT GetFileAltLocs(long FileID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, FileID);
		return result;
	}
	VARIANT GetSharedDirIDs()
	{
		VARIANT result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	CString GetDirName(long DirID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DirID);
		return result;
	}
	BOOL GetDirRecursive(long DirID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DirID);
		return result;
	}
	long GetDirFileCount(long DirID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DirID);
		return result;
	}
	void SetSharedDirs(VARIANT * pDirPaths)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pDirPaths);
	}
	BOOL IsLoading()
	{
		BOOL result;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long GetFileCount()
	{
		long result;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetFileMetaID(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	CString GetFileAttributeValue(long FileID, long AttributeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID, AttributeID);
		return result;
	}
	void SetFileAttributeValue(long FileID, long AttributeID, LPCTSTR Value)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileID, AttributeID, Value);
	}
	long GetTotalFileSize()
	{
		long result;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetFileMetaID(long FileID, long MetaID)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileID, MetaID);
	}
	unsigned __int64 GetFileSize2(long FileID)
	{
		unsigned __int64 result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_UI8, (void*)&result, parms, FileID);
		return result;
	}
	unsigned __int64 GetTotalFileSize2()
	{
		unsigned __int64 result;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_UI8, (void*)&result, NULL);
		return result;
	}

	// IShare properties
public:
long GetHashSpeed()
{
	long result;
	GetProperty(0x1b, VT_I4, (void*)&result);
	return result;
}
void SetHashSpeed(long propVal)
{
	SetProperty(0x1b, VT_I4, propVal);
}

};
