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
#include "Common.h"


DWORD AssignThreadToCPU(CWinThread *pThread, DWORD cpuNumber)
{
	DWORD dwErr = 0;
	
	// Legal thread pointer is needed ;) and Only Windows NT/2000/XP support multiple CPU
	if(pThread != NULL && GetVersion() < 0x80000000)			 
	{
	
		// Do the simple way if checking the number of CPUs: Get it from the environment ;)
		long lNrOfCpus = 0;
		char *pEnvData = getenv("NUMBER_OF_PROCESSORS");
	
		
		if(pEnvData != NULL) 
			lNrOfCpus = atoi(pEnvData);

		if(lNrOfCpus == 0) 
			lNrOfCpus = 1;


		// If only one Cpu, then forget it ;)
		// Otherwise assign the affinity (Note: We assume there are max 4 Cpus
		if (lNrOfCpus > 1) 
		{
			DWORD dwProcAffinityMask = cpuNumber;
	
			if (!SetThreadAffinityMask(pThread->m_hThread, dwProcAffinityMask)) 
			{
				// Failure will be returned as errorcode ;)
				dwErr = GetLastError();
			}
		}
	}

	return dwErr;
}


CString GuidtoStr(GUID inGuid)
{
	byte* bGuid = (byte*) &inGuid;

	CString strGuid;
	strGuid.Format("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				bGuid[0],  bGuid[1],  bGuid[2],  bGuid[3],  bGuid[4],
				bGuid[5],  bGuid[6],  bGuid[7],  bGuid[8],  bGuid[9],
				bGuid[10], bGuid[11], bGuid[12], bGuid[13], bGuid[14],
				bGuid[15]);
	strGuid.MakeUpper();

	return strGuid;
}

GUID StrtoGuid(CString strGuid)
{
	GUID outGuid = GUID_NULL;
	byte* bGuid = (byte*) &outGuid;

	int nGuid[16];

	sscanf(strGuid, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				&nGuid[0],  &nGuid[1],  &nGuid[2],  &nGuid[3],  &nGuid[4],
				&nGuid[5],  &nGuid[6],  &nGuid[7],  &nGuid[8],  &nGuid[9],
				&nGuid[10], &nGuid[11], &nGuid[12], &nGuid[13], &nGuid[14],
				&nGuid[15]);

	for(int i = 0; i < 16; i++)
		bGuid[i] = (byte) nGuid[i];


	return outGuid;
}

// Return the first string before the delim char
// and remove the string from the main string
// Repeat calls to return all items until empty string is returned
CString ParseString( CString &Str, char delim /* = ',' */)
{
	CString RetStr;

	if (!Str.IsEmpty())
	{
		int delimpos = Str.Find(delim);
        if (delimpos == -1)
        {
			RetStr = Str;
			Str = "";
		}
        else
        {
            RetStr = Str.Left(delimpos);
            Str = Str.Mid(delimpos + 1);
        }
    }
   
    return RetStr;
}

