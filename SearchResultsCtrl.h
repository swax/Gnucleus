#if !defined(AFX_SEARCHRESULTSCTRL_H__DF5200B9_0DFC_4ED7_8397_5810E6799BE8__INCLUDED_)
#define AFX_SEARCHRESULTSCTRL_H__DF5200B9_0DFC_4ED7_8397_5810E6799BE8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ListCtrlEx.h"
#include "ViewSearch.h"


class CGnucleusDoc;
class CViewSearch;
class CAutSearch;

class CSearchResultsCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSearchResultsCtrl)

public:
	CSearchResultsCtrl();
	virtual ~CSearchResultsCtrl();

	void MyInit(CViewSearch* pView);
	void SetItemState(int iItem, UINT uiState, UINT uiMask, bool bChangeData = true);

	// m_data operations
	void Insert(SearchResult* Result);
	void Insert(std::list<SearchResult> &ResultList);
	void ClearList();
	
	SearchResult* GetItem(int idx);

	// updates the actual gui
	void SyncList();

	bool m_SyncTotals;

	typedef CColumnClickMixIn<CSearchResultsCtrl, SearchResult*> MixInType;
	enum
	{
		eNameCol,
		eSizeCol,
		eTypeCol,
		eSpeedCol,
		eDistCol,

		eHashCol,
		eInfoCol,
	};

	static int SortBy;

	typedef std::map<SearchResult*, char> mapGroup;
	mapGroup mapSel; //Makslane: store selected itens

	typedef std::map<int, SearchResult*> mapIntGroup;
	mapIntGroup mapVisible; //Makslane: store visible itens

	//static int CALLBACK Sort(ResultGroup* pInfo1, ResultGroup* pInfo2, CSearchResultsCtrl* pThis);
	
	
	//{{AFX_VIRTUAL(CSearchResultsCtrl)
	public:
	protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	

private:
	typedef std::deque<SearchResult*> dataset;

	/*friend struct node_lt;
	
	struct node_lt : 
		public std::binary_function<UINT, UINT, bool>			
		{
			bool operator () (UINT x, UINT y)
			{
				return x < y;
			}
		};*/

	dataset m_data;
	dataset m_lastview;
	bool m_sync_needed;
	
	bool bChangeData; //Makslane: for safe item state modification
	
protected:
	// Figure out where the file should go.
	//int InsertWhere(ResultGroup* pGroup, int nCurListCount, bool bReverse);


	CViewSearch*    m_pView;
	CGnucleusDoc*	m_pDoc;
	CAutSearch*     m_autSearch;

	CCriticalSection m_ListBoxCrit;
	
	
	//{{AFX_MSG(CSearchResultsCtrl)
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdstatechanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

bool ResultComp( SearchResult*, SearchResult*);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHRESULTSCTRL_H__DF5200B9_0DFC_4ED7_8397_5810E6799BE8__INCLUDED_)
