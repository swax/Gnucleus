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
#include "gnucleus.h"

#include "ViewBrowser.h"

#include "FrameBrowser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFrameBrowser, CGnuMdiChildWnd)

CFrameBrowser::CFrameBrowser()
{
}

CFrameBrowser::~CFrameBrowser()
{
}


BEGIN_MESSAGE_MAP(CFrameBrowser, CGnuMdiChildWnd)
	//{{AFX_MSG_MAP(CFrameBrowser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_SIZING()
END_MESSAGE_MAP()


BOOL CFrameBrowser::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~FWS_ADDTOTITLE; 
	cs.lpszName = _T("Browser");

	if( !CGnuMdiChildWnd::PreCreateWindow(cs) )
		return false;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return true;
}

BOOL CFrameBrowser::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(wParam == 1 && lParam == 0)
		((CViewBrowser*) GetActiveView())->CommandSent();
	
	return CGnuMdiChildWnd::OnCommand(wParam, lParam);
}

#define YWINMIN 250	
#define XWINMIN 500

#define FORM_BOTTOM \
	if(cRect.Height() < YWINMIN) \
		pRect->bottom = pRect->top + YWINMIN;

#define FORM_TOP \
	if(cRect.Height() < YWINMIN) \
		pRect->top = pRect->bottom - YWINMIN;

#define FORM_LEFT \
	if(cRect.Width() < XWINMIN) \
		pRect->left = pRect->right - XWINMIN;

#define FORM_RIGHT \
	if(cRect.Width() < XWINMIN) \
		pRect->right = pRect->left + XWINMIN;


void CFrameBrowser::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CGnuMdiChildWnd::OnSizing(fwSide, pRect);

	CRect cRect = pRect;

	switch(fwSide)
	{
	case WMSZ_BOTTOM:
		FORM_BOTTOM;
		break;

	case WMSZ_BOTTOMLEFT:
		FORM_BOTTOM;
		FORM_LEFT;
		break;

	case WMSZ_BOTTOMRIGHT:
		FORM_BOTTOM;
		FORM_RIGHT;
		break;

	case WMSZ_TOP:
		FORM_TOP;
		break;

	case WMSZ_TOPLEFT:
		FORM_TOP;
		FORM_LEFT;
		break;

	case WMSZ_TOPRIGHT:
		FORM_TOP;
		FORM_RIGHT;
		break;

	case WMSZ_LEFT:
		FORM_LEFT;
		break;

	case WMSZ_RIGHT:
		FORM_RIGHT;
		break;

	default:
		break;
	}

}

