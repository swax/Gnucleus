#if !defined(AFX_PREFPLUGINS_H__C79A88A5_479D_4DA5_B5F1_CE71F9DE91BC__INCLUDED_)
#define AFX_PREFPLUGINS_H__C79A88A5_479D_4DA5_B5F1_CE71F9DE91BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;


class CPrefPlugins : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefPlugins)

public:
	CPrefPlugins();
	~CPrefPlugins();


	//{{AFX_DATA(CPrefPlugins)
	enum { IDD = IDD_PREF_PLUGINS };
	CComboBox	m_cmbStartup;
	CListCtrl	m_lstPlugins;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefPlugins)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;


	// Generated message map functions
	//{{AFX_MSG(CPrefPlugins)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFPLUGINS_H__C79A88A5_479D_4DA5_B5F1_CE71F9DE91BC__INCLUDED_)
