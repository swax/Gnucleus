#if !defined(AFX_VIEWSTATISTICS_H__65D1A921_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_VIEWSTATISTICS_H__65D1A921_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define PACKETCACHE_SIZE 1000

class CGnucleusDoc;
class CAutNetwork;

class CStatisticsMain;
class CStatisticsDistribution;
class CStatisticsPackets;
class CStatisticsBandwidth;
class CStatisticsLog;
class CStatisticsMemory;
class CStatisticsError;


class CViewStatistics : public CFormView
{
protected:
	CViewStatistics();           
	DECLARE_DYNCREATE(CViewStatistics)
	virtual ~CViewStatistics();

public:
	void SelectNode(int);

	void OnSockUpdate();
	void OnPacketIncoming(int NodeID, packet_Header* packet, int size, int ErrorCode, bool Local);
	void OnPacketOutgoing(int NodeID, packet_Header* packet, int size, bool Local);
		
	CPropertySheet		m_pSheet;

	CStatisticsMain*		 m_TabMain;
	CStatisticsDistribution* m_TabDistribution;
	CStatisticsPackets*		 m_TabPackets;
	CStatisticsBandwidth*	 m_TabBandwidth;
	CStatisticsLog*			 m_TabLog;
	CStatisticsMemory*		 m_TabMemory;
	CStatisticsError*		 m_TabError;

	bool	  m_Paused;
	
	CGnucleusDoc* m_pDoc;
	CAutNetwork*  m_autNetwork;

	//{{AFX_DATA(CViewStatistics)
	enum { IDD = IDD_VIEW_STATISTICS };
	CButton	m_btnPauseGo;
	CStatic	m_tabSheet;
	CListCtrl	m_lstNodes;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CViewStatistics)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	void ExtendedInfo(int);
	
	bool SockSelected(int);
	BOOL CreatePropSheet(CWnd*, CPropertySheet*);
	void SizePropSheet(CWnd*, CPropertySheet*, UINT);
	void SizePropSheet(CWnd*, CPropertySheet*, CRect);

	CGnucleusDoc* GetDocument();

	CTime m_TimeStarted;


	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif


	//{{AFX_MSG(CViewStatistics)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonPauseGo();
	afx_msg void OnItemchangedListNodes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListNodes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListNodes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTATISTICS_H__65D1A921_8F03_11D4_ACF2_00A0CC533D52__INCLUDED_)
