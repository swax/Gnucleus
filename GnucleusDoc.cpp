/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (c) 2000-2003 John Marshall

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


#include "stdafx.h"
#include "Gnucleus.h"
#include "PrefsEx.h"
#include "EvolverDlg.h"

#include "AutCore.h"
#include "AutPrefs.h"
#include "AutNetwork.h"
#include "AutCache.h"
#include "AutMeta.h"
#include "AutShare.h"
#include "AutSearch.h"
#include "AutDownload.h"
#include "AutUpload.h"
#include "AutUpdate.h"

#include "AutNetworkSink.h"
#include "AutShareSink.h"
#include "AutSearchSink.h"
#include "AutDownloadSink.h"
#include "AutUploadSink.h"
#include "AutUpdateSink.h"

#include "ChatControl.h"
#include "ChatServer.h"
#include "ChatPrefs.h"

#include "ViewConnect.h"
#include "ViewShare.h"
#include "ViewSearch.h"
#include "ViewTransfers.h"
#include "ViewStatistics.h"
#include "ViewChat.h"
#include "NetSetup.h"

#include <afxctl.h>

#include "GnucleusDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGnucleusDoc, CDocument)

BEGIN_MESSAGE_MAP(CGnucleusDoc, CDocument)
	//{{AFX_MSG_MAP(CGnucleusDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGnucleusDoc::CGnucleusDoc()
{
}

CGnucleusDoc::CGnucleusDoc(CGnucleusApp* pApp)
{
	m_pApp = pApp;

	m_pViewConnect	 = NULL;
	m_pViewShare	 = NULL;
	m_pViewTransfers = NULL;
	m_pViewChat		 = NULL;

	// Load main core
	ConnectCore();

	m_autCore->SetClientName("Gnucleus");
	m_autCore->SetClientVersion(GNUCLEUS_VERSION); 
	m_autCore->SetClientCode("GNUC");

	m_RunPath = m_autCore->GetRunPath();


	// Load Meta data schemas
	m_autMeta->LoadSchemaDir(m_RunPath + "\\Schemas");


	// Load preferences
	m_autPrefs->LoadConfig( m_RunPath + "Data\\GnuConfig.ini");
	m_autPrefs->LoadBlocked( m_RunPath + "Data\\GnuBlocked.net");


	// Load Gnuc specific prefs
	m_pPrefsEx = new CPrefsEx;
	m_pPrefsEx->LoadPrefsEx( m_RunPath + "Data\\GnuConfigEx.ini");


	// Load node caches
	m_autCache->LoadCache(m_RunPath + "Data\\GnuCache.net");
	

	// Load web caches
	m_autCache->LoadWebCache(m_RunPath + "Data\\WebCache.net");
	
	if(!m_autCache->GetWebCacheSize())
	{
		// HARDCODED: a few default caches, sites down
		//m_autCache->AddWebCache("http://www.zero-g.net/gcache/gcache.php");
		//m_autCache->AddWebCache("http://www.gnucleus.net/gcache/gcache.php");
	}


	
	// Load update servers and check for update
	m_autUpdate->AddServer("http://gnucleus.gnutelliums.com:80/update2/update.xml");
	m_autUpdate->AddServer("http://www.gnucleus.net/update2/update.xml");
	m_autUpdate->Check();


	// Load Chat
	m_pChat  = new CChatControl(this);


	// Display LAN setup dialog
	if(m_autPrefs->GetLanMode())
	{
		m_autNetwork->LanModeOn();

		m_autCache->LoadWebCache(m_RunPath + "Data\\LanWebCache.net");
		
		if( !(m_pApp->m_Min && !m_autPrefs->GetLanName().IsEmpty()) )
		{		
			CNetSetup NetSetup(this);
			NetSetup.DoModal();
		}
	}

	// Start connecting
	if(m_pPrefsEx->m_AutoConnect)
	{
		m_autCore->Connect2(NETWORK_GNUTELLA);
		m_autCore->Connect2(NETWORK_G2);
		
		m_autNetwork->ClientMode();
		m_pChat->m_AutoConnect = true;
	}

	//DELETE THIS
	//m_autNetwork->ForceUltrapeer2(false, NETWORK_GNUTELLA);
	//m_autNetwork->ForceUltrapeer2(true, NETWORK_G2);
	//m_autPrefs->SetMaxLeaves(10);

	// Connect to chat
	if(m_autPrefs->GetLanMode() && m_pChat->m_pPrefs->m_InternalIRC)
		m_pChat->GetConnect();


	m_pDiagEvolve = NULL;
	m_nLastPref	  = PREF_LOCAL;
}

void CGnucleusDoc::ConnectCore()
{
	m_autCore = new CAutCore;
	if(!m_autCore->CreateDispatch("GnucDNA.Core"))
	{
		AfxMessageBox("Could not load GnucDNA");
		return;
	}

	// Make sure version of core is same or newer than what we built with
	CString CoreVersion = m_autCore->GetCoreVersion();
	CoreVersion.Remove('.');

	CString BuildVersion = BUILD_CORE_VERSION;
	BuildVersion.Remove('.');

	if(atoi(CoreVersion) < atoi(BuildVersion))
	{
		AfxMessageBox("GnucDNA " + CString(BUILD_CORE_VERSION) + " or higher needed, please update");
		return;
	}

	// Attach prefs object
	m_autPrefs = new CAutPrefs;
	m_autPrefs->AttachDispatch( m_autCore->GetIPrefs(), false );

	// Attach network object
	m_autNetwork = new CAutNetwork;
	m_autNetwork->AttachDispatch( m_autCore->GetINetwork(), false );

	// Attach cache object
	m_autCache = new CAutCache;
	m_autCache->AttachDispatch( m_autCore->GetICache(), false );

	// Attach meta object
	m_autMeta = new CAutMeta;
	m_autMeta->AttachDispatch( m_autCore->GetIMeta(), false );

	// Attach share object
	m_autShare = new CAutShare;
	m_autShare->AttachDispatch( m_autCore->GetIShare(), false );

	// Attach share object
	m_autSearch = new CAutSearch;
	m_autSearch->AttachDispatch( m_autCore->GetISearch(), false );

	// Attach download object
	m_autDownload = new CAutDownload;
	m_autDownload->AttachDispatch( m_autCore->GetIDownload(), false );

	// Attach upload object
	m_autUpload = new CAutUpload;
	m_autUpload->AttachDispatch( m_autCore->GetIUpload(), false );

	// Attach update object
	m_autUpdate = new CAutUpdate;
	m_autUpdate->AttachDispatch( m_autCore->GetIUpdate(), false );

	// Network event object and establish a connection between source and sink.
	m_autNetworkSink = new CAutNetworkSink(this);
	AfxConnectionAdvise(m_autNetwork->m_lpDispatch, IID_INetworkEvent, m_autNetworkSink->GetIDispatch(false), false, &m_NetEventCookie);

	// Share event object and establish a connection between source and sink.
	m_autShareSink = new CAutShareSink(this);
	AfxConnectionAdvise(m_autShare->m_lpDispatch, IID_IShareEvent, m_autShareSink->GetIDispatch(false), false, &m_ShareEventCookie);

	// Share event object and establish a connection between source and sink.
	m_autSearchSink = new CAutSearchSink(this);
	AfxConnectionAdvise(m_autSearch->m_lpDispatch, IID_ISearchEvent, m_autSearchSink->GetIDispatch(false), false, &m_SearchEventCookie);

	// Download event object and establish a connection between source and sink.
	m_autDownloadSink = new CAutDownloadSink(this);
	AfxConnectionAdvise(m_autDownload->m_lpDispatch, IID_IDownloadEvent, m_autDownloadSink->GetIDispatch(false), false, &m_DownloadEventCookie);

	// Upload event object and establish a connection between source and sink.
	m_autUploadSink = new CAutUploadSink(this);
	AfxConnectionAdvise(m_autUpload->m_lpDispatch, IID_IUploadEvent, m_autUploadSink->GetIDispatch(false), false, &m_UploadEventCookie);

	// Update event object and establish a connection between source and sink.
	m_autUpdateSink = new CAutUpdateSink(this);
	AfxConnectionAdvise(m_autUpdate->m_lpDispatch, IID_IUpdateEvent, m_autUpdateSink->GetIDispatch(false), false, &m_UpdateEventCookie);

}

void CGnucleusDoc::DisconnectCore()
{
	AfxConnectionUnadvise(m_autUpdate->m_lpDispatch, IID_IUpdateEvent, m_autUpdateSink->GetIDispatch(false), false, m_UpdateEventCookie);
	delete m_autUpdateSink;
	m_autUpdateSink = NULL;

	AfxConnectionUnadvise(m_autUpload->m_lpDispatch, IID_IUploadEvent, m_autUploadSink->GetIDispatch(false), false, m_UploadEventCookie);
	delete m_autUploadSink;
	m_autUploadSink = NULL;

	AfxConnectionUnadvise(m_autDownload->m_lpDispatch, IID_IDownloadEvent, m_autDownloadSink->GetIDispatch(false), false, m_DownloadEventCookie);
	delete m_autDownloadSink;
	m_autDownloadSink = NULL;

	AfxConnectionUnadvise(m_autSearch->m_lpDispatch, IID_ISearchEvent, m_autSearchSink->GetIDispatch(false), false, m_SearchEventCookie);
	delete m_autSearchSink;
	m_autSearchSink = NULL;

	AfxConnectionUnadvise(m_autShare->m_lpDispatch, IID_IShareEvent, m_autShareSink->GetIDispatch(false), false, m_ShareEventCookie);
	delete m_autShareSink;
	m_autShareSink = NULL;

	AfxConnectionUnadvise(m_autNetwork->m_lpDispatch, IID_INetworkEvent, m_autNetworkSink->GetIDispatch(false), false, m_NetEventCookie);
	delete m_autNetworkSink;
	m_autNetworkSink = NULL;


	delete m_autUpdate;
	m_autUpdate = NULL;

	delete m_autUpload;
	m_autUpload = NULL;

	delete m_autDownload;
	m_autDownload = NULL;

	delete m_autSearch;
	m_autSearch = NULL;

	delete m_autShare;
	m_autShare = NULL;

	delete m_autMeta;
	m_autMeta = NULL;	

	delete m_autCache;
	m_autCache = NULL;

	delete m_autNetwork;
	m_autNetwork = NULL;

	delete m_autPrefs;
	m_autPrefs = NULL;

	delete m_autCore;
	m_autCore = NULL;
}

CGnucleusDoc::~CGnucleusDoc()
{
	m_pPrefsEx->SavePrefsEx(m_RunPath + "Data\\GnuConfigEx.ini");

	// Save preferences
	m_autPrefs->SaveConfig( m_RunPath + "Data\\GnuConfig.ini");
	m_autPrefs->SaveBlocked(m_RunPath  + "Data\\GnuBlocked.net");


	// Save node caches
	m_autCache->SaveCache(m_RunPath + "Data\\GnuCache.net");


	// Save web caches
	if(m_autPrefs->GetLanMode())
		m_autCache->SaveWebCache(m_RunPath + "Data\\LanWebCache.net");
	else
	{	
		if(!m_autCache->GetWebCacheSize())
		{
			// HARDCODED: a few default caches
			//m_autCache->AddWebCache("http://www.zero-g.net/gcache/gcache.php");
			//m_autCache->AddWebCache("http://www.gnucleus.net/gcache/gcache.php");
		}
		
		m_autCache->SaveWebCache(m_RunPath + "Data\\WebCache.net");
	}

	delete m_pPrefsEx;
	m_pPrefsEx = NULL;

	delete m_pChat;
	m_pChat = NULL;

	DisconnectCore();

	TRACE0("*** CGnucleusDoc Deconstructing\n");
}


/////////////////////////////////////////////////////////////////////////////
// CGnucleusDoc serialization

void CGnucleusDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGnucleusDoc diagnostics

#ifdef _DEBUG
void CGnucleusDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGnucleusDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGnucleusDoc commands




CString CGnucleusDoc::ResolveIP(IP HostIP)
{
	/*hostent* pHostent;
	pHostent = (LPHOSTENT) m_pComm->m_HostEnt;
	pHostent = gethostbyaddr((char*) &HostIP.S_addr, sizeof(HostIP), AF_INET);
 
	if(pHostent)
	{
		CString HostName( pHostent->h_name);
		return HostName;
	}*/
	
	return IPtoStr(HostIP);
}


FullIcon CGnucleusDoc::GetIconIndex(const CString& file)
{
	FullIcon Icon;

	Icon.Index = GetIconIndexFromName(file);
	Icon.Type  = GetIconDescFromName(file);

	return Icon;
}

void CGnucleusDoc::DisplayEvolver()
{
	if(m_pDiagEvolve == NULL)
	{
		m_pDiagEvolve = new CEvolverDlg(AfxGetApp()->GetMainWnd());
		m_pDiagEvolve->m_pDoc = this;
		m_pDiagEvolve->m_autUpdate = m_autUpdate;

		m_pDiagEvolve->Create(IDD_EVOLVE_MAIN, AfxGetApp()->m_pMainWnd);
	}

	m_pDiagEvolve->ShowWindow(SW_SHOW);
	m_pDiagEvolve->BringWindowToTop();
}

