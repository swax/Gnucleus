#if !defined(AFX_SEARCHRESULTS_H__6B412205_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_SEARCHRESULTS_H__6B412205_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"
#include "SearchResultsCtrl.h"


class CGnucleusDoc;
class CAutSearch;
class CViewSearch;
class CSearchInfo;

class CSearchResults : public CPropertyPage
{
	DECLARE_DYNCREATE(CSearchResults)
public:
	CSearchResults();
	CSearchResults(CViewSearch*);
	~CSearchResults();

	void UpdateList(SearchResult* Result);
	void UpdateTotals();

	afx_msg void OnChecksScreen();
	void OnRefresh();
	void ExtendedInfo(SearchResult*);

	void CancelDownload();

	
	CGnucleusDoc*	m_pDoc;
	CAutSearch*     m_autSearch;

	CViewSearch*    m_pView;
	CSearchInfo *   m_InfoDlg;

	CDlgResizer m_DlgResizer;


	//{{AFX_DATA(CSearchResults)
	enum { IDD = IDD_SEARCH_RESULTS };
	CButton	m_btnStopSearch;
	CButton	m_chkScreen;
	CEdit	m_ebRefine;
	CButton	m_btnConfig;
	CSearchResultsCtrl	m_lstResults;
	CButton	m_btnDownload;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSearchResults)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:	
	int  GetFilterMode(CString);

	bool m_status_update_needed;


	//{{AFX_MSG(CSearchResults)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDownload();
	afx_msg void OnButtonConfig();
	afx_msg void OnChangeEditRefine();
	afx_msg void OnDblclkListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStopsearch();
	afx_msg void OnKeydownListResults(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonViewTransfers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHRESULTS_H__6B412205_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
