#if !defined(AFX_FRAMECHAT_H__2EC15A22_1AF9_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_FRAMECHAT_H__2EC15A22_1AF9_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMECHAT_H__2EC15A22_1AF9_11D5_ACF2_00A0CC533D52__INCLUDED_)
