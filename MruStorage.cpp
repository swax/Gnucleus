// MruStorage.cpp
// Copyright © 2001 Scott Kirkwood
// 
// Permission to use, copy, modify, distribute and sell this code
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies or
// any derived copies.  Scott Kirkwood makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlbase.h>
#include "MruStorage.h"

// Took out some more dependancies by just putting the string here.
const char* k_strStoreHere = "Software\\Gnucleus";


CMruStorage::CMruStorage()
{
	m_nSize = 0;
}

CMruStorage::~CMruStorage()
{
	
}


void CMruStorage::SetSection(HWND hWnd, LPCTSTR lpszSection, LPCTSTR lpszEntryFormat, int nSize)
{
	if (lpszSection && *lpszSection)
	{
		m_strSection = lpszSection;
	}
	else
	{
		ASSERT(hWnd);

		LONG nId = ::GetWindowLong(GetParent(hWnd), GWL_ID);
		m_strSection.Format("DLG-%d", nId);

		// Get the name of the dialog
		::GetWindowText(GetParent(hWnd), m_strParentWindowName.GetBuffer(256), 255);
		m_strParentWindowName.ReleaseBuffer();
	}

	if (lpszEntryFormat && *lpszEntryFormat)
	{
		m_strEntryFormat = lpszEntryFormat;
	}
	else if (hWnd)
	{
		// Use the ctrl ID as a unique value to store the information
		m_strEntryFormat .Format("%d-%%d", ::GetDlgCtrlID(hWnd));
	}
	else
	{
		m_strEntryFormat = "File-%d";
	}

	if (m_strEntryFormat.Find("%d") == -1)
	{
		m_strEntryFormat += "-%d";
	}

	ASSERT(nSize > 0);
	m_nSize = nSize;
}


void CMruStorage::Add(LPCTSTR lpszItem)
{
	ASSERT(lpszItem != NULL);
	ASSERT(AfxIsValidString(lpszItem));

	// Let's not add blank strings
	if (NULL == lpszItem || '\0' == *lpszItem )
	{
		return;
	}

	POSITION pos = m_lstStrings.GetHeadPosition();

	bool bFound = false;

	// update the MRU list, if an existing MRU string matches file name
	while (pos)
	{
		if (strcmp(m_lstStrings.GetAt(pos), lpszItem) == 0)
		{
			bFound = true;
			break;      // iMRU will point to matching entry
		}

		m_lstStrings.GetNext(pos);
	}

	if (bFound && pos)
	{
		CString strHold = m_lstStrings.GetAt(pos);
		m_lstStrings.RemoveAt(pos);
		m_lstStrings.AddHead(strHold);
	}
	else
	{
		m_lstStrings.AddHead(lpszItem);
	}

	if (m_lstStrings.GetCount() > m_nSize)
	{
		m_lstStrings.RemoveAt(m_lstStrings.GetTailPosition());
	}
}


void CMruStorage::WriteList()
{
	ASSERT(!m_strSection.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());

	LPTSTR pszEntry = new TCHAR[m_strEntryFormat.GetLength() + 5];
	
	CString strHKeyString = k_strStoreHere;
	strHKeyString += "\\";
	strHKeyString += m_strSection;

	CRegKey aKey;
	DWORD dwNew = 0;

	if (ERROR_SUCCESS == aKey.Create(HKEY_CURRENT_USER, strHKeyString, 
		REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &dwNew))
	{
		if (dwNew == REG_CREATED_NEW_KEY)
		{
			// Let's save the DLG name as well to make it easier to recognize this
			// id when browsing the registry.
			aKey.SetValue(HKEY_CURRENT_USER, strHKeyString, m_strParentWindowName);
		}

		POSITION pos = m_lstStrings.GetHeadPosition();

		for (int iMRU = 1; pos && iMRU <= m_nSize; iMRU++)
		{
			wsprintf(pszEntry, m_strEntryFormat, iMRU);

			if (!m_lstStrings.GetAt(pos).IsEmpty())
			{
				VERIFY(ERROR_SUCCESS == aKey.SetValue(HKEY_CURRENT_USER, strHKeyString, m_lstStrings.GetAt(pos), pszEntry));
			}

			m_lstStrings.GetNext(pos);
		}
	}
	else
	{
		ASSERT(0); // Should never fail.
	}

	delete [] pszEntry;
}


void CMruStorage::ReadList()
{
	ASSERT(!m_strSection.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());

	m_lstStrings.RemoveAll();

	LPTSTR pszEntry = new TCHAR[m_strEntryFormat.GetLength() + 5];

	CString strHKeyString = k_strStoreHere;
	strHKeyString += _T("\\");
	strHKeyString += m_strSection;

	CRegKey aKey;
	if (ERROR_SUCCESS == aKey.Open(HKEY_CURRENT_USER, strHKeyString))
	{
		for (int iMRU = 0; iMRU < m_nSize; iMRU++)
		{
			wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);

			CString strWork;
			ULONG nSize = 255;
			
			#if _MSC_VER < 1300	
				LONG ret = aKey.QueryValue(strWork.GetBuffer(nSize + 1), pszEntry, &nSize);
				strWork.ReleaseBuffer();
			#else
				char buffer[256];
				LONG ret = aKey.QueryValue(pszEntry, NULL, buffer, &nSize);
				if(ret == ERROR_SUCCESS)
					strWork = CString(buffer, nSize);
			#endif
			
			if (ERROR_SUCCESS == ret)
			{
				m_lstStrings.AddTail(strWork);
			}
			else
			{
				break;
			}
		}
	}

	delete [] pszEntry;
}


void CMruStorage::ResetList() // Zap the registry entry
{
	ASSERT(!m_strSection.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());

	LPTSTR pszEntry = new TCHAR[m_strEntryFormat.GetLength() + 5];
	
	CString strHKeyString = k_strStoreHere;
	strHKeyString += "\\";
	strHKeyString += m_strSection;

	CRegKey aKey;
	DWORD dwNew = 0;

	if (ERROR_SUCCESS == aKey.Open(HKEY_CURRENT_USER, strHKeyString))
	{
		for (int iMRU = 0; iMRU < m_nSize; iMRU++)
		{
			wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);

			LONG ret = aKey.DeleteValue(pszEntry);
		}
	}

	m_lstStrings.RemoveAll();
	delete [] pszEntry;
}
