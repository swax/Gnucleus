#if !defined(AFX_FRAMECONNECT_H__24A0CECA_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMECONNECT_H__24A0CECA_881B_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GnuMdiChildWnd.h"


class CFrameConnect : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameConnect)

public:
	CFrameConnect();
	virtual ~CFrameConnect();

	//{{AFX_VIRTUAL(CFrameConnect)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif


protected:
	//{{AFX_MSG(CFrameConnect)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMECONNECT_H__24A0CECA_881B_11D4_ACF2_00A0CC533D52__INCLUDED_)
