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

#include "PrefPlugins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPrefPlugins, CPropertyPage)

CPrefPlugins::CPrefPlugins() : CPropertyPage(CPrefPlugins::IDD)
{
	//{{AFX_DATA_INIT(CPrefPlugins)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDoc	 = NULL;

}

CPrefPlugins::~CPrefPlugins()
{
}

void CPrefPlugins::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefPlugins)
	DDX_Control(pDX, IDC_COMBO_STARTUP, m_cmbStartup);
	DDX_Control(pDX, IDC_LIST_PLUGINS, m_lstPlugins);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefPlugins, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefPlugins)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPrefPlugins::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// There may be a simpler way to get this pointer
	m_pDoc   = (CGnucleusDoc*) ((CGnucleusApp*)AfxGetApp())->m_pDoc;


	// Setup listbox
	int offSet = m_lstPlugins.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstPlugins.GetWindowRect(&rect);

	m_lstPlugins.InsertColumn(0, "Name", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/4, 0);
	m_lstPlugins.InsertColumn(1, "Description", LVCFMT_LEFT,
		(rect.Width() - offSet) * 2/4, 1);
	m_lstPlugins.InsertColumn(2, "Status", LVCFMT_LEFT,
		(rect.Width() - offSet) * 1/4, 2);

	m_lstPlugins.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Setup combo box
	m_cmbStartup.InsertString(0, "Disabled");
	m_cmbStartup.InsertString(0, "Manual");
	m_cmbStartup.InsertString(0, "Automatic");
	m_cmbStartup.SelectString(-1, "Automatic");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
