// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutUpload wrapper class

class CAutUpload : public COleDispatchDriver
{
public:
	CAutUpload(){} // Calls COleDispatchDriver default constructor
	CAutUpload(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutUpload(const CAutUpload& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IUpload methods
public:
	long GetStatus(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	CString GetName(long UploadID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, UploadID);
		return result;
	}
	DATE GetChangeTime(long UploadID)
	{
		DATE result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_DATE, (void*)&result, parms, UploadID);
		return result;
	}
	long GetBytesCompleted(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	long GetFileLength(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	long GetBytesPerSec(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	long GetSecETD(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	VARIANT GetUploadIDs()
	{
		VARIANT result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void RunFile(long UploadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, UploadID);
	}
	void Remove(long UploadID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, UploadID);
	}
	CString GetErrorStr(long UploadID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, UploadID);
		return result;
	}
	long GetIndex(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	long GetIP(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	long GetPort(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	CString GetHandshake(long UploadID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, UploadID);
		return result;
	}
	long GetAttempts(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}
	long GetQueuePos(long UploadID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, UploadID);
		return result;
	}

	// IUpload properties
public:

};
