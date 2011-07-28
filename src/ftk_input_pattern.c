/*
 * File: ftk_input_pattern.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   input pattern to format some special input, such as date, 
 *  time and ip address.
 *
 * Copyright (c) 2009 - 2011  Li XianJing <xianjimli@hotmail.com>
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
 * 2011-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_allocator.h"
#include "ftk_input_pattern.h"

typedef enum _ValidType
{
	VT_DIGIT  = 'D',
	VT_XDIGIT = 'X',
	VT_ALPHA  = 'A',
	VT_ID     = 'I'
}ValidType;

typedef int (*IsValidChar)(unsigned char c);

typedef struct _InputPattern
{
	size_t offset;
	size_t size;
	size_t min_size;
	size_t max_size;
	unsigned char default_char;
	unsigned char delim_char;
	struct _InputPattern* next;
	IsValidChar is_valid_char;
}InputPattern;

struct _FtkInputPattern
{
	char* text;
	size_t caret;

	InputPattern* pattern;
};

static int is_digit(unsigned char c)
{
	return c >= '0' && c <= '9';
}

static int is_xdigit(unsigned char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static int is_alpha(unsigned char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static int is_id(unsigned char c)
{
	return is_alpha(c) || is_digit(c) || c == '_';
}

static IsValidChar find_valid_function(unsigned char d)
{
	switch(d)
	{
		case 'D': return is_digit;
		case 'X': return is_xdigit;
		case 'A': return is_alpha;
		case 'I': return is_id;
		default: break;
	}

	return NULL;
}

static Ret ftk_input_pattern_append(FtkInputPattern* thiz, InputPattern* p)
{
	return RET_OK;
}

/*D[2]0:D[2]0:D[2]0*/
/*type[min-max]default_char delim_char*/
static Ret ftk_input_pattern_parse(FtkInputPattern* thiz, const char* pattern)
{
	int state = 0;
	size_t i = 0;
	unsigned char c = 0;
	InputPattern p = {0};
	enum 
	{
		ST_TYPE,
		ST_MIN,
		ST_MAX,
		ST_DEFAULT,
		ST_DELIM
	};

	state = ST_TYPE;

	for(i = 0; pattern[i]; i++)
	{
		c = pattern[i];

		switch(state)
		{
			case ST_TYPE:
			{
				memset(&p, sizeof(p), 0x00);
				if(c == '[')
				{
					state = ST_MIN;
				}
				else
				{
					p.is_valid_char = find_valid_function(c);
				}
				break;
			}
			case ST_MIN:
			{
				if(c == ']')
				{
					state = ST_DEFAULT;
				}
				else if(c == '-')
				{
					state = ST_MAX;
				}
				else
				{
					p.min_size = p.min_size * 10 + c - '0';
				}
				break;
			}
			case ST_MAX:
			{
				if(c == ']')
				{
					state = ST_DEFAULT;
				}
				else
				{
					p.max_size = p.min_size * 10 + c - '0';
				}
				
				break;
			}
			case ST_DEFAULT:
			{
				p.default_char = c;
				state = ST_DELIM;
			}
			case ST_DELIM:
			{
				p.delim_char = c;
				state = ST_TYPE;
				ftk_input_pattern_append(thiz, &p);
				memset(&p, sizeof(p), 0x00);
			}
			default:break;
		}
	}
	
	ftk_input_pattern_append(thiz, &p);

	return RET_OK;
}

FtkInputPattern* ftk_input_pattern_create(const char* pattern, const char* init)
{
	FtkInputPattern* thiz = NULL;

	return_val_if_fail(pattern != NULL && init != NULL, NULL);
	
	thiz = (FtkInputPattern*)FTK_ZALLOC(sizeof(FtkInputPattern));

	if(thiz != NULL)
	{
		ftk_input_pattern_parse(thiz, pattern);
	}

	return thiz;
}

Ret    ftk_input_pattern_input(FtkInputPattern* thiz, FtkKey key)
{
	return RET_OK;
}

Ret    ftk_input_pattern_set_caret(FtkInputPattern* thiz, size_t caret)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	if(caret <= strlen(thiz->text))
	{
		thiz->caret = caret;
	}

	return RET_OK;
}

Ret    ftk_input_pattern_set_text(FtkInputPattern* thiz, const char* text)
{
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);

	if(strlen(thiz->text) >= strlen(text))
	{
		strcpy(thiz->text, text);
	}
	else
	{
		FTK_FREE(thiz->text);
		thiz->text = ftk_strdup(text);
	}

	return RET_OK;
}

size_t ftk_input_pattern_get_caret(FtkInputPattern* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->caret;
}


const char* ftk_input_pattern_get_text(FtkInputPattern* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->text;
}

void ftk_input_pattern_destroy(FtkInputPattern* thiz)
{
	InputPattern* save = NULL;
	InputPattern* iter = NULL;

	if(thiz != NULL)
	{
		iter = thiz->pattern;

		while(iter != NULL)
		{
			save = iter->next;
			FTK_FREE(iter);
			iter = save;
		}

		FTK_FREE(thiz);
	}

	return;
}



