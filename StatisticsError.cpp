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
	license your contribution.

	For support, questions, commercial use, etc...
	E-Mail: swabby@c0re.net

********************************************************************************/

#include "stdafx.h"
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "ViewStatistics.h"
#include "StatisticsMain.h"
#include "StatisticsError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsError, CPropertyPage)

CStatisticsError::CStatisticsError(CViewStatistics *pView) : CPropertyPage(CStatisticsError::IDD)
{
	m_pStatView = pView;

	//{{AFX_DATA_INIT(CStatisticsError)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStatisticsError::~CStatisticsError()
{
}

void CStatisticsError::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsError)
	DDX_Control(pDX, IDC_LIST_ERROR, m_lstError);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsError, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsError)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsError::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Error listbox
	int offSet = m_lstError.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstError.GetWindowRect(&rect);

	m_lstError.InsertColumn( 0, "Time", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/5, 0);
	m_lstError.InsertColumn( 1, "Description", LVCFMT_LEFT,
		(rect.Width() - offSet) * 4/5, 1);

	m_lstError.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_DlgResizer.InitResizer(this);
	m_DlgResizer.ListCtrlItem(IDC_LIST_ERROR);
	m_DlgResizer.SizeItem(IDC_LIST_ERROR, CDlgResizer::DownAndRight);
	m_DlgResizer.Done();

	
	/*for(int i = 0; i < m_pStatView->m_pDoc->m_ErrorList.size(); i++)
	{	
		m_lstError.InsertItem(0, m_pStatView->m_pDoc->m_ErrorList[i].Time.Format("%I:%M %p"));
		m_lstError.SetItemText(0, 1, m_pStatView->m_pDoc->m_ErrorList[i].Description);
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void CStatisticsError::NewError(ErrorInfo* pError)
//{
//	/*m_lstError.InsertItem(0, pError->Time.Format("%I:%M %p"));
//	m_lstError.SetItemText(0, 1, pError->Description);*/
//}
