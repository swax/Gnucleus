#if !defined(AFX_FRAMETRANSFERS_H__AA610E41_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMETRANSFERS_H__AA610E41_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GnuMdiChildWnd.h"


class CFrameTransfers : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameTransfers)

protected:
	CFrameTransfers();           // protected constructor used by dynamic creation

public:
	//{{AFX_VIRTUAL(CFrameTransfers)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


protected:
	virtual ~CFrameTransfers();


	// Generated message map functions
	//{{AFX_MSG(CFrameTransfers)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMETRANSFERS_H__AA610E41_CE06_11D4_ACF2_00A0CC533D52__INCLUDED_)
