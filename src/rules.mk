INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DHAS_BMP \
	-I$(abs_top_srcdir)/src -I$(abs_top_srcdir)/src/im \
	-DFTK_SUPPORT_C99 \
	-DFTK_SHARED_CANVAS

#INCLUDES+= \
	-DPSP \
	-I$(abs_top_srcdir)/src/os/psp

INCLUDES+=-DLINUX \
	-DDATA_DIR="\"$(datadir)/ftk/base\"" \
	-DFTK_CNF="\"$(datadir)/ftk/ftk.cnf\"" \
	-DFTK_DATA_ROOT="\"$(datadir)/ftk\"" \
	-DTESTDATA_DIR="\"$(abs_top_srcdir)/testdata\"" \
	-DLOCAL_DATA_DIR="\"$(abs_top_srcdir)\"" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-I$(abs_top_srcdir)/src/os/linux

MYLDFLAGS=$(abs_top_builddir)/src/libftk.la -lpng -ljpeg -lz

