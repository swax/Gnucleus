#pragma once


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "DlgResizer.h"


class CGnucleusDoc;
class CChatControl;
class CChatRoom;


class CViewChat : public CFormView
{
protected:
	CViewChat();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewChat)
	virtual ~CViewChat();

public:
	//{{AFX_DATA(CViewChat)
	enum { IDD = IDD_VIEW_CHAT };
	CButton	m_btnHelp;
	CListCtrl	m_lstUsers;
	CStatic	m_stcConnect;
	CStatic	m_stcChannel;
	CStatic	m_stcUser;
	CTreeCtrl	m_treeConnections;
	CRichEditCtrl	m_rebChat;
	CEdit	m_ebInput;
	CButton	m_btnConfigure;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CViewChat)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	void RelayCommand();
	bool ClientCommand(CString);
	void UpdateChat();
	void UpdateUsers(CString*);
	void UpdateTopic(CChatRoom*);
	void LoadTree();
	CChatRoom* GetTreeData(HTREEITEM);

	CString GetUserFromList(int);
	void ScrollChattoBottom();

	std::vector<CString> m_BackLog;
	int					 m_BackLogPos;

	CGnucleusDoc* m_pDoc;
	CChatControl* m_pChat;

	CToolBar   m_wndToolBar;

	CDlgResizer m_DlgResizer;

	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

	//{{AFX_MSG(CViewChat)
	afx_msg void OnDestroy();
	afx_msg LRESULT OnChatUpdate(WPARAM, LPARAM);
	afx_msg LRESULT OnChatSelect(WPARAM, LPARAM);
	afx_msg LRESULT OnChatUser(WPARAM, LPARAM);
	afx_msg LRESULT OnChatTopic(WPARAM, LPARAM);
	afx_msg LRESULT OnChatNotice(WPARAM, LPARAM);
	afx_msg void OnSelchangedTreeConnects(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonConfigure();
	afx_msg void OnRclickTreeConnects(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonHelp();
	afx_msg void OnDblclkListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChatConnect();
	afx_msg void OnChatDisconnect();
	afx_msg void OnChatHelp();
	afx_msg void OnChatPrefs();
	afx_msg void OnUpdateChatDisconnect(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


static DWORD CALLBACK ChatStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

