SUBDIRS=src data theme tools testdata doc
if FTK_BACKEND_PSP
else
SUBDIRS+=script_binding apps
endif
EXTRA_DIST=autogen.sh
pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=ftk.pc ftk.cnf
ACLOCAL_AMFLAGS=-I m4

cnfdir=$(datadir)/ftk/
cnf_DATA=ftk.cnf
