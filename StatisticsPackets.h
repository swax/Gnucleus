#if !defined(AFX_STATISTICSPACKETS_H__D7008EA3_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_STATISTICSPACKETS_H__D7008EA3_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewStatistics;
class CAutNetwork;

class CStatisticsPackets : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsPackets)

public:
	CStatisticsPackets(CViewStatistics* pView = NULL);
	~CStatisticsPackets();

	void UpdateData();
	

	//{{AFX_DATA(CStatisticsPackets)
	enum { IDD = IDD_STAT_PACKETS };
	CListCtrl	m_lstData;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsPackets)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewStatistics*	m_pStatView;
	CAutNetwork*		m_autNetwork;


	//{{AFX_MSG(CStatisticsPackets)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSPACKETS_H__D7008EA3_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
