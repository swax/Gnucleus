#if !defined(AFX_PREFSEARCHSCREEN_H__066322C5_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFSEARCHSCREEN_H__066322C5_C266_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 


class CPrefSearchScreen : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefSearchScreen)

public:
	CPrefSearchScreen();
	~CPrefSearchScreen();


	//{{AFX_DATA(CPrefSearchScreen)
	enum { IDD = IDD_PREF_SEARCH_SCREEN };
	CEdit	m_ebWord;
	CListCtrl	m_lstScreen;
	CButton	m_btnRemove;
	CButton	m_btnAdd;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefSearchScreen)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	

	//{{AFX_MSG(CPrefSearchScreen)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnRclickListScreen(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSEARCHSCREEN_H__066322C5_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
