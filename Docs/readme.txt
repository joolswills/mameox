
More project information and help can be found on the MAMEoX website
http://mameox.sourceforge.net or the MAMEoX sourceforge project page
http://sourceforge.net/projects/mameox

---------------------------------------------------------------------------
Compilation requirements:

  Visual Studio .NET 2003
  ZLIB source (http://www.gzip.org/zlib/)

---------------------------------------------------------------------------
Installation:

1) Unzip ZLIB to the ZLIB directory. You only need the following files:
	adler32.c
	compress.c
	crc32.c
	deflate.c
	deflate.h
	gzio.c
	infblock.c
	infblock.h
	infcodes.c
	infcodes.h
	inffast.c
	inffast.h
	inffixed.h
	inflate.c
	inftrees.c
	inftrees.h
	infutil.c
	infutil.h
	maketree.c
	trees.c
	trees.h
	uncompr.c
	zconf.h
	zlib.h
	zutil.c
	zutil.h

	(Also the project files, and any license text files)

   ZLIB can be downloaded for free off the web, the homepage is currently
    http://www.gzip.org/zlib/


2) Open the MAMEoX.sln file and build the solution for Release mode 
   (or Debug mode if you're doing actual development). 
    
    This can be done via the following steps:
	a) Build menu -> Configuration Manager...
	   Change the "Active Solution Configuration" field to "Release"
	   Click "Close"
	b) Build menu -> Build solution

    Note: You may have to change the include/link paths, depending on where
          you placed MAME and the SDK files. This can be done via the "properties"
	  submenu of the MAMEoX (and possibly the MAME) project.

    Note: You may also have to patch the resulting XBE file if your 
    set does not execute debug files.


3) Create a directory for the program on your Xbox. The default directory 
   structure is:
   
  - <Your Directory Name>
    - Media
      > All of the resource files from the MAMEoX\Media directory in the source
        package, including help.txt and Resource.xpr (created during the build
	process)

    default.xbe : The Launcher executable file
    MAMEoX.xbe : The helper executable file


    - roms : This is where you should put all of your ROM files/directories.
             NOTE: There is currently a speed hack that _requires_ all ROMs
	           to be zipped! Unzipped files will not be read at all!
      - backup : Directory where "deleted" ROMs are stored
    - artwork: This is where you put bezels, overlays, etc...
    - samples : This is where you should put all of your audio sample zips
    - hdimages : This is where you should place your CHD hard disk image 
                 directories
    - general  : This is where cfg files, cheat file, and other general files 
                 are stored. When in doubt as to where to put a 3rd party file,
		 this is probably the place.


   NOTE: You shouldn't have to actually create any of the sub directories (except
   roms, roms/backup, and other directories with data you provide (hdimages, audio
   samples, etc...)) as of v0.5a. They will be automatically created when you
   first run MAMEoX.

   NOTE: Starting with version 0.64b, if you are playing MAMEoX off of a CD/DVD,
         most of these directories will be created on the U:, which is probably
	 mapped from E:\UDATA\4d414d45 (MAME in hex)


   !!!IMPORTANT!!!
     DO NOT RENAME OR MOVE EITHER OF THE XBE FILES! MAMEoX (as, I believe,
     any XBOX executable) is _very_ picky about where its files are and
     what they are named. Renaming or moving either file will cause MAMEoX
     to fail!



---------------------------------------------------------------------------
Compilation Errors:


NOTE: If you use the SourceForge distribution you shouldn't get any compiler
	errors. If you do, please submit them as a bug.

Compilation errors that I've dealt with in the past:
  a) png_get_info is defined multiple times: You can safely #ifdef this
     function out of the MAME source, it is not called.
  b) Internal compiler errors
    i) You may see errors on switch( offset<<1 ) statements. To fix this,
       you'll have to rewrite the switch statement as "switch( offset )"
       and divide all of the case values by two. Hopefully this will be
       fixed in future versions of MAME, as switching on offset<<1 has
       no practical value.
    ii) I've seen at least one error in the x86 cpu code where the compiler
       choked trying to build up a value via bitwise or's (|) and pass it
       immediately to a function/macro. To fix this, I delcared a temporary
       variable, assigned to the value that had to be passed to the function,
       then passed the temp var to the function. Unlike (4.b.i), there's no 
       reason not to build up a variable while passing it to a function, and
       it's such a common operation that it's somewhat surprising to see an
       error here. Therefore the kludge has not been reported, and may or may
       not have to be reimplemented in the future, depending on where and
       when the compiler feels like barfing.
    iii) Other internal errors: Try reordering header files :) If this does
       not fix your problem, feel free to post a bug, and it will be looked
       into.

  c) Normal compiler errors:
    i) I've encountered some compiler errors where driver writers have not
       produced fully ANSI compliant code. For the most part, these are simple
       cases where a variable is declared after the first non-declaration 
       statement. They may be fixed (with some care) by moving the declaration
       to the top of the function.

       ex: 
		int blah;
		blah = 1;
		int foo;	<= Compiler error
	
		int foo;	<= Move it before the first non-declaration
		int blah;
		blah = 1;
    ii) Symbol redefinitions in multi32.c: Several global variables are
        defined in both multi32.c and system32.c (drivers). They can be
	fixed (probably, I've not tested any ROMs that use these files)
	by making the declarations in multi32.c "extern"

	extern data16_t *system32_mixerregs_monitor_a;		// mixer registers
	extern data16_t *sys32_tilebank_external;
	extern data16_t* sys32_displayenable;
	extern int system32_temp_kludge;
	extern data16_t *sys32_spriteram16;
	extern data16_t *sys32_txtilemap_ram;
	extern data16_t *sys32_ramtile_ram;
	extern data16_t *scrambled_paletteram16;
	extern int system32_palMask;
	extern int system32_mixerShift;
	extern int system32_use_default_eeprom;

  d) Crashes:
    i) There's a known (and reported) 0.67 crash bug in tilemap.c in the
       tilemap_dispose function. This bug will cause the system to crash
       if it runs out of memory when creating the graphical tilemaps. To fix
       it, replace the while loop and the following line with:

	while( prev && prev->next != tilemap ) prev = prev->next;
	if( prev ) prev->next = tilemap->next;
	
	This ensures that the system will not mind if a tilemap is freed that
	has not been added to the linked list.

  e) Missing files:
    i) If your XDK is installed to a directory other than default (and
       sometimes even if it's in the default) you may see build errors
       about missing files: xbutil.cpp, xbfont.cpp, xbresource.cpp
       If this happens, delete these files from the MAMEoX and MAMEoXLauncher
       projects, then re-add them from the correct directory
 


---------------------------------------------------------------------------
Usage Help:

  Read the help screen from within the MAMEoX launcher (press START to
  bring up the menu from which you can read the screen)


---------------------------------------------------------------------------
Contributions:

  - Any reports of bugs, crashes, etc... that you can file would be great.
    Just be sure never to send any copyrighted materials (ROMs, compiled 
    XBE images).  The more detailed the report, the better. Bugs will most
    likely be handled in the order of legibility, reproducibility, and general
    "niceness" of the report. That is to say, if you report "ROM x doesn't
    wurk u fag, u suk", the report will likely be sent to the circular file.
    However, if you don't have the technical background to give a really 
    detailed report, just saying "ROM x crashed my machine when booting" or
    "ROM x crashed my machine on level 3" w/out any flames is enough. If you
    can, please try to reproduce the problem and be sure to mention whether
    or not you can consistently reproduce it.


---------------------------------------------------------------------------

  If this guide is not complete please report your problems. The intent 
  of this project is to make it easier for anybody w/ the right tools and 
  a little bit of coding experience to compile and run this port of MAME.

---------------------------------------------------------------------------

  Special thanks to Klaus of www.wolfsoft.de for donating 128 meg upgrade
  chips to allow me to profile and test larger ROMs!

---------------------------------------------------------------------------
Licensing:
  The contents of the MAME directory (other than the MAME.vcproj, MAME.plg,
  and MAME.dsp files) and the c_m68000 directory are protected under the 
  MAME licensing agreement. A copy of this agreement may be found in the 
  MAME/Docs directory, as "mame.txt". This license may also be found 
  online at www.mame.net/readme.html

  All other files distributed with this project are protected by the GNU Public
  License, unless otherwise noted within the file.
