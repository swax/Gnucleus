#if !defined(AFX_VIEWBROWSER_H__F051CA61_38F6_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_VIEWBROWSER_H__F051CA61_38F6_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


class CGnucleusDoc;


class CViewBrowser : public CHtmlView
{
protected:
	CViewBrowser();           // protected constructor used by dynamic creation
	virtual ~CViewBrowser();
	DECLARE_DYNCREATE(CViewBrowser)

	CGnucleusDoc* m_pDoc;

	

public:
	void CommandSent();

	//{{AFX_DATA(CViewBrowser)
	enum { IDD = IDD_BROWSER_URL };
	CEdit	m_ebUrl;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CViewBrowser)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CToolBar   m_wndToolBar;	
	CDialogBar m_AddressBar;
	CReBar     m_wndReBar;
	
	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	//{{AFX_MSG(CViewBrowser)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSurfBack();
	afx_msg void OnSurfForward();
	afx_msg void OnSurfHome();
	afx_msg void OnSurfRefresh();
	afx_msg void OnSurfStop();
	afx_msg void OnButtonGo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWBROWSER_H__F051CA61_38F6_11D5_ACF2_00A0CC533D52__INCLUDED_)
