
lib_LTLIBRARIES = libsmb++.la
libsmb___la_LDFLAGS = -version-info@LIBVERSION@ @EXTRA_FLAGS@

# AAHAHAHAHAHRGL. I really hate automake by now :-(
# I wasted this whole silly day trying to set:
# . Nice subdirectories: impossible
# . Partial compilation (define the lists in configure.in): bugged, the SOURCES
#   are defined but not the OBJECTS in the resulting makefile.
# GRRR! For now, I just compile everything, and if nothing better, we'll just
# #ifdef out the non relevant files.

libsmb___la_SOURCES = types.cpp Options.cpp Util.cpp Resolve.cpp FileManagementInterface.cpp NameQueryInterface.cpp DES.cpp IOTypes.cpp NMBIO.cpp SMBIO.cpp NetBIOSPacket.cpp SMBPacket.cpp SessionIO.cpp CharCnv.cpp IODescriptors.cpp SessionPacket.cpp NBHostCache.cpp NameServicePacket.cpp NativeNMB.cpp SambaLink.cpp SambaNMB.cpp

#libsmb___la_SOURCES = types.cpp Options.cpp Util.cpp utiltst.cpp Resolve.cpp strtool.cpp FileManagementInterface.cpp NameQueryInterface.cpp @LSRC@
#types.cpp Options.cpp Util.cpp utiltst.cpp Resolve.cpp strtool.cpp FileManagementInterface.cpp NameQueryInterface.cpp SambaLink.cpp SambaNMB.cpp
#EXTRA_libsmb___la_SOURCES = @ESRC@

bin_SCRIPTS = smb++-config

include_HEADERS = smb++.h

pkginclude_HEADERS = \
	defines.h \
	Util.h \
	FileManagementInterface.h \
	Options.h \
	types.h \
	smb++.h \
	SelectedNMB.h \
	SelectedSMB.h \
	Resolve.h \
	NameQueryInterface.h \
	SambaLink.h \
	SambaNMB.h \
	SambaDefs.h \
	SambaExterns.h \
	DES.h \
	CharCnv.h \
	IODescriptors.h \
	IOTypes.h \
	NBHostCache.h \
	NMBIO.h \
	NameServicePacket.h \
	NativeNMB.h \
	NetBIOSPacket.h \
	SMBCodes.h \
	SMBIO.h \
	SMBPacket.h \
	SessionCodes.h \
	SessionIO.h \
	SessionPacket.h

noinst_HEADERS = \
	strtool.h \
	cursesHacks.h
