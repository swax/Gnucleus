#ifndef __MRUSTORAGE_H__
#define __MRUSTORAGE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMruStorage
{
public:
	CMruStorage();
	virtual ~CMruStorage();

	// Initialize
	void SetSection(HWND hWnd, LPCTSTR lpszSection = NULL, LPCTSTR lpszEntryFormat = NULL, int nSize = NULL);
	virtual void Add( LPCTSTR lpszPathName );
	CStringList& GetList() { return m_lstStrings; }
	int GetSize() const { return m_lstStrings.GetCount(); }
	void WriteList();
	void ReadList();
	void ResetList(); // Zap the registry entry
	CString& GetHead() { return m_lstStrings.GetHead(); }
private:
	CStringList m_lstStrings;
	CString m_strSection;
	CString m_strEntryFormat;
	CString m_strParentWindowName;
	int m_nSize;
};


class CMruStorageIter
{
public:
	CMruStorageIter(CMruStorage& init)
		: m_StringList(init.GetList())
	{
		m_Pos = m_StringList.GetHeadPosition();
	}
	operator bool() const { return m_Pos != NULL; }
	CMruStorageIter& operator++() 
	{
		m_StringList.GetNext(m_Pos);
		return *this;
	}

	CString& Get() { return m_StringList.GetAt(m_Pos); } 

private:
	CStringList& m_StringList;
	POSITION m_Pos;
};

#endif //__MRUSTORAGE_H__