INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DHAS_BMP \
	-DDATA_DIR="\"$(datadir)/ftk/base\"" -DFTK_DATA_ROOT="\"$(datadir)/ftk\"" \
	-I$(abs_top_srcdir)/src -I$(abs_top_srcdir)/src/im -DTESTDATA_DIR="\"$(abs_top_srcdir)/testdata\"" \
	-DLOCAL_DATA_DIR="\"$(abs_top_srcdir)\"" -DFTK_SUPPORT_C99 -I$(abs_top_srcdir)/src/os/linux \
	-DFTK_SHARED_CANVAS

#INCLUDES+=-DPSP
INCLUDES+=-DLINUX

INCLUDES+=-I$(abs_top_srcdir)/src/os/psp

MYLDFLAGS=$(abs_top_builddir)/src/libftk.la -lpng -ljpeg -lz

