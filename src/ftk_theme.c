/*
 * File: ftk_theme.c   
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   theme
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
 * 2009-12-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_util.h"
#include "ftk_mmap.h"
#include "ftk_theme.h"
#include "ftk_icon_cache.h"
#include "ftk_xml_parser.h"

typedef struct _FtkWidgetTheme
{
	FtkColor   bg[FTK_WIDGET_STATE_NR];
	FtkColor   fg[FTK_WIDGET_STATE_NR];
	FtkColor   border[FTK_WIDGET_STATE_NR];
	FtkBitmap* bg_image[FTK_WIDGET_STATE_NR];
	char bg_image_name[FTK_WIDGET_STATE_NR][32];
}FtkWidgetTheme;

struct _FtkTheme
{
	char name[32];
	FtkIconCache* icon_cache;
	FtkWidgetTheme widget_themes[FTK_WIDGET_TYPE_NR];
};

static const char* s_default_theme = "\
<theme name=\"default\">\
<button bg_image[normal]=\"btn_default_normal.png\" \
bg_image[focused]=\"btn_default_selected.png\" \
bg_image[active]=\"btn_default_pressed.png\" \
bg_image[disable]=\"btn_default_normal_disable.png\" \
/> \
<label fg[disable]=\"ff000000\"\
/>\
<progress_bar \ 
bg[normal]=\"ff9d9e9d\" fg[normal]=\"ffffd300\"\
bg[focused]=\"ff9d9e9d\" fg[focused]=\"ffffd300\"\
bg[active]=\"ff9d9e9d\" fg[active]=\"ffffd300\"\
bg[disable]=\"ff9d9e9d\" fg[disable]=\"ffffd300\"\
/>\
<entry \ 
bg[normal]=\"ffffffff\" fg[normal]=\"ff000000\" bd[normal]=\"ffe2ceee\"\
bg[focused]=\"ffffffff\" fg[focused]=\"ff000000\" bd[focused]=\"ffffbb00\"\
bg[active]=\"ffffffff\" fg[active]=\"ff000000\" bd[active]=\"ffe2ceee\"\
bg[disable]=\"ffffffff\" fg[disable]=\"ffaca899\" bd[disable]=\"ffe2ceee\"\
/>\
<check_button \
	fg[normal]=\"ff000000\"\
	fg[focused]=\"ffffbb00\"\
	fg[active]=\"ffffbb00\"\
	fg[disable]=\"ffaca899\"\
/>\
<radio_button \
	fg[normal]=\"ff000000\"\
	fg[focused]=\"ffffbb00\"\
	fg[active]=\"ffffbb00\"\
	fg[disable]=\"ffaca899\"\
/>\
</theme>";

static Ret ftk_theme_init_default(FtkTheme* thiz)
{
	return ftk_theme_parse_data(thiz, s_default_theme, strlen(s_default_theme));
}

FtkTheme*  ftk_theme_create(int init_default)
{
	FtkTheme* thiz = FTK_ZALLOC(sizeof(FtkTheme));

	if(thiz != NULL)
	{
		size_t i = 0;
		size_t j = 0;
		char icon_path[FTK_MAX_PATH] = {0};

		for(i = 0; i < FTK_WIDGET_TYPE_NR; i++)
		{
			for(j = 0; j < FTK_WIDGET_STATE_NR; j++)
			{
				thiz->widget_themes[i].bg[j].a = 0xff;
				thiz->widget_themes[i].bg[j].r = 0xff;
				thiz->widget_themes[i].bg[j].g = 0xff;
				thiz->widget_themes[i].bg[j].b = 0xff;
				thiz->widget_themes[i].fg[j].a = 0xff;
				thiz->widget_themes[i].fg[j].r = 0x00;
				thiz->widget_themes[i].fg[j].g = 0x00;
				thiz->widget_themes[i].fg[j].b = 0x00;
			}
		}

		if(init_default)
		{
			ftk_theme_init_default(thiz);
		}
		ftk_snprintf(icon_path, sizeof(icon_path), "theme/%s", thiz->name);
		thiz->icon_cache = ftk_icon_cache_create(icon_path);
	}

	return thiz;
}

typedef struct _WidgetNameType
{
	const char* name;
	FtkWidgetType type;
}WidgetNameType;

static const WidgetNameType s_widget_name_types[] = 
{
	{"label",        FTK_LABEL},
	{"entry",        FTK_ENTRY},
	{"button",       FTK_BUTTON},
	{"wait_box",     FTK_WAIT_BOX},
	{"progress_bar", FTK_PROGRESS_BAR},
	{"radio_group",  FTK_RADIO_GROUP},
	{"radio_button", FTK_RADIO_BUTTON},
	{"check_button", FTK_CHECK_BUTTON},
	{"image",        FTK_IMAGE},
	{"scroll_vbar",   FTK_SCROLL_VBAR},
	{"scroll_hbar",   FTK_SCROLL_HBAR},
	{"window",       FTK_WINDOW},
	{"dialog",       FTK_DIALOG},
	{"menu_item",    FTK_MENU_ITEM},
	{"status_item",  FTK_STATUS_ITEM},
	{"status_panel", FTK_STATUS_PANEL},
	{"menu_panel",   FTK_MENU_PANEL},
	{"list_view",    FTK_LIST_VIEW},
	{"icon_view",    FTK_ICON_VIEW},
	{NULL, FTK_NONE},
};

static FtkWidgetType ftk_theme_get_widget_type(FtkTheme* thiz, const char* name)
{
	size_t i = 0;
	return_val_if_fail(thiz != NULL && name != NULL, FTK_NONE);

	for(i = 0; s_widget_name_types[i].name != NULL; i++)
	{
		if(strcmp(name, s_widget_name_types[i].name) == 0)
		{
			return s_widget_name_types[i].type;
		}
	}

	return FTK_NONE;
}

typedef struct _PrivInfo
{
	FtkTheme* theme;
}PrivInfo;

#define TO_STATE(v)\
	switch(v)\
	{\
		case 'n':\
		{\
			state = FTK_WIDGET_NORMAL;\
			break;\
		}\
		case 'f':\
		{\
			state = FTK_WIDGET_FOCUSED;\
			break;\
		}\
		case 'a':\
		{\
			state = FTK_WIDGET_ACTIVE;\
			break;\
		}\
		case 'd':\
		{\
			state = FTK_WIDGET_INSENSITIVE;\
			break;\
		}\
		default:\
		{\
			return RET_FAIL;\
		}\
	}
	
static Ret  ftk_theme_parse_fg_color(FtkTheme* thiz, FtkWidgetType type, const char* name, const char* value)
{
	FtkWidgetState state = FTK_WIDGET_NORMAL;
	FtkWidgetTheme* theme = thiz->widget_themes+type;

	TO_STATE(name[3]);
	theme->fg[state] = ftk_parse_color(value);
	ftk_logd("fg type=%d state=%d (%02x %02x %2x %02x) \n", type, state, 
		theme->fg[state].a, theme->fg[state].r, theme->fg[state].g, theme->fg[state].b);

	return RET_OK;
}

static Ret  ftk_theme_parse_bg_color(FtkTheme* thiz, FtkWidgetType type, const char* name, const char* value)
{
	FtkWidgetState state = FTK_WIDGET_NORMAL;
	FtkWidgetTheme* theme = thiz->widget_themes+type;

	TO_STATE(name[3]);
	theme->bg[state] = ftk_parse_color(value);
	
	ftk_logd("bg type=%d state=%d (%02x %02x %2x %02x) \n", type, state, 
		theme->bg[state].a, theme->bg[state].r, theme->bg[state].g, theme->bg[state].b);

	return RET_OK;
}

static Ret  ftk_theme_parse_bd_color(FtkTheme* thiz, FtkWidgetType type, const char* name, const char* value)
{
	FtkWidgetState state = FTK_WIDGET_NORMAL;
	FtkWidgetTheme* theme = thiz->widget_themes+type;

	TO_STATE(name[3]);
	theme->border[state] = ftk_parse_color(value);
	
	ftk_logd("bd type=%d state=%d (%02x %02x %2x %02x) \n", type, state, 
		theme->border[state].a, theme->border[state].r, theme->border[state].g, theme->border[state].b);

	return RET_OK;
}

static Ret  ftk_theme_parse_bg_image(FtkTheme* thiz, FtkWidgetType type, const char* name, const char* value)
{
	FtkWidgetState state = FTK_WIDGET_NORMAL;
	FtkWidgetTheme* theme = thiz->widget_themes+type;

	TO_STATE(name[9]);
	ftk_strncpy(theme->bg_image_name[state], value, sizeof(theme->bg_image_name[state]));

	ftk_logd("type=%d state=%d name=%s \n", type, state, theme->bg_image_name[state]);
	return RET_OK;
}

static void ftk_theme_builder_on_start(FtkXmlBuilder* thiz, const char* tag, const char** attrs)
{
	size_t i = 0;
	DECL_PRIV(thiz, priv);
	const char* name = NULL;
	const char* value = NULL;
	FtkTheme* theme = priv->theme;
	FtkWidgetType type = FTK_NONE;
	return_if_fail(tag != NULL && attrs != NULL);

	if(strcmp(tag, "theme") == 0)
	{
		for(i = 0; attrs[i] != NULL; i += 2)
		{
			name = attrs[i];
			value = attrs[i+1];
			if(name[0] == 'n')
			{
				ftk_strncpy(theme->name, value, sizeof(theme->name));	
				break;
			}
		}
		
		return;
	}

	type = ftk_theme_get_widget_type(theme, tag);
	return_if_fail(type != FTK_NONE);

	for(i = 0; attrs[i] != NULL; i += 2)
	{
		name = attrs[i];
		value = attrs[i+1];
		if(strlen(name) < 9)
		{
			ftk_logd("%s: unknow %s=%s\n", __func__, name, value);
			continue;		
		}

		switch(name[0])
		{
			case 'f':
			{
				if(name[1] == 'g' && name[2] == '[')
				{
					ftk_theme_parse_fg_color(theme, type, name, value);
				}
				break;
			}
			case 'b':
			{
				if(name[1] == 'd')
				{
					ftk_theme_parse_bd_color(theme, type, name, value);
				}
				else if(name[1] == 'g' && name[2] == '[')
				{
					ftk_theme_parse_bg_color(theme, type, name, value);
				}
				else if(name[1] == 'g' && name[2] == '_')
				{
					ftk_theme_parse_bg_image(theme, type, name, value);
				}
				break;
			}
			default:
			{
				ftk_logd("%s: unknow %s=%s\n", __func__, name, value);
				break;
			}
		}
	}

	return;
}

static void ftk_theme_builder_on_end(FtkXmlBuilder* thiz, const char* tag)
{
	return;
}

static void ftk_theme_builder_on_text(FtkXmlBuilder* thiz, const char* text, size_t length)
{
	return;
}

static void ftk_theme_builder_on_comment(FtkXmlBuilder* thiz, const char* text, size_t length)
{
	return;
}

static void ftk_theme_builder_on_pi(FtkXmlBuilder* thiz, const char* tag, const char** attrs)
{
	return;
}

static void ftk_theme_builder_on_error(FtkXmlBuilder* thiz, int line, int row, const char* message)
{
	return;
}

static void ftk_theme_builder_destroy(FtkXmlBuilder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(FtkXmlBuilder) + sizeof(PrivInfo));
	}

	return;
}
static FtkXmlBuilder* ftk_theme_builder_create(void)
{
	FtkXmlBuilder* thiz = FTK_ZALLOC(sizeof(FtkXmlBuilder) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		thiz->on_start_element = ftk_theme_builder_on_start;
		thiz->on_end_element   = ftk_theme_builder_on_end;
		thiz->on_text          = ftk_theme_builder_on_text;
		thiz->on_comment       = ftk_theme_builder_on_comment;
		thiz->on_pi_element    = ftk_theme_builder_on_pi;
		thiz->on_error         = ftk_theme_builder_on_error;
		thiz->destroy          = ftk_theme_builder_destroy;
	}

	return thiz;
}

Ret        ftk_theme_parse_data(FtkTheme* thiz, const char* xml, size_t length)
{
	FtkXmlParser* parser = NULL;
	FtkXmlBuilder* builder = NULL;
	return_val_if_fail(xml != NULL, RET_FAIL);

	parser  = ftk_xml_parser_create();
	return_val_if_fail(parser != NULL, RET_FAIL);

	builder = ftk_theme_builder_create();

	if(builder != NULL)
	{
		PrivInfo* priv = (PrivInfo*)builder->priv;
		priv->theme = thiz;
		ftk_xml_parser_set_builder(parser, builder);
		ftk_xml_parser_parse(parser, xml, strlen(xml));
	}
	ftk_xml_builder_destroy(builder);
	ftk_xml_parser_destroy(parser);

	return RET_OK;
}

Ret        ftk_theme_parse_file(FtkTheme* thiz, const char* filename)
{
	FtkMmap* m = NULL;
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL && filename != NULL, RET_FAIL);
	
	m = ftk_mmap_create(filename, 0, -1);
	return_val_if_fail(m != NULL, RET_FAIL);
	ret = ftk_theme_parse_data(thiz, ftk_mmap_data(m), ftk_mmap_length(m));
	ftk_mmap_destroy(m);

	return ret;
}

FtkBitmap* ftk_theme_get_bg(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state)
{
	return_val_if_fail(thiz != NULL, NULL);

	if(thiz->widget_themes[type].bg_image[state] == NULL)
	{
		if(thiz->widget_themes[type].bg_image_name[state][0])
		{
			thiz->widget_themes[type].bg_image[state] = ftk_icon_cache_load(thiz->icon_cache, 
				thiz->widget_themes[type].bg_image_name[state]);
		}
	}
	
	if(thiz->widget_themes[type].bg_image[state] != NULL)
	{
		ftk_bitmap_ref(thiz->widget_themes[type].bg_image[state]);
	}
	
	return thiz->widget_themes[type].bg_image[state];
}

FtkBitmap* ftk_theme_load_image(FtkTheme* thiz, const char* filename)
{
	return_val_if_fail(thiz != NULL && filename != NULL, NULL);

	return ftk_icon_cache_load(thiz->icon_cache, filename);
}

FtkColor   ftk_theme_get_bg_color(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state)
{
	FtkColor c = {0};
	return_val_if_fail(thiz != NULL, c);

	return thiz->widget_themes[type].bg[state];
}

FtkColor   ftk_theme_get_border_color(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state)
{
	FtkColor c = {0};
	return_val_if_fail(thiz != NULL, c);

	return thiz->widget_themes[type].border[state];
}

FtkColor   ftk_theme_get_fg_color(FtkTheme* thiz, FtkWidgetType type, FtkWidgetState state)
{
	FtkColor c = {0};
	return_val_if_fail(thiz != NULL, c);

	return thiz->widget_themes[type].fg[state];
}

void       ftk_theme_destroy(FtkTheme* thiz)
{
	if(thiz != NULL)
	{
		size_t i = 0;
		size_t j = 0;

		for(i = 0; i < FTK_WIDGET_TYPE_NR; i++)
		{
			for(j = 0; j < FTK_WIDGET_STATE_NR; j++)
			{
				if(thiz->widget_themes[i].bg_image[j] != NULL)
				{
					ftk_bitmap_unref(thiz->widget_themes[i].bg_image[j]);
					thiz->widget_themes[i].bg_image[j] = NULL;
				}
			}
		}
	}

	return;
}


