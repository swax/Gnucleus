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

#include "AutUpload.h"

#include "TransfersUpEx.h"
#include "ViewTransfers.h"
#include "TransfersUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTransfersUp, CPropertyPage)

CTransfersUp::CTransfersUp() : CPropertyPage(CTransfersUp::IDD)
{
}

CTransfersUp::CTransfersUp(CViewTransfers* pView) : CPropertyPage(CTransfersUp::IDD)
{
	m_pView     = pView;
	m_pDoc      = m_pView->m_pDoc;
	m_autUpload = m_pDoc->m_autUpload;

	//{{AFX_DATA_INIT(CTransfersUp)
	//}}AFX_DATA_INIT
}

CTransfersUp::~CTransfersUp()
{
}

void CTransfersUp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransfersUp)
	DDX_Control(pDX, IDC_LIST_UPLOADS, m_lstUploads);
	DDX_Control(pDX, IDC_CHECK_ERROR, m_chkError);
	DDX_Control(pDX, IDC_CHECK_COMPLETE, m_chkComplete);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_chkActive);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_CONFIGURE, m_btnConfigure);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_QUEUED, m_chkQueued);
}


BEGIN_MESSAGE_MAP(CTransfersUp, CPropertyPage)
	//{{AFX_MSG_MAP(CTransfersUp)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE, OnCheckActive)
	ON_BN_CLICKED(IDC_CHECK_COMPLETE, OnCheckComplete)
	ON_BN_CLICKED(IDC_CHECK_ERROR, OnCheckError)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CONFIGURE, OnButtonConfigure)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UPLOADS, OnRclickListUploads)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_UPLOADS, OnDblclkListUploads)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_UPLOADS, OnKeydownListUploads)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_UPLOADS, OnColumnclickListUploads)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UPLOADS, OnItemchangedListUploads)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_QUEUED, OnCheckQueued)
END_MESSAGE_MAP()

int m_UpSortColumn = -1, m_UpSortOrder = 0;

BOOL CTransfersUp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_chkActive.SetCheck(true);
	m_chkComplete.SetCheck(true);

	/// Active listbox
	int offSet = m_lstUploads.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstUploads.GetClientRect(&rect);

	int nWidth = rect.Width() - offSet;

	m_lstUploads.InsertColumn( 0, "File", LVCFMT_LEFT,
		nWidth * 12/40, 0);
	m_lstUploads.InsertColumn( 1, "Time", LVCFMT_RIGHT,
		nWidth * 4/40, 1);
	m_lstUploads.InsertColumn( 2, "Status", LVCFMT_LEFT,
		nWidth * 8/40, 2);
	m_lstUploads.InsertColumn( 3, "Completed", LVCFMT_RIGHT,
		nWidth * 8/40, 3);
	m_lstUploads.InsertColumn( 4, "Speed", LVCFMT_RIGHT,
		nWidth * 8/40, 4);

	m_lstUploads.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstUploads.SetImageList(GetSharedImageList(), LVSIL_SMALL);

	m_DlgResizer.InitResizer(this);
	m_DlgResizer.ListCtrlItem(IDC_LIST_UPLOADS);
	m_DlgResizer.SizeItem(IDC_LIST_UPLOADS, CDlgResizer::DownAndRight);
	m_DlgResizer.MoveItem(IDC_BUTTON_REMOVE, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_CLEAR, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_CONFIGURE, CDlgResizer::Down);

	m_DlgResizer.Done();

	ReloadLists();

	return TRUE;
}

struct UpOrder : public std::binary_function<int, int, bool>
{
	CTransfersUp* This;
	CAutUpload*   autUpload;

	UpOrder(CTransfersUp* t)
	{
		This = t;
		autUpload = t->m_autUpload;
	}

	double GetSpeed(int UploadID)
	{		
		if(autUpload->GetStatus(UploadID) == TRANSFER_SENDING)
		{
			int    BytesLeft   = autUpload->GetFileLength(UploadID) - autUpload->GetBytesCompleted(UploadID);
			double BytesPerSec = autUpload->GetBytesPerSec(UploadID);

			
			if (BytesPerSec == 0)
				return 0.0;
			
			return BytesLeft / BytesPerSec;
		}
		else
			return -1.0;
	}
	
	double GetStatus(int UploadID)
	{
		switch(autUpload->GetStatus(UploadID))
		{
			case TRANSFER_CONNECTING:
				return 2.0;
				break;

			case TRANSFER_PUSH:
				return 3.0;
				break;

			case TRANSFER_CONNECTED:
				return 4.0;
				break;

			case TRANSFER_QUEUED:
				return 4.5;
				break;

			case TRANSFER_SENDING:
				if(autUpload->GetFileLength(UploadID) == 0)
					return 5.0;
				else
					return 6.0;
				break;
	
			case TRANSFER_COMPLETED:
				return 7.0;
				break;

			case TRANSFER_CLOSED:
				return 1.0;
				break;
		}

		return 0.0;
	}

	bool operator () (int x, int y)
	{
		switch (This->m_UpSortColumn)
		{
			case 0:
				return stricmp(autUpload->GetName(x), autUpload->GetName(y)) * (1-2 * This->m_UpSortOrder) > 0;
			case 1:
				return (This->m_UpSortOrder != 0) ? (autUpload->GetChangeTime(x) < autUpload->GetChangeTime(y)) : (autUpload->GetChangeTime(x) > autUpload->GetChangeTime(y));
			case 2:
				return This->m_UpSortOrder != 0? (GetStatus(x) < GetStatus(y)) : (GetStatus(x) > GetStatus(y));
			case 3:
				switch (This->m_UpSortOrder)
				{
					case 0:  return autUpload->GetBytesCompleted(x) < autUpload->GetBytesCompleted(y);
					case 1:  return autUpload->GetBytesCompleted(x) > autUpload->GetBytesCompleted(y);
					case 2:  return autUpload->GetFileLength(x) < autUpload->GetFileLength(y);
					default: return autUpload->GetFileLength(x) > autUpload->GetFileLength(y);
				}
			case 4:
				return This->m_UpSortOrder != 0 ? (GetSpeed(x) < GetSpeed(y)) : (GetSpeed(x) > GetSpeed(y));
		}

		return true;
	}
};

void CTransfersUp::OnUpdate(int UploadID) 
{
	if(!m_lstUploads.IsWindowVisible())
		return;

 
	// Depending on view options, determine if this item should be visible
 	int shouldshow = false;

	switch(m_autUpload->GetStatus(UploadID))
	{
	case TRANSFER_QUEUED:
		shouldshow = m_chkQueued.GetCheck();
		break;

	case TRANSFER_CONNECTING:
	case TRANSFER_PUSH:
	case TRANSFER_CONNECTED:
	case TRANSFER_SENDING:
		shouldshow = m_chkActive.GetCheck();
		break;
		
	case TRANSFER_COMPLETED:
		shouldshow = m_chkComplete.GetCheck();
		break;

	case TRANSFER_CLOSED:
		shouldshow = m_chkError.GetCheck();
		break;
	}


	// Find transfer in list box
 	LVFINDINFO info;
 	info.flags  = LVFI_PARAM;
 	info.lParam = UploadID;
 
 	int row = m_lstUploads.FindItem(&info);
 
 	m_lstUploads.SetRedraw(false);
 
 	// This line should be shown based on user selected categories
 	if(shouldshow) 
	{
 		// Not in list - have to add
 		if (row == -1) 
		{
 			FullIcon FileIcon = m_pDoc->GetIconIndex(m_autUpload->GetName(UploadID));
 			UpOrder o(this);
 
 			// Find right row for insertion
 			row = 2;
 			while (row < m_lstUploads.GetItemCount()) 
			{		
 				int TmpID = m_lstUploads.GetItemData(row);
 
 				if(TmpID != 0)
 					if (o(UploadID, TmpID)) 
						break;

 				row++;
 			}
 
 			// Make space for the header lines, e.g. "* Files..." on the top row
 			while (m_lstUploads.GetItemCount() < 2) 
				m_lstUploads.InsertItem(0, "", -1);
 
 
 			// Add the new line
 			m_lstUploads.InsertItem(row, m_autUpload->GetName(UploadID), FileIcon.Index);
 			m_lstUploads.SetItemData(row, UploadID);
 
 			// Handle the header lines
 			int i = m_lstUploads.GetItemCount() - 2;
 			CString ItemTotal = CommaIze(DWrdtoStr(i));
 			ItemTotal += (i == 1) ? " File" : " Files";
 
 			m_lstUploads.SetItem(0, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, ItemTotal, -1, 0, LVIS_SELECTED, NULL);
 			m_lstUploads.SetItemText(0, 1, "");
 			m_lstUploads.SetItemText(0, 2, "");
 			m_lstUploads.SetItemText(0, 3, "");
 			m_lstUploads.SetItem(1, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, "", -1, 0, LVIS_SELECTED, NULL);
 			m_lstUploads.SetItemText(1, 1, "");
 			m_lstUploads.SetItemText(1, 2, "");
 			m_lstUploads.SetItemText(1, 3, "");
 		}
 		
 
 		// Set time column
		m_lstUploads.SetItemText(row, 1, COleDateTime( m_autUpload->GetChangeTime(UploadID) ).Format("%I:%M %p"));
 
 		// Set status column
 		m_lstUploads.SetItemText(row, 2, GetStatus(UploadID));
 		
		// Set Completed colum
		CString Completed  = CommaIze( DWrdtoStr(m_autUpload->GetBytesCompleted(UploadID) / 1024)) + " / ";
				Completed += CommaIze( DWrdtoStr(m_autUpload->GetFileLength(UploadID) / 1024)) + " KB";

		m_lstUploads.SetItemText(row, 3, Completed);
	

		// Set Speed column
		if(m_autUpload->GetStatus(UploadID) == TRANSFER_SENDING)	
		{
			int SecsETD = m_autUpload->GetSecETD(UploadID);

			CString EstTime = SecsETD ? GetEstTime(SecsETD) : "Undetermined";
			m_lstUploads.SetItemText(row, 4, EstTime + " at " + InsertDecimal((double) m_autUpload->GetBytesPerSec(UploadID) / (double) 1024) + " KB/s");
		}
		else
			m_lstUploads.SetItemText(row, 4, ""); 	
 	} 

	// Handle if not shouldshow and line exists
	else if(row != -1) 
	{
 		// Remove it
 		m_lstUploads.DeleteItem(row);
 
 		// Handle the header lines
 		int i = m_lstUploads.GetItemCount() - 2;
 		CString ItemTotal;
 
 		if(i > 0) 
		{
 			ItemTotal = CommaIze(DWrdtoStr(i));
 			ItemTotal += (i == 1) ? " File" : " Files";
 		} 
		else 
 			ItemTotal = "";
 
 		m_lstUploads.SetItem(0, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, ItemTotal, -1, 0, LVIS_SELECTED, NULL);
 	}
 
	m_lstUploads.SetRedraw(true);
}

void CTransfersUp::ReloadLists()
{
	m_ActiveList.clear();
	m_CompletedList.clear();
	m_ErroredList.clear();   
	m_QueuedList.clear();

	
	// Get new download ID list
	_variant_t var = m_autUpload->GetUploadIDs();
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
	{
		switch(m_autUpload->GetStatus(nArray[i]))
		{
		case TRANSFER_CONNECTING:
		case TRANSFER_PUSH:
		case TRANSFER_CONNECTED:
		case TRANSFER_SENDING:
			m_ActiveList.push_back(nArray[i]);
			break;

		case TRANSFER_QUEUED:
			m_QueuedList.push_back(nArray[i]);
			break;

		case TRANSFER_COMPLETED:
			m_CompletedList.push_back(nArray[i]);
			break;

		case TRANSFER_CLOSED:
			m_ErroredList.push_back(nArray[i]);
			break;
		}
	}

	SafeArrayUnaccessData(psa);

	UpdateView();
}

void CTransfersUp::UpdateView()
{
	std::vector<int> ListItems;
	std::vector<int> ListSelected;

	POSITION lstPos = m_lstUploads.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstUploads.GetNextSelectedItem(lstPos);

		if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
			ListSelected.push_back(m_lstUploads.GetItemData(nItem));
	}

	// Clear and rebuild lists
	std::vector<int>::iterator itItem;

	ListItems.clear();

	if(m_chkActive.GetCheck())
		for(itItem = m_ActiveList.begin(); itItem != m_ActiveList.end(); itItem++)
			ListItems.push_back(*itItem);

	if(m_chkComplete.GetCheck())
		for(itItem = m_CompletedList.begin(); itItem != m_CompletedList.end(); itItem++)
			ListItems.push_back(*itItem);

	if(m_chkQueued.GetCheck())
		for(itItem = m_QueuedList.begin(); itItem != m_QueuedList.end(); itItem++)
			ListItems.push_back(*itItem);

	if(m_chkError.GetCheck())
		for(itItem = m_ErroredList.begin(); itItem != m_ErroredList.end(); itItem++)
			ListItems.push_back(*itItem);


	if (!ListItems.size())
	{
		m_lstUploads.DeleteAllItems();
		return;
	}

	UpOrder o(this);
    std::stable_sort(ListItems.begin(), ListItems.end(), o);

	m_lstUploads.SetRedraw(false);

	int itemCount = m_lstUploads.GetItemCount();

	while(itemCount < 2)
		m_lstUploads.InsertItem(itemCount++, "", -1);


	for(int i = 0, pos = 0; i < ListItems.size(); i++)
	{
		pos = i + 2;
		int UploadID = ListItems[i];

		FullIcon FileIcon = m_pDoc->GetIconIndex(m_autUpload->GetName(UploadID));
	
		if (itemCount <= pos)
			m_lstUploads.InsertItem(itemCount++, m_autUpload->GetName(UploadID), FileIcon.Index);
		else
			m_lstUploads.SetItem(pos, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, m_autUpload->GetName(UploadID), FileIcon.Index, 0, LVIS_SELECTED, NULL);
	

		m_lstUploads.SetItemText(pos, 1, COleDateTime(m_autUpload->GetChangeTime(UploadID)).Format("%I:%M %p"));
		m_lstUploads.SetItemText(pos, 2, GetStatus(UploadID));
		m_lstUploads.SetItemData(pos, UploadID);

		// Set Completed column
		CString Completed  = CommaIze( DWrdtoStr(m_autUpload->GetBytesCompleted(UploadID) / 1024)) + " / ";
				Completed += CommaIze( DWrdtoStr(m_autUpload->GetFileLength(UploadID) / 1024)) + " KB";

		m_lstUploads.SetItemText(pos, 3, Completed);
	

		// Set Speed column
		if(m_autUpload->GetStatus(UploadID) == TRANSFER_SENDING)	
		{
			int SecsETD = m_autUpload->GetSecETD(UploadID);

			CString EstTime = SecsETD ? GetEstTime(SecsETD) : "Undetermined";
			m_lstUploads.SetItemText(pos, 4, EstTime + " at " + InsertDecimal((double) m_autUpload->GetBytesPerSec(UploadID) / (double) 1024) + " KB/s");
		}
		else
			m_lstUploads.SetItemText(pos, 4, "");
		

		// Reselect previously selected items
		for (int j = 0; j < ListSelected.size(); j++)
			if(ListSelected[j] == UploadID)
				m_lstUploads.SetItem(pos, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
	}

	for (i = pos + 1; i < itemCount; i++)
		m_lstUploads.DeleteItem(pos + 1);

	i = ListItems.size();
	CString ItemTotal = CommaIze(DWrdtoStr(i));
	ItemTotal += (i == 1) ? " File" : " Files";

	m_lstUploads.SetItem(0, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, ItemTotal, -1, 0, LVIS_SELECTED, NULL);
	m_lstUploads.SetItemText(0, 1, "");
	m_lstUploads.SetItemText(0, 2, "");
	m_lstUploads.SetItemText(0, 3, "");
	m_lstUploads.SetItem(1, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, "", -1, 0, LVIS_SELECTED, NULL);
	m_lstUploads.SetItemText(1, 1, "");
	m_lstUploads.SetItemText(1, 2, "");
	m_lstUploads.SetItemText(1, 3, "");

	m_lstUploads.SetRedraw(true);
}


CString CTransfersUp::GetStatus(int UploadID)
{
	CString Status = "Unknown";

	switch(m_autUpload->GetStatus(UploadID))
	{
	case TRANSFER_CONNECTING:
		Status = "Connecting...";
		break;

	case TRANSFER_PUSH:
		Status = "Attempting Push...";
		break;

	case TRANSFER_CONNECTED:
		Status = "Connected";
		break;

	case TRANSFER_QUEUED:
		Status = "Number " + DWrdtoStr(m_autUpload->GetQueuePos(UploadID)) + " in Queue";
		break;

	case TRANSFER_SENDING:
		if(m_autUpload->GetFileLength(UploadID))
			Status = "Sending, " + DWrdtoStr( (double) m_autUpload->GetBytesCompleted(UploadID) / (double) m_autUpload->GetFileLength(UploadID) * 100) + "% Complete";
		else
			Status = "Sending, 0% Complete";
		break;

	case TRANSFER_COMPLETED:
		Status = "Completed";
		break;

	case TRANSFER_CLOSED:
		Status = m_autUpload->GetErrorStr(UploadID);
		break;
	}

	return Status;
}

void CTransfersUp::OnButtonRemove() 
{
	m_lstUploads.SetRedraw(false);

	int      nItem;
	POSITION pos = m_lstUploads.GetFirstSelectedItemPosition();

	while(pos)
	{
		nItem = m_lstUploads.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
			RemoveItem(m_lstUploads.GetItemData(nItem));
	}

	ReloadLists();
	m_lstUploads.SetRedraw(true);
}

void CTransfersUp::OnButtonClear() 
{
	m_lstUploads.SetRedraw(false);
	
	for(int i = 2; i < m_lstUploads.GetItemCount(); i++)
		RemoveItem(m_lstUploads.GetItemData(i));

	ReloadLists();
	m_lstUploads.SetRedraw(true);
}

void CTransfersUp::OnButtonConfigure() 
{
	// Dispaly Preferences, transfer tab
	m_pDoc->m_nLastPref = PREF_TRANSFER;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);	
}

void CTransfersUp::OnCheckActive() 
{
	ReloadLists();
}

void CTransfersUp::OnCheckComplete() 
{
	ReloadLists();	
}

void CTransfersUp::OnCheckError() 
{
	ReloadLists();	
}

void CTransfersUp::OnCheckQueued()
{
	ReloadLists();
}

void CTransfersUp::OnRclickListUploads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstUploads.GetSelectionMark();
	if (nItem < 2 || nItem > m_lstUploads.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_UPLOAD_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstUploads.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_LSTUPLOADS_RUN:
		while(pos)
		{
			nItem = m_lstUploads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
				RunFile(m_lstUploads.GetItemData(nItem));
		}
		break;
	case ID_LSTUPLOADS_INFO:
		if(pos)
			if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
				ExtendedInfo(m_lstUploads.GetItemData(nItem));
		
		break;
	case ID_LSTUPLOADS_REMOVE:
		m_lstUploads.SetRedraw(false);

		while(pos)
		{
			nItem = m_lstUploads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
				RemoveItem(m_lstUploads.GetItemData(nItem));
		}

		ReloadLists();
		m_lstUploads.SetRedraw(true);
		break;
	}
		
	*pResult = 0;
}

void CTransfersUp::OnDblclkListUploads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lstUploads.GetFirstSelectedItemPosition();
	
	while(pos)
	{
		int nItem = m_lstUploads.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
			RunFile(m_lstUploads.GetItemData(nItem));
	}

	*pResult = 0;
}

void CTransfersUp::RunFile(int UploadID)
{
	m_autUpload->RunFile(UploadID);
}

void CTransfersUp::ExtendedInfo(int UploadID)
{
	CTransfersUpEx InfoDlg;

	InfoDlg.m_FileName    = m_autUpload->GetName(UploadID);
	InfoDlg.m_FileIndex   = m_autUpload->GetIndex(UploadID);
	InfoDlg.m_Host.S_addr = m_autUpload->GetIP(UploadID);
	InfoDlg.m_Port		  = m_autUpload->GetPort(UploadID);
	InfoDlg.m_Handshake   = m_autUpload->GetHandshake(UploadID);
	InfoDlg.m_Attempts    = m_autUpload->GetAttempts(UploadID);

	InfoDlg.DoModal();
}

void CTransfersUp::RemoveItem(int UploadID)
{
	m_autUpload->Remove(UploadID);
}

CString CTransfersUp::GetEstTime(int Seconds)
{
	int Minutes = 0;
	int Hours   = 0;

	if(Seconds / 60)
	{
		Minutes = Seconds / 60;
		Seconds = Seconds % 60;

		if(Minutes / 60)
		{
			Hours   = Minutes / 60;
			Minutes = Minutes % 60;
		}
	}
	
	if(Hours)
		return DWrdtoStr(Hours) + " hr, " + DWrdtoStr(Minutes) + " min";
	if(Minutes)
		return DWrdtoStr(Minutes) + " min, " + DWrdtoStr(Seconds) + " sec";

	return DWrdtoStr(Seconds) + " sec";
}

void CTransfersUp::OnKeydownListUploads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
	// E key
	if(pLVKeyDow->wVKey == 69)
	{
		int nItem = m_lstUploads.GetSelectionMark();
		if (nItem < 2 || nItem > m_lstUploads.GetItemCount() - 1)
			return;

		POSITION pos = m_lstUploads.GetFirstSelectedItemPosition();

		if(pos)
			ExtendedInfo(m_lstUploads.GetItemData(nItem));
	}
	

	*pResult = 0;
}

void CTransfersUp::OnColumnclickListUploads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
        
    int sortColumn = pNMListView->iSubItem;

    if (m_UpSortColumn == sortColumn)
    {
        m_UpSortOrder++;

        if (m_UpSortOrder > 1 || (m_UpSortOrder > 1 && m_UpSortColumn != 3))
        	m_UpSortOrder = 0;
    }
    else
    {
        m_UpSortColumn = pNMListView->iSubItem;
        m_UpSortOrder = 0;
    }

    ReloadLists();

	*pResult = 0;
}

BOOL CTransfersUp::OnSetActive() 
{
	ReloadLists();

	return CPropertyPage::OnSetActive();
}


void CTransfersUp::OnItemchangedListUploads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
}

//CString CTransfersUp::GetFirstSelectedItem()
//{
//	POSITION pos = m_lstUploads.GetFirstSelectedItemPosition();
//
//	if(pos)
//	{
//		int nItem = m_lstUploads.GetNextSelectedItem(pos);
//
//		if(nItem > 1 && nItem < m_lstUploads.GetItemCount())
//		{
//			CGnuUploadShell *pUploadShell = (CGnuUploadShell *)m_lstUploads.GetItemData(nItem);
//			return pUploadShell->m_FileName;
//		}
//	}
//
//	return "";
//}


