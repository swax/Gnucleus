#if !defined(AFX_PREFSEARCH_H__066322C4_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFSEARCH_H__066322C4_C266_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 


class CPrefSearch : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefSearch)

public:
	CPrefSearch();
	~CPrefSearch();


	//{{AFX_DATA(CPrefSearch)
	enum { IDD = IDD_PREF_SEARCH };
	CButton	m_chkScreenNodes;
	CButton	m_chkDoubleCheck;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefSearch)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;


	//{{AFX_MSG(CPrefSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDoublecheck();
	afx_msg void OnCheckDisplayScreened();
	afx_msg void OnButtonClearhistory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSEARCH_H__066322C4_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
