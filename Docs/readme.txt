

---------------------------------------------------------------------------
Installation:


X) This step is currently unnecessary, as the source distribution includes the
   latest supported MAME core.

   XX Unzip MAME 0.67 (or later, this document was written at the time of 0.67)
   XX into the MAME directory.


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

X) This step is currently unnecessary, as the source distribution includes the
   latest supported MAME core, pre patched w/ Sectionize.pl


   Run the Sectionize.pl script to break the various ROMs into XBE sections
    (this is done to reduce runtime memory requirements)

2) Open the MAMEoX.sln (Visual Studio.NET 2003) or MAMEoX.NET2002.sln 
   (Visual Studio .NET 2002) file and build the solution for Release mode 
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


3) Create a directory for the program on your box. The directory must match
   the following structure:

  - <Your Directory Name>
    - general  : This is where cfg files, cheat file, and other general files 
                 are stored
    - Media
      XX Font.xpr : This should be taken from the SDK. The project currently
                   has no fonts (or graphics in general, see "Contributions")
        NOTE: Font.xpr is no longer required as of 0.70.1b
      > All of the resource files from the MAMEoX\Media directory in the source
        package, including help.txt and Resource.xpr (created during the build
	process)
    - roms : This is where you should put all of your ROM files/directories.
             NOTE: There is currently a speed hack that _requires_ all ROMs
	           to be zipped! Unzipped files will not be read at all!
      - backup : Directory where "deleted" ROMs are stored
    - samples : This is where you should put all of your audio sample zips
    - hdimages : This is where you should place your CHD hard disk image 
                 directories
    default.xbe : The executable file

   UPDATE: You shouldn't have to actually create any of the sub directories (except
   roms, roms/backup, and other directories with data you provide (hdimages, audio
   samples, etc...)) as of v0.5a. They will be automatically created when you
   first run MAMEoX.

   NOTE: Starting w/ version 0.63b, there are _two_ XBE files that must be 
         copied over, default.xbe and MAMEoX.xbe. Failure to copy both files will
	 result in error messages.

   NOTE: Starting with version 0.64b, if you are playing MAMEoX off of a CD/DVD,
         most of these directories will be created on the U:, which is probably
	 mapped from E:\UDATA\4d414d45 (MAME in hex)

   NOTE: Starting with version 0.65b the INI file will only be read from 
         e:\UDATA\4d414d45. Any INI directory on the D: will be ignored. The
	 INI can be modified from within MAME by pressing B + Y on the ROMList
	 screen. (As of 0.66b there are still some things that can't be changed
	 from within the program, such as filenames and directory paths. This
	 will change in the near future)

   !!!IMPORTANT!!!
     DO NOT RENAME OR MOVE EITHER OF THE XBE FILES! MAMEoX (as, I believe,
     any XBOX executable) is _very_ picky about where its files are and
     what they are named. Renaming or moving either file will cause MAMEoX
     to fail!

---------------------------------------------------------------------------
Compilation Errors:


NOTE: If you use the SourceForge distribution you shouldn't get any compiler
	errors. If you do, please submit them as a bug.

Chances are good that you'll see some compile errors:
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
Usage:

  Available ROMs screen:
    X - Show Help file (the readme.txt isn't kept up to date, except
                        for this one key :))



  MAME input mappings:
    As of version 0.5a, there is no INI file to reconfigure the keypad mappings.
    Reconfiguration can be done via editing the osd_customize_inputport_defaults
    function in xbox_JoystickMouse.c.


	All UI config utilizes the "back" (coin 1) button, as this is (hopefully)
	the least disruptive during normal play.
    "Cancel" :		  Joypad 1, BACK and START
    "Toggle Crosshair" :  Joypad 1, BACK and Left Analog button
    "UI Configure" :	  Joypad 1, BACK and White button
    "Pause" :		  Joypad 1, BACK and Black button
    "On screen display" : Joypad 1, BACK and Right Analog Button
    "Reset ROM" :	  Joypad 1, BACK, Left Trigger, and Right Trigger
    "Load State" :        Joypad 1, BACK, Right Analog button, and Left trigger
    "Save State" :        Joypad 1, BACK, Right Analog button, and Right trigger

		Player Controls
    Button 1   - A
    Button 2   - X
    Button 3   - B
    Button 4   - Y
    Button 5   - Left Trigger
    Button 6   - Right Trigger
    Button 7   - Left Analog button
    Button 8   - Right Analog button
    Button 9   - White
    Button 10  - Black
    Enter Coin - Back
    Start      - Start


  Enabling cheats:
1) Edit your INI file and set the CheatsEnabled field to 1. 
   (Version 0.64b (CVS) has an INI editor built in, so this 
    will become easier in the future)
   UPDATE: With the release of 0.64b it is now easier to enable
           cheats by pressing B + Y in the ROMList screen and
	   pressing right or left on the dpad on the appropriate
	   option.

2) Copy your cheat.dat file to the GENERAL directory 

Cheats in general are now enabled, you can enable/disable 
specific cheats through the in game MAME menu (back+white). 


---------------------------------------------------------------------------
Contributions:

  - The project is currently in need of some decent artwork for the splash
    screen and initial UI. Feel free to contribute files, you won't be paid,
    but you'll at least get your name on the thing :) More information
    can be found on the project website. Be sure not to send any copyrighted
    material (unless you own the copyright and are willing to allow us to use
    the image(s))

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