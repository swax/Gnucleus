#if !defined(AFX_GNUMDICHILDWND_H__22D6C596_15BA_4058_94AD_563FBE0094EF__INCLUDED_)
#define AFX_GNUMDICHILDWND_H__22D6C596_15BA_4058_94AD_563FBE0094EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


////////////////////////////////////////////////////////////////////////////////
// @class   Should derive all your CMDIChildWnd from this instead.
//  Passes the important messages (except SetWindowText()) to the tabs.
//  If you change the window text, call UpdateTitle().
//
// @comm    
// @devnote 
//   In order to get the right icon displayed you need to update WindowListDlg.cpp
//   which knows about each type of CFrame... class.  Not great OO style, but that's 
//   life.
// 2001-05-22 Scott Kirkwood <nl>
//    Created <nl>
// 
// @base    
// 


class CGnuMdiChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGnuMdiChildWnd)

protected:
	CGnuMdiChildWnd();           // protected constructor used by dynamic creation


public:
	void UpdateTitle();


	//{{AFX_VIRTUAL(CGnuMdiChildWnd)
	public:
	//}}AFX_VIRTUAL


protected:
	virtual ~CGnuMdiChildWnd();


	// Generated message map functions
	//{{AFX_MSG(CGnuMdiChildWnd)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GNUMDICHILDWND_H__22D6C596_15BA_4058_94AD_563FBE0094EF__INCLUDED_)
