#pragma once
#include "afxwin.h"


// CBrowseHostDlg dialog

class CBrowseHostDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrowseHostDlg)

public:
	CBrowseHostDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBrowseHostDlg();

// Dialog Data
	enum { IDD = IDD_TOOL_BROWSEHOST };

	CString m_HostPort;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ebHostPort;
	afx_msg void OnBnClickedOk();
};
