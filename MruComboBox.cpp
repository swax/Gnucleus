#include "stdafx.h"
#include <afxadv.h>
#include <io.h>
#include "MruComboBox.h"

IMPLEMENT_DYNCREATE(CMruComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CMruComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMruComboBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMruComboBox::CMruComboBox()
{
	m_pFilenameBuffer = new char [_MAX_PATH];
	memset(m_pFilenameBuffer, 0, _MAX_PATH);
	m_pFileDlg = new CFileDialog(TRUE);
	ASSERT(m_pFileDlg);

	// Just make sure that CFileDialog() is still setting this
	// properly.
    ASSERT(m_pFileDlg->m_ofn.lStructSize == sizeof(OPENFILENAME));
	m_pFileDlg->m_ofn.lpstrFile = m_pFilenameBuffer;
	m_pFileDlg->m_ofn.nMaxFile = _MAX_PATH;

	m_FileList.SetSection(NULL, "RecentFileList", "File-%d", 7);
	
	m_bUseLastByDefault = TRUE;
	m_bFindFiles = TRUE;
	m_pFilters = NULL;
}


CMruComboBox::~CMruComboBox()
{
	delete m_pFileDlg;
	delete [] m_pFilenameBuffer;
	delete [] m_pFilters;
}


int CMruComboBox::DoModalFileOpen()
{
	// If it hasn't been created yet, create default!
	ReadList();
	InitFromList();

	CString closestPath;
	GetWindowText(closestPath);

	int ret = 0;
	if (closestPath.GetLength())
	{
		// Try the path as is.
		struct _finddata_t fileinfo;
		long found = _findfirst(closestPath, &fileinfo);

		if (found == -1 || !(fileinfo.attrib & _A_SUBDIR))
		{
			// that didn't work.  Try stripping off to the last
			// slash (both forward and backwards slashes)
			int pos = closestPath.ReverseFind('\\');
			if (pos == -1)
				pos = closestPath.ReverseFind('/');

			if (pos != -1)
			{
				closestPath = closestPath.Left(pos);
				found = _findfirst(closestPath, &fileinfo);
			}
		}

		if (found != -1 && (fileinfo.attrib & _A_SUBDIR))
		{
			// We found a directory! Let's use it as the default
			// path.
			m_pFileDlg->m_ofn.lpstrInitialDir = closestPath;
		}
	}

	ret = m_pFileDlg->DoModal();

	if (ret == IDOK)
	{
		TRACE("Added %s\n", m_pFilenameBuffer);

		Add(m_pFilenameBuffer);
		InitFromList();

		//AddCurrentToList();
	}

	return ret;
}


void CMruComboBox::SetFlags(DWORD flags)
{
	m_pFileDlg->m_ofn.Flags = flags;
}


void CMruComboBox::SetCustFilter(const char* str)
{
	delete m_pFilters;
	m_pFilters = new char [strlen(str) + 1];

	strcpy(m_pFilters, str);
	char* p = m_pFilters;

	while (*p)
	{
		if (*p == '|')
		{
			*p = '\0';
		}

		p++;
	};
	m_pFileDlg->m_ofn.lpstrFilter = m_pFilters;
}


void CMruComboBox::Init(LPCTSTR strSection, LPCTSTR strFormatStr, int nSize)
{
	// Turn off sorting if it exists
	if ((::GetWindowLong(m_hWnd, GWL_STYLE) & CBS_SORT) != 0)
	{
		::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) & ~CBS_SORT);	
	}

	m_FileList.SetSection(m_hWnd, strSection, strFormatStr, nSize);

	ReadList();
	InitFromList();
}


void CMruComboBox::ReadList()
{
	m_FileList.ReadList();
}


void CMruComboBox::InitFromList()
{
	ResetContent();

	CMruStorageIter iter(m_FileList);

	for (; iter; ++iter)
	{
		CComboBox::AddString(iter.Get());
	}

	if (GetCount() && m_bUseLastByDefault)
	{
		VERIFY(CB_ERR != SetCurSel(0));
	}
}


void CMruComboBox::Set(const CString& str)
{
	Add(str);
	InitFromList();
	SetCurSel(0);
}


void CMruComboBox::AddCurrentToList()
{
	CString text;

	Get(text);
	// Add this item to the list if it's not blank
	Add(text);
	InitFromList();
}


void CMruComboBox::Add(const CString& Text)
{
	m_FileList.Add(Text);
}


void CMruComboBox::OnDestroy() 
{
	AddCurrentToList();

	m_FileList.WriteList();

	CComboBox::OnDestroy();
}

//void CMruComboBox::Remove(int remove)
//{
//	m_FileList.Remove(remove);
//	VERIFY(CB_ERR != DeleteString(remove)); 
//}


bool CMruComboBox::IsEmpty()
{
	if (m_FileList.GetSize())
	{
		return false;
	}
	
	return true;
}


void CMruComboBox::Get(OUT CString& strRet) const
{
	int nIndex = GetCurSel();

	if (nIndex != -1)
	{
		GetLBText(nIndex, strRet);
	}
	else
	{
		GetWindowText(strRet.GetBuffer(255), 254);
		strRet.ReleaseBuffer();
	}
}



void CMruComboBox::ResetList()
{
	m_FileList.ResetList();
	
	ResetContent();
}
