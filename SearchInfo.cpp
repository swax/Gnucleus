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
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "PrefDialog.h"
#include "PrefSearchBlock.h"

#include "ViewSearch.h"
#include "SearchResults.h"
#include "AutSearch.h"

#include "SearchInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSearchInfo::CSearchInfo(CWnd* pParent, CSearchResults* pView, UINT ResultID)
	: CDialog(CSearchInfo::IDD, pParent)
{
	m_pView = pView;
	m_pDoc  = pView->m_pDoc;
	m_autSearch = pView->m_autSearch;

	m_SearchID = pView->m_pView->m_SearchID;
	m_ResultID = ResultID;

	//{{AFX_DATA_INIT(CSearchInfo)
	//}}AFX_DATA_INIT
}


void CSearchInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchInfo)
	DDX_Control(pDX, IDC_STATIC_SHA1HASH, m_stcSha1Hash);
	DDX_Control(pDX, IDC_STATIC_BITPRINTHASH, m_stcBitprintHash);
	DDX_Control(pDX, IDC_STATIC_HOSTS, m_stcHosts);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_stcSpeed);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_stcSize);
	DDX_Control(pDX, IDC_STATIC_NAME, m_stcName);
	DDX_Control(pDX, IDC_LIST_RESULTS, m_lstResults);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchInfo, CDialog)
	//{{AFX_MSG_MAP(CSearchInfo)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
//	ON_WM_CLOSE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_RESULTS, OnColumnclickListResults)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESULTS, OnRclickListResults)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

int m_SearchExSortColumn = -1, m_SearchExSortOrder = 0;

BOOL CSearchInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_SEARCH);
	SetIcon(m_hIcon, FALSE);

	// Setup list box
	int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 3;
	CRect rect;
	m_lstResults.GetWindowRect(&rect);

	m_lstResults.InsertColumn( 0, "Host",			LVCFMT_RIGHT, (rect.Width() - offSet) * 3/10,  0);
	m_lstResults.InsertColumn( 1, "Speed",			LVCFMT_RIGHT, (rect.Width() - offSet) * 2/10,  1);
	m_lstResults.InsertColumn( 2, "Properties",		LVCFMT_LEFT,  (rect.Width() - offSet) * 2/10,  2);
	m_lstResults.InsertColumn( 3, "Client",			LVCFMT_LEFT,  (rect.Width() - offSet) * 2/10,  3);
	m_lstResults.InsertColumn( 4, "Hops",			LVCFMT_RIGHT, (rect.Width() - offSet) * 1/10,  4);
	m_lstResults.InsertColumn( 5, "Extended Info",  LVCFMT_LEFT,  (rect.Width() - offSet) * 10/10, 5);


	m_lstResults.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	m_lstResults.SetImageList(GetSharedImageList(), LVSIL_SMALL);

	m_DlgResizer.InitResizer(this);
	m_DlgResizer.SizeItem(IDC_LIST_RESULTS, CDlgResizer::DownAndRight);
	m_DlgResizer.ListCtrlItem(IDC_LIST_RESULTS);

	m_DlgResizer.SizeItem(IDC_STATIC_NAME, CDlgResizer::Right);
	m_DlgResizer.MoveItem(IDC_STATIC_SPEED, CDlgResizer::Down);
	m_DlgResizer.MoveItem(IDC_BUTTON_OK, CDlgResizer::Down);
	m_DlgResizer.Done();

	UpdateInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

struct SearchExOrder : public std::binary_function<ResultHost*, ResultHost*, bool>
{
	CSearchInfo* This;
	SearchExOrder(CSearchInfo* t)
	{
		This = t;
	}

	bool operator () (ResultHost* x, ResultHost* y)
	{
		switch (This->m_SearchExSortColumn)
		{
			case 0:
				return This->m_SearchExSortOrder ? ntohl(x->Host.S_addr) < ntohl(y->Host.S_addr) : ntohl(x->Host.S_addr) > ntohl(y->Host.S_addr);
			case 1:
				return This->m_SearchExSortOrder ? x->Speed < y->Speed : x->Speed > y->Speed;
			case 2:
				return This->m_SearchExSortOrder ? x->Busy < y->Busy : x->Busy > y->Busy;
			case 3:
				return stricmp(x->Vendor, y->Vendor) * (1-2 * This->m_SearchExSortOrder) > 0;
			case 4:
				return This->m_SearchExSortOrder ? x->Distance < y->Distance : x->Distance > y->Distance;
			case 5:
				return stricmp(x->strInfo, y->strInfo) * (1-2 * This->m_SearchExSortOrder) > 0;
		}

		return true;
	}
};

void CSearchInfo::UpdateInfo()
{	
	// Display data to dialog box
	m_stcName.SetWindowText("Name:  " + m_autSearch->GetResultName(m_SearchID, m_ResultID));
	m_stcSize.SetWindowText("Size:  " + CommaIze(DWrdtoStr( m_autSearch->GetResultSize(m_SearchID, m_ResultID) )) + " Bytes");

	m_stcSha1Hash.SetWindowText("SHA1 Hash:  " + m_autSearch->GetResultHash(m_SearchID, m_ResultID, HASH_SHA1));
	
	m_stcSpeed.SetWindowText("Average Speed: " + CommaIze(DWrdtoStr( m_autSearch->GetResultSpeed(m_SearchID, m_ResultID))) + " KB/s");
	
	int HostCount = m_autSearch->GetResultHostCount(m_SearchID, m_ResultID);
	CString Plural = (HostCount == 1) ? " Host:" : " Hosts";
	
	m_stcHosts.SetWindowText(CommaIze(DWrdtoStr(HostCount)) + Plural);

	// Save selected
	std::vector<ResultHost*> ListSelected;

	POSITION lstPos = m_lstResults.GetFirstSelectedItemPosition();
	while(lstPos)
	{
		int nItem = m_lstResults.GetNextSelectedItem(lstPos);

		//m_lstResults.SetItem(nItem, 0, LVIF_STATE, NULL, 0, 0, LVIS_SELECTED, 0);
		if(nItem < m_lstResults.GetItemCount())
			ListSelected.push_back( (ResultHost*) m_lstResults.GetItemData(nItem));
	}

	// Get new Host ID list
	VARIANT var = m_autSearch->GetHostIDs(m_SearchID, m_ResultID);
	SAFEARRAY* psa = var.parray;

	UINT* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
	{
		UINT HostID = nArray[i];

		std::map<UINT, ResultHost*>::iterator itHost = m_HostMap.find(HostID);


		// Add host if not in list yet
		if(itHost == m_HostMap.end())
		{
			ResultHost* pHost = new ResultHost;

			pHost->HostID = HostID;

			pHost->Host.S_addr = m_autSearch->GetHostIP(m_SearchID, m_ResultID, HostID);
			pHost->Port	    = m_autSearch->GetHostPort(m_SearchID, m_ResultID, HostID);
			pHost->Speed	= m_autSearch->GetHostSpeed(m_SearchID, m_ResultID, HostID);
			pHost->Distance = m_autSearch->GetHostDistance(m_SearchID, m_ResultID, HostID);

			pHost->Firewall	= m_autSearch->GetHostFirewall(m_SearchID, m_ResultID, HostID) != 0;
			pHost->Stable	= m_autSearch->GetHostStable(m_SearchID, m_ResultID, HostID) != 0;
			pHost->Busy		= m_autSearch->GetHostBusy(m_SearchID, m_ResultID, HostID) != 0;
			
			pHost->Vendor	= m_autSearch->GetHostVendor(m_SearchID, m_ResultID, HostID);
			pHost->Icon	    = m_pDoc->GetIconIndex(m_autSearch->GetResultName(m_SearchID, m_ResultID));

			// Get extended host info
			VARIANT var2 = m_autSearch->GetHostExtended(m_SearchID, m_ResultID, HostID);
			SAFEARRAY* psa2 = var2.parray;

			BSTR* strArray;
			SafeArrayAccessData(psa2, reinterpret_cast<void**> (&strArray));

			for(int i = 0; i < psa2->rgsabound->cElements; i++)
			{
				if(i > 0)
					pHost->strInfo += " and " + CString(strArray[i]);
				else
					pHost->strInfo = strArray[i];
			}

			SafeArrayUnaccessData(psa2);
			VariantClear(&var2);


			m_HostMap[HostID] = pHost;
			m_HostList.push_back(pHost);
		}
	}

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	SearchExOrder o(this);
	std::stable_sort(m_HostList.begin(), m_HostList.end(), o);

	int itemCount = m_lstResults.GetItemCount();

	// Insert items into list
	for(int i = 0; i < m_HostList.size(); i++)
	{
		ResultHost* p = m_HostList[i];

		CString Properties = "";

		if(p->Firewall)
			Properties += "Firewall, ";

		if(p->Busy)
			Properties += "Busy, ";
		
		if(p->Stable)
			Properties += "Stable, ";

		if(Properties.ReverseFind(',') != -1)
			Properties = Properties.Mid(0, Properties.GetLength() - 2);


		if (itemCount <= i)
			m_lstResults.InsertItem(itemCount++, IPtoStr(p->Host), p->Icon.Index);
		else
			m_lstResults.SetItem(i, 0, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE, IPtoStr(p->Host), p->Icon.Index, 0, LVIS_SELECTED, NULL);
	
		m_lstResults.SetItemText(i, 1, CommaIze( DWrdtoStr( p->Speed)) + " KB/s");
		m_lstResults.SetItemText(i, 2, Properties);
		m_lstResults.SetItemText(i, 3, GetVendor(p->Vendor));
		m_lstResults.SetItemText(i, 4, DWrdtoStr(p->Distance));
		m_lstResults.SetItemText(i, 5, p->strInfo);
		
		m_lstResults.SetItemData(i, (DWORD) p);

		// Re-select previously selected item
		for (int sel = 0; sel < ListSelected.size(); sel++)
			if(ListSelected[sel] == p)
				m_lstResults.SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
	}

	for (int j = i + 1; j < itemCount; j++)
		m_lstResults.DeleteItem(i + 1);
}

void CSearchInfo::OnButtonOk() 
{
	OnOK();	
}

void CSearchInfo::OnColumnclickListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int sortColumn = pNMListView->iSubItem;

    if (m_SearchExSortColumn == sortColumn)
    {
        m_SearchExSortOrder++;

        if (m_SearchExSortOrder > 1)
        	m_SearchExSortOrder = 0;
    }
    else
    {
        m_SearchExSortColumn = pNMListView->iSubItem;
        m_SearchExSortOrder = 0;
    }

    UpdateInfo();
	
	*pResult = 0;
}

void CSearchInfo::OnRclickListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstResults.GetSelectionMark();
	if (0 > nItem || nItem > m_lstResults.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_SEARCHEX_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstResults.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_LSTSEARCHEX_BLOCK:
		if(pos)
		{
			pos   = m_lstResults.GetFirstSelectedItemPosition();
			nItem = m_lstResults.GetNextSelectedItem(pos);

			ResultHost* p = (ResultHost*) m_lstResults.GetItemData(nItem);


			// Dispaly Preferences, sharing tab
			m_pDoc->m_nLastPref = PREF_SEARCHBLOCK;
			
			CPrefDialog PrefDlg("Preferences");
		
			PrefDlg.m_TabSearchBlock->m_SearchExInit = IPtoStr(p->Host);
			PrefDlg.DoModal();

			m_pView->OnChecksScreen();
		}

		break;
	}
		
	*pResult = 0;
}

void CSearchInfo::OnDestroy()
{
	CDialog::OnDestroy();

	for(int i = 0; i < m_HostList.size(); i++)
		delete m_HostList[i];
}
