# additional include paths necessary to compile the C++ library
AM_CFLAGS = -I$(top_srcdir)/src @AM_CFLAGS@

# the library names to build (note we are building static libs only)
lib_LIBRARIES = libxtk.a

# where to install the headers on the system
libxtk_adir = $(includedir)/xtk

# the list of header files that belong to the library (to be installed later)
libxtk_a_HEADERS = gtkcellrendererwidget.h

# the sources to add to the library and to add to the source distribution
libxtk_a_SOURCES =  $(libxtk_a_HEADERS) \
                    gtkcellrendererwidget.c
