/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2004 John Marshall

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	For support, questions, comments, etc...
	E-Mail:
		swabby@c0re.net

********************************************************************************/


#include "StdAfx.h"
#include "prefsex.h"

CPrefsEx::CPrefsEx(void)
{
	m_AutoConnect	= true;

	m_TrayOnMin			= true;
	m_TrayOnClose		= false;

	// Scheduler
	m_SchedActive		= false;
	m_SchedConnect		= CTime(2000, 1, 1, 22, 0, 0);
	m_SchedDisconnect	= CTime(2000, 1, 1, 8, 0, 0);
}

CPrefsEx::~CPrefsEx(void)
{
}

void CPrefsEx::LoadPrefsEx(CString ConfigFile)
{
	char buffer[256];

	// Network
	GetPrivateProfileString("Local", "AutoConnect", "1", buffer, 256, ConfigFile);
	m_AutoConnect = (0 != atoi(buffer));

	GetPrivateProfileString("Local", "ConnectGnutella", "1", buffer, 256, ConfigFile);
	m_ConnectGnutella = (0 != atoi(buffer));

	GetPrivateProfileString("Local", "ConnectG2", "1", buffer, 256, ConfigFile);
	m_ConnectG2 = (0 != atoi(buffer));

	GetPrivateProfileString("Local", "ForceGnuUltra", "0", buffer, 256, ConfigFile);
	m_ForceGnuUltra = (0 != atoi(buffer));

	GetPrivateProfileString("Local", "ForceG2Hub", "0", buffer, 256, ConfigFile);
	m_ForceG2Hub = (0 != atoi(buffer));

	// Client
	GetPrivateProfileString("Local", "TrayOnMin", "1", buffer, 256, ConfigFile);
	m_TrayOnMin = (0 != atoi(buffer));
	GetPrivateProfileString("Local", "TrayOnClose", "0", buffer, 256, ConfigFile);
	m_TrayOnClose = (0 != atoi(buffer));


	// Scheduler
	int hour = 0, minute = 0;
	GetPrivateProfileString("Scheduler",	"Active",			"0",	buffer, 256, ConfigFile);
	m_SchedActive = (0 != atoi(buffer));

	GetPrivateProfileString("Scheduler",  "Connect",		"22:00",	buffer, 256, ConfigFile);
	sscanf(buffer, "%d:%d", &hour, &minute);
	m_SchedConnect		= CTime(2000, 1, 1, hour, minute, 0);

	GetPrivateProfileString("Scheduler",  "Disconnect",		"8:00",		buffer, 256, ConfigFile);
	sscanf(buffer, "%d:%d", &hour, &minute);
	m_SchedDisconnect	= CTime(2000, 1, 1, hour, minute, 0);
}

void CPrefsEx::SavePrefsEx(CString ConfigFile)
{
	WritePrivateProfileString("Local", "AutoConnect",		  DWrdtoStr(m_AutoConnect),		ConfigFile);

	WritePrivateProfileString("Local", "ConnectGnutella", DWrdtoStr(m_ConnectGnutella),	ConfigFile);
	WritePrivateProfileString("Local", "ConnectG2",		  DWrdtoStr(m_ConnectG2),		ConfigFile);
	WritePrivateProfileString("Local", "ForceGnuUltra",	  DWrdtoStr(m_ForceGnuUltra),	ConfigFile);
	WritePrivateProfileString("Local", "ForceG2Hub",      DWrdtoStr(m_ForceG2Hub),		ConfigFile);

	WritePrivateProfileString("Local", "TrayOnMin",		  DWrdtoStr(m_TrayOnMin),			ConfigFile);
	WritePrivateProfileString("Local", "TrayOnClose",     DWrdtoStr(m_TrayOnClose),			ConfigFile);


	// Scheduler
	WritePrivateProfileString("Scheduler",		"Active",			DWrdtoStr(m_SchedActive),		ConfigFile);
	WritePrivateProfileString("Scheduler",		"Connect",			DWrdtoStr(m_SchedConnect.GetHour())    + ":" + DWrdtoStr(m_SchedConnect.GetMinute()),		ConfigFile);
	WritePrivateProfileString("Scheduler",		"Disconnect",		DWrdtoStr(m_SchedDisconnect.GetHour()) + ":" + DWrdtoStr(m_SchedDisconnect.GetMinute()),	ConfigFile);
}