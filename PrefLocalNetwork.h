#if !defined(AFX_PREFLOCALNETWORK_H__B1DA38A5_9C10_437A_968F_B6EC875B67FF__INCLUDED_)
#define AFX_PREFLOCALNETWORK_H__B1DA38A5_9C10_437A_968F_B6EC875B67FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs;
class CChatPrefs;


class CPrefLocalNetwork : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefLocalNetwork)

public:
	CPrefLocalNetwork();
	~CPrefLocalNetwork();


	//{{AFX_DATA(CPrefLocalNetwork)
	enum { IDD = IDD_PREF_LOCAL_NETWORK };
	CEdit	m_ebMaxLeaves;
	CStatic	m_stcUsage;
	CSliderCtrl	m_sldrUsage;
	CButton	m_chkDedicated;
	CButton	m_chkSuperNode;
	CEdit	m_ebIRCServer;
	CButton	m_chkIRCServer;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefLocalNetwork)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	CChatPrefs*	  m_chatPrefs;

	//{{AFX_MSG(CPrefLocalNetwork)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioModelInternet();
	afx_msg void OnRadioModelPrivate();
	afx_msg void OnCheckIrcserver();
	afx_msg void OnChangeEditIrcserver();
	afx_msg void OnReleasedcaptureSliderUsage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderUsage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckSupernode();
	afx_msg void OnCheckDedicated();
	afx_msg void OnChangeEditMaxleaf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFLOCALNETWORK_H__B1DA38A5_9C10_437A_968F_B6EC875B67FF__INCLUDED_)
