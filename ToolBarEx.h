#pragma once


// CToolBarEx

class CToolBarEx : public CToolBar
{
	DECLARE_DYNAMIC(CToolBarEx)

public:
	CToolBarEx();
	virtual ~CToolBarEx();

	bool m_HaveFocus;


protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnPaint();
};


