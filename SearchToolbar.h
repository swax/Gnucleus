#if !defined(AFX_SEARCHTOOLBAR_H__4F0DE057_0E7A_4920_ACFB_A929E74CCEA4__INCLUDED_)
#define AFX_SEARCHTOOLBAR_H__4F0DE057_0E7A_4920_ACFB_A929E74CCEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Gnucleus.h"
#include "MruComboBox.h"


class CSearchToolbar : public CDialogBar
{
public:
	CSearchToolbar();   // standard constructor

	CMruComboBox m_ctlComboBox;

	void EnableSearch(bool bYes);


	//{{AFX_DATA(CSearchToolbar)
	enum { IDD = IDR_MAINFRAME };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSearchToolbar)
	public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate,	UINT nStyle, UINT nID);
	protected:
	//}}AFX_VIRTUAL


protected:
	void ChangeColor(const CBitmap& bitOrig, COLORREF fromColor, COLORREF toColor);

	void OnUpdateSearchButton(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	CButton m_ctlSearchButton;
	bool m_bSearchEnabled;
	HBITMAP m_hButBitmap;
	CBitmap m_origBitmap;
	
	
	//{{AFX_MSG(CSearchToolbar)
	//}}AFX_MSG
public:
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHTOOLBAR_H__4F0DE057_0E7A_4920_ACFB_A929E74CCEA4__INCLUDED_)
