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
#include "AutNetwork.h"
#include "AutUpdate.h"
#include "PrefsEx.h"

#include "PrefLocal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefLocal, CPropertyPage)

CPrefLocal::CPrefLocal() : CPropertyPage(CPrefLocal::IDD)
{
	//{{AFX_DATA_INIT(CPrefLocal)
	//}}AFX_DATA_INIT

	m_pDoc	   = NULL;
	m_autPrefs = NULL;
	m_autNetwork = NULL;
}

CPrefLocal::~CPrefLocal()
{
}

void CPrefLocal::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefLocal)
	DDX_Control(pDX, IDC_CHECK_TRAYMIN, m_chkTrayOnMin);
	DDX_Control(pDX, IDC_CHECK_TRAYCLOSE, m_chkTrayOnClose);
	DDX_Control(pDX, IDC_STATIC_EVOLVE, m_stcEvolve);
	DDX_Control(pDX, IDC_CHECK_EVOLVE, m_chkEvolve);
	DDX_Control(pDX, IDC_COMBO_EVOLVE, m_cmbEvolveMode);
	DDX_Control(pDX, IDC_EDIT_PORT, m_ebPort);
	DDX_Control(pDX, IDC_EDIT_IP, m_ebIP);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_cmbSpeed);
	DDX_Control(pDX, IDC_CHECK_SPEED, m_chkSpeed);
	DDX_Control(pDX, IDC_CHECK_PORT, m_chkPort);
	DDX_Control(pDX, IDC_CHECK_IP, m_chkIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefLocal, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefLocal)
	ON_BN_CLICKED(IDC_CHECK_IP, OnCheckIp)
	ON_EN_CHANGE(IDC_EDIT_IP, OnChangeEditIp)
	ON_BN_CLICKED(IDC_CHECK_PORT, OnCheckPort)
	ON_EN_CHANGE(IDC_EDIT_PORT, OnChangeEditPort)
	ON_BN_CLICKED(IDC_CHECK_SPEED, OnCheckSpeed)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEED, OnEditchangeComboSpeed)
	ON_BN_CLICKED(IDC_CHECK_EVOLVE, OnCheckEvolve)
	ON_CBN_EDITCHANGE(IDC_COMBO_EVOLVE, OnEditchangeComboEvolve)
	ON_BN_CLICKED(IDC_CHECK_TRAYMIN, OnCheckTraymin)
	ON_BN_CLICKED(IDC_CHECK_TRAYCLOSE, OnCheckTrayclose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefLocal::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;
	m_autNetwork = m_pDoc->m_autNetwork;

	IP NodeIP;

	// IP check box
	if(m_autPrefs->GetForcedHost() == 0)
	{
		m_chkIP.SetCheck(false);
		m_ebIP.EnableWindow(false);

		
		NodeIP.S_addr = m_autNetwork->GetLocalIP();
		m_ebIP.SetWindowText( IPtoStr(NodeIP));
	}
	else
	{
		m_chkIP.SetCheck(true);

		NodeIP.S_addr = m_autPrefs->GetForcedHost();
		m_ebIP.SetWindowText( IPtoStr(NodeIP) );
	}

	// Port check box
	if(m_autPrefs->GetForcedPort())
	{
		m_chkPort.SetCheck(true);

		m_ebPort.SetWindowText( DWrdtoStr(m_autPrefs->GetForcedPort()));
	}
	else
	{
		m_chkPort.SetCheck(false);
		m_ebPort.EnableWindow(FALSE);
		m_ebPort.SetWindowText("Random (" + DWrdtoStr(m_autNetwork->GetLocalPort()) + ")");
	}

	// Speed check box
	if(m_autPrefs->GetSpeedStat())
	{
		m_chkSpeed.SetCheck(true);
		m_cmbSpeed.SetWindowText( GetSpeedString(m_autPrefs->GetSpeedStat()) );
	}
	else
	{
		m_chkSpeed.SetCheck(false);
		m_cmbSpeed.EnableWindow(false);
		m_cmbSpeed.SetWindowText( GetSpeedString(m_autNetwork->GetLocalSpeed()) );
	}

	m_cmbSpeed.InsertString(0, "T3 (or Greater)");
	m_cmbSpeed.InsertString(0, "T1");
	m_cmbSpeed.InsertString(0, "DSL");
	m_cmbSpeed.InsertString(0, "Cable");
	m_cmbSpeed.InsertString(0, "ISDN");
	m_cmbSpeed.InsertString(0, "56K Modem");
	m_cmbSpeed.InsertString(0, "28.8 Modem");
	m_cmbSpeed.InsertString(0, "14.4 Modem");
	m_cmbSpeed.InsertString(0, "Cellular Modem");
	m_cmbSpeed.InsertString(0, "");

	// Auto-Update Mode
	m_cmbEvolveMode.InsertString(0, "Release");
	m_cmbEvolveMode.InsertString(1, "Beta");


	if(m_autPrefs->GetUpdate() == UPDATE_RELEASE)
		m_cmbEvolveMode.SelectString(-1, "Release");
	else if(m_autPrefs->GetUpdate() == UPDATE_BETA)
		m_cmbEvolveMode.SelectString(-1, "Beta");
	else
		m_cmbEvolveMode.SelectString(-1, "Release");

	if(m_autPrefs->GetUpdate() == UPDATE_NONE)
		m_cmbEvolveMode.EnableWindow(false);
	else
		m_chkEvolve.SetCheck(true);

	if(m_pDoc->m_pPrefsEx->m_TrayOnMin)
		m_chkTrayOnMin.SetCheck(true);
	if(m_pDoc->m_pPrefsEx->m_TrayOnClose)
		m_chkTrayOnClose.SetCheck(true);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefLocal::OnCheckIp() 
{
	if(m_chkIP.GetCheck())
	{
		m_ebIP.EnableWindow();
		m_ebIP.SetWindowText("");
	}
	else
	{
		m_ebIP.EnableWindow(false);

		IP NodeIP;
		NodeIP.S_addr = m_autNetwork->GetLocalIP();
		m_ebIP.SetWindowText( IPtoStr(NodeIP) );
	}

	SetModified();
}

void CPrefLocal::OnChangeEditIp() 
{
	SetModified();
}

void CPrefLocal::OnCheckPort() 
{
	if(m_chkPort.GetCheck())
	{
		m_ebPort.EnableWindow();
		m_ebPort.SetWindowText("");
	}
	else
	{
		m_ebPort.EnableWindow(false);
		m_ebPort.SetWindowText("Random");
	}

	SetModified();	
}

void CPrefLocal::OnChangeEditPort() 
{
	SetModified();
}

void CPrefLocal::OnCheckSpeed() 
{
	if(m_chkSpeed.GetCheck())
	{
		m_cmbSpeed.EnableWindow();
		m_cmbSpeed.SetWindowText("");
	}
	else
	{
		m_cmbSpeed.EnableWindow(false);
		m_cmbSpeed.SetWindowText( GetSpeedString(m_autNetwork->GetLocalIP()) );
	}

	SetModified();
}

void CPrefLocal::OnEditchangeComboSpeed() 
{
	SetModified();
}


void CPrefLocal::OnCheckEvolve() 
{
	if(m_chkEvolve.GetCheck())
		m_cmbEvolveMode.EnableWindow();
	else
		m_cmbEvolveMode.EnableWindow(false);

	SetModified();
}

void CPrefLocal::OnEditchangeComboEvolve() 
{
	SetModified();
}

void CPrefLocal::OnCheckTraymin() 
{
	SetModified();
}

void CPrefLocal::OnCheckTrayclose() 
{
	SetModified();
}


BOOL CPrefLocal::OnApply() 
{
	CString store;


	// IP check box
	if(m_chkIP.GetCheck())
	{
		m_ebIP.GetWindowText(store);
		
		m_autPrefs->SetForcedHost(StrtoIP(store).S_addr);
	}
	else
		m_autPrefs->SetForcedHost(0);


	m_ebPort.GetWindowText(store);

	// If forcing port selected
	if(m_chkPort.GetCheck() && atol(store) > 0)
		m_autPrefs->SetForcedPort(atol(store));

	else
		m_autPrefs->SetForcedPort(0);

	
	// Speed check box
	if(m_chkSpeed.GetCheck())
	{
		m_cmbSpeed.GetWindowText(store);

		m_autPrefs->SetSpeedStat(GetSpeedinBytes(store));
	}
	else
		m_autPrefs->SetSpeedStat(0);


	// Auto-Update
	if(m_chkEvolve.GetCheck())
	{
		m_cmbEvolveMode.GetWindowText(store);

		int OldPref = m_autPrefs->GetUpdate();

		if(store == "Release")
			m_autPrefs->SetUpdate(UPDATE_RELEASE);
		else if(store == "Beta")
			m_autPrefs->SetUpdate(UPDATE_BETA);
		else
			m_autPrefs->SetUpdate(UPDATE_RELEASE);

		if(OldPref != m_autPrefs->GetUpdate())
			m_pDoc->m_autUpdate->Check();
	}
	else
		m_autPrefs->SetUpdate(UPDATE_NONE);


	// Tray stuff
	m_pDoc->m_pPrefsEx->m_TrayOnMin   = m_chkTrayOnMin.GetCheck()   ? true : false;
	m_pDoc->m_pPrefsEx->m_TrayOnClose = m_chkTrayOnClose.GetCheck() ? true : false;


	return CPropertyPage::OnApply();
}


