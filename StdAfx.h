#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif


#pragma warning(disable:4786)
#pragma warning(disable:4018) // Disable un-signed/signed comparison warnings
#pragma warning(disable:4244) // Disable type conversion warnings


// MFC
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


// Common Controls
#include <afxdtctl.h>			// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				// MFC support for Windows Common Controls
#endif		


#include <afxtempl.h>
#include <afxmt.h>			// MFC Threading
#include <afxinet.h>		// MFC Internet
#include <afxsock.h>		// MFC Sockets
#include <afxhtml.h>		// HTML Control
#include <comdef.h>			// NEED FOR COM!!
#include <process.h>


// STL includes
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <map>
#include <deque>


// Custom includes
#include "Conversions.h"
#include "Packet.h"
#include "Common.h"
#include "FileTypes.h"


// For better debugging
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


// Master Version
#define GNUCLEUS_VERSION    "1.9.9.4"
#define BUILD_CORE_VERSION  "0.9.0.5"

// Threading
#define CPU_0	0x0001
#define CPU_1	0x0002
#define CPU_2	0x0004 
#define CPU_3	0x0008


// Hardcoded Enums
#define CLIENT_NORMAL		1
#define CLIENT_ULTRAPEER	2
#define CLIENT_LEAF			3

#define UPDATE_RELEASE		1
#define UPDATE_BETA			2
#define UPDATE_NONE			3

#define SOCK_UNKNOWN		0
#define SOCK_CONNECTING		1
#define SOCK_CONNECTED		2
#define SOCK_CLOSED			3

#define ERROR_HOPS			1
#define ERROR_LOOPBACK		2
#define ERROR_TTL			3
#define ERROR_DUPLICATE		4
#define ERROR_ROUTING		5

#define PREF_LOCAL			1
#define PREF_LOCALNETWORK	2
#define PREF_LOCALFIREWALL	3
#define PREF_CONNECT		4
#define PREF_CONNECTSERVER  5
#define PREF_CONNECTSCREEN	6
#define PREF_SEARCH			7
#define PREF_SEARCHSCREEN	8
#define PREF_SEARCHBLOCK	9
#define PREF_SHARE			10
#define PREF_TRANSFER		11
#define PREF_BANDWIDTH	    12
#define PREF_CHAT		    13
#define PREF_PLUGINS		14

#define TRANSFER_NOSOURCES  1
#define TRANSFER_PENDING	2
#define TRANSFER_CONNECTING	3
#define TRANSFER_CONNECTED	4
#define TRANSFER_SENDING	5
#define TRANSFER_RECEIVING  6
#define TRANSFER_PUSH		7
#define TRANSFER_CLOSED		8
#define TRANSFER_COMPLETED	9
#define TRANSFER_COOLDOWN	10
#define TRANSFER_QUEUED		11

#define RESULT_INACTIVE		1
#define RESULT_TRYING		2
#define RESULT_DOWNLOADING	3
#define RESULT_COMPLETED	4
#define RESULT_NOSOURCES	5

#define HASH_SHA1			0
#define HASH_MD5			1
#define HASH_MD4_ED2K		2
#define HASH_TIGER			3
#define HASH_BITPRINT		4

#define ROOM_SERVER			1
#define ROOM_RAW			2
#define ROOM_CHANNEL		3
#define ROOM_PRIVATE		4

#define MAX_TTL				7
#define MAX_SHARE			31458312

#define CONNECT_TIMEOUT		4
#define TRANSFER_TIMEOUT    8
#define NODECACHE_SIZE		200
#define MAX_EVENTS			128
#define CHAT_MAXLINES		2000

#define ALLOW				1
#define DENY				0

#define LIMIT_NONE			0
#define LIMIT_LESS			1
#define LIMIT_EXACTLY		2
#define LIMIT_MORE			3


// Windows Custom Messages

#define PREFERENCES_CHANGE  (WM_APP + 25)

#define WM_TRAYICON_NOTIFY	(WM_APP + 36)

#define EVOLVE_CLOSE		(WM_APP + 60)	

#define CHAT_UPDATE			(WM_APP + 61)
#define CHAT_SELECT			(WM_APP + 62)
#define CHAT_USER			(WM_APP + 63)
#define CHAT_TOPIC			(WM_APP + 64)
#define CHAT_NOTICE			(WM_APP + 65)

#define TRACE_ERROR			(WM_APP + 66)

#define CONNECT_INFO	     (WM_APP + 80)

