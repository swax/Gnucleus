#if !defined(AFX_TRANSFERDOWNEX_H__8E5B70E1_1597_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_TRANSFERDOWNEX_H__8E5B70E1_1597_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"
#include "TransfersDownBar.h"


class CTransfersDown;
class CAutDownload;

class CTransfersDownEx : public CDialog
{
public:
	CTransfersDownEx(CWnd* pParent = NULL, CTransfersDown* pView = NULL);   // standard constructor
	
	void UpdateInfo();

	int m_DownloadID;

	std::vector<CString> m_HandshakeList;

	CDlgResizer m_DlgResizer;

	int m_DownExSortColumn, m_DownExSortOrder;
	friend struct DownExOrder;

	//{{AFX_DATA(CTransfersDownEx)
	enum { IDD = IDD_TRANSFER_DOWN_INFO };
	CStatic	m_stcBitprintHash;
	CStatic	m_stcSha1Hash;
	CListCtrl	m_lstChunks;
	CTransfersDownBar	m_stcProgress;
	CStatic	m_stcSize;
	CStatic	m_stcHandshake;
	CEdit	m_ebHandshake;
	CButton	m_btnExpand;
	CStatic	m_stcName;
	CStatic	m_stcHosts;
	CListCtrl	m_lstDownloads;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTransfersDownEx)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	
	bool m_Expanded;
	bool m_Receiving;

	CTransfersDown* m_pView;
	CAutDownload*   m_autDownload;

	void Expand();

	int m_HostNum;

	
	//{{AFX_MSG(CTransfersDownEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonExpand();
	afx_msg void OnItemchangedListDownloads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListDownloads(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERDOWNEX_H__8E5B70E1_1597_11D5_ACF2_00A0CC533D52__INCLUDED_)
