#if !defined(AFX_STATISTICSERROR_H__2CA364EE_0686_4F48_B668_E5DE9F19CC0A__INCLUDED_)
#define AFX_STATISTICSERROR_H__2CA364EE_0686_4F48_B668_E5DE9F19CC0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"


class CViewStatistics;


class CStatisticsError : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsError)

public:
	CStatisticsError(CViewStatistics* pView = NULL);
	~CStatisticsError();

	//void NewError(ErrorInfo*);


	//{{AFX_DATA(CStatisticsError)
	enum { IDD = IDD_STAT_ERROR };
	CListCtrl	m_lstError;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsError)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	CViewStatistics*	m_pStatView;

	CDlgResizer m_DlgResizer;


	//{{AFX_MSG(CStatisticsError)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSERROR_H__2CA364EE_0686_4F48_B668_E5DE9F19CC0A__INCLUDED_)
