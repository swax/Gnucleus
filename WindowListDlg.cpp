/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
	Copyright (C) 2000-2005 John Marshall Group

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	By contributing code you grant John Marshall Group an unlimited, non-exclusive
	license to your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/


#include "stdafx.h"
#include "gnucleus.h"
#include "WindowListDlg.h"
#include "FrameConnect.h"
#include "FrameStatistics.h"
#include "FrameShare.h"
#include "FrameChat.h"
#include "FrameTransfers.h"
#include "FrameSearch.h"
#include "FrameBrowser.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWindowListDlg::CWindowListDlg()
{
	//{{AFX_DATA_INIT(CWindowListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Slots = 0L;
}

//void CWindowListDlg::DoDataExchange(CDataExchange* pDX)
//{
	//{{AFX_DATA_MAP(CWindowListDlg)
	//}}AFX_DATA_MAP
//}

BOOL CWindowListDlg::Create(CWnd* pParentWnd, UINT nIDTemplate,	UINT nStyle, UINT nID)
{
	BOOL bRet = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);

	ASSERT(bRet);

	VERIFY(m_ctlWindowList.SubclassDlgItem(IDC_WINDOW_TABS, this));

	m_ImageLst.Create (16, 16, ILC_COLOR8 | TRUE, 0, 10);
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_CONNECT));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_SHARE));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_TRANSFERS));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_CHAT));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_STATISTICS));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_BROWSER));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_SEARCH));

	m_ctlWindowList.SetImageList(&m_ImageLst);

	return bRet;
}

BEGIN_MESSAGE_MAP(CWindowListDlg, CDialogBar)
	//{{AFX_MSG_MAP(CWindowListDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_WINDOW_TABS, OnSelchangeWindowTabs)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWindowListDlg::OnSelchangeWindowTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWnd* pWnd = GetWnd(m_ctlWindowList.GetCurSel());

	if (IsWindow(pWnd->m_hWnd))
	{
		WINDOWPLACEMENT placement = { sizeof(WINDOWPLACEMENT) };

		pWnd->GetWindowPlacement(&placement);

		if (placement.showCmd == SW_HIDE || 
			placement.showCmd == SW_MINIMIZE ||
			placement.showCmd == SW_SHOWMINIMIZED)
		{
			pWnd->ShowWindow(SW_SHOWNORMAL);
		}
		pWnd->BringWindowToTop();
	}
	
	*pResult = 0;
}

 
void CWindowListDlg::Activate(CWnd* pWnd)
{
	if (!IsWindow(m_ctlWindowList.m_hWnd))
	{
		ASSERT(0);
		return;
	}

	int nIndex = MyFind(pWnd);

	if (nIndex == -1)
	{
		nIndex = AddNew(pWnd);
	}
	m_ctlWindowList.SetCurSel(nIndex);
}


void CWindowListDlg::Close(CWnd* pWnd)
{
	int nIndex = MyFind(pWnd);
	
	if (nIndex == -1)
	{
	//	ASSERT(nIndex != -1);
		return;
	}

	int nIcon = GetIconByWindowType(GetWnd(nIndex));

	if (nIcon != eSearch)
	{
		unsigned long nBit = 1;;
		nBit <<= nIcon;
		m_Slots &= ~nBit;
	}
	m_ctlWindowList.DeleteItem(nIndex);
}


int CWindowListDlg::MyFind(CWnd* pWnd)
{
	int nCount = m_ctlWindowList.GetItemCount();

	for (int i = 0; i < nCount; ++i)
	{
		if (GetWnd(i) == pWnd)
		{
			return i;
		}
	}

	return -1; // didn't find it.
}


void CWindowListDlg::Update(CWnd* pWnd)
{
	int nIndex = MyFind(pWnd);

	CString strTitle;
	pWnd->GetWindowText(strTitle);

	TCITEM aItem;
	aItem.mask = TCIF_TEXT;
	aItem.pszText = (char*)(LPCTSTR)strTitle;
	m_ctlWindowList.SetItem(nIndex, &aItem);
}


int CWindowListDlg::AddNew(CWnd* pWnd)
{
	CString strTitle;
	
	pWnd->GetWindowText(strTitle);

	int nIcon = GetIconByWindowType(pWnd);
	int nIndex = FindSlot(nIcon);

	if (nIndex >= 999)
	{
		m_ctlWindowList.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, 999, strTitle, nIcon, (long)pWnd);
		m_ctlWindowList.SetCurSel(nIndex);
	}
	else
	{
		m_ctlWindowList.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, nIndex, strTitle, nIcon, (long)pWnd);
		m_ctlWindowList.SetCurSel(nIndex);
	}
	
	// Little but in CTabCtrl, it's doesn't always repaint the background
	m_ctlWindowList.RedrawWindow();

	return nIndex;
}


int CWindowListDlg::FindSlot(int nIcon)
{
	if (nIcon == eSearch)
	{
		return 999; // At end
	}

	unsigned long nBit = 1;
	int nIndex = 0;

	for (int i = 0; i < nIcon; ++i)
	{
		if (nBit & m_Slots)
		{
			++nIndex; // Counting the number of bits before our position
		}
		nBit <<= 1; // Shift left one
	}

	m_Slots |= nBit; // Fill the slot

	return nIndex; // Return where to place
}


int CWindowListDlg::GetIconByWindowType(CWnd* pWnd)
{
	int nIcon = -1;
	if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameConnect)))
	{
		nIcon = eConnect;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameShare)))
	{
		nIcon = eShare;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameTransfers)))
	{
		nIcon = eTransfers;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameChat)))
	{
		nIcon = eChat;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameStatistics)))
	{
		nIcon = eStatistics;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameBrowser)))
	{
		nIcon = eBrowser;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameSearch)))
	{
		nIcon = eSearch;
	}

	return nIcon;
}


void CWindowListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);

	if (::IsWindow(m_ctlWindowList.m_hWnd))
	{
		CRect win_size;
		GetClientRect(&win_size);
		
		CRect was;
		m_ctlWindowList.GetWindowRect(was);
		ScreenToClient(was);
		win_size.top = was.top;
		win_size.left = was.left;

		CRect lastItem;
		m_ctlWindowList.GetItemRect(m_ctlWindowList.GetItemCount() - 1, lastItem);

		// if (lastItem.right > 0)
		// {
//			win_size.right = win_size.left + lastItem.right + 1;
// 		}
		m_ctlWindowList.MoveWindow(win_size);
	}
}


void CWindowListDlg::OnContextMenu(CWnd*, CPoint point) 
{
	CPoint ptClient(point);
	m_ctlWindowList.ScreenToClient(&ptClient);

	TCHITTESTINFO info;
	info.flags = TCHT_ONITEM;
	info.pt = ptClient;

	int nIndex = m_ctlWindowList.HitTest(&info);

	CWnd* pWnd = NULL;
	if (nIndex != -1)
	{
		CMenu menu;
		menu.LoadMenu(IDR_WNDLIST_RCLICK);
		CMenu *pMenu = menu.GetSubMenu(0);
		ASSERT(pMenu != NULL);

		WINDOWPLACEMENT placement = { sizeof(WINDOWPLACEMENT) };
		pWnd = GetWnd(nIndex);
		pWnd->GetWindowPlacement(&placement);

		if (placement.showCmd == SW_HIDE)
		{
			menu.EnableMenuItem(ID_WNDLIST_CLOSE, MF_BYCOMMAND | MF_GRAYED);
		}
		else if (placement.showCmd == SW_SHOWMAXIMIZED)
		{
			menu.EnableMenuItem(ID_WNDLIST_MAX,   MF_BYCOMMAND | MF_GRAYED);
		}
		else if (placement.showCmd == SW_SHOWMINIMIZED)
		{
			menu.EnableMenuItem(ID_WNDLIST_MIN,   MF_BYCOMMAND | MF_GRAYED);
		}
		else if (m_ctlWindowList.GetCurSel() == nIndex)
		{
			menu.EnableMenuItem(ID_WNDLIST_SHOW,  MF_BYCOMMAND | MF_GRAYED);
		}
		pWnd->GetWindowPlacement(&placement);

		int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
					 point.x, point.y, this);

		switch(res)
		{
		case ID_WNDLIST_CLOSE:
			pWnd->PostMessage(WM_CLOSE);
			break;
		case ID_WNDLIST_MAX:
			placement.showCmd = SW_SHOWMAXIMIZED;
			pWnd->SetWindowPlacement(&placement);
			break;
		case ID_WNDLIST_MIN:
			placement.showCmd = SW_SHOWMINIMIZED;
			pWnd->SetWindowPlacement(&placement);
			break;
		case ID_WNDLIST_SHOW:
			pWnd->ShowWindow(SW_SHOWNORMAL);
			pWnd->BringWindowToTop();
			break;
		}
	}
}


CWnd* CWindowListDlg::GetWnd(int nIndex)
{
	TCITEM aItem;
	aItem.mask = TCIF_PARAM;
	m_ctlWindowList.GetItem(nIndex, &aItem);
	CWnd* pWnd = (CWnd*)aItem.lParam;

	return pWnd;
}