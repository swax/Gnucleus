#if !defined(AFX_VIEWCONNECT_H__24A0CECE_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_VIEWCONNECT_H__24A0CECE_881B_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"



class CGnucleusDoc;
class CConnectBasic;
class CConnectAdvanced;


class CViewConnect : public CFormView
{
protected:
	CViewConnect();
	DECLARE_DYNCREATE(CViewConnect)

public:
	virtual ~CViewConnect();

	void OnSockUpdate();
	
	CGnucleusDoc* GetDocument();

	CPropertySheet	m_pSheet;

	CConnectBasic*		m_tabBasic;
	CConnectAdvanced*	m_tabAdvanced;

	CGnucleusDoc* m_pDoc;


	

	//{{AFX_DATA(CViewConnect)
	enum { IDD = IDD_VIEW_CONNECT };
	CStatic	m_tabConnect;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CViewConnect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif


protected:
	CImageList m_ImageLst;

	BOOL CreatePropSheet(CWnd*, CPropertySheet*);
	void SizePropSheet(CWnd*, CPropertySheet*, UINT);
	void SizePropSheet(CWnd*, CPropertySheet*, CRect);


	//{{AFX_MSG(CViewConnect)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ViewConnect.cpp
inline CGnucleusDoc* CViewConnect::GetDocument()
   { return (CGnucleusDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCONNECT_H__24A0CECE_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
