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
#include "AutNetwork.h"

#include "NetSetupNew.h"
#include "NetSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetSetup dialog


CNetSetup::CNetSetup(CGnucleusDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CNetSetup::IDD, pParent)
{
	m_pDoc			= pDoc;
	m_autPrefs		= pDoc->m_autPrefs;
	m_autNetwork    = pDoc->m_autNetwork;
	
	m_ListSize = 0;

	//{{AFX_DATA_INIT(CNetSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CNetSetup::~CNetSetup()
{

}


void CNetSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetSetup)
	DDX_Control(pDX, IDC_LIST_LAN, m_lstLan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetSetup, CDialog)
	//{{AFX_MSG_MAP(CNetSetup)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_JOIN, OnButtonJoin)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetSetup message handlers

BOOL CNetSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// Setup LAN listbox
	CRect rect;
	m_lstLan.GetWindowRect(&rect);
	int nWidth = rect.Width() - 5;

	m_lstLan.InsertColumn( 0, "Network", LVCFMT_LEFT,
		nWidth  * 3/4, 0);
	m_lstLan.InsertColumn( 1, "Users", LVCFMT_RIGHT,
		nWidth * 1/4, 1);

	m_lstLan.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	SetTimer(1, 500, NULL);
	

	return TRUE; 
}

void CNetSetup::OnTimer(UINT nIDEvent) 
{
	// Get current node IDs and put them into a vector
	_variant_t var = m_autNetwork->GetLanNodeIDs();
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> LanNodeIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 LanNodeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);


	if(!LanNodeIDs.size())
		return;

	m_lstLan.SetRedraw(false);


	for(i = 0; i < m_lstLan.GetItemCount(); i++)
		m_lstLan.SetItemText(i, 1, "0");


	for(i = 0; i < LanNodeIDs.size(); i++)
	{
		if(m_autNetwork->GetLanNodeName(LanNodeIDs[i]) == "")
			continue;

		LVFINDINFO info;
		info.flags = LVFI_STRING;
		info.psz   = m_autNetwork->GetLanNodeName(LanNodeIDs[i]);

		int pos = m_lstLan.FindItem(&info);

		if(pos == -1)
		{
			pos = m_lstLan.GetItemCount();
			m_lstLan.InsertItem(pos, m_autNetwork->GetLanNodeName(LanNodeIDs[i]));
			m_lstLan.SetItemText(pos, 1, DWrdtoStr(1 + m_autNetwork->GetLanNodeLeaves(LanNodeIDs[i])));
		}
		else
		{
			int Users = atoi( m_lstLan.GetItemText(pos, 1));

			Users += 1 + m_autNetwork->GetLanNodeLeaves(LanNodeIDs[i]);

			m_lstLan.SetItemText(pos, 1, DWrdtoStr(Users));
		}
	}

	m_lstLan.SetRedraw();

	// If nothing selected select default
	int sel = m_lstLan.GetSelectionMark();
	if(sel == -1 && m_autPrefs->GetLanName() != "")
	{
		LVFINDINFO info;
		info.flags = LVFI_STRING;
		info.psz   = m_autPrefs->GetLanName();

		int ipos = m_lstLan.FindItem(&info);

		if(ipos != -1)
		{
			m_lstLan.SetItemState(ipos, LVIS_SELECTED, LVIS_SELECTED);
			m_lstLan.SetSelectionMark(ipos);
		}
	}
	
	
	CDialog::OnTimer(nIDEvent);
}

void CNetSetup::OnButtonJoin() 
{
	int pos = m_lstLan.GetSelectionMark();

	if(pos == -1)
		return;

	CString Network	= m_lstLan.GetItemText(pos, 0);

	m_autNetwork->LanModeOn(Network);

	OnOK();
}

void CNetSetup::OnButtonNew() 
{
	//AfxMessageBox( DWrdtoStr(m_pLAN->m_HostList.size()));

	// Open Setup Dialog
	CNetSetupNew NewNetworkDlg;
	
	if(NewNetworkDlg.DoModal() == IDOK)
	{
		if(NewNetworkDlg.m_Name == "")
			return;

		m_autNetwork->LanModeOn(NewNetworkDlg.m_Name);

		OnOK();
	}
	else
		return;
}

