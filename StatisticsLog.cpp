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
#include "StatisticsLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CStatisticsLog, CPropertyPage)

CStatisticsLog::CStatisticsLog() : CPropertyPage(CStatisticsLog::IDD)
{
	//{{AFX_DATA_INIT(CStatisticsLog)
	//}}AFX_DATA_INIT
}

CStatisticsLog::~CStatisticsLog()
{
}

void CStatisticsLog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsLog)
	DDX_Control(pDX, IDC_BUTTON_FLUSH, m_btnFlush);
	DDX_Control(pDX, IDC_CHECK_QUERYHITS, m_chkQueryHits);
	DDX_Control(pDX, IDC_CHECK_BAD, m_chkBad);
	DDX_Control(pDX, IDC_CHECK_GOOD, m_chkGood);
	DDX_Control(pDX, IDC_STATIC_BUFFSIZE, m_stBuffSize);
	DDX_Control(pDX, IDC_LIST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_EDIT_BUFFSIZE, m_ebBuffSize);
	DDX_Control(pDX, IDC_CHECK_QUERIES, m_chkQueries);
	DDX_Control(pDX, IDC_CHECK_PUSHES, m_chkPushes);
	DDX_Control(pDX, IDC_CHECK_PONGS, m_chkPongs);
	DDX_Control(pDX, IDC_CHECK_PINGS, m_chkPings);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_IN, m_chkIn);
	DDX_Control(pDX, IDC_CHECK_OUT, m_chkOut);
}


BEGIN_MESSAGE_MAP(CStatisticsLog, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticsLog)
	ON_BN_CLICKED(IDC_BUTTON_FLUSH, OnButtonFlush)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStatisticsLog::PreCreateWindow(CREATESTRUCT& cs) 
{
	SetWindowText("Log");
	
	return CPropertyPage::PreCreateWindow(cs);
}

BOOL CStatisticsLog::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_ebBuffSize.SetWindowText("500");

	// Load icons
	m_LogIcons.Create(16, 16, ILC_MASK, 0, 4);
	m_LogIcons.Add(AfxGetApp()->LoadIcon(IDR_INCOMING));    // 0
	m_LogIcons.Add(AfxGetApp()->LoadIcon(IDR_BROADCASTED)); // 1
	m_LogIcons.Add(AfxGetApp()->LoadIcon(IDR_ROUTED));      // 2
	m_LogIcons.Add(AfxGetApp()->LoadIcon(IDR_TRASHED));     // 3

	m_lstLog.SetImageList(&m_LogIcons, LVSIL_SMALL);

	// Log listbox
	int offSet = m_lstLog.GetScrollLimit(SB_VERT) ? ::GetSystemMetrics(SM_CXVSCROLL) + 3 : 4;

	CRect rect;
	m_lstLog.GetWindowRect(&rect);

	m_lstLog.InsertColumn( 0, "Incoming Packet", LVCFMT_LEFT,
		(rect.Width() - offSet) * 4/10, 0);
	m_lstLog.InsertColumn( 1, "Hops", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/10, 1);
	m_lstLog.InsertColumn( 2, "TTL", LVCFMT_RIGHT,
		(rect.Width() - offSet) * 1/10, 2);
	m_lstLog.InsertColumn( 3, "Payload", LVCFMT_LEFT,
		(rect.Width() - offSet) * 6/10, 3);
	m_lstLog.InsertColumn( 4, "GUID", LVCFMT_LEFT,
		(rect.Width() - offSet) * 4/10, 4);

	m_lstLog.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_DlgResizer.InitResizer(this);
	m_DlgResizer.ListCtrlItem(IDC_LIST_LOG);
	m_DlgResizer.SizeItem(IDC_LIST_LOG, CDlgResizer::DownAndRight);
	m_DlgResizer.MoveItem(IDC_BUTTON_FLUSH, CDlgResizer::Right);
	m_DlgResizer.MoveItem(IDC_EDIT_BUFFSIZE, CDlgResizer::Right);
	m_DlgResizer.MoveItem(IDC_STATIC_BUFFSIZE, CDlgResizer::Right);
	m_DlgResizer.Done();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsLog::PacketIncoming(packet_Header* packet)
{
	if(m_chkGood.GetCheck() && PacketChecked(packet) && m_chkIn.GetCheck())
	{
		m_lstLog.InsertItem(0, "In: " + PacketName(packet) + "Received", 0);
		m_lstLog.SetItemText(0, 1, DWrdtoStr(packet->Hops));
		m_lstLog.SetItemText(0, 2, DWrdtoStr(packet->TTL));
		m_lstLog.SetItemText(0, 3, PacketExtra(packet));
		m_lstLog.SetItemText(0, 4, GetGuid(packet));
	}

	CleanBuffer();
}

void CStatisticsLog::PacketGood(packet_Header* packet)
{
	if(m_chkGood.GetCheck() && PacketChecked(packet) && m_chkIn.GetCheck())
	{
		m_lstLog.InsertItem(0, "In: " + PacketName(packet), PacketIcon(packet));
		m_lstLog.SetItemText(0, 1, DWrdtoStr(packet->Hops));
		m_lstLog.SetItemText(0, 2, DWrdtoStr(packet->TTL));
		m_lstLog.SetItemText(0, 3, PacketExtra(packet));
		m_lstLog.SetItemText(0, 4, GetGuid(packet));
	}
		
	CleanBuffer();
}

void CStatisticsLog::PacketBad(packet_Header* packet, int ErrorCode)
{
	if(m_chkBad.GetCheck() && PacketChecked(packet) && m_chkIn.GetCheck())
	{
		m_lstLog.InsertItem(0, "In: " + PacketName(packet) + ErrorName(ErrorCode), 3);
		m_lstLog.SetItemText(0, 1, DWrdtoStr(packet->Hops));
		m_lstLog.SetItemText(0, 2, DWrdtoStr(packet->TTL));
		m_lstLog.SetItemText(0, 3, PacketExtra(packet));
		m_lstLog.SetItemText(0, 4, GetGuid(packet));
	}
}

void CStatisticsLog::PacketOutgoing(packet_Header* packet)
{
	if(m_chkGood.GetCheck() && PacketChecked(packet) && m_chkOut.GetCheck())
	{
		m_lstLog.InsertItem(0, "Out: " + PacketName(packet), PacketIcon(packet));
		m_lstLog.SetItemText(0, 1, DWrdtoStr(packet->Hops));
		m_lstLog.SetItemText(0, 2, DWrdtoStr(packet->TTL));
		m_lstLog.SetItemText(0, 3, PacketExtra(packet));
		m_lstLog.SetItemText(0, 4, GetGuid(packet));
	}

	CleanBuffer();
}

void CStatisticsLog::PacketOutgoingLocal(packet_Header* packet)
{
	if(m_chkGood.GetCheck() && PacketChecked(packet) && m_chkOut.GetCheck())
	{
		m_lstLog.InsertItem(0, "Out: " + PacketName(packet) + " Sent", PacketIcon(packet));
		m_lstLog.SetItemText(0, 1, DWrdtoStr(packet->Hops));
		m_lstLog.SetItemText(0, 2, DWrdtoStr(packet->TTL));
		m_lstLog.SetItemText(0, 3, PacketExtra(packet));
		m_lstLog.SetItemText(0, 4, GetGuid(packet));
	}

	CleanBuffer();
}

void CStatisticsLog::CleanBuffer()
{
	CString BuffSize;
	m_ebBuffSize.GetWindowText(BuffSize);
	while(m_lstLog.GetItemCount() > atol(BuffSize))
		m_lstLog.DeleteItem( m_lstLog.GetItemCount() - 1);
}

bool CStatisticsLog::PacketChecked(packet_Header* packet)
{
	switch(packet->Function)
	{
	case 0x00:
		return m_chkPings.GetCheck() ? 1:0;
		break;
	case 0x01:
		return m_chkPongs.GetCheck() ? 1:0;
		break;
	case 0x40:
		return m_chkPushes.GetCheck() ? 1:0;
		break;
	case 0x80:
		return m_chkQueries.GetCheck() ? 1:0;
		break;
	case 0x81:
		return m_chkQueryHits.GetCheck() ? 1:0;
		break;
	}
	
	return 0;
}

CString CStatisticsLog::PacketName(packet_Header* packet)
{
	CString Name;

	switch(packet->Function )
	{
	case 0x00:
		Name = "Ping ";
		break;
	case 0x01:
		Name = "Pong ";
		break;
	case 0x40:
		Name = "Push ";
		break;
	case 0x80:
		Name = "Query ";
		break;
	case 0x81:
		Name = "Query Hit ";
		break;
	default:
		Name = "Unknown ";
	}

	return Name;
}

int CStatisticsLog::PacketIcon(packet_Header* packet)
{
	switch(packet->Function)
	{
	case 0x00:
		return 1;
		break;
	case 0x01:
		return 2;
		break;
	case 0x40:
		return 2;
		break;
	case 0x80:
		return 1;
		break;
	case 0x81:
		return 2;
		break;
	default:
		return 3;
	}
}

CString CStatisticsLog::PacketExtra(packet_Header* packet)
{
	CString strExtra;
	
	packet_Pong*	 pPong		= NULL;
	packet_Push*	 pPush		= NULL;
	packet_Query*	 pQuery		= NULL;
	byte*			 pSearch	= NULL;
	packet_QueryHit* pQueryHit  = NULL;

	int QuerySize, TextSize;

	switch(packet->Function)
	{
	case 0x00:
		return "";
		break;
	case 0x01:
		pPong = (packet_Pong*) packet;
		strExtra = IPtoStr(pPong->Host) + ":" + DWrdtoStr(pPong->Port) + " sharing " + CommaIze(DWrdtoStr( pPong->FileSize/1024 )) + " MB";

		return strExtra;
		break;
	case 0x40:
		pPush = (packet_Push*) packet;
		strExtra = "From " + IPtoStr(pPush->Host) + " wanting file #" + CommaIze(DWrdtoStr(pPush->Index));

		return strExtra;
		break;
	case 0x80:
		pQuery = (packet_Query*) packet;

		strExtra = "For ( ";

		strExtra += (char*) pQuery + 25;
		
		QuerySize  = pQuery->Header.Payload - 2;
		TextSize   = strlen((char*) pQuery + 25) + 1;
		
		if (TextSize < QuerySize)
		{
			int ExtendedSize = strlen((char*) pQuery + 25 + TextSize);
		
			if(ExtendedSize)
			{
				CString ExtendedQuery = CString((char*) pQuery + 25 + TextSize, ExtendedSize);
				ExtendedQuery.Replace(0x1C, 0x20);
				strExtra += " and " + ExtendedQuery;
			}
		}

		strExtra += " )";

		return strExtra;
		break;

	case 0x81:
		pQueryHit = (packet_QueryHit*) packet;
		strExtra = "From " + IPtoStr(pQueryHit->Host) + " with " + DWrdtoStr( pQueryHit->TotalHits );

		if(pQueryHit->TotalHits == 1)
			strExtra += " match";
		else 
			strExtra += " matches";

		return strExtra;
		break;
	default:
		return strExtra;
	}
}

CString CStatisticsLog::GetGuid(packet_Header* packet)
{
	byte* ClientID = (byte*) &packet->Guid;

	CString Guid;
	Guid.Format("%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x",
				ClientID[0],  ClientID[1],  ClientID[2],  ClientID[3],  ClientID[4],
				ClientID[5],  ClientID[6],  ClientID[7],  ClientID[8],  ClientID[9],
				ClientID[10], ClientID[11], ClientID[12], ClientID[13], ClientID[14],
				ClientID[15]);

	return Guid;
}

CString CStatisticsLog::ErrorName(DWORD dwError)
{
	switch(dwError)
	{
	case ERROR_HOPS:
		return "(Hops)";
		break;
	case ERROR_LOOPBACK:
		return "(Loopback)";
		break;
	case ERROR_TTL:
		return "(Time to Live)";
		break;
	case ERROR_DUPLICATE:
		return "(Duplicate)";
		break;
	case ERROR_ROUTING:
		return "(Routing)";
		break;
	}

	return "";
}

void CStatisticsLog::OnButtonFlush() 
{
	m_lstLog.DeleteAllItems();	
}
