#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CGnucleusDoc;
class CFrameMain;
class CAutMeta;
class CAutSearch;

class CSearchMeta : public CDialog
{
	DECLARE_DYNAMIC(CSearchMeta)

public:
	CSearchMeta(CWnd* pParent = NULL, CFrameMain* pFrame = NULL);   // standard constructor
	virtual ~CSearchMeta();

	int m_MetaID;

	CGnucleusDoc* m_pDoc;
	CFrameMain*   m_pFrame;
	CAutMeta*	  m_autMeta;
	CAutSearch*   m_autSearch;

// Dialog Data
	enum { IDD = IDD_SEARCH_META };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CComboBox m_cmbType;
	CListCtrl m_lstAttributes;
	CEdit m_ebQuery;
	afx_msg void OnCbnSelchangeComboMeta();
	afx_msg void OnLvnItemchangedListAttributes(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cmbSpecify;
	afx_msg void OnCbnEditchangeComboSpecify();
	afx_msg void OnNMClickListAttributes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboSpecify();
};
