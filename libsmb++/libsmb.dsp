# Microsoft Developer Studio Project File - Name="libsmb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Xbox Static Library" 0x0b04

CFG=libsmb - Xbox Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libsmb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libsmb.mak" CFG="libsmb - Xbox Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libsmb - Xbox Release" (based on "Xbox Static Library")
!MESSAGE "libsmb - Xbox Debug" (based on "Xbox Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "libsmb - Xbox Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "_XBOX" /D "NDEBUG" /YX /FD /G6 /Ztmp /c
# ADD CPP /nologo /W3 /GX /O2 /Ob0 /D "WIN32" /D "_XBOX" /D "NDEBUG" /YX /FD /G6 /Ztmp /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libsmb - Xbox Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_XBOX" /D "_DEBUG" /YX /FD /G6 /Ztmp /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_XBOX" /D "_DEBUG" /YX /FD /G6 /Ztmp /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\libsmbd.lib"

!ENDIF 

# Begin Target

# Name "libsmb - Xbox Release"
# Name "libsmb - Xbox Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\CharCnv.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DES.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FileManagementInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\IODescriptors.cpp
# End Source File
# Begin Source File

SOURCE=.\src\IOTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NameQueryInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NameServicePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NativeNMB.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NBHostCache.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NetBIOSPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NMBIO.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Resolve.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SambaLink.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SambaNMB.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SessionIO.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SessionPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SMBIO.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SMBPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\types.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XBoxGetHost.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\CharCnv.h
# End Source File
# Begin Source File

SOURCE=.\src\cursesHacks.h
# End Source File
# Begin Source File

SOURCE=.\src\defines.h
# End Source File
# Begin Source File

SOURCE=.\src\DES.h
# End Source File
# Begin Source File

SOURCE=.\src\FileManagementInterface.h
# End Source File
# Begin Source File

SOURCE=.\src\IODescriptors.h
# End Source File
# Begin Source File

SOURCE=.\src\IOTypes.h
# End Source File
# Begin Source File

SOURCE=.\src\NameQueryInterface.h
# End Source File
# Begin Source File

SOURCE=.\src\NameServicePacket.h
# End Source File
# Begin Source File

SOURCE=.\src\NativeNMB.h
# End Source File
# Begin Source File

SOURCE=.\src\NBHostCache.h
# End Source File
# Begin Source File

SOURCE=.\src\NetBIOSPacket.h
# End Source File
# Begin Source File

SOURCE=.\src\NMBIO.h
# End Source File
# Begin Source File

SOURCE=.\src\Options.h
# End Source File
# Begin Source File

SOURCE=.\src\Resolve.h
# End Source File
# Begin Source File

SOURCE=.\src\SambaDefs.h
# End Source File
# Begin Source File

SOURCE=.\src\SambaExterns.h
# End Source File
# Begin Source File

SOURCE=.\src\SambaLink.h
# End Source File
# Begin Source File

SOURCE=.\src\SambaNMB.h
# End Source File
# Begin Source File

SOURCE=.\src\SelectedNMB.h
# End Source File
# Begin Source File

SOURCE=.\src\SelectedSMB.h
# End Source File
# Begin Source File

SOURCE=.\src\SessionCodes.h
# End Source File
# Begin Source File

SOURCE=.\src\SessionIO.h
# End Source File
# Begin Source File

SOURCE=.\src\SessionPacket.h
# End Source File
# Begin Source File

SOURCE=".\src\smb++.h"
# End Source File
# Begin Source File

SOURCE=.\src\SMBCodes.h
# End Source File
# Begin Source File

SOURCE=.\src\SMBIO.h
# End Source File
# Begin Source File

SOURCE=.\src\SMBPacket.h
# End Source File
# Begin Source File

SOURCE=.\src\strtool.h
# End Source File
# Begin Source File

SOURCE=.\src\types.h
# End Source File
# Begin Source File

SOURCE=.\src\Util.h
# End Source File
# End Group
# End Target
# End Project
