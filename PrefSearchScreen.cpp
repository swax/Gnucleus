/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2004 John Marshall

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
#include "PrefSearchScreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefSearchScreen, CPropertyPage)

CPrefSearchScreen::CPrefSearchScreen() : CPropertyPage(CPrefSearchScreen::IDD)
{
	m_pDoc	 = NULL;
	m_autPrefs = NULL;

	//{{AFX_DATA_INIT(CPrefSearchScreen)
	//}}AFX_DATA_INIT
}

CPrefSearchScreen::~CPrefSearchScreen()
{
}

void CPrefSearchScreen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefSearchScreen)
	DDX_Control(pDX, IDC_EDIT_FILE, m_ebWord);
	DDX_Control(pDX, IDC_LIST_SCREEN, m_lstScreen);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefSearchScreen, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefSearchScreen)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SCREEN, OnRclickListScreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefSearchScreen::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;

	// Setup listbox
	int offSet = m_lstScreen.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstScreen.GetWindowRect(&rect);
	m_lstScreen.InsertColumn( 0, "Word", LVCFMT_LEFT, (rect.Width() - offSet), 0);
	m_lstScreen.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Copy vector to word array
	VARIANT var = m_autPrefs->GetScreenedWords();
	SAFEARRAY* psa = var.parray;

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> BadWords;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 BadWords.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	// Load list box
	for (int i = 0; i < BadWords.size(); i++)
	{
		int pos = m_lstScreen.GetItemCount();
		m_lstScreen.InsertItem(pos, BadWords[i]);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPrefSearchScreen::OnButtonAdd() 
{
	CString store;
	m_ebWord.GetWindowText(store);

	if(store != "")
	{
		m_lstScreen.InsertItem(m_lstScreen.GetItemCount(), store);
		SetModified();
	}

}

void CPrefSearchScreen::OnButtonRemove() 
{
	int      nItem;
	POSITION pos = m_lstScreen.GetFirstSelectedItemPosition();

	while(pos)
	{
		pos = m_lstScreen.GetFirstSelectedItemPosition();
		nItem = m_lstScreen.GetNextSelectedItem(pos);

		m_lstScreen.DeleteItem(nItem);
	}

	SetModified();
}

BOOL CPrefSearchScreen::OnApply() 
{
	std::vector<CString> BadWords;
	for(int i = 0; i < m_lstScreen.GetItemCount(); i++)
		BadWords.push_back(m_lstScreen.GetItemText(i, 0));

	VARIANT vaWords;
	VariantInit(&vaWords);

	vaWords.vt = VT_ARRAY | VT_BSTR;

	SAFEARRAYBOUND bounds = {BadWords.size(), 0};
	SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	for(int i = 0; i < BadWords.size(); i++)
		strArray[i] = BadWords[i].AllocSysString();

	SafeArrayUnaccessData(psa);
	vaWords.parray = psa;


	m_autPrefs->SetScreenedWords(&vaWords);

	VariantClear(&vaWords);

	return CPropertyPage::OnApply();
}

void CPrefSearchScreen::OnRclickListScreen(NMHDR* pNMHDR, LRESULT* pResult) 
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
			pos   = m_lstScreen.GetFirstSelectedItemPosition();
			nItem = m_lstScreen.GetNextSelectedItem(pos);

			m_lstScreen.DeleteItem(nItem);
		}

		SetModified();
		break;
	}
		
	*pResult = 0;
}
