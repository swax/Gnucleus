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
#include "AutMeta.h"

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

	m_MetaID = 0;

	//{{AFX_DATA_INIT(CShareExDlg)
	//}}AFX_DATA_INIT
}


void CShareExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_ALTLOCS, m_lstAltLocs);
	DDX_Control(pDX, IDC_EDIT_SHA1HASH, m_ebSha1Hash);
	DDX_Control(pDX, IDC_LIST_KEYWORDS, m_lstKeywords);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_ebSize);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ebName);
	DDX_Control(pDX, IDC_EDIT_INDEX, m_ebIndex);
	DDX_Control(pDX, IDC_STATIC_METADATA, m_stcMetaData);
	DDX_Control(pDX, IDC_LIST_METADATA, m_lstMetaData);
	DDX_Control(pDX, IDC_COMBO1, m_cmbModify);
	DDX_Control(pDX, IDC_BUTTON_SET, m_btnSet);
}


BEGIN_MESSAGE_MAP(CShareExDlg, CDialog)
	//{{AFX_MSG_MAP(CShareExDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_METADATA, OnLvnItemchangedListMetadata)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(ID_BUTTON_OK, OnBnClickedButtonOk)
END_MESSAGE_MAP()



BOOL CShareExDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_autShare = m_pDoc->m_autShare;
	m_autMeta  = m_pDoc->m_autMeta;


	m_ebName.SetWindowText( m_autShare->GetFileName(m_FileID) );
	m_ebIndex.SetWindowText( DWrdtoStr(m_autShare->GetFileIndex(m_FileID)));
	m_ebSize.SetWindowText( CommaIze( DWrdtoStr(m_autShare->GetFileSize(m_FileID))) + " Bytes");

	m_ebSha1Hash.SetWindowText( m_autShare->GetFileHash(m_FileID) );

	
	int i = 0;
	int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 3;
	CRect rect;
	
	// Meta-Data listbox
	m_lstMetaData.GetWindowRect(&rect);
	m_lstMetaData.InsertColumn( 0, "Attribute", LVCFMT_LEFT, (rect.Width() - offSet) * 1/2, 0);
	m_lstMetaData.InsertColumn( 1, "Value",     LVCFMT_LEFT, (rect.Width() - offSet) * 1/2, 1);

	m_lstMetaData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	// Keyword listbox
	m_lstKeywords.GetWindowRect(&rect);
	m_lstKeywords.InsertColumn( 0, "Word", LVCFMT_LEFT, (rect.Width() - offSet) * 3/5, 0);
	m_lstKeywords.InsertColumn( 1, "Hash", LVCFMT_RIGHT, (rect.Width() - offSet) * 2/5, 1);

	m_lstKeywords.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	// Alt-loc listbox
	m_lstAltLocs.GetWindowRect(&rect);
	m_lstAltLocs.InsertColumn( 0, "Alt Location",  LVCFMT_LEFT, (rect.Width() - offSet) * 1/1, 0);

	m_lstAltLocs.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	
	// Set Meta Name	
	m_MetaID = m_autShare->GetFileMetaID(m_FileID);
	
	CString MetaName = m_autMeta->GetMetaName(m_MetaID);

	if(!MetaName.IsEmpty())
		MetaName.SetAt(0, CString(MetaName.GetAt(0)).MakeUpper().GetAt(0) ); // make first char uppercase, fun line
					
	m_stcMetaData.SetWindowText("Meta-Data: " + MetaName);


	// Set MetaData listbox
	_variant_t var = m_autMeta->GetAttributeIDs(m_MetaID);
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> AtrributeIDs;
	for(i = 0; i < psa->rgsabound->cElements; i++)
		 AtrributeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);

	for(i = 0; i < AtrributeIDs.size(); i++)
	{
		CString AttrName = m_autMeta->GetAttributeName(m_MetaID, AtrributeIDs[i]);

		if(!AttrName.IsEmpty())
			AttrName.SetAt(0, CString(AttrName.GetAt(0)).MakeUpper().GetAt(0) ); // make first char uppercase, fun line
					
		m_lstMetaData.InsertItem(i, AttrName, -1);
		m_lstMetaData.SetItemText(i , 1, m_autShare->GetFileAttributeValue(m_FileID, AtrributeIDs[i]));
		m_lstMetaData.SetItemData(i, AtrributeIDs[i]);
	}


	// Set Keyword listbox
	var = m_autShare->GetFileKeywords(m_FileID);
	psa = var.parray;

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> KeywordList;
	for(i = 0; i < psa->rgsabound->cElements; i++)
		 KeywordList.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);

	for(i = 0; i < KeywordList.size(); i++)
		if(KeywordList[i].Find(":") > 0)
		{
			int pos = KeywordList[i].Find(":");
			m_lstKeywords.InsertItem(i, KeywordList[i].Left(pos), -1);
			m_lstKeywords.SetItemText(i, 1, CommaIze(KeywordList[i].Mid(pos + 1)));
		}


	// Set AltLoc Listbox
	var = m_autShare->GetFileAltLocs(m_FileID);
	psa = var.parray;

	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	std::vector<CString> AltLocList;
	for(i = 0; i < psa->rgsabound->cElements; i++)
		 AltLocList.push_back(strArray[i]);

	SafeArrayUnaccessData(psa);

	// Fill AltLoc info
	for(i = 0; i < AltLocList.size(); i++)
		m_lstAltLocs.InsertItem(i, AltLocList[i], -1);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShareExDlg::OnLvnItemchangedListMetadata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	m_cmbModify.ResetContent();
	m_cmbModify.EnableWindow(true);
	m_btnSet.EnableWindow(true);
	

	POSITION pos = m_lstMetaData.GetFirstSelectedItemPosition();
	int nItem	 = m_lstMetaData.GetNextSelectedItem(pos);

	if(0 <= nItem && nItem < m_lstMetaData.GetItemCount())
	{
		int AttributeID = m_lstMetaData.GetItemData(nItem);

		m_cmbModify.SetWindowText( m_autShare->GetFileAttributeValue(m_FileID, AttributeID));

		if(m_autMeta->GetAttributeReadOnly(m_MetaID, AttributeID))
		{
			m_cmbModify.EnableWindow(false);
			m_btnSet.EnableWindow(false);
		}
		
	
		CString Type = m_autMeta->GetAttributeType(m_MetaID, AttributeID);

		if(Type.CompareNoCase("enum") == 0)
		{
			m_cmbModify.InsertString(0, "");

			_variant_t var = m_autMeta->GetAttributeEnums(m_MetaID, AttributeID);
			SAFEARRAY* psa = var.parray;

			BSTR* strArray;
			SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

			std::vector<CString> EnumList;
			for(int i = 0; i < psa->rgsabound->cElements; i++)
				m_cmbModify.InsertString(i + 1, CString(strArray[i]));

			SafeArrayUnaccessData(psa);
		}
	}
	

	*pResult = 0;
}

void CShareExDlg::OnBnClickedButtonSet()
{
	POSITION pos	= m_lstMetaData.GetFirstSelectedItemPosition();
	int nItem		= m_lstMetaData.GetNextSelectedItem(pos);
	
	if(0 <= nItem && nItem < m_lstMetaData.GetItemCount())
	{
		int AttributeID = m_lstMetaData.GetItemData(nItem);

		CString Value;
		m_cmbModify.GetWindowText(Value);

		// Set new value
 		LVFINDINFO info;
 		info.flags  = LVFI_PARAM;
 		info.lParam = (LPARAM) AttributeID;
	 
 		int row = m_lstMetaData.FindItem(&info);

		m_lstMetaData.SetItemText(row , 1, Value);


		for(int i = 0; i < m_ModifiedIDs.size(); i++)
			if(m_ModifiedIDs[i] == AttributeID)
				return;

		m_ModifiedIDs.push_back(AttributeID);
	}
}

void CShareExDlg::OnBnClickedButtonOk()
{
	for(int i = 0; i < m_ModifiedIDs.size(); i++)
	{
		LVFINDINFO info;
 		info.flags  = LVFI_PARAM;
 		info.lParam = (LPARAM) m_ModifiedIDs[i];
	 
 		int row = m_lstMetaData.FindItem(&info);

		m_autShare->SetFileAttributeValue(m_FileID, m_ModifiedIDs[i], m_lstMetaData.GetItemText(row, 1));
	}

	OnOK();
}
