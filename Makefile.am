SUBDIRS=po src data theme tools testdata docs

if ENABLE_OPENGLES
SUBDIRS += opengles
endif

if FTK_BACKEND_PSP
else
SUBDIRS+=fbus services script_binding apps
endif
pkgconfigdir=$(libdir)/pkgconfig

pkgconfig_DATA=ftk.pc ftk.cnf
ACLOCAL_AMFLAGS=-I m4

cnfdir=$(datadir)/ftk/
cnf_DATA=ftk.cnf

EXTRA_DIST=autogen.sh ftk.cnf ftk.pc.in gtk-doc.make

ftkdir=$(includedir)/ftk
ftk_HEADERS=config_ftk.h
