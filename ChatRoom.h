#if !defined(AFX_CHATROOM_H__E84CB1C1_21E7_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_CHATROOM_H__E84CB1C1_21E7_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChatControl;
class CChatServer;


class CChatRoom  
{
public:
	CChatRoom(CChatServer *pServer);
	virtual ~CChatRoom();

	void WriteToDisplay(CString, COLORREF);
	void WriteToDisplay(ChatLine);

	int  FindUser(CString);

	void AddUser(CString);
	void RemoveUser(CString);
	void RenameUser(CString, CString);

	void Say(CString);
	void ServerCommand(CString);	
	
	CString m_Name;
	int		m_RoomType;
	CString m_Topic;

	std::list<ChatLine> m_Display;
	ChatLine			m_Pending;

	bool m_UserLock;
	std::vector<CString> m_UserList;

	CChatControl*	m_pChat;
	CChatServer*	m_pServer;
};

#endif // !defined(AFX_CHATROOM_H__E84CB1C1_21E7_11D5_ACF2_00A0CC533D52__INCLUDED_)
