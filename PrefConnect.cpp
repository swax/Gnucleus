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
#include "PrefsEx.h"

#include "AutPrefs.h"

#include "PrefConnect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefConnect, CPropertyPage)

CPrefConnect::CPrefConnect() : CPropertyPage(CPrefConnect::IDD)
{
	//{{AFX_DATA_INIT(CPrefConnect)
	//}}AFX_DATA_INIT

	m_pDoc		= NULL;
	m_autPrefs  = NULL;
	m_pPrefsEx  = NULL;
}

CPrefConnect::~CPrefConnect()
{
	
}

void CPrefConnect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefConnect)
	DDX_Control(pDX, IDC_EDIT_MIN_CONNECTS, m_ebMinConnects);
	DDX_Control(pDX, IDC_EDIT_MAX_CONNECTS, m_ebMaxConnects);
	DDX_Control(pDX, IDC_EDIT_LEAF_CONNECTS, m_ebLeafConnects);
	DDX_Control(pDX, IDC_CHECK_MIN_CONNECTS, m_chkMinConnects);
	DDX_Control(pDX, IDC_CHECK_MAX_CONNECTS, m_chkMaxConnects);
	DDX_Control(pDX, IDC_CHECK_AUTOCONNECT, m_chkAutoConnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefConnect, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefConnect)
	ON_BN_CLICKED(IDC_CHECK_AUTOCONNECT, OnCheckAutoconnect)
	ON_BN_CLICKED(IDC_CHECK_MIN_CONNECTS, OnCheckMinConnects)
	ON_EN_CHANGE(IDC_EDIT_MIN_CONNECTS, OnChangeEditMinConnects)
	ON_BN_CLICKED(IDC_CHECK_MAX_CONNECTS, OnCheckMaxConnects)
	ON_EN_CHANGE(IDC_EDIT_MAX_CONNECTS, OnChangeEditMaxConnects)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_LEAF_CONNECTS, OnEnChangeEditLeafConnects)
END_MESSAGE_MAP()


BOOL CPrefConnect::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;
	m_pPrefsEx = m_pDoc->m_pPrefsEx;


	// AutoConnect
	if(m_pPrefsEx->m_AutoConnect)
		m_chkAutoConnect.SetCheck(1);

	m_ebLeafConnects.SetWindowText(DWrdtoStr(m_autPrefs->GetLeafModeConnects()));


	if(m_autPrefs->GetMinConnects() != -1)
	{
		m_chkMinConnects.SetCheck(1);
		m_ebMinConnects.SetWindowText( DWrdtoStr(m_autPrefs->GetMinConnects()));
	}
	else
		m_ebMinConnects.EnableWindow(false);

	if(m_autPrefs->GetMaxConnects() != -1)
	{
		m_chkMaxConnects.SetCheck(1);
		m_ebMaxConnects.SetWindowText( DWrdtoStr(m_autPrefs->GetMaxConnects()));
	}
	else
		m_ebMaxConnects.EnableWindow(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefConnect::OnCheckAutoconnect() 
{
	SetModified();	
}

void CPrefConnect::OnCheckMinConnects() 
{
	if(m_chkMinConnects.GetCheck() == 1)
		m_ebMinConnects.EnableWindow();
	else
		m_ebMinConnects.EnableWindow(false);

	SetModified();	
}

void CPrefConnect::OnChangeEditMinConnects() 
{
	SetModified();
}

void CPrefConnect::OnCheckMaxConnects() 
{
	if(m_chkMaxConnects.GetCheck() == 1)
		m_ebMaxConnects.EnableWindow();
	else
		m_ebMaxConnects.EnableWindow(false);

	SetModified();
}

void CPrefConnect::OnChangeEditMaxConnects() 
{
	SetModified();
}

void CPrefConnect::OnEnChangeEditLeafConnects()
{
	SetModified();
}

BOOL CPrefConnect::OnApply() 
{
	CString store;

	// AutoConnect
	if(m_chkAutoConnect.GetCheck() == 1)
		m_pPrefsEx->m_AutoConnect = true;
	else
		m_pPrefsEx->m_AutoConnect = false;

	m_ebLeafConnects.GetWindowText(store);
	m_autPrefs->SetLeafModeConnects(atoi(store));


	// Node Management
	if(m_chkMinConnects.GetCheck() == 1)
	{
		m_ebMinConnects.GetWindowText(store);
		m_autPrefs->SetMinConnects(atoi(store));
	}
	else
		m_autPrefs->SetMinConnects(-1);

	if(m_chkMaxConnects.GetCheck() == 1)
	{
		m_ebMaxConnects.GetWindowText(store);
		m_autPrefs->SetMaxConnects(atoi(store));
	}
	else
		m_autPrefs->SetMaxConnects(-1);


	return CPropertyPage::OnApply();
}




