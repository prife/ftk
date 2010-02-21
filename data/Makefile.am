ftkdatadir=$(datadir)/ftk/base/data

ftkdata_DATA=$(srcdir)/fhw.bin  $(srcdir)/py.bin  $(srcdir)/wbx.bin

if FONT_ENGINE_FREETYPE
ftkdata_DATA+=$(srcdir)/font.ttf
endif

if FONT_ENGINE_DEFAULT
ftkdata_DATA+=$(srcdir)/unicode.fnt
endif

EXTRA_DIST=font.ttf unicode.fnt fhw.bin  py.bin  wbx.bin
