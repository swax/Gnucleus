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
	license your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/


#include "stdafx.h"
#include "Gnucleus.h"
#include "GnucleusDoc.h"
#include "ViewSearch.h"

#include "AutSearch.h"

#include "FrameMain.h"

#include <algorithm>
#include <functional>
#include <assert.h>

#include "SearchResultsCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSearchResultsCtrl, CListCtrl);

int CSearchResultsCtrl::SortBy = 0;

CSearchResultsCtrl::CSearchResultsCtrl()
{
	bChangeData  = true;	
	m_SyncTotals = true;
}

CSearchResultsCtrl::~CSearchResultsCtrl()
{
	m_data.clear();
	mapSel.clear();
	mapVisible.clear();
}


BEGIN_MESSAGE_MAP(CSearchResultsCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSearchResultsCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(LVN_ODSTATECHANGED, OnOdstatechanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSearchResultsCtrl::MyInit(CViewSearch* pView)
{
	m_pView     = pView;
	m_pDoc      = pView->m_pDoc;
	m_autSearch = pView->m_autSearch;

	// Results Listbox
	//int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 3;
	int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 30;	// fix me!

	CRect rect;
	GetClientRect(&rect);

	int nWidth = rect.Width() - offSet;
	
	InsertColumn(eNameCol, "Name", LVCFMT_LEFT,			 nWidth * 8./18, 0);
	InsertColumn(eSizeCol, "Size", LVCFMT_RIGHT,		 nWidth * 2./18, 1);
	InsertColumn(eTypeCol, "Type", LVCFMT_LEFT,			 nWidth * 4./18, 2);
	InsertColumn(eSpeedCol,"Speed", LVCFMT_RIGHT,		 nWidth * 2./18, 3);
	InsertColumn(eDistCol, "Distribution", LVCFMT_RIGHT, nWidth * 2./18, 4);

	SetExtendedStyle(LVS_EX_FULLROWSELECT);

	SetImageList(GetSharedImageList(), LVSIL_SMALL);

	SortBy = -5;

	m_sync_needed = false;
	SetTimer(145, 1000, NULL);
}

void CSearchResultsCtrl::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*) pNMHDR;
	LV_ITEM*     pItem     = &(pDispInfo)->item;

	if(pItem->iItem == 0 && pItem->iSubItem == 0)
		if (pItem->mask & LVIF_TEXT)
		{
			CString Good  = CommaIze(DWrdtoStr(m_autSearch->CountGoodResults(m_pView->m_SearchID))),
					Total = CommaIze(DWrdtoStr(m_autSearch->CountTotalResults(m_pView->m_SearchID)));
			
			CString Message = Good + " of " + Total + " Files";
			lstrcpy(pItem->pszText, Message);
		}

	if (pItem->iItem < 2 || pItem->iItem >= m_data.size() + 2)
	{
		mapVisible[pItem->iItem - GetScrollPos(SB_VERT)] = NULL;
		*pResult = 0;
		return;
	}

	UINT ByteSize;
	UINT KbSize;

	SearchResult* Result = m_data[pItem->iItem - 2];
	
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		ASSERT(pItem->cchTextMax > 200);
		
		switch (GetHeaderCtrl()->OrderToIndex(pItem->iSubItem))
		{
		case eNameCol:
			lstrcpy(pItem->pszText, Result->Name);

			//Makslane: assures that the item appeared selected correctly 
			if(mapSel.find(Result) != mapSel.end())
				SetItemState(pItem->iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED, false);
			else
				ListView_SetItemState(m_hWnd, pItem->iItem, 0, -1);			
			
			//Makslane: store the item based on index relative to scroll
			mapVisible[pItem->iItem - GetScrollPos(SB_VERT)] = Result;
			break;

		case eSizeCol:
			ByteSize = Result->Size;
			KbSize   = ByteSize / 1024;

			if (KbSize == 0)
			{
				lstrcpy(pItem->pszText, CommaIze(DWrdtoStr(ByteSize)) + " Bytes");
			}
			else
			{
				lstrcpy(pItem->pszText, CommaIze(DWrdtoStr(KbSize)) + " KB");
			}

			break;

		case eTypeCol:
			lstrcpy(pItem->pszText, Result->Icon.Type);

			break;

		case eSpeedCol:
			lstrcpy(pItem->pszText, CommaIze(DWrdtoStr(Result->AvgSpeed)) + " KB/s");
			
			break;

		case eDistCol:
			{
			int HostCount = Result->HostCount;

			const TCHAR* pPlural = (HostCount == 1) ? " Host" : " Hosts";

			lstrcpy(pItem->pszText, DWrdtoStr(HostCount) + pPlural);

			break;
			}

		/*
		case eHashCol:
			lstrcpy(pItem->pszText, (pGroup->Hash == "") ? "No" : "Yes");
			break;

		case eInfoCol:
			lstrcpy(pItem->pszText, pGroup->Info);
			break;
		*/

		default:
			ASSERT(0); // Unknown column
		}
	}	

	if (pItem->mask & LVIF_IMAGE) //valid image?
        pItem->iImage = Result->Icon.Index;

	*pResult = 0;
}

void CSearchResultsCtrl::Insert(SearchResult* pResult)
{
	m_sync_needed = true;
	
	for(dataset::iterator it = m_data.begin(); it != m_data.end(); it++)
		if(*it == pResult)
		{
			m_data.erase(it);
			break;
		}

	m_data.insert(std::equal_range(m_data.begin(), m_data.end(), pResult, ResultComp).first, pResult);
}

void CSearchResultsCtrl::Insert(std::list<SearchResult> &ResultList)
{
	std::list<SearchResult>::iterator itResult;
	for(itResult = ResultList.begin(); itResult != ResultList.end(); itResult++)
		m_data.push_back(&(*itResult));

	std::sort(m_data.begin(), m_data.end(), ResultComp);

	m_sync_needed = true;
}

bool ResultComp( SearchResult* first, SearchResult* second )
{
	bool bResult = false;


	if (CSearchResultsCtrl::SortBy < 0)
	{
		switch(abs(CSearchResultsCtrl::SortBy))
		{
		// Name
		case 1:
			if(first->NameLower > second->NameLower)
				bResult = true;
			else if(first->NameLower == second->NameLower)
				bResult = (first->Size > second->Size);
			break;

		// Size
		case 2:
			if(first->Size > second->Size)
				bResult = true;
			else if(first->Size == second->Size)
				bResult = (first->NameLower > second->NameLower);
			break;

		// Type
		case 3:
			if(first->Icon.Type > second->Icon.Type)
				bResult = true;
			else if(first->Icon.Type == second->Icon.Type)
				bResult = (first->NameLower > second->NameLower);
			break;

		// Speed
		case 4:
			if(first->AvgSpeed > second->AvgSpeed)
				bResult = true;
			else if(first->AvgSpeed == second->AvgSpeed)
				bResult = (first->NameLower > second->NameLower);
			break;

		// Host
		case 5:
			if(first->HostCount > second->HostCount)
				bResult = true;
			else if(first->HostCount == second->HostCount)
				bResult = (first->NameLower > second->NameLower);
			break;

		default:
			bResult = (first->NameLower > second->NameLower);
			break;
		}
	}
	else
	{
		switch (abs(CSearchResultsCtrl::SortBy))
		{
		// Name
		case 1:
			if(first->NameLower < second->NameLower)
				bResult = true;
			else if(first->NameLower == second->NameLower)
				bResult = (first->Size < second->Size);
			break;

		// Size
		case 2:
			if(first->Size < second->Size)
				bResult = true;
			else if(first->Size == second->Size)
				bResult = (first->NameLower < second->NameLower);
			break;

		// Type
		case 3:
			if(first->Icon.Type < second->Icon.Type)
				bResult = true;
			else if(first->Icon.Type == second->Icon.Type)
				bResult = (first->NameLower < second->NameLower);
			break;

		// Speed
		case 4:
			if(first->AvgSpeed < second->AvgSpeed)
				bResult = true;
			else if(first->AvgSpeed == second->AvgSpeed)
				bResult = (first->NameLower < second->NameLower);
			break;

		// Host
		case 5:
			if(first->HostCount < second->HostCount)
				bResult = true;
			else if(first->HostCount == second->HostCount)
				bResult = (first->NameLower < second->NameLower);
			break;

		default:
			bResult = (first->NameLower < second->NameLower);
			break;
		}
	}

	return bResult;
}


void CSearchResultsCtrl::ClearList()
{
	m_data.clear();

	m_sync_needed = true;
} 

////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @parm    
// @parm    
// @comm    
// 
// @devnote 
// 
// 2001-06-29 - Scott Kirkwood <nl>
//   Reverse no longer required. The column click mix-in handles all that.
// 
void CSearchResultsCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	int selidx = pNMListView->iSubItem + 1;

	if (abs(SortBy) == selidx)
		SortBy = -SortBy ;
	else
	{
		switch(abs(selidx))
		{
		case 1:
			SortBy = selidx;
			break;
		case 2:
			SortBy = -selidx;
			break;
		case 3:
			SortBy = selidx;
			break;
		case 4:
			SortBy = -selidx;
			break;
		case 5:
			SortBy = -selidx;
			break;
		}
	}

	std::sort(m_data.begin(), m_data.end(), ResultComp);

	SyncList();
}

void CSearchResultsCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();
}

void CSearchResultsCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItem) 
{
	CDrawItemHelper drawItem(lpDrawItem);
	CDrawItemIter iter(drawItem, this);

	for (iter.Begin(); iter; ++iter)
	{
		iter.DrawCell();
	}
}

void CSearchResultsCtrl::OnTimer(UINT nIDEvent) 
{
	if (m_sync_needed)
		SyncList();
}

void CSearchResultsCtrl::SyncList()
{
	m_sync_needed = false;

	if(GetItemCount() != m_data.size() + 2)
		SetItemCountEx(m_data.size() + 2, LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);

	if(!m_data.size())
		return;
	
	int TopPos    = 0,
		BottomPos = 0;

	if(GetTopIndex() == 0)
		BottomPos = GetCountPerPage() - 2;
	else if(GetTopIndex() == 1)
		BottomPos = GetCountPerPage() - 1;
	else
	{
		TopPos    = GetTopIndex() - 2;
		BottomPos = TopPos + GetCountPerPage();
	}

	static dataset thisview;
	thisview.clear();

	dataset::iterator top    = TopPos    >= m_data.size() ? m_data.end() : m_data.begin() + TopPos;
	dataset::iterator bottom = BottomPos >= m_data.size() ? m_data.end() : m_data.begin() + BottomPos;

	CRect itemRect;

	int ListPos = GetTopIndex();


	SetRedraw(false);
	
	// Update totals in title if needed
	if(ListPos == 0 && m_SyncTotals)
	{
		m_SyncTotals = false;
		GetSubItemRect(0, 0, LVIR_BOUNDS, itemRect);
		InvalidateRect(itemRect, false);
	}

	if(ListPos < 2)
		ListPos = 2;

	
	for(dataset::iterator itThis = top, itLast = m_lastview.begin(); itThis != bottom; itThis++)
	{
		thisview.push_back(*itThis);

		// Check if whole item needs to be changed
		if(itLast >= m_lastview.end() || *itThis != *itLast)
		{
			GetItemRect(ListPos, &itemRect, LVIR_BOUNDS);
			InvalidateRect(itemRect, false);
		}

		// Check if just parts of it need to be changed (icon, speed, dist)
		else if((*itThis)->SyncNeeded)
		{
			GetSubItemRect(ListPos, 0, LVIR_ICON, itemRect);
			InvalidateRect(itemRect, false);
		
			GetSubItemRect(ListPos, 3, LVIR_BOUNDS, itemRect);
			InvalidateRect(itemRect, false);

			GetSubItemRect(ListPos, 4, LVIR_BOUNDS, itemRect);
			InvalidateRect(itemRect, false);
		}
		
		(*itThis)->SyncNeeded = false;

		if(itLast < m_lastview.end())
			itLast++;

		ListPos++;
	}

	SetRedraw(true);

	

	/*if (thisview != m_lastview)
	{
		this->SetRedraw(false);
		RedrawItems(0, m_data.size() + 2);
		this->SetRedraw(true);
	}*/

	m_lastview = thisview;	
}

SearchResult* CSearchResultsCtrl::GetItem(int idx)
{
	idx -= 2;

	if (idx < m_data.size())
		return m_data[idx];		
	
	return NULL;
}

//Makslane: manages individuals selections/deselections
void CSearchResultsCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->iItem != -1 && (pNMListView->iItem < 2 || pNMListView->iItem >= m_data.size() + 2))
	{
		*pResult = 0;
		return;
	}
	
	SearchResult* pSearchResult = NULL;
	if(pNMListView->iItem != -1)
		pSearchResult = m_data[pNMListView->iItem - 2];

	if (bChangeData && (pNMListView->uChanged & LVIF_STATE) == LVIF_STATE )
	{
		if((pNMListView->uOldState & LVIS_SELECTED) && !(pNMListView->uNewState & LVIS_SELECTED))
		{
			if(pNMListView->iItem >= 2)
			{				
				mapSel.erase(pSearchResult);
				//TRACE("Item %ld deselected (%ld)\n", pNMListView->iItem, mapSel.size());				
			}
			else if(pNMListView->iItem == -1)
			{
				mapSel.clear();
				//TRACE("All itens deselected (%ld)\n", mapSel.size());
			}
		}
		
		if(pNMListView->iItem >= 2 && (pNMListView->uNewState & LVIS_SELECTED))
		{
			mapSel[pSearchResult] = 1;
			//TRACE("Item %ld selected (%ld)\n", pNMListView->iItem, mapSel.size());
		}
	}
	
	*pResult = 0;
}

//Makslane: safe item state modification
void CSearchResultsCtrl::SetItemState(int iItem, UINT uiState, UINT uiMask, bool bChangeData)
{
	LV_ITEM lvi;
	
	memset(&lvi, 0, sizeof(LV_ITEM));
	lvi.mask		= LVIF_STATE;
	lvi.iItem		= iItem;
	lvi.stateMask	= uiMask;
	lvi.state		= uiState;

	this->bChangeData = bChangeData;
	::SendMessage(m_hWnd, LVM_SETITEMSTATE, iItem, (LPARAM)&lvi);
	this->bChangeData = true;
}

//Makslane: manages shifted selections
void CSearchResultsCtrl::OnOdstatechanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVODSTATECHANGE* pStateChanged = (NMLVODSTATECHANGE*)pNMHDR;

	if(pStateChanged->uNewState & LVIS_SELECTED)
	{
		for(int i = pStateChanged->iFrom; i <= pStateChanged->iTo; i++)
		{
			if(i >= 2)
			{
				SearchResult *pResult =  m_data[i-2];
				mapSel[pResult] = 1;
			}
		}

		//TRACE("Itens %ld to %ld selected (%ld)\n", pStateChanged->iFrom, pStateChanged->iTo, mapSel.size());
	}
		
	*pResult = 0;
}



