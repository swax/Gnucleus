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

#include "FrameMain.h"
#include "MruComboBox.h"
#include "MruStorage.h"

#include "AutPrefs.h"
#include "PrefSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefSearch, CPropertyPage)

CPrefSearch::CPrefSearch() : CPropertyPage(CPrefSearch::IDD)
{
	m_pDoc	   = NULL;
	m_autPrefs = NULL;

	//{{AFX_DATA_INIT(CPrefSearch)
	//}}AFX_DATA_INIT
}

CPrefSearch::~CPrefSearch()
{
}

void CPrefSearch::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefSearch)
	DDX_Control(pDX, IDC_CHECK_DISPLAY_SCREENED, m_chkScreenNodes);
	DDX_Control(pDX, IDC_CHECK_DOUBLECHECK, m_chkDoubleCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefSearch, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefSearch)
	ON_BN_CLICKED(IDC_CHECK_DOUBLECHECK, OnCheckDoublecheck)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_SCREENED, OnCheckDisplayScreened)
	ON_BN_CLICKED(IDC_BUTTON_CLEARHISTORY, OnButtonClearhistory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefSearch::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pDoc   = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;

	// Double Check
	if(m_autPrefs->GetDoubleCheck())
		m_chkDoubleCheck.SetCheck(true);
	
	// Screen Nodes
	if(m_autPrefs->GetScreenNodes())
		m_chkScreenNodes.SetCheck(true);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefSearch::OnCheckDoublecheck() 
{
	SetModified();
}

void CPrefSearch::OnCheckDisplayScreened() 
{
	SetModified();	
}

BOOL CPrefSearch::OnApply() 
{
	CString Temp;

	// Double Check
	if(m_chkDoubleCheck.GetCheck())
		m_autPrefs->SetDoubleCheck(true);
	else
		m_autPrefs->SetDoubleCheck(false);
	
	// Screen Nodes
	if(m_chkScreenNodes.GetCheck())
		m_autPrefs->SetScreenNodes(true);
	else
		m_autPrefs->SetScreenNodes(false);


	return CPropertyPage::OnApply();
}



void CPrefSearch::OnButtonClearhistory() 
{
	AfxMessageBox("Search History Cleared");	

	
	((CFrameMain*) AfxGetApp()->m_pMainWnd)->m_SearchToolbar.m_ctlComboBox.ResetList();
}
