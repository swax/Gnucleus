#if !defined(AFX_PREFLOCAL_H__066322C2_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_PREFLOCAL_H__066322C2_C266_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutPrefs; 
class CAutNetwork;

class CPrefLocal : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefLocal)

public:
	CPrefLocal();
	~CPrefLocal();


	//{{AFX_DATA(CPrefLocal)
	enum { IDD = IDD_PREF_LOCAL };
	CButton	m_chkMapping;
	CButton	m_chkTrayOnMin;
	CButton	m_chkTrayOnClose;
	CStatic	m_stcEvolve;
	CButton	m_chkEvolve;
	CComboBox	m_cmbEvolveMode;
	CEdit	m_ebPort;
	CEdit	m_ebIP;
	CComboBox	m_cmbSpeed;
	CButton	m_chkSpeed;
	CButton	m_chkPort;
	CButton	m_chkIP;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrefLocal)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CAutPrefs*    m_autPrefs;
	CAutNetwork*  m_autNetwork;


	//{{AFX_MSG(CPrefLocal)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckIp();
	afx_msg void OnChangeEditIp();
	afx_msg void OnCheckPort();
	afx_msg void OnChangeEditPort();
	afx_msg void OnCheckSpeed();
	afx_msg void OnEditchangeComboSpeed();
	afx_msg void OnCheckEvolve();
	afx_msg void OnEditchangeComboEvolve();
	afx_msg void OnCheckMinimize();
	afx_msg void OnCheckClose();
	afx_msg void OnCheckTraymin();
	afx_msg void OnCheckTrayclose();
	afx_msg void OnCheckMapping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFLOCAL_H__066322C2_C266_11D4_ACF2_00A0CC533D52__INCLUDED_)
