#if !defined(AFX_STATISTICSBANDWIDTH_H__D7008EA0_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_STATISTICSBANDWIDTH_H__D7008EA0_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewStatistics;
class CAutNetwork;

class CStatisticsBandwidth : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsBandwidth)

public:
	CStatisticsBandwidth(CViewStatistics* pView = NULL);
	~CStatisticsBandwidth();

	void UpdateBandwidth();


	//{{AFX_DATA(CStatisticsBandwidth)
	enum { IDD = IDD_STAT_BANDWIDTH };
	CListCtrl	m_lstBandwidth;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsBandwidth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewStatistics* m_pStatView;
	CAutNetwork*	 m_autNetwork;

	int	m_nTimerID;


	//{{AFX_MSG(CStatisticsBandwidth)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSBANDWIDTH_H__D7008EA0_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
