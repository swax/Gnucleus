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

#include "FrameConnect.h"

#include "ConnectBasic.h"
#include "ConnectAdvanced.h"

#include "ViewConnect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewConnect, CFormView)

BEGIN_MESSAGE_MAP(CViewConnect, CFormView)
	//{{AFX_MSG_MAP(CViewConnect)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CViewConnect::CViewConnect()
	: CFormView(CViewConnect::IDD)
{
	m_pDoc  = NULL;

	//{{AFX_DATA_INIT(CViewConnect)
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CViewConnect::~CViewConnect()
{
	TRACE0("*** CViewConnect Deconstructing\n");

	delete m_tabBasic;
	delete m_tabAdvanced;

	m_tabBasic    = NULL;
	m_tabAdvanced = NULL;


	// Deleting view deletes main document
	//m_pDoc->m_pViewConnect = NULL;
}

void CViewConnect::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewConnect)
	DDX_Control(pDX, IDC_PSHEET_CONNECT, m_tabConnect);
	//}}AFX_DATA_MAP
}

void CViewConnect::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//ResizeParentToFit();

	m_pDoc  = (CGnucleusDoc*) GetDocument();

	m_pDoc->m_pViewConnect = GetSafeHwnd();

	// Setup image list
	m_ImageLst.Create (16, 16, ILC_COLOR8 | TRUE, 0, 10);
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_CONNECT_BASIC));
	m_ImageLst.Add(AfxGetApp()->LoadIcon(IDR_CONNECT_ADVANCED));

	// Property Sheets
	m_tabBasic = new CConnectBasic(this);
	m_pSheet.AddPage(m_tabBasic);
	m_tabAdvanced = new CConnectAdvanced(this);
	m_pSheet.AddPage(m_tabAdvanced);
	
	CreatePropSheet(this, &m_pSheet);
	SizePropSheet(this, &m_pSheet, IDC_PSHEET_CONNECT);
	
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

void CViewConnect::OnSize(UINT nType, int cx, int cy) 
{
	if( cx < 300 || cy < 200)		// Too small and formatting goes to hell.
		return;						// so just ignore the size change.

	CFormView::OnSize(nType, cx, cy);

	if(m_tabConnect.m_hWnd != NULL)
	{
		int top_tabConnect, left_tabConnect;

		RECT rect_Wnd,         
			 rect_tabConnect;

		GetWindowRect(&rect_Wnd);
		m_tabConnect.GetWindowRect(&rect_tabConnect);

		top_tabConnect  = rect_tabConnect.top  - rect_Wnd.top;
		left_tabConnect = rect_tabConnect.left - rect_Wnd.left;

		m_tabConnect.MoveWindow(left_tabConnect, top_tabConnect, cx - left_tabConnect , cy - top_tabConnect );
		SizePropSheet(this, &m_pSheet, IDC_PSHEET_CONNECT);
	}
}


BOOL CViewConnect::CreatePropSheet(CWnd* pParent, CPropertySheet* pPSheet)
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

void CViewConnect::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, UINT nSheetID)
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

void CViewConnect::SizePropSheet(CWnd* pParent, CPropertySheet* pPSheet, CRect rcNewPos)
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


#ifdef _DEBUG
void CViewConnect::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewConnect::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGnucleusDoc* CViewConnect::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGnucleusDoc)));
	return (CGnucleusDoc*)m_pDocument;
}
#endif //_DEBUG


void CViewConnect::OnSockUpdate()
{
	if(m_tabAdvanced)
		m_tabAdvanced->OnSockUpdate();

	if(m_tabBasic)
		m_tabBasic->OnSockUpdate();
}



BOOL CViewConnect::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CFormView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}
