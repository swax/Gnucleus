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
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "AutPrefs.h"
#include "AutNetwork.h"
#include "AutCore.h"
#include "ChatPrefs.h"
#include "ChatControl.h"
#include "NetSetup.h"
#include "PrefsEx.h"

#include "PrefLocalNetwork.h"
#include ".\preflocalnetwork.h"

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
	m_autNetwork = NULL;

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
	DDX_Control(pDX, IDC_CHECK_SUPERNODE, m_chkSuperNode);
	DDX_Control(pDX, IDC_EDIT_IRCSERVER, m_ebIRCServer);
	DDX_Control(pDX, IDC_CHECK_IRCSERVER, m_chkIRCServer);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_CONNECT_GNUTELLA, m_chkConnectGnutella);
	DDX_Control(pDX, IDC_CHECK_CONNECT_G2, m_chkConnectG2);
	DDX_Control(pDX, IDC_CHECK_FORCE_ULTRA, m_chkForceGnutella);
	DDX_Control(pDX, IDC_CHECK_FORCE_HUB, m_chkForceG2);
}


BEGIN_MESSAGE_MAP(CPrefLocalNetwork, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefLocalNetwork)
	ON_BN_CLICKED(IDC_RADIO_MODEL_INTERNET, OnRadioModelInternet)
	ON_BN_CLICKED(IDC_RADIO_MODEL_PRIVATE, OnRadioModelPrivate)
	ON_BN_CLICKED(IDC_CHECK_IRCSERVER, OnCheckIrcserver)
	ON_EN_CHANGE(IDC_EDIT_IRCSERVER, OnChangeEditIrcserver)
	ON_BN_CLICKED(IDC_CHECK_SUPERNODE, OnCheckSupernode)
	ON_EN_CHANGE(IDC_EDIT_MAXLEAF, OnChangeEditMaxleaf)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()


BOOL CPrefLocalNetwork::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc       = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs   = m_pDoc->m_autPrefs;
	m_autNetwork = m_pDoc->m_autNetwork;
	m_chatPrefs  = m_pDoc->m_pChat->m_pPrefs;


	if( m_pDoc->m_pPrefsEx->m_ConnectGnutella)
		m_chkConnectGnutella.SetCheck(1);
	if( m_pDoc->m_pPrefsEx->m_ConnectG2)
		m_chkConnectG2.SetCheck(1);

	m_ebMaxLeaves.SetWindowText( DWrdtoStr(m_autPrefs->GetMaxLeaves()));

	if(m_autPrefs->GetSuperNodeAble())
	{
		m_chkSuperNode.SetCheck(true);
	}
	else
	{
		m_ebMaxLeaves.EnableWindow(false);
		m_chkForceGnutella.EnableWindow(false);
		m_chkForceG2.EnableWindow(false);
	}
	
	if( m_pDoc->m_pPrefsEx->m_ForceGnuUltra)
		m_chkForceGnutella.SetCheck(1);
	if( m_pDoc->m_pPrefsEx->m_ForceG2Hub)
		m_chkForceG2.SetCheck(1);
	

	//Network Model
	if(m_autPrefs->GetLanMode())
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
		m_chkForceGnutella.EnableWindow(true);
		m_chkForceG2.EnableWindow(true);
	}
	else
	{
		m_ebMaxLeaves.EnableWindow(false);
		m_chkForceGnutella.EnableWindow(false);
		m_chkForceG2.EnableWindow(false);
	}

	SetModified();
}

void CPrefLocalNetwork::OnChangeEditMaxleaf() 
{
	SetModified();
}

BOOL CPrefLocalNetwork::OnApply() 
{
	m_pDoc->m_pPrefsEx->m_ConnectGnutella = m_chkConnectGnutella.GetCheck();
	if(m_chkConnectGnutella.GetCheck())
		m_pDoc->m_autCore->Connect2(NETWORK_GNUTELLA);
	else
		m_pDoc->m_autCore->Disconnect2(NETWORK_GNUTELLA);
	
	m_pDoc->m_pPrefsEx->m_ConnectG2 = m_chkConnectG2.GetCheck();
	if(m_chkConnectG2.GetCheck())
		m_pDoc->m_autCore->Connect2(NETWORK_G2);
	else
		m_pDoc->m_autCore->Disconnect2(NETWORK_G2);

	if(m_chkSuperNode.GetCheck())
	{
		m_autPrefs->SetSuperNodeAble(true);

		CString strTemp;
		m_ebMaxLeaves.GetWindowText(strTemp);
		m_autPrefs->SetMaxLeaves(atoi(strTemp));

		m_pDoc->m_pPrefsEx->m_ForceGnuUltra = m_chkForceGnutella.GetCheck();
		m_pDoc->m_autNetwork->ForceUltrapeer2(m_chkForceGnutella.GetCheck(), NETWORK_GNUTELLA);

		m_pDoc->m_pPrefsEx->m_ForceG2Hub = m_chkForceG2.GetCheck();
		m_pDoc->m_autNetwork->ForceUltrapeer2(m_chkForceG2.GetCheck(), NETWORK_G2);

	}
	else
	{
		m_autPrefs->SetSuperNodeAble(false);

		m_pDoc->m_autNetwork->ForceUltrapeer2(false, NETWORK_GNUTELLA);
		m_pDoc->m_autNetwork->ForceUltrapeer2(false, NETWORK_G2);

		m_pDoc->m_pPrefsEx->m_ForceG2Hub    = false;
		m_pDoc->m_pPrefsEx->m_ForceGnuUltra = false;
	}


	// Network model
	if(((CButton*) GetDlgItem(IDC_RADIO_MODEL_INTERNET))->GetCheck())
	{
		m_autNetwork->LanModeOff();

		m_chatPrefs->m_InternalIRC	= false;
	}

	else if(((CButton*) GetDlgItem(IDC_RADIO_MODEL_PRIVATE))->GetCheck())
	{
		CString store;
		
		m_autNetwork->LanModeOn();
			
		CNetSetup NetSetup(m_pDoc);
		NetSetup.DoModal();
		

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
		m_autNetwork->LanModeOff();


	m_pDoc->m_pPrefsEx->SavePrefsEx( m_pDoc->m_RunPath + "Data\\GnuConfigEx.ini");


	return CPropertyPage::OnApply();
}









void CPrefLocalNetwork::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}
