#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_SHAREEXDLG_H__D97EE6B7_0E1E_4AE4_B1E3_B33F0EA2AEA8__INCLUDED_)
#define AFX_SHAREEXDLG_H__D97EE6B7_0E1E_4AE4_B1E3_B33F0EA2AEA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CAutShare;
class CAutMeta;


class CShareExDlg : public CDialog
{
public:
	CShareExDlg(CWnd* pParent = NULL);

	UINT m_FileID;
	int  m_MetaID;

	std::vector<int> m_ModifiedIDs;

	CGnucleusDoc* m_pDoc;
	CAutShare*	  m_autShare;
	CAutMeta*	  m_autMeta;


	//{{AFX_DATA(CShareExDlg)
	enum { IDD = IDD_SHARE_INFO };
	CListCtrl	m_lstAltLocs;
	CEdit	m_ebSha1Hash;
	CListCtrl	m_lstKeywords;
	CEdit	m_ebSize;
	CEdit	m_ebName;
	CEdit	m_ebIndex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShareExDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CShareExDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stcMetaData;
	CListCtrl m_lstMetaData;
	afx_msg void OnLvnItemchangedListMetadata(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cmbModify;
	CButton m_btnSet;
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonOk();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAREEXDLG_H__D97EE6B7_0E1E_4AE4_B1E3_B33F0EA2AEA8__INCLUDED_)
