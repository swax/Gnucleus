#if !defined(AFX_VIEWSHARE_H__94A5C4C4_C7CA_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_VIEWSHARE_H__94A5C4C4_C7CA_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "DlgResizer.h"


struct FileItem
{
	DWORD   FileID;

	int		IconIndex;
	CString Name;
	CString NameLower;

	CString Hash;
	DWORD   Size;
	CString Type;
	int		Matches;
	int		Uploads;

	static int  SortBy;
	static bool Reverse;

	inline int operator < (struct FileItem& first)
	{
		bool bResult = false;
		switch (SortBy)
		{
		// NameLower
		case 0:
			if(first.NameLower > NameLower)
				bResult = true;
			else if(first.NameLower == NameLower)
				bResult = (first.Size > Size);
			break;

		// Size
		case 1:
			if(first.Size > Size)
				bResult = true;
			else if(first.Size == Size)
				bResult = (first.NameLower > NameLower);
			break;

		// Type
		case 2:
			if(first.Type > Type)
				bResult = true;
			else if(first.Type == Type)
				bResult = (first.NameLower > NameLower);
			break;

		// Matches
		case 3:
			if(first.Matches > Matches)
				bResult = true;
			else if(first.Matches == Matches)
				bResult = (first.NameLower > NameLower);
			break;

		// Uploads
		case 4:
			if(first.Uploads > Uploads)
				bResult = true;
			else if(first.Uploads == Uploads)
				bResult = (first.NameLower > NameLower);
			break;
		}

		return (Reverse ? !bResult : bResult);	
	};

	inline int operator == (struct FileItem first)
	{
		return (FileID == first.FileID);
	};
};


class CGnucleusDoc;
class CAutShare;


class CViewShare : public CFormView
{
protected:
	CViewShare();           // protected constructor used by dynamic creation
	virtual ~CViewShare();
	DECLARE_DYNCREATE(CViewShare)


public:
	void OnShareReload();
	void OnShareUpdate(UINT FileID);

	CString GetFirstSelectedItemPath();

	//{{AFX_DATA(CViewShare)
	enum { IDD = IDD_VIEW_SHARE };
	CButton	m_chkStopHashing;
	CButton	m_btnStart;
	CButton	m_btnStop;
	CListCtrl	m_lstShared;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CViewShare)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL


protected:
	void LoadSharedList();
	void RunSelected();

	DWORD TotalFiles;
	DWORD TotalSize;
	DWORD TotalMatched;
	DWORD TotalUploaded;

	CGnucleusDoc* m_pDoc;
	CAutShare*    m_autShare;

	std::list<FileItem> FileList;

	CDlgResizer m_DlgResizer;
	

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif


	// Generated message map functions
	//{{AFX_MSG(CViewShare)
	afx_msg void OnColumnclickListShared(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonConfigShare();
	afx_msg void OnButtonStopSharing();
	afx_msg void OnRclickListShared(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListShared(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCheckStophashing();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckRunSelected();
	afx_msg void OnButtonRunSelected();
	afx_msg void OnItemchangedListShared(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSHARE_H__94A5C4C4_C7CA_11D4_ACF2_00A0CC533D52__INCLUDED_)
