/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
    Copyright (C) 2000-2002 John Marshall

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

#include "ViewSearch.h"

#include "AutSearchSink.h"


IMPLEMENT_DYNAMIC(CAutSearchSink, CCmdTarget)
CAutSearchSink::CAutSearchSink(CGnucleusDoc* pDoc)
{
	m_pDoc = pDoc;

	EnableAutomation();
}

CAutSearchSink::~CAutSearchSink()
{
}


void CAutSearchSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAutSearchSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAutSearchSink, CCmdTarget)
		DISP_FUNCTION_ID(CAutSearchSink, "OnUpdate", 1, OnUpdate, VT_EMPTY, VTS_I4 VTS_UI4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnResult", 2, OnResult, VT_EMPTY, VTS_I4 VTS_UI4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnRefresh", 3, OnRefresh, VT_EMPTY, VTS_UI4)
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CAutSearchSink, CCmdTarget)
	INTERFACE_PART(CAutSearchSink, IID_ISearchEvent, Dispatch)
END_INTERFACE_MAP()


// CAutSearchSink message handlers

void CAutSearchSink::OnUpdate(LONG SearchID, ULONG ResultID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnUpdate(ResultID);
}

void CAutSearchSink::OnResult(LONG SearchID, ULONG ResultID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnResult(ResultID);
}

void CAutSearchSink::OnRefresh(LONG SearchID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnRefresh();
}
