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
#include "PrefBandwidth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefBandwidth, CPropertyPage)

CPrefBandwidth::CPrefBandwidth() : CPropertyPage(CPrefBandwidth::IDD)
{
	//{{AFX_DATA_INIT(CPrefBandwidth)
	//}}AFX_DATA_INIT

	m_pDoc     = NULL;
	m_autPrefs = NULL;
}

CPrefBandwidth::~CPrefBandwidth()
{
}

void CPrefBandwidth::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefBandwidth)
	DDX_Control(pDX, IDC_CHECK_LIMIT_DOWN, m_chkLimitDown);
	DDX_Control(pDX, IDC_CHECK_LIMIT_UP, m_chkLimitUp);
	DDX_Control(pDX, IDC_EDIT_LIMIT_UP, m_ebLimitUp);
	DDX_Control(pDX, IDC_EDIT_LIMIT_DOWN, m_ebLimitDown);
	DDX_Control(pDX, IDC_EDIT_DROP_UL, m_ebDropUL);
	DDX_Control(pDX, IDC_EDIT_DROP_DL, m_ebDropDL);
	DDX_Control(pDX, IDC_CHECK_DROP_UL, m_chkDropUL);
	DDX_Control(pDX, IDC_CHECK_DROP_DL, m_chkDropDL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefBandwidth, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefBandwidth)
	ON_BN_CLICKED(IDC_CHECK_DROP_DL, OnCheckDropDl)
	ON_EN_CHANGE(IDC_EDIT_DROP_DL, OnChangeEditDropDl)
	ON_BN_CLICKED(IDC_CHECK_DROP_UL, OnCheckDropUl)
	ON_EN_CHANGE(IDC_EDIT_DROP_UL, OnChangeEditDropUl)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_UP, OnCheckLimitUp)
	ON_EN_CHANGE(IDC_EDIT_LIMIT_UP, OnChangeEditLimitUp)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_DOWN, OnCheckLimitDown)
	ON_EN_CHANGE(IDC_EDIT_LIMIT_DOWN, OnChangeEditLimitDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefBandwidth::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pDoc       = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;


	// Download Bandwidth Limit
	if(m_autPrefs->GetBandwidthDown())
	{
		m_chkLimitDown.SetCheck(1);
		m_ebLimitDown.SetWindowText( InsertDecimal( (double) m_autPrefs->GetBandwidthDown()));
	}
	else
		m_ebLimitDown.EnableWindow(false);

	// Upload Bandwidth Limit
	if(m_autPrefs->GetBandwidthUp())
	{
		m_chkLimitUp.SetCheck(1);
		m_ebLimitUp.SetWindowText( InsertDecimal((double) m_autPrefs->GetBandwidthUp()));
	}
	else
		m_ebLimitUp.EnableWindow(false);


	// Drop slow downloads
	if(m_autPrefs->GetMinDownSpeed())
	{
		m_chkDropDL.SetCheck(true);
		m_ebDropDL.SetWindowText( InsertDecimal( (double) m_autPrefs->GetMinDownSpeed()));
	}
	else
		m_ebDropDL.EnableWindow(false);


	// Drop slow uploads
	if(m_autPrefs->GetMinUpSpeed())
	{
		m_chkDropUL.SetCheck(true);
		m_ebDropUL.SetWindowText( InsertDecimal( (double) m_autPrefs->GetMinUpSpeed()) );
	}
	else
		m_ebDropUL.EnableWindow(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefBandwidth::OnCheckLimitUp() 
{
	if(m_chkLimitUp.GetCheck())
		m_ebLimitUp.EnableWindow(true);
	else
		m_ebLimitUp.EnableWindow(false);

	SetModified();
}

void CPrefBandwidth::OnChangeEditLimitUp() 
{
	SetModified();	
}

void CPrefBandwidth::OnCheckLimitDown() 
{
	if(m_chkLimitDown.GetCheck())
		m_ebLimitDown.EnableWindow(true);
	else
		m_ebLimitDown.EnableWindow(false);

	SetModified();
}

void CPrefBandwidth::OnChangeEditLimitDown() 
{
	SetModified();	
}

void CPrefBandwidth::OnCheckDropDl() 
{
	if(m_chkDropDL.GetCheck())
		m_ebDropDL.EnableWindow(true);
	else
		m_ebDropDL.EnableWindow(false);

	SetModified();
}

void CPrefBandwidth::OnChangeEditDropDl() 
{
	SetModified();
}

void CPrefBandwidth::OnCheckDropUl() 
{
	if(m_chkDropUL.GetCheck())
		m_ebDropUL.EnableWindow(true);
	else
		m_ebDropUL.EnableWindow(false);

	SetModified();
}

void CPrefBandwidth::OnChangeEditDropUl() 
{
	SetModified();
}


BOOL CPrefBandwidth::OnApply() 
{
	CString store;

	// Limit download bandwidth
	if(m_chkLimitDown.GetCheck())
	{
		m_ebLimitDown.GetWindowText(store);
		
		m_autPrefs->SetBandwidthDown( atof(store));
	}
	else
		m_autPrefs->SetBandwidthDown(0);	

	// Limit upload bandwidth
	if(m_chkLimitUp.GetCheck())
	{
		m_ebLimitUp.GetWindowText(store);
		
		m_autPrefs->SetBandwidthUp( atof(store));
	}
	else
		m_autPrefs->SetBandwidthUp(0);	


	// Min download speed
	if(m_chkDropDL.GetCheck())
	{
		m_ebDropDL.GetWindowText(store);
		
		m_autPrefs->SetMinDownSpeed(atof(store));
	}
	else
		m_autPrefs->SetMinDownSpeed(0);	

		// Min upload speed
	if(m_chkDropUL.GetCheck())
	{
		m_ebDropUL.GetWindowText(store);
		
		m_autPrefs->SetMinUpSpeed(atof(store));
	}
	else
		m_autPrefs->SetMinUpSpeed(0);

	return CPropertyPage::OnApply();
}



