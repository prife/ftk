INCLUDES=-Wall  -g -DHAS_PNG -DHAS_JPEG \
	-DDATA_DIR="\"$(datadir)/ftk\"" \
	-DLINUX -I$(abs_top_srcdir)/src -DTESTDATA_DIR="\"$(abs_top_srcdir)/src/testdata\"" \
	-DLOCAL_DATA_DIR="\"$(abs_top_srcdir)/src\"" -DLINUX -DFTK_SUPPORT_C99 -I$(abs_top_srcdir)/src/os/linux 

MYLDFLAGS=$(abs_top_builddir)/src/libftk.la -lpng -ljpeg

