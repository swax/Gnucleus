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
#include "Gnucleus.h"
#include "BrowseHostDlg.h"


// CBrowseHostDlg dialog

IMPLEMENT_DYNAMIC(CBrowseHostDlg, CDialog)
CBrowseHostDlg::CBrowseHostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowseHostDlg::IDD, pParent)
{
}

CBrowseHostDlg::~CBrowseHostDlg()
{
}

void CBrowseHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ebHostPort);
}


BEGIN_MESSAGE_MAP(CBrowseHostDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CBrowseHostDlg message handlers

void CBrowseHostDlg::OnBnClickedOk()
{
	m_ebHostPort.GetWindowText(m_HostPort);

	OnOK();
}
