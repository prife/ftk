#include "ftk.h"
#ifdef USE_FREETYPE 
#include "ftk_font_freetype.h"
#else
#include "ftk_font_default.h"
#endif

typedef struct _RtlInfo
{
	int in_len;
	unsigned char* in;

	int out_len;
	unsigned char* out;
}RtlInfo;

Ret font_data_encode(RtlInfo* thiz)
{
	int i = 0;
	unsigned char c = 0;
	unsigned char* in = NULL;
	unsigned char* out = NULL;
	return_val_if_fail(thiz != NULL && thiz->in != NULL, RET_FAIL);
	
	if(thiz->out_len < thiz->in_len || thiz->out == NULL)
	{
		thiz->out = FTK_ZALLOC(thiz->in_len);
	}

	in = thiz->in;
	out = thiz->out;
	for(i = 0; i < thiz->in_len; i+=2, out++)
	{
		c = (i+1) < thiz->in_len ? in[i+1] : 0;
		*out = (in[i] & 0xf0) | (c >> 4);
	}

	thiz->out_len = out - thiz->out;

	return RET_OK;
}

Ret font_data_decode(RtlInfo* thiz)
{
	int i = 0;
	unsigned char c = 0;
	unsigned char* in = NULL;
	unsigned char* out = NULL;
	return_val_if_fail(thiz != NULL && thiz->in != NULL, RET_FAIL);

	if(thiz->out_len < thiz->in_len * 2 || thiz->out == NULL)
	{
		thiz->out = FTK_ZALLOC(thiz->in_len * 2);
	}

	in = thiz->in;
	out = thiz->out;
	for(i = 0; i < thiz->in_len; i++, out+=2)
	{
		c = in[i];
		out[0] = c & 0xf0;
		out[1] = (c & 0xf) << 4;
	}

	thiz->out_len = out - thiz->out;

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 0;
	FtkGlyph glyph = {0};
	unsigned short c = 'a';
	FtkFont* font = NULL;
	RtlInfo info = {0};
	FtkFontDesc* font_desc = ftk_font_desc_create(FTK_DEFAULT_FONT);
	const char* filename = argv[1] != NULL ? argv[1] : "../../data/unicode.fnt";
	ftk_set_allocator(ftk_allocator_default_create());
#ifdef USE_FREETYPE 
	font = ftk_font_freetype_create(filename, font_desc);
#else
	font = ftk_font_default_create(filename, font_desc);
#endif	
	ftk_font_desc_unref(font_desc);

	memset(&info, 0xff, sizeof(info));
	for(c = 0x6129; c != 0x61ff; c++)
	{
		assert(ftk_font_lookup(font, c, &glyph) == RET_OK);
		
		info.in = glyph.data;
		info.in_len = glyph.w * glyph.h;
		info.out = NULL;
		info.out_len = 0;

		font_data_encode(&info);

		printf("in_len=%d out_len=%d\n", info.in_len, info.out_len);
		printf("%04x x=%d y=%d w=%d h=%d\n", c, glyph.x, glyph.y, glyph.w, glyph.h);
		info.in = info.out;
		info.in_len = info.out_len;
		info.out = NULL;
		info.out_len = 0;
		font_data_decode(&info);
		n = glyph.w * glyph.h;
		//assert(n == info.out_len);
		for(i = 0; i < n; i++)
		{
			assert(info.out[i] == (glyph.data[i] & 0xf0));
		}
		FTK_FREE(info.in);
		FTK_FREE(info.out);
	}
	assert(ftk_font_lookup(font, '.', &glyph) == RET_OK);
	printf("%04x x=%d y=%d w=%d h=%d\n", c, glyph.x, glyph.y, glyph.w, glyph.h);
	ftk_font_destroy(font);

	return 0;
}
