/********************************************************************************

	Gnucleus - An Application for the Gnutella Network
	Copyright (C) 2000-2005 John Marshall Group

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	By contributing code you grant John Marshall Group an unlimited, non-exclusive
	license to your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

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
		DISP_FUNCTION_ID(CAutSearchSink, "OnUpdate", 1, OnUpdate, VT_EMPTY, VTS_I4 VTS_I4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnResult", 2, OnResult, VT_EMPTY, VTS_I4 VTS_I4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnRefresh", 3, OnRefresh, VT_EMPTY, VTS_I4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnBrowseUpdate", 4, OnBrowseUpdate, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnProgress", 5, OnProgress, VT_EMPTY, VTS_I4)
		DISP_FUNCTION_ID(CAutSearchSink, "OnPaused", 6, OnPaused, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CAutSearchSink, CCmdTarget)
	INTERFACE_PART(CAutSearchSink, IID_ISearchEvent, Dispatch)
END_INTERFACE_MAP()


// CAutSearchSink message handlers

void CAutSearchSink::OnUpdate(int SearchID, int ResultID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnUpdate(ResultID);
}

void CAutSearchSink::OnResult(int SearchID, int ResultID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnResult(ResultID);
}

void CAutSearchSink::OnRefresh(int SearchID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnRefresh();
}

void CAutSearchSink::OnBrowseUpdate(int SearchID, int State, int Progress)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnBrowseUpdate(State, Progress);
}

void CAutSearchSink::OnProgress(int SearchID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnProgress();
}

void CAutSearchSink::OnPaused(int SearchID)
{
	std::vector<HWND>::iterator itSearch;
	for(itSearch = m_pDoc->m_pViewSearch.begin(); itSearch != m_pDoc->m_pViewSearch.end(); itSearch++)
		if( SearchID == ((CViewSearch*) CWnd::FromHandle(*itSearch))->m_SearchID)
			((CViewSearch*) CWnd::FromHandle(*itSearch))->OnPaused();
}