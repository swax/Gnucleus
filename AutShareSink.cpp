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


#include "stdafx.h"
#include "Gnucleus.h"
#include "GnucleusDoc.h"

#include "ViewShare.h"

#include "AutShareSink.h"


IMPLEMENT_DYNAMIC(CAutShareSink, CCmdTarget)
CAutShareSink::CAutShareSink(CGnucleusDoc* pDoc)
{
	m_pDoc = pDoc;

	EnableAutomation();
}

CAutShareSink::~CAutShareSink()
{
}


void CAutShareSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAutShareSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAutShareSink, CCmdTarget)
	DISP_FUNCTION_ID(CAutShareSink, "OnUpdate", 1, OnUpdate, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CAutShareSink, "OnReload", 2, OnReload, VT_EMPTY, NULL)
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CAutShareSink, CCmdTarget)
	INTERFACE_PART(CAutShareSink, IID_IShareEvent, Dispatch)
END_INTERFACE_MAP()


// CAutShareSink message handlers

void CAutShareSink::OnUpdate(int FileID)
{
	if(m_pDoc->m_pViewShare)
		((CViewShare*) CWnd::FromHandle(m_pDoc->m_pViewShare))->OnShareUpdate(FileID);
}

void CAutShareSink::OnReload()
{
	if(m_pDoc->m_pViewShare)
		((CViewShare*) CWnd::FromHandle(m_pDoc->m_pViewShare))->OnShareReload();
}

