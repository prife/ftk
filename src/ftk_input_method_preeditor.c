/*
 * File: ftk_input_method_preeditor.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   input candidate selector.
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
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
 * 2010-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_button.h"
#include "ftk_window.h"
#include "ftk_globals.h"
#include "ftk_list_view.h"
#include "ftk_text_buffer.h"
#include "ftk_list_model_default.h"
#include "ftk_list_render_default.h"
#include "ftk_input_method_preeditor.h"

typedef struct _PrivInfo
{
	FtkWidget* editor;
	FtkListModel* model;
	FtkWidget* raw_text_button;
	FtkTextBuffer* text_buffer;
}PrivInfo;

#define IDC_LIST  1001
#define IDC_RAW_TEXT 1000
#define FTK_IM_PREEDITOR_WIDTH       120
#define FTK_IM_PREEDITOR_MAX_ROW     5
#define FTK_IM_PREEDITOR_ITEM_HEIGHT 20
#define FTK_ROW_HEIGHT (ftk_font_height(ftk_default_font()) + 2 * FTK_V_MARGIN)

static void list_item_info_destroy(void* user_data)
{
	return;
}

static Ret ftk_popup_on_raw_text_clicked(void* ctx, void* button)
{
	PrivInfo* priv = ctx;
	FtkEvent event = {0};
	
	event.widget   = priv->editor;
	event.u.extra  = (char*)ftk_widget_get_text(button);
	event.type     = FTK_EVT_IM_COMMIT;

	ftk_widget_event(priv->editor, &event);
	ftk_widget_show_all(ftk_widget_toplevel(button), 0);
	ftk_widget_set_text(button, "");

	return RET_OK;
}

static Ret ftk_popup_on_item_clicked(void* ctx, void* list)
{
	PrivInfo* priv = ctx;
	FtkListItemInfo* info = NULL;
	FtkListModel* model = priv->model;
	int i = ftk_list_view_get_selected(list);
	
	ftk_list_model_get_data(model, i, (void**)&info);
	if(info != NULL)
	{
		FtkEvent event = {0};
		event.widget   = priv->editor;
		event.u.extra  = (void*)info->text;
		event.type     = FTK_EVT_IM_COMMIT;

		ftk_widget_event(priv->editor, &event);
	}
	ftk_widget_show_all(ftk_widget_toplevel(list), 0);

	return RET_OK;
}

static Ret ftk_input_method_preeditor_resize(FtkWidget* thiz, int candidate_nr, FtkPoint* caret)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	FtkRect work_rect = {0};

	w = FTK_IM_PREEDITOR_WIDTH;
	h = (candidate_nr + 1) * FTK_IM_PREEDITOR_ITEM_HEIGHT + 2 * FTK_V_MARGIN;
	ftk_wnd_manager_get_work_area(ftk_default_wnd_manager(), &work_rect);

	if((caret->y + h + FTK_ROW_HEIGHT) <= (work_rect.y + work_rect.height))
	{
		y = caret->y + FTK_ROW_HEIGHT;
	}
	else
	{
		y = caret->y - h;
	}

	if((caret->x + w) <= (work_rect.x + work_rect.width))
	{
		x = caret->x;
	}
	else
	{
		x = work_rect.x + work_rect.width - w;
	}

	ftk_widget_move_resize(thiz, x, y, w, h);
	ftk_widget_move_resize(ftk_widget_lookup(thiz, IDC_RAW_TEXT), 0, 
		0, w, FTK_IM_PREEDITOR_ITEM_HEIGHT);
	ftk_widget_move_resize(ftk_widget_lookup(thiz, IDC_LIST), 0, 
		FTK_IM_PREEDITOR_ITEM_HEIGHT, w, h - FTK_IM_PREEDITOR_ITEM_HEIGHT);

	return RET_OK;
}

static void ftk_free_priv(void* data)
{
	PrivInfo* priv = data;

	if(priv != NULL)
	{
		ftk_list_model_unref(priv->model);
		ftk_text_buffer_destroy(priv->text_buffer);
		FTK_FREE(priv);
	}

	return;
}

FtkWidget* ftk_input_method_preeditor_create(void)
{
	int x = 0;
	int y = 0;
	FtkGc gc = {0};
	PrivInfo*  priv  = NULL;
	FtkWidget* list  = NULL;
	FtkWidget* button = NULL;
	FtkWidget* popup = NULL;
	FtkListModel* model = NULL;
	FtkListRender* render = NULL;
	int w = FTK_IM_PREEDITOR_WIDTH;
	int h = FTK_IM_PREEDITOR_ITEM_HEIGHT * FTK_IM_PREEDITOR_MAX_ROW;

	model = ftk_list_model_default_create(10, list_item_info_destroy);
	return_val_if_fail(model != NULL, NULL);

	popup = ftk_window_create_ex(FTK_WINDOW_ANY, 0, x, y, w, h);
	ftk_widget_set_text(popup, "im preeditor");
	render = ftk_list_render_default_create();
	button = ftk_button_create(popup, 0, 0, w, FTK_IM_PREEDITOR_ITEM_HEIGHT);
	ftk_widget_set_id(button, IDC_RAW_TEXT);
	ftk_widget_unset_attr(button, FTK_ATTR_TRANSPARENT);

	gc.mask = FTK_GC_BG | FTK_GC_FG;
	gc.bg = ftk_theme_get_bg_color(ftk_default_theme(), FTK_BUTTON, FTK_WIDGET_NORMAL);
	gc.fg = ftk_theme_get_fg_color(ftk_default_theme(), FTK_BUTTON, FTK_WIDGET_NORMAL);
	ftk_widget_reset_gc(button, FTK_WIDGET_NORMAL, &gc);
	
	gc.bg = ftk_theme_get_bg_color(ftk_default_theme(), FTK_BUTTON, FTK_WIDGET_FOCUSED);
	gc.fg = ftk_theme_get_fg_color(ftk_default_theme(), FTK_BUTTON, FTK_WIDGET_FOCUSED);
	ftk_widget_reset_gc(button, FTK_WIDGET_FOCUSED, &gc);
	
	gc.bg = ftk_theme_get_bg_color(ftk_default_theme(), FTK_BUTTON, FTK_WIDGET_ACTIVE);
	gc.fg = ftk_theme_get_fg_color(ftk_default_theme(), FTK_BUTTON, FTK_WIDGET_ACTIVE);
	ftk_widget_reset_gc(button, FTK_WIDGET_ACTIVE, &gc);

	list = ftk_list_view_create(popup, 0, FTK_IM_PREEDITOR_ITEM_HEIGHT, 
		w, h - FTK_IM_PREEDITOR_ITEM_HEIGHT);
	ftk_widget_set_id(list, IDC_LIST);
	ftk_window_set_focus(popup, list);

	ftk_list_view_init(list, model, render, FTK_IM_PREEDITOR_ITEM_HEIGHT);

	if((priv = FTK_ZALLOC(sizeof(PrivInfo))) != NULL)
	{
		priv->model = model;
		priv->raw_text_button = button;
		priv->text_buffer = ftk_text_buffer_create(256);
		ftk_widget_set_user_data(popup, ftk_free_priv, priv);
		ftk_button_set_clicked_listener(button, ftk_popup_on_raw_text_clicked, priv);
	}
	ftk_list_view_set_clicked_listener(list, ftk_popup_on_item_clicked, priv);

	return popup;
}

Ret ftk_input_method_preeditor_reset(FtkWidget* thiz)
{
	PrivInfo* priv = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv = ftk_widget_user_data(thiz);
	return_val_if_fail(priv != NULL, RET_FAIL);

	ftk_list_model_reset(priv->model);
	ftk_text_buffer_reset(priv->text_buffer);
	ftk_widget_set_text(priv->raw_text_button, "          ");

	return RET_OK;
}

Ret ftk_input_method_preeditor_connect(FtkWidget* thiz, FtkWidget* editor)
{
	PrivInfo* priv = NULL;
	return_val_if_fail(thiz != NULL && editor != NULL, RET_FAIL);

	priv = ftk_widget_user_data(thiz);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->editor = editor;

	return RET_OK;
}

Ret ftk_input_method_preeditor_disconnect(FtkWidget* thiz)
{
	PrivInfo* priv = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv = ftk_widget_user_data(thiz);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->editor = NULL;
	ftk_widget_show_all(thiz, 0);

	return RET_OK;
}

Ret ftk_input_method_preeditor_set_raw_text(FtkWidget* thiz, const char* text)
{
	PrivInfo* priv = NULL;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	priv = ftk_widget_user_data(thiz);
	return_val_if_fail(priv != NULL, RET_FAIL);

	ftk_widget_set_text(priv->raw_text_button, text);

	return RET_OK;
}

Ret ftk_input_method_preeditor_add_candidate(FtkWidget* thiz, const char* text)
{
	PrivInfo* priv = NULL;
	FtkListItemInfo info = {0};
	return_val_if_fail(thiz != NULL && text != NULL, RET_FAIL);
	
	priv = ftk_widget_user_data(thiz);
	return_val_if_fail(priv != NULL, RET_FAIL);

	info.user_data = thiz;
	info.type = FTK_LIST_ITEM_NORMAL;
	info.text = ftk_text_buffer_append_string(priv->text_buffer, text);

	return ftk_list_model_add(priv->model, &info);
}

Ret ftk_input_method_preeditor_show(FtkWidget* editor, FtkPoint* caret_pos, FtkCommitInfo* info)
{
	int i = 0;
	int candidate_nr = 0;
	const char* item = NULL;
	FtkWidget* preeditor = ftk_default_input_method_preeditor();

	return_val_if_fail(info != NULL && preeditor != NULL, RET_FAIL);

	ftk_widget_show_all(preeditor, 0);

	if(info->raw_text[0] == '\0' && info->candidate_nr == 0)
	{
		return RET_OK;
	}

	ftk_input_method_preeditor_reset(preeditor);
	ftk_input_method_preeditor_connect(preeditor, editor);
	ftk_input_method_preeditor_set_raw_text(preeditor, info->raw_text);
	ftk_wnd_manager_restack(ftk_default_wnd_manager(), preeditor, FTK_MAX_WINDOWS);
	
	item = info->candidates;
	for(i = 0; i < info->candidate_nr; i++)
	{
		ftk_input_method_preeditor_add_candidate(preeditor, item);
		item += strlen(item) + 1;
	}
	
	candidate_nr = FTK_MIN(info->candidate_nr, FTK_IM_PREEDITOR_MAX_ROW);
	ftk_input_method_preeditor_resize(preeditor, candidate_nr, caret_pos);

	ftk_widget_show_all(preeditor, 1);

	return RET_OK;
}

