#if !defined(AFX_VIEWSEARCH_H__6B412200_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_VIEWSEARCH_H__6B412200_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

struct SearchResult
{
	UINT ResultID;

	CString Name;
	CString NameLower;

	CString Sha1Hash;
	uint64 Size;
	int AvgSpeed;
	int HostCount;

	int  DownloadID;
	FullIcon Icon;
	bool SyncNeeded;
	int  State;
};

class CGnucleusDoc;
class CSearchResults;
class CSearchAdvanced;
class CAutSearch;


class CViewSearch : public CFormView
{
protected:
	CViewSearch();           // protected constructor used by dynamic creation
	virtual ~CViewSearch();
	DECLARE_DYNCREATE(CViewSearch)


public:
	void SendQuery();
	void UpdateTitle();

	void OnUpdate(UINT ResultID);
	void OnResult(UINT ResultID);
	void OnRefresh();
	void OnBrowseUpdate(int State, int Progress);
	void OnProgress();
	void OnPaused();

	SearchResult* AddResult(UINT ResultID);


	CString m_Search;
	int     m_SearchID;

	bool m_SearchPaused;
	
	std::list<SearchResult> m_ResultList;
	std::map<UINT, SearchResult*> m_ResultMap;
	
	bool m_Browsing;
	CString m_BrowseStatus;

	CAutSearch*   m_autSearch;
	CGnucleusDoc* m_pDoc;
	
	CPropertySheet	 m_pSheet;
	CSearchResults*	 m_tabResults;
	CSearchAdvanced* m_tabAdvanced;
	

	//////////////////////////////////////////
	

	bool m_OverflowTriggered;
	
	

	//{{AFX_DATA(CViewSearch)
	enum { IDD = IDD_VIEW_SEARCH };
	CStatic	m_tabSearch;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CViewSearch)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL


protected:
	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif


	CImageList m_ImageLst;

	BOOL CreatePropSheet(CWnd*, CPropertySheet*);
	void SizePropSheet(CWnd*, CPropertySheet*, UINT);
	void SizePropSheet(CWnd*, CPropertySheet*, CRect);

	CGnucleusDoc* GetDocument();


	//{{AFX_MSG(CViewSearch)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPreferencesChange();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSEARCH_H__6B412200_DB34_11D4_ACF2_00A0CC533D52__INCLUDED_)
