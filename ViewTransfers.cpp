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

#include "TransfersUp.h"
#include "TransfersDown.h"

#include "ViewTransfers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewTransfers, CFormView)

CViewTransfers::CViewTransfers()
	: CFormView(CViewTransfers::IDD)
{
	m_pDoc = NULL;

	//{{AFX_DATA_INIT(CViewTransfers)
	//}}AFX_DATA_INIT
}

CViewTransfers::~CViewTransfers()
{
	delete m_tabDownloads;
	delete m_tabUploads;
}

void CViewTransfers::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// Delete to view list
	m_pDoc->m_pViewTransfers = NULL;
}


void CViewTransfers::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTransfers)
	DDX_Control(pDX, IDC_PSHEET_TRANSFER, m_tabTransfers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTransfers, CFormView)
	//{{AFX_MSG_MAP(CViewTransfers)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CViewTransfers::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewTransfers::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CViewTransfers::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_pDoc  = (CGnucleusDoc*) GetDocument();

	// Add to view list
	m_pDoc->m_pViewTransfers = GetSafeHwnd();


	// Setup image list
	m_ImageLst.Create (16, 16, ILC_COLOR8 | TRUE, 0, 10);
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_TRANSFERS_DOWN));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_TRANSFERS_UP));

	// Property Sheets
	m_tabDownloads = new CTransfersDown(this);
	m_pSheet.AddPage(m_tabDownloads);
	m_tabUploads = new CTransfersUp(this);
	m_pSheet.AddPage(m_tabUploads);
	
	CreatePropSheet(this, &m_pSheet);
	SizePropSheet(this, &m_pSheet, IDC_PSHEET_TRANSFER);
	
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

	CRect rect;
	GetParentFrame()->GetClientRect(&rect);
	OnSize(SIZE_RESTORED, rect.right - 4, rect.bottom - 4);	
}

void CViewTransfers::OnSize(UINT nType, int cx, int cy) 
{
	if( cx < 300 || cy < 200)		// Too small and formatting goes to hell.
		return;						// so just ignore the size change.

	CFormView::OnSize(nType, cx, cy);

	if(m_tabTransfers.m_hWnd != NULL)
	{
		int top_tabTransfers, left_tabTransfers;

		RECT rect_Wnd,         
			 rect_tabTransfers;

		GetWindowRect(&rect_Wnd);
		m_tabTransfers.GetWindowRect(&rect_tabTransfers);

		top_tabTransfers  = rect_tabTransfers.top  - rect_Wnd.top;
		left_tabTransfers = rect_tabTransfers.left - rect_Wnd.left;

		m_tabTransfers.MoveWindow(left_tabTransfers, top_tabTransfers, cx - left_tabTransfers , cy - top_tabTransfers );
		SizePropSheet(this, &m_pSheet, IDC_PSHEET_TRANSFER);
	}
}

CGnucleusDoc* CViewTransfers::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGnucleusDoc)));
	return (CGnucleusDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// Property Sheet 

BOOL CViewTransfers::CreatePropSheet(CWnd* pParent, CPropertySheet* pPSheet)
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

void CViewTransfers::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, UINT nSheetID)
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

void CViewTransfers::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, CRect rcNewPos)
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

void CViewTransfers::OnDownloadUpdate(int DownloadID)
{
	m_tabDownloads->OnUpdate(DownloadID);
}

void CViewTransfers::OnUploadUpdate(int UploadID)
{
	m_tabUploads->OnUpdate(UploadID);
}

void CViewTransfers::ActivateDownloadPane()
{
	m_pSheet.SetActivePage(0);
}

void CViewTransfers::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
BOOL CViewTransfers::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CFormView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}
