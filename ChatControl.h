#pragma once

class CGnucleusDoc;
class CAutPrefs;
class CAutNetwork;

class CChatIdentd;
class CChatServer;
class CChatPrefs;


class CChatControl : public CAsyncSocket
{
public:
	CChatControl(CGnucleusDoc*);
	virtual ~CChatControl();

	bool StartIdentd();
	void StopIdentd();

	void GetConnect();

	void AddServer(CString , UINT );
	void RemoveServer(CChatServer*);
	
	bool SendRequest();
	void ChatMessage(UINT dwMessID, WPARAM room = NULL, LPARAM user = NULL);

	void CheckNicks();
	void Timer();


	bool m_InetTested;
	bool m_AutoConnect;
	int  m_SecsRequest;

	int  m_TryUltraCache;
	bool m_UltraCacheMode;

	std::vector<CChatServer*> m_ServerList;
	std::vector<CChatIdentd*> m_IdentdList;
	int m_IdentdStatus;

	CGnucleusDoc* m_pDoc;
	CChatPrefs*   m_pPrefs;
	
	CAutNetwork*  m_autNetwork;
	CAutPrefs*    m_autPrefs;
	

	CMapStringToString	m_AliasMap;
	CStringArray		m_RemoteScript;

	bool m_NickAlert;

	//{{AFX_VIRTUAL(CChatControl)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatControl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

