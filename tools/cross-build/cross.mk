all: check zlib png jpeg tslib freetype libftk
all_clean:  zlib_clean png_clean jpeg_clean  libftk_clean tslib_clean freetype_clean 

all_dfb: check zlib png jpeg tslib freetype directfb libftk
all_dfb_clean:  zlib_clean png_clean jpeg_clean  libftk_clean tslib_clean freetype_clean directfb_clean

check:
	if [ ! -e packages ]; then mkdir packages;fi

packages/zlib-1.2.5.tar.gz:
	cd packages && wget http://www.zlib.net/zlib-1.2.5.tar.gz
zlib-1.2.5: packages/zlib-1.2.5.tar.gz
	tar xf packages/zlib-1.2.5.tar.gz
zlib: zlib-1.2.5
	export CC=$(TARGET_CC) PREFIX=$(PREFIX) && \
	cd zlib-1.2.5 && \
	./configure --prefix=$(PREFIX) --shared &&\
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
zlib_clean:
	cd zlib-1.2.5 && make clean

packages/libpng-1.2.35.tar.bz2:
	cd packages && wget http://nchc.dl.sourceforge.net/sourceforge/libpng/libpng-1.2.35.tar.bz2
libpng-1.2.35: packages/libpng-1.2.35.tar.bz2
	tar xf packages/libpng-1.2.35.tar.bz2
png: libpng-1.2.35
	mkdir libpng-1.2.35/$(ARCH); cd libpng-1.2.35/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
png_clean:
	rm -rf libpng-1.2.35/$(ARCH)

packages/jpegsrc.v7.tar.gz:
	cd packages && wget ftp://ftp.carnet.hr/misc/imagemagick/delegates/jpegsrc.v7.tar.gz
jpegsrc.v7: packages/jpegsrc.v7.tar.gz
	tar xf packages/jpegsrc.v7.tar.gz
jpeg: jpegsrc.v7
	mkdir jpeg-7/$(ARCH); cd jpeg-7/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
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
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 

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
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
freetype_clean:
	rm -rf freetype-2.3.9/$(ARCH)

packages/DirectFB-1.2.9.tar.gz:
	cd packages && wget http://www.directfb.org/downloads/Core/DirectFB-1.2/DirectFB-1.2.9.tar.gz
DirectFB-1.2.9: packages/DirectFB-1.2.9.tar.gz
	tar xf packages/DirectFB-1.2.9.tar.gz
directfb: DirectFB-1.2.9
	mkdir DirectFB-1.2.9/$(ARCH); cd DirectFB-1.2.9/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX) --with-inputdrivers="keyboard,linuxinput,tslib" --with-gfxdrivers= $(HOST_PARAM) --prefix=$(PREFIX) --enable-unique &&\
	make clean; make && make install DESTDIR=${STAGING} &&
	make install DESTDIR=/ 
directfb_clean:
	rm -rf DirectFB-1.2.9/$(ARCH)

packages/expat-2.0.1.tar.gz:
	cd packages && wget http://downloads.sourceforge.net/project/expat/expat/2.0.1/expat-2.0.1.tar.gz?use_mirror=nchc
expat-2.0.1: packages/expat-2.0.1.tar.gz
	tar xf packages/expat-2.0.1.tar.gz
expat:expat-2.0.1
	cd expat* && mkdir $(ARCH); cd $(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX) && \
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
expat_clean:
	cd expat* && rm -fr $(ARCH)

packages/fontconfig-2.8.0.tar.gz:
	cd packages && wget http://www.fontconfig.org/release/fontconfig-2.8.0.tar.gz
fontconfig-2.8.0:packages/fontconfig-2.8.0.tar.gz
	tar xf packages/fontconfig-2.8.0.tar.gz
fontconfig: fontconfig-2.8.0 expat freetype
	cd fontconfig* && mkdir $(ARCH); cd $(ARCH) && \
	../configure --disable-libxml2 --with-arch=$(ARCH)-linux $(HOST_PARAM) --prefix=$(PREFIX) && \
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
fontconfig_clean:
	cd fontconfig* && rm -fr $(ARCH)

packages/pixman-0.18.0.tar.gz:
	cd packages && wget http://cairographics.org/releases/pixman-0.18.0.tar.gz
pixman-0.18.0:packages/pixman-0.18.0.tar.gz
	tar xf packages/pixman-0.18.0.tar.gz
pixman:pixman-0.18.0
	cd pixman* && mkdir $(ARCH); cd $(ARCH) && \
	../configure --disable-gtk $(HOST_PARAM) --prefix=$(PREFIX) && \
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
pixman_clean:
	cd pixman* && rm -fr $(ARCH)

packages/cairo-1.8.10.tar.gz:
	cd packages && wget http://cairographics.org/releases/cairo-1.8.10.tar.gz
cairo-1.8.10:packages/cairo-1.8.10.tar.gz
	tar xf packages/cairo-1.8.10.tar.gz
cairo: cairo-1.8.10 fontconfig pixman
	cd cairo* && mkdir $(ARCH); cd $(ARCH) && \
	../configure  --enable-ft   --disable-svg  --disable-pdf  --disable-ps \
	--disable-directfb  --disable-xlib  --disable-xlib-xrender \
	$(HOST_PARAM) --prefix=$(PREFIX) && \
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
cairo_clean:
	cd cairo* && rm -fr $(ARCH)

libftk: $(CAIRO)
	cd ftk* && . ./autogen.sh;
	mkdir ftk/$(ARCH);  cd ftk/$(ARCH) && \
	../configure $(ENABLE_CAIRO) ac_cv_func_realloc_0_nonnull=yes ac_cv_func_malloc_0_nonnull=yes --enable-tslib $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install DESTDIR=${STAGING} && \
	make install DESTDIR=/ 
libftk_clean:
	rm -rf ftk/$(ARCH)

