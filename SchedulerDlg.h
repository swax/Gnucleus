#pragma once


class CGnucleusDoc;
class CPrefsEx;

class CSchedulerDlg : public CDialog
{
// Construction
public:
	CSchedulerDlg(CWnd* pParent = NULL, CGnucleusDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSchedulerDlg)
	enum { IDD = IDD_TOOL_SCHEDULER };
	CButton	m_stcScheduler;
	CStatic	m_stcDisconnect;
	CStatic	m_stcConnect;
	CDateTimeCtrl	m_tmDisconnect;
	CDateTimeCtrl	m_tmConnect;
	CButton	m_chkScheduler;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSchedulerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CGnucleusDoc* m_pDoc;
	CPrefsEx*     m_pPrefsEx;

	// Generated message map functions
	//{{AFX_MSG(CSchedulerDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSchedActivated();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
