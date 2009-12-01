export ARCH=arm
export HOST_PARAM=--host=arm-linux
export TARGET_CC=arm-linux-gcc

export BUILD_DIR=$PWD/$ARCH
export PREFIX=$BUILD_DIR/usr
export PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig/
export LDFLAGS="-L$PREFIX/lib"
export CFLAGS="-I$PREFIX/include"
