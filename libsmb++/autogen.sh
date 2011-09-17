#!/bin/sh
# Run this to generate all the initial makefiles, etc.
# This file was imported by Arthur Frysiak, probably from the GNOME project
# Simplified to our needs by Nicolas Brodu on 20000202

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

(test -f $srcdir/configure.in \
  && test -d $srcdir/macros \
) || {
	echo "Couldn't find the macros subdirectory."
    exit 1
}

DIE=0

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed to compile libsmb++."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

(grep "^AM_PROG_LIBTOOL" $srcdir/configure.in >/dev/null) && {
  (libtoolize --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`libtool' installed to compile libsmb++."
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-1.2d.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}

grep "^AM_GNU_GETTEXT" $srcdir/configure.in >/dev/null && {
  grep "sed.*POTFILES" $srcdir/configure.in >/dev/null || \
  (gettextize --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`gettext' installed to compile libsmb++."
    echo "Get ftp://alpha.gnu.org/gnu/gettext-0.10.35.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`automake' installed to compile libsmb++."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.3.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
  NO_AUTOMAKE=yes
}


# if no automake, don't bother testing for aclocal
test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
  echo "installed doesn't appear recent enough."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.3.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
}

if test "$DIE" -eq 1; then
  exit 1
fi

if test -z "$*"; then
  echo "**Warning**: I am going to run \`configure' with no arguments."
  echo "If you wish to pass any to it, please specify them on the"
  echo \`$0\'" command line."
  echo
fi

echo "Running libtoolize..."
libtoolize --force --copy
echo "Running aclocal..."
aclocal -I macros
echo "Running autoheader..."
autoheader
echo "Running automake..."
automake --add-missing -c
echo "Running autoconf ..."
autoconf

if test x$NOCONFIGURE = x; then
  echo Running $srcdir/configure "$@" ...
  $srcdir/configure "$@" \
  && echo Now type \`make\' to compile libsmb++
else
  echo Skipping configure process.
fi

