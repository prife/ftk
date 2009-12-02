all: check zlib png jpeg libftk
all_clean:  zlib_clean png_clean jpeg_clean  libftk_clean

check:
	if [ ! -e packages ]; then mkdir packages;fi

packages/zlib-1.2.3.tar.gz:
	cd packages && wget http://www.zlib.net/zlib-1.2.3.tar.gz
zlib-1.2.3: packages/zlib-1.2.3.tar.gz
	tar xf packages/zlib-1.2.3.tar.gz
zlib: zlib-1.2.3
	export CC=$(TARGET_CC) PREFIX=$(PREFIX) && \
	cd zlib-1.2.3 && \
	./configure --prefix=$(PREFIX) --shared &&\
	make clean; make && make install
zlib_clean:
	cd zlib-1.2.3 && make clean

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

libftk:
	cd ftk* && . ./autogen.sh;
	mkdir ftk/$(ARCH);  cd ftk/$(ARCH) && \
	../configure $(HOST_PARAM) --prefix=$(PREFIX)  &&\
	make clean; make && make install
libftk_clean:
	rm -rf ftk/$(ARCH)

