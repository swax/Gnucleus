#if !defined(AFX_CONNECTBASIC_H__4E3B1624_C9F8_49DA_9209_B71F46E0223F__INCLUDED_)
#define AFX_CONNECTBASIC_H__4E3B1624_C9F8_49DA_9209_B71F46E0223F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAutCore;
class CAutPrefs;
class CAutNetwork;
class CGnucleusDoc;


class CConnectBasic : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectBasic)

public:
	CConnectBasic();
	CConnectBasic(CViewConnect*);
	~CConnectBasic();

	void OnSockUpdate();


	//{{AFX_DATA(CConnectBasic)
	enum { IDD = IDD_CONNECT_BASIC };
	CStatic	m_stcTransfers;
	CStatic	m_stcShare;
	CStatic	m_stcChat;
	CStatic	m_stcSearch;
	CProgressCtrl	m_progConnect;
	CStatic	m_stcConnect;
	CButton	m_btnSearch;
	CButton	m_btnTransfers;
	CButton	m_btnShared;
	CButton	m_btnChat;
	CStatic	m_stcLogo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CConnectBasic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	CAutCore*	  m_autCore;
	CAutNetwork*  m_autNetwork;
	CAutShare*    m_autShare;
	

	int m_BarPos;
	int m_Status;
	int m_HashDots;

	bool m_Ready;

	//{{AFX_MSG(CConnectBasic)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonTransfers();
	afx_msg void OnButtonShared();
	afx_msg void OnButtonChat();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStaticTransfers();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTBASIC_H__4E3B1624_C9F8_49DA_9209_B71F46E0223F__INCLUDED_)
