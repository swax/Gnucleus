/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2004 John Marshall

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
#include "AutCache.h"

#include "WebCacheDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebCacheDlg dialog


CWebCacheDlg::CWebCacheDlg(CWnd* pParent /*=NULL*/, CGnucleusDoc* pDoc)
	: CDialog(CWebCacheDlg::IDD, pParent)
{
	m_pDoc   = pDoc;


	//{{AFX_DATA_INIT(CWebCacheDlg)
	//}}AFX_DATA_INIT
}


void CWebCacheDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebCacheDlg)
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_PROGRESS_ADD, m_prgAdd);
	DDX_Control(pDX, IDC_EDIT_NEWCACHE, m_ebNewCache);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebCacheDlg, CDialog)
	//{{AFX_MSG_MAP(CWebCacheDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCacheDlg message handlers

BOOL CWebCacheDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_prgAdd.SetRange(0, 10);
	//m_pCache->m_TotalSites = 1;
	//m_pCache->m_TriedSites = 0;

	SetTimer(1, 1000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CWebCacheDlg::OnButtonClose() 
{
	OnOK();
}

void CWebCacheDlg::OnTimer(UINT nIDEvent) 
{
	//m_prgAdd.SetRange(0, m_pCache->m_TotalSites);
	//m_prgAdd.SetPos(m_pCache->m_TriedSites);
	
	CDialog::OnTimer(nIDEvent);
}



void CWebCacheDlg::OnButtonAdd() 
{
	CString NewSite;

	m_ebNewCache.GetWindowText(NewSite);

	if(!NewSite.IsEmpty())
	{
		m_pDoc->m_autCache->SeedWebCache(NewSite);
		m_btnAdd.EnableWindow(false);
	}
}
