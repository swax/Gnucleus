#pragma once
#include "afxwin.h"

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
	CAutNetwork*  m_autNetwork;
	CChatPrefs*	  m_chatPrefs;

	//{{AFX_MSG(CPrefLocalNetwork)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioModelInternet();
	afx_msg void OnRadioModelPrivate();
	afx_msg void OnCheckIrcserver();
	afx_msg void OnChangeEditIrcserver();
	afx_msg void OnCheckSupernode();
	afx_msg void OnChangeEditMaxleaf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnBnClickedCheck1();
	CButton m_chkConnectGnutella;
	CButton m_chkConnectG2;
	CButton m_chkForceGnutella;
	CButton m_chkForceG2;
};
