#if !defined(AFX_PREFLOCALFIREWALL_H__066322C3_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFLOCALFIREWALL_H__066322C3_C266_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs;


class CPrefLocalFirewall : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefLocalFirewall)

public:
	CPrefLocalFirewall();
	~CPrefLocalFirewall();


	//{{AFX_DATA(CPrefLocalFirewall)
	enum { IDD = IDD_PREF_LOCAL_FIREWALL };
	CButton	m_chkFirewall;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefLocalFirewall)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	

	//{{AFX_MSG(CPrefLocalFirewall)
	afx_msg void OnCheckFirewall();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFLOCALFIREWALL_H__066322C3_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
