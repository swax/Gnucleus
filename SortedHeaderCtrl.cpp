// SortedHeaderCtrl.cpp
// Copyright © 2000 Scott Kirkwood
// 
// Permission to use, copy, modify, distribute and sell this code
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies or
// any derived copies.  Scott Kirkwood makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

// This class was written by someone else originally (probably from CodeGuru)
// I modified it to offset the rectangle from the text instead of from the right
// hand side.
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortedHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CSortedHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSortedHeaderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


const int CSortedHeaderCtrl::NOT_SORTED = -1;


CSortedHeaderCtrl::CSortedHeaderCtrl()
{
	m_nSortCol = NOT_SORTED;
	m_bSortAsc = TRUE;
}


CSortedHeaderCtrl::~CSortedHeaderCtrl()
{
}


int CSortedHeaderCtrl::SetSortImage(int nCol, BOOL bAsc)
{
	int nPrevCol = m_nSortCol;

	m_nSortCol = nCol;
	m_bSortAsc = bAsc;

	SetOwnerDraw(nCol);

	// Invalidate header control so that it gets redrawn
	Invalidate();

	return nPrevCol;
}


BOOL CSortedHeaderCtrl::IsAscSorted()
{
	return m_bSortAsc;
}


int CSortedHeaderCtrl::GetSortedColumn()
{
	return m_nSortCol;
}


void CSortedHeaderCtrl::SetOwnerDraw(int nCol)
{
	HD_ITEM hditem;
	
	hditem.mask = HDI_FORMAT;
	GetItem(nCol, &hditem);
	hditem.fmt |= HDF_OWNERDRAW;
	SetItem(nCol, &hditem);
}


void CSortedHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// Get the column rect
	CRect rcLabel(lpDrawItemStruct->rcItem);
	
	// Draw the background
	CBrush Three3Face(::GetSysColor(COLOR_3DFACE));

    pDC->FillRect(rcLabel, &Three3Face);	

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1).cx * 2;

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;	

	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;
	
	GetItem(lpDrawItemStruct->itemID, &hditem);

	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS ;

	if (hditem.fmt & HDF_CENTER)
	{
		uFormat |= DT_CENTER;
	}
	else if (hditem.fmt & HDF_RIGHT)
	{
		uFormat |= DT_RIGHT;
	}
	else
	{
		uFormat |= DT_LEFT;
	}

	// Adjust the rect if the mouse button is pressed on it
	if (lpDrawItemStruct->itemState == ODS_SELECTED)
	{
		rcLabel.left++;
		rcLabel.top += 2;
		rcLabel.right++;
	}

#if 0
	// SAK not needed anymore
	// Adjust the rect further if Sort arrow is to be displayed
	if (lpDrawItemStruct->itemID == static_cast<UINT>(m_nSortCol))
	{
		rcLabel.right -= 2 * offset;
	}
#endif
	rcLabel.left += offset;
	rcLabel.right -= offset;
	
	// Draw column label
	pDC->DrawText(buf, -1, rcLabel, uFormat);

	// Draw the Sort arrow
	if (lpDrawItemStruct->itemID == static_cast<UINT>(m_nSortCol))
	{
		CRect rcIcon(rcLabel);
		const int nArrow = 6;

		if ((uFormat & DT_RIGHT) == 0)
		{
			pDC->DrawText(buf, -1, rcIcon, uFormat | DT_CALCRECT);
			// Normal case, left aligned text.
			rcIcon.OffsetRect(offset * 5, 0);
		}
		else
		{
			rcIcon.right = rcIcon.left + nArrow + 2 * offset;
		}

		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
		CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		CPen *pOldPen = pDC->SelectObject(&penLight);

		if (m_bSortAsc)
		{
			// Draw triangle pointing upwards
			pDC->MoveTo(rcIcon.right - 2*nArrow, nArrow - 1);
			pDC->LineTo(rcIcon.right - 3*nArrow/2 + 1, rcIcon.bottom - nArrow);
			pDC->LineTo(rcIcon.right - 5*nArrow/2-2, rcIcon.bottom - nArrow);

			pDC->SelectObject(&penShadow);
			pDC->MoveTo(rcIcon.right - 5*nArrow/2 - 2, rcIcon.bottom - nArrow);
			pDC->LineTo(rcIcon.right - 2*nArrow, nArrow-2);
		}
		else	
		{
			// Draw triangle pointing downwards
			pDC->MoveTo(rcIcon.right - 3*nArrow/2 + 1, nArrow-1);
			pDC->LineTo(rcIcon.right - 2*nArrow-1, rcIcon.bottom - nArrow + 1);

			pDC->SelectObject(&penShadow);
			pDC->MoveTo(rcIcon.right - 2*nArrow - 1, rcIcon.bottom - nArrow);
			pDC->LineTo(rcIcon.right - 5*nArrow/2 - 2, nArrow - 1);
			pDC->LineTo(rcIcon.right - 3*nArrow/2 + 1, nArrow - 1);
		}
		
		// Restore the pen
		pDC->SelectObject(pOldPen);
	}
}

