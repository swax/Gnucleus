#if !defined(AFX_SEARCHADVANCED_H__6B412206_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_SEARCHADVANCED_H__6B412206_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewSearch;
class CGnucleusDoc;


class CSearchAdvanced : public CPropertyPage
{
	DECLARE_DYNCREATE(CSearchAdvanced)

public:
	CSearchAdvanced();
	CSearchAdvanced(CViewSearch*);
	~CSearchAdvanced();


	//{{AFX_DATA(CSearchAdvanced)
	enum { IDD = IDD_SEARCH_ADVANCED };
	CEdit	m_ebSpeed;
	CEdit	m_ebSize;
	CComboBox	m_cmbSpeed;
	CComboBox	m_cmbSize;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSearchAdvanced)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewSearch*    m_pView;
	CGnucleusDoc*	m_pDoc;

	
	//{{AFX_MSG(CSearchAdvanced)
	afx_msg void OnButtonConfigSearch();
	afx_msg void OnButtonScreenhosts();
	afx_msg void OnButtonScreenwords();
	afx_msg void OnButtonRefresh();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHADVANCED_H__6B412206_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
