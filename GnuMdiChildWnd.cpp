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
#include "resource.h"
#include "FrameMain.h"
#include "GnuMdiChildWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGnuMdiChildWnd, CMDIChildWnd)

CGnuMdiChildWnd::CGnuMdiChildWnd()
{
}

CGnuMdiChildWnd::~CGnuMdiChildWnd()
{
}


BEGIN_MESSAGE_MAP(CGnuMdiChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CGnuMdiChildWnd)
	ON_WM_MDIACTIVATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CGnuMdiChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	CFrameMain* pMain = (CFrameMain*)AfxGetMainWnd();
	ASSERT(pMain);
	if (!pMain)
	{
		return;
	}

	if (bActivate)
	{
		pMain->m_wndWindowListDlgBar.Activate(pActivateWnd);
	}
}


void CGnuMdiChildWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);

	CFrameMain* pMain = (CFrameMain*)AfxGetMainWnd();
	ASSERT(pMain);
	if (!pMain)
	{
		return;
	}

	if (bShow == FALSE)
	{
		pMain->m_wndWindowListDlgBar.Close(this);
	}
	else
	{
		pMain->m_wndWindowListDlgBar.Activate(this);
	}
}


void CGnuMdiChildWnd::UpdateTitle() 
{
	CFrameMain* pMain = (CFrameMain*)AfxGetMainWnd();
	ASSERT(pMain);
	if (!pMain)
	{
		return;
	}
	pMain->m_wndWindowListDlgBar.Update(this);
}

// Makes no difference
//void CGnuMdiChildWnd::ActivateFrame(int nCmdShow) 
