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
#include "FrameMain.h"

#include "AutUpdate.h"

#include "EvolverDlg.h"


IMPLEMENT_DYNAMIC(CEvolverDlg, CDialog)
CEvolverDlg::CEvolverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvolverDlg::IDD, pParent)
{
	m_pDoc = NULL;

	m_NotifyTimer  = 0;
	m_UpgradeTimer = 0;
	m_CloseTimer   = 0;
}

CEvolverDlg::~CEvolverDlg()
{
}

void CEvolverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvolverDlg)
	DDX_Control(pDX, IDC_STATIC_STATUS, m_stcStatus);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_stcProgress);
	DDX_Control(pDX, IDC_STATIC_LABEL, m_stcLabel);
	DDX_Control(pDX, IDC_PROGRESS_UPDATE, m_progBar);
	DDX_Control(pDX, IDC_BUTTON_EVOVLE, m_btnEvolve);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvolverDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_EVOLVE, OnBnClickedButtonEvolve)
END_MESSAGE_MAP()


// CEvolverDlg message handlers

BOOL CEvolverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT MainWin;
	AfxGetApp()->GetMainWnd()->GetWindowRect(&MainWin);
	SetWindowPos(NULL, MainWin.right - 390, MainWin.bottom - 170, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_btnEvolve.EnableWindow(false);
	m_progBar.SetRange(0, 100);

	//m_CheckTimer = SetTimer(4, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEvolverDlg::OnUpdateFound(CString Version)
{
	CString Title = "Update to ";
	m_stcStatus.SetWindowText(Title + Version + " Detected");
	m_progBar.SetPos(0);
	m_stcProgress.SetWindowText("Evolution in 15 seconds...");
	
	//CString Remaining = DWrdtoStr( m_FileList.size() );
	//Remaining += m_FileList.size() == 1 ? " File" : " Files";
	//m_stcLabel.SetWindowText(Remaining);

	m_btnEvolve.EnableWindow(true);

	ShowWindow(SW_SHOW);

	// Set the countdown timer
	m_progBar.SetRange(0, 150);

	m_Counter = 0;
	m_NotifyTimer = SetTimer(1, 100, NULL);
}

void CEvolverDlg::OnUpdateFailed(CString Reason)
{
	m_stcProgress.SetWindowText("Error: " + Reason);
}

void CEvolverDlg::OnVersionCurrent()
{
	m_stcProgress.SetWindowText("No Updates Available");
	m_btnCancel.SetWindowText("&Close");
}

void CEvolverDlg::OnBnClickedButtonCancel()
{
	m_pDoc->m_pDiagEvolve = NULL;

	m_autUpdate->CancelUpdate();

	DestroyWindow();
}

void CEvolverDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == m_NotifyTimer)
	{
		if(m_Counter == 0)
		{
			// Show changelog
			CFrameMain* pFrame = (CFrameMain*) AfxGetApp()->GetMainWnd();
			pFrame->OpenBrowser("http://www.gnucleus.com/update2/ChangeLog.txt");
		}

		m_Counter++;
		m_progBar.SetPos(m_Counter);
		m_stcProgress.SetWindowText("Evolution in " + DWrdtoStr((150 - m_Counter) / 10) + " seconds...");
	
		if(m_Counter == 150)
			StartDownload();
	}

	if(nIDEvent == m_UpgradeTimer)
	{
		// Get File IDs
		std::vector<int> FileIDs;

		VARIANT var = m_autUpdate->GetFileIDs();
		SAFEARRAY* psa = var.parray;

		int* nArray;
		SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

		for(int i = 0; i < psa->rgsabound->cElements; i++)
			FileIDs.push_back(nArray[i]);

		SafeArrayUnaccessData(psa);
		VariantClear(&var);

		// Update status
		if(m_autUpdate->GetTotalCompleted())
			m_stcProgress.SetWindowText("Downloading Files...");

		if(m_autUpdate->GetTotalSize())
			m_progBar.SetPos(m_autUpdate->GetTotalCompleted() * 100 / m_autUpdate->GetTotalSize());
	}

	if(nIDEvent == m_CloseTimer)
	{
		m_Counter++;
		m_progBar.SetPos(m_Counter);
		m_stcProgress.SetWindowText("Restarting in " + DWrdtoStr((150 - m_Counter) / 10) + " seconds...");
	
		if(m_Counter == 150)
		{
			Restart();
			return;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CEvolverDlg::OnBnClickedButtonEvolve()
{
	if(m_NotifyTimer)
	{
		StartDownload();
	}

	// Else Restart 
	else
	{
		Restart();
		return;
	}
}

void CEvolverDlg::StartDownload()
{
	KillTimer(m_NotifyTimer);
	m_NotifyTimer = 0;

	m_UpgradeTimer = SetTimer(2, 1000, NULL);
	//m_DialogMode = DIALOG_UPDATE;

	m_stcStatus.SetWindowText("Downloading Upgrade");
	m_stcProgress.SetWindowText("");

	m_progBar.SetRange(0, 100);
	m_progBar.SetPos(0);

	m_btnEvolve.EnableWindow(false);

	m_stcProgress.SetWindowText("Searching for files over network...");

	m_autUpdate->StartDownload();
}

void CEvolverDlg::OnUpdateComplete()
{
	KillTimer(m_UpgradeTimer);
	m_UpgradeTimer = 0;

	m_stcStatus.SetWindowText("Download Complete");
	
	m_progBar.SetPos(0);
	m_stcProgress.SetWindowText("Restarting in 15 seconds...");


	m_btnEvolve.EnableWindow(true);

	ShowWindow(SW_SHOW);

	// Set the countdown timer
	m_progBar.SetRange(0, 150);

	m_Counter = 0;
	m_CloseTimer = SetTimer(3, 100, NULL);
}	

void CEvolverDlg::Restart()
{
	KillTimer(m_CloseTimer);
	m_CloseTimer = 0;

	m_autUpdate->LaunchUpdate();
	
	m_pDoc->m_ShutDown = true;

	AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
	return;
}


void CEvolverDlg::PostNcDestroy()
{
	delete this;

	CDialog::PostNcDestroy();
}
