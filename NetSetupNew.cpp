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

#include "NetSetupNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetSetupNew dialog


CNetSetupNew::CNetSetupNew(CWnd* pParent /*=NULL*/)
	: CDialog(CNetSetupNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetSetupNew)
	//}}AFX_DATA_INIT
}


void CNetSetupNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetSetupNew)
	DDX_Control(pDX, IDC_EDIT_NAME, m_ebName);
	DDX_Control(pDX, IDC_EDIT_IRC, m_ebIRC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetSetupNew, CDialog)
	//{{AFX_MSG_MAP(CNetSetupNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetSetupNew message handlers

void CNetSetupNew::OnOK() 
{
	m_ebName.GetWindowText(m_Name);
	m_ebIRC.GetWindowText(m_IRCServer);
	
	CDialog::OnOK();
}
