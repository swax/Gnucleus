
#pragma once


class CAutCore;
class CAutPrefs;
class CAutNetwork;
class CAutCache;
class CAutMeta;
class CAutShare;
class CAutSearch;
class CAutDownload;
class CAutUpload;
class CAutUpdate;
class CAutChat;

class CAutNetworkSink;
class CAutShareSink;
class CAutSearchSink;
class CAutDownloadSink;
class CAutUploadSink;
class CAutUpdateSink;
class CAutChatSink;

class CGnucleusApp;
class CPrefsEx;
class CChatControl;
class CEvolverDlg;

class CViewConnect;
class CViewShare;
class CViewSearch;
class CViewTransfers;
class CViewStatistics;
class CViewBrowser;


class CGnucleusDoc : public CDocument
{
public:
	CGnucleusDoc();
	CGnucleusDoc(CGnucleusApp* pApp);
	virtual ~CGnucleusDoc();
	
	void ConnectCore();
	void DisconnectCore();
	CString GetVersionInfo(void);

	void DisplayEvolver();
	CEvolverDlg* m_pDiagEvolve;

	CString	m_RunPath;

	CAutCore*	  m_autCore;
	CAutPrefs*	  m_autPrefs;
	CAutNetwork*  m_autNetwork;
	CAutCache*	  m_autCache;
	CAutMeta*	  m_autMeta;
	CAutShare*	  m_autShare;
	CAutSearch*   m_autSearch;
	CAutDownload* m_autDownload;
	CAutUpload*   m_autUpload;
	CAutUpdate*   m_autUpdate;
	CAutChat*     m_autChat;

	CAutNetworkSink*   m_autNetworkSink;
	CAutShareSink*     m_autShareSink;
	CAutSearchSink*    m_autSearchSink;
	CAutDownloadSink*  m_autDownloadSink;
	CAutUploadSink*    m_autUploadSink;
	CAutUpdateSink*	   m_autUpdateSink;
	CAutChatSink*	   m_autChatSink;

	DWORD m_NetEventCookie;
	DWORD m_ShareEventCookie;
	DWORD m_SearchEventCookie;
	DWORD m_DownloadEventCookie;
	DWORD m_UploadEventCookie;
	DWORD m_UpdateEventCookie;
	DWORD m_ChatEventCookie;

	CPrefsEx*	  m_pPrefsEx;
	CGnucleusApp* m_pApp;
	CChatControl* m_pChat;

	HWND m_pViewConnect;
	HWND m_pViewChat;
	HWND m_pViewShare;
	HWND m_pViewTransfers;
	
	std::vector<HWND> m_pViewSearch;
	std::vector<HWND> m_pViewStatistics;

	///////////////////////////////////////////////////////
	
	CString ResolveIP(IP HostIP);

	FullIcon  GetIconIndex(const CString&);

	bool	MultiInstances;
	bool	m_ShutDown;			   // Signal to update
	int		m_nLastPref;		   // Last preference selected
	CString m_CurrentSite;		   // Tells browser where to go
	CString m_GnuVersion;		   // Gnucleus version info
	bool    m_RunningXP;

	//{{AFX_VIRTUAL(CGnucleusDoc)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif


protected: 
	DECLARE_DYNCREATE(CGnucleusDoc)

	//HMODULE m_hRas;
	//DWORD (WINAPI* m_pfGetConnectStatus)(HRASCONN, LPRASCONNSTATUSA);
	//DWORD (WINAPI* m_pfEnumConnections)(LPRASCONNA, LPDWORD, LPDWORD);

	//{{AFX_MSG(CGnucleusDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

