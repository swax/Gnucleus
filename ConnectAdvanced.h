#if !defined(AFX_CONNECTADVANCED_H__3BE6D132_79FC_4FD9_9931_8057311FBF14__INCLUDED_)
#define AFX_CONNECTADVANCED_H__3BE6D132_79FC_4FD9_9931_8057311FBF14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"


class CAutNetwork;


class CConnectAdvanced : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectAdvanced)

public:
	CConnectAdvanced();
	CConnectAdvanced(CViewConnect*);
	~CConnectAdvanced();

	void ExtendedInfo(int);

	void UpdateCacheView();
	void UpdateConnectView();

	CString GetBandwidth(int);
	CString GetEfficiency(int);

	void OnSockUpdate();
	void OnNodeCacheAdd();

	bool m_SetActive;

	CAutNetwork*  m_autNetwork;
	CGnucleusDoc* m_pDoc;

	CDlgResizer m_DlgResizer;


	//{{AFX_DATA(CConnectAdvanced)
	enum { IDD = IDD_CONNECT_ADVANCED };
	CListCtrl	m_lstCached;
	CStatic	m_stcPort;
	CStatic	m_stcHost;
	CStatic	m_stcExist;
	CStatic	m_stcConnect;
	CButton	m_stcAdd;
	CListCtrl	m_lstConnected;
	CEdit	m_ebPort;
	CEdit	m_ebHost;
	CButton	m_btnRemove;
	CButton	m_btnAdd;
	CButton m_radioGnutella;
	CButton m_radioG2;
	//}}AFX_DATA

	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectAdvanced)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CConnectAdvanced)
	afx_msg void OnButtonAdd();
	afx_msg BOOL OnSetActive();
	afx_msg void OnButtonRemove();
	afx_msg void OnRclickLstKnown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickLstConnected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLstConnected(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeydownLstConnected(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioGnutella();
	afx_msg void OnBnClickedRadioG2();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTADVANCED_H__3BE6D132_79FC_4FD9_9931_8057311FBF14__INCLUDED_)
