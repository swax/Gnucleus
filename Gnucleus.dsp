# Microsoft Developer Studio Project File - Name="Gnucleus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Gnucleus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Gnucleus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Gnucleus.mak" CFG="Gnucleus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Gnucleus - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Gnucleus - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Gnucleus - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /opt:ref,icf /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /pdb:none /machine:I386
# SUBTRACT LINK32 /map /debug

!ELSEIF  "$(CFG)" == "Gnucleus - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "Gnucleus - Win32 Release"
# Name "Gnucleus - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Browser .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewBrowser.cpp
# End Source File
# End Group
# Begin Group "Chat .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChatControl.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatIdentd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatServer.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameChat.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewChat.cpp
# End Source File
# End Group
# Begin Group "Common .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoLoadDll.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnClickMixIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Conversions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgResizer.cpp
# End Source File
# Begin Source File

SOURCE=.\FileLock.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MruComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MruStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\SortedHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# End Group
# Begin Group "Connect .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConnectAdvanced.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\NetSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\NetSetupNew.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewConnect.cpp
# End Source File
# End Group
# Begin Group "Network .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GnuCache.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuControl.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuDownloadShell.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuEvolve.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuHttp.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuNode.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuRouting.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuSock.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuUpload.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuUploadShell.cpp
# End Source File
# End Group
# Begin Group "Preferences .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PrefBandwidth.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefChat.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefConnectScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefConnectServers.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefLocalFirewall.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefLocalNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefPlugins.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefSearchBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefSearchScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefShare.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefTransfer.cpp
# End Source File
# End Group
# Begin Group "Search .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchAdvanced.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchResults.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchResultsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSearch.cpp
# End Source File
# End Group
# Begin Group "Share .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameShare.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuAltLoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuFileHash.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuShare.cpp
# End Source File
# Begin Source File

SOURCE=.\GnuWordHash.cpp
# End Source File
# Begin Source File

SOURCE=.\Sha1.cpp
# End Source File
# Begin Source File

SOURCE=.\ShareExDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewShare.cpp
# End Source File
# End Group
# Begin Group "Statistics .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameStatistics.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsBandwidth.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsError.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsLog.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsMain.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsPackets.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewStatistics.cpp
# End Source File
# End Group
# Begin Group "Tools .cpp"

# PROP Default_Filter ""
# Begin Group "DriveInfo .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorSwatch.cpp
# End Source File
# Begin Source File

SOURCE=.\DiskUsageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderIterator.cpp
# End Source File
# Begin Source File

SOURCE=.\PieChartButton.cpp
# End Source File
# Begin Source File

SOURCE=.\PieChartButton.h
# End Source File
# End Group
# Begin Group "WindowList .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GnuMdiChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowListDlg.cpp
# End Source File
# End Group
# Begin Group "Mapping .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BuildMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DistMapControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DistMapSock.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Evolve.cpp
# End Source File
# Begin Source File

SOURCE=.\SchedulerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WebCacheDlg.cpp
# End Source File
# End Group
# Begin Group "Transfers .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameTransfers.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersDown.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersDownBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersDownEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersPartial.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersPartialEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersUp.cpp
# End Source File
# Begin Source File

SOURCE=.\TransfersUpEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewTransfers.cpp
# End Source File
# End Group
# Begin Group "zlib .c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# End Source File
# End Group
# Begin Group "Media .cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\media\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\media\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\media\Listen.cpp
# End Source File
# Begin Source File

SOURCE=.\media\MediaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\media\MediaToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\media\Reqsock.cpp
# End Source File
# Begin Source File

SOURCE=.\media\Request.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\FrameMain.cpp
# End Source File
# Begin Source File

SOURCE=.\Gnucleus.cpp
# End Source File
# Begin Source File

SOURCE=.\Gnucleus.rc
# End Source File
# Begin Source File

SOURCE=.\GnucleusDoc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Browser .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameBrowser.h
# End Source File
# Begin Source File

SOURCE=.\ViewBrowser.h
# End Source File
# End Group
# Begin Group "Chat .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChatControl.h
# End Source File
# Begin Source File

SOURCE=.\ChatIdentd.h
# End Source File
# Begin Source File

SOURCE=.\ChatRoom.h
# End Source File
# Begin Source File

SOURCE=.\ChatServer.h
# End Source File
# Begin Source File

SOURCE=.\FrameChat.h
# End Source File
# Begin Source File

SOURCE=.\ViewChat.h
# End Source File
# End Group
# Begin Group "Common .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoLoadDll.h
# End Source File
# Begin Source File

SOURCE=.\ColumnClickMixIn.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\Conversions.h
# End Source File
# Begin Source File

SOURCE=.\DlgResizer.h
# End Source File
# Begin Source File

SOURCE=.\FileLock.h
# End Source File
# Begin Source File

SOURCE=.\FileTypes.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\MruComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MruStorage.h
# End Source File
# Begin Source File

SOURCE=.\Packet.h
# End Source File
# Begin Source File

SOURCE=.\SortedHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StlUtilities.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# End Group
# Begin Group "Connect .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConnectAdvanced.h
# End Source File
# Begin Source File

SOURCE=.\ConnectBasic.h
# End Source File
# Begin Source File

SOURCE=.\ConnectInfo.h
# End Source File
# Begin Source File

SOURCE=.\FrameConnect.h
# End Source File
# Begin Source File

SOURCE=.\NetSetup.h
# End Source File
# Begin Source File

SOURCE=.\NetSetupNew.h
# End Source File
# Begin Source File

SOURCE=.\ViewConnect.h
# End Source File
# End Group
# Begin Group "Network .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GnuCache.h
# End Source File
# Begin Source File

SOURCE=.\GnuControl.h
# End Source File
# Begin Source File

SOURCE=.\GnuDownload.h
# End Source File
# Begin Source File

SOURCE=.\GnuDownloadShell.h
# End Source File
# Begin Source File

SOURCE=.\GnuEvolve.h
# End Source File
# Begin Source File

SOURCE=.\GnuHttp.h
# End Source File
# Begin Source File

SOURCE=.\GnuLocal.h
# End Source File
# Begin Source File

SOURCE=.\GnuNode.h
# End Source File
# Begin Source File

SOURCE=.\GnuRouting.h
# End Source File
# Begin Source File

SOURCE=.\GnuSock.h
# End Source File
# Begin Source File

SOURCE=.\GnuUpload.h
# End Source File
# Begin Source File

SOURCE=.\GnuUploadShell.h
# End Source File
# End Group
# Begin Group "Preferences .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PrefBandwidth.h
# End Source File
# Begin Source File

SOURCE=.\PrefChat.h
# End Source File
# Begin Source File

SOURCE=.\PrefConnect.h
# End Source File
# Begin Source File

SOURCE=.\PrefConnectScreen.h
# End Source File
# Begin Source File

SOURCE=.\PrefConnectServers.h
# End Source File
# Begin Source File

SOURCE=.\PrefDialog.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\PrefLocal.h
# End Source File
# Begin Source File

SOURCE=.\PrefLocalFirewall.h
# End Source File
# Begin Source File

SOURCE=.\PrefLocalNetwork.h
# End Source File
# Begin Source File

SOURCE=.\PrefPlugins.h
# End Source File
# Begin Source File

SOURCE=.\PrefSearch.h
# End Source File
# Begin Source File

SOURCE=.\PrefSearchBlock.h
# End Source File
# Begin Source File

SOURCE=.\PrefSearchScreen.h
# End Source File
# Begin Source File

SOURCE=.\PrefShare.h
# End Source File
# Begin Source File

SOURCE=.\PrefTransfer.h
# End Source File
# End Group
# Begin Group "Search .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameSearch.h
# End Source File
# Begin Source File

SOURCE=.\SearchAdvanced.h
# End Source File
# Begin Source File

SOURCE=.\SearchInfo.h
# End Source File
# Begin Source File

SOURCE=.\SearchResults.h
# End Source File
# Begin Source File

SOURCE=.\SearchResultsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SearchToolbar.h
# End Source File
# Begin Source File

SOURCE=.\ViewSearch.h
# End Source File
# End Group
# Begin Group "Share .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameShare.h
# End Source File
# Begin Source File

SOURCE=.\GnuAltLoc.h
# End Source File
# Begin Source File

SOURCE=.\GnuFileHash.h
# End Source File
# Begin Source File

SOURCE=.\GnuShare.h
# End Source File
# Begin Source File

SOURCE=.\GnuWordHash.h
# End Source File
# Begin Source File

SOURCE=.\Sha1.h
# End Source File
# Begin Source File

SOURCE=.\ShareExDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewShare.h
# End Source File
# End Group
# Begin Group "Statistics .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameStatistics.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsBandwidth.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsDistribution.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsError.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsGraph.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsLog.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsMain.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsMemory.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsPackets.h
# End Source File
# Begin Source File

SOURCE=.\ViewStatistics.h
# End Source File
# End Group
# Begin Group "Tools .h"

# PROP Default_Filter ""
# Begin Group "DriveInfo.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorSwatch.h
# End Source File
# Begin Source File

SOURCE=.\DiskUsageDlg.h
# End Source File
# Begin Source File

SOURCE=.\DriveInfo.h
# End Source File
# Begin Source File

SOURCE=.\FolderIterator.h
# End Source File
# Begin Source File

SOURCE=.\GdiUtil.h
# End Source File
# End Group
# Begin Group "WindowList .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GnuMdiChildWnd.h
# End Source File
# Begin Source File

SOURCE=.\PieChartButton.h
# End Source File
# Begin Source File

SOURCE=.\WindowListDlg.h
# End Source File
# End Group
# Begin Group "Mapping .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BuildMap.h
# End Source File
# Begin Source File

SOURCE=.\DistMapControl.h
# End Source File
# Begin Source File

SOURCE=.\DistMapSock.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Evolve.h
# End Source File
# Begin Source File

SOURCE=.\SchedulerDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebCacheDlg.h
# End Source File
# End Group
# Begin Group "Transfers .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FrameTransfers.h
# End Source File
# Begin Source File

SOURCE=.\TransfersDown.h
# End Source File
# Begin Source File

SOURCE=.\TransfersDownBar.h
# End Source File
# Begin Source File

SOURCE=.\TransfersDownEx.h
# End Source File
# Begin Source File

SOURCE=.\TransfersPartial.h
# End Source File
# Begin Source File

SOURCE=.\TransfersPartialEx.h
# End Source File
# Begin Source File

SOURCE=.\TransfersUp.h
# End Source File
# Begin Source File

SOURCE=.\TransfersUpEx.h
# End Source File
# Begin Source File

SOURCE=.\ViewTransfers.h
# End Source File
# End Group
# Begin Group "zlib .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "Media .h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\media\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\media\Label.h
# End Source File
# Begin Source File

SOURCE=.\media\Listen.h
# End Source File
# Begin Source File

SOURCE=.\media\MediaDlg.h
# End Source File
# Begin Source File

SOURCE=.\media\MediaToolBar.h
# End Source File
# Begin Source File

SOURCE=.\media\Reqsock.h
# End Source File
# Begin Source File

SOURCE=.\media\Request.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\FrameMain.h
# End Source File
# Begin Source File

SOURCE=.\Gnucleus.h
# End Source File
# Begin Source File

SOURCE=.\GnucleusDoc.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Blank.ico
# End Source File
# Begin Source File

SOURCE=.\res\Broadcasted.ico
# End Source File
# Begin Source File

SOURCE=.\res\Browser.ico
# End Source File
# Begin Source File

SOURCE=.\res\chat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Chat.ico
# End Source File
# Begin Source File

SOURCE=.\res\ChatBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\connect.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Connect.ico
# End Source File
# Begin Source File

SOURCE=.\res\Connect_Advanced.ico
# End Source File
# Begin Source File

SOURCE=.\res\Connect_Basic.ico
# End Source File
# Begin Source File

SOURCE=.\res\Disconnect.ico
# End Source File
# Begin Source File

SOURCE=.\res\Gnucleus.ico
# End Source File
# Begin Source File

SOURCE=.\res\Gnucleus.rc2
# End Source File
# Begin Source File

SOURCE=.\res\help.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_gobutton.ico
# End Source File
# Begin Source File

SOURCE=.\res\Incoming.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo_black.bmp
# End Source File
# Begin Source File

SOURCE=.\mdi.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mfr.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mfrover.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mpause.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mpauseover.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mplay.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mplayover.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mrw.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mrwover.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mstop.ico
# End Source File
# Begin Source File

SOURCE=.\media\res\mstopover.ico
# End Source File
# Begin Source File

SOURCE=.\res\ResultCompleted.ico
# End Source File
# Begin Source File

SOURCE=.\res\ResultDownloading.ico
# End Source File
# Begin Source File

SOURCE=.\res\ResultNoHosts.ico
# End Source File
# Begin Source File

SOURCE=.\res\ResultTrying.ico
# End Source File
# Begin Source File

SOURCE=.\res\Routed.ico
# End Source File
# Begin Source File

SOURCE=.\res\search.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Search.ico
# End Source File
# Begin Source File

SOURCE=.\res\Search_Advanced.ico
# End Source File
# Begin Source File

SOURCE=.\res\search_button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Search_Results.ico
# End Source File
# Begin Source File

SOURCE=.\res\SearchDownload.ico
# End Source File
# Begin Source File

SOURCE=.\res\SearchDownloading.ico
# End Source File
# Begin Source File

SOURCE=.\res\SearchNoHosts.ico
# End Source File
# Begin Source File

SOURCE=.\res\SearchTrying.ico
# End Source File
# Begin Source File

SOURCE=.\res\settings.ico
# End Source File
# Begin Source File

SOURCE=.\res\share.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Share.ico
# End Source File
# Begin Source File

SOURCE=.\res\Statistics.ico
# End Source File
# Begin Source File

SOURCE=.\res\SurfBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\transfers.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Transfers.ico
# End Source File
# Begin Source File

SOURCE=.\res\Transfers_Down.ico
# End Source File
# Begin Source File

SOURCE=.\res\Transfers_Partial.ico
# End Source File
# Begin Source File

SOURCE=.\res\Transfers_Up.ico
# End Source File
# Begin Source File

SOURCE=.\res\Trashed.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrayLanOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrayLanOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrayOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrayOn.ico
# End Source File
# Begin Source File

SOURCE=".\res\WPI Logo.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChangeLog.txt
# End Source File
# Begin Source File

SOURCE=.\Chat.txt
# End Source File
# Begin Source File

SOURCE=.\gnucleus.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\GPL.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ToDo.txt
# End Source File
# End Target
# End Project
