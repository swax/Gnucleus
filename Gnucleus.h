#if !defined(AFX_GNUCLEUS_H__24A0CEC4_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_GNUCLEUS_H__24A0CEC4_881B_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


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


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GNUCLEUS_H__24A0CEC4_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
