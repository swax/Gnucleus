// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutMeta wrapper class

class CAutMeta : public COleDispatchDriver
{
public:
	CAutMeta(){} // Calls COleDispatchDriver default constructor
	CAutMeta(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutMeta(const CAutMeta& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IMeta methods
public:
	void LoadSchemaDir(LPCTSTR DirPath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, DirPath);
	}
	VARIANT GetMetaIDs()
	{
		VARIANT result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	CString GetMetaName(long MetaID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, MetaID);
		return result;
	}
	VARIANT GetAttributeIDs(long MetaID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, MetaID);
		return result;
	}
	CString GetAttributeName(long MetaID, long AttributeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, MetaID, AttributeID);
		return result;
	}
	BOOL GetAttributeReadOnly(long MetaID, long AttributeID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, MetaID, AttributeID);
		return result;
	}
	CString GetAttributeType(long MetaID, long AttributeID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, MetaID, AttributeID);
		return result;
	}
	VARIANT GetAttributeEnums(long MetaID, long AttributeID)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, MetaID, AttributeID);
		return result;
	}

	// IMeta properties
public:

};
