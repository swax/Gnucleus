#if !defined(CONVERSIONS_H)
#define CONVERSIONS_H


#include "Packet.h"


#if _MSC_VER > 1000
#pragma once
#endif 


CString DWrdtoStr(UINT);     // Convert DWORD to a CString

CString IPtoStr(IP);		   // Convert IP to CString
IP      StrtoIP(CString in);   // Convert CString to an IP
ExIP    StrtoExIP(CString in); // String to extended IP
CString ExIPtoStr(ExIP in);	   // ExtendedIP to String	

CString CommaIze(CString in); // Add commas to big numbers
CString InsertDecimal(double);
CString GetPercentage(DWORD , DWORD );
CString IncrementName(CString);

DWORD GetSpeedinBytes(CString Speed);
CString GetSpeedString(DWORD dwSpeed);

bool    ValidVendor(CString VendorID);
CString GetVendor(CString VendorID);

CTime	StrToCTime(CString& str);	// Converts a string representation to a CTime
CString CTimeToStr(CTime& time);	// reverse of above
CString GetTimeZoneStr(void);

#endif