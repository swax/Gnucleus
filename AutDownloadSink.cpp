/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2004 John Marshall

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
#include "GnucleusDoc.h"

#include "ViewTransfers.h"

#include "AutDownloadSink.h"


IMPLEMENT_DYNAMIC(CAutDownloadSink, CCmdTarget)
CAutDownloadSink::CAutDownloadSink(CGnucleusDoc* pDoc)
{
	m_pDoc = pDoc;

	EnableAutomation();
}

CAutDownloadSink::~CAutDownloadSink()
{
}


void CAutDownloadSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAutDownloadSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAutDownloadSink, CCmdTarget)
	DISP_FUNCTION_ID(CAutDownloadSink, "OnUpdate", 1, OnUpdate, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// Note: we add support for IID_IAutDownloadSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

BEGIN_INTERFACE_MAP(CAutDownloadSink, CCmdTarget)
	INTERFACE_PART(CAutDownloadSink, IID_IDownloadEvent, Dispatch)
END_INTERFACE_MAP()


// CAutDownloadSink message handlers

void CAutDownloadSink::OnUpdate(LONG DownloadID)
{
	if(m_pDoc->m_pViewTransfers)
		((CViewTransfers*) CWnd::FromHandle(m_pDoc->m_pViewTransfers))->OnDownloadUpdate(DownloadID);
}