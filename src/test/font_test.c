#ifdef USE_FREETYPE 
#include "ftk_font_freetype.h"
#else
#include "ftk_font_default.h"
#endif

int main(int argc, char* argv[])
{
	FtkGlyph glyph = {0};
	unsigned short c = 'a';
	FtkFont* font = NULL;
	const char* filename = argv[1] != NULL ? argv[1] : "./unicode.fnt";
#ifdef USE_FREETYPE 
	font = ftk_font_freetype_create(filename, 0, 0, 16);
#else
	font = ftk_font_default_create(filename, 0, 0, 16);
#endif	
	for(c = 'a'; c != 'z'; c++)
	{
		assert(ftk_font_lookup(font, c, &glyph) == RET_OK);
		printf("%04x x=%d y=%d w=%d h=%d\n", c, glyph.x, glyph.y, glyph.w, glyph.h);
	}
	assert(ftk_font_lookup(font, '.', &glyph) == RET_OK);
	printf("%04x x=%d y=%d w=%d h=%d\n", c, glyph.x, glyph.y, glyph.w, glyph.h);
	ftk_font_destroy(font);

	return 0;
}
