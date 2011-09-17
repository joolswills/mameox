@SET_MAKE@

SUBDIRS = src doc macros

@PACKAGE@.spec:

EXTRA_DIST = @PACKAGE@.spec autogen.sh FAQ

## make rpms
rpm: Makefile
	$(MAKE) dist
	rpm -ta --clean $(PACKAGE)-$(VERSION).tar.gz
		
