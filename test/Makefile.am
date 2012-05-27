# this is example-file: src/bin/Makefile.am

# additional include paths necessary to compile the C programs
AM_CFLAGS = -I$(top_srcdir)/src @AM_CFLAGS@

# the program to build (the names of the final binaries)
bin_PROGRAMS = test

# list of sources for the 'test' binary
test_SOURCES =    test.c

# the additional libraries needed to link test
test_LDADD =  $(top_builddir)/src/xtk/libxtk.a $(AM_LDFLAGS)
