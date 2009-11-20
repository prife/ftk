/*
 * File: fontdata.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   functions to operate font data. 
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-09-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "fontdata.h"
#define FD_ASSERT(p) assert(p)
#define VERSION 0x00000100 /*1.0*/

typedef struct _FontDataHeader
{
	unsigned int   version;
	unsigned int   char_nr;
	unsigned char  width;
	unsigned char  height;
	unsigned short encoding;
	unsigned int   unused;
	char author[64];
	char family[32];
	char style[32];
}FontDataHeader;

struct _FontData
{
	FontDataHeader header;

	Glyph* glyphs;
	size_t data_size;
	size_t data_buffer_size;
	unsigned char* data;

	void* org_data;
	int   new_created;
};

FontData* font_data_create(int char_nr, Encoding encoding)
{
	FontData* thiz = (FontData*)calloc(1, sizeof(FontData));

	if(thiz != NULL)
	{
		thiz->header.version  = VERSION;
		thiz->header.char_nr  = char_nr;
		thiz->header.encoding = encoding;
		
		if(char_nr > 0)
		{
			thiz->new_created = 1;
			thiz->glyphs = calloc(char_nr, sizeof(Glyph));
		}
	}

	return thiz;
}

FontData* font_data_load(char* data, int length)
{
	FontData* thiz = font_data_create(0, 0);

	return_val_if_fail(data != NULL && length > sizeof(FontDataHeader), NULL);

	if(thiz != NULL)
	{
		int glyph_size = 0;
		thiz->header = *(FontDataHeader*)data;
		glyph_size = thiz->header.char_nr * sizeof(Glyph);

		thiz->glyphs = (Glyph*)(data + sizeof(FontDataHeader));
		thiz->data = (unsigned char*)(data + sizeof(FontDataHeader) + glyph_size);
		thiz->data_size = length - ((char*)thiz->data - data);
		thiz->data_buffer_size = thiz->data_size;
		thiz->new_created = 0;
		thiz->org_data = data;
	}

	return thiz;
}

Ret font_data_add_glyph(FontData* thiz, Glyph* glyph)
{
	int i = 0;
	return_val_if_fail(thiz != NULL && glyph != NULL && thiz->new_created, RET_FAIL);
	
	for(i = 0; i < thiz->header.char_nr; i++)
	{
		if(thiz->glyphs[i].code == glyph->code)
		{
			return RET_FOUND;
		}

		if(thiz->glyphs[i].code > glyph->code || thiz->glyphs[i].code == 0)
		{
			int size = glyph->w * glyph->h;
			if(thiz->glyphs[i].code > glyph->code)
			{
				int k = 0;
				for(k = thiz->header.char_nr - 1; k > i; k--)
				{
					thiz->glyphs[k] = thiz->glyphs[k-1];
				}
			}

			if((thiz->data_size + size) >= thiz->data_buffer_size)
			{
				int data_buffer_size = thiz->data_buffer_size + (thiz->data_buffer_size >> 1) + (size << 4);
				unsigned char* data = (unsigned char*)realloc(thiz->data, data_buffer_size);
				if(data != NULL)
				{
					thiz->data = data;
					thiz->data_buffer_size = data_buffer_size;
				}
			}

			thiz->glyphs[i] = *glyph;
			if((thiz->data_size + size) < thiz->data_buffer_size)
			{
				memcpy(thiz->data + thiz->data_size, glyph->data, size);
				thiz->glyphs[i].data = (unsigned char*)thiz->data_size;
				thiz->data_size += size;
			}

			return RET_OK;
		}
	}

	return RET_FAIL;
}

Ret font_data_get_glyph(FontData* thiz, unsigned short code, Glyph* glyph)
{
    int low    = 0;
    int mid    = 0;
    int high   = 0;
    int result = 0;

    return_val_if_fail(thiz != NULL && glyph != NULL, RET_FAIL);

    high = thiz->header.char_nr;
    while(low <= high)
    {
        mid  = low + ((high - low) >> 1);
        result = thiz->glyphs[mid].code - code;

        if(result == 0)
        {
			*glyph = thiz->glyphs[mid];
			glyph->data = (int)(thiz->glyphs[mid].data) + thiz->data;

            return RET_OK;
        }
        else if(result < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return RET_FAIL;
}

int  font_data_get_version(FontData* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->header.version;
}

int  font_data_get_width(FontData* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->header.width;
}

int  font_data_get_height(FontData* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->header.height;
}

const char* font_data_get_author(FontData* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->header.author;
}

const char* font_data_get_family(FontData* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->header.family;
}

const char* font_data_get_style(FontData* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->header.style;
}

void font_data_set_size(FontData* thiz, int width, int height)
{
	return_if_fail(thiz != NULL);

	thiz->header.width  = width;
	thiz->header.height = height;
	
	return;
}

void font_data_set_author(FontData* thiz, const char* author)
{
	return_if_fail(thiz != NULL);
	
	strncpy(thiz->header.author, author, sizeof(thiz->header.author));

	return;
}

void font_data_set_family(FontData* thiz, const char* family)
{
	return_if_fail(thiz != NULL);
	
	strncpy(thiz->header.family, family, sizeof(thiz->header.family));

	return;
}

void font_data_set_style(FontData* thiz, const char* style)
{
	return_if_fail(thiz != NULL);
	
	strncpy(thiz->header.style, style, sizeof(thiz->header.style));

	return;
}

void font_data_destroy(FontData* thiz)
{
	return_if_fail(thiz != NULL);

	if(thiz != NULL)
	{
		if(thiz->new_created)
		{
			free(thiz->glyphs);
			free(thiz->data);
		}
		free(thiz);
	}

	return;
}

#ifdef HAS_FONT_DATA_SAVE
#include <stdio.h>
Ret font_data_save(FontData* thiz, const char* filename)
{
	FILE* fp = fopen(filename, "wb+");
	{
		fwrite(&thiz->header, 1, sizeof(FontDataHeader), fp);
		fwrite(thiz->glyphs, 1, sizeof(Glyph) * thiz->header.char_nr, fp);
		fwrite(thiz->data, 1, thiz->data_size, fp);
		fclose(fp);
	}

	return RET_OK;
}
#endif

#ifdef FONT_DATA_TEST
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char* read_file(const char* file_name, int* length)
{
	struct stat st = {0};
	if(stat(file_name, &st))
	{
		return NULL;
	}
	else
	{
		char* buffer = malloc(st.st_size + 1);
		FILE* fp = fopen(file_name, "rb");
		fread(buffer, 1, st.st_size, fp);
		fclose(fp);
		buffer[st.st_size] = '\0';
		*length = st.st_size;

		return buffer;
	}
}

int main(int argc, char* argv[])
{
	int ch = 'a';
	int length = 0;
	char* buffer = NULL;
	Glyph ret_glyph = {0};
	FontData* data = font_data_create(36, 0);
	Glyph* glyph = (Glyph*)calloc(1, sizeof(Glyph));

	glyph->w = 7;
	glyph->h = 9;
	glyph->x = 5;
	glyph->y = 5;
	glyph->data = calloc(1, glyph->w * glyph->h);

	for(; ch <= 'z'; ch++)
	{
		glyph->code = ch;
		assert(font_data_add_glyph(data, glyph) == RET_OK);
	}
	
	for(ch='0'; ch <= '9'; ch++)
	{
		glyph->code = ch;
		assert(font_data_add_glyph(data, glyph) == RET_OK);
	}

	for(ch='a'; ch <= 'z'; ch++)
	{
		glyph->code = ch;
		assert(font_data_get_glyph(data, ch, &ret_glyph) == RET_OK);
		assert(memcmp(glyph, &ret_glyph, sizeof(ret_glyph)-sizeof(void*)) == 0);
		assert(memcmp(glyph->data, ret_glyph.data, ret_glyph.w * ret_glyph.h) == 0);
	}
	
	for(ch='0'; ch <= '9'; ch++)
	{
		glyph->code = ch;
		assert(font_data_get_glyph(data, ch, &ret_glyph) == RET_OK);
		assert(memcmp(glyph, &ret_glyph, sizeof(ret_glyph)-sizeof(void*)) == 0);
		assert(memcmp(glyph->data, ret_glyph.data, ret_glyph.w * ret_glyph.h) == 0);
	}

	font_data_set_size(data, 16, 16);
	assert(font_data_get_width(data) == 16);
	assert(font_data_get_height(data) == 16);
	font_data_set_author(data, "Li XianJing <xianjimli@hotmail.com>");
	font_data_save(data, "test.fnt");
	font_data_destroy(data);

	buffer = read_file("test.fnt", &length);
	data = font_data_load(buffer, length);
	assert(data != NULL);
	for(ch='a'; ch <= 'z'; ch++)
	{
		glyph->code = ch;
		assert(font_data_get_glyph(data, ch, &ret_glyph) == RET_OK);
		assert(memcmp(glyph, &ret_glyph, sizeof(ret_glyph)-sizeof(void*)) == 0);
		assert(memcmp(glyph->data, ret_glyph.data, ret_glyph.w * ret_glyph.h) == 0);
	}
	assert(font_data_get_version(data) == VERSION);
	assert(strcmp(font_data_get_author(data), "Li XianJing <xianjimli@hotmail.com>") == 0);
	font_data_destroy(data);
	free(glyph->data);
	free(glyph);

	return 0;
}
#endif/*FONT_DATA_TEST*/
