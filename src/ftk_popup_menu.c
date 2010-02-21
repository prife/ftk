/*
 * File: ftk_popup_menu.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   popup menu
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
 * 2010-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_dialog.h"
#include "ftk_list_view.h"
#include "ftk_popup_menu.h"
#include "ftk_list_render_default.h"

#define FTK_POPUP_MENU_LIST_ID 1000

static Ret ftk_popup_menu_on_item_clicked(void* ctx, void* list)
{
	FtkListItemInfo* info = NULL;
	int i = ftk_list_view_get_selected(list);
	FtkListModel* model = ftk_list_view_get_model(list);
	ftk_list_model_get_data(model, i, (void**)&info);
	
	if(info != NULL)
	{
		FtkListener listener = (FtkListener)info->extra_user_data;
		if(listener != NULL)
		{
			listener(info->user_data, info);
		}
	}

	ftk_widget_unref(ctx);

	ftk_logd("%s: %d/%d\n", __func__, i, ftk_list_model_get_total(model));

	return RET_OK;
}

FtkWidget* ftk_popup_menu_create(int x, int y, int w, int h, FtkBitmap* icon, const char* title)
{
	int has_title = icon != NULL || title != NULL;
	FtkWidget* thiz = ftk_dialog_create(x, y, w, h);

	if(has_title)
	{
		ftk_dialog_set_icon(thiz, icon);
		ftk_widget_set_text(thiz, title);
	}
	else
	{
		ftk_dialog_hide_title(thiz);
	}

	return thiz;
}

Ret ftk_popup_menu_init(FtkWidget* thiz, FtkListItemInfo* info, size_t nr, FtkDestroy on_item_destroy)
{
	int w = 0;
	int h = 0;
	size_t i = 0;
	FtkWidget* list = NULL;
	FtkListModel* model = NULL;
	FtkListRender* render = NULL;
	return_val_if_fail(thiz != NULL && info != NULL, RET_FAIL);
	return_val_if_fail(ftk_widget_lookup(thiz, FTK_POPUP_MENU_LIST_ID) == NULL, RET_FAIL);

	w = ftk_widget_width(thiz)  - 2 * FTK_DIALOG_BORDER;
	h = ftk_widget_height(thiz) - FTK_DIALOG_BORDER - FTK_DIALOG_TITLE_HEIGHT;

	list = ftk_list_view_create(thiz, 0, 0, w, h);
	ftk_widget_set_id(list, FTK_POPUP_MENU_LIST_ID);
	ftk_list_view_set_clicked_listener(list, ftk_popup_menu_on_item_clicked, thiz);
	ftk_window_set_focus(thiz, list);

	model = ftk_list_model_default_create(nr, on_item_destroy);
	render = ftk_list_render_default_create();

	for(i = 0; i < nr; i++)
	{
		ftk_list_model_add(model, info+i);
	}

	ftk_list_view_init(list, model, render, FTK_POPUP_MENU_ITEM_HEIGHT);
	ftk_list_model_unref(model);

	return RET_OK;
}

int ftk_popup_menu_get_selected(FtkWidget* thiz)
{
	FtkWidget* list = NULL;
	return_val_if_fail(thiz != NULL, -1);

	list = ftk_widget_lookup(thiz, FTK_POPUP_MENU_LIST_ID);
	return_val_if_fail(list != NULL, -1);

	return ftk_list_view_get_selected(list);

}

int ftk_popup_menu_calc_height(int visible_items)
{
	return FTK_DIALOG_TITLE_HEIGHT 
		+ 2 * FTK_DIALOG_BORDER + 
		visible_items * FTK_POPUP_MENU_ITEM_HEIGHT;
}

