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

	// Tray
	bool  m_TrayOnMin;			
	bool  m_TrayOnClose;

	// Scheduler
	bool  m_SchedActive;
	CTime m_SchedConnect;
	CTime m_SchedDisconnect;
};
