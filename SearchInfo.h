#if !defined(AFX_SEARCHINFO_H__5FD1ED01_155E_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_SEARCHINFO_H__5FD1ED01_155E_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"


struct ResultHost
{
	int HostID;

	IP  Host;
	int Port;
	int Speed;
	int Distance;

	bool Firewall;
	bool Stable;
	bool Busy;
	
	CString Vendor;
	CString strInfo;
	
	FullIcon Icon;
};


class CGnucleusDoc;
class CSearchResults;
class CAutSearch;

class CSearchInfo : public CDialog
{

public:
	CSearchInfo(CWnd* pParent, CSearchResults* pView, UINT ResultID);   // standard constructor

	void UpdateInfo();

	int  m_SearchID;
	UINT m_ResultID;

	CDlgResizer m_DlgResizer;

	int m_SearchExSortColumn, m_SearchExSortOrder;
	friend struct SearchExOrder;

	std::vector<ResultHost*>    m_HostList;
	std::map<UINT, ResultHost*> m_HostMap;


	//{{AFX_DATA(CSearchInfo)
	enum { IDD = IDD_SEARCH_INFO };
	CStatic	m_stcSha1Hash;
	CStatic	m_stcBitprintHash;
	CStatic	m_stcHosts;
	CStatic	m_stcSpeed;
	CStatic	m_stcSize;
	CStatic	m_stcName;
	CListCtrl	m_lstResults;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSearchInfo)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CSearchResults* m_pView;
	CGnucleusDoc*   m_pDoc;
	CAutSearch*     m_autSearch;

	
	
	//{{AFX_MSG(CSearchInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOk();
//	afx_msg void OnClose();
	afx_msg void OnColumnclickListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListResults(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHINFO_H__5FD1ED01_155E_11D5_ACF2_00A0CC533D52__INCLUDED_)
