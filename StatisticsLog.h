#if !defined(AFX_STATISTICSLOG_H__65D1A924_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_STATISTICSLOG_H__65D1A924_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DlgResizer.h"
#include "afxwin.h"


class CStatisticsLog : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticsLog)

public:
	CStatisticsLog();
	~CStatisticsLog();

	void PacketIncoming(packet_Header*);
	void PacketGood(packet_Header*);
	void PacketBad(packet_Header*, int);

	void PacketOutgoing(packet_Header*);
	void PacketOutgoingLocal(packet_Header*);

	void CleanBuffer();


	//{{AFX_DATA(CStatisticsLog)
	enum { IDD = IDD_STAT_LOG };
	CButton	m_btnFlush;
	CButton	m_chkQueryHits;
	CButton	m_chkBad;
	CButton	m_chkGood;
	CStatic	m_stBuffSize;
	CListCtrl	m_lstLog;
	CEdit	m_ebBuffSize;
	CButton	m_chkQueries;
	CButton	m_chkPushes;
	CButton	m_chkPongs;
	CButton	m_chkPings;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStatisticsLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	CImageList m_LogIcons;
	CDlgResizer m_DlgResizer;

	bool PacketChecked(packet_Header*);
	CString PacketName(packet_Header*);
	int		PacketIcon(packet_Header*);
	CString PacketExtra(packet_Header*);
	CString ErrorName(DWORD);
	CString GetGuid(packet_Header*);
	
	
	//{{AFX_MSG(CStatisticsLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonFlush();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_chkIn;
	CButton m_chkOut;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSLOG_H__65D1A924_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
