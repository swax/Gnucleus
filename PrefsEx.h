#pragma once

class CPrefsEx
{
public:
	CPrefsEx(void);
	~CPrefsEx(void);

	void LoadPrefsEx(CString);
	void SavePrefsEx(CString);

	// Network
	bool m_AutoConnect;
	
	bool m_ConnectGnutella;
	bool m_ConnectG2;
	
	bool m_ForceGnuUltra;
	bool m_ForceG2Hub;

	// Tray
	bool  m_TrayOnMin;			
	bool  m_TrayOnClose;

	// Scheduler
	bool  m_SchedActive;
	CTime m_SchedConnect;
	CTime m_SchedDisconnect;
};
