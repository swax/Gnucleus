#if !defined(AFX_WEBCACHEDLG_H__5C82C49D_C554_4158_A3DF_390C14D8D0D0__INCLUDED_)
#define AFX_WEBCACHEDLG_H__5C82C49D_C554_4158_A3DF_390C14D8D0D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CGnuCache;

class CWebCacheDlg : public CDialog
{
public:
	CWebCacheDlg(CWnd* pParent = NULL, CGnucleusDoc* pDoc = NULL); // standard constructor



	//{{AFX_DATA(CWebCacheDlg)
	enum { IDD = IDD_TOOL_ADDWEBCACHE };
	CButton	m_btnAdd;
	CButton	m_btnClose;
	CProgressCtrl	m_prgAdd;
	CEdit	m_ebNewCache;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebCacheDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CGnucleusDoc* m_pDoc;
	CGnuCache*	  m_pCache;


	//{{AFX_MSG(CWebCacheDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCACHEDLG_H__5C82C49D_C554_4158_A3DF_390C14D8D0D0__INCLUDED_)
