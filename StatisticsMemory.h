#if !defined(AFX_STATISTICSMEMORY_H__D7008EA2_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_STATISTICSMEMORY_H__D7008EA2_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnucleusDoc;
class CViewStatistics;


class CStatisticsMemory : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsMemory)

public:
	CStatisticsMemory(CViewStatistics* pView = NULL);
	~CStatisticsMemory();


	//{{AFX_DATA(CStatisticsMemory)
	enum { IDD = IDD_STAT_MEMORY };
	CStatic	m_stcTemp9;
	CStatic	m_stcTemp8;
	CStatic	m_stcTemp7;
	CStatic	m_stcTemp6;
	CStatic	m_stcTemp5;
	CStatic	m_stcTemp4;
	CStatic	m_stcTemp3;
	CStatic	m_stcTemp2;
	CStatic	m_stcTemp1;
	CListCtrl	m_lstMemory;
	CEdit	m_ebHash;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsMemory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewStatistics*	m_pStatView;
	CGnucleusDoc*		m_pDoc;

	int m_nTimerID;

	CString InsertDec(double dNumber, int place);

	//{{AFX_MSG(CStatisticsMemory)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSMEMORY_H__D7008EA2_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
