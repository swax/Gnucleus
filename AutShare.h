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
	unsigned long GetFileIndex(unsigned long FileID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, FileID);
		return result;
	}
	CString GetFileDir(unsigned long FileID)
	{
		CString result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID);
		return result;
	}
	CString GetFileName(unsigned long FileID)
	{
		CString result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID);
		return result;
	}
	unsigned long GetFileSize(unsigned long FileID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, FileID);
		return result;
	}
	unsigned long GetFileMatches(unsigned long FileID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, FileID);
		return result;
	}
	unsigned long GetFileUploads(unsigned long FileID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, FileID);
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
	CString GetFileHash(unsigned long FileID)
	{
		CString result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID);
		return result;
	}
	void StopSharingFile(unsigned long FileID)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileID);
	}
	VARIANT GetFileKeywords(unsigned long FileID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, FileID);
		return result;
	}
	VARIANT GetFileAltLocs(unsigned long FileID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, FileID);
		return result;
	}
	VARIANT GetSharedDirIDs()
	{
		VARIANT result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	CString GetDirName(unsigned long DirID)
	{
		CString result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, DirID);
		return result;
	}
	BOOL GetDirRecursive(unsigned long DirID)
	{
		BOOL result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DirID);
		return result;
	}
	unsigned long GetDirFileCount(unsigned long DirID)
	{
		unsigned long result;
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, DirID);
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
	unsigned long GetFileCount()
	{
		unsigned long result;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
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

	// IShare properties
public:

};
