include $(abs_top_srcdir)/src/rules.mk

bin_PROGRAMS=calc
calc_SOURCES=main.c
calc_LDFLAGS=$(MYLDFLAGS)
calc_CFLAGS=-DAPP_DATA_DIR="\"$(datadir)/ftk/calc\"" \
	-DAPP_LOCAL_DATA_DIR="\"$(srcdir)\""

