#if !defined(AFX_PREFCHAT_H__18331C81_2611_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFCHAT_H__18331C81_2611_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CChatPrefs;


class CPrefChat : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefChat)

public:
	CPrefChat();
	~CPrefChat();


	//{{AFX_DATA(CPrefChat)
	enum { IDD = IDD_PREF_CHAT };
	CStatic	m_stcServers;
	CListCtrl	m_lstServers;
	CEdit	m_ebServer;
	CEdit	m_ebPrefChannel;
	CComboBox	m_cmbNetwork;
	CButton	m_chkPrefChannel;
	CButton	m_chkPrefNet;
	CButton	m_btnAdd;
	CEdit	m_ebNick;
	CEdit	m_ebAltNick;
	CButton m_chkIdentDisable;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefChat)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CChatPrefs*   m_chatPrefs;


	//{{AFX_MSG(CPrefChat)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditNick();
	afx_msg void OnChangeEditAltnick();
	afx_msg void OnCheckPrefchan();
	afx_msg void OnChangeEditPrefchan();
	afx_msg void OnSelchangeComboNetwork();
	afx_msg void OnButtonAdd();
	afx_msg void OnCheckDefnetwork();
	afx_msg void OnRclickListServers(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckIdentDisable();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFCHAT_H__18331C81_2611_11D5_ACF2_00A0CC533D52__INCLUDED_)
