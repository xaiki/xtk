# additional include paths necessary to compile the C++ library
AM_CFLAGS = -I$(top_srcdir)/src @AM_CFLAGS@ @GTK_CFLAGS@

libxtk_CPPFLAGS = $(GTK_CFLAGS)
libxtk_LIBS = $(GTK_LIBS)

# the library names to build (note we are building static libs only)
lib_LTLIBRARIES = libxtk-1.0.la

# where to install the headers on the system
libxtk_1_0_ladir = $(includedir)/xtk

# the list of header files that belong to the library (to be installed later)
libxtk_1_0_la_HEADERS = gtkcellrendererwidget.h

# the sources to add to the library and to add to the source distribution
libxtk_1_0_la_SOURCES =  $(libxtk_1_0_la_HEADERS) \
                    gtkcellrendererwidget.c
libxtk_1_0_la_LDFLAGS = -no-undefined -version-info @XTK_LT_CURRENT@:@XTK_LT_REVISION@:@XTK_LT_AGE@

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libxtk_1_0_la_SOURCES)

Xtk-1.0.gir: libxtk-1.0.la
Xtk_1_0_gir_INCLUDES = Gtk-3.0
Xtk_1_0_gir_CFLAGS = $(INCLUDES) $(GTK_CFLAGS)
Xtk_1_0_gir_LIBS = libxtk-1.0.la gtk-3
Xtk_1_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += Xtk-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
