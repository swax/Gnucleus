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

#include "ViewSearch.h"

#include "AutMeta.h"
#include "AutSearch.h"

#include "SearchMeta.h"


IMPLEMENT_DYNAMIC(CSearchMeta, CDialog)
CSearchMeta::CSearchMeta(CWnd* pParent /*=NULL*/, CFrameMain* pFrame)
	: CDialog(CSearchMeta::IDD, pParent)
{
	m_pFrame    = pFrame;
	m_pDoc      = pFrame->m_pDoc;
	m_autMeta   = m_pDoc->m_autMeta;
	m_autSearch = m_pDoc->m_autSearch;

	m_MetaID = 0;
}

CSearchMeta::~CSearchMeta()
{
}

void CSearchMeta::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_META, m_cmbType);
	DDX_Control(pDX, IDC_LIST_ATTRIBUTES, m_lstAttributes);
	DDX_Control(pDX, IDC_EDIT_QUERY, m_ebQuery);
	DDX_Control(pDX, IDC_COMBO_SPECIFY, m_cmbSpecify);
}


BEGIN_MESSAGE_MAP(CSearchMeta, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_META, OnCbnSelchangeComboMeta)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ATTRIBUTES, OnLvnItemchangedListAttributes)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPECIFY, OnCbnEditchangeComboSpecify)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ATTRIBUTES, OnNMClickListAttributes)
	ON_CBN_SELCHANGE(IDC_COMBO_SPECIFY, OnCbnSelchangeComboSpecify)
END_MESSAGE_MAP()


BOOL CSearchMeta::OnInitDialog()
{
	CDialog::OnInitDialog();


	// Set meta-data combo box
	m_cmbType.InsertString(0, "Unspecified");
	m_cmbType.SelectString(-1, "Unspecified");
	m_cmbType.SetItemData(0, 0);

	VARIANT var = m_autMeta->GetMetaIDs();
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> MetaIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
	{
		CString MetaName = m_autMeta->GetMetaName(nArray[i]);
		
		if(!MetaName.IsEmpty())
			MetaName.SetAt(0, CString(MetaName.GetAt(0)).MakeUpper().GetAt(0) ); // make first char uppercase, fun line
						
		m_cmbType.InsertString(i + 1, MetaName);
		m_cmbType.SetItemData(i + 1, nArray[i]);
	}

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	// Meta-Data listbox
	CRect rect;
	int offSet = ::GetSystemMetrics(SM_CXVSCROLL) + 4;
	
	m_lstAttributes.GetWindowRect(&rect);
	m_lstAttributes.InsertColumn( 0, "Attribute", LVCFMT_LEFT, (rect.Width() - offSet) * 1/2, 0);
	m_lstAttributes.InsertColumn( 1, "Value",     LVCFMT_LEFT, (rect.Width() - offSet) * 1/2, 1);
	m_lstAttributes.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchMeta::OnCbnSelchangeComboMeta()
{
	m_MetaID = m_cmbType.GetItemData( m_cmbType.GetCurSel() );

	m_lstAttributes.DeleteAllItems();

	VARIANT var = m_autMeta->GetAttributeIDs(m_MetaID);
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<int> AtrributeIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 AtrributeIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	for(i = 0; i < AtrributeIDs.size(); i++)
	{
		CString AttrName = m_autMeta->GetAttributeName(m_MetaID, AtrributeIDs[i]);
		
		if(!AttrName.IsEmpty())
			AttrName.SetAt(0, CString(AttrName.GetAt(0)).MakeUpper().GetAt(0) ); // make first char uppercase, fun line
					
		m_lstAttributes.InsertItem(i, AttrName, -1);
		m_lstAttributes.SetItemData(i, AtrributeIDs[i]);
	}

}


void CSearchMeta::OnLvnItemchangedListAttributes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	m_cmbSpecify.ResetContent();
	m_cmbSpecify.EnableWindow(true);

	POSITION pos = m_lstAttributes.GetFirstSelectedItemPosition();
	int nItem	 = m_lstAttributes.GetNextSelectedItem(pos);

	if(0 <= nItem && nItem < m_lstAttributes.GetItemCount())
	{
		int AttributeID = m_lstAttributes.GetItemData(nItem);

		m_cmbSpecify.SetWindowText( m_lstAttributes.GetItemText(nItem, 1));

		if(m_autMeta->GetAttributeReadOnly(m_MetaID, AttributeID))
			m_cmbSpecify.EnableWindow(false);
		
	
		CString Type = m_autMeta->GetAttributeType(m_MetaID, AttributeID);

		if(Type.CompareNoCase("enum") == 0)
		{
			m_cmbSpecify.InsertString(0, "");

			VARIANT var = m_autMeta->GetAttributeEnums(m_MetaID, AttributeID);
			SAFEARRAY* psa = var.parray;

			BSTR* strArray;
			SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

			std::vector<CString> EnumList;
			for(int i = 0; i < psa->rgsabound->cElements; i++)
				m_cmbSpecify.InsertString(i + 1, CString(strArray[i]));

			SafeArrayUnaccessData(psa);
			VariantClear(&var);
		}	
	}

	*pResult = 0;
}

void CSearchMeta::OnCbnEditchangeComboSpecify()
{
	POSITION pos = m_lstAttributes.GetFirstSelectedItemPosition();
	int nItem	 = m_lstAttributes.GetNextSelectedItem(pos);

	if(0 <= nItem && nItem < m_lstAttributes.GetItemCount())
	{
		CString NewVal;
		m_cmbSpecify.GetWindowText(NewVal);

		m_lstAttributes.SetItemText(nItem, 1, NewVal);
	}
}
void CSearchMeta::OnNMClickListAttributes(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_cmbSpecify.SetFocus();
}

void CSearchMeta::OnCbnSelchangeComboSpecify()
{
	POSITION pos = m_lstAttributes.GetFirstSelectedItemPosition();
	int nItem	 = m_lstAttributes.GetNextSelectedItem(pos);

	if(0 <= nItem && nItem < m_lstAttributes.GetItemCount())
	{
		CString NewVal;
		m_cmbSpecify.GetLBText(m_cmbSpecify.GetCurSel(), NewVal);

		m_lstAttributes.SetItemText(nItem, 1, NewVal);
	}
}

void CSearchMeta::OnOK()
{
	
	std::vector<CString> AttributeList;

	for(int i = 0; i < m_lstAttributes.GetItemCount(); i++)
		if( !m_lstAttributes.GetItemText(i, 1).IsEmpty() )
			AttributeList.push_back(m_lstAttributes.GetItemText(i, 0) + ":" + m_lstAttributes.GetItemText(i, 1));


	SAFEARRAYBOUND bounds = {AttributeList.size(), 0};
	SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &bounds);

	BSTR* strArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

	for(int i = 0; i < AttributeList.size(); i++)
		strArray[i] = AttributeList[i].AllocSysString();

	SafeArrayUnaccessData(psa);

	VARIANT vaAttr;
	VariantInit(&vaAttr);

	vaAttr.vt = VT_ARRAY | VT_BSTR;
	vaAttr.parray = psa;


	// Create search window
	CViewSearch* pSearch = m_pFrame->CreateSearchWindow();

	m_ebQuery.GetWindowText(pSearch->m_Search);
	pSearch->UpdateTitle();

	pSearch->m_SearchID = m_autSearch->StartMetaSearch(pSearch->m_Search, m_MetaID, &vaAttr);

	VariantClear(&vaAttr);

	CDialog::OnOK();
}
