#ifndef _GDI_UTIL_H
#define _GDI_UTIL_H


class PSaveGdiContext 
{
public:
	enum SaveWhat{ 
		eSavePen       = 0x1, 
		eSaveBrush     = 0x2, 
		eSaveBkColor   = 0x4, 
		eSaveTextColor = 0x8,
		eSaveAll       = 0xff};
	PSaveGdiContext(CDC* pDc, SaveWhat what = eSaveAll);
	~PSaveGdiContext();


protected:
	SaveWhat m_WhatIsSaved;
	CDC*     m_pDc;
	CPen*    m_OldPen;
	CBrush*  m_OldBrush;
	COLORREF m_OldBkColor;
	COLORREF m_OldTextColor;
};


// Was CAutoSelectObject
class CAutoSelectObject
{
public:
	CAutoSelectObject(HDC hDc, HGDIOBJ hobj, bool bCheck = true)
	{
		SetGdiObject(hDc, hobj, bCheck);
	}

	CAutoSelectObject(CDC* pDc, int index, bool bCheck = false)
	{
		m_DcType = epDc;
		m_pDc = pDc;
		m_GdiType = eStockObject;
		m_pOldGdi = (CGdiObject*)m_pDc->SelectStockObject(index);
		#ifdef _DEBUG
			if (bCheck)
			{
				ASSERT(m_pOldGdi != NULL);
			}
			m_bCheck = bCheck;
		#else
				bCheck;
		#endif
	}

	CAutoSelectObject(CDC* pDc, CGdiObject* pObj, bool bCheck = true)
	{
		SetGdiObject(pDc, (CGdiObject*)pObj, bCheck);
	}

	CAutoSelectObject(CDC* pDc, CBrush* pObj, bool bCheck = true)
	{
		SetGdiObject(pDc, (CGdiObject*)pObj, bCheck);
	}

	CAutoSelectObject(CDC* pDc, CPen* pObj, bool bCheck = true)
	{
		SetGdiObject(pDc, (CGdiObject*)pObj, bCheck);
	}

	CAutoSelectObject(CDC* pDc, CBitmap* pObj, bool bCheck = true)
	{
		SetGdiObject(pDc, (CGdiObject*)pObj, bCheck);
	}

	CAutoSelectObject(CDC* pDc, CFont* pObj, bool bCheck = true)
	{
		SetGdiObject(pDc, (CGdiObject*)pObj, bCheck);
	}

	CAutoSelectObject(HDC hDc, HBRUSH hBrush, bool bCheck = true)
	{
		SetGdiObject(hDc, (HGDIOBJ)hBrush, bCheck);
	}

	CAutoSelectObject(HDC hDc, HPEN hPen, bool bCheck = true)
	{
		SetGdiObject(hDc, (HGDIOBJ)hPen, bCheck);
	}

	CAutoSelectObject(HDC hDc, HBITMAP hBitmap, bool bCheck = true)
	{
		SetGdiObject(hDc, (HGDIOBJ)hBitmap, bCheck);
	}

	CAutoSelectObject(HDC hDc, HFONT hFont, bool bCheck = true)
	{
		SetGdiObject(hDc, (HGDIOBJ)hFont, bCheck);
	}

	~CAutoSelectObject()
	{
		switch (m_DcType)
		{
		case epDc:
			if (m_GdiType != eStockObject)
			{
				CGdiObject* pVerify = m_pDc->SelectObject(m_pOldGdi);
#ifdef _DEBUG
				if (m_bCheck)
				{
					ASSERT(pVerify == m_pbCheckGdi);
				}
#endif
			}
			break;
		case ehDc:
			{
				HGDIOBJ verify = ::SelectObject(m_hDc, m_hOldObj);
#ifdef _DEBUG
				if (m_bCheck)
				{
					ASSERT(verify == m_hbCheckObj);
				}
#endif
			}
			break;
		default:
			ASSERT(0);
		}
	}

	enum gdiType {eGdiHandle, eStockObject, eCGdiObj};
	enum dcType { epDc, ehDc };


protected:
	void SetGdiObject(CDC* pDc, CGdiObject* pObj, bool bCheck)
	{
		m_DcType = epDc;
		m_pDc = pDc;
		m_GdiType = eCGdiObj;
		m_pOldGdi = (CGdiObject*)m_pDc->SelectObject(pObj);
#ifdef _DEBUG
		m_bCheck = bCheck;
		m_pbCheckGdi = pObj;
#else
		bCheck;
#endif
	}

	void SetGdiObject(HDC hDc, HGDIOBJ hobj, bool bCheck)
	{
		m_DcType = ehDc;
		m_hDc = hDc;
		m_GdiType = eGdiHandle;
		m_hOldObj = ::SelectObject(hDc, hobj);
		
#ifdef _DEBUG
		if (bCheck)
			ASSERT(m_hOldObj != NULL);
		m_bCheck = bCheck;
		m_hbCheckObj = hobj;
#else
		bCheck;
#endif
	}
	dcType m_DcType;
	union 
	{ 
		HDC m_hDc;
		CDC* m_pDc;
	};

	gdiType m_GdiType;
	union 
	{
		HGDIOBJ m_hOldObj;
		CGdiObject* m_pOldGdi;
	};
#ifdef _DEBUG
	bool m_bCheck;
	union {
		HGDIOBJ m_hbCheckObj;
		CGdiObject* m_pbCheckGdi;
	};
#endif
};


// ------------------------------------------------
class AutoDeleteDc : public CDC
{
public:
	AutoDeleteDc(CDC* pDc)
	{
		CreateCompatibleDC(pDc);
	}
	~AutoDeleteDc()
	{
	    VERIFY(DeleteDC());
	}
};


class CAutoCreateCompatible
{
public:
	CAutoCreateCompatible(CDC* pDc)
	{
		m_eType = eDc;
		m_pDc = new CDC; 
		ASSERT(m_pDc);
		m_pDc->CreateCompatibleDC(pDc);
	}

	CAutoCreateCompatible(HDC hDc)
	{
		m_eType = eHDc;
		m_hDc = ::CreateCompatibleDC(hDc);
		ASSERT(m_hDc);
	}

	CAutoCreateCompatible(HDC hDc, int w, int h)
	{
		m_eType = eHBitmap;
		m_hBitmap  = ::CreateCompatibleBitmap(hDc, w, h);
		ASSERT(m_hBitmap);
	}

	~CAutoCreateCompatible()
	{
		switch (m_eType)
		{
		case eDc:
			m_pDc->DeleteDC();
			delete m_pDc;
			m_pDc = NULL;
			break;
		case eHDc:
			::DeleteDC(m_hDc);
			break;
		case eHBitmap:
			::DeleteObject(m_hBitmap);
			break;
		default:
			ASSERT(0);
		}
	}
	operator HDC() { ASSERT(m_eType == eHDc); return m_hDc; }
	operator CDC*() { ASSERT(m_eType == eDc); return m_pDc; }
	operator HBITMAP() { ASSERT(m_eType == eHBitmap); return m_hBitmap; }


private:
	enum objectType {eDc, eHDc, eHBitmap};
	objectType m_eType;

	union
	{
		CDC* m_pDc;
		HDC m_hDc;
		HBITMAP m_hBitmap;
	};
};


// ------------------------------------------------
// Was just AutoDeleteHDc
class CAutoDeleteHDc
{
public:
	CAutoDeleteHDc(HDC dc)
		: m_hDc(dc)
	{
		ASSERT(m_hDc);
	}
	~CAutoDeleteHDc()
	{
	    VERIFY(::DeleteDC(m_hDc));
	}
	operator HDC() { return m_hDc; }


protected:
	HDC m_hDc;
};


class CAutoDeleteObject
{
public:
	CAutoDeleteObject(HGDIOBJ hobj)
		: m_hObj(hobj)
	{
		ASSERT(m_hObj);
	}

	~CAutoDeleteObject() { VERIFY(::DeleteObject(m_hObj)); }

	operator HGDIOBJ() { return m_hObj; }
protected:
	HGDIOBJ m_hObj;
};


class CAutoSelectPalette
{
public:
	CAutoSelectPalette(HDC hDc, HPALETTE hPal, bool bFirst, bool bSecond)
	{
		m_bSecond = bSecond;
		m_hOldPal = ::SelectPalette(hDc, hPal, bFirst ? TRUE : FALSE);

		ASSERT(m_hOldPal);
	}

	~CAutoSelectPalette()
	{
		if (m_hOldPal)
		{
			::SelectPalette(m_hDc, m_hOldPal, m_bSecond ? TRUE : FALSE);
		}
	}


private:
	HDC m_hDc;
	HPALETTE m_hOldPal;
	bool m_bSecond;
};


// Gives you the amount you need to move the small rect to get it inside the big rect.
// Normalizes the rects for you.
CPoint MoveRectInsideLargeRect(CRect smallRect, CRect largerRect);


#endif
