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

#include "AutShare.h"

#include "ShareExDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CShareExDlg::CShareExDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShareExDlg::IDD, pParent)
{
	m_pDoc = NULL;

	//{{AFX_DATA_INIT(CShareExDlg)
	//}}AFX_DATA_INIT
}


void CShareExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShareExDlg)
	DDX_Control(pDX, IDC_LIST_ALTLOCS, m_lstAltLocs);
	DDX_Control(pDX, IDC_EDIT_BITPRINTHASH, m_ebBitprintHash);
	DDX_Control(pDX, IDC_EDIT_SHA1HASH, m_ebSha1Hash);
	DDX_Control(pDX, IDC_LIST_KEYWORDS, m_lstKeywords);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_ebSize);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ebName);
	DDX_Control(pDX, IDC_EDIT_INDEX, m_ebIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShareExDlg, CDialog)
	//{{AFX_MSG_MAP(CShareExDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CShareExDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_autShare = m_pDoc->m_autShare;
	

	// Setup list box
	int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 3;
	CRect rect;
	m_lstKeywords.GetWindowRect(&rect);

	m_lstKeywords.InsertColumn( 0, "Word", LVCFMT_LEFT, (rect.Width() - offSet) * 4/5, 0);
	m_lstKeywords.InsertColumn( 1, "Hash", LVCFMT_RIGHT, (rect.Width() - offSet) * 1/5, 1);

	m_lstAltLocs.InsertColumn( 0, "Alt Location",  LVCFMT_LEFT, (rect.Width() - offSet) * 10/10, 0);


	
	m_ebName.SetWindowText( m_autShare->GetFileName(m_FileID) );
	m_ebIndex.SetWindowText( DWrdtoStr(m_autShare->GetFileIndex(m_FileID)));
	m_ebSize.SetWindowText( CommaIze( DWrdtoStr(m_autShare->GetFileSize(m_FileID))) + " Bytes");

	m_ebSha1Hash.SetWindowText( m_autShare->GetFileHash(m_FileID) );
	m_ebBitprintHash.SetWindowText("");


	// Copy vector to keyword list array
	_variant_t var = m_autShare->GetFileKeywords(m_FileID);
	SAFEARRAY* psa = var.parray;

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> KeywordList;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 KeywordList.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);

	// Fill Hash info
	for(int i = 0; i < KeywordList.size(); i++)
		if(KeywordList[i].Find(":") > 0)
		{
			int pos = KeywordList[i].Find(":");
			m_lstKeywords.InsertItem(i, KeywordList[i].Left(pos), -1);
			m_lstKeywords.SetItemText(i, 1, CommaIze(KeywordList[i].Mid(pos + 1)));
		}


	// Copy vector to alt loc list array
	var = m_autShare->GetFileAltLocs(m_FileID);
	psa = var.parray;

	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> AltLocList;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 AltLocList.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);

	// Fill AltLoc info
	for(i = 0; i < AltLocList.size(); i++)
		m_lstAltLocs.InsertItem(i, AltLocList[i], -1);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
