// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"
#include "resource.h"
#include "SearchResultsCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CListCtrlEx::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	CColumnClickMixIn<CListCtrlEx>::PreSubclassWindow(this);
}

//
//  FUNCTION:   DrawListViewItem(const LPDRAWITEMSTRUCT)
//
//  PURPOSE:    Draws one item in the listview control.
//
//  PARAMETERS:
//      lpDrawItem - Pointer to the information needed to draw the item.  The
//                   item number is in the itemID member.
//

void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItem) 
{
	CDrawItemHelper drawItem(lpDrawItem);
	CDrawItemIter iter(drawItem);

	for (iter.Begin(); iter; ++iter)
	{
		iter.DrawCell();
	}
}


void CListCtrlEx::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CColumnClickMixIn<CListCtrlEx>::SetSortColumn(pNMListView->iSubItem);

	*pResult = 0;
}

class CDrawItemIter;

 
CDrawItemHelper::CDrawItemHelper(LPDRAWITEMSTRUCT lpDrawItem)
{
	m_lpDrawItem = lpDrawItem;
	m_cxImage = 0;
	m_cyImage = 0;
//	m_uFirstColWidth = 0;
	m_iColumn = 1;
	m_uiFlags = ILD_TRANSPARENT;
	Setup();
}


CDrawItemHelper::~CDrawItemHelper()
{
	TearDown();
}


void CDrawItemHelper::Setup()
{
	// Get the item image to be displayed
	memset(&m_lvi, 0, sizeof(m_lvi));

	m_lvi.mask = LVIF_IMAGE | LVIF_STATE;
	m_lvi.iItem = m_lpDrawItem->itemID;
	m_lvi.iSubItem = 0;
	m_lvi.stateMask = 0xFFFF;		// get all state flags
	ListView_GetItem(m_lpDrawItem->hwndItem, &m_lvi);
	
	int cxColumn = ListView_GetColumnWidth(m_lpDrawItem->hwndItem, 0);

	HIMAGELIST hIconImage = ListView_GetImageList(m_lpDrawItem->hwndItem, LVSIL_SMALL);
	if (hIconImage)
	{
		// Find out how big the image we are about to draw is
		ImageList_GetIconSize(hIconImage, &m_cxImage, &m_cyImage);
	}

	// Calculate the width of the first column after the image width.  If
	// There was no image, then cxImage will be zero.

	m_rcClip = m_lpDrawItem->rcItem;

	// Should the item be highlighted
	bool bHighlight = (m_lpDrawItem->itemState & (ODS_SELECTED | ODS_FOCUS)) ? true : false;

	CBrush MyBrush;
	if (bHighlight)
	{
		// Set the text background and foreground colors
		::SetTextColor(m_lpDrawItem->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
		::SetBkColor(m_lpDrawItem->hDC, GetSysColor(COLOR_HIGHLIGHT));

		if (GetFocus() == m_lpDrawItem->hwndItem) 
		{
			MyBrush.CreateSysColorBrush(COLOR_HIGHLIGHT);
		}
		else
		{
			MyBrush.CreateSysColorBrush(COLOR_3DFACE);
		}
	}
	else
	{
		// Set the text background and foreground colors to the standard window
		// colors
		::SetTextColor(m_lpDrawItem->hDC, GetSysColor(COLOR_WINDOWTEXT));
		::SetBkColor(m_lpDrawItem->hDC, GetSysColor(COLOR_WINDOW));

		MyBrush.CreateSysColorBrush(COLOR_WINDOW);
	}

	::FillRect(m_lpDrawItem->hDC, &m_rcClip, MyBrush);

#if 0
	// Set up the new clipping rect for the first column text and draw it
	m_rcClip.left   = m_lpDrawItem->rcItem.left + k_nGapX;
	if (m_cxImage)
	{
		m_rcClip.left += m_cxImage + k_nGapX;
	}
#endif

	m_rcClip.right  = m_lpDrawItem->rcItem.left + cxColumn;
	m_rcClip.top    = m_lpDrawItem->rcItem.top;
	m_rcClip.bottom = m_lpDrawItem->rcItem.bottom;
}


void CDrawItemHelper::TearDown()
{
	// If we changed the colors for the selected item, undo it
	if (m_lpDrawItem->itemState & ODS_SELECTED)
	{
		// Set the text background and foreground colors
		::SetTextColor(m_lpDrawItem->hDC, GetSysColor(COLOR_WINDOWTEXT));
		::SetBkColor(m_lpDrawItem->hDC, GetSysColor(COLOR_WINDOW));
	}

	// If the item is focused, now draw a focus rect around the entire row
	if (m_lpDrawItem->itemState & ODS_FOCUS)
	{
		// Adjust the left edge to exclude the image
		m_rcClip = m_lpDrawItem->rcItem;
		m_rcClip.left += m_cxImage;

		// Draw the focus rect
		DrawFocusRect(m_lpDrawItem->hDC, &m_rcClip);
	}
}


CDrawItemIter::CDrawItemIter(CDrawItemHelper& drawItemHelper, CObject *pParent)
		: m_DrawItemHelper(drawItemHelper), m_lvi(drawItemHelper.m_lvi)
{
	this->pParent = pParent;
	m_nColumn   = 0;
	m_lvc.mask  = LVCF_FMT | LVCF_WIDTH;

	m_rcCol = m_DrawItemHelper.m_rcClip;
	m_hWnd  = m_DrawItemHelper.m_lpDrawItem->hwndItem;
	m_hDc   = m_DrawItemHelper.m_lpDrawItem->hDC;

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character

	CSize textSize;
	::GetTextExtentPoint(m_hDc, _T(" "), 1, &textSize);

	m_nOffset = textSize.cx * 2;

	HWND hHeader = ListView_GetHeader(m_hWnd);

	m_nNumColumns = Header_GetItemCount(hHeader);

	m_Brush.CreateSysColorBrush(COLOR_WINDOW);
}


void CDrawItemIter::operator ++()
{
	++m_nColumn;

	int nWidth;
	nWidth = ListView_GetColumnWidth(m_hWnd, m_nColumn);

	m_rcCol.left = m_rcCol.right;
	m_rcCol.right += nWidth;
}


void CDrawItemIter::DrawCell()
{
	if (m_nColumn == 0)
	{
		DrawIcons();
	}

	CString sLabel;
	ListView_GetItemText(m_hWnd, 
		m_DrawItemHelper.m_lpDrawItem->itemID, 
		m_nColumn,
		sLabel.GetBuffer(256), 255);
	sLabel.ReleaseBuffer();

	m_lvc.iSubItem = m_nColumn;
	ListView_GetColumn(m_hWnd,
			m_nColumn,
			&m_lvc);

	if (sLabel.GetLength() > 0)
	{
		// Get the text justification
		UINT nJustify = DT_LEFT;

		switch(m_lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}

		CRect rcLabel = m_rcCol;
		rcLabel.left  += m_nOffset;
		rcLabel.right -= m_nOffset;

		if (m_nColumn == 0)
		{
			rcLabel.left += m_DrawItemHelper.m_cxImage;
		}

		::DrawText(m_hDc,
			sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
					DT_NOPREFIX | DT_VCENTER | DT_WORD_ELLIPSIS);
	}
}


void CDrawItemIter::DrawIcons()
{
	UINT uStyle = ILD_TRANSPARENT;

	if (m_DrawItemHelper.m_lpDrawItem->itemState & (ODS_FOCUS | ODS_SELECTED))
	{
		uStyle = ILD_FOCUS;
	}

	CRect rcLabel = m_rcCol;
//	rcLabel.left = m_nOffset;

	// Draw state icon
	if (m_lvi.state & LVIS_STATEIMAGEMASK)
	{
		int nImage = ((m_lvi.state & LVIS_STATEIMAGEMASK) >> 12) - 1;

		HIMAGELIST hStateIcon = ListView_GetImageList(m_hWnd, LVSIL_STATE);
		ASSERT(hStateIcon);

		if (hStateIcon)
		{
			VERIFY(ImageList_Draw(hStateIcon, nImage, m_hDc,
				rcLabel.left, rcLabel.top,
				uStyle));
		}
	}
	else
	{
		// Get the image list and draw the image
		HIMAGELIST hIconImage = ListView_GetImageList(m_hWnd, LVSIL_SMALL);
		if (hIconImage)
		{
			ImageList_Draw(hIconImage, m_lvi.iImage, m_hDc,
				   rcLabel.left, rcLabel.top,
				   uStyle);
		}
	}
	
	////Makslane: changes icon appearance 
	if(pParent && pParent->IsKindOf(RUNTIME_CLASS(CSearchResultsCtrl)))
	{
		CSearchResultsCtrl *pResultCtrl = (CSearchResultsCtrl *)pParent;		
		CSearchResultsCtrl::mapIntGroup::iterator iter = pResultCtrl->mapVisible.find(m_DrawItemHelper.m_lpDrawItem->itemID - pResultCtrl->GetScrollPos(SB_VERT));
		
		if(iter != pResultCtrl->mapVisible.end())
		{		
			SearchResult *pGroup = iter->second;
		
			if(pGroup)
				if(pGroup->State != RESULT_INACTIVE)
				{
					CDC *pDC = CDC::FromHandle(m_hDc);
					HICON hIcon = NULL;
					
					switch(pGroup->State)
					{
					case RESULT_TRYING:
						hIcon = AfxGetApp()->LoadIcon(IDR_RESULT_TRYING);
						break;
					case RESULT_DOWNLOADING:
						hIcon = AfxGetApp()->LoadIcon(IDR_RESULT_DOWNLOADING);
						break;
					case RESULT_COMPLETED:
						hIcon = AfxGetApp()->LoadIcon(IDR_RESULT_COMPLETED);
						break;
					case RESULT_NOSOURCES:
						hIcon = AfxGetApp()->LoadIcon(IDR_RESULT_NOHOSTS);
						break;
					}
						
					
					if(hIcon)
						pDC->DrawIcon(rcLabel.left + (rcLabel.bottom - rcLabel.top) - 7, rcLabel.bottom - 6, hIcon);
				}
		}
	}
}

