# Microsoft Developer Studio Project File - Name="MAMEoX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Xbox Application" 0x0b01

CFG=MAMEoX - Xbox Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MAMEoX_VC6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MAMEoX_VC6.mak" CFG="MAMEoX - Xbox Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MAMEoX - Xbox Release" (based on "Xbox Application")
!MESSAGE "MAMEoX - Xbox Debug" (based on "Xbox Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "MAMEoX - Xbox Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "_XBOX" /D "NDEBUG" /YX /FD /G6 /Ztmp /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\Includes" /I "..\MAME\src" /I "..\\" /I "C:\Program Files\Microsoft XBox SDK\Samples\Xbox\Common\Include" /FI"VCPPMame.h" /D "WIN32" /D "_XBOX" /D "NDEBUG" /D "_VC6" /Fp"Release/MAMEoX.pch" /YX /FD /G6 /Ztmp /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/MAMEoX.bsc"
LINK32=link.exe
# ADD BASE LINK32 xapilib.lib d3d8.lib d3dx8.lib xgraphics.lib dsound.lib dmusic.lib xnet.lib xboxkrnl.lib /nologo /machine:I386 /subsystem:xbox /fixed:no /TMP /OPT:REF
# ADD LINK32 xapilib.lib d3d8.lib d3dx8.lib xgraphics.lib dsound.lib dmusic.lib xnet.lib xboxkrnl.lib ZLIB.lib MAME.lib /nologo /machine:I386 /out:"Release/MAMEoX.exe" /libpath:"..\MAME\Release" /libpath:"..\ZLIB\Release" /subsystem:xbox /fixed:no /TMP /OPT:REF
# SUBTRACT LINK32 /incremental:yes /map
XBE=imagebld.exe
# ADD BASE XBE /nologo /stack:0x10000
# ADD XBE /nologo /testname:"MAMEoX" /stack:0x10000 /out:"Release/MAMEoX.xbe" /limitmem
# SUBTRACT XBE /debug
XBCP=xbecopy.exe
# ADD BASE XBCP /NOLOGO
# ADD XBCP /NOLOGO

!ELSEIF  "$(CFG)" == "MAMEoX - Xbox Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_XBOX" /D "_DEBUG" /YX /FD /G6 /Ztmp /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I ".\Includes" /I "..\MAME\src" /I "..\\" /I "C:\Program Files\Microsoft Xbox SDK\Samples\Xbox\Common\Include" /FI"VCPPMame.h" /D "WIN32" /D "_XBOX" /D "_DEBUG" /D "_VC6" /Fp"Debug/MAMEoX.pch" /YX /FD /G6 /Ztmp /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/MAMEoX.bsc"
LINK32=link.exe
# ADD BASE LINK32 xapilibd.lib d3d8d.lib d3dx8d.lib xgraphicsd.lib dsoundd.lib dmusicd.lib xnetd.lib xboxkrnl.lib /nologo /incremental:no /debug /machine:I386 /subsystem:xbox /fixed:no /TMP
# ADD LINK32 xapilibd.lib d3d8d.lib d3dx8d.lib xgraphicsd.lib dsoundd.lib dmusicd.lib xnetd.lib xboxkrnl.lib ZLIB.lib MAME.lib /nologo /incremental:no /debug /machine:I386 /out:"Debug/MAMEoX.exe" /libpath:"..\MAME\Debug" /libpath:"..\ZLIB\Debug" /subsystem:xbox /fixed:no /TMP
XBE=imagebld.exe
# ADD BASE XBE /nologo /stack:0x10000 /debug
# ADD XBE /nologo /stack:0x10000 /debug /out:"Debug/MAMEoX.xbe" /limitmem
XBCP=xbecopy.exe
# ADD BASE XBCP /NOLOGO
# ADD XBCP /NOLOGO

!ENDIF 

# Begin Target

# Name "MAMEoX - Xbox Release"
# Name "MAMEoX - Xbox Debug"
# Begin Group "Includes"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Includes\DebugLogger.h
# End Source File
# Begin Source File

SOURCE=.\Includes\GraphicsManager.h
# End Source File
# Begin Source File

SOURCE=.\Includes\InputManager.h
# End Source File
# Begin Source File

SOURCE=.\Includes\MAMEoX.h
# End Source File
# Begin Source File

SOURCE=.\Includes\osd_cpu.h
# End Source File
# Begin Source File

SOURCE=..\MAME\src\osdepend.h
# End Source File
# Begin Source File

SOURCE=.\Includes\osinline.h
# End Source File
# Begin Source File

SOURCE=.\Includes\ROMList.h
# End Source File
# Begin Source File

SOURCE=.\Includes\Sections.h
# End Source File
# Begin Source File

SOURCE=.\Includes\System_IniFile.h
# End Source File
# Begin Source File

SOURCE=.\Includes\System_StringModifier.h
# End Source File
# Begin Source File

SOURCE=..\VCPPMame.h
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Xbox SDK\Samples\Xbox\Common\Include\xbfont.h"
# End Source File
# Begin Source File

SOURCE=.\Includes\xbox_Direct3DRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Includes\xbox_FileIO.h
# End Source File
# Begin Source File

SOURCE=.\Includes\xbox_JoystickMouse.h
# End Source File
# Begin Source File

SOURCE=.\Includes\xbox_Timing.h
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\XboxSDK_June2002\Samples\Xbox\Common\Include\xbresource.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\XboxSDK_June2002\Samples\Xbox\Common\Include\xbutil.h"
# End Source File
# End Group
# Begin Group "Sources"

# PROP Default_Filter "*.c;*.cpp;*.asm"
# Begin Source File

SOURCE=.\Sources\DebugLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\DriverSections.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\GraphicsManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\InputManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\ROMList.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\System_IniFile.cpp
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft XBox SDK\Samples\Xbox\Common\Src\xbfont.cpp"
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Direct3DRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Display.c
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_FileIO.c
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_JoystickMouse.c
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Keyboard.c
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Miscelaneous.c
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Sound.c
# End Source File
# Begin Source File

SOURCE=.\Sources\xbox_Timing.c
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Xbox SDK\Samples\Xbox\Common\Src\xbresource.cpp"
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Xbox SDK\Samples\Xbox\Common\Src\xbutil.cpp"
# End Source File
# End Group
# End Target
# End Project
