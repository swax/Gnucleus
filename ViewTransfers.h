#if !defined(AFX_VIEWTRANSFERS_H__AA610E40_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_VIEWTRANSFERS_H__AA610E40_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


class CGnucleusDoc;
class CTransfersDown;
class CTransfersUp;


class CViewTransfers : public CFormView
{
protected:
	CViewTransfers();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewTransfers)


public:
	void ActivateDownloadPane();
		
	void OnDownloadUpdate(int DownloadID);
	void OnUploadUpdate(int UploadID);

	CPropertySheet	m_pSheet;

	CTransfersDown*		m_tabDownloads;
	CTransfersUp*		m_tabUploads;

	CGnucleusDoc* m_pDoc;


	//{{AFX_DATA(CViewTransfers)
	enum { IDD = IDD_VIEW_TRANSFER };
	CStatic	m_tabTransfers;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CViewTransfers)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL


protected:
	CImageList m_ImageLst;

	BOOL CreatePropSheet(CWnd*, CPropertySheet*);
	void SizePropSheet(CWnd*, CPropertySheet*, UINT);
	void SizePropSheet(CWnd*, CPropertySheet*, CRect);

	CGnucleusDoc* GetDocument();
	virtual ~CViewTransfers();


	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	// Generated message map functions
	//{{AFX_MSG(CViewTransfers)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTRANSFERS_H__AA610E40_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
