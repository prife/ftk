#include "ftk_typedef.h"

#ifndef FONT_DATA_H
#define FONT_DATA_H

typedef enum _Encoding
{
	ENC_UTF16 = 0,
	ENC_ANSI  = 1
}Encoding;

typedef struct _Glyph
{
	char x;
	char y;
	unsigned char w;
	unsigned char h;
	unsigned short code;
	unsigned char* data;
}Glyph;

struct _FontData;
typedef struct _FontData FontData;

FontData* font_data_create(int char_nr, Encoding encoding);
FontData* font_data_load(char* data, int length);
Ret font_data_add_glyph(FontData* thiz, Glyph* glyph);
Ret font_data_get_glyph(FontData* thiz, unsigned short code, Glyph* glyph);


int  font_data_get_version(FontData* thiz);
int  font_data_get_width(FontData* thiz);
int  font_data_get_height(FontData* thiz);
const char* font_data_get_author(FontData* thiz);
const char* font_data_get_family(FontData* thiz);
const char* font_data_get_style(FontData* thiz);

void font_data_set_size(FontData* thiz, int width, int height);
void font_data_set_author(FontData* thiz, const char* author);
void font_data_set_family(FontData* thiz, const char* family);
void font_data_set_style(FontData* thiz, const char* style);

#ifdef HAS_FONT_DATA_SAVE
Ret font_data_save(FontData* thiz, const char* filename);
#endif
void font_data_destroy(FontData* thiz);

#endif/*FONT_DATA_H*/

