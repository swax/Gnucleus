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
#include "FileTypes.h"
#include <map>	// I don't like this because the MS STL is buggy.
				// The SGI STL is better but I can't get it to work
				// with MSVC.

// Uncomment this line to use the system image list instread of our
// own. This will use less memory and is faster, but you MUST be
// sure that any classes which use the image list DO NOT delete it!
// [It's actually very interesting... all your file icons will 
// disappear from explorer until you re-boot; trust me!].
// For ListView controls this can be done by using the style
// LVS_SHAREIMAGELISTS. I don't know about other controls but it
// *should* be similar. ;)
//
// Also, do not use the system image list if you need to add your
// own icons to the list. It *may* work, but I wouldn't recommend it.

#define USE_SYSTEM_IMAGE_LIST


// typedef 'file_info' to represent an STL pair<int, CString>. (It's
// just easier this way :P)
// file_info.first	= icon index in image list
// file_info.second	= file description
typedef std::pair<int, CString> file_info;

// Store all file type info here
std::map<CString, file_info> g_mapFileTypes;


// I implement CImageList in this wrapper class so, among other 
// things, I can detach the HIMAGELIST from the object during 
// destruction if USE_SYSTEM_IMAGE_LIST is defined. Otherwise all 
// your icons are erased from memory :P
class CSharedImageList
{
	public:
	CSharedImageList() : m_bAttached(false)
	{
	}

	~CSharedImageList()
	{
		#ifdef USE_SYSTEM_IMAGE_LIST
			if (m_bAttached) m_cImageList.Detach();
		#endif
	}

	// Allow use as pointer
	CImageList * operator-> ()
	{
		EnsureCreated();
		return &m_cImageList;
	}

	// Castable to CImageList * 
	operator CImageList * ()
	{
		EnsureCreated();
		return &m_cImageList;
	}


	private:
	
	// Ensure the image list has been created/attached, depending
	// on wether USE_SYSTEM_IMAGE_LIST is defined.
	void EnsureCreated()
	{
		if (m_bAttached)
			return;

		// If we are using the system image list, just attach it to
		// the CImageList, otherwise create a new one.
		#ifdef USE_SYSTEM_IMAGE_LIST
		
		// Make a dummy call to SHGetFileInfo to get the handle to
		// the system image list
		SHFILEINFO info;
		HIMAGELIST hImageList;
		hImageList = (HIMAGELIST)SHGetFileInfo(".exe", FILE_ATTRIBUTE_NORMAL, &info, sizeof(info), 
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);

		m_cImageList.Attach(hImageList);
		#else

		// Your code or'd ILC_COLOR8 with TRUE, but I have no idea
		// why. I'll leave it commented out for now, but feel free
		// to change it.
		//g_sharedImageList.Create(16, 16, ILC_COLOR8 | TRUE, 0, 10);
		m_cImageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 0, 10);
		#endif

		m_bAttached = true;
	}

	CImageList m_cImageList;
	bool m_bAttached;
} g_sharedImageList;


/*
 * GetSharedImageList:
 *  Returns a pointer to the shared image list.
 */
CImageList * GetSharedImageList()
{
	return g_sharedImageList;
}


/* 
 * GetFileInfoFromExt:
 *  This function used the SHGetFileInfo API call to load the file
 *  icon and description into g_mapFileTypes. It returns an 
 *  iterator to the inserted element.
 */
std::map<CString, file_info>::iterator GetFileInfoFromExt(const CString &fileext)
{
	SHFILEINFO info; // Receives the file information
	HIMAGELIST hImageList;	// The handle to the system image list
							// returned by SHGetFileInfo if 
							// SHGFI_ICON is specified in uFlags

	// The API function SHGetFileInfo can be used to get all sorts
	// of info about a file. Although the first paramater is normally
	// a full file-name, we can pass just the extension if we pass
	// FILE_ATTRIBUTE_NORMAL for dwFileAttributes and set the
	// SHGFI_USEFILEATTRIBUTES flag in uFlags
	
	hImageList = (HIMAGELIST)SHGetFileInfo(fileext, FILE_ATTRIBUTE_NORMAL, &info, sizeof(info), 
			SHGFI_TYPENAME |
			SHGFI_ICON | 
			SHGFI_SMALLICON | 
			SHGFI_USEFILEATTRIBUTES);

	// If we aren't using the system image list, we insert the icon
	// into our image list and set the iIcon member of SHFILEINFO
	// to the index of the new image so we can use it later.
	#ifndef USE_SYSTEM_IMAGE_LIST
	info.iIcon = g_sharedImageList->Add(info.hIcon);
	#endif

	// Finally, add the information into the map for future use
	return g_mapFileTypes.insert(std::pair<CString, file_info>(fileext, file_info(info.iIcon, info.szTypeName))).first;
}

/*
 * GetFileExtFromName:
 *  Helper function to extract the file extension from a file name.
 *  Note that it is OK if ReverseFind() returns -1 (not found) since
 *  if the second argument to Delete() is -1, the entire string is 
 *  deleted, which is what we want anyway.
 */
CString GetFileExtFromName(const CString &filename)
{
	CString fileext(filename);
	int nPeriod = fileext.ReverseFind('.');
	if(nPeriod != -1)
		fileext.Delete(0, nPeriod);
	else
		fileext = "";

	return fileext;
}

/*
 * GetIconIndexFromName:
 *  Function you want #1:
 *  Returns the index of the image in the shared image list
 */
int GetIconIndexFromName(const CString &filename)
{
	// Get the file extension
	CString fileext = GetFileExtFromName(filename);

	// Look for the file info in the map. If it isn't there, then
	// call GetFileInfoFromExt to load it.
	std::map<CString, file_info>::iterator map_iterator;
	map_iterator = g_mapFileTypes.find(fileext);
	if (map_iterator == g_mapFileTypes.end())
		map_iterator = GetFileInfoFromExt(fileext);

	// Return the icon index
	return map_iterator->second.first;
}


/*
 * GetIconDescFromName:
 *  Function you want #2:
 *  Returns the file type description.
 */
CString GetIconDescFromName(const CString &filename)
{
	// Get the file extension
	CString fileext = GetFileExtFromName(filename);

	// Look for the file info in the map. If it isn't there, then
	// call GetFileInfoFromExt to load it.
	std::map<CString, file_info>::iterator map_iterator;
	map_iterator = g_mapFileTypes.find(fileext);
	if (map_iterator == g_mapFileTypes.end())
		map_iterator = GetFileInfoFromExt(fileext);

	// Return the file description
	return map_iterator->second.second;
}

