// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAutPrefs wrapper class

class CAutPrefs : public COleDispatchDriver
{
public:
	CAutPrefs(){} // Calls COleDispatchDriver default constructor
	CAutPrefs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAutPrefs(const CAutPrefs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IPrefs methods
public:
	void LoadConfig(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void SaveConfig(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void LoadBlocked(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	void SaveBlocked(LPCTSTR FilePath)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FilePath);
	}
	VARIANT GetClientID()
	{
		VARIANT result;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SetClientID(VARIANT * pNewVal)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pNewVal);
	}
	VARIANT GetHostServers()
	{
		VARIANT result;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SetHostServers(VARIANT * pNewVal)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pNewVal);
	}
	VARIANT GetScreenedNodes()
	{
		VARIANT result;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SetScreenedNodes(VARIANT * pNewVal)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pNewVal);
	}
	VARIANT GetScreenedWords()
	{
		VARIANT result;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SetScreenedWords(VARIANT * pNewVal)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pNewVal);
	}
	VARIANT GetBlockList()
	{
		VARIANT result;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void SetBlockList(VARIANT * pNewVal)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pNewVal);
	}

	// IPrefs properties
public:
	unsigned long GetForcedHost()
	{
		unsigned long result;
		GetProperty(0x6, VT_UI4, (void*)&result);
		return result;
	}
	void SetForcedHost(unsigned long propVal)
	{
		SetProperty(0x6, VT_UI4, propVal);
	}
	unsigned long GetForcedPort()
	{
		unsigned long result;
		GetProperty(0x8, VT_UI4, (void*)&result);
		return result;
	}
	void SetForcedPort(unsigned long propVal)
	{
		SetProperty(0x8, VT_UI4, propVal);
	}
	unsigned long GetSpeedStat()
	{
		unsigned long result;
		GetProperty(0x9, VT_UI4, (void*)&result);
		return result;
	}
	void SetSpeedStat(unsigned long propVal)
	{
		SetProperty(0x9, VT_UI4, propVal);
	}
	long GetUpdate()
	{
		long result;
		GetProperty(0xb, VT_I4, (void*)&result);
		return result;
	}
	void SetUpdate(long propVal)
	{
		SetProperty(0xb, VT_I4, propVal);
	}
	BOOL GetSuperNodeAble()
	{
		BOOL result;
		GetProperty(0x11, VT_BOOL, (void*)&result);
		return result;
	}
	void SetSuperNodeAble(BOOL propVal)
	{
		SetProperty(0x11, VT_BOOL, propVal);
	}
	BOOL GetDedicated()
	{
		BOOL result;
		GetProperty(0x12, VT_BOOL, (void*)&result);
		return result;
	}
	void SetDedicated(BOOL propVal)
	{
		SetProperty(0x12, VT_BOOL, propVal);
	}
	unsigned long GetMaxLeaves()
	{
		unsigned long result;
		GetProperty(0x13, VT_UI4, (void*)&result);
		return result;
	}
	void SetMaxLeaves(unsigned long propVal)
	{
		SetProperty(0x13, VT_UI4, propVal);
	}
	BOOL GetLanMode()
	{
		BOOL result;
		GetProperty(0x14, VT_BOOL, (void*)&result);
		return result;
	}
	void SetLanMode(BOOL propVal)
	{
		SetProperty(0x14, VT_BOOL, propVal);
	}
	CString GetLanName()
	{
		CString result;
		GetProperty(0x15, VT_BSTR, (void*)&result);
		return result;
	}
	void SetLanName(CString propVal)
	{
		SetProperty(0x15, VT_BSTR, propVal);
	}
	BOOL GetBehindFirewall()
	{
		BOOL result;
		GetProperty(0x1a, VT_BOOL, (void*)&result);
		return result;
	}
	void SetBehindFirewall(BOOL propVal)
	{
		SetProperty(0x1a, VT_BOOL, propVal);
	}
	BOOL GetAutoConnect()
	{
		BOOL result;
		GetProperty(0x1b, VT_BOOL, (void*)&result);
		return result;
	}
	void SetAutoConnect(BOOL propVal)
	{
		SetProperty(0x1b, VT_BOOL, propVal);
	}
	long GetLeafModeConnects()
	{
		long result;
		GetProperty(0x1c, VT_I4, (void*)&result);
		return result;
	}
	void SetLeafModeConnects(long propVal)
	{
		SetProperty(0x1c, VT_I4, propVal);
	}
	long GetMinConnects()
	{
		long result;
		GetProperty(0x1d, VT_I4, (void*)&result);
		return result;
	}
	void SetMinConnects(long propVal)
	{
		SetProperty(0x1d, VT_I4, propVal);
	}
	long GetMaxConnects()
	{
		long result;
		GetProperty(0x1e, VT_I4, (void*)&result);
		return result;
	}
	void SetMaxConnects(long propVal)
	{
		SetProperty(0x1e, VT_I4, propVal);
	}
	CString GetDownloadPath()
	{
		CString result;
		GetProperty(0x21, VT_BSTR, (void*)&result);
		return result;
	}
	void SetDownloadPath(CString propVal)
	{
		SetProperty(0x21, VT_BSTR, propVal);
	}
	BOOL GetDoubleCheck()
	{
		BOOL result;
		GetProperty(0x22, VT_BOOL, (void*)&result);
		return result;
	}
	void SetDoubleCheck(BOOL propVal)
	{
		SetProperty(0x22, VT_BOOL, propVal);
	}
	BOOL GetScreenNodes()
	{
		BOOL result;
		GetProperty(0x23, VT_BOOL, (void*)&result);
		return result;
	}
	void SetScreenNodes(BOOL propVal)
	{
		SetProperty(0x23, VT_BOOL, propVal);
	}
	BOOL GetReplyFilePath()
	{
		BOOL result;
		GetProperty(0x29, VT_BOOL, (void*)&result);
		return result;
	}
	void SetReplyFilePath(BOOL propVal)
	{
		SetProperty(0x29, VT_BOOL, propVal);
	}
	long GetMaxReplies()
	{
		long result;
		GetProperty(0x2a, VT_I4, (void*)&result);
		return result;
	}
	void SetMaxReplies(long propVal)
	{
		SetProperty(0x2a, VT_I4, propVal);
	}
	BOOL GetSendOnlyAvail()
	{
		BOOL result;
		GetProperty(0x2b, VT_BOOL, (void*)&result);
		return result;
	}
	void SetSendOnlyAvail(BOOL propVal)
	{
		SetProperty(0x2b, VT_BOOL, propVal);
	}
	long GetMaxDownloads()
	{
		long result;
		GetProperty(0x2d, VT_I4, (void*)&result);
		return result;
	}
	void SetMaxDownloads(long propVal)
	{
		SetProperty(0x2d, VT_I4, propVal);
	}
	long GetMaxUploads()
	{
		long result;
		GetProperty(0x2e, VT_I4, (void*)&result);
		return result;
	}
	void SetMaxUploads(long propVal)
	{
		SetProperty(0x2e, VT_I4, propVal);
	}
	BOOL GetMultisource()
	{
		BOOL result;
		GetProperty(0x2f, VT_BOOL, (void*)&result);
		return result;
	}
	void SetMultisource(BOOL propVal)
	{
		SetProperty(0x2f, VT_BOOL, propVal);
	}
	float GetBandwidthUp()
	{
		float result;
		GetProperty(0x30, VT_R4, (void*)&result);
		return result;
	}
	void SetBandwidthUp(float propVal)
	{
		SetProperty(0x30, VT_R4, propVal);
	}
	float GetBandwidthDown()
	{
		float result;
		GetProperty(0x31, VT_R4, (void*)&result);
		return result;
	}
	void SetBandwidthDown(float propVal)
	{
		SetProperty(0x31, VT_R4, propVal);
	}
	float GetMinDownSpeed()
	{
		float result;
		GetProperty(0x32, VT_R4, (void*)&result);
		return result;
	}
	void SetMinDownSpeed(float propVal)
	{
		SetProperty(0x32, VT_R4, propVal);
	}
	float GetMinUpSpeed()
	{
		float result;
		GetProperty(0x33, VT_R4, (void*)&result);
		return result;
	}
	void SetMinUpSpeed(float propVal)
	{
		SetProperty(0x33, VT_R4, propVal);
	}
	CString GetPartialsDir()
	{
		CString result;
		GetProperty(0x3e, VT_BSTR, (void*)&result);
		return result;
	}
	void SetPartialsDir(CString propVal)
	{
		SetProperty(0x3e, VT_BSTR, propVal);
	}

};
