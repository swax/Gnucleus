#if !defined(AFX_PREFSHARE_H__A6C36B23_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFSHARE_H__A6C36B23_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs;
class CAutShare;

class CPrefShare : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefShare)

public:
	CPrefShare();
	~CPrefShare();


	//{{AFX_DATA(CPrefShare)
	enum { IDD = IDD_PREF_SHARE };
	CButton	m_chkSendResults;
	CEdit	m_ebReply;
	CButton	m_chkReply;
	CListCtrl	m_lstShared;
	CEdit	m_ebShared;
	CButton	m_chkRecurse;
	CButton	m_chkPath;
	CButton	m_btnRemove;
	CButton	m_btnAdd;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefShare)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	void  Browse();
	DWORD RecurseCount(LPCTSTR);

	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	CAutShare*	  m_autShare;
	
	bool m_FilesSharedModified;
	

	//{{AFX_MSG(CPrefShare)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnChangeEditShared();
	afx_msg void OnCheckPath();
	afx_msg void OnCheckReplymax();
	afx_msg void OnChangeEditReply();
	afx_msg void OnCheckSendresults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSHARE_H__A6C36B23_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
