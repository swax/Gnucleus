#pragma once


#include "GnuMdiChildWnd.h"


class CFrameChat : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameChat)

protected:
	CFrameChat();           // protected constructor used by dynamic creation
	virtual ~CFrameChat();


	//{{AFX_VIRTUAL(CFrameChat)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFrameChat)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

