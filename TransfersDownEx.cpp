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
#include "GnucleusDoc.h"

#include "TransfersDown.h"
#include "AutDownload.h"

#include "TransfersDownEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTransfersDownEx::CTransfersDownEx(CWnd* pParent /*=NULL*/, CTransfersDown* pView)
	: CDialog(CTransfersDownEx::IDD, pParent)
{
	m_pView  = pView;
	m_autDownload = pView->m_autDownload;

	m_DownloadID = 0;

	m_Expanded  = true;
	m_Receiving = false;

	m_HostNum = 0;

	//{{AFX_DATA_INIT(CTransferDownEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTransfersDownEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransfersDownEx)
	DDX_Control(pDX, IDC_STATIC_BITPRINTHASH, m_stcBitprintHash);
	DDX_Control(pDX, IDC_STATIC_SHA1HASH, m_stcSha1Hash);
	DDX_Control(pDX, IDC_LIST_CHUNKS, m_lstChunks);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_stcProgress);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_stcSize);
	DDX_Control(pDX, IDC_STATIC_HAND, m_stcHandshake);
	DDX_Control(pDX, IDC_EDIT_HANDSHAKE, m_ebHandshake);
	DDX_Control(pDX, IDC_BUTTON_EXPAND, m_btnExpand);
	DDX_Control(pDX, IDC_STATIC_NAME, m_stcName);
	DDX_Control(pDX, IDC_STATIC_HOSTS, m_stcHosts);
	DDX_Control(pDX, IDC_LIST_DOWNLOADS, m_lstDownloads);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransfersDownEx, CDialog)
	//{{AFX_MSG_MAP(CTransfersDownEx)
	ON_BN_CLICKED(IDC_BUTTON_EXPAND, OnButtonExpand)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOWNLOADS, OnItemchangedListDownloads)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DOWNLOADS, OnColumnclickListDownloads)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int m_DownExSortColumn = -1, m_DownExSortOrder = 0;

BOOL CTransfersDownEx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_stcProgress.m_autDownload = m_autDownload;
	m_stcProgress.m_pDoc = m_pView->m_pDoc;

	// Setup list box
	int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 3;
	CRect rect;
	m_lstDownloads.GetWindowRect(&rect);

	m_lstDownloads.InsertColumn( 0, "Host", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 5/20, 0);
	m_lstDownloads.InsertColumn( 1, "Speed", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 4/20, 1);
	m_lstDownloads.InsertColumn( 2, "Status", LVCFMT_LEFT,
		(rect.Width() - offSet) * 8/20, 2);
	m_lstDownloads.InsertColumn( 3, "Client", LVCFMT_LEFT,
		(rect.Width() - offSet) * 3/20, 3);

	m_lstDownloads.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_lstDownloads.SetImageList(GetSharedImageList(), LVSIL_SMALL);


	m_lstChunks.GetWindowRect(&rect);
	m_lstChunks.InsertColumn( 0, "Start", LVCFMT_RIGHT,		rect.Width() * 1/3, 0);
	m_lstChunks.InsertColumn( 1, "Completed", LVCFMT_RIGHT,		rect.Width() * 1/3, 1);
	m_lstChunks.InsertColumn( 2, "Size", LVCFMT_RIGHT, rect.Width() * 1/3, 2);
	
	m_lstChunks.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	m_stcName.SetWindowText("Name:  " + m_autDownload->GetName(m_DownloadID));
	uint64 size = m_pView->m_pDoc->m_RunningXP ? m_autDownload->GetFileLength2(m_DownloadID) : m_autDownload->GetFileLength(m_DownloadID);
	m_stcSize.SetWindowText("Size:  " + CommaIze(DWrdtoStr(size)) + " bytes");
	
	m_stcSha1Hash.SetWindowText("SHA1 Hash:  " + m_autDownload->GetHash(m_DownloadID, HASH_SHA1));
	
	Expand();
	UpdateInfo();

	m_stcProgress.InvalidateRect(NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

struct DownExOrder : public std::binary_function<int, int, bool>
{
	CTransfersDownEx* This;
	CAutDownload*     autDownload;
	int DownloadID;

	DownExOrder(CTransfersDownEx* t)
	{
		This = t;
		autDownload = t->m_autDownload;
		t->m_DownloadID;
	}

	bool operator () (int x, int y)
	{
		switch (This->m_DownExSortColumn)
		{
			case 0:
				return This->m_DownExSortOrder ? ntohl(autDownload->GetSourceIP(DownloadID, x)) < ntohl(autDownload->GetSourceIP(DownloadID, y)) : ntohl(autDownload->GetSourceIP(DownloadID, x)) > ntohl(autDownload->GetSourceIP(DownloadID, y));
			case 1:
				return This->m_DownExSortOrder ? autDownload->GetSourceSpeed(DownloadID, x) < autDownload->GetSourceSpeed(DownloadID, y) : autDownload->GetSourceSpeed(DownloadID, x) > autDownload->GetSourceSpeed(DownloadID, y);
			case 2:
				return stricmp(autDownload->GetSourceStatusStr(DownloadID, x), autDownload->GetSourceStatusStr(DownloadID, y)) * (1-2 * This->m_DownExSortOrder) > 0;
			case 3:
				return stricmp(autDownload->GetSourceVendor(DownloadID, x), autDownload->GetSourceVendor(DownloadID, y)) * (1-2 * This->m_DownExSortOrder) > 0;
		}

		return true;
	}
};

void CTransfersDownEx::UpdateInfo()
{
	m_stcBitprintHash.SetWindowText("TigerTree Hash:  " + m_autDownload->GetHash(m_DownloadID, HASH_TIGER));

	m_HostNum = m_autDownload->GetSourceCount(m_DownloadID);
	CString Plural = m_HostNum == 1 ? " Host:" : " Hosts";
	m_stcHosts.SetWindowText(CommaIze(DWrdtoStr(m_HostNum)) + Plural);

	// Save selected
	std::vector<int> ListSelected;

	POSITION lstPos = m_lstDownloads.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstDownloads.GetNextSelectedItem(lstPos);

		m_lstDownloads.SetItem(nItem, 0, LVIF_STATE, NULL, 0, 0, LVIS_SELECTED, 0);
		if(nItem < m_lstDownloads.GetItemCount())
			ListSelected.push_back(m_lstDownloads.GetItemData(nItem));
	}

	// Make list
	m_HandshakeList.clear();
	std::vector<int> ListItems;

	// Get new Source IDs
	VARIANT var = m_autDownload->GetSourceIDs(m_DownloadID);
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
		ListItems.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	DownExOrder o(this);
    std::stable_sort(ListItems.begin(), ListItems.end(), o);

	int itemCount = m_lstDownloads.GetItemCount();

	for(i = 0; i < ListItems.size(); i++)
	{
		int SourceID = ListItems[i];

		FullIcon FileIcon = m_pView->m_pDoc->GetIconIndex(m_autDownload->GetSourceName(m_DownloadID, SourceID));

		IP HostIP;
		HostIP.S_addr = m_autDownload->GetSourceIP(m_DownloadID, SourceID);
		int Port = m_autDownload->GetSourcePort(m_DownloadID, SourceID);

		if (itemCount <= i)
			m_lstDownloads.InsertItem(itemCount++, IPtoStr(HostIP) + ":" + DWrdtoStr(Port), FileIcon.Index);
		else
			m_lstDownloads.SetItem(i, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, IPtoStr(HostIP) + ":" + DWrdtoStr(Port), FileIcon.Index, 0, LVIS_SELECTED, NULL);
	

		m_lstDownloads.SetItemText(i, 1,  CommaIze( DWrdtoStr( m_autDownload->GetSourceSpeed(m_DownloadID, SourceID ))) + " KB/s");
		m_lstDownloads.SetItemText(i, 2, m_autDownload->GetSourceStatusStr(m_DownloadID, SourceID));
		m_lstDownloads.SetItemText(i, 3, m_autDownload->GetSourceVendor(m_DownloadID, SourceID));

		m_lstDownloads.SetItemData(i, SourceID);

		m_HandshakeList.push_back( m_autDownload->GetSourceHandshake(m_DownloadID, SourceID) );

		// Re-select previously selected item
		for (int sel = 0; sel < ListSelected.size(); sel++)
			if(ListSelected[sel] == SourceID)
				m_lstDownloads.SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
	}

	for (int j = i + 1; j < itemCount; j++)
		m_lstDownloads.DeleteItem(i + 1);


	// Chunks list
	std::vector<int> ChunkList;

	// Get new Source IDs
	VariantClear(&var);
	var = m_autDownload->GetChunkIDs(m_DownloadID);
	psa = var.parray;

	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(i = 0; i < psa->rgsabound->cElements; i++)
		ChunkList.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);

	//m_lstChunks.SetItemText(0, 1, CommaIze(DWrdtoStr(Download->m_BytesCompleted)));
	//m_lstChunks.SetItemText(0, 2, CommaIze(DWrdtoStr(Download->m_StopPos)));

	itemCount = m_lstChunks.GetItemCount();

	for(i = 0; i < ChunkList.size(); i++)
	{
		int ChunkID = ChunkList[i];

		uint64 chunkStart = m_pView->m_pDoc->m_RunningXP ? m_autDownload->GetChunkStart2(m_DownloadID, ChunkID) : m_autDownload->GetChunkStart(m_DownloadID, ChunkID);
	
		if (itemCount <= i)
			m_lstChunks.InsertItem(itemCount++, CommaIze(DWrdtoStr(chunkStart)));
		else
			m_lstChunks.SetItemText(i, 0, CommaIze(DWrdtoStr(chunkStart)));
	
		m_lstChunks.SetItemText(i, 1, CommaIze(DWrdtoStr(m_autDownload->GetChunkCompleted(m_DownloadID, ChunkID))));
		m_lstChunks.SetItemText(i, 2, CommaIze(DWrdtoStr(m_autDownload->GetChunkSize(m_DownloadID, ChunkID))));
	}

	for(int j = i + 1; j < itemCount; j++)
		m_lstChunks.DeleteItem(i + 1);

	
	// Set handshake box
	POSITION npos = m_lstDownloads.GetFirstSelectedItemPosition();
	int nItem	  = m_lstDownloads.GetNextSelectedItem(npos);

	if(0 <= nItem && nItem < m_lstDownloads.GetItemCount())
		m_ebHandshake.SetWindowText(m_HandshakeList[nItem]);


	// Display file completion status
	m_stcProgress.InvalidateRect(NULL);
}



BOOL CTransfersDownEx::DestroyWindow() 
{
	//m_pView->m_ExtendedDlg = NULL;

	return CDialog::DestroyWindow();
}

void CTransfersDownEx::OnButtonExpand() 
{
	Expand();	
}

void CTransfersDownEx::Expand() 
{
	CRect rControl;
	CRect rDialog;
	
	GetWindowRect(&rDialog);

	int height = 0;
	int width  = 0;

	// Collapse the box
	if(m_Expanded)
	{
		m_stcHandshake.GetWindowRect(&rControl);
		height = rControl.top - rDialog.top;

		m_lstChunks.GetWindowRect(&rControl);
		width  = rControl.left - rDialog.left;

		m_btnExpand.SetWindowText("More >>");
		m_Expanded = false;
	}
	
	// Expand the box
	else
	{
		m_ebHandshake.GetWindowRect(&rControl);
		height = rControl.bottom - rDialog.top + 12;

		m_lstChunks.GetWindowRect(&rControl);
		width  = rControl.right - rDialog.left + 12;

		m_btnExpand.SetWindowText("<< Less");
		m_Expanded = true;
	}

	SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void CTransfersDownEx::OnItemchangedListDownloads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	POSITION pos = m_lstDownloads.GetFirstSelectedItemPosition();
	int nItem	 = m_lstDownloads.GetNextSelectedItem(pos);

	if(0 <= nItem && nItem < m_lstDownloads.GetItemCount())
		m_ebHandshake.SetWindowText(m_HandshakeList[nItem]);
	
	*pResult = 0;
}


void CTransfersDownEx::OnColumnclickListDownloads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	int sortColumn = pNMListView->iSubItem;

    if (m_DownExSortColumn == sortColumn)
    {
        m_DownExSortOrder++;

        if (m_DownExSortOrder > 1)
        	m_DownExSortOrder = 0;
    }
    else
    {
        m_DownExSortColumn = pNMListView->iSubItem;
        m_DownExSortOrder = 0;
    }

    UpdateInfo();

	*pResult = 0;
}
