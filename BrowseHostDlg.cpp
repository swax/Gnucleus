// BrowseHostDlg.cpp : implementation file
//

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
