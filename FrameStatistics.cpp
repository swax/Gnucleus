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
#include "FrameStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFrameStatistics, CGnuMdiChildWnd)

CFrameStatistics::CFrameStatistics()
{
	
}

CFrameStatistics::~CFrameStatistics()
{
}


BEGIN_MESSAGE_MAP(CFrameStatistics, CGnuMdiChildWnd)
	//{{AFX_MSG_MAP(CFrameStatistics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFrameStatistics::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~FWS_ADDTOTITLE; 
	cs.lpszName = "Statistics";

	if( !CGnuMdiChildWnd::PreCreateWindow(cs) )
		return false;

	return true;
}
