#if !defined(AFX_PREFSEARCHBLOCK_H__81D0ACDD_6E56_47BD_BC48_4752A2888501__INCLUDED_)
#define AFX_PREFSEARCHBLOCK_H__81D0ACDD_6E56_47BD_BC48_4752A2888501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 


class CPrefSearchBlock : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefSearchBlock)

// Construction
public:
	CPrefSearchBlock();
	~CPrefSearchBlock();

	CString m_SearchExInit;

	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;


	//{{AFX_DATA(CPrefSearchBlock)
	enum { IDD = IDD_PREF_SEARCH_BLOCK };
	CListCtrl	m_lstBlocked;
	CEdit	m_ebReason;
	CEdit	m_ebHost;
	//}}AFX_DATA

	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefSearchBlock)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	
	

	//{{AFX_MSG(CPrefSearchBlock)
	afx_msg void OnButtonAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRemove();
	afx_msg void OnRclickListScreen(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSEARCHBLOCK_H__81D0ACDD_6E56_47BD_BC48_4752A2888501__INCLUDED_)
