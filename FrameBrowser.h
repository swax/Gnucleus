#if !defined(AFX_FRAMEBROWSER_H__F051CA62_38F6_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMEBROWSER_H__F051CA62_38F6_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEBROWSER_H__F051CA62_38F6_11D5_ACF2_00A0CC533D52__INCLUDED_)
