#pragma once

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
