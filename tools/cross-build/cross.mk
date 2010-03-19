all: check zlib png jpeg tslib freetype libftk
all_clean:  zlib_clean png_clean jpeg_clean  libftk_clean tslib_clean freetype_clean 

all_dfb: check zlib png jpeg tslib freetype directfb libftk
all_dfb_clean:  zlib_clean png_clean jpeg_clean  libftk_clean tslib_clean freetype_clean directfb_clean

check:
	if [ ! -e packages ]; then mkdir packages;fi

packages/zlib-1.2.4.tar.gz:
	cd packages && wget http://www.zlib.net/zlib-1.2.4.tar.gz
zlib-1.2.4: packages/zlib-1.2.4.tar.gz
	tar xf packages/zlib-1.2.4.tar.gz
zlib: zlib-1.2.4
	export CC=$(TARGET_CC) PREFIX=$(PREFIX) && \
	cd zlib-1.2.4 && \
	./configure --prefix=$(PREFIX) --shared &&\
	make clean; make && make install
zlib_clean:
	cd zlib-1.2.4 && make clean

packages/libpng-1.2.35.tar.bz2:
	cd packages && wget http://nchc.dl.sourceforge.net/sourceforge/libpng/libpng-1.2.35.tar.bz2
libpng-1.2.35: packages/libpng-1.2.35.tar.bz2
	tar xf packages/libpng-1.2.35.tar.bz2
png: libpng-1.2.35
	mkdir libpng-1.2.35/$(ARCH); cd libpng-1.2.35/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install
png_clean:
	rm -rf libpng-1.2.35/$(ARCH)

packages/jpegsrc.v7.tar.gz:
	cd packages && wget ftp://ftp.carnet.hr/misc/imagemagick/delegates/jpegsrc.v7.tar.gz
jpegsrc.v7: packages/jpegsrc.v7.tar.gz
	tar xf packages/jpegsrc.v7.tar.gz
jpeg: jpegsrc.v7
	mkdir jpeg-7/$(ARCH); cd jpeg-7/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install
jpeg_clean:
	rm -rf jpeg-7/$(ARCH)

packages/tslib-1.0.tar.bz2:
	cd packages && wget http://download.berlios.de/tslib/tslib-1.0.tar.bz2
tslib-1.0: packages/tslib-1.0.tar.bz2
	tar xf packages/tslib-1.0.tar.bz2
tslib: tslib-1.0
	cd tslib-1.0 && ./autogen.sh; cd -;\
	mkdir tslib-1.0/$(ARCH); cd tslib-1.0/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX) -sysconfdir=$(PREFIX)/etc  ac_cv_func_malloc_0_nonnull=yes &&\
	make clean; make && make install
tslib_clean:
	rm -rf tslib-1.0/$(ARCH)
tslib_source_clean:
	rm -rf tslib-1.0

packages/freetype-2.3.9.tar.gz:
	cd packages && wget http://ftp.twaren.net/Unix/NonGNU/freetype/freetype-2.3.9.tar.gz
freetype-2.3.9: packages/freetype-2.3.9.tar.gz
	tar xf packages/freetype-2.3.9.tar.gz
freetype: freetype-2.3.9
	mkdir freetype-2.3.9/$(ARCH); cd freetype-2.3.9/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install
freetype_clean:
	rm -rf freetype-2.3.9/$(ARCH)
freetype_source_clean:
	rm -rf freetype-2.3.9

packages/DirectFB-1.2.9.tar.gz:
	cd packages && wget http://www.directfb.org/downloads/Core/DirectFB-1.2/DirectFB-1.2.9.tar.gz
DirectFB-1.2.9: packages/DirectFB-1.2.9.tar.gz
	tar xf packages/DirectFB-1.2.9.tar.gz
directfb: DirectFB-1.2.9
	mkdir DirectFB-1.2.9/$(ARCH); cd DirectFB-1.2.9/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX) --with-inputdrivers="keyboard,linuxinput,tslib" --with-gfxdrivers= $(HOST_PARAM) --prefix=$(PREFIX) --enable-unique &&\
	make clean; make && make install
directfb_clean:
	rm -rf DirectFB-1.2.9/$(ARCH)
directfb_source_clean:
	rm -rf DirectFB-1.2.9

libftk:
	cd ftk* && . ./autogen.sh;
	mkdir ftk/$(ARCH);  cd ftk/$(ARCH) && \
	../configure ac_cv_func_realloc_0_nonnull=yes ac_cv_func_malloc_0_nonnull=yes --enable-tslib $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install
libftk_clean:
	rm -rf ftk/$(ARCH)

