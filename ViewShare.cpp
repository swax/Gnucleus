/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2002 John Marshall

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
#include "FrameMain.h"

#include "AutShare.h"

#include "ShareExDlg.h"

#include "ViewShare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int  FileItem::SortBy = 0;
bool FileItem::Reverse = true;


IMPLEMENT_DYNCREATE(CViewShare, CFormView)

CViewShare::CViewShare()
	: CFormView(CViewShare::IDD)
{
	//{{AFX_DATA_INIT(CViewShare)
	//}}AFX_DATA_INIT

	TotalFiles	  = 0;
	TotalSize     = 0;
	TotalMatched  = 0;
	TotalUploaded = 0;
}

CViewShare::~CViewShare()
{
}

void CViewShare::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// Remove from view list
	m_pDoc->m_pViewShare = NULL;
}

void CViewShare::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewShare)
	DDX_Control(pDX, IDC_CHECK_STOPHASHING, m_chkStopHashing);
	DDX_Control(pDX, IDC_BUTTON_STARTSHARING, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOPSHARING, m_btnStop);
	DDX_Control(pDX, IDC_LIST_SHARED, m_lstShared);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_HASHSPEED, m_stcHashSpeed);
	DDX_Control(pDX, IDC_SLIDER_CPU, m_sldrCpu);
	DDX_Control(pDX, IDC_STATIC_CPU, m_stcCpu);
}


BEGIN_MESSAGE_MAP(CViewShare, CFormView)
	//{{AFX_MSG_MAP(CViewShare)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SHARED, OnColumnclickListShared)
	ON_BN_CLICKED(IDC_BUTTON_STARTSHARING, OnButtonConfigShare)
	ON_BN_CLICKED(IDC_BUTTON_STOPSHARING, OnButtonStopSharing)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SHARED, OnRclickListShared)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHARED, OnDblclkListShared)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_STOPHASHING, OnCheckStophashing)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RUNSELECTED, OnButtonRunSelected)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SHARED, OnItemchangedListShared)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CPU, OnNMReleasedcaptureSliderCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CPU, OnNMCustomdrawSliderCpu)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CViewShare::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewShare::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CViewShare::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_pDoc     = (CGnucleusDoc*) GetDocument();
	m_autShare = m_pDoc->m_autShare;

	// Shared Files Listbox
	int offSet = m_lstShared.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstShared.GetClientRect(&rect);

	int nWidth = rect.Width() - offSet;

	m_lstShared.InsertColumn( 0, "Name as Shared", LVCFMT_LEFT,  nWidth * 11/25, 0);
	m_lstShared.InsertColumn( 1, "Size",           LVCFMT_RIGHT, nWidth * 3/25, 1);
	m_lstShared.InsertColumn( 2, "Type",           LVCFMT_LEFT,  nWidth * 5/25, 2);
	m_lstShared.InsertColumn( 3, "Searched",       LVCFMT_RIGHT, nWidth * 2/25, 3);
	m_lstShared.InsertColumn( 4, "Uploaded",       LVCFMT_RIGHT, nWidth * 2/25, 4);
	m_lstShared.InsertColumn( 5, "Hashed",         LVCFMT_LEFT,  nWidth * 2/25, 5);


	m_lstShared.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	

	m_lstShared.SetImageList(GetSharedImageList(), LVSIL_SMALL);

	//m_DlgResizer.InitResizer(this);
	m_DlgResizer.SizeItem(IDC_LIST_SHARED, CDlgResizer::DownAndRight);
	m_DlgResizer.ListCtrlItem(IDC_LIST_SHARED);

	m_DlgResizer.MoveItem(IDC_BUTTON_RUNSELECTED,   CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_STOPSHARING,  CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_STARTSHARING, CDlgResizer::Down);
	
	m_DlgResizer.MoveItem(IDC_CHECK_STOPHASHING,   CDlgResizer::DownAndRight);
	
	m_DlgResizer.MoveItem(IDC_STATIC_HASHSPEED, CDlgResizer::DownAndRight);
	m_DlgResizer.MoveItem(IDC_SLIDER_CPU,		CDlgResizer::DownAndRight);
	m_DlgResizer.MoveItem(IDC_STATIC_CPU,		CDlgResizer::DownAndRight);

	m_DlgResizer.Done();


	// Add to view list	
	m_pDoc->m_pViewShare = GetSafeHwnd();

	LoadSharedList();


	if(m_autShare->IsHashingStopped())
		m_chkStopHashing.SetCheck(true);

	if(m_autShare->IsEverythingHashed())
		m_chkStopHashing.EnableWindow(false);

	GetParentFrame()->GetClientRect(&rect);
	PostMessage(WM_SIZE, SIZE_RESTORED, MAKELONG(rect.right - 4, rect.bottom - 4));

	
	// Setup slider
	m_sldrCpu.SetRange(0, 100);
	m_sldrCpu.SetPos( m_autShare->GetHashSpeed() );

	m_stcCpu.SetWindowText( DWrdtoStr(m_autShare->GetHashSpeed()) + "% cpu");


	SetTimer(1, 1000, NULL);
}

void CViewShare::OnShareReload()
{
	LoadSharedList();
}

void CViewShare::LoadSharedList()
{
	m_lstShared.SetRedraw(false);
	m_lstShared.DeleteAllItems();

	FileItem File;
	FullIcon Icon;
	FileList.clear();

	TotalFiles	  = 0;
	TotalSize     = 0;
	TotalMatched  = 0;
	TotalUploaded = 0;

	int ListCount = 0;


	// Get current file IDs and put them into a vector
	_variant_t var = m_autShare->GetFileIDs();
	SAFEARRAY* psa = var.parray;

	UINT* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> FileIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 FileIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);


	for(int i = 0; i < FileIDs.size(); i++)
	{
		if( m_autShare->GetFileDir(FileIDs[i]) == "")
			continue;
		

		File.FileID    = FileIDs[i];

		File.Name	   = m_autShare->GetFileName(FileIDs[i]);
		File.Size	   = m_autShare->GetFileSize(FileIDs[i]);	
		File.Matches   = m_autShare->GetFileMatches(FileIDs[i]);
		File.Uploads   = m_autShare->GetFileUploads(FileIDs[i]);
		File.Hash	   = m_autShare->GetFileHash(FileIDs[i]);


		File.NameLower = File.Name;
		File.NameLower.MakeLower();
		
		Icon = m_pDoc->GetIconIndex(File.Name);
		File.Type	   = Icon.Type;
		File.IconIndex = Icon.Index;
		
		TotalSize     += File.Size / 1024;
		TotalMatched  += File.Matches;
		TotalUploaded += File.Uploads;
		
		m_lstShared.InsertItem(ListCount, File.Name, Icon.Index);
		m_lstShared.SetItemText(ListCount, 1, CommaIze( DWrdtoStr( File.Size / 1024)) + " KB");
		m_lstShared.SetItemText(ListCount, 2, Icon.Type);
		m_lstShared.SetItemText(ListCount, 3, CommaIze( DWrdtoStr( File.Matches)));
		m_lstShared.SetItemText(ListCount, 4, CommaIze( DWrdtoStr( File.Uploads)));
		m_lstShared.SetItemData(ListCount, FileIDs[i]);

		if(File.Hash.IsEmpty())
			m_lstShared.SetItemText(ListCount, 5, "No");
		else
			m_lstShared.SetItemText(ListCount, 5, "Yes");

		FileList.push_back(File);

		ListCount++;
	}


	TotalFiles = ListCount;


	// Totals
	m_lstShared.InsertItem(0, "", -1);
	if(TotalFiles == 1)
		m_lstShared.InsertItem(0, CommaIze( DWrdtoStr(TotalFiles)) + " File Shared", -1);
	else
		m_lstShared.InsertItem(0, CommaIze( DWrdtoStr(TotalFiles)) + " Files Shared", -1);

	m_lstShared.SetItemText(0, 1, CommaIze( DWrdtoStr(TotalSize / 1024) ) + " MB");
	m_lstShared.SetItemText(0, 3, CommaIze( DWrdtoStr(TotalMatched) ));
	m_lstShared.SetItemText(0, 4, CommaIze( DWrdtoStr(TotalUploaded) ));


	m_lstShared.SetRedraw(true);
}

void CViewShare::OnColumnclickListShared(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (FileItem::SortBy == pNMListView->iSubItem)
	{
		FileItem::Reverse = !FileItem::Reverse;
	}
	else
	{
		FileItem::SortBy = pNMListView->iSubItem;
		
		if(FileItem::SortBy == 1 || FileItem::SortBy == 3 || FileItem::SortBy ==4)
			FileItem::Reverse = true;
		else
			FileItem::Reverse = false;
	}


	m_lstShared.SetRedraw (false);


	// Set header
	if(TotalFiles == 1)
		m_lstShared.SetItemText(0, 0, CommaIze( DWrdtoStr(TotalFiles)) + " File Shared");
	else
		m_lstShared.SetItemText(0, 0, CommaIze( DWrdtoStr(TotalFiles)) + " Files Shared");

	m_lstShared.SetItemText(0, 1, CommaIze( DWrdtoStr(TotalSize / 1024) ) + " MB");
	m_lstShared.SetItemText(0, 3, CommaIze( DWrdtoStr(TotalMatched) ));
	m_lstShared.SetItemText(0, 4, CommaIze( DWrdtoStr(TotalUploaded) ));

	

	// File list
	FileList.sort();
	
	int row = 2;
	std::list<FileItem>::iterator it;
	for (it = FileList.begin(); it != FileList.end(); it++)
	{
		if( (*it).Name == "")
			continue;

		// Add to listbox
		m_lstShared.SetItem(row, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, (*it).Name, (*it).IconIndex, 0, LVIS_SELECTED, NULL);
		m_lstShared.SetItemText(row, 1, CommaIze(DWrdtoStr((*it).Size / 1024))+ " KB");
		m_lstShared.SetItemText(row, 2, (*it).Type) ;
		m_lstShared.SetItemText(row, 3, CommaIze( DWrdtoStr( (*it).Matches )));
		m_lstShared.SetItemText(row, 4, CommaIze( DWrdtoStr( (*it).Uploads )));

		if((*it).Hash.IsEmpty())
			m_lstShared.SetItemText(row, 5, "No");
		else
			m_lstShared.SetItemText(row, 5, "Yes");

		m_lstShared.SetItemData(row, (*it).FileID);

		row++;
	}

	m_lstShared.SetRedraw(true);

	
	*pResult = 0;
}

void CViewShare::OnButtonConfigShare() 
{
	// Dispaly Preferences, sharing tab
	m_pDoc->m_nLastPref = PREF_SHARE;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);
}

void CViewShare::RunSelected()
{
	int Running = 0;

	POSITION pos = m_lstShared.GetFirstSelectedItemPosition();	
	while(pos)
	{
		int nItem = m_lstShared.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstShared.GetItemCount())
		{
			CString Path = m_autShare->GetFileDir(m_lstShared.GetItemData(nItem));
		
			if(Path != "")
				ShellExecute(NULL, "open", Path, NULL, NULL, SW_SHOWNORMAL);	
		}
		else
			break;

		if(Running == 5)
			if(AfxMessageBox("Continue opening files?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
				break;
			
		Running++;
	}
}

void CViewShare::OnButtonStopSharing() 
{
	bool FileFound = false;

	// Remove from vector
	POSITION pos = m_lstShared.GetFirstSelectedItemPosition();	
	while(pos)
	{
		pos       = m_lstShared.GetFirstSelectedItemPosition();
		int nItem = m_lstShared.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstShared.GetItemCount())
		{
			int index = m_lstShared.GetItemData(nItem);

			std::list<FileItem>::iterator itFile;
			for(itFile = FileList.begin(); itFile != FileList.end(); itFile++)
				if((*itFile).FileID == index)
				{
					TotalFiles--;
					TotalSize	  -= (*itFile).Size / 1024; 
					TotalMatched  -= (*itFile).Matches;
					TotalUploaded -= (*itFile).Uploads;

					m_autShare->StopSharingFile(index);
					m_lstShared.DeleteItem(nItem);

					FileList.erase(itFile);

					FileFound = true;
					break;
				}
		}
		else
			break;
	}

	if(TotalFiles == 1)
		m_lstShared.SetItemText(0, 0, CommaIze( DWrdtoStr(TotalFiles)) + " File Shared");
	else
		m_lstShared.SetItemText(0, 0, CommaIze( DWrdtoStr(TotalFiles)) + " Files Shared");

	m_lstShared.SetItemText(0, 1, CommaIze( DWrdtoStr(TotalSize / 1024) ) + " MB");
	m_lstShared.SetItemText(0, 3, CommaIze( DWrdtoStr(TotalMatched) ));
	m_lstShared.SetItemText(0, 4, CommaIze( DWrdtoStr(TotalUploaded) ));
}


void CViewShare::OnShareUpdate(UINT FileID)
{
	std::list<FileItem>::iterator itFile;
	for(itFile = FileList.begin(); itFile != FileList.end(); itFile++)
		if( (*itFile).FileID == FileID)
		{
			// Find index in list box
 			LVFINDINFO info;
 			info.flags  = LVFI_PARAM;
 			info.lParam = (LPARAM) FileID;
 
 			int row = m_lstShared.FindItem(&info);

			if(row == -1)
				break;

			
			UINT NewMatches = m_autShare->GetFileMatches(FileID);
			if((*itFile).Matches != NewMatches)
			{
				(*itFile).Matches = NewMatches;
				m_lstShared.SetItemText( row, 3, CommaIze( DWrdtoStr(NewMatches)));
				m_lstShared.SetItemText(0, 3, CommaIze( DWrdtoStr(++TotalMatched)));
			}

		
			UINT NewUploads = m_autShare->GetFileUploads(FileID);
			if((*itFile).Uploads != NewUploads)
			{
				(*itFile).Uploads = NewUploads;
				m_lstShared.SetItemText( row, 4, CommaIze( DWrdtoStr(NewUploads)));
				m_lstShared.SetItemText(0, 4, CommaIze( DWrdtoStr(++TotalUploaded)));
			}

			CString NewHash = m_autShare->GetFileHash(FileID);

			if(NewHash == "")
				m_lstShared.SetItemText(row, 5, "No");
			else
			{
				(*itFile).Hash = NewHash;
				m_lstShared.SetItemText(row, 5, "Yes");
			}
		}
}

void CViewShare::OnRclickListShared(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstShared.GetSelectionMark();
	if (nItem < 2 || nItem > m_lstShared.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_SHARE_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	switch(res)
	{
	case ID_LSTSHARED_RUN:
		RunSelected();
		break;
	case ID_LSTSHARED_STOP:
		OnButtonStopSharing();
	case ID_LSTSHARED_INFO:
		POSITION pos = m_lstShared.GetFirstSelectedItemPosition();	
	
		if(pos)
		{
			int nItem = m_lstShared.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstShared.GetItemCount())
			{
		
				CShareExDlg ExInfo;
			
				ExInfo.m_pDoc   = m_pDoc;
				ExInfo.m_FileID = m_lstShared.GetItemData(nItem);

				ExInfo.DoModal();
			}
		}

		break;
	}
		
	*pResult = 0;
}


void CViewShare::OnDblclkListShared(NMHDR* pNMHDR, LRESULT* pResult) 
{
	RunSelected();
	
	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////
// @mfunc   
// 
// @parm    
// @rdesc   
// 
// @comm    
// 
// @devnote 
// 
// 2001-06-09 - Scott Kirkwood <nl>
//   Added this to make CDlgResizer happy about the original size.
// 
int CViewShare::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_DlgResizer.InitResizer(this);
	
	return 0;
}

void CViewShare::OnCheckStophashing() 
{
	if(m_chkStopHashing.GetCheck())
	{
		AfxMessageBox("Hashing is only done once and is saved between runs.\nThis option is here to give you the onvenience of choosing\n when to hash your own files.");
		m_autShare->StopHashing();
	}
	else
	{
		m_autShare->StartHashing();
	}
	
}

void CViewShare::OnTimer(UINT nIDEvent) 
{
	if(m_autShare->IsEverythingHashed())
		m_chkStopHashing.EnableWindow(false);
	else
		m_chkStopHashing.EnableWindow(true);
	
	CFormView::OnTimer(nIDEvent);
}

void CViewShare::OnButtonRunSelected() 
{
	RunSelected();
}

//Makslane
CString CViewShare::GetFirstSelectedItemPath()
{
	CString sPath;

	POSITION pos = m_lstShared.GetFirstSelectedItemPosition();	
	if(pos)
	{
		int nItem = m_lstShared.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstShared.GetItemCount())
			sPath = m_autShare->GetFileDir(m_lstShared.GetItemData(nItem));
	}

	return sPath;
}

void CViewShare::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CViewShare::OnItemchangedListShared(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	
	*pResult = 0;
}


BOOL CViewShare::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL nRet = CFormView::PreCreateWindow(cs);

	cs.lpszClass = AfxRegisterWndClass(CS_SAVEBITS, 0, 0, 0);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return nRet;
}

void CViewShare::OnNMReleasedcaptureSliderCpu(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_autShare->SetHashSpeed( m_sldrCpu.GetPos() );

	m_stcCpu.SetWindowText( DWrdtoStr(m_sldrCpu.GetPos()) + "% cpu" );

	*pResult = 0;
}

void CViewShare::OnNMCustomdrawSliderCpu(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_stcCpu.SetWindowText(DWrdtoStr(m_sldrCpu.GetPos()) + "% cpu");

	*pResult = 0;
}
