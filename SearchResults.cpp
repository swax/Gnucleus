/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2003 John Marshall

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	For support, questions, comments, etc...
	E-Mail:
		swabby@c0re.net

********************************************************************************/


#include "stdafx.h"
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "AutSearch.h"
#include "AutDownload.h"

#include "ViewTransfers.h"
#include "ViewSearch.h"
#include "SearchInfo.h"
#include "SearchAdvanced.h"

#include "SearchResults.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSearchResults, CPropertyPage)

CSearchResults::CSearchResults() : CPropertyPage(CSearchResults::IDD)
{
	
}

CSearchResults::CSearchResults(CViewSearch* pView) : CPropertyPage(CSearchResults::IDD)
{
	m_pView     = pView;
	m_pDoc      = pView->m_pDoc;
	m_autSearch = pView->m_autSearch;
	m_InfoDlg   = NULL;

	//{{AFX_DATA_INIT(CTransfersUp)
	//}}AFX_DATA_INIT
}

CSearchResults::~CSearchResults()
{
	if (m_InfoDlg)
		delete m_InfoDlg;
}

void CSearchResults::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchResults)
	DDX_Control(pDX, IDC_BUTTON_STOPSEARCH, m_btnStopSearch);
	DDX_Control(pDX, IDC_CHECKS_SCREEN, m_chkScreen);
	DDX_Control(pDX, IDC_EDIT_REFINE, m_ebRefine);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_LIST_RESULTS, m_lstResults);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_btnDownload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchResults, CPropertyPage)
	//{{AFX_MSG_MAP(CSearchResults)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, OnButtonConfig)
	ON_EN_CHANGE(IDC_EDIT_REFINE, OnChangeEditRefine)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RESULTS, OnDblclkListResults)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESULTS, OnRclickListResults)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOPSEARCH, OnButtonStopsearch)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_RESULTS, OnKeydownListResults)
	ON_BN_CLICKED(IDC_CHECKS_SCREEN, OnChecksScreen)
	ON_BN_CLICKED(IDC_BUTTON_VIEWTRANSFERS, OnButtonViewTransfers)
	//}}AFX_MSG_MAP
//	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_RESULTS, OnColumnclickListResults)
END_MESSAGE_MAP()


BOOL CSearchResults::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_lstResults.MyInit(m_pView);

	m_chkScreen.SetCheck(true);

	m_DlgResizer.InitResizer(this);
	m_DlgResizer.ListCtrlItem(IDC_LIST_RESULTS);
	m_DlgResizer.SizeItem(IDC_LIST_RESULTS, CDlgResizer::DownAndRight);
	m_DlgResizer.MoveItem(IDC_STATIC_STARNOTE, CDlgResizer::Right);
	m_DlgResizer.MoveItem(IDC_BUTTON_DOWNLOAD, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_VIEWTRANSFERS, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_CONFIG, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_STOPSEARCH, CDlgResizer::DownAndRight);
	m_DlgResizer.Done();

	//m_DlgResizer.DialogIsMinSize(); // Means don't size smaller than original dialog

	SetTimer(146,500,NULL);
	m_status_update_needed = true;

	return TRUE;  
}


void CSearchResults::UpdateList(SearchResult* Result)
{
	m_lstResults.Insert(Result);


	// Check to see if its in the extended info dialog
	if(m_InfoDlg)
		if(m_InfoDlg->m_ResultID == Result->ResultID)
			m_InfoDlg->UpdateInfo();	
}

void CSearchResults::UpdateTotals()
{
	m_status_update_needed = true;
}

void CSearchResults::OnChecksScreen() 
{
	// Set values set in advanced tab
	CString Limit, Value;
	m_pView->m_tabAdvanced->m_cmbSize.GetWindowText(Limit);
	m_pView->m_tabAdvanced->m_ebSize.GetWindowText(Value);
	m_autSearch->FilterSize(m_pView->m_SearchID, GetFilterMode(Limit), atoi(Value) * 1024);

	m_pView->m_tabAdvanced->m_cmbSpeed.GetWindowText(Limit);
	m_pView->m_tabAdvanced->m_ebSpeed.GetWindowText(Value);
	m_autSearch->FilterSpeed(m_pView->m_SearchID, GetFilterMode(Limit), atoi(Value));


	if(m_chkScreen.GetCheck())
		m_autSearch->SetFiltering(m_pView->m_SearchID, true);
	else
		m_autSearch->SetFiltering(m_pView->m_SearchID, false);


	UpdateTotals();
}

void CSearchResults::OnRefresh()
{
	// Clear lists in list control
	m_lstResults.mapSel.clear();
	m_lstResults.mapVisible.clear();


	// Redraw list
	m_lstResults.ClearList();

	m_lstResults.Insert(m_pView->m_ResultList);
	
	m_lstResults.SyncList();
}

int CSearchResults::GetFilterMode(CString Mode) 
{
	if(Mode == "More Than")
		return LIMIT_MORE;

	else if(Mode == "Exactly")
		return LIMIT_EXACTLY;

	else if(Mode == "Less Than")
		return LIMIT_LESS;
	
	return LIMIT_NONE;
}

void CSearchResults::OnButtonConfig() 
{
	// Dispaly Preferences, sharing tab
	m_pDoc->m_nLastPref = PREF_SEARCH;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);
}

void CSearchResults::OnChangeEditRefine() 
{
	CString Refined;
	m_ebRefine.GetWindowText(Refined);

	m_autSearch->RefineSearch(m_pView->m_SearchID, Refined);
}

void CSearchResults::OnButtonDownload() 
{
	CSearchResultsCtrl::mapGroup::iterator  iter;
	for(iter = m_lstResults.mapSel.begin(); iter != m_lstResults.mapSel.end(); iter++)
	{
		SearchResult* pGroup = iter->first;

		pGroup->DownloadID = m_autSearch->DownloadResult(m_pView->m_SearchID, pGroup->ResultID);
	}

	m_lstResults.SyncList();

	//OnButtonDownload(false);
}

void CSearchResults::CancelDownload()
{
	CSearchResultsCtrl::mapGroup::iterator  iter;
	for(iter = m_lstResults.mapSel.begin(); iter != m_lstResults.mapSel.end(); iter++)
	{
		SearchResult* pResult = iter->first;	

		if(!pResult)
			return;

		m_pDoc->m_autDownload->Remove(pResult->DownloadID );
	}

	m_lstResults.SyncList();
}

void CSearchResults::OnDblclkListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonDownload();
	*pResult = 0;
}

void CSearchResults::OnRclickListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstResults.GetSelectionMark();
	if (nItem < 2 || nItem > m_lstResults.GetItemCount() - 1)
	{
		return;
	}

    CMenu menu;
    menu.LoadMenu(IDR_SEARCH_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	std::list<SearchResult>::iterator itGroup;
	POSITION pos = m_lstResults.GetFirstSelectedItemPosition();
	
	
	switch(res)
	{
	case ID_LSTSEARCH_DOWNLOAD:	
		//Makslane:
		OnButtonDownload();
		break;
	
	case ID_LSTSEARCH_CANCEL:	
		CancelDownload();
		break;

	case ID_LSTSEARCH_INFO:
		//Makslane:
		CSearchResultsCtrl::mapGroup::iterator  iter;
		for(iter = m_lstResults.mapSel.begin(); iter != m_lstResults.mapSel.end(); iter++)
		{
			SearchResult* pResult = iter->first;	
			if(pResult)
				ExtendedInfo(pResult);
			
			break;
		}
		break;
	}
		
	*pResult = 0;
}

void CSearchResults::ExtendedInfo(SearchResult* Results)
{
	if(!m_InfoDlg)
	{
		m_InfoDlg = new CSearchInfo(NULL, this, Results->ResultID);

		m_InfoDlg->DoModal();

		delete m_InfoDlg;
		m_InfoDlg = NULL;
	}
}

void CSearchResults::OnTimer(UINT nIDEvent) 
{
	if (!m_status_update_needed)
		return;

	m_status_update_needed = false;

	m_lstResults.m_SyncTotals = true;


	
	
	CString Title;
	
	// Browsing: 11.123.44.32 - 35 Results / 23% Received
	if(m_pView->m_Browsing)
		Title = "Browsing: ";
	
	// Finding: divx - 35 Results
	else
		Title = "Finding: ";
	
	Title += m_pView->m_Search + " - " + CommaIze( DWrdtoStr( m_autSearch->CountGoodResults(m_pView->m_SearchID))) + " Results";

	if(m_pView->m_Browsing)
		Title += " / " +  m_pView->m_BrowseStatus;


	GetParentFrame()->SetWindowText(Title);
}

void CSearchResults::OnButtonStopsearch() 
{
	m_autSearch->PauseSearch(m_pView->m_SearchID);

	m_btnStopSearch.SetWindowText("Search Stopped");
	m_btnStopSearch.EnableWindow(false);
}

void CSearchResults::OnKeydownListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	// Dont know why, but selection always changes when this funtion is called

	// E key
	if(pLVKeyDow->wVKey == 69)
	{
		int nItem = m_lstResults.GetSelectionMark();
		if (nItem < 2 || nItem > m_lstResults.GetItemCount() - 1)
		{
			*pResult = 0;
			return;
		}

		//Makslane:
		CSearchResultsCtrl::mapGroup::iterator  iter;
		for(iter = m_lstResults.mapSel.begin(); iter != m_lstResults.mapSel.end(); iter++)
		{
			SearchResult* pResult = iter->first;	
			if(pResult)
				ExtendedInfo(pResult);
			
			break;
		}
	}


	*pResult = 0;
}

void CSearchResults::OnButtonViewTransfers() 
{
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_TRANSFERS, NULL);

	if(m_pDoc->m_pViewTransfers)
		((CViewTransfers*) CWnd::FromHandle(m_pDoc->m_pViewTransfers))->ActivateDownloadPane();
}
