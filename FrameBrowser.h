#pragma once

#include "GnuMdiChildWnd.h"


class CFrameBrowser : public CGnuMdiChildWnd
{
	DECLARE_DYNCREATE(CFrameBrowser)

protected:
	CFrameBrowser();           // protected constructor used by dynamic creation
	virtual ~CFrameBrowser();


	//{{AFX_VIRTUAL(CFrameBrowser)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	

	//{{AFX_MSG(CFrameBrowser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

