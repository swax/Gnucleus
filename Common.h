#if !defined(INLCUDE_ICON_H)
#define INLCUDE_ICON_H

#if _MSC_VER > 1000
#pragma once
#endif 


DWORD	AssignThreadToCPU(CWinThread *pThread, DWORD cpuNumber);
CString ParseString( CString &Str, char delim = ',');

CTime	StrToCTime(CString& str);
CString CTimetoStr(CTime& time);

CString GuidtoStr(GUID inGuid);
GUID    StrtoGuid(CString strGuid);

CTimeSpan LocalTimeZone();


struct FullIcon
{
	int Index;
	CString Type;
};

struct ChatNetwork
{
	CString Name;

	std::vector<CString> Servers;
};

struct ChatLine
{
	CString  Text;
	COLORREF Color;
};




#endif