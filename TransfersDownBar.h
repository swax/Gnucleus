#if !defined(AFX_TRANSFERSDOWNBAR_H__8335B72D_3497_4006_9917_8B51C537BC02__INCLUDED_)
#define AFX_TRANSFERSDOWNBAR_H__8335B72D_3497_4006_9917_8B51C537BC02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGnuDownloadShell;
class CAutDownload;

class CTransfersDownBar : public CStatic
{
public:
	CTransfersDownBar();
	virtual ~CTransfersDownBar();

	int m_DownloadID;

	CGnuDownloadShell* Download;
	CAutDownload*      m_autDownload;


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransfersDownBar)
	//}}AFX_VIRTUAL




protected:


	//{{AFX_MSG(CTransfersDownBar)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERSDOWNBAR_H__8335B72D_3497_4006_9917_8B51C537BC02__INCLUDED_)
