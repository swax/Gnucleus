#if !defined(AFX_STATISTICSDISTRIBUTION_H__D7008EA1_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_STATISTICSDISTRIBUTION_H__D7008EA1_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewStatistics;


class CStatisticsDistribution : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsDistribution)

public:
	CStatisticsDistribution(CViewStatistics* pView = NULL);
	~CStatisticsDistribution();

	void UpdateDistribution();
	void PacketIncoming(packet_Header*);


	//{{AFX_DATA(CStatisticsDistribution)
	enum { IDD = IDD_STAT_DISTRIBUTION };
	CStatic	m_stcShare;
	CListCtrl	m_lstDist;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsDistribution)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewStatistics*	m_pStatView;

	DWORD				m_colFriends[MAX_TTL];
	DWORD				m_colLibrary[MAX_TTL];
	DWORD				dwTotalFriends;
	DWORD				dwTotalLibrary;
	DWORD				dwTotalSharing;


	//{{AFX_MSG(CStatisticsDistribution)
	afx_msg void OnButtonRefresh();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSDISTRIBUTION_H__D7008EA1_0E78_11D5_ACF2_00A0CC533D52__INCLUDED_)
