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
#include "GnucleusDoc.h"

#include "AutPrefs.h"
#include "AutShare.h"

#include "PrefShare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefShare, CPropertyPage)

CPrefShare::CPrefShare() : CPropertyPage(CPrefShare::IDD)
{
	//{{AFX_DATA_INIT(CPrefShare)
	//}}AFX_DATA_INIT

	m_pDoc     = NULL;
	m_autPrefs = NULL;
	m_autShare = NULL;
}

CPrefShare::~CPrefShare()
{
}

void CPrefShare::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefShare)
	DDX_Control(pDX, IDC_CHECK_SENDRESULTS, m_chkSendResults);
	DDX_Control(pDX, IDC_EDIT_REPLY, m_ebReply);
	DDX_Control(pDX, IDC_CHECK_REPLYMAX, m_chkReply);
	DDX_Control(pDX, IDC_LIST_SHARED, m_lstShared);
	DDX_Control(pDX, IDC_EDIT_SHARED, m_ebShared);
	DDX_Control(pDX, IDC_CHECK_RECURSE, m_chkRecurse);
	DDX_Control(pDX, IDC_CHECK_PATH, m_chkPath);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefShare, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefShare)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_EN_CHANGE(IDC_EDIT_SHARED, OnChangeEditShared)
	ON_BN_CLICKED(IDC_CHECK_PATH, OnCheckPath)
	ON_BN_CLICKED(IDC_CHECK_REPLYMAX, OnCheckReplymax)
	ON_EN_CHANGE(IDC_EDIT_REPLY, OnChangeEditReply)
	ON_BN_CLICKED(IDC_CHECK_SENDRESULTS, OnCheckSendresults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefShare::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pDoc     = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;
	m_autPrefs = m_pDoc->m_autPrefs;
	m_autShare = m_pDoc->m_autShare;
	

	// Prevent reloading all files if no change, save time
	m_FilesSharedModified = false;

	// Setup listbox
	int offSet = m_lstShared.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstShared.GetWindowRect(&rect);

	m_lstShared.InsertColumn(0, "Directory", LVCFMT_LEFT,
		(rect.Width() - offSet) * 3/4, 0);
	m_lstShared.InsertColumn(1, "File Count", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/4, 1);

	m_lstShared.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Copy vector to alt loc list array
	VARIANT var = m_autShare->GetSharedDirIDs();
	SAFEARRAY* psa = var.parray;

	UINT* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	std::vector<UINT> DirIDs;
	for(int i = 0; i < psa->rgsabound->cElements; i++)
		 DirIDs.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);


	// Load up the listbox
	for(int i = 0; i < DirIDs.size(); i++)
	{
		int row = m_lstShared.GetItemCount();

		CString Directory = m_autShare->GetDirName( DirIDs[i] );

		if(m_autShare->GetDirRecursive( DirIDs[i] ))
			Directory += ", Recursive";

		m_lstShared.InsertItem(row, Directory);
		m_lstShared.SetItemText(row, 1, CommaIze(DWrdtoStr(m_autShare->GetDirFileCount( DirIDs[i] ))));
	}

	// Set the recuse check as default
	m_chkRecurse.SetCheck(true);

	if(m_autPrefs->GetReplyFilePath())
		m_chkPath.SetCheck(true);

	// Reply max
	if(m_autPrefs->GetMaxReplies())
	{
		m_chkReply.SetCheck(1);
		m_ebReply.SetWindowText( DWrdtoStr(m_autPrefs->GetMaxReplies()) );

	}
	else
		m_ebReply.EnableWindow(false);

	// Send results only when upload slots are available
	if(m_autPrefs->GetSendOnlyAvail())
		m_chkSendResults.SetCheck(true);

	return TRUE;
}

void CPrefShare::OnButtonAdd() 
{
	CFileFind finder;
	CString DirPath;

	// get updated string
	 m_ebShared.GetWindowText(DirPath);

	 if(DirPath.IsEmpty() )
	 {
		 // call browse routine
		 Browse();
		 m_ebShared.GetWindowText(DirPath);
	 }
	
	if(!DirPath.IsEmpty() )
	{
		// Simple check to see if the directory actually exists.  Will not catch entering a file name.
		SHFILEINFO psfi;
		SHGetFileInfo(DirPath, NULL, &psfi, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME);
		if(strlen(psfi.szDisplayName) )
		{
			// clear edit box,
			m_ebShared.SetWindowText("");
			
			DWORD FileCount = RecurseCount(DirPath);

			if(m_chkRecurse.GetCheck())
				DirPath += ", Recursive";

			int row = m_lstShared.GetItemCount();

			// Add the directory to the list box
			m_lstShared.InsertItem(row, DirPath);

			// Get the number of files inside the directory
			m_lstShared.SetItemText(row, 1, CommaIze(DWrdtoStr(FileCount)));

			SetModified();

			m_FilesSharedModified = true;
		}
		else
			MessageBox("Directory not found.", "Invalid Directory", MB_ICONEXCLAMATION | MB_OK);
	}
}

void CPrefShare::OnButtonRemove() 
{
	int      nItem;
	POSITION pos = m_lstShared.GetFirstSelectedItemPosition();

	while(pos)
	{
		pos = m_lstShared.GetFirstSelectedItemPosition();
		nItem = m_lstShared.GetNextSelectedItem(pos);

		m_lstShared.DeleteItem(nItem);

		m_FilesSharedModified = true;
	}
	
	SetModified();
}

void CPrefShare::Browse() 
{
// Get the directory to add from the user
	char *Directory = new char[255];

	LPBROWSEINFO Settings	 = new BROWSEINFO;
	Settings->hwndOwner		 = m_hWnd;
	Settings->pidlRoot		 = NULL;
	Settings->pszDisplayName = Directory;
	Settings->lpszTitle		 = "Choose a Directory to Share...";
	Settings->ulFlags		 = BIF_RETURNONLYFSDIRS;
	Settings->lpfn			 = NULL;
	Settings->lParam		 = NULL;
	Settings->iImage		 = NULL;

	LPCITEMIDLIST FolderID = SHBrowseForFolder(Settings);
	delete Settings;

	if(FolderID)
	{
		SHGetPathFromIDList(FolderID, Directory);
		m_ebShared.SetWindowText(Directory);
	}

	delete [] Directory;
}

void CPrefShare::OnChangeEditShared() 
{
	SetModified();
}

void CPrefShare::OnCheckPath() 
{
	m_FilesSharedModified = true;

	SetModified();	
}

void CPrefShare::OnCheckReplymax() 
{
	if(m_chkReply.GetCheck())
		m_ebReply.EnableWindow(true);
	else
		m_ebReply.EnableWindow(false);

	SetModified();
}

void CPrefShare::OnChangeEditReply() 
{
	
}

void CPrefShare::OnCheckSendresults() 
{
	SetModified();
}

BOOL CPrefShare::OnApply() 
{
	CString temp;
	
	if(m_chkPath.GetCheck())
		m_autPrefs->SetReplyFilePath(true);
	else
		m_autPrefs->SetReplyFilePath(false);

	// Rebuild the shared files list
	if(m_FilesSharedModified)
	{
		// Set shared dirs
		std::vector<CString> DirList;
		for(int i = 0; i < m_lstShared.GetItemCount(); i++)
			DirList.push_back(m_lstShared.GetItemText(i, 0));

		VARIANT vaDirs;
		VariantInit(&vaDirs);

		vaDirs.vt = VT_ARRAY | VT_BSTR;

		SAFEARRAYBOUND dBounds = {DirList.size(), 0};
		SAFEARRAY* psa = SafeArrayCreate(VT_BSTR, 1, &dBounds);

		BSTR* strArray;
		SafeArrayAccessData(psa, reinterpret_cast<void**> (&strArray));

		for(i = 0; i < DirList.size(); i++)
			strArray[i] = DirList[i].AllocSysString();

		SafeArrayUnaccessData(psa);
		vaDirs.parray = psa;

		m_autShare->SetSharedDirs(&vaDirs);

		VariantClear(&vaDirs);

		m_FilesSharedModified = false;
	}

	// Max Replies
	CString store;

	if(m_chkReply.GetCheck())
	{
		m_ebReply.GetWindowText(store);
		
		m_autPrefs->SetMaxReplies(atoi(store));
	}
	else
		m_autPrefs->SetMaxReplies(0);	

	// Send results only when upload slots are available
	if(m_chkSendResults.GetCheck())
		m_autPrefs->SetSendOnlyAvail(true);
	else
		m_autPrefs->SetSendOnlyAvail(false);

	return CPropertyPage::OnApply();
}

DWORD CPrefShare::RecurseCount(LPCTSTR pstr)
{
	CFileFind finder;

	DWORD FileCount = 0;

	// build a string with wildcards
	CString strWildcard(pstr);

	if(strWildcard.GetAt( strWildcard.GetLength() - 1) != '\\')
		strWildcard += _T("\\*");
	else
		strWildcard += _T("*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files
		if (finder.IsDots())
			continue;

		// if it's a directory, recursively search it
		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();

			if(m_chkRecurse.GetCheck())
				FileCount += RecurseCount(str);
		}
		else
			FileCount++;
	}

	finder.Close();

	return FileCount;
}




BOOL CPrefShare::OnWizardFinish()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnWizardFinish();
}
