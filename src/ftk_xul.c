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
#include "ftk_xml_parser.h"

typedef struct _PrivInfo
{
	int skip_end_tag;
	FtkWidget* root;
	FtkWidget* current;
}PrivInfo;

typedef struct _FtkWidgetCreateInfo
{
	int id;
	int x;
	int y;
	int w;
	int h;
	const char* value;
	FtkWidget* parent;
	FtkGc gcs[FTK_WIDGET_STATE_NR];
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
	ftk_widget_set_id(widget, info->id);
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
	ftk_widget_set_id(widget, info->id);
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
	ftk_widget_set_id(widget, info->id);
	
	if(info->value != NULL)
	{
		ftk_entry_set_text(widget, info->value);
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
	{"label",   ftk_xul_label_create},
	{"entry",   ftk_xul_entry_create},
	{"button",  ftk_xul_button_create},
	{"window",   ftk_xul_window_create},
	{"dialog",   ftk_xul_dialog_create},
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
				info->x = atoi(value);
				break;
			}
			case 'y':
			{
				info->y = atoi(value);
				break;
			}
			case 'w':
			{
				info->w = atoi(value);
				break;
			}
			case 'h':
			{
				info->h = atoi(value);
				break;
			}
			case 'v':
			{
				info->value = value;
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
	FtkWidgetCreateInfo info = {0};
	FtkWidget* widget = NULL;
	DECL_PRIV(thiz, priv);
	FtkXulWidgetCreate create = ftk_xul_find_creator(tag);

	priv->skip_end_tag = 1;
	return_if_fail(create != NULL && attrs != NULL);

	ftk_xul_builder_init_widget_info(thiz, attrs, &info);
	widget = create(&info);
	ftk_xul_builder_reset_widget_info(thiz, &info);
	return_if_fail(widget != NULL);

	priv->current = widget;
	priv->skip_end_tag = 0;
	if(priv->root == NULL)
	{
	 	priv->root = widget;
	}

	return;
}

static void ftk_xul_builder_on_end(FtkXmlBuilder* thiz, const char* tag)
{
	DECL_PRIV(thiz, priv);

	if(!priv->skip_end_tag)
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


