#if !defined(AFX_FRAMESHARE_H__94A5C4C3_C7CA_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMESHARE_H__94A5C4C3_C7CA_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GnuMdiChildWnd.h"


class CFrameShare : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameShare)
protected:
	CFrameShare();           // protected constructor used by dynamic creation
	virtual ~CFrameShare();


	// Generated message map functions
	//{{AFX_MSG(CFrameShare)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	//{{AFX_VIRTUAL(CFrameShare)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMESHARE_H__94A5C4C3_C7CA_11D4_ACF2_00A0CC533D52__INCLUDED_)
