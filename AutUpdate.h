// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutUpdate wrapper class

class CAutUpdate : public COleDispatchDriver
{
public:
	CAutUpdate(){} // Calls COleDispatchDriver default constructor
	CAutUpdate(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutUpdate(const CAutUpdate& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IUpdate methods
public:
	void AddServer(LPCTSTR Server)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Server);
	}
	void Check()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void StartDownload()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CancelUpdate()
	{
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	VARIANT GetFileIDs()
	{
		VARIANT result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long GetTotalCompleted()
	{
		long result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetTotalSize()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetFileName(long FileID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, FileID);
		return result;
	}
	long GetFileSize(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	long GetFileCompleted(long FileID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileID);
		return result;
	}
	void LaunchUpdate()
	{
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// IUpdate properties
public:

};
