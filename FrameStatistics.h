#if !defined(AFX_FRAMESTATISTICS_H__65D1A920_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMESTATISTICS_H__65D1A920_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GnuMdiChildWnd.h"


class CFrameStatistics : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameStatistics)

protected:
	CFrameStatistics(); 
	virtual ~CFrameStatistics();

public:
	//{{AFX_VIRTUAL(CFrameStatistics)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CFrameStatistics)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMESTATISTICS_H__65D1A920_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
