#pragma once


class CGnucleusDoc;
class CAutUpdate;

class CEvolverDlg : public CDialog
{
	DECLARE_DYNAMIC(CEvolverDlg)

public:
	CEvolverDlg(CWnd* pParent = NULL);
	virtual ~CEvolverDlg();

	void OnUpdateFound(CString Version);
	void OnUpdateFailed(CString Reason);
	void OnVersionCurrent();
	void OnUpdateComplete();

	void StartDownload();
	void Restart();


	int m_Counter;
	int m_NotifyTimer;
	int m_UpgradeTimer;
	int m_CloseTimer;

	CGnucleusDoc* m_pDoc;
	CAutUpdate*   m_autUpdate;

	enum { IDD = IDD_EVOLVE_MAIN };

	CStatic	m_stcStatus;
	CStatic	m_stcProgress;
	CStatic	m_stcLabel;
	CProgressCtrl	m_progBar;
	CButton	m_btnEvolve;
	CButton	m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonEvolve();
protected:
	virtual void PostNcDestroy();
};
