#if !defined(AFX_PREFBANDWIDTH_H__A6C36B24_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFBANDWIDTH_H__A6C36B24_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs;


class CPrefBandwidth : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefBandwidth)

public:
	CPrefBandwidth();
	~CPrefBandwidth();


	//{{AFX_DATA(CPrefBandwidth)
	enum { IDD = IDD_PREF_BANDWIDTH };
	CButton	m_chkLimitDown;
	CButton	m_chkLimitUp;
	CEdit	m_ebLimitUp;
	CEdit	m_ebLimitDown;
	CEdit	m_ebDropUL;
	CEdit	m_ebDropDL;
	CButton	m_chkDropUL;
	CButton	m_chkDropDL;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefBandwidth)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;


	//{{AFX_MSG(CPrefBandwidth)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDropDl();
	afx_msg void OnChangeEditDropDl();
	afx_msg void OnCheckDropUl();
	afx_msg void OnChangeEditDropUl();
	afx_msg void OnCheckLimitUp();
	afx_msg void OnChangeEditLimitUp();
	afx_msg void OnCheckLimitDown();
	afx_msg void OnChangeEditLimitDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFBANDWIDTH_H__A6C36B24_AC46_11D4_ACF2_00A0CC533D52__INCLUDED_)
