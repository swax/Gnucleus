#pragma once


#include "resource.h"       // main symbols


class CGnucleusDoc;


class CGnucleusApp : public CWinApp
{
public:
	CGnucleusApp();
	~CGnucleusApp();

	CGnucleusDoc* m_pDoc;

	CMultiDocTemplate* m_pConnectTemplate;
	CMultiDocTemplate* m_pStatisticsTemplate;
	CMultiDocTemplate* m_pShareTemplate;
	CMultiDocTemplate* m_pTransfersTemplate;
	CMultiDocTemplate* m_pSearchTemplate;
	CMultiDocTemplate* m_pChatTemplate;
	CMultiDocTemplate* m_pBrowserTemplate;

	bool m_Min;


	//{{AFX_VIRTUAL(CGnucleusApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGnucleusApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void LoadTemplates();
	bool InstanceRunning();

	HANDLE m_hMutex;
};


// Got this from MSDN
class CMyCmdLineInfo : public CCommandLineInfo 
{
public:
	virtual void ParseParam(LPCSTR pszParam, BOOL bFlag, BOOL bLast);

	CGnucleusApp* m_App;
	bool		  m_Minimize;
};

