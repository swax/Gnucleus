#if !defined(AFX_TRANSFERSUP_H__AA610E43_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_TRANSFERSUP_H__AA610E43_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"
#include "afxwin.h"


class CViewTransfers;
class CGnucleusDoc;
class CAutUpload;

class CTransfersUp : public CPropertyPage
{
	DECLARE_DYNCREATE(CTransfersUp)

public:
	CTransfersUp();
	CTransfersUp(CViewTransfers*);
	~CTransfersUp();

	void OnUpdate(int UploadID);
	void ReloadLists();
	void UpdateView();

	CString GetEstTime(int);
	
	void RunFile(int);
	void ExtendedInfo(int);

	//CString GetFirstSelectedItem();


	//{{AFX_DATA(CTransfersUp)
	enum { IDD = IDD_TRANSFER_UP };
	CListCtrl	m_lstUploads;
	CButton	m_chkError;
	CButton	m_chkComplete;
	CButton	m_chkActive;
	CButton	m_btnClear;
	CButton	m_btnRemove;
	CButton	m_btnConfigure;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTransfersUp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	void RemoveItem(int);
	CString GetStatus(int);

	int m_UpSortColumn;
	int m_UpSortOrder;
	friend struct UpOrder;

	CViewTransfers* m_pView;
	CGnucleusDoc*	m_pDoc;
	CAutUpload*     m_autUpload;

	std::vector<int> m_ActiveList;
	std::vector<int> m_CompletedList;
	std::vector<int> m_QueuedList;
	std::vector<int> m_ErroredList;    

	CDlgResizer m_DlgResizer;


	//{{AFX_MSG(CTransfersUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckActive();
	afx_msg void OnCheckComplete();
	afx_msg void OnCheckError();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonConfigure();
	afx_msg void OnRclickListUploads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListUploads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListUploads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListUploads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListUploads(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnSetActive();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()	
public:
	CButton m_chkQueued;
	afx_msg void OnCheckQueued();
	afx_msg void OnTimer(UINT nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERSUP_H__AA610E43_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
