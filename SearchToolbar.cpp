// SearchToolbar.cpp
// Copyright © 2001 Scott Kirkwood
// 
// Permission to use, copy, modify, distribute and sell this code
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies or
// any derived copies.  Scott Kirkwood makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Gnucleus.h"
#include "Resource.h"
#include "gdiutil.h"
#include "SearchToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSearchToolbar::CSearchToolbar()
{
	//{{AFX_DATA_INIT(CSearchToolbar)
	//}}AFX_DATA_INIT
	m_bSearchEnabled = false;
	m_hButBitmap = NULL;
}


//void CSearchToolbar::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchToolbar)
	//}}AFX_DATA_MAP
//}


BEGIN_MESSAGE_MAP(CSearchToolbar, CDialogBar)
	//{{AFX_MSG_MAP(CSearchToolbar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SEARCH, OnUpdateSearchButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSearchToolbar::ChangeColor(const CBitmap& bitOrig, COLORREF fromColor, COLORREF toColor)
{
	BITMAP aBitmap;
	memset(&aBitmap, 0, sizeof(BITMAP));
	VERIFY(GetObject(bitOrig, sizeof(BITMAP), &aBitmap));
	int nHeight = aBitmap.bmHeight;
	int nWidth = aBitmap.bmWidth;

	CBitmap aMask;

	{
	CClientDC clientDc(NULL);
	AutoDeleteDc destDc(&clientDc);

	HBITMAP newHBitmap;
	newHBitmap = ::CreateCompatibleBitmap(clientDc.m_hDC, nWidth, nHeight);
	CAutoSelectObject oldDestBitmap(destDc.m_hDC, newHBitmap);

	// Create memory DCs to work with
	AutoDeleteDc maskDc(&clientDc);

	// Create a monochrome bitmap for the mask
	aMask.CreateBitmap(nWidth, nHeight, 1, 1, NULL);
	{
	// Select the mono bitmap into its DC
	CAutoSelectObject oldMask(&maskDc, &aMask);

	CAutoDeleteHDc anotherDc(::CreateCompatibleDC(clientDc.m_hDC));
	CAutoSelectObject tempBitmap(anotherDc, (HBITMAP)bitOrig);

	VERIFY(::BitBlt(destDc.m_hDC, 0, 0, nWidth, nHeight, anotherDc, 0, 0, SRCCOPY));

	::SetBkColor(anotherDc, fromColor);
	VERIFY(::BitBlt(maskDc.m_hDC, 0, 0, nWidth, nHeight, anotherDc, 0, 0, SRCCOPY));

	CBrush newBrush(toColor);
	CAutoSelectObject oldBrush(&destDc, &newBrush);

	// Draw brush where we have 1's in the mask
	const UINT ROP_DSPDxax = 0x00E20746;
	VERIFY(destDc.BitBlt(0, 0, nWidth, nHeight, &maskDc, 0, 0, ROP_DSPDxax));

	m_hButBitmap = newHBitmap;

	}
	}

	VERIFY(aMask.DeleteObject());
}


BOOL CSearchToolbar::Create(CWnd* pParentWnd, UINT nIDTemplate,	UINT nStyle, UINT nID)
{
	BOOL bRet = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	ASSERT(bRet);

	VERIFY(m_ctlComboBox.SubclassDlgItem(IDC_COMBO_SEARCH, this));
	VERIFY(m_ctlSearchButton.SubclassDlgItem(IDC_BUTTON_SEARCH, this));

	VERIFY(m_origBitmap.LoadBitmap(IDR_SEARCH_BUTTON));
	ChangeColor(m_origBitmap, RGB(0xFF, 0xFF, 0xFF), ::GetSysColor(COLOR_BTNFACE));

	m_ctlSearchButton.SetBitmap(m_hButBitmap);

	// This sets up the searches to be saved in a location called "Searches" in the 
	// registry.
	m_ctlComboBox.Init("Searches");
	m_ctlComboBox.SetUseLastByDefault(false);

	return bRet;
}


void CSearchToolbar::EnableSearch(bool bYes)
{
	m_bSearchEnabled = bYes;

	m_ctlComboBox.EnableWindow(m_bSearchEnabled);
	m_ctlSearchButton.EnableWindow(m_bSearchEnabled);
	if (m_bSearchEnabled)
	{
		m_ctlSearchButton.SetBitmap(m_hButBitmap);
	}
	else
	{
		m_ctlSearchButton.SetBitmap(m_origBitmap);
	}
}


void CSearchToolbar::OnUpdateSearchButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bSearchEnabled);
}

