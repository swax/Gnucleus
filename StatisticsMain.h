#if !defined(AFX_STATISTICSMAIN_H__65D1A926_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_STATISTICSMAIN_H__65D1A926_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewStatistics;


class CStatisticsMain : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsMain)

public:
	CStatisticsMain(CViewStatistics* pView = NULL);
	~CStatisticsMain();

	void UpdateSummary();

	CString m_Sharing;
	CString m_Efficiency;
	CString m_Bandwidth;
	CString m_Memory;


	//{{AFX_DATA(CStatisticsMain)
	enum { IDD = IDD_STAT_MAIN };
	CStatic	m_stcVersion;
	CListCtrl	m_lstSummary;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewStatistics*	m_pStatView;
	
	
	//{{AFX_MSG(CStatisticsMain)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSMAIN_H__65D1A926_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
