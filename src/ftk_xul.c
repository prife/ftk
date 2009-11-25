/*
 * File: ftk_xul.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: create ui from xml.
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
 * 2009-11-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk.h"
#include "ftk_xul.h"
#include "ftk_expr.h"
#include "ftk_xml_parser.h"

typedef struct _PrivInfo
{
	FtkWidget* root;
	FtkWidget* current;
	int  meet_start_tag;
	char processed_value[128];
}PrivInfo;

typedef struct _FtkWidgetCreateInfo
{
	int id;
	int x;
	int y;
	int w;
	int h;
	int attr;
	int visible;
	const char* value;
	FtkWidget* parent;
	FtkGc gcs[FTK_WIDGET_STATE_NR];
	
	/*check button*/
	int checked;
	/*scroll bar*/
	int max_value;
	int page_delta;
}FtkWidgetCreateInfo;

typedef FtkWidget* (*FtkXulWidgetCreate)(FtkWidgetCreateInfo* info);

typedef struct _WidgetCreator
{
	const char* name;
	FtkXulWidgetCreate create;
}WidgetCreator;

static FtkWidget* ftk_xul_label_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_label_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_widget_set_text(widget, info->value);
	}

	return widget;
}

static FtkWidget* ftk_xul_button_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_button_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_widget_set_text(widget, info->value);
	}

	return widget;
}

static FtkWidget* ftk_xul_entry_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_entry_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_entry_set_text(widget, info->value);
	}

	return widget;
}

static FtkWidget* ftk_xul_wait_box_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_wait_box_create(info->parent, info->x, info->y);
	if(info->value != NULL && atoi(info->value))
	{
		ftk_wait_box_start_waiting(widget);
	}

	return widget;
}

static FtkWidget* ftk_xul_progress_bar_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_progress_bar_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_progress_bar_set_percent(widget, atoi(info->value));
	}

	return widget;
}

static FtkWidget* ftk_xul_radio_group_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_radio_group_create(info->parent, info->x, info->y, info->w, info->h);

	return widget;
}

static FtkWidget* ftk_xul_radio_button_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_radio_button_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_widget_set_text(widget, info->value);
	}
	
	if(info->checked)
	{
		ftk_check_button_set_checked(widget, info->checked);
	}

	return widget;
}

static FtkWidget* ftk_xul_check_button_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_check_button_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_widget_set_text(widget, info->value);
	}
	
	if(info->checked)
	{
		ftk_check_button_set_checked(widget, info->checked);
	}

	return widget;
}

static FtkWidget* ftk_xul_image_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_image_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_image_set_image_file(widget, info->value);
	}

	return widget;
}

static FtkWidget* ftk_xul_scroll_bar_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_scroll_bar_create(info->parent, info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_scroll_bar_set_param(widget, atoi(info->value), info->max_value, info->page_delta);
	}

	return widget;
}

static FtkWidget* ftk_xul_window_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_app_window_create();
	if(info->value != NULL)
	{
		ftk_widget_set_text(widget, info->value);
	}

	return widget;
}

static FtkWidget* ftk_xul_dialog_create(FtkWidgetCreateInfo* info)
{
	FtkWidget* widget = NULL;

	widget = ftk_dialog_create(info->x, info->y, info->w, info->h);
	if(info->value != NULL)
	{
		ftk_widget_set_text(widget, info->value);
	}

	return widget;
}

static const WidgetCreator s_widget_creaters[] = 
{
	{"label",        ftk_xul_label_create},
	{"entry",        ftk_xul_entry_create},
	{"button",       ftk_xul_button_create},
	{"wait_box",     ftk_xul_wait_box_create},
	{"progress_bar", ftk_xul_progress_bar_create},
	{"radio_group",  ftk_xul_radio_group_create},
	{"radio_button", ftk_xul_radio_button_create},
	{"check_button", ftk_xul_check_button_create},
	{"image",        ftk_xul_image_create},
	{"scroll_bar",   ftk_xul_scroll_bar_create},
	{"window",       ftk_xul_window_create},
	{"dialog",       ftk_xul_dialog_create},
	{NULL, NULL},
};

static FtkXulWidgetCreate ftk_xul_find_creator(const char* name)
{
	int i = 0;
	return_val_if_fail(name != NULL, NULL);

	for(i = 0; s_widget_creaters[i].name != NULL; i++)
	{
		if(strcmp(s_widget_creaters[i].name, name) == 0)
		{
			return s_widget_creaters[i].create;
		}
	}

	return NULL;
}

typedef int (*FtkXulVarGetter)(FtkXmlBuilder* thiz);

typedef struct _VarGetter
{
	const char* name;
	FtkXulVarGetter get;
}VarGetter;

static int ftk_xul_builder_get_parent_width(FtkXmlBuilder* thiz)
{
	DECL_PRIV(thiz, priv);
	
	return ftk_widget_width(ftk_widget_parent(priv->current));
}

static int ftk_xul_builder_get_parent_height(FtkXmlBuilder* thiz)
{
	DECL_PRIV(thiz, priv);

	return ftk_widget_height(ftk_widget_parent(priv->current));
}

static int ftk_xul_builder_get_window_width(FtkXmlBuilder* thiz)
{
	DECL_PRIV(thiz, priv);

	return ftk_widget_width(priv->root);
}

static int ftk_xul_builder_get_window_height(FtkXmlBuilder* thiz)
{
	DECL_PRIV(thiz, priv);

	return ftk_widget_height(priv->root);
}

static int ftk_xul_builder_get_display_width(FtkXmlBuilder* thiz)
{
	return ftk_display_width(ftk_default_display());
}

static int ftk_xul_builder_get_display_height(FtkXmlBuilder* thiz)
{
	return ftk_display_height(ftk_default_display());
}

static const VarGetter s_var_getters[] = 
{
	{"pw",             ftk_xul_builder_get_parent_width},
	{"parent_width",   ftk_xul_builder_get_parent_width},
	{"ph",             ftk_xul_builder_get_parent_height},
	{"parent_height",  ftk_xul_builder_get_parent_height},
	{"ww",             ftk_xul_builder_get_window_width},
	{"window_width",   ftk_xul_builder_get_window_width},
	{"wh",             ftk_xul_builder_get_window_height},
	{"window_height",  ftk_xul_builder_get_window_height},
	{"dw",             ftk_xul_builder_get_display_width},
	{"display_width",  ftk_xul_builder_get_display_width},
	{"dh",             ftk_xul_builder_get_display_height},
	{"display_height", ftk_xul_builder_get_display_height},
	{NULL, NULL} 
};

static int ftk_xul_find_getter(const char* name)
{
	int i = 0;
	return_val_if_fail(name != NULL, 0);

	for(i = 0; s_var_getters[i].name != NULL; i++)
	{
		if(strncmp(s_var_getters[i].name, name, strlen(s_var_getters[i].name)) == 0)
		{
			return i;
		}
	}

	return -1;
}

typedef struct _VarConst
{
	const char* name;
	int value;
}VarConst;

static const VarConst s_var_conts[] =
{
	{"FTK_ATTR_TRANSPARENT",    FTK_ATTR_TRANSPARENT},
	{"FTK_ATTR_BG_CENTER",      FTK_ATTR_BG_CENTER},
	{"FTK_ATTR_BG_TILE",        FTK_ATTR_BG_TILE},
	{"FTK_ATTR_BG_FOUR_CORNER", FTK_ATTR_BG_FOUR_CORNER},
	{"FTK_ATTR_NO_FOCUS",       FTK_ATTR_NO_FOCUS},
	{"FTK_ATTR_INSENSITIVE",    FTK_ATTR_INSENSITIVE},
	{"FTK_ATTR_FOCUSED",        FTK_ATTR_FOCUSED},
	{NULL, 0},
};

static int ftk_xul_find_const(const char* name)
{
	int i = 0;
	for(i = 0; s_var_conts[i].name != NULL; i++)
	{
		if(strcmp(s_var_conts[i].name, name) == 0)
		{
			return i;
		}
	}

	return -1;
}

static const char* ftk_xul_builder_preprocess_value(FtkXmlBuilder* thiz, const char* value)
{
	int i = 0;
	int dst = 0;
	DECL_PRIV(thiz, priv);
	const char* iter = NULL;
	return_val_if_fail(value != NULL, NULL);

	for(iter = value; *iter && dst < sizeof(priv->processed_value); iter++)
	{
		if(*iter == '$')
		{
			if((i = ftk_xul_find_getter(iter+1)) >= 0)
			{
				dst += snprintf(priv->processed_value+dst, sizeof(priv->processed_value)-dst,
					"%d", s_var_getters[i].get(thiz));
				iter += strlen(s_var_getters[i].name);
				continue;
			}
			else if((i = ftk_xul_find_const(iter+1)) >= 0)
			{
				dst += snprintf(priv->processed_value+dst, sizeof(priv->processed_value)-dst,
					"%d", s_var_conts[i].value);
				iter += strlen(s_var_getters[i].name);
				continue;
			}
		}

		priv->processed_value[dst++] = *iter;
	}
	priv->processed_value[dst] = '\0';

	return priv->processed_value;
}

static void ftk_xul_builder_init_widget_info(FtkXmlBuilder* thiz, const char** attrs, FtkWidgetCreateInfo* info)
{
	int i = 0;
	const char* name = NULL;
	const char* value = NULL;
	DECL_PRIV(thiz, priv);
	return_if_fail(attrs != NULL && info != NULL);

	info->parent = priv->current;
	for(i = 0; attrs[i] != NULL; i += 2)
	{
		name = attrs[i];
		value = attrs[i+1];

		switch(name[0])
		{
			case 'i':
			{
				info->id = atoi(value);
				break;
			}
			case 'x':
			{
				value = ftk_xul_builder_preprocess_value(thiz, value);
				info->x = (int)ftk_expr_eval(value);
				break;
			}
			case 'y':
			{
				value = ftk_xul_builder_preprocess_value(thiz, value);
				info->y = (int)ftk_expr_eval(value);
				break;
			}
			case 'w':
			{
				value = ftk_xul_builder_preprocess_value(thiz, value);
				info->w = (int)ftk_expr_eval(value);
				break;
			}
			case 'h':
			{
				value = ftk_xul_builder_preprocess_value(thiz, value);
				info->h = (int)ftk_expr_eval(value);
				break;
			}
			case 'a':
			{
				/*attr*/
				value = ftk_xul_builder_preprocess_value(thiz, value);
				info->attr = (int)ftk_expr_eval(value);
				break;
			}
			case 'v':
			{
				if(name[1] == 'a')
				{
					info->value = value;
				}
				else if(name[1] == 'i')
				{
					info->visible = atoi(value);
				}
				break;
			}
			case 'm':
			{
				info->max_value = atoi(value);
				break;
			}
			case 'p':
			{
				info->page_delta = atoi(value);
				break;
			}
			case 'c':
			{
				info->checked = atoi(value);
				break;
			}
			case 'b':
			{
				/*TODO: backgroup*/
				break;
			}
			case 'f':
			{
				/*TODO: foreground*/
				break;
			}
			default:break;/*TODO: handle other attrs*/
		}
	}

	return;
}

static void ftk_xul_builder_reset_widget_info(FtkXmlBuilder* thiz, FtkWidgetCreateInfo* info)
{
	/*TODO*/
	return;
}

static void ftk_xul_builder_on_start(FtkXmlBuilder* thiz, const char* tag, const char** attrs)
{
	DECL_PRIV(thiz, priv);
	FtkWidget* widget = NULL;
	FtkWidgetCreateInfo info = {0};
	FtkXulWidgetCreate create = ftk_xul_find_creator(tag);
	
	priv->meet_start_tag = 0;
	return_if_fail(create != NULL && attrs != NULL && thiz != NULL);

	ftk_xul_builder_init_widget_info(thiz, attrs, &info);
	if((widget = create(&info)) != NULL)
	{
		ftk_widget_set_id(widget, info.id);
		ftk_widget_set_attr(widget, info.attr);
		if(info.visible)
		{
			ftk_widget_show(widget, info.visible);
		}
	}
	ftk_xul_builder_reset_widget_info(thiz, &info);
	return_if_fail(widget != NULL);

	priv->current = widget;
	priv->meet_start_tag = 1;

	if(priv->root == NULL)
	{
	 	priv->root = widget;
	}

	return;
}

static void ftk_xul_builder_on_end(FtkXmlBuilder* thiz, const char* tag)
{
	DECL_PRIV(thiz, priv);

	if(priv->meet_start_tag)
	{
		priv->current = ftk_widget_parent(priv->current);
	}

	return;
}

static void ftk_xul_builder_on_text(FtkXmlBuilder* thiz, const char* text, size_t length)
{
	return;
}

static void ftk_xul_builder_on_comment(FtkXmlBuilder* thiz, const char* text, size_t length)
{
	return;
}

static void ftk_xul_builder_on_pi(FtkXmlBuilder* thiz, const char* tag, const char** attrs)
{
	return;
}

static void ftk_xul_builder_on_error(FtkXmlBuilder* thiz, int line, int row, const char* message)
{
	return;
}

static void ftk_xul_builder_destroy(FtkXmlBuilder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(FtkXmlBuilder) + sizeof(PrivInfo));
	}

	return;
}

static FtkXmlBuilder* ftk_xul_builder_create(void)
{
	FtkXmlBuilder* thiz = FTK_ZALLOC(sizeof(FtkXmlBuilder) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		thiz->on_start_element = ftk_xul_builder_on_start;
		thiz->on_end_element   = ftk_xul_builder_on_end;
		thiz->on_text          = ftk_xul_builder_on_text;
		thiz->on_comment       = ftk_xul_builder_on_comment;
		thiz->on_pi_element    = ftk_xul_builder_on_pi;
		thiz->on_error         = ftk_xul_builder_on_error;
		thiz->destroy          = ftk_xul_builder_destroy;
	}

	return thiz;
}

FtkWidget* ftk_xul_load(const char* xml, int length)
{
	FtkWidget* widget = NULL;
	FtkXmlParser* parser = NULL;
	FtkXmlBuilder* builder = NULL;
	return_val_if_fail(xml != NULL, NULL);

	parser  = ftk_xml_parser_create();
	return_val_if_fail(parser != NULL, NULL);

	builder = ftk_xul_builder_create();

	if(builder != NULL)
	{
		PrivInfo* priv = (PrivInfo*)builder->priv;
		ftk_xml_parser_set_builder(parser, builder);
		ftk_xml_parser_parse(parser, xml);
		widget = priv->root;
	}
	ftk_xml_builder_destroy(builder);
	ftk_xml_parser_destroy(parser);

	return widget;
}


