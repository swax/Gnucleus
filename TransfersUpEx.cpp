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
#include "Gnucleus.h"

#include "TransfersUpEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTransfersUpEx::CTransfersUpEx(CWnd* pParent /*=NULL*/)
	: CDialog(CTransfersUpEx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransfersUpEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTransfersUpEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransfersUpEx)
	DDX_Control(pDX, IDC_STATIC_ATTEMPTS, m_stcAttempts);
	DDX_Control(pDX, IDC_STATIC_NAME, m_stcName);
	DDX_Control(pDX, IDC_STATIC_INDEX, m_stcIndex);
	DDX_Control(pDX, IDC_STATIC_HOST, m_stcHost);
	DDX_Control(pDX, IDC_EDIT_HANDSHAKE, m_ebHandshake);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransfersUpEx, CDialog)
	//{{AFX_MSG_MAP(CTransfersUpEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTransfersUpEx::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Handshake.Replace("\n\n", "\r\n\r\n");

	m_stcName.SetWindowText(m_FileName);
	m_stcIndex.SetWindowText( CommaIze(DWrdtoStr(m_FileIndex)));
	
	CString Host = IPtoStr(m_Host);
			Host += ":" + DWrdtoStr(m_Port);

	m_stcHost.SetWindowText(Host);
	m_stcAttempts.SetWindowText( DWrdtoStr(m_Attempts) );
	m_ebHandshake.SetWindowText(m_Handshake);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
