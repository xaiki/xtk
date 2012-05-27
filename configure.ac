# initial information about the project
AC_INIT([Xtk],[3.0],[xaiki@evilgiggle.com])

# check if the source folder is available
AC_CONFIG_SRCDIR([src/gtkcellrendererwidget.c])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# automake initialization (mandatory) including a check for automake API version >= 1.11
AM_INIT_AUTOMAKE([1.11 -Wall no-define -Wno-portability tar-ustar no-dist-gzip dist-xz])
AM_MAINTAINER_MODE([enable])

# use the C++ compiler for the following checks
AC_LANG([C])


# required versions of other packages
m4_define([glib_required_version], [2.33.1])
m4_define([pango_required_version], [1.30.0])
m4_define([atk_required_version], [2.2.0])
m4_define([cairo_required_version], [1.10.0])
m4_define([gdk_pixbuf_required_version], [2.26.0])
m4_define([introspection_required_version], [1.32.0])
GLIB_REQUIRED_VERSION=glib_required_version
PANGO_REQUIRED_VERSION=pango_required_version
ATK_REQUIRED_VERSION=atk_required_version
CAIRO_REQUIRED_VERSION=cairo_required_version
GDK_PIXBUF_REQUIRED_VERSION=gdk_pixbuf_required_version
INTROSPECTION_REQUIRED_VERSION=introspection_required_version

AC_SUBST(GLIB_REQUIRED_VERSION)
AC_SUBST(PANGO_REQUIRED_VERSION)
AC_SUBST(ATK_REQUIRED_VERSION)
AC_SUBST(CAIRO_REQUIRED_VERSION)
AC_SUBST(GDK_PIXBUF_REQUIRED_VERSION)
AC_SUBST(INTROSPECTION_REQUIRED_VERSION)

PKG_CHECK_MODULES(BASE_DEPENDENCIES,
  [glib-2.0 >= glib_required_version dnl
   atk >= atk_required_version dnl
   pango >= pango_required_version dnl
   cairo >= cairo_required_version dnl
   cairo-gobject >= cairo_required_version dnl
   gdk-pixbuf-2.0 >= gdk_pixbuf_required_version])


# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# distribute additional compiler and linker flags among Makefiles
# --> set and change these variables instead of CXXFLAGS or LDFLAGS (for user only)
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

# files to generate via autotools (prepare .am or .in source files)
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([test/Makefile])

# finally this generates the Makefiles etc. for the build
AC_OUTPUT
