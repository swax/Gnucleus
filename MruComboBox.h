#ifndef _CMRUCOMBOBOX_H
#define _CMRUCOMBOBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MruStorage.h"


//@doc
//@class The CMruComboBox is a MFC derived CComboBox
// which keeps track of the last N files that the user has
// typed in.  Can be used in conjuction with a "Browse..."
// button by calling the DoModalFileOpen() function.
class CFileDialog;
class CRecentFileList;


class CMruComboBox : public CComboBox
{
	DECLARE_DYNCREATE(CMruComboBox)
public:
	CMruComboBox();
	virtual ~CMruComboBox();
	CFileDialog* GetFileDlg() const
		{ return m_pFileDlg; }
	int DoModalFileOpen();
	// Your str can't go out of scope.
	void SetCustFilter(const char* str);
	void SetFlags(DWORD flags);
	void SetUseLastByDefault(bool yup)
		{ m_bUseLastByDefault = yup; }
	void Init(LPCTSTR strSection = NULL, LPCTSTR strFormatStr = NULL, int size = 7);
	void AddCurrentToList();
	void ReadList(); // Read list from registry and set contents of combo box
	void InitFromList();
	void ResetList();

	void Set(const CString& text);

//	void Remove(int remove);

	// Was GetCurrent()
	void Get(OUT CString& strCurrent) const;

	bool IsEmpty();

	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMruComboBox)
	protected:
	//}}AFX_VIRTUAL
protected:
	virtual void Add(const CString& Text);  

	//{{AFX_MSG(CMruComboBox)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CFileDialog* m_pFileDlg;
	CMruStorage m_FileList;
	char* m_pFilenameBuffer;
	char* m_pFilters;
	bool m_bUseLastByDefault;
	bool m_bFindFiles;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif

