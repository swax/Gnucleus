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
#include "GnucleusDoc.h"

#include "EvolverDlg.h"

#include "AutUpdateSink.h"


// CAutUpdateSink

IMPLEMENT_DYNAMIC(CAutUpdateSink, CCmdTarget)
CAutUpdateSink::CAutUpdateSink(CGnucleusDoc* pDoc)
{
	m_pDoc = pDoc;

	EnableAutomation();
}

CAutUpdateSink::~CAutUpdateSink()
{
}


void CAutUpdateSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAutUpdateSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAutUpdateSink, CCmdTarget)
	DISP_FUNCTION_ID(CAutUpdateSink, "OnUpdateFound", 1, OnUpdateFound, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CAutUpdateSink, "OnUpdateFailed", 2, OnUpdateFailed, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CAutUpdateSink, "OnVersionCurrent", 3, OnVersionCurrent, VT_EMPTY, NULL)
	DISP_FUNCTION_ID(CAutUpdateSink, "OnUpdateComplete", 4, OnUpdateComplete, VT_EMPTY, NULL)
END_DISPATCH_MAP()

// Note: we add support for IID_IAutUpdateSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

BEGIN_INTERFACE_MAP(CAutUpdateSink, CCmdTarget)
	INTERFACE_PART(CAutUpdateSink, IID_IUpdateEvent, Dispatch)
END_INTERFACE_MAP()


// CAutUpdateSink message handlers

void CAutUpdateSink::OnUpdateFound(LPCTSTR Version)
{
	m_pDoc->DisplayEvolver();

	if(m_pDoc->m_pDiagEvolve)
		m_pDoc->m_pDiagEvolve->OnUpdateFound(Version);
}

void CAutUpdateSink::OnUpdateFailed(LPCTSTR Reason)
{
	if(m_pDoc->m_pDiagEvolve)
		m_pDoc->m_pDiagEvolve->OnUpdateFailed(Reason);
}

void CAutUpdateSink::OnVersionCurrent()
{
	if(m_pDoc->m_pDiagEvolve)
		m_pDoc->m_pDiagEvolve->OnVersionCurrent();
}

void CAutUpdateSink::OnUpdateComplete()
{
	if(m_pDoc->m_pDiagEvolve)
		m_pDoc->m_pDiagEvolve->OnUpdateComplete();
}

