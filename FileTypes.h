#ifndef __FILETYPES_H__
#define __FILETYPES_H__

// NOTE: See FileTypes.cpp for info about using the system image list
//       instead of creating your own. It's faster and uses less
//       memory, but there are restritions.

// NOTE: Your code or'd ILC_COLOR8 with TRUE when you created the
//       image list. I don't know why you did this, but I just used
//		 (ILC_COLOR8 | ILC_MASK). See the implementation of 
//       CSharedImageList::EnsureCreated for more information.

// For compatability with your code...
#define GetIconDesc GetIconDescFromName

int GetIconIndexFromName(const CString &filename);
CString GetIconDescFromName(const CString &filename);
CImageList * GetSharedImageList();

#endif