#include "ftk_font.h"

int main(int argc, char* argv[])
{
	FtkGlyph glyph = {0};
	unsigned short c = 'a';
	FtkFont* font = ftk_font_create("./unicode.fnt", 16);
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
