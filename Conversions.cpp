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
#include "Packet.h"
#include "Conversions.h"

// Get rid of ugly warnings
#pragma warning (disable : 4786)

CString DWrdtoStr(UINT in)
{
	char buff[16];

	::sprintf (buff, "%u", in);

	return buff;
}

CString IPtoStr(IP in)
{
	char* buffer;
	buffer = inet_ntoa(*(in_addr *)&in);

	return buffer;
}

IP StrtoIP(CString in)
{
	IP out;

	out.S_addr = inet_addr(in);		// standard WinSock2 function

	return out;	
}

CString ExIPtoStr(ExIP in)
{
	char buffer[20];
	::sprintf(buffer, "%d.%d.%d.%d:", in.a, in.b, in.c, in.d);

	CString out = buffer;

	out.Replace("-1", "*");
	if(in.mode == DENY)
		out.Replace(":", ":Deny");
	else
		out.Replace(":", ":Allow");

	return out;
}

ExIP StrtoExIP(CString in)
{
	char rawMode[8] = "";

	in.Replace("*", "-1");

	int  a = 0, b = 0, c = 0, d = 0;

	::sscanf ((LPCTSTR) in, "%d.%d.%d.%d:%s", &a, &b, &c, &d, rawMode);

	ExIP out;
	out.a = a;	out.b = b;	out.c = c;	out.d = d;
	
	CString mode(rawMode);

	mode.MakeUpper();
	if(mode == "ALLOW")
		out.mode = ALLOW;
	else
		out.mode = DENY;

	return out;
}

CString CommaIze(CString in)
{
	if (in.GetLength() > 3)
		return CommaIze(in.Left(in.GetLength() - 3)) + "," + in.Right(3);
	else
		return in;
}

CString InsertDecimal(double dNumber)
{
	if(dNumber <= 0.00)
		return "0.00";

	int    decimal, sign;
	//char*  buffer;

	CString strNumber( _fcvt( dNumber, 2, &decimal, &sign));
	
	if(decimal == 0)
		strNumber.Insert(0, "0.");
	else if(decimal == -1)
		strNumber.Insert(0, "0.0");
	else if(decimal < -1)
		return "0.00";
	else
		strNumber.Insert(decimal, '.');

	return strNumber;
}

CString GetPercentage(DWORD dWhole, DWORD dPart)
{
	CString result = "0.00";

	if(dPart > dWhole)
		dPart = dWhole;

	if(dWhole)
	{
		result = DWrdtoStr(dPart * 10000 / dWhole);

		if(result.GetLength() > 2)
			result.Insert( result.GetLength() - 2, ".");
		else
		{
			switch(result.GetLength())
			{
			case 2:
				result.Insert(0, "0.");
				break;
			case 1:
				result.Insert(0, "0.0");
				break;
			default:
				result = "0.00";
				break;
			}
		}
	}

	return result + " %";
}

CString IncrementName(CString FileName)
{
	CString Front = FileName;
				
	int dotpos = FileName.ReverseFind('.');
	if(dotpos != -1)
		Front = FileName.Left(dotpos);
	
	int copy = 1;
	int spacepos = Front.ReverseFind(' ');

	if(spacepos != -1)
	{
		copy = atoi( Front.Right(Front.GetLength() - spacepos - 1));
		
		if(copy == 0)
			Front += " 1";
		else
		{
			copy++;
			Front = Front.Left(spacepos) + " " + DWrdtoStr(copy);
		}
	}
	else
		Front += " 1";

	CString NewFile = Front;
	
	if(dotpos != -1)
		Front += FileName.Right( FileName.GetLength() - dotpos);

	return Front;
}

DWORD GetSpeedinBytes(CString Speed)
// The protocol is messed, bytes are bits 
{
	if(Speed == "Cellular Modem")
		return 0;
	if(Speed == "14.4 Modem")
		return 14;
	if(Speed == "28.8 Modem")
		return 28;
	if(Speed == "56K Modem")
		return 53;
	if(Speed == "ISDN")
		return 128;
	if(Speed == "Cable")
		return 384;
	if(Speed == "DSL")
		return 768;
	if(Speed == "T1")
		return 1500;
	if(Speed == "T3 (or Greater)")
		return 45000;
	
	return atol(Speed);
}

CString GetSpeedString(DWORD dwSpeed)
// The protocol is messed, bytes are bits 
{
	if(dwSpeed >= 45000)
		return "T3 (or Greater)";
	if(dwSpeed >= 1500)
		return "T1";
	if(dwSpeed >= 768)
		return "DSL";
	if(dwSpeed >= 384)
		return "Cable";
	if(dwSpeed >= 128)
		return "ISDN";
	if(dwSpeed >= 53)
		return "56K Modem";
	if(dwSpeed >= 28)
		return "28.8 Modem";
	if(dwSpeed >= 14)
		return "14.4 Modem";
	else
		return "Cellular Modem";
}

bool ValidVendor(CString VendorID)
{
	if(GetVendor(VendorID) == "")
		return false;
	
	return true;
}

CString GetVendor(CString VendorID)
{
	VendorID.MakeUpper();

	if(VendorID == "BEAR") return "BearShare";

	if(VendorID == "CULT") return "Cultiv8r";
	
	if(VendorID == "GNOT") return "Gnotella";
	
	if(VendorID == "GNUC") return "Gnucleus";

	if(VendorID == "GNUT") return "Gnut";

	if(VendorID == "GTKG") return "Gtk-Gnutella";

	if(VendorID == "HSLG") return "Hagelslag";
	
	if(VendorID == "LIME") return "LimeWire";
	
	if(VendorID == "MACT") return "Mactella";
	
	if(VendorID == "MNAP") return "MyNapster";

	if(VendorID == "MRPH" || VendorID == "MMMM") return "Morpheus";
	
	if(VendorID == "NAPS") return "NapShare";

	if(VendorID == "OCFG") return "OCFolders";

	if(VendorID == "RAZA") return "Shareaza";

	if(VendorID == "SNUT") return "SwapNut";

	if(VendorID == "TOAD") return "ToadNode";

	return VendorID;
}

// Converts a string representation to a CTime
CTime	StrToCTime(CString& str)
{
	CTime temp(0);
	
	// 0         1         2 
	// 0123456789012345678901
	// YYYY-MM-DDTHH:MM+HH:MM
	// or
	// YYYY-MM-DDTHH:MMZ

	if (str[4] != '-'           || str[7] != '-' || 
		tolower(str[10]) != 't' || str[13] != ':')
	{
		return temp;
	}

	int Year  = atoi(str.Mid(0,4));
	int Month = atoi(str.Mid(5,2));
	int Day   = atoi(str.Mid(8,2));
	int Hour  = atoi(str.Mid(11,2));
	int Min   = atoi(str.Mid(14,2));

	// Check for valid values
	CTime CurrentTime = CTime::GetCurrentTime();

	if(Year  < 1970 || Year  > CurrentTime.GetYear() ||
	   Month < 1    || Month > 12 ||
	   Day   < 1    || Day   > 31 )
	{
	   return temp;
	}

	temp = CTime(Year, Month, Day, Hour, Min, 0, 0);

	if (tolower(str[16]) == 'z')
	{
		return temp;
	}

	int tzh   = atoi(str.Mid(16,3));
	int tzm   = atoi(str.Mid(20,2));


	return temp;
}

// Convert Ctime to String
CString CTimeToStr(CTime& time)
{
	// The Format as per http://www.w3.org/TR/NOTE-datetime 
	// YYYY-MM-DDTHH:MMTZD
	CString TimeStr;

	TimeStr = time.Format("%Y-%m-%dT%H:%MZ");

	// CTime doesn't store time zone so have to convert all times to UTC
	//	TimeStr += GetTimeZoneStr();

	return TimeStr;
}

CString GetTimeZoneStr(void)
{
	CString TZStr;

	int TZHours = abs(_timezone) / (60 * 60);
	int TZMins = ((abs(_timezone) / 60) - (TZHours * 60));

	TZStr.Format("%c%2.2d:%2.2d", (_timezone < 0 ? '-' : '+'), TZHours, TZMins);

	return TZStr;
}

CTimeSpan LocalTimeZone()
{
	int TZHours = _timezone / (60 * 60);
	int TZMins = ((abs(_timezone) / 60) - (abs(TZHours) * 60));

	return CTimeSpan(0, TZHours, TZMins, 0);
}

static byte base16Chars[17] = "0123456789ABCDEF";

#define BASE16_LOOKUP_MAX 23
static byte base16Lookup[BASE16_LOOKUP_MAX][2] =
{
    { '0', 0x0 },
    { '1', 0x1 },
    { '2', 0x2 },
    { '3', 0x3 },
    { '4', 0x4 },
    { '5', 0x5 },
    { '6', 0x6 },
    { '7', 0x7 },
    { '8', 0x8 },
    { '9', 0x9 },
	{ ':', 0x9 },
    { ';', 0x9 },
    { '<', 0x9 },
    { '=', 0x9 },
    { '>', 0x9 },
    { '?', 0x9 },
    { '@', 0x9 },
    { 'A', 0xA },
    { 'B', 0xB },
    { 'C', 0xC },
    { 'D', 0xD },
    { 'E', 0xE },
    { 'F', 0xF }
};

CString EncodeBase16(byte* buffer, unsigned int bufLen)
{
	CString Base16Buff;

	for(int i = 0; i < bufLen; i++)
	{
		Base16Buff += base16Chars[buffer[i] >> 4];
		Base16Buff += base16Chars[buffer[i] & 0xf];
	}

    return Base16Buff;
}

