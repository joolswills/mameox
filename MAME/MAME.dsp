# Microsoft Developer Studio Project File - Name="MAME" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Xbox Static Library" 0x0b04

CFG=MAME - Xbox Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MAME.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MAME.mak" CFG="MAME - Xbox Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MAME - Xbox Release" (based on "Xbox Static Library")
!MESSAGE "MAME - Xbox Debug" (based on "Xbox Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "_XBOX" /D "NDEBUG" /YX /FD /G6 /Ztmp /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /I "src" /I "src\includes" /I "..\MAMEoX\Includes" /I "src\cpu\adsp2100" /I "src\cpu\m68000" /I "..\\" /I "..\c_m68000\output" /I "..\zlib" /FI"VCPPMame.h" /D "WIN32" /D "_XBOX" /D "NDEBUG" /FD /G6 /Ztmp /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "dbg_obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_XBOX" /D "_DEBUG" /YX /FD /G6 /Ztmp /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "src" /I "src\includes" /I "..\MAMEoX\Includes" /I "src\cpu\adsp2100" /I "src\cpu\m68000" /I "..\\" /I "..\c_m68000\output" /I "..\zlib" /FI"VCPPMame.h" /D "_XBOX" /D "_DEBUG" /D INLINE="static __inline" /YX /FD /G6 /Ztmp /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "MAME - Xbox Release"
# Name "MAME - Xbox Debug"
# Begin Group "cpu"

# PROP Default_Filter ""
# Begin Group "adsp2100"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\adsp2100\2100dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\adsp2100\2100ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\adsp2100\adsp2100.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\adsp2100\adsp2100.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "arm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\arm\arm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\arm\arm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\arm\armdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "asap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\asap\asap.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\asap\asap.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\asap\asapdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ccpu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\ccpu\ccpu.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\ccpu\ccpu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\ccpu\ccpudasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\ccpu\ccputabl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "dsp32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\dsp32\dsp32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\dsp32\dsp32.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\dsp32\dsp32dis.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\dsp32\dsp32ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "h6280"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\h6280\6280dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\h6280\h6280.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\h6280\h6280.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\h6280\h6280ops.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\h6280\tblh6280.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "hd6309"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\hd6309\6309dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\hd6309\6309ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\hd6309\6309tbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\hd6309\hd6309.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\hd6309\hd6309.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "i8039"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\i8039\8039dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8039\i8039.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8039\i8039.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "i8085"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\i8085\8085dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8085\i8085.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8085\i8085.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8085\i8085cpu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8085\i8085daa.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "i86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\i86\ea.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\host.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i186intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i188intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i286.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i286.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i286intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i86.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i86.txt

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i86dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i86intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i86time.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\i88intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instr186.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instr186.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instr286.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instr286.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instr86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instr86.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instrv30.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\instrv30.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\modrm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\table186.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\table286.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\table86.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\tablev30.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\v20intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\v30.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\v30intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i86\v33intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "i8x41"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\i8x41\8x41dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8x41\i8x41.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\i8x41\i8x41.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "jaguar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\jaguar\jagdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\jaguar\jaguar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\jaguar\jaguar.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "konami"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\konami\knmidasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\konami\konami.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\konami\konami.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\konami\konamops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\konami\konamtbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "m6502"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\m6502\6502dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\ill02.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m4510.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m4510.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m6502.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m6502.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m6502.txt

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m6509.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m6509.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m65ce02.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\m65ce02.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\ops02.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\ops09.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\ops4510.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\opsc02.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\opsce02.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\opsn2a03.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\t6502.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\t6509.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\t6510.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\t65c02.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\t65ce02.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\t65sc02.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\tdeco16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6502\tn2a03.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "m6800"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\m6800\6800dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6800\6800ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6800\6800tbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6800\m6800.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6800\m6800.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "m68000"

# PROP Default_Filter ""
# Begin Group "ccore"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\c_m68000\output\m68kopac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\c_m68000\output\m68kopdm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\c_m68000\output\m68kopnz.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\c_m68000\output\m68kops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\c_m68000\output\m68kops.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\src\cpu\m68000\cpudefs.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68000.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68k.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68k_in.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kconf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kcpu.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kcpu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kmake.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kmame.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\m68kmame.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m68000\make68k.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "m6805"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\m6805\6805dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6805\6805ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6805\m6805.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6805\m6805.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "m6809"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\m6809\6809dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6809\6809ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6809\6809tbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6809\m6809.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\m6809\m6809.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "mips"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\mips\dismips.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\dismips32.mak

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\mips.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\mips.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\mips3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\mips3.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\mips3dsm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\mipsdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\r3000.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\r3000.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\mips\r3kdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "nec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\nec\nec.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\nec.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\necdasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\necea.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\nechost.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\necinstr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\necintrf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\nec\necmodrm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "s2650"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\s2650\2650dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\s2650\s2650.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\s2650\s2650.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\s2650\s2650cpu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "sh2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\sh2\sh2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\sh2\sh2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\sh2\sh2dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "t11"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\t11\t11.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\t11\t11.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\t11\t11dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\t11\t11ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\t11\t11table.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "tms32010"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\tms32010\32010dsm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32010\dis32010.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32010\tms32010.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32010\tms32010.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "tms32025"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\tms32025\32025dsm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32025\dis32025.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32025\tms32025.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32025\tms32025.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "tms32031"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\tms32031\32031ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32031\dis32031.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32031\tms32031.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms32031\tms32031.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "tms34010"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\tms34010\34010dsm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\34010fld.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\34010gfx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\34010ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\34010ops.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\34010tbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\dis34010.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\makefile

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\tms34010.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms34010\tms34010.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "tms9900"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\tms9900\9900dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\9900stat.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\99xxcore.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\99xxstat.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\tms9900.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\tms9900.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\tms9980a.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\tms9900\tms9995.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "upd7810"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\upd7810\7810dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\upd7810\7810ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\upd7810\7810tbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\upd7810\upd7810.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\upd7810\upd7810.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "v60"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\v60\am.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\am1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\am2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\am3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op12.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op4.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op5.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op6.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\op7a.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\optable.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\v60.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\v60.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\v60d.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\v60\v60mem.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "z180"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\z180\z180.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180cb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180daa.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180dd.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180ed.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180fd.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180op.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180ops.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180tbl.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z180\z180xy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "z80"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\z80\z80.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z80\z80.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z80\z80daa.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z80\z80dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z80\z80dasm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# End Group
# Begin Group "z8000"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cpu\z8000\8000dasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\makedab.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\z8000.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\z8000.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\z8000cpu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\z8000dab.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\z8000ops.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cpu\z8000\z8000tbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\cpu"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\cpu"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\drivers\1942.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\1943.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\40love.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\4enraya.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\8080bw.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\88games.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\aburner.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\actfancr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\aeroboto.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\aerofgt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\afega.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\airbustr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ajax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\aliens.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\alpha68k.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ambush.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\amidar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\amspdwy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\angelkds.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\appoooh.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\aquarium.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\arabian.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\arcadecl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\argus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\arkanoid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\armedf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\artmagic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\asterix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\asteroid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\astinvad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\astrocde.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\astrof.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\asuka.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarifb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarig1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarig42.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarigt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarigx2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarisy1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atarisy2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ataxx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\atetris.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\avalnche.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\avengrgs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\aztarac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\badlands.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bagman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\balsente.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bankp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\baraduke.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\batman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\battlane.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\battlera.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\battlex.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\battlnts.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bbusters.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\beaminv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\beathead.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\beezer.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\berzerk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bigstrkb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bionicc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bking2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bladestl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blktiger.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blmbycar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blockade.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blockhl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blockout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bloodbro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blstroid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\blueprnt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bogeyman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bombjack.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bosco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bottom9.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\brkthru.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bsktball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\btime.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\btoads.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bublbobl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\buggychl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bwidow.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\bzone.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cabal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\canyon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\capbowl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\carjmbre.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\carpolo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cave.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cbasebal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cbuster.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ccastles.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cchasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cclimber.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\centiped.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\champbas.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cheekyms.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\chinagat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\chqflag.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cinemat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\circus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\circusc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cischeat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\citycon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\clayshoo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cloak.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cloud9.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\clshroad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cninja.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cojag.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\combatsc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\commando.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\congo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\contra.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\coolpool.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cop01.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\copsnrob.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cosmic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cps1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cps2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\crbaloon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\crgolf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\crimfght.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\crshrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cshooter.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cvs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\cyberbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\darius.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\darkseal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dassault.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dbz2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dcon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dday.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ddenlovr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ddragon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ddragon3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ddrible.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dec0.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dec8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\deco32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\decocass.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\deniam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\destroyr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\digdug.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\diverboy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dkong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dlair.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\docastle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dogfgt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dominos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dooyong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dotrikun.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dragrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dribling.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\drmicro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dynax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\dynduke.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\epos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\eprom.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\equites.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\esd16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\espial.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exctsccr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exedexes.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exerion.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exidy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exidy440.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exprraid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exterm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\exzisus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\f1gp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fastfred.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fastlane.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fcombat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\finalizr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\findout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\firetrap.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\firetrk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fitfight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\flkatck.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\flower.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\flstory.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\flyball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\foodf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\freekick.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\frogger.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fromanc2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fromance.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\funkybee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\funkyjet.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\fuuki16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gaelco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gaelco2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gaiden.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\galaga.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\galaxian.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\galivan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\galpani2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\galpanic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\galspnbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gameplan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gaplus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gauntlet.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gberet.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gbusters.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gcpinbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\geebee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gijoe.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ginganin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gladiatr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gng.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\goindol.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gomoku.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gotcha.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gottlieb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gotya.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gradius3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\grchamp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gridlee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\grobda.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gstriker.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gsword.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gumbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gunbustr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gundealr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gunsmoke.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\gyruss.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hal21.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\halleys.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hanaawas.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hanaroku.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\harddriv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hcastle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hexa.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hexion.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\higemaru.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hitme.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hnayayoi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\holeland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\homedata.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hyhoo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hyperspt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\hyprduel.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ikki.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\iqblock.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\irobot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ironhors.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\itech32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\itech8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\jack.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\jackal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\jailbrek.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\jedi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\jrcrypt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\jrpacman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\junofrst.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kaneko16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kangaroo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\karnov.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kchamp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kickgoal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kingobox.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kinst.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\klax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kncljoe.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kopunch.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\kyugo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\labyrunr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ladybug.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ladyfrog.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lasso.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lastduel.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lazercmd.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\legionna.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\leland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lemmings.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\leprechn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\liberate.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\liberatr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lkage.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\locomotn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\looping.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lsasquad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\lwings.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m107.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m62.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m72.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m79amb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m90.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m92.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\m92.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\macrossp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\madmotor.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\magmax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mainevt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mappy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\marineb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mario.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\markham.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\marvins.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\matmania.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mayumi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mcatadv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mcr1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mcr2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mcr3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mcr68.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\meadows.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\megasys1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\megazone.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mermaid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\metro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mexico86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mhavoc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\midtunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\midvunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\midwunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\midxunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\midyunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mikie.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\minivadr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\missb2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\missile.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mitchell.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mjkjidai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mjsister.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mnight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mole.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\momoko.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\moo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mosaic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mouser.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mpatrol.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mrdo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mrflea.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mrjong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ms32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\msisaac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mugsmash.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\munchmo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\mystston.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcoic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcona1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namconb1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcond1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcos1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcos2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcos21.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcos22.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\namcos86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\naughtyb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nbmj8688.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nbmj8891.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nbmj8991.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nbmj9195.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nemesis.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\neogeo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\news.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ninjakd2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ninjakid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ninjaw.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nitedrvr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\niyanpai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nmk16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nova2001.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nycaptor.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\nyny.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\offtwall.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ohmygod.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ojankohs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\olibochu.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\omegaf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\omegrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\oneshot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\opwolf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\orbit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\othldrby.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\othunder.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\outrun.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\overdriv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pacland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pacman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pandoras.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\parodius.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pass.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pastelgl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pbaction.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pcktgal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pengo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pgm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\phoenix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\phozon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pingpong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pipedrm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pirates.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pkunwar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\playch10.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\playmark.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\polepos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\policetr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\polyplay.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\poolshrk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pooyan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\popeye.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\popper.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\powerins.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\prehisle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\psikyo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\psikyo4.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\psikyosh.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\psychic5.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\punchout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\pushman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\qix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\quantum.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\quizdna.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\raiden.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\raiders5.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rainbow.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rallyx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rampart.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rastan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\redalert.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\redclash.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\relief.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\renegade.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\retofinv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rmhaihai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rockola.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rockrage.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rocnrope.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rohga.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rollerg.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rollrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\route16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\royalmah.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rpunch.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\rungun.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\safarir.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sauro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sbasketb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sbrkout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\scobra.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\scramble.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\scregg.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sega.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\segac2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\segar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\segasyse.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\seicross.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\senjyo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\seta.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\seta2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sf1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sgladiat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shadfrce.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shangha3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shanghai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shangkid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shaolins.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sharrier.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shisen.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shootout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\shuuz.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sidearms.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sidepckt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\silkroad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\simpsons.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\skullxbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\skychut.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\skydiver.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\skyfox.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\skykid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\skyraid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\slapfght.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\slapshot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\snk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\snk68.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\snowbros.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\solomon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sonson.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spacefb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spbactn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spcforce.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spdbuggy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spdodgeb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\speedbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\speedspn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spiders.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\splash.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sprcros2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sprint2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\spy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\srmp2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\srumbler.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sshangha.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\sslam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ssozumo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ssv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\stactics.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\stadhero.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\starcrus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\starfire.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\starshp1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\starwars.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\statriv2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\stfight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\stlforce.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\strnskil.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\stv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\subs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\suna16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\suna8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\supbtime.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\superchs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\superpac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\superqix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\suprloco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\suprnova.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\suprslam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\surpratk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\system1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\system16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\system18.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\system32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tagteam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tail2nos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_f2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_f3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_h.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_l.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_x.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taito_z.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taitoair.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taitosj.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tankbatt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tankbust.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taotaido.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\targeth.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\taxidrvr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tbowl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tecmo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tecmo16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tecmosys.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tehkanwc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tempest.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\terracre.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tetrisp2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\thepit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\thief.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\thoop2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\thunderj.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\thunderx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tickee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tigeroad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\timelimt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\timeplt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tmnt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tnzs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\toaplan1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\toaplan2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\toki.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\toobin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\topspeed.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\toratora.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\toypop.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tp84.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\trackfld.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\travrusa.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\triplhnt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\troangel.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tsamurai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ttmahjng.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tubep.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tugboat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tumblep.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tunhunt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\turbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\tutankhm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\twin16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\twincobr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ultraman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\ultratnk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\undrfire.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\unico16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\usgames.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vaportra.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vastar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vendetta.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vicdual.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\victory.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\videopin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vigilant.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vindictr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\volfied.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vsnes.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\vulgus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wardner.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\warpwarp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\warriorb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wc90.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wc90b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wecleman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\welltris.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wgp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\williams.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wilytowr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wiping.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wiz.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wrally.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wwfsstar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\wwfwfest.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xain.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xevious.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xexex.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xmen.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xxmissio.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xybots.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\xyonix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\yamato.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\yard.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\yiear.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\yunsun16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\yunsung8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\zac2650.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\zaccaria.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\zaxxon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\zerozone.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\zn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\drivers\zodiack.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\drivers"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\drivers"

!ENDIF 

# End Source File
# End Group
# Begin Group "includes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\includes\8080bw.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\appoooh.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\arabian.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\arcadecl.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\artmagic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\asteroid.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarifb.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarig1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarig42.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarigt.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarigx2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarisy1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atarisy2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\atetris.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\avalnche.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\aztarac.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\badlands.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\balsente.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\batman.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\beaminv.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\beathead.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\berzerk.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\blstroid.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\bsktball.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\btoads.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\bzone.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\canyon.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\capbowl.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\carpolo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cave.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\ccastles.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cchasm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\centiped.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cinemat.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\clayshoo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cloak.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cloud9.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cninja.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cojag.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\copsnrob.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cps1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\crgolf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\crshrace.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\cyberbal.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\dassault.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\dec0.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\deco16ic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\deco32.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\decocrpt.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\decoprot.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\dogfgt.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\dominos.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\dribling.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\dynax.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\epos.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\eprom.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\espial.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\exerion.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\exidy.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\exidy440.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\f1gp.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\fastfred.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\firetrk.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\foodf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\fromance.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\funkyjet.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\galaxian.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\gauntlet.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\gcpinbal.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\gridlee.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\gstriker.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\harddriv.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\homedata.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\iqblock.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\irobot.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\itech32.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\itech8.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\jaguar.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\jedi.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\jrpacman.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\kaneko16.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\kangaroo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\kinst.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\klax.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\kyugo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\lasso.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\leland.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\lemmings.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\leprechn.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\liberatr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\lwings.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\mcr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\meadows.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\megasys1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\mhavoc.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\midtunit.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\midvunit.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\midwunit.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\midyunit.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\missile.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namcoic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namcona1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namconb1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namcond1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namcos2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namcos22.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\namcos3d.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\nb1413m3.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\neogeo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\news.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\nitedrvr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\offtwall.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\pacman.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\pengo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\policetr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\psikyosh.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\qix.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\rampart.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\relief.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\sbrkout.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\sega.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\segac2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\segar.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\seta.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\shuuz.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\skullxbo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\skydiver.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\slapstic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\sprint2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\starwars.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\subs.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\system16.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\taito_f2.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\taito_f3.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\taxidrvr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\thunderj.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\tickee.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\toaplan1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\toobin.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\tubep.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\tunhunt.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\turbo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\videopin.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\vindictr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\williams.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\wwfwfest.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\includes\xybots.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\includes"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\includes"

!ENDIF 

# End Source File
# End Group
# Begin Group "machine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\machine\6522via.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\6522via.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\6821pia.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\6821pia.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\74123.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\74123.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\74148.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\74148.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\74153.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\74153.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\7474.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\7474.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\8080bw.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\8254pit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\8254pit.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\8255ppi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\8255ppi.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\ajax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\arkanoid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\asic65.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\asic65.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\asteroid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\astrocde.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\atari_vg.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\atari_vg.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\atarifb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\atarigen.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\atarigen.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\avalnche.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\bagman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\balsente.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\beezer.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\berzerk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\bonzeadv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\bosco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\bsktball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\btime.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\bublbobl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\buggychl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\capbowl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\carpolo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\cchasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\cchip.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\clayshoo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\copsnrob.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\dec0.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\decocass.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\decocass.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\decocrpt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\decoprot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\digdug.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\docastle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\dominos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\eeprom.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\eeprom.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\equites.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\exctsccr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\flstory.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\gaelco2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\galaga.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\gaplus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\geebee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\grchamp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\grobda.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\harddriv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\idectrl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\idectrl.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\irem_cpu.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\irem_cpu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\irobot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\jackal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\jumpshot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\kabuki.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\konami.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\kyugo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\leland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\leprechn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\lkage.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\lsasquad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\m68kfmly.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\m68kfmly.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\maniach.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mappy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mathbox.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mathbox.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mb87078.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mb87078.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mcr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mcr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mexico86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mhavoc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\midtunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\midwayic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\midwayic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\midwunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\midyunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\missile.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\mspacman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\namcond1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\namcos1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\namcos2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\nb1413m3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\neocrypt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\neogeo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\nitedrvr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\nycaptor.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\pacplus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\pd4990a.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\pd4990a.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\pgmcrypt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\phozon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\playch10.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\polepos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\qix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\rainbow.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\random.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\random.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\retofinv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\rp5h01.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\rp5h01.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\sbrkout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\scramble.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\sega.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\segacrpt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\segacrpt.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\segar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\simpsons.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\slapfght.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\slapstic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\slikshot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\spiders.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\sprint2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\stactics.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\starwars.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\stfight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\subs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\superpac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\system16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\tait8741.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\tait8741.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\taitosj.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\theglobp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\ticket.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\ticket.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\tnzs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\toaplan1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\toypop.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\turbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\twincobr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\videopin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\volfied.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\vsnes.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\williams.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\wrally.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\xevious.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\z80fmly.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\machine\z80fmly.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\machine"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\machine"

!ENDIF 

# End Source File
# End Group
# Begin Group "sndhrdw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\sndhrdw\8080bw.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\asteroid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\astinvad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\astrocde.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\astrof.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\atarijsa.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\atarijsa.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\aztarac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\berzerk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\bosco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\bzone.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\carnival.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\cchasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\cclimber.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\cinemat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\cyberbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\dcs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\dcs.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\depthch.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\dkong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\exidy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\exidy440.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\flower.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\galaxian.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\geebee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\gomoku.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\gorf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\gottlieb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\gotya.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\gridlee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\gyruss.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\harddriv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\invinco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\irem.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\irem.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\jaguar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\leland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\llander.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\m72.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\m72.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\mario.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\mcr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\mcr.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\meadows.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\phoenix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\pleiads.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\polepos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\polyplay.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\pulsar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\rastan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\redalert.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\redbaron.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\rockola.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\scramble.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\sega.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\segar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\seibu.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\seibu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\senjyo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\starwars.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\system16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\taito_f3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\taitosnd.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\taitosnd.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\targ.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\timeplt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\timeplt.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\trackfld.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\warpwarp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\williams.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\williams.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\wiping.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sndhrdw\zaxxon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sndhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sndhrdw"

!ENDIF 

# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\sound\2151intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2151intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2203intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2203intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2413intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2413intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2608intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2608intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2610intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2610intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2612intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\2612intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\262intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\262intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\3812intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\3812intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\5110intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\5110intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\5220intf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\5220intf.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\adpcm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\adpcm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\astrocde.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\astrocde.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ay8910.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ay8910.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\bsmt2000.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\bsmt2000.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\c140.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\c140.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\cem3394.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\cem3394.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\dac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\dac.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\disc_dev.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\disc_flt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\disc_inp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\disc_mth.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\disc_out.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\disc_wav.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\discrete.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\discrete.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\es5506.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\es5506.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\filter.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\filter.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\fm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\fm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\fmopl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\fmopl.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\gaelco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\gaelco.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\hc55516.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\hc55516.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\iremga20.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\iremga20.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k005289.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k005289.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k007232.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k007232.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k051649.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k051649.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k053260.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k053260.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k054539.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\k054539.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\mixer.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\mixer.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\msm5205.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\msm5205.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\msm5232.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\msm5232.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\namco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\namco.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\namcona.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\namcona.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\nes_apu.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\nes_apu.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\nes_defs.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\pokey.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\pokey.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\qsound.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\qsound.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\rf5c68.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\rf5c68.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\saa1099.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\saa1099.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\samples.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\samples.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\segapcm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\segapcm.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\sn76477.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\sn76477.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\sn76496.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\sn76496.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\streams.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\streams.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms36xx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms36xx.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms5110.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms5110.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms5110r.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms5220.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms5220.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\tms5220r.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\upd7759.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\upd7759.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\vlm5030.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\vlm5030.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\votrax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\wavwrite.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\wavwrite.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\x1_010.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\x1_010.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ym2151.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ym2151.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ym2413.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ym2413.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymdeltat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymdeltat.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymf262.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymf262.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymf278b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymf278b.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymz280b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\sound\ymz280b.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\sound"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\sound"

!ENDIF 

# End Source File
# End Group
# Begin Group "vidhrdw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\vidhrdw\1942.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\1943.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\40love.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\4enraya.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\8080bw.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\88games.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\actfancr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\aeroboto.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\aerofgt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\afega.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\airbustr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ajax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\aliens.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\alpha68k.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ambush.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\amspdwy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\angelkds.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\appoooh.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\aquarium.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\arabian.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\arcadecl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\argus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\arkanoid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\armedf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\artmagic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\asterix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\astinvad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\astrocde.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\astrof.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\asuka.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarifb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarig1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarig42.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarigt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarigx2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarimo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarimo.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarirle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarirle.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarisy1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atarisy2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\atetris.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\avalnche.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\avengrgs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\avgdvg.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\avgdvg.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\aztarac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\badlands.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bagman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\balsente.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bankp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\baraduke.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\batman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\battlane.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\battlera.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\battlex.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\battlnts.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bbusters.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\beaminv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\beathead.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\beezer.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\berzerk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bigstrkb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bionicc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bking2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bladestl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blktiger.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blmbycar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blockade.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blockhl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blockout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bloodbro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blstroid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\blueprnt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bogeyman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bombjack.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bosco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bottom9.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\brkthru.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bsktball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\btime.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\btoads.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\bublbobl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\buggychl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cabal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\canyon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\capbowl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\carjmbre.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\carpolo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cave.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cbasebal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cbuster.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ccastles.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cchasm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cclimber.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\centiped.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\champbas.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cheekyms.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\chqflag.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cinemat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\circus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\circusc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cischeat.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\citycon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\clayshoo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cloak.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cloud9.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\clshroad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cninja.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\combatsc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\commando.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\contra.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cop01.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\copsnrob.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cosmic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cps1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\crbaloon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\crgolf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\crimfght.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\crshrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\crtc6845.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\crtc6845.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cvs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\cyberbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\darius.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\darkseal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dassault.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dbz2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dcon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dday.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ddragon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ddragon3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ddrible.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dec0.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dec8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\deco16ic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\deco32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\decocass.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\deniam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\destroyr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\digdug.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\diverboy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dkong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\docastle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dogfgt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dominos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dooyong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dotrikun.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dragrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dribling.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\drmicro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dynax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\dynduke.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\epos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\eprom.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\equites.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\esd16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\espial.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exctsccr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exedexes.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exerion.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exidy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exidy440.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exprraid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exterm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\exzisus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\f1gp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\fastfred.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\fastlane.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\finalizr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\firetrap.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\firetrk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\fitfight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\flkatck.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\flower.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\flstory.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\flyball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\foodf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\freekick.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\fromanc2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\fromance.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\funkybee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\funkyjet.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\fuuki16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gaelco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gaelco2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gaiden.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\galaga.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\galaxian.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\galivan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\galpani2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\galpanic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\galspnbl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gameplan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gaplus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gauntlet.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gberet.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gbusters.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gcpinbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\geebee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\generic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\generic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gijoe.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ginganin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gladiatr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gng.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\goindol.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gomoku.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gotcha.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gottlieb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gotya.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gradius3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\grchamp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gridlee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\grobda.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gstriker.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gsword.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gumbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gunbustr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gundealr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gunsmoke.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\gyruss.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hanaawas.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\harddriv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hcastle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hexa.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hexion.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\higemaru.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hitme.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hnayayoi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\holeland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\homedata.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hyhoo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hyperspt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\hyprduel.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ikki.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\iqblock.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\irobot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ironhors.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\itech32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\itech8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jack.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jackal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jagblit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jagblit.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jagobj.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jaguar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jailbrek.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jedi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\jrpacman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kaneko16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kangaroo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\karnov.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kchamp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kickgoal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kingobox.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kinst.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\klax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kncljoe.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\konamiic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\konamiic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\kyugo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\labyrunr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ladybug.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lasso.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lastduel.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lazercmd.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lazercmd.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\legionna.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\leland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lemmings.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\leprechn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\liberate.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\liberatr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lkage.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lsasquad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\lwings.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\m107.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\m62.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\m72.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\m79amb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\m90.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\m92.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\macrossp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\madmotor.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\magmax.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mainevt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mappy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\marineb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mario.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\markham.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\marvins.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\matmania.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mayumi.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mcatadv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mcr12.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mcr3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mcr68.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\meadows.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\medlanes.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\megasys1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\megazone.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mermaid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\metro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mexico86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\midtunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\midvunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\midyunit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mikie.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\minivadr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\missile.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mitchell.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mjkjidai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mjsister.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mnight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mole.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\momoko.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\moo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mosaic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mouser.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mpatrol.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mrdo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mrflea.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mrjong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ms32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\msisaac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mugsmash.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\munchmo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\mystston.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcona1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namconb1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcos1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcos2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcos21.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcos22.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcos3d.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\namcos86.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\naughtyb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nbmj8688.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nbmj8891.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nbmj8991.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nbmj9195.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nemesis.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\neogeo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\news.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ninjakd2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ninjakid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ninjaw.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nitedrvr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\niyanpai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nmk16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nova2001.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nycaptor.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\nyny.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\offtwall.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ohmygod.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ojankohs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\omegaf.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\oneshot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\orbit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\othldrby.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\othunder.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\overdriv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pacland.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pandoras.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\parodius.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pass.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pastelgl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pbaction.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pcktgal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pengo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pgm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\phoenix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\phozon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pingpong.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pirates.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pkunwar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\playch10.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\playmark.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\polepos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\policetr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\poly.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\poly.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\polyplay.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\poolshrk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pooyan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\popeye.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\popper.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\powerins.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ppu2c03b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ppu2c03b.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\prehisle.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\psikyo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\psikyo4.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\psikyosh.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\psychic5.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\punchout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\pushman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\qix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\quizdna.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\raiden.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\raiders5.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rallyx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rampart.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rastan.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\redalert.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\redclash.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\relief.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\renegade.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\res_net.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\retofinv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rockola.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rockrage.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rocnrope.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rohga.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rollerg.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rollrace.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\route16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rpunch.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\rungun.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\s2636.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\s2636.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sauro.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sbasketb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sbrkout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sega.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\segac2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\segar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\segasyse.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\seicross.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\senjyo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\seta.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\seta2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sf1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shadfrce.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shangha3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shangkid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shaolins.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shisen.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shootout.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\shuuz.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sidearms.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sidepckt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\silkroad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\simpsons.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\skullxbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\skychut.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\skydiver.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\skyfox.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\skykid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\skyraid.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\slapfght.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\slapshot.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\snk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\snk68.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\snowbros.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\solomon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sonson.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\spacefb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\spbactn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\spcforce.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\spdodgeb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\speedbal.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\speedspn.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\spiders.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\splash.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sprcros2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sprint2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\spy.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\srmp2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\srumbler.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sshangha.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sslam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ssozumo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ssv.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\stactics.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\stadhero.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\starcrus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\starfire.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\starshp1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\stfight.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\stlforce.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\strnskil.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\subs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\suna16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\suna8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\supbtime.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\superchs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\superpac.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\superqix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\suprloco.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\suprnova.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\suprslam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\surpratk.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\sys16spr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\system1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\system1.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\system16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\system32.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tagteam.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tail2nos.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_f2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_f3.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_h.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_l.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_x.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taito_z.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taitoair.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taitoic.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taitoic.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taitosj.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tankbatt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tankbust.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taotaido.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\targeth.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\taxidrvr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tbowl.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tecmo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tecmo16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tehkanwc.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\terracre.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tetrisp2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\thepit.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\thief.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\thoop2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\thunderj.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\thunderx.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tickee.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tigeroad.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\timelimt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\timeplt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tlc34076.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tlc34076.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tmnt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tms34061.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tms34061.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tnzs.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\toaplan1.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\toaplan2.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\toki.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\toobin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\topspeed.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\toypop.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tp84.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\trackfld.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\travrusa.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\triplhnt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\troangel.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tsamurai.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ttmahjng.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tubep.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tumblep.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tunhunt.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\turbo.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\tutankhm.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\twin16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\twincobr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ultraman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\undrfire.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\unico16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\usgames.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vaportra.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vastar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vball.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vector.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vector.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vendetta.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vicdual.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\victory.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\videopin.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vigilant.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vindictr.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\volfied.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vsnes.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\vulgus.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\warpwarp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\warriorb.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wc90.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wc90b.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wecleman.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\welltris.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wgp.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\williams.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wiping.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wiz.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wrally.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wwfsstar.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\wwfwfest.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xain.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xevious.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xexex.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xmen.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xxmissio.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xybots.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\xyonix.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\yard.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ygv608.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\ygv608.h

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\yiear.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\yunsun16.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\yunsung8.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\zac2650.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\zaccaria.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\zaxxon.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\zerozone.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vidhrdw\zodiack.c

!IF  "$(CFG)" == "MAME - Xbox Release"

# PROP Intermediate_Dir "obj\vidhrdw"

!ELSEIF  "$(CFG)" == "MAME - Xbox Debug"

# PROP Intermediate_Dir "dbg_obj\vidhrdw"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\src\artwork.c
# End Source File
# Begin Source File

SOURCE=.\src\artwork.h
# End Source File
# Begin Source File

SOURCE=.\src\audit.c
# End Source File
# Begin Source File

SOURCE=.\src\audit.h
# End Source File
# Begin Source File

SOURCE=.\src\cheat.c
# End Source File
# Begin Source File

SOURCE=.\src\cheat.h
# End Source File
# Begin Source File

SOURCE=.\src\common.c
# End Source File
# Begin Source File

SOURCE=.\src\common.h
# End Source File
# Begin Source File

SOURCE=.\src\cpuexec.c
# End Source File
# Begin Source File

SOURCE=.\src\cpuexec.h
# End Source File
# Begin Source File

SOURCE=.\src\cpuint.c
# End Source File
# Begin Source File

SOURCE=.\src\cpuint.h
# End Source File
# Begin Source File

SOURCE=.\src\cpuintrf.c
# End Source File
# Begin Source File

SOURCE=.\src\cpuintrf.h
# End Source File
# Begin Source File

SOURCE=.\src\datafile.c
# End Source File
# Begin Source File

SOURCE=.\src\datafile.h
# End Source File
# Begin Source File

SOURCE=.\src\drawgfx.c
# End Source File
# Begin Source File

SOURCE=.\src\drawgfx.h
# End Source File
# Begin Source File

SOURCE=.\src\driver.c
# End Source File
# Begin Source File

SOURCE=.\src\driver.h
# End Source File
# Begin Source File

SOURCE=.\src\fileio.c
# End Source File
# Begin Source File

SOURCE=.\src\fileio.h
# End Source File
# Begin Source File

SOURCE=.\src\harddisk.c
# End Source File
# Begin Source File

SOURCE=.\src\harddisk.h
# End Source File
# Begin Source File

SOURCE=.\src\hdcomp.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\hiscore.c
# End Source File
# Begin Source File

SOURCE=.\src\hiscore.h
# End Source File
# Begin Source File

SOURCE=.\src\info.c
# End Source File
# Begin Source File

SOURCE=.\src\info.h
# End Source File
# Begin Source File

SOURCE=.\src\inptport.c
# End Source File
# Begin Source File

SOURCE=.\src\inptport.h
# End Source File
# Begin Source File

SOURCE=.\src\input.c
# End Source File
# Begin Source File

SOURCE=.\src\input.h
# End Source File
# Begin Source File

SOURCE=.\src\legacy.h
# End Source File
# Begin Source File

SOURCE=.\src\mame.c
# End Source File
# Begin Source File

SOURCE=.\src\mame.h
# End Source File
# Begin Source File

SOURCE=.\src\mamedbg.c
# End Source File
# Begin Source File

SOURCE=.\src\mamedbg.h
# End Source File
# Begin Source File

SOURCE=.\src\md5.c
# End Source File
# Begin Source File

SOURCE=.\src\md5.h
# End Source File
# Begin Source File

SOURCE=.\src\memory.c
# End Source File
# Begin Source File

SOURCE=.\src\memory.h
# End Source File
# Begin Source File

SOURCE=.\src\network.c
# End Source File
# Begin Source File

SOURCE=.\src\network.h
# End Source File
# Begin Source File

SOURCE=.\src\osdepend.h
# End Source File
# Begin Source File

SOURCE=.\src\palette.c
# End Source File
# Begin Source File

SOURCE=.\src\palette.h
# End Source File
# Begin Source File

SOURCE=.\src\png.c
# End Source File
# Begin Source File

SOURCE=.\src\png.h
# End Source File
# Begin Source File

SOURCE=.\src\profiler.c
# End Source File
# Begin Source File

SOURCE=.\src\profiler.h
# End Source File
# Begin Source File

SOURCE=.\src\romcmp.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\sndintrf.c
# End Source File
# Begin Source File

SOURCE=.\src\sndintrf.h
# End Source File
# Begin Source File

SOURCE=.\src\state.c
# End Source File
# Begin Source File

SOURCE=.\src\state.h
# End Source File
# Begin Source File

SOURCE=.\src\tilemap.c
# End Source File
# Begin Source File

SOURCE=.\src\tilemap.h
# End Source File
# Begin Source File

SOURCE=.\src\timer.c
# End Source File
# Begin Source File

SOURCE=.\src\timer.h
# End Source File
# Begin Source File

SOURCE=.\src\ui_text.c
# End Source File
# Begin Source File

SOURCE=.\src\ui_text.h
# End Source File
# Begin Source File

SOURCE=.\src\unzip.c
# End Source File
# Begin Source File

SOURCE=.\src\unzip.h
# End Source File
# Begin Source File

SOURCE=.\src\usrintrf.c
# End Source File
# Begin Source File

SOURCE=.\src\usrintrf.h
# End Source File
# Begin Source File

SOURCE=..\VCPPMame.h
# End Source File
# Begin Source File

SOURCE=.\src\version.c
# End Source File
# Begin Source File

SOURCE=.\src\window.c
# End Source File
# Begin Source File

SOURCE=.\src\window.h
# End Source File
# End Target
# End Project
