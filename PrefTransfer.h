#include "afxwin.h"
#if !defined(AFX_PREFTRANSFER_H__066322C6_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFTRANSFER_H__066322C6_C266_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs;


class CPrefTransfer : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefTransfer)

public:
	CPrefTransfer();
	~CPrefTransfer();


	//{{AFX_DATA(CPrefTransfer)
	enum { IDD = IDD_PREF_TRANSFER };
	CButton	m_chkMultisource;
	CEdit	m_ebPath;
	CButton	m_btnBrowse;
	CEdit	m_ebMaxUL;
	CEdit	m_ebMaxDL;
	CButton	m_chkMaxUL;
	CButton	m_chkMaxDL;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefTransfer)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	

	//{{AFX_MSG(CPrefTransfer)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckMaxDl();
	afx_msg void OnChangeEditMaxDl();
	afx_msg void OnCheckMaxUl();
	afx_msg void OnChangeEditMaxUl();
	afx_msg void OnButtonBrowse();
	afx_msg void OnChangeEditPath();
	afx_msg void OnCheckMultisource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowsePartials();
	afx_msg void OnEnChangeEditPartials();
	CEdit m_ebPartialPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFTRANSFER_H__066322C6_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
