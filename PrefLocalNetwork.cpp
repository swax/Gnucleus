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

#include "AutPrefs.h"
#include "ChatPrefs.h"
#include "ChatControl.h"

#include "PrefLocalNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefLocalNetwork, CPropertyPage)

CPrefLocalNetwork::CPrefLocalNetwork() : CPropertyPage(CPrefLocalNetwork::IDD)
{
	//{{AFX_DATA_INIT(CPrefLocalNetwork)
	//}}AFX_DATA_INIT

	m_pDoc	     = NULL;
	m_autPrefs   = NULL;
	m_chatPrefs  = NULL;
}

CPrefLocalNetwork::~CPrefLocalNetwork()
{
}

void CPrefLocalNetwork::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefLocalNetwork)
	DDX_Control(pDX, IDC_EDIT_MAXLEAF, m_ebMaxLeaves);
	DDX_Control(pDX, IDC_STATIC_USAGE, m_stcUsage);
	DDX_Control(pDX, IDC_SLIDER_USAGE, m_sldrUsage);
	DDX_Control(pDX, IDC_CHECK_DEDICATED, m_chkDedicated);
	DDX_Control(pDX, IDC_CHECK_SUPERNODE, m_chkSuperNode);
	DDX_Control(pDX, IDC_EDIT_IRCSERVER, m_ebIRCServer);
	DDX_Control(pDX, IDC_CHECK_IRCSERVER, m_chkIRCServer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefLocalNetwork, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefLocalNetwork)
	ON_BN_CLICKED(IDC_RADIO_MODEL_INTERNET, OnRadioModelInternet)
	ON_BN_CLICKED(IDC_RADIO_MODEL_PRIVATE, OnRadioModelPrivate)
	ON_BN_CLICKED(IDC_CHECK_IRCSERVER, OnCheckIrcserver)
	ON_EN_CHANGE(IDC_EDIT_IRCSERVER, OnChangeEditIrcserver)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_USAGE, OnReleasedcaptureSliderUsage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_USAGE, OnCustomdrawSliderUsage)
	ON_BN_CLICKED(IDC_CHECK_SUPERNODE, OnCheckSupernode)
	ON_BN_CLICKED(IDC_CHECK_DEDICATED, OnCheckDedicated)
	ON_EN_CHANGE(IDC_EDIT_MAXLEAF, OnChangeEditMaxleaf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefLocalNetwork::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc       = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs   = m_pDoc->m_autPrefs;
	m_chatPrefs  = m_pDoc->m_pChat->m_pPrefs;
	
	//Network Model
	if(m_autPrefs->GetNetworkModel() == NETWORK_INTERNET)
	{
		((CButton*) GetDlgItem(IDC_RADIO_MODEL_INTERNET))->SetCheck(true);
		
		m_chkIRCServer.EnableWindow(false);
		m_ebIRCServer.EnableWindow(false);
	}

	//else if(m_pPrefs->m_NetworkModel == NETWORK_LAN)
	//	((CButton*) GetDlgItem(IDC_RADIO_MODEL_LAN))->SetCheck(true);

	else if(m_autPrefs->GetNetworkModel() == NETWORK_PRIVATE)
	{
		((CButton*) GetDlgItem(IDC_RADIO_MODEL_PRIVATE))->SetCheck(true);
	
		m_ebIRCServer.EnableWindow(false);


		if( m_chatPrefs->m_InternalIRC)
		{
			m_chkIRCServer.SetCheck(1);
			m_ebIRCServer.EnableWindow(true);
		}
	}
	else
		((CButton*) GetDlgItem(IDC_RADIO_MODEL_INTERNET))->SetCheck(true);


	m_ebIRCServer.SetWindowText(m_chatPrefs->m_InternalIRCAddr);


	// SuperNode Settings
	m_sldrUsage.SetRange(10, 100);
	m_sldrUsage.SetTicFreq(5);
	m_sldrUsage.SetPos(15);

	m_ebMaxLeaves.SetWindowText( DWrdtoStr(m_autPrefs->GetMaxLeaves()));

	if(m_autPrefs->GetSuperNodeAble())
		m_chkSuperNode.SetCheck(true);
	else
	{
		m_ebMaxLeaves.EnableWindow(false);
		m_chkDedicated.EnableWindow(false);
		m_stcUsage.EnableWindow(false);
		m_sldrUsage.EnableWindow(false);
	}

	if(m_autPrefs->GetDedicated())
		m_chkDedicated.SetCheck(true);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefLocalNetwork::OnRadioModelInternet() 
{
	((CButton*) GetDlgItem(IDC_RADIO_MODEL_PRIVATE))->SetCheck(false);
	
	m_chkIRCServer.EnableWindow(false);
	m_ebIRCServer.EnableWindow(false);

	SetModified();
}

void CPrefLocalNetwork::OnRadioModelPrivate() 
{
	((CButton*) GetDlgItem(IDC_RADIO_MODEL_INTERNET))->SetCheck(false);
	
	
	m_chkIRCServer.EnableWindow(true);
	m_ebIRCServer.EnableWindow(false);


	if(m_chkIRCServer.GetCheck())
		m_ebIRCServer.EnableWindow(true);


	SetModified();
}

void CPrefLocalNetwork::OnCheckIrcserver() 
{
	if(m_chkIRCServer.GetCheck())
		m_ebIRCServer.EnableWindow(true);
	else
		m_ebIRCServer.EnableWindow(false);

	SetModified();
}

void CPrefLocalNetwork::OnChangeEditIrcserver() 
{
	SetModified();
}

void CPrefLocalNetwork::OnCheckSupernode() 
{
	if(m_chkSuperNode.GetCheck())
	{
		m_ebMaxLeaves.EnableWindow(true);
		m_chkDedicated.EnableWindow(true);
		m_stcUsage.EnableWindow(true);
		m_sldrUsage.EnableWindow(true);
	}
	else
	{
		m_ebMaxLeaves.EnableWindow(false);
		m_chkDedicated.EnableWindow(false);
		m_stcUsage.EnableWindow(false);
		m_sldrUsage.EnableWindow(false);
	}

	SetModified();
}

void CPrefLocalNetwork::OnCheckDedicated() 
{
	SetModified();
}

void CPrefLocalNetwork::OnReleasedcaptureSliderUsage(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_stcUsage.SetWindowText("Max Processor Usage, " + DWrdtoStr(m_sldrUsage.GetPos()) + "%");
	
	SetModified();

	*pResult = 0;
}

void CPrefLocalNetwork::OnCustomdrawSliderUsage(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_stcUsage.SetWindowText("Max Processor Usage, " + DWrdtoStr(m_sldrUsage.GetPos()) + "%");
	
	SetModified();

	*pResult = 0;
}

void CPrefLocalNetwork::OnChangeEditMaxleaf() 
{
	SetModified();
}

BOOL CPrefLocalNetwork::OnApply() 
{
	// Network model
	if(((CButton*) GetDlgItem(IDC_RADIO_MODEL_INTERNET))->GetCheck())
	{
		if(m_autPrefs->GetNetworkModel() == NETWORK_PRIVATE)
			AfxMessageBox("Restart Gnucleus to use on the Internet");

		m_autPrefs->SetNetworkModel(NETWORK_INTERNET);
		
		m_autPrefs->SetLan(false);
		m_autPrefs->SetInternalUpdate(false);
		m_chatPrefs->m_InternalIRC	= false;
	}

	//else if(((CButton*) GetDlgItem(IDC_RADIO_MODEL_LAN))->GetCheck())
	//	m_pPrefs->m_NetworkModel = NETWORK_LAN;
	
	else if(((CButton*) GetDlgItem(IDC_RADIO_MODEL_PRIVATE))->GetCheck())
	{
		CString store;
		
		if(m_autPrefs->GetNetworkModel() == NETWORK_INTERNET)
			AfxMessageBox("Restart Gnucleus to Finish LAN Setup");


		m_autPrefs->SetNetworkModel(NETWORK_PRIVATE);
			
		
		// Internal IRC Server
		if(m_chkIRCServer.GetCheck())
		{
			m_chatPrefs->m_InternalIRC = true;

			m_ebIRCServer.GetWindowText(store);
			m_chatPrefs->m_InternalIRCAddr = store;
		}
		else
			m_chatPrefs->m_InternalIRC = false;
	}
	else
		m_autPrefs->SetNetworkModel(NETWORK_INTERNET);
	


	if(m_chkSuperNode.GetCheck())
		m_autPrefs->SetSuperNodeAble(true);
	else
		m_autPrefs->SetSuperNodeAble(false);


	CString strTemp;
	m_ebMaxLeaves.GetWindowText(strTemp);
	m_autPrefs->SetMaxLeaves(atoi(strTemp));



	return CPropertyPage::OnApply();
}








