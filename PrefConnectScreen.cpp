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

#include "AutPrefs.h"

#include "PrefConnectScreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefConnectScreen, CPropertyPage)

CPrefConnectScreen::CPrefConnectScreen() : CPropertyPage(CPrefConnectScreen::IDD)
{
	//{{AFX_DATA_INIT(CPrefConnectScreen)
	//}}AFX_DATA_INIT

	m_pDoc		= NULL;
	m_autPrefs    = NULL;
}

CPrefConnectScreen::~CPrefConnectScreen()
{
}

void CPrefConnectScreen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefConnectScreen)
	DDX_Control(pDX, IDC_EDIT_HOST, m_ebHost);
	DDX_Control(pDX, IDC_COMBO_RULE, m_cmbRule);
	DDX_Control(pDX, IDC_LIST_SCREEN, m_lstScreen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefConnectScreen, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefConnectScreen)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SCREEN, OnRclickListScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefConnectScreen::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;

	// Setup listbox
	int offSet = m_lstScreen.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstScreen.GetWindowRect(&rect);

	m_lstScreen.InsertColumn( 0, "Host", LVCFMT_LEFT,
		(rect.Width() - offSet) * 3/4, 0);
	m_lstScreen.InsertColumn( 1, "Rule", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/4, 1);

	m_lstScreen.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Get screen node vector
	VARIANT var = m_autPrefs->GetScreenedNodes();
	SAFEARRAY* psa = var.parray;

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> ScreenedNodes;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 ScreenedNodes.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);

	// Load list box
	for(int i = 0; i < ScreenedNodes.size(); i++)
	{
		CString Host = ScreenedNodes[i];
		int pos = m_lstScreen.GetItemCount();

		m_lstScreen.InsertItem(pos, Host.Mid(0, Host.Find(":")) );
		m_lstScreen.SetItemText(pos, 1, Host.Mid( Host.Find(":") + 1) );
	}
	
	// Load rule listbox
	m_cmbRule.InsertString(0, "Allow");
	m_cmbRule.InsertString(1, "Deny");
	m_cmbRule.SelectString(-1, "Deny");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefConnectScreen::OnButtonAdd() 
{
	CString store;
	m_ebHost.GetWindowText(store);

	// Check for 3 dots
	int pos = 0;
	for(int i = 0; i < 3; i++)
	{
		pos = store.Find('.', pos) + 1;

		if(pos == 0)
			return;
	}
	
	// Denied IPs at the front, allowed at the back
	m_cmbRule.GetWindowText(store);
	if(store == "Deny")
		pos = 0;
	else
		pos = m_lstScreen.GetItemCount();
	
	m_ebHost.GetWindowText(store);
	m_lstScreen.InsertItem(pos, store);

	m_cmbRule.GetWindowText(store);
	m_lstScreen.SetItemText(pos, 1, store);
	
	SetModified();
}

void CPrefConnectScreen::OnButtonRemove() 
{
	int      nItem;
	POSITION pos = m_lstScreen.GetFirstSelectedItemPosition();

	while(pos)
	{
		pos   = m_lstScreen.GetFirstSelectedItemPosition();
		nItem = m_lstScreen.GetNextSelectedItem(pos);

		m_lstScreen.DeleteItem(nItem);
	}

	SetModified();
}

BOOL CPrefConnectScreen::OnApply() 
{
	VARIANT vaNodes;
	VariantInit(&vaNodes);

	vaNodes.vt = VT_ARRAY | VT_BSTR;

	SAFEARRAYBOUND bounds = {m_lstScreen.GetItemCount(), 0};
	SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	for(int i = 0; i < m_lstScreen.GetItemCount(); i++)
	{
		CString buffer = m_lstScreen.GetItemText(i, 0);
		buffer = buffer + ":" + m_lstScreen.GetItemText(i, 1);

		strArray[i] = buffer.AllocSysString();
	}

	SafeArrayUnaccessData(psa);
	vaNodes.parray = psa;


	m_autPrefs->SetScreenedNodes(&vaNodes);
	
	VariantClear(&vaNodes);
		
	return CPropertyPage::OnApply();
}

void CPrefConnectScreen::OnRclickListScreen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHDR Control = (LPNMHDR) pNMHDR;		// stay compatable with IE 3.0!
	
	int nItem = m_lstScreen.GetSelectionMark();
	if (0 > nItem || nItem > m_lstScreen.GetItemCount() - 1)
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

	POSITION pos = m_lstScreen.GetFirstSelectedItemPosition();
	
	switch(res)
	{
	case ID_SCREENCONN_REMOVE:
		while(pos)
		{
			pos = m_lstScreen.GetFirstSelectedItemPosition();
			nItem = m_lstScreen.GetNextSelectedItem(pos);

			m_lstScreen.DeleteItem(nItem);
		}

		SetModified();
		break;
	}
		
	*pResult = 0;
}
