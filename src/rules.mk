INCLUDES=-Wall -g -DHAS_PNG -DHAS_JPEG -DHAS_BMP \
	-DDATA_DIR="\"$(datadir)/ftk/base\"" -DFTK_DATA_ROOT="\"$(datadir)/ftk\"" \
	-DLINUX -I$(abs_top_srcdir)/src -I$(abs_top_srcdir)/src/im -DTESTDATA_DIR="\"$(abs_top_srcdir)/testdata\"" \
	-DLOCAL_DATA_DIR="\"$(abs_top_srcdir)\"" -DLINUX -DFTK_SUPPORT_C99 -I$(abs_top_srcdir)/src/os/linux \
	-DFTK_SHARED_CANVAS

MYLDFLAGS=$(abs_top_builddir)/src/libftk.la -lpng -ljpeg -lz

