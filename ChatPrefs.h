#pragma once

class CChatControl;

class CChatPrefs
{
public:
	CChatPrefs(CChatControl* pChat);
	~CChatPrefs(void);

	void LoadServers(CString);
	void SaveServers(CString);

	void AddServer(CString, CString);

	void LoadChatAliases(CString);
	void SaveChatAliases(CString);

	void LoadChatScripts(CString);
	void SaveChatScripts(CString);

	void LoadChatHotlist(CString);
	void SaveChatHotlist(CString);

	CString GetNick();


	// Chat
	bool	m_InternalIRC;
	CString m_InternalIRCAddr;

	CString m_Nick;
	CString m_AltNick;
	bool	m_DisableIdent;

	CString m_PrefNet;
	CString m_PrefChannel;
	std::vector<ChatNetwork> m_ChatNets;

	CChatControl*  m_pChat;
};
