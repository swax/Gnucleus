#if !defined(AFX_CHATSERVER_H__25344302_2067_11D5_ACF2_00A0CC533D52__INCLUDED_)
#define AFX_CHATSERVER_H__25344302_2067_11D5_ACF2_00A0CC533D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define COLOR_RED			0x000000FF
#define COLOR_ORANGE		0x000080FF
#define COLOR_GREEN			0x00009300
#define COLOR_BLUE			0x00800000
#define COLOR_PURPLE		0x009C009C
#define COLOR_BROWN			0x00000080
#define COLOR_BLACK			0x00000000

#define RPL_WELCOME			001
#define RPL_YOURHOST		002
#define RPL_CREATED			003
#define RPL_MYINFO			004
#define RPL_BOUNCE			005

#define RPL_STATSDLINE		250
#define RPL_LUSERCLIENT		251
#define RPL_LUSEROP			252
#define RPL_LUSERUNKNOWN	253
#define RPL_LUSERCHANNELS	254
#define RPL_LUSERME			255
#define RPL_ADMINME			256
#define RPL_ADMINLOC1		257
#define RPL_ADMINLOC2		258
#define RPL_ADMINEMAIL		259
#define RPL_TRYAGAIN		263
#define RPL_LOCALUSERS		265
#define RPL_GLOBALUSERS		266

#define RPL_AWAY			301
#define RPL_USERHOST		302
#define RPL_ISON			303
#define RPL_UNAWAY			305
#define RPL_NOWAWAY			306
#define RPL_WHOISUSER		311
#define RPL_WHOISSERVER		312
#define RPL_WHOISOPERATOR	313
#define RPL_WHOWASUSER		314
#define RPL_ENDOFWHO		315
#define RPL_WHOISIDLE		317
#define RPL_ENDOFWHOIS		318
#define RPL_WHOISCHANNELS	319
#define RPL_LISTSTART		321
#define RPL_LIST			322
#define RPL_LISTEND			323
#define RPL_CHANNELMODEIS	324
#define RPL_UNIQOPIS		325
#define RPL_NOTOPIC			331
#define RPL_TOPIC			332
#define RPL_INVITING		341
#define RPL_SUMMONING		342
#define RPL_INVITELIST		346
#define RPL_ENDOFINVITELIST	347
#define RPL_EXCEPTLIST		348
#define RPL_ENDOFEXCEPTLIST 349
#define RPL_VERSION			351
#define RPL_WHOREPLY		352
#define RPL_NAMREPLY		353
#define RPL_LINKS			364
#define RPL_ENDOFLINKS		365
#define RPL_ENDOFNAMES		366
#define RPL_BANLIST			367
#define RPL_ENDOFBANLIST	368
#define RPL_ENDOFWHOWAS		369
#define RPL_INFO			371
#define RPL_MOTD			372
#define RPL_ENDOFINFO		374
#define RPL_MOTDSTART		375
#define RPL_ENDOFMOTD		376
#define RPL_YOUREOPER		381
#define RPL_REHASHING		382
#define RPL_YOURESERVICE	383
#define RPL_TIME			391
#define RPL_USERSSTART		392
#define RPL_USERS			393
#define RPL_ENDOFUSERS		394
#define RPL_NOUSERS			395

#define ERR_NOSUCHNICK		401
#define ERR_NOSUCHSERVER	402
#define ERR_NOSUCHCHANNEL	403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS 405
#define ERR_WASNOSUCHNICK	406
#define ERR_TOOMANYTARGETS	407
#define ERR_NOSUCHSERVICE	408
#define ERR_NOORIGIN		409
#define ERR_NORECIPIENT		411
#define ERR_NOTEXTTOSEND	412
#define ERR_NOTOPLEVEL		413
#define ERR_WILDTOPLEVEL	414
#define ERR_BADMASK			415
#define ERR_UNKNOWNCOMMAND	421
#define ERR_NOMOTD			422
#define ERR_NOADMININFO		423
#define ERR_FILEERROR		424
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE	433
#define ERR_NICKCOLLISION	436
#define ERR_UNAVAILRESOURCE 437
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL	442
#define ERR_USERONCHANNEL	443
#define ERR_NOLOGIN			444
#define ERR_SUMMONDISABLED	445
#define ERR_USERSDISABLED	446
#define ERR_NOTREGISTERED	451
#define ERR_NEEDMOREPARAMS	461
#define ERR_ALREADYREGISTRED 462
#define ERR_NOPERMFORHOST	463
#define ERR_PASSWDMISMATCH	464
#define ERR_YOUREBANNEDCREEP 465
#define ERR_YOUWILLBEBANNED	466
#define ERR_KEYSET			467
#define ERR_CHANNELISFULL	471
#define ERR_UNKNOWNMODE		472
#define ERR_INVITEONLYCHAN	473
#define ERR_BANNEDFROMCHAN	474
#define ERR_BADCHANNELKEY	475
#define ERR_BADCHANMASK		476
#define ERR_NOCHANMODES		477
#define ERR_BANLISTFULL		478
#define ERR_NOPRIVILEGES	481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_CANTKILLSERVER	483
#define ERR_RESTRICTED		484
#define ERR_UNIQOPPRIVSNEEDED 485
#define ERR_NOOPERHOST		491

#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH	502


class CChatPrefs;
class CChatRoom;
class CChatControl;
class CAutCache;


class CChatServer : public CAsyncSocket
{
public:
	CChatServer(CChatControl*, CString, UINT);
	virtual ~CChatServer();

	void Receive_Welcome(CString, int);
	void Receive_ServerStats(CString, int);
	void Receive_ServerCount(CString, int);
	void Receive_MOTD(CString, int);
	
	void Receive_Ping(CString);
	void Receive_Whois(CString, int);
	void Receive_Nick(CString, CString);

	void Receive_PrivMsg(CString);
	void Receive_Join(CString, int);
	void Receive_Mode(CString, int);
	void Receive_Topic(CString, int);
	void Receive_Invite(CString, int);
	void Receive_Away(CString, int);
	void Receive_Kick(CString);
	void Receive_Part(CString);
	void Receive_Quit(CString);

	void Receive_Notice(CString);
	void Receive_Error(CString);
	void Receive_ErrorCode(CString, int);

	CChatRoom* GetChannel(CString);
	CChatRoom* GetPrivate(CString);

	CChatRoom* CreateRoom(CString);
	void	   RemoveRoom(CChatRoom*);

	void	EventHandler(CString, CString, CString, CChatRoom*, CString);
	void	AnalyzeAndSendCommand(CString Command, CChatRoom* pRoom, CString Who = "");
	CString ParseAliasesString(CStringArray&, CString);

	CString		m_Nick;
	CChatRoom*  m_Console; // Server console

	int m_CacheNum;

	std::vector<CChatRoom*> m_RoomList;    // Joined channels
	std::vector<CChatRoom*> m_PrivateList; // Private messages

	CString m_Host;
	UINT	m_Port;
	
	int		m_Status;
	int		m_SecsDead;
	bool	m_LoggedIn;

	int GnuUsers;

	CString m_Error;

	CChatControl* m_pChat;
	CChatPrefs*   m_pPrefs;
	CAutCache*	  m_autCache;


	//{{AFX_VIRTUAL(CChatServer)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnSend(int nErrorCode);
	virtual void Close(CString Error);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatServer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


protected:
	DWORD Breakup(byte*, DWORD);
	void  Process(CString);

	bool m_DataSending;

	DWORD m_dwExtraLength;
	byte  m_pBuff[4096];
	byte  m_pExtra[4096];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSERVER_H__25344302_2067_11D5_ACF2_00A0CC533D52__INCLUDED_)
