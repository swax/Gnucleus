#if !defined(AFX_TRANSFERSDOWN_H__AA610E42_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_TRANSFERSDOWN_H__AA610E42_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"


class CGnucleusDoc;
class CAutDownload;
class CViewTransfers;
class CAutPrefs;
class CTransfersDownEx;

class CTransfersDown : public CPropertyPage
{
	DECLARE_DYNCREATE(CTransfersDown)

public:
	CTransfersDown();
	CTransfersDown(CViewTransfers*);
	~CTransfersDown();

	void OnUpdate(int DownloadID);
	void OnRefresh();
	void ReloadLists();
	void UpdateView();

	void RemoveDownload(int);
	void RunFile(int);
	void ReSearch(int);
	void ExtendedInfo(int);

	int GetFirstSelectedItem();

	CString GetEstTime(int);

	CTransfersDownEx* m_ExtendedDlg;

		
	CGnucleusDoc*	m_pDoc;
	CViewTransfers* m_pView;
	CAutDownload*	m_autDownload;
	CAutPrefs*		m_autPrefs;

	CDlgResizer m_DlgResizer;


	//{{AFX_DATA(CTransfersDown)
	enum { IDD = IDD_TRANSFER_DOWN };
	CListCtrl	m_lstDownloads;
	CButton	m_chkPending;
	CButton	m_chkError;
	CButton	m_chkComplete;
	CButton	m_chkActive;
	CButton	m_btnRemove;
	CButton	m_btnClear;
	CButton	m_btnConfigure;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTransfersDown)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CString GetStatus(int DownloadID);
	CString GetHostPos(int DownloadID);
	void UpdateColumns (int row, int DownloadID);
	float GetPercentComplete (int idDownload);

	int m_DownSortColumn, m_DownSortOrder;
	friend struct DownOrder;

	bool m_sync_needed;

	std::vector<int> m_ActiveList;
	std::vector<int> m_PendingList;
	std::vector<int> m_CompletedList;
	std::vector<int> m_ErroredList;   

	//{{AFX_MSG(CTransfersDown)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckActive();
	afx_msg void OnCheckPending();
	afx_msg void OnCheckComplete();
	afx_msg void OnCheckError();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonConfigure();
	afx_msg void OnRclickListDownloads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDownloads(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeydownListDownloads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOpenDownloads();
	afx_msg void OnItemchangedListDownloads(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnSetActive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	 
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERSDOWN_H__AA610E42_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
