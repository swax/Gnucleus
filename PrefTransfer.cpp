/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2003 John Marshall

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

#include "PrefTransfer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefTransfer, CPropertyPage)

CPrefTransfer::CPrefTransfer() : CPropertyPage(CPrefTransfer::IDD)
{
	//{{AFX_DATA_INIT(CPrefTransfer)
	//}}AFX_DATA_INIT

	m_pDoc     = NULL;
	m_autPrefs = NULL;
}

CPrefTransfer::~CPrefTransfer()
{
}

void CPrefTransfer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefTransfer)
	DDX_Control(pDX, IDC_CHECK_MULTISOURCE, m_chkMultisource);
	DDX_Control(pDX, IDC_EDIT_PATH, m_ebPath);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_EDIT_MAX_UL, m_ebMaxUL);
	DDX_Control(pDX, IDC_EDIT_MAX_DL, m_ebMaxDL);
	DDX_Control(pDX, IDC_CHECK_MAX_UL, m_chkMaxUL);
	DDX_Control(pDX, IDC_CHECK_MAX_DL, m_chkMaxDL);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PARTIALS, m_ebPartialPath);
}


BEGIN_MESSAGE_MAP(CPrefTransfer, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefTransfer)
	ON_BN_CLICKED(IDC_CHECK_MAX_DL, OnCheckMaxDl)
	ON_EN_CHANGE(IDC_EDIT_MAX_DL, OnChangeEditMaxDl)
	ON_BN_CLICKED(IDC_CHECK_MAX_UL, OnCheckMaxUl)
	ON_EN_CHANGE(IDC_EDIT_MAX_UL, OnChangeEditMaxUl)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_PATH, OnChangeEditPath)
	ON_BN_CLICKED(IDC_CHECK_MULTISOURCE, OnCheckMultisource)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PARTIALS, OnBnClickedButtonBrowsePartials)
	ON_EN_CHANGE(IDC_EDIT_PARTIALS, OnEnChangeEditPartials)
END_MESSAGE_MAP()




BOOL CPrefTransfer::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;


	// Download directory
	m_ebPath.SetWindowText(m_autPrefs->GetDownloadPath());

	m_ebPartialPath.SetWindowText(m_autPrefs->GetPartialsDir());

	// Download max
	if(m_autPrefs->GetMaxDownloads())
	{
		m_chkMaxDL.SetCheck(true);
		m_ebMaxDL.SetWindowText( DWrdtoStr(m_autPrefs->GetMaxDownloads()) );
	}
	else
		m_ebMaxDL.EnableWindow(false);

	// Upload max
	if(m_autPrefs->GetMaxUploads())
	{
		m_chkMaxUL.SetCheck(true);
		m_ebMaxUL.SetWindowText( DWrdtoStr(m_autPrefs->GetMaxUploads()) );
	}
	else
		m_ebMaxUL.EnableWindow(false);

	// Enable 
	if(!m_autPrefs->GetMultisource())
		m_chkMultisource.SetCheck(true);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefTransfer::OnButtonBrowse() 
{
	// Get the directory to add from the user
	char *Directory = new char[255];

	LPBROWSEINFO Settings	 = new BROWSEINFO;
	Settings->hwndOwner		 = m_hWnd;
	Settings->pidlRoot		 = NULL;
	Settings->pszDisplayName = Directory;
	Settings->lpszTitle		 = "Save Downloads to...";
	Settings->ulFlags		 = BIF_RETURNONLYFSDIRS;
	Settings->lpfn			 = NULL;
	Settings->lParam		 = NULL;
	Settings->iImage		 = NULL;

	LPCITEMIDLIST FolderID = SHBrowseForFolder(Settings);
	delete Settings;

	if(FolderID)
	{
		SHGetPathFromIDList(FolderID, Directory);
		m_ebPath.SetWindowText(Directory);
	}

	delete [] Directory;
}

void CPrefTransfer::OnChangeEditPath() 
{
	SetModified();
}

void CPrefTransfer::OnBnClickedButtonBrowsePartials()
{
		// Get the directory to add from the user
	char *Directory = new char[255];

	LPBROWSEINFO Settings	 = new BROWSEINFO;
	Settings->hwndOwner		 = m_hWnd;
	Settings->pidlRoot		 = NULL;
	Settings->pszDisplayName = Directory;
	Settings->lpszTitle		 = "Save Partials to...";
	Settings->ulFlags		 = BIF_RETURNONLYFSDIRS;
	Settings->lpfn			 = NULL;
	Settings->lParam		 = NULL;
	Settings->iImage		 = NULL;

	LPCITEMIDLIST FolderID = SHBrowseForFolder(Settings);
	delete Settings;

	if(FolderID)
	{
		SHGetPathFromIDList(FolderID, Directory);
		m_ebPartialPath.SetWindowText(Directory);
	}

	delete [] Directory;
}

void CPrefTransfer::OnEnChangeEditPartials()
{
	SetModified();
}

void CPrefTransfer::OnCheckMaxDl() 
{
	if(m_chkMaxDL.GetCheck())
		m_ebMaxDL.EnableWindow(true);
	else
		m_ebMaxDL.EnableWindow(false);

	SetModified();
}

void CPrefTransfer::OnChangeEditMaxDl() 
{
	SetModified();
}

void CPrefTransfer::OnCheckMaxUl() 
{
	if(m_chkMaxUL.GetCheck())
		m_ebMaxUL.EnableWindow(true);
	else
		m_ebMaxUL.EnableWindow(false);

	SetModified();
}

void CPrefTransfer::OnChangeEditMaxUl() 
{
	SetModified();
}

void CPrefTransfer::OnCheckMultisource() 
{
	SetModified();	
}

BOOL CPrefTransfer::OnApply() 
{
	CString store;

	// Download path
	m_ebPath.GetWindowText(store);
	m_autPrefs->SetDownloadPath(store);
	CreateDirectory(m_autPrefs->GetDownloadPath(), NULL);

	m_ebPartialPath.GetWindowText(store);
	m_autPrefs->SetPartialsDir(store);
	CreateDirectory(m_autPrefs->GetPartialsDir(), NULL);


	// Max Downloads
	if(m_chkMaxDL.GetCheck())
	{
		m_ebMaxDL.GetWindowText(store);
		
		m_autPrefs->SetMaxDownloads(atoi(store));
	}
	else
		m_autPrefs->SetMaxDownloads(0);	

	// Max Uploads
	if(m_chkMaxUL.GetCheck())
	{
		m_ebMaxUL.GetWindowText(store);
		
		m_autPrefs->SetMaxUploads(atoi(store));
	}
	else
		m_autPrefs->SetMaxUploads(0);	
	
	if(m_chkMultisource.GetCheck())
		m_autPrefs->SetMultisource(false);
	else
		m_autPrefs->SetMultisource(true);


	return CPropertyPage::OnApply();
}





