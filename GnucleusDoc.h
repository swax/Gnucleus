#if !defined(AFX_GNUCLEUSDOC_H__24A0CECC_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_GNUCLEUSDOC_H__24A0CECC_881B_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAutCore;
class CAutPrefs;
class CAutNetwork;
class CAutCache;
class CAutShare;
class CAutSearch;
class CAutDownload;
class CAutUpload;

class CAutNetworkSink;
class CAutShareSink;
class CAutSearchSink;
class CAutDownloadSink;
class CAutUploadSink;

class CGnucleusApp;
//class CEvolve;
class CPrefsEx;
class CChatControl;

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

	CString	m_RunPath;

	CAutCore*	  m_autCore;
	CAutPrefs*	  m_autPrefs;
	CAutNetwork*  m_autNetwork;
	CAutCache*	  m_autCache;
	CAutShare*	  m_autShare;
	CAutSearch*   m_autSearch;
	CAutDownload* m_autDownload;
	CAutUpload*   m_autUpload;

	CAutNetworkSink*   m_autNetworkSink;
	CAutShareSink*     m_autShareSink;
	CAutSearchSink*    m_autSearchSink;
	CAutDownloadSink*  m_autDownloadSink;
	CAutUploadSink*    m_autUploadSink;

	DWORD m_NetEventCookie;
	DWORD m_ShareEventCookie;
	DWORD m_SearchEventCookie;
	DWORD m_DownloadEventCookie;
	DWORD m_UploadEventCookie;

	CPrefsEx*	  m_pPrefsEx;


	///////////////////////////////////////////////////////
	
	CString ResolveIP(IP HostIP);

	FullIcon  GetIconIndex(const CString&);

	void CheckVersion(bool Silent = true);
	//CEvolve* m_pDiagEvolve;

	CGnucleusApp* m_pApp;
	CChatControl* m_pChat;


	bool	MultiInstances;
	

	bool	m_ShutDown;			   // Signal to update
	bool	m_StopPost;			   // Signal to stop posting messages

	int		m_nLastPref;		   // Last preference selected
	CString m_CurrentSite;		   // Tells browser where to go



	HWND m_pViewConnect;
	HWND m_pViewChat;
	HWND m_pViewShare;
	HWND m_pViewTransfers;
	
	std::vector<HWND> m_pViewSearch;
	std::vector<HWND> m_pViewStatistics;


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


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GNUCLEUSDOC_H__24A0CECC_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
