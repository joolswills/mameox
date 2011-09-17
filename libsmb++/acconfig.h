/*=== Curses version detection defines ===*/
/* Found some version of curses that we're going to use */
#undef HAS_CURSES

/* Use SunOS SysV curses? */
#undef USE_SUNOS_CURSES

/* Use old BSD curses - not used right now */
#undef USE_BSD_CURSES

/* Use SystemV curses? */
#undef USE_SYSV_CURSES

/* Use Ncurses? */
#undef USE_NCURSES

/* If you Curses does not have color define this one */
#undef NO_COLOR_CURSES

/* Define if you want to turn on SCO-specific code */
#undef SCO_FLAVOR

/* Set to reflect version of ncurses *
 *  *   0 = version 1.*
 *   *   1 = version 1.9.9g
 *    *   2 = version 4.0/4.1/4.2 */
#undef NCURSES_970530

/* Debug level for the native code */
#undef DEBUG

/* resolve name using DNS in addition to NetBIOS for the native code */
#undef DNSQUERY

/* NetBIOS scope for the native code */
#undef NBSCOPE

/* Native code assumes SMB hosts are not reliable */
#undef OVERKILL

/* compiler understand bool */
#undef HAVE_BOOL

/* Select native code or Samba */
#undef USE_SAMBA

/* Define if you have a 64 bit off_t type. */
#undef HAVE_OFF64_T

/* Location of smb.conf */
#undef CONFIGFILE
