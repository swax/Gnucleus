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

#include "AutPrefs.h"

#include "PrefSearchBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefSearchBlock property page

IMPLEMENT_DYNCREATE(CPrefSearchBlock, CPropertyPage)

CPrefSearchBlock::CPrefSearchBlock() : CPropertyPage(CPrefSearchBlock::IDD)
{
	m_pDoc		= NULL;
	m_autPrefs  = NULL;

	//{{AFX_DATA_INIT(CPrefSearchBlock)
	//}}AFX_DATA_INIT
}

CPrefSearchBlock::~CPrefSearchBlock()
{
}

void CPrefSearchBlock::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefSearchBlock)
	DDX_Control(pDX, IDC_LIST_BLOCKED, m_lstBlocked);
	DDX_Control(pDX, IDC_EDIT_REASON, m_ebReason);
	DDX_Control(pDX, IDC_EDIT_HOST, m_ebHost);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefSearchBlock, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefSearchBlock)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BLOCKED, OnRclickListScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefSearchBlock message handlers


BOOL CPrefSearchBlock::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	// There may be a simpler way to get this pointer
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;


	// Setup listbox
	int offSet = m_lstBlocked.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstBlocked.GetWindowRect(&rect);

	m_lstBlocked.InsertColumn( 0, "Host",   LVCFMT_LEFT, (rect.Width() - offSet) * 1/4, 0);
	m_lstBlocked.InsertColumn( 1, "Reason", LVCFMT_LEFT, (rect.Width() - offSet) * 3/4, 1);

	m_lstBlocked.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Copy vector to host servers array
	_variant_t var = m_autPrefs->GetBlockList();
	SAFEARRAY* psa = var.parray;

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> BlockList;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 BlockList.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);


	for(int i = 0; i < BlockList.size(); i++)
	{
		int pos = BlockList[i].Find(":");

		if(pos < 0)
			continue;

		CString HostRange = BlockList[i].Left(pos);
		CString Reason    = BlockList[i].Mid(pos + 1);
		
		pos = Reason.Find(":");
		if(pos >= 0)
			Reason = Reason.Left(pos);

		int InsertPos = m_lstBlocked.GetItemCount();;

		m_lstBlocked.InsertItem(InsertPos, HostRange, -1);
		m_lstBlocked.SetItemText(InsertPos, 1, Reason);
	}

	m_ebHost.SetWindowText(m_SearchExInit);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefSearchBlock::OnButtonAdd() 
{
	CString store;

	// Get Reason
	m_ebReason.GetWindowText(store);

	if(store.IsEmpty())
	{
		AfxMessageBox("You must specify a reason for blocking");
		return;
	}

	CString Reason = store;

	// Get Host
	IP StartIP;
	IP EndIP;

	m_ebHost.GetWindowText(store);
	CString HostRange = store;

	HostRange.Remove(' ');

	int HyphenPos = HostRange.Find('-');

	if(HyphenPos != -1)
	{
		StartIP = StrtoIP( HostRange.Left(HyphenPos) );
		EndIP   = StrtoIP( HostRange.Mid(HyphenPos + 1) );
	}
	else
	{
		StartIP = StrtoIP(HostRange);
		EndIP   = StrtoIP(HostRange);
	}

	// Verify and add to list
	if(StrtoIP("255.255.255.255").S_addr == StartIP.S_addr || StrtoIP("255.255.255.255").S_addr == EndIP.S_addr)
		return;

	if(StartIP.S_addr > 0 && ntohl(EndIP.S_addr) >= ntohl(StartIP.S_addr))
	{
		int pos = m_lstBlocked.GetItemCount();

		CString Host = IPtoStr(StartIP);

		if(StartIP.S_addr != EndIP.S_addr)
			Host += " - " + IPtoStr(EndIP);

		m_lstBlocked.InsertItem(pos, Host, -1);
		m_lstBlocked.SetItemText(pos, 1,Reason);
	}

	SetModified();
}

void CPrefSearchBlock::OnButtonRemove() 
{
	int nItem = m_lstBlocked.GetSelectionMark();
	if (0 > nItem || nItem > m_lstBlocked.GetItemCount() - 1)
		return;

	POSITION pos = m_lstBlocked.GetFirstSelectedItemPosition();

	while(pos)
	{
		pos   = m_lstBlocked.GetFirstSelectedItemPosition();
		int nItem = m_lstBlocked.GetNextSelectedItem(pos);

		m_lstBlocked.DeleteItem(nItem);
	}

	SetModified();
}

void CPrefSearchBlock::OnRclickListScreen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstBlocked.GetSelectionMark();
	if (0 > nItem || nItem > m_lstBlocked.GetItemCount() - 1)
		return;

    CMenu menu;
    menu.LoadMenu(IDR_SCREENCONN_RCLICK);
    CMenu *pMenu = menu.GetSubMenu(0);
    ASSERT(pMenu != NULL);

    // Display and track the popup menu
    CPoint point;
    GetCursorPos(&point);

	int res = pMenu->TrackPopupMenu( (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
                 point.x, point.y, this);

	POSITION pos = m_lstBlocked.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_SCREENCONN_REMOVE:
		while(pos)
		{
			pos   = m_lstBlocked.GetFirstSelectedItemPosition();
			nItem = m_lstBlocked.GetNextSelectedItem(pos);

			m_lstBlocked.DeleteItem(nItem);
		}

		SetModified();
		break;
	}
		
	*pResult = 0;
}

BOOL CPrefSearchBlock::OnApply() 
{
	std::vector<CString> BlockList;	

	for(int i = 0; i < m_lstBlocked.GetItemCount(); i++)
	{
		CString HostRange = m_lstBlocked.GetItemText(i, 0);
		CString Reason    = m_lstBlocked.GetItemText(i, 1);
			
		BlockList.push_back(HostRange + ":" + Reason + ":");
	}

	

	SAFEARRAYBOUND bounds = {BlockList.size(), 0};
	SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	for(int i = 0; i < BlockList.size(); i++)
		strArray[i] = BlockList[i].AllocSysString();

	SafeArrayUnaccessData(psa);
	
	VARIANT vaBlocked;
	VariantInit(&vaBlocked);

	vaBlocked.vt = VT_ARRAY | VT_BSTR;
	vaBlocked.parray = psa;


	m_autPrefs->SetBlockList(&vaBlocked);


	return CPropertyPage::OnApply();
}
