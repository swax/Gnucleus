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

#include "AutDownload.h"
#include "AutPrefs.h"

#include "ViewSearch.h"
#include "FrameSearch.h"
#include "SearchAdvanced.h"
#include "SearchResultsCtrl.h"

#include "TransfersUpEx.h"
#include "ViewTransfers.h"
#include "TransfersDownEx.h"
#include "TransfersDownBar.h"
#include "TransfersDown.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


enum COLUMNS
{
	COL_NAME = 0,
	COL_STATUS,
	COL_SIZE,
	COL_COMPLETED,
	COL_PERCENT,
	COL_SPEED,
	COL_ETA,
	COL_COUNT_COLUMNS	// keep this last
};


IMPLEMENT_DYNCREATE(CTransfersDown, CPropertyPage)

CTransfersDown::CTransfersDown() : CPropertyPage(CTransfersDown::IDD)
{
}

CTransfersDown::CTransfersDown(CViewTransfers* pView) : CPropertyPage(CTransfersDown::IDD)
{
	m_pView		  = pView;
	m_pDoc		  = m_pView->m_pDoc;
	m_autDownload = m_pDoc->m_autDownload;
	m_autPrefs	  = m_pDoc->m_autPrefs;

	m_ExtendedDlg = NULL;


	//{{AFX_DATA_INIT(CTransfersDown)
	//}}AFX_DATA_INIT
}

CTransfersDown::~CTransfersDown()
{
}

void CTransfersDown::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransfersDown)
	DDX_Control(pDX, IDC_LIST_DOWNLOADS, m_lstDownloads);
	DDX_Control(pDX, IDC_CHECK_PENDING, m_chkPending);
	DDX_Control(pDX, IDC_CHECK_ERROR, m_chkError);
	DDX_Control(pDX, IDC_CHECK_COMPLETE, m_chkComplete);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_chkActive);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_CONFIGURE, m_btnConfigure);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransfersDown, CPropertyPage)
	//{{AFX_MSG_MAP(CTransfersDown)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE, OnCheckActive)
	ON_BN_CLICKED(IDC_CHECK_PENDING, OnCheckPending)
	ON_BN_CLICKED(IDC_CHECK_COMPLETE, OnCheckComplete)
	ON_BN_CLICKED(IDC_CHECK_ERROR, OnCheckError)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CONFIGURE, OnButtonConfigure)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DOWNLOADS, OnRclickListDownloads)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOWNLOADS, OnDblclkListDownloads)
//	ON_WM_TIMER()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_DOWNLOADS, OnKeydownListDownloads)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DOWNLOADS, OnColumnClick)
	ON_BN_CLICKED(IDC_BUTTON_OPENDOWNLOADS, OnButtonOpenDownloads)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOWNLOADS, OnItemchangedListDownloads)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int m_DownSortColumn = -1, m_DownSortOrder = 0;


BOOL CTransfersDown::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_chkActive.SetCheck(true);
	m_chkPending.SetCheck(true);
	m_chkComplete.SetCheck(true);


	/// Active listbox
	int offSet = m_lstDownloads.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstDownloads.GetClientRect(&rect);

	int nWidth = rect.Width() - offSet;

	for (int iCol = 0; iCol < COL_COUNT_COLUMNS; iCol ++)
	{
		switch (iCol)
		{
		case COL_NAME:
			m_lstDownloads.InsertColumn( COL_NAME, "File", LVCFMT_LEFT, nWidth * 15./40, COL_NAME);
			break;
		case COL_STATUS:
			m_lstDownloads.InsertColumn( COL_STATUS, "Status", LVCFMT_LEFT, nWidth * 11/40, COL_STATUS);
			break;
		case COL_COMPLETED:
			m_lstDownloads.InsertColumn( COL_COMPLETED, "Completed", LVCFMT_RIGHT, nWidth * 6./40, COL_COMPLETED);
			break;
		case COL_PERCENT:
			m_lstDownloads.InsertColumn( COL_PERCENT, "%", LVCFMT_RIGHT, nWidth * 4./40, COL_PERCENT);
			break;
		case COL_SPEED:
			m_lstDownloads.InsertColumn( COL_SPEED, "Speed", LVCFMT_RIGHT, nWidth * 6./40, COL_SPEED);
			break;
		case COL_SIZE:
			m_lstDownloads.InsertColumn (COL_SIZE, "Size", LVCFMT_RIGHT, nWidth * 6./40, COL_SIZE);
			break;
		case COL_ETA:
			m_lstDownloads.InsertColumn (COL_ETA, "ETA", LVCFMT_RIGHT, nWidth * 6./40, COL_ETA);
			break;
		}
	}

	m_lstDownloads.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	m_lstDownloads.SetImageList(GetSharedImageList(), LVSIL_SMALL);
	
	m_DlgResizer.InitResizer(this);
	m_DlgResizer.SizeItem(IDC_LIST_DOWNLOADS, CDlgResizer::DownAndRight);
	m_DlgResizer.ListCtrlItem(IDC_LIST_DOWNLOADS);
	
	m_DlgResizer.MoveItem(IDC_BUTTON_REMOVE, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_CLEAR, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_CONFIGURE, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_OPENDOWNLOADS, CDlgResizer::DownAndRight);
	m_DlgResizer.Done();

	ReloadLists();


	return TRUE;
}

struct DownOrder : public std::binary_function<int, int, bool>
{
	CTransfersDown* This;
	CAutDownload*   autDownload;

	DownOrder(CTransfersDown* t)
	{
		This = t;
		autDownload = t->m_autDownload;
	}

	double GetSpeed(int DownloadID)
	{	
		if(TRANSFER_RECEIVING == autDownload->GetStatus(DownloadID))
			return autDownload->GetBytesPerSec(DownloadID);
		else
			return -1.0;
	}
	
	double GetStatus(int DownloadID)
	{
		switch(autDownload->GetStatus(DownloadID))
		{
		case TRANSFER_CLOSED:
			if(autDownload->GetReasonClosed(DownloadID) == "")
				return 0.0;		 // Closed
			else
				return 1.0 + (double)* autDownload->GetReasonClosed(DownloadID) / 256.0; // dead
		
		case TRANSFER_NOSOURCES:
			return 2.0;

		case TRANSFER_PENDING:
			return 3.0;		// Pending

		case TRANSFER_COOLDOWN:
			return 4.0 + (double) autDownload->GetCoolingCount(DownloadID) / (double) 100; //Waiting to Retry in 

		case TRANSFER_CONNECTING:
			return 5.0;

		case TRANSFER_QUEUED:
			return 5.5;
		
		case TRANSFER_RECEIVING: // Receiving
			if(autDownload->GetFileLength(DownloadID))
				return 6.0 + (double) autDownload->GetBytesCompleted(DownloadID) / (double) autDownload->GetFileLength(DownloadID);
			else
				return 6.0;
		
		case TRANSFER_COMPLETED:
			return 7.0;	
		}

		return 0.0;
	}

	bool operator () (int x, int y)
	{
		bool fResult = false;
		bool fReverse = This->m_DownSortOrder & 1;
		switch (This->m_DownSortColumn)
		{
			case COL_NAME:
				fResult = stricmp(CString(autDownload->GetName(x)), CString(autDownload->GetName(y))) < 0;
				break;
			case COL_STATUS:
				fResult = GetStatus(x) > GetStatus(y);
				break;
			case COL_ETA:
			{
				DWORD statusX = autDownload->GetStatus(x);
				DWORD statusY = autDownload->GetStatus(y);
				bool fReceivingX = (TRANSFER_RECEIVING == statusX);
				bool fReceivingY = (TRANSFER_RECEIVING == statusY);
				bool fCompletedX = (TRANSFER_COMPLETED == statusX);
				bool fCompletedY = (TRANSFER_COMPLETED == statusY);
				if (fReceivingX || fReceivingY)
				{
					if (fReceivingX && fReceivingY)
					{
						long lEtaX = autDownload->GetSecETA(x);
						long lEtaY = autDownload->GetSecETA(y);
						if (lEtaX != lEtaY)
						{
							fResult = lEtaX < lEtaY;
							break;
						}
					}
					else if (fCompletedX || fCompletedY)
					{
						if (fCompletedX != fCompletedY)
						{
							fResult = fCompletedX;
							break;
						}
					}
					else
					{
						fResult = fReceivingX;
						break;
					}
				}
			}
			// FALL THROUGH!!!
			case COL_SPEED:
			{
				float fSpeedX = GetSpeed (x);
				float fSpeedY = GetSpeed (y);
				if (fSpeedX != fSpeedY)
				{
					fResult = fSpeedX > fSpeedY;
					break;
				}
			}
			// FALL THROUGH!!!
			case COL_PERCENT:
			{
				float dPercentCompleteX = This->GetPercentComplete (x);
				float dPercentCompleteY = This->GetPercentComplete (y);

				if (dPercentCompleteX != dPercentCompleteY)
				{
					fResult = dPercentCompleteX > dPercentCompleteY;
					break;
				}
			}
			// FALL THROUGH!!!
			case COL_COMPLETED:
			{
				int cbX = autDownload->GetBytesCompleted(x);
				int cbY = autDownload->GetBytesCompleted(y);
				if (cbX != cbY)
				{
					fResult = cbX > cbY;
					break;
				}
			}
			// FALL THROUGH!!!
			case COL_SIZE:
				fResult = autDownload->GetFileLength(x) < autDownload->GetFileLength(y);
				break;
		}
		if (fReverse)
			fResult = !fResult;

		return fResult;
	}
};

void CTransfersDown::OnRefresh() 
{
	ReloadLists();
}

void CTransfersDown::OnUpdate(int DownloadID) 
{
	// Check if message if for extended info box
	if(m_ExtendedDlg)
 		if(m_ExtendedDlg->m_DownloadID == DownloadID)
 			m_ExtendedDlg->UpdateInfo();
	
	
	if(!m_lstDownloads.IsWindowVisible())
		return;

 
	// Depending on view options, determine if this item should be visible
 	int shouldshow = false;
 
 	switch(m_autDownload->GetStatus(DownloadID)) 
	{
 	case TRANSFER_CONNECTING:
 	case TRANSFER_RECEIVING:
 		shouldshow = m_chkActive.GetCheck();
 		break;
 
	case TRANSFER_NOSOURCES:
 	case TRANSFER_COOLDOWN:
 	case TRANSFER_PENDING:
	case TRANSFER_QUEUED:
 		shouldshow = m_chkPending.GetCheck();
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
 	info.lParam = (LPARAM) DownloadID;
 
 	int row = m_lstDownloads.FindItem(&info);
 
 	m_lstDownloads.SetRedraw(false);
 
 	// This line should be shown based on user selected categories
 	if(shouldshow) 
	{
 		// Not in list - have to add
 		if (row == -1) 
		{
 			FullIcon FileIcon = m_pDoc->GetIconIndex(m_autDownload->GetName(DownloadID));
 			DownOrder o(this);
 
 			// Find right row for insertion
 			row = 2;
 			while (row < m_lstDownloads.GetItemCount()) 
			{		
 				int tmpID = m_lstDownloads.GetItemData(row);
 
 				if(tmpID != NULL)
 					if (o(DownloadID, tmpID)) 
						break;

 				row++;
 			}
 
 			// Make space for the header lines, e.g. "* Files..." on the top row
 			while (m_lstDownloads.GetItemCount() < 2) 
				m_lstDownloads.InsertItem(0, "", -1);
 
 			// Add the new line
 			m_lstDownloads.InsertItem(row, m_autDownload->GetName(DownloadID), FileIcon.Index);
 			m_lstDownloads.SetItemData(row, DownloadID);
 
 			// Handle the header lines
 			int i = m_lstDownloads.GetItemCount()-2;
 			CString ItemTotal = CommaIze(DWrdtoStr(i));
 			ItemTotal += (i == 1) ? " File" : " Files";
 
 			m_lstDownloads.SetItem(0, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, ItemTotal, -1, 0, LVIS_SELECTED, NULL);
			for (int iCol = 1; iCol < COL_COUNT_COLUMNS; iCol++)
 				m_lstDownloads.SetItemText(0, iCol, "");
 			m_lstDownloads.SetItem(1, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, "", -1, 0, LVIS_SELECTED, NULL);
			for (iCol = 1; iCol < COL_COUNT_COLUMNS; iCol++)
 				m_lstDownloads.SetItemText(0, iCol, "");
 		}
 		
		UpdateColumns (row, DownloadID);
 	} 

	// Handle if not shouldshow and line exists
	else if(row != -1) 
	{
 		// Remove it
 		m_lstDownloads.DeleteItem(row);
 
 		// Handle the header lines
 		int i = m_lstDownloads.GetItemCount() - 2;
 		CString ItemTotal;
 
 		if(i > 0) 
		{
 			ItemTotal = CommaIze(DWrdtoStr(i));
 			ItemTotal += (i == 1) ? " File" : " Files";
 		} 
		else 
 			ItemTotal = "";
 
 		m_lstDownloads.SetItem(0, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, ItemTotal, -1, 0, LVIS_SELECTED, NULL);
 	}
 
	m_lstDownloads.SetRedraw(true);
}

void CTransfersDown::UpdateColumns (int row, int DownloadID)
{
 	// Set status column
 	m_lstDownloads.SetItemText(row, COL_STATUS, GetStatus(DownloadID));


	// Set Completed column
	int cbCompleted = m_autDownload->GetBytesCompleted(DownloadID);
	int cbTotal = m_autDownload->GetFileLength(DownloadID);
	CString Completed  = CommaIze( DWrdtoStr(cbCompleted / 1024)) + " KB";
	m_lstDownloads.SetItemText(row, COL_COMPLETED, Completed);

	CString strSize = CommaIze( DWrdtoStr( cbTotal / 1024)) + " KB";
	m_lstDownloads.SetItemText (row, COL_SIZE, strSize);

	float dPercentComplete = GetPercentComplete (DownloadID);
	if (dPercentComplete == 100.0)
	{
		m_lstDownloads.SetItemText(row, COL_PERCENT, "100%");
	}
	else
	{
		char rgch[16];
		::sprintf (rgch, "%.1f%%", dPercentComplete);
		m_lstDownloads.SetItemText(row, COL_PERCENT, rgch);
	}

	// Set speed column
	if(TRANSFER_RECEIVING == m_autDownload->GetStatus(DownloadID))
	{
		double BytesPerSec = m_autDownload->GetBytesPerSec(DownloadID);
		CString EstTime = BytesPerSec ? GetEstTime(m_autDownload->GetSecETA(DownloadID)) : "Undetermined";

		m_lstDownloads.SetItemText(row, COL_SPEED, InsertDecimal(BytesPerSec / (double) 1024) + " KB/s");
		m_lstDownloads.SetItemText(row, COL_ETA, EstTime);
	}
	else
	{
		m_lstDownloads.SetItemText(row, COL_SPEED, "");
		m_lstDownloads.SetItemText(row, COL_ETA, "");
	}

}

void CTransfersDown::ReloadLists()
{
	m_ActiveList.clear();
	m_PendingList.clear();
	m_CompletedList.clear();
	m_ErroredList.clear();   

	// Get new download ID list
	_variant_t var = m_autDownload->GetDownloadIDs();
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
	{
		switch(m_autDownload->GetStatus(nArray[i]))
		{
		case TRANSFER_NOSOURCES:
		case TRANSFER_PENDING:
		case TRANSFER_COOLDOWN:	
		case TRANSFER_QUEUED:
			m_PendingList.push_back(nArray[i]);
			break;

		case TRANSFER_CONNECTING:
		case TRANSFER_RECEIVING:
			m_ActiveList.push_back(nArray[i]);
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
                

void CTransfersDown::UpdateView()
{
	std::vector<int> ListSelected;

	POSITION lstPos = m_lstDownloads.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstDownloads.GetNextSelectedItem(lstPos);

		if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
			ListSelected.push_back( m_lstDownloads.GetItemData(nItem));
	}

	// Reload lists
	std::vector<int> ListItems;
	std::vector<int>::iterator itItem;

	if(m_chkActive.GetCheck())
		for(itItem = m_ActiveList.begin(); itItem != m_ActiveList.end(); itItem++)
			ListItems.push_back(*itItem);

	if(m_chkPending.GetCheck())
		for(itItem = m_PendingList.begin(); itItem != m_PendingList.end(); itItem++)
			ListItems.push_back(*itItem);

	if(m_chkComplete.GetCheck())
		for(itItem = m_CompletedList.begin(); itItem != m_CompletedList.end(); itItem++)
			ListItems.push_back(*itItem);

	if(m_chkError.GetCheck())
		for(itItem = m_ErroredList.begin(); itItem != m_ErroredList.end(); itItem++)
			ListItems.push_back(*itItem);


	if (!ListItems.size())
	{
		m_lstDownloads.DeleteAllItems();
		return;
	}


	DownOrder o(this);
    std::stable_sort(ListItems.begin(), ListItems.end(), o);

	int itemCount = m_lstDownloads.GetItemCount();

	m_lstDownloads.SetRedraw(false);
	
	while (itemCount < 2)
		m_lstDownloads.InsertItem(itemCount++, "", -1);


	// Insert items into list
	for(int i = 0, pos = 0; i < ListItems.size(); i++)
	{
		pos = i + 2;
		int DownloadID = ListItems[i];

		CString  FileName = m_autDownload->GetName(DownloadID);
		FullIcon FileIcon = m_pDoc->GetIconIndex(FileName);
	
		if (itemCount <= pos)
			m_lstDownloads.InsertItem(itemCount++, FileName, FileIcon.Index);
		else
			m_lstDownloads.SetItem(pos, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, FileName, FileIcon.Index, 0, LVIS_SELECTED, NULL);
	
		m_lstDownloads.SetItemData(pos, DownloadID);
		UpdateColumns (pos, DownloadID);

		// Reselect previously selected item
		for (int j = 0; j < ListSelected.size(); j++)
			if(ListSelected[j] == DownloadID)
				m_lstDownloads.SetItem(pos, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
	}

	for (i = pos + 1; i < itemCount; i++)
		m_lstDownloads.DeleteItem(pos + 1);

	i = ListItems.size();
	CString ItemTotal = CommaIze(DWrdtoStr(i));
	ItemTotal += (i == 1) ? " File" : " Files";

	m_lstDownloads.SetItem(0, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, ItemTotal, -1, 0, LVIS_SELECTED, NULL);
	for (int iCol = 1; iCol < COL_COUNT_COLUMNS; iCol++)
 		m_lstDownloads.SetItemText(0, iCol, "");
	m_lstDownloads.SetItem(1, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, "", -1, 0, LVIS_SELECTED, NULL);
	for (iCol = 1; iCol < COL_COUNT_COLUMNS; iCol++)
 		m_lstDownloads.SetItemText(0, iCol, "");

	m_lstDownloads.SetRedraw(true);
}

CString CTransfersDown::GetStatus(int DownloadID)
{
	CString Status  = "Unknown";
	DWORD QueueSize = 0;

	switch(m_autDownload->GetStatus(DownloadID))
	{
	case TRANSFER_NOSOURCES:
		Status = "Waiting, more hosts needed";
		break;

	case TRANSFER_PENDING:
		
        QueueSize = m_autDownload->GetSourceCount(DownloadID);
        Status = "Pending, " + DWrdtoStr(QueueSize) + " Host";
        if (QueueSize != 1)
            Status += "s";
		Status += " Found";
        
		break;

	case TRANSFER_CONNECTING:
		if(m_autDownload->IsSearching(DownloadID))
		{
            QueueSize = m_autDownload->GetSourceCount(DownloadID);
            Status = "Searching... " + DWrdtoStr(QueueSize) + " Host";
            if (QueueSize != 1)
                Status += "s";
			Status += " Found";
        }
		else if(m_autDownload->IsRetrying(DownloadID))
			Status = "Retrying " + GetHostPos(DownloadID);
		else
			Status = "Connecting to " + GetHostPos(DownloadID);
		break;

	case TRANSFER_COOLDOWN:
		Status = "Waiting to Retry in " + DWrdtoStr(m_autDownload->GetCoolingCount(DownloadID));
		break;
	
	case TRANSFER_QUEUED:
		Status = "Remotely Queued...";
		break;

	case TRANSFER_RECEIVING:
		if(m_autDownload->GetFileLength(DownloadID))
		{
			int Sources = m_autDownload->GetActiveSourceCount(DownloadID);

			Status = "Receiving, " + DWrdtoStr( (double) m_autDownload->GetBytesCompleted(DownloadID) / (double) m_autDownload->GetFileLength(DownloadID) * 100) + "%";

			if(Sources > 1)
				Status += " from " + DWrdtoStr(Sources) + " Hosts";
		}
		else
			Status = "Receiving, 0% Complete";
		break;

	case TRANSFER_COMPLETED:
		Status = "Completed";
		break;

	case TRANSFER_CLOSED:
		
		CString ReasonDead = m_autDownload->GetReasonClosed(DownloadID);

		if(ReasonDead == "")
			Status = "Closed";
		else
			Status = ReasonDead;

		break;
	}

	return Status;
}

CString CTransfersDown::GetHostPos(int DownloadID)
{
	CString HostPos = "Host ";
	HostPos += DWrdtoStr(m_autDownload->GetSourcePos(DownloadID));
	HostPos += " of " + DWrdtoStr(m_autDownload->GetSourceCount(DownloadID));

	return HostPos;
}

void CTransfersDown::OnCheckActive() 
{
	ReloadLists();
}

void CTransfersDown::OnCheckPending() 
{
	ReloadLists();	
}

void CTransfersDown::OnCheckComplete() 
{
	ReloadLists();
}

void CTransfersDown::OnCheckError() 
{
	ReloadLists();
}

void CTransfersDown::OnButtonRemove() 
{
	m_lstDownloads.SetRedraw(false);

	int nItem;
	POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();

	while(pos)
	{
		nItem = m_lstDownloads.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
			RemoveDownload(m_lstDownloads.GetItemData(nItem));
	}

	ReloadLists();
	m_lstDownloads.SetRedraw(true);
}

void CTransfersDown::OnButtonClear() 
{
	m_autDownload->RemoveCompleted();
	
	ReloadLists();
}

void CTransfersDown::OnButtonConfigure() 
{
	// Dispaly Preferences, transfer tab
	m_pDoc->m_nLastPref = PREF_TRANSFER;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);		
}

void CTransfersDown::OnRclickListDownloads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstDownloads.GetSelectionMark();
	if (nItem < 2 || nItem > m_lstDownloads.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_DOWNLOAD_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_LSTDOWNLOADS_RUN:
		while(pos)
		{
			nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				RunFile(m_lstDownloads.GetItemData(nItem));
		}
		break;
	case ID_LSTDOWNLOADS_RESEARCH:
		m_lstDownloads.SetRedraw(false);
		while (pos)
		{
			// get the selection again, it may have changed because of sorting.
			nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
			{
				// TODO: here we need to check for a specific hash to research for
				// if the file has a hash associated with it
				//ReSearch(m_lstDownloads.GetItemData(nItem));
				m_autDownload->ReSearch(m_lstDownloads.GetItemData(nItem));
			}
		}
		ReloadLists();
		m_lstDownloads.SetRedraw(true);

		break;
	case ID_LSTDOWNLOADS_INFO:
		if(pos)
		{
			// get the selection again, it may have changed because of sorting.
			nItem = m_lstDownloads.GetNextSelectedItem(pos);
			
			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				ExtendedInfo(m_lstDownloads.GetItemData(nItem));
		}
		
		break;
	case ID_LSTDOWNLOADS_REMOVE:
		m_lstDownloads.SetRedraw(false);

		while(pos)
		{
			int nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				RemoveDownload(m_lstDownloads.GetItemData(nItem));
		}

		ReloadLists();
		m_lstDownloads.SetRedraw(true);
		break;

	/*case ID_LSTDOWNLOADS_NEXT:
		m_lstDownloads.SetRedraw(false);

		while(pos)
		{
			int nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				if(((CGnuDownloadShell*) m_lstDownloads.GetItemData(nItem))->GetCurrent())
				{
					((CGnuDownloadShell*) m_lstDownloads.GetItemData(nItem))->GetCurrent()->SetError("Skipped");
					((CGnuDownloadShell*) m_lstDownloads.GetItemData(nItem))->GetCurrent()->Close();
				}
		}

		ReloadLists();
		m_lstDownloads.SetRedraw(true);
		break;
	*/

	case ID_LSTDOWNLOADS_START:
		m_lstDownloads.SetRedraw(false);

		while(pos)
		{
			int nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				m_autDownload->ForceStart(m_lstDownloads.GetItemData(nItem));
		}
		
		ReloadLists();
		m_lstDownloads.SetRedraw(true);
		break;

	case ID_LSTDOWNLOADS_STOP:

		m_lstDownloads.SetRedraw(false);

		while(pos)
		{
			int nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				m_autDownload->Stop(m_lstDownloads.GetItemData(nItem));
		}

		ReloadLists();
		m_lstDownloads.SetRedraw(true);
		break;
	}


	*pResult = 0;
}

void CTransfersDown::OnDblclkListDownloads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();
	
	while(pos)
	{
		int nItem = m_lstDownloads.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
			RunFile(m_lstDownloads.GetItemData(nItem));
	}

	*pResult = 0;
}

void CTransfersDown::RemoveDownload(int DownloadID)
{
	m_autDownload->Remove(DownloadID);	
}

void CTransfersDown::RunFile(int DownloadID)
{
	m_autDownload->RunFile(DownloadID);	
}

void CTransfersDown::ReSearch(int DownloadID)
{
	m_autDownload->ReSearch(DownloadID);	

	ReloadLists();
}

CString CTransfersDown::GetEstTime(int Seconds)
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

void CTransfersDown::ExtendedInfo(int DownloadID)
{
	if(!m_ExtendedDlg)
	{
		m_ExtendedDlg = new CTransfersDownEx(NULL, this);

		m_ExtendedDlg->m_DownloadID = DownloadID;
		m_ExtendedDlg->m_stcProgress.m_DownloadID = DownloadID;

		m_ExtendedDlg->DoModal();

		delete m_ExtendedDlg;
		m_ExtendedDlg = NULL;
	}
}

void CTransfersDown::OnKeydownListDownloads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
	// E key
	if(pLVKeyDow->wVKey == 69)
	{
		int nItem = m_lstDownloads.GetSelectionMark();
		if (nItem < 2 || nItem > m_lstDownloads.GetItemCount() - 1)
			return;

		POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();
			
		nItem = m_lstDownloads.GetNextSelectedItem(pos);

		ExtendedInfo(m_lstDownloads.GetItemData(nItem));

	}
	if(pLVKeyDow->wVKey == VK_DELETE)
	{
		int nItem;
		POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();

		while(pos)
		{
			nItem = m_lstDownloads.GetNextSelectedItem(pos);

			if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
				RemoveDownload(m_lstDownloads.GetItemData(nItem));
		}

		ReloadLists();
	}

	*pResult = 0;
}

void CTransfersDown::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    
    int sortColumn = pNMListView->iSubItem;
    if (m_DownSortColumn == sortColumn)
    {
        m_DownSortOrder++;
    }
    else
    {
        m_DownSortColumn = pNMListView->iSubItem;
        m_DownSortOrder = 0;
    }

    ReloadLists();

	*pResult = 0;
}


void CTransfersDown::OnButtonOpenDownloads() 
{
	::ShellExecute(NULL, "explore", m_autPrefs->GetDownloadPath(), NULL, NULL, SW_SHOWNORMAL);
}

BOOL CTransfersDown::OnSetActive() 
{
	ReloadLists();

	return CPropertyPage::OnSetActive();
}

void CTransfersDown::OnItemchangedListDownloads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
}

//Makslane
int CTransfersDown::GetFirstSelectedItem()
{
	int nItem;
	POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();

	if(pos)
	{
		nItem = m_lstDownloads.GetNextSelectedItem(pos);

		if(nItem > 1 && nItem < m_lstDownloads.GetItemCount())
			return m_lstDownloads.GetItemData(nItem);
	}

	return NULL;
}

float CTransfersDown::GetPercentComplete (int idDownload)
{
	int cbTotal = m_autDownload->GetFileLength(idDownload);
	if (cbTotal == 0)
		return 100.0;

	int cbCompleted = m_autDownload->GetBytesCompleted(idDownload);
	return 100.0 * cbCompleted / (float) cbTotal;
}
