// ToolBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "Gnucleus.h"
#include "ToolBarEx.h"


// CToolBarEx

IMPLEMENT_DYNAMIC(CToolBarEx, CToolBar)
CToolBarEx::CToolBarEx()
{
	m_HaveFocus = true;
}

CToolBarEx::~CToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)

	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	//ON_WM_PAINT()
END_MESSAGE_MAP()





void CToolBarEx::OnCaptureChanged(CWnd *pWnd)
{
	// TODO: Add your message handler code here

	CToolBar::OnCaptureChanged(pWnd);
}

void CToolBarEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CToolBar::OnMouseMove(nFlags, point);
}

/*void CToolBarEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CToolBar::OnPaint() for painting messages

	CToolBar::OnPaint();
}*/
