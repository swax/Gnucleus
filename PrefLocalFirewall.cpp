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
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "AutPrefs.h"

#include "PrefLocalFirewall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefLocalFirewall, CPropertyPage)

CPrefLocalFirewall::CPrefLocalFirewall() : CPropertyPage(CPrefLocalFirewall::IDD)
{
	//{{AFX_DATA_INIT(CPrefLocalFirewall)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc	 = NULL;
	m_autPrefs = NULL;
}

CPrefLocalFirewall::~CPrefLocalFirewall()
{
}

void CPrefLocalFirewall::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefLocalFirewall)
	DDX_Control(pDX, IDC_CHECK_FIREWALL, m_chkFirewall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefLocalFirewall, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefLocalFirewall)
	ON_BN_CLICKED(IDC_CHECK_FIREWALL, OnCheckFirewall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefLocalFirewall::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc   = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;

	if(m_autPrefs->GetBehindFirewall())
		m_chkFirewall.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefLocalFirewall::OnCheckFirewall() 
{
	SetModified();
}

BOOL CPrefLocalFirewall::OnApply() 
{
	if(m_chkFirewall.GetCheck())
		m_autPrefs->SetBehindFirewall(true);
	else
		m_autPrefs->SetBehindFirewall(false);

	return CPropertyPage::OnApply();
}

