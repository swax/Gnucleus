#if !defined(AFX_FRAMESEARCH_H__6B412201_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMESEARCH_H__6B412201_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GnuMdiChildWnd.h"


class CFrameSearch : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameSearch)
protected:
	CFrameSearch();           // protected constructor used by dynamic creation
	virtual ~CFrameSearch();

	// Generated message map functions
	//{{AFX_MSG(CFrameSearch)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	//{{AFX_VIRTUAL(CFrameSearch)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMESEARCH_H__6B412201_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
