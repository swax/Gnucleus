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
	license your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/


#include "stdafx.h"
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "ViewSearch.h"
#include "SearchResults.h"
#include "SearchAdvanced.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSearchAdvanced, CPropertyPage)

CSearchAdvanced::CSearchAdvanced() : CPropertyPage(CSearchAdvanced::IDD)
{
	//{{AFX_DATA_INIT(CSearchAdvanced)
	//}}AFX_DATA_INIT
}


CSearchAdvanced::CSearchAdvanced(CViewSearch* pView) : CPropertyPage(CSearchAdvanced::IDD)
{
	m_pView = pView;
	m_pDoc  = m_pView->m_pDoc;

	//{{AFX_DATA_INIT(CTransfersUp)
	//}}AFX_DATA_INIT
}

CSearchAdvanced::~CSearchAdvanced()
{
}

void CSearchAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchAdvanced)
	DDX_Control(pDX, IDC_EDIT_SPEED, m_ebSpeed);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_ebSize);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_cmbSpeed);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_cmbSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchAdvanced, CPropertyPage)
	//{{AFX_MSG_MAP(CSearchAdvanced)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG_SEARCH, OnButtonConfigSearch)
	ON_BN_CLICKED(IDC_BUTTON_SCREENHOSTS, OnButtonScreenhosts)
	ON_BN_CLICKED(IDC_BUTTON_SCREENWORDS, OnButtonScreenwords)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSearchAdvanced::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_cmbSize.InsertString(0, "Less Than");
	m_cmbSize.InsertString(0, "Exactly");
	m_cmbSize.InsertString(0, "More Than");
	m_cmbSize.InsertString(0, "");
	
	m_cmbSpeed.InsertString(0, "Less Than");
	m_cmbSpeed.InsertString(0, "Exactly");
	m_cmbSpeed.InsertString(0, "More Than");
	m_cmbSpeed.InsertString(0, "");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchAdvanced::OnButtonConfigSearch() 
{
	// Dispaly Preferences, sharing tab
	m_pDoc->m_nLastPref = PREF_SEARCH;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);
}

void CSearchAdvanced::OnButtonScreenhosts() 
{
	// Dispaly Preferences, sharing tab
	m_pDoc->m_nLastPref = PREF_SEARCHBLOCK;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);	
}

void CSearchAdvanced::OnButtonScreenwords() 
{
	// Dispaly Preferences, sharing tab
	m_pDoc->m_nLastPref = PREF_SEARCHSCREEN;
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_VIEW_PREFERENCES, NULL);
}

void CSearchAdvanced::OnButtonRefresh() 
{
	m_pView->m_tabResults->OnChecksScreen();
}

