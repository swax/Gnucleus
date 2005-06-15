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
#include "Gnucleus.h"
#include "GnucleusDoc.h"
#include "FrameMain.h"

#include "AutSearch.h"

#include "SearchResults.h"
#include "SearchAdvanced.h"

#include "ViewSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewSearch, CFormView)

CViewSearch::CViewSearch()
	: CFormView(CViewSearch::IDD)
{
	m_pDoc   = NULL;
	m_autSearch = NULL;

	m_Search      = "Unknown";
	m_SearchID    = 0;

	m_SearchPaused = false;

	m_OverflowTriggered = false;

	m_tabResults = NULL;
	m_tabAdvanced = NULL;

	m_Browsing = false;
	m_BrowseStatus = "Connecting...";
}

CViewSearch::~CViewSearch()
{
	if (m_tabResults != NULL) 
	{
		delete m_tabResults;
		m_tabResults = NULL;
	}

	if (m_tabAdvanced != NULL) 
	{
		delete m_tabAdvanced;
		m_tabAdvanced = NULL;
	}
}

void CViewSearch::OnDestroy() 
{
	CFormView::OnDestroy();

	std::vector<HWND>::iterator itView;
	for(itView = m_pDoc->m_pViewSearch.begin(); itView != m_pDoc->m_pViewSearch.end(); itView++)
		if(*itView == GetSafeHwnd())
			break;

	m_pDoc->m_pViewSearch.erase(itView);
	
	m_autSearch->EndSearch(m_SearchID);
}

void CViewSearch::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSearch)
	DDX_Control(pDX, IDC_PSHEET_SEARCH, m_tabSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSearch, CFormView)
	//{{AFX_MSG_MAP(CViewSearch)
	ON_WM_SIZE()
	ON_COMMAND(PREFERENCES_CHANGE, OnPreferencesChange)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSearch diagnostics

#ifdef _DEBUG
void CViewSearch::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewSearch::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CViewSearch::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_pDoc      = (CGnucleusDoc*) GetDocument();
	m_autSearch = m_pDoc->m_autSearch;


	// Setup image list
	m_ImageLst.Create (16, 16, ILC_COLOR8 | TRUE, 0, 10);
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_SEARCH_RESULTS));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_SEARCH_ADVANCED));


	// Property Sheets
	m_tabResults = new CSearchResults(this);
	m_pSheet.AddPage(m_tabResults);
	m_tabAdvanced = new CSearchAdvanced(this);
	m_pSheet.AddPage(m_tabAdvanced);
	
	CreatePropSheet(this, &m_pSheet);
	SizePropSheet(this, &m_pSheet, IDC_PSHEET_SEARCH);

	m_pSheet.GetTabControl()->SetImageList(&m_ImageLst);


	// Add icons
	TCITEM TabItem;
	TabItem.mask = TCIF_IMAGE;

	for(int i = 0; i < 2; i++)
	{
		TabItem.iImage = i;	
		m_pSheet.GetTabControl()->SetItem( i, &TabItem);
		m_pSheet.SetActivePage(i);
	}
	m_pSheet.SetActivePage(0);

	m_pDoc->m_pViewSearch.push_back( GetSafeHwnd() );


	// Size tabs with controls
	CRect rect;
	GetParentFrame()->GetClientRect(&rect);
	OnSize(SIZE_RESTORED, rect.right - 4, rect.bottom - 4);	
}

void CViewSearch::OnSize(UINT nType, int cx, int cy) 
{
	if( cx < 300 || cy < 200)		// Too small and formatting goes to hell.
		return;						// so just ignore the size change.

	CFormView::OnSize(nType, cx, cy);

	if(m_tabSearch.m_hWnd != NULL)
	{
		int top_tabSearch,	  left_tabSearch;

		RECT rect_Wnd,         
			 rect_tabSearch;

		GetWindowRect(&rect_Wnd);
		m_tabSearch.GetWindowRect(&rect_tabSearch);

		top_tabSearch  = rect_tabSearch.top  - rect_Wnd.top;
		left_tabSearch = rect_tabSearch.left - rect_Wnd.left;

		m_tabSearch.MoveWindow(left_tabSearch, top_tabSearch, cx - left_tabSearch, cy - top_tabSearch);
		SizePropSheet(this, &m_pSheet, IDC_PSHEET_SEARCH);
	}
}

CGnucleusDoc* CViewSearch::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGnucleusDoc)));
	return (CGnucleusDoc*) m_pDocument;
}


// Propery Sheet functions

BOOL CViewSearch::CreatePropSheet(CWnd* pParent, CPropertySheet* pPSheet)
{
	ASSERT_VALID( pParent );
	ASSERT_VALID( pPSheet );

	if( !pPSheet->Create( pParent, WS_CHILD | WS_VISIBLE, 0))
		return false;

	pPSheet->ModifyStyle(0, WS_TABSTOP);
	pPSheet->ModifyStyle(0, WS_EX_CONTROLPARENT);

	pParent->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	//CTabCtrl* Tab = pPSheet->GetTabControl();
	//Tab->ModifyStyle(NULL, TCS_BUTTONS);

	return true;
}

void CViewSearch::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, UINT nSheetID)
{
	ASSERT_VALID( pParent );
	ASSERT_VALID( pPSheet );

	CRect rcNewPos;
	CWnd* pWndNewArea = pParent->GetDlgItem(nSheetID);

	if(pWndNewArea == NULL)
		return;

	pWndNewArea->GetWindowRect(&rcNewPos);
	pParent->ScreenToClient(&rcNewPos);

	SizePropSheet(pParent, pPSheet, rcNewPos);
}

void CViewSearch::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, CRect rcNewPos)
{
	ASSERT_VALID( pParent );
	ASSERT_VALID( pPSheet );

	CTabCtrl* pTabCtrl = pPSheet->GetTabControl();
	ASSERT(pTabCtrl != NULL);

	CRect rcTabCtrl;
	pTabCtrl->GetWindowRect(&rcTabCtrl);
	pParent->ScreenToClient(&rcTabCtrl);

	CRect rcPSheet;
	pPSheet->GetWindowRect(&rcPSheet);
	pParent->ScreenToClient(&rcPSheet);

	int dcx = rcPSheet.Width() - rcTabCtrl.Width();
	int dcy = rcPSheet.Height() - rcTabCtrl.Height();

	pPSheet->MoveWindow(rcNewPos.left, rcNewPos.top, rcNewPos.Width(), rcNewPos.Height());

	pTabCtrl->SetWindowPos(NULL, -1, 4, rcNewPos.Width() + 4, rcNewPos.Height(), SWP_NOZORDER | SWP_NOACTIVATE);

	int nCurrentPage = pPSheet->GetActiveIndex();
	//for(int i = 0; i < pPSheet->GetPageCount(); ++i)
	//	pPSheet->SetActivePage(i);

	pPSheet->SetActivePage(nCurrentPage);
}

void CViewSearch::OnPreferencesChange()
{
	m_tabResults->OnChecksScreen();
}

void CViewSearch::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CViewSearch::UpdateTitle()
{
	CString Title;
	
	if(m_Browsing)
		Title = "Browsing: ";
	else
		Title =  "Finding: ";
	
	Title += m_Search;
	GetParentFrame()->SetWindowText(Title);

	CFrameMain* pMain = (CFrameMain*) AfxGetMainWnd();
	ASSERT(pMain);
	if (!pMain)
		return;

	pMain->m_wndWindowListDlgBar.Update(GetParent());
	pMain->m_wndWindowListDlgBar.Activate(GetParent());

	Title += ", 0 Results";
	GetParentFrame()->SetWindowText(Title);
}

void CViewSearch::SendQuery()
{
	UpdateTitle();

	m_SearchID = m_autSearch->StartSearch(m_Search);
}

//void CViewSearch::OnPacketIncoming()
//{
//	if(!m_OverflowTriggered && m_WholeList.size() > 2000)
//	{
//		m_OverflowTriggered = true;
//		m_StopResults = true;
//
//
//		if(AfxMessageBox("Warning: Over 2000 results for " + m_Search + ", stop search?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
//			m_StopResults = true;
//		else
//			m_StopResults = false;
//	}
//	
//	if(MessagesPending())
//	{
//		AnalyzeResult( (packet_Log*) m_Messages.front().Packet );
//		
//
//		m_MessageAccess.Lock();
//		m_Messages.pop_front();
//		m_MessageAccess.Unlock();
//	}	
//}

void CViewSearch::OnUpdate(UINT ResultID)
{
	std::map<UINT, SearchResult*>::iterator itResult = m_ResultMap.find(ResultID);

	if(itResult != m_ResultMap.end())
	{
		itResult->second->AvgSpeed	 = m_autSearch->GetResultSpeed(m_SearchID, ResultID);
		itResult->second->HostCount  = m_autSearch->GetResultHostCount(m_SearchID, ResultID);
		itResult->second->State      = m_autSearch->GetResultState(m_SearchID, ResultID);
		
		itResult->second->SyncNeeded = true;

		m_tabResults->UpdateList(itResult->second);
		m_tabResults->UpdateTotals();
	}
}

void CViewSearch::OnResult(UINT ResultID)
{
	// Update view
	m_tabResults->UpdateList( AddResult(ResultID) );
	m_tabResults->UpdateTotals();
}

void CViewSearch::OnRefresh()
{
	m_ResultMap.clear();
	m_ResultList.clear();

	// Get new result ID list
	VARIANT var = m_autSearch->GetResultIDs(m_SearchID);
	SAFEARRAY* psa = var.parray;

	UINT* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 AddResult(nArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);

	// Update view
	m_tabResults->OnRefresh();
}

SearchResult* CViewSearch::AddResult(UINT ResultID)
{
	SearchResult Result;

	Result.ResultID  = ResultID;
	Result.Name      = m_autSearch->GetResultName(m_SearchID, ResultID);
	Result.NameLower = Result.Name;
	Result.NameLower.MakeLower();

	Result.Sha1Hash  = m_autSearch->GetResultHash(m_SearchID, ResultID, HASH_SHA1);
	Result.Size		 = m_pDoc->m_RunningXP ? m_autSearch->GetResultSize2(m_SearchID, ResultID) : m_autSearch->GetResultSize(m_SearchID, ResultID);
	Result.AvgSpeed	 = m_autSearch->GetResultSpeed(m_SearchID, ResultID);
	Result.HostCount = m_autSearch->GetResultHostCount(m_SearchID, ResultID);

	Result.DownloadID = 0;
	Result.Icon		  = m_pDoc->GetIconIndex(Result.Name);
	Result.SyncNeeded = false;
	Result.State      = m_autSearch->GetResultState(m_SearchID, ResultID);

	m_ResultList.push_back(Result);
	m_ResultMap[ResultID] = &m_ResultList.back();

	return &m_ResultList.back();
}


BOOL CViewSearch::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CFormView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}

void CViewSearch::OnBrowseUpdate(int State, int Progress)
{
	if(State == SOCK_CONNECTING)
		m_BrowseStatus = "Connecting...";
	
	else if(State == SOCK_CLOSED && Progress == 0)
		m_BrowseStatus = "Browse Failed";

	else
		m_BrowseStatus = DWrdtoStr(Progress) + "% Received";

	m_tabResults->UpdateTotals();
}

void CViewSearch::OnProgress()
{
	m_tabResults->m_stcNodesSearched.SetWindowText( CommaIze(DWrdtoStr(m_autSearch->CountHostsSearched(m_SearchID))) + " G2 Nodes Searched, Gnutella Unknown");
}

void CViewSearch::OnPaused()
{
	m_SearchPaused = true;

	m_tabResults->m_btnPauseContinue.SetWindowText("Search More");
}
