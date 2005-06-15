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
#include "gnucleus.h"
#include "GnucleusDoc.h"

#include "AutDownload.h"

#include "TransfersDownBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransfersDownBar

CTransfersDownBar::CTransfersDownBar()
{
	m_autDownload = NULL;
}

CTransfersDownBar::~CTransfersDownBar()
{
}


BEGIN_MESSAGE_MAP(CTransfersDownBar, CStatic)
	//{{AFX_MSG_MAP(CTransfersDownBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransfersDownBar message handlers

void CTransfersDownBar::OnPaint() 
{
	if(!m_autDownload)
		return;

	//CPaintDC dc(this); // device context for painting 

	int offset = 0; 
	CBrush* pOldBrush; 
	CBrush Brush; 
	CPaintDC BarDC(this); 
	CRect BarSize; 

	GetClientRect(&BarSize); 


	// Fill rect 
	Brush.CreateSolidBrush(RGB(255, 255, 255)); 
	pOldBrush = (CBrush *) BarDC.SelectObject(&Brush); 

	BarDC.FillRect(&BarSize, &Brush); 

	BarDC.SelectObject(pOldBrush); 
	Brush.DeleteObject(); 

	// Display begining of file 
	Brush.CreateSolidBrush(RGB(0, 255, 0)); 
	pOldBrush = (CBrush *) BarDC.SelectObject(&Brush); 

	CRect ColorBar; 

	//ColorBar.top = 0; 
	//ColorBar.left = 0; 
	//ColorBar.bottom = BarSize.bottom; 
	//ColorBar.right = BarSize.right;  // * (float(Download->m_BytesCompleted) / float(Download->m_FileLength)); 

	//BarDC.FillRect(&ColorBar, &Brush); 

	BarDC.SelectObject(pOldBrush); 
	Brush.DeleteObject(); 

	std::vector<int> ChunkList;

	// Get new Source IDs
	VARIANT var = m_autDownload->GetChunkIDs(m_DownloadID);
	SAFEARRAY* psa = var.parray;

	int* nArray;
	SafeArrayAccessData(psa, reinterpret_cast<void**> (&nArray));

	for(int i = 0; i < psa->rgsabound->cElements; i++)
		ChunkList.push_back(nArray[i]);

	SafeArrayUnaccessData(psa);
	VariantClear(&var);

	int ChunkID;

	// Display chunks 
	for(int i = 0; i < ChunkList.size(); i++) 
	{ 
		ChunkID = ChunkList[i];

		// Draw completed 
		Brush.CreateSolidBrush( m_autDownload->GetChunkFamily(m_DownloadID, ChunkID) /*RGB(0, 255, 0)*/ ); 
		pOldBrush = (CBrush *) BarDC.SelectObject(&Brush); 

		uint64 chunkStart = m_pDoc->m_RunningXP ? m_autDownload->GetChunkStart2(m_DownloadID, ChunkID) : m_autDownload->GetChunkStart(m_DownloadID, ChunkID);
		uint64 fileLength = m_pDoc->m_RunningXP ? m_autDownload->GetFileLength2(m_DownloadID) : m_autDownload->GetFileLength(m_DownloadID);

		int offset = (BarSize.right-1) * (float(chunkStart) / float(fileLength)); 

		ColorBar.top = 0; 
		ColorBar.left = offset; 
		ColorBar.bottom = BarSize.bottom; 
		ColorBar.right = offset + BarSize.right * (float(m_autDownload->GetChunkCompleted(m_DownloadID, ChunkID)) / float(fileLength)); 

		BarDC.FillRect(&ColorBar, &Brush); 

		BarDC.SelectObject(pOldBrush); 
		Brush.DeleteObject(); 
	} 

	// Drawing dividers
	for(i = 0; i < ChunkList.size(); i++) 
	{ 
		ChunkID = ChunkList[i];

		// Start Pos dividers
		offset = (BarSize.right-1) * (float(m_autDownload->GetChunkStart2(m_DownloadID, ChunkID)) / float(m_autDownload->GetFileLength2(m_DownloadID))); 

		BarDC.MoveTo(offset, 0); 
		BarDC.LineTo(offset, BarSize.bottom / 2); 
		BarDC.MoveTo(0, 0); 

		// End Pos dividers
		uint64 chunkStart = m_pDoc->m_RunningXP ? m_autDownload->GetChunkStart2(m_DownloadID, ChunkID) : m_autDownload->GetChunkStart(m_DownloadID, ChunkID);
		uint64 fileLength = m_pDoc->m_RunningXP ? m_autDownload->GetFileLength2(m_DownloadID) : m_autDownload->GetFileLength(m_DownloadID);

		offset = (BarSize.right-1) * (float(chunkStart + m_autDownload->GetChunkSize(m_DownloadID, ChunkID)) / float(fileLength)); 

		BarDC.MoveTo(offset, BarSize.bottom / 2); 
		BarDC.LineTo(offset, BarSize.bottom); 
		BarDC.MoveTo(0, 0); 
	} 

	//offset = (BarSize.right-1) * (float(Download->m_StopPos) / float(Download->m_FileLength)); 

	//BarDC.MoveTo(offset, BarSize.bottom / 2); 
	//BarDC.LineTo(offset, BarSize.bottom); 
	BarDC.MoveTo(0, 0); 


	// Draw Border 
	Brush.CreateSolidBrush(RGB(0, 0, 0)); 
	pOldBrush = (CBrush *) BarDC.SelectObject(&Brush); 

	BarDC.FrameRect(&BarSize, &Brush); 

	BarDC.SelectObject(pOldBrush); 
	Brush.DeleteObject(); 
}
