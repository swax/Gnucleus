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
#include "gnucleus.h"
#include "GnucleusDoc.h"
#include "PrefsEx.h"

#include "SchedulerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSchedulerDlg dialog


CSchedulerDlg::CSchedulerDlg(CWnd* pParent /*=NULL*/, CGnucleusDoc* pDoc)
	: CDialog(CSchedulerDlg::IDD, pParent)
{
	m_pDoc     = pDoc;
	m_pPrefsEx = pDoc->m_pPrefsEx;

	//{{AFX_DATA_INIT(CSchedulerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSchedulerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSchedulerDlg)
	DDX_Control(pDX, IDC_STATIC_SCHED_FRAME, m_stcScheduler);
	DDX_Control(pDX, IDC_STATIC_SCHED_DISCONNECT, m_stcDisconnect);
	DDX_Control(pDX, IDC_STATIC_SCHED_CONNECT, m_stcConnect);
	DDX_Control(pDX, IDC_SCHED_INACTIVE, m_tmDisconnect);
	DDX_Control(pDX, IDC_SCHED_ACTIVE, m_tmConnect);
	DDX_Control(pDX, IDC_CHECK_SCHED_ACTIVATED, m_chkScheduler);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSchedulerDlg, CDialog)
	//{{AFX_MSG_MAP(CSchedulerDlg)
	ON_BN_CLICKED(IDC_CHECK_SCHED_ACTIVATED, OnCheckSchedActivated)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchedulerDlg message handlers


BOOL CSchedulerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_tmConnect.SetFormat("HH:mm");
	m_tmDisconnect.SetFormat("HH:mm");
	

	if(m_pPrefsEx->m_SchedActive)
	{
		m_chkScheduler.SetCheck(true);
	}
	else
	{
		m_stcScheduler.EnableWindow(false);

		m_stcConnect.EnableWindow(false);
		m_stcDisconnect.EnableWindow(false);
		m_tmConnect.EnableWindow(false);
		m_tmDisconnect.EnableWindow(false);
	}


	m_tmConnect.SetTime(&m_pPrefsEx->m_SchedConnect);
	m_tmDisconnect.SetTime(&m_pPrefsEx->m_SchedDisconnect);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSchedulerDlg::OnCheckSchedActivated() 
{
	if(m_chkScheduler.GetCheck())
	{
		m_stcScheduler.EnableWindow();

		m_stcConnect.EnableWindow();
		m_stcDisconnect.EnableWindow();
		m_tmConnect.EnableWindow();
		m_tmDisconnect.EnableWindow();
	}
	else
	{
		m_stcScheduler.EnableWindow(false);

		m_stcConnect.EnableWindow(false);
		m_stcDisconnect.EnableWindow(false);
		m_tmConnect.EnableWindow(false);
		m_tmDisconnect.EnableWindow(false);
	}
	
}

void CSchedulerDlg::OnOK() 
{
	if(m_chkScheduler.GetCheck())
	{
		m_pPrefsEx->m_SchedActive = true;

		m_tmConnect.GetTime(m_pPrefsEx->m_SchedConnect);
		m_tmDisconnect.GetTime(m_pPrefsEx->m_SchedDisconnect);
	}
	else
		m_pPrefsEx->m_SchedActive = false;
	
	CDialog::OnOK();
}
