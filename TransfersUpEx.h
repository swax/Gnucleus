#if !defined(AFX_TRANSFERINFO_H__167224A7_DFDE_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_TRANSFERINFO_H__167224A7_DFDE_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTransfersUpEx : public CDialog
{
public:
	CTransfersUpEx(CWnd* pParent = NULL);   // standard constructor

	CString m_FileName;
	DWORD   m_FileIndex;

	IP		m_Host;
	UINT	m_Port;

	int     m_Attempts;
	CString m_Handshake;


	//{{AFX_DATA(CTransfersUpEx)
	enum { IDD = IDD_TRANSFER_UP_INFO };
	CStatic	m_stcAttempts;
	CStatic	m_stcName;
	CStatic	m_stcIndex;
	CStatic	m_stcHost;
	CEdit	m_ebHandshake;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTransfersUpEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CTransfersUpEx)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERINFO_H__167224A7_DFDE_11D4_ACF2_00A0CC533D52__INCLUDED_)
