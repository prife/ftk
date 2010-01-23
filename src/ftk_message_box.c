/*
 * File: ftk_messagebox.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  messagebox
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
 * 2010-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_label.h"
#include "ftk_globals.h"
#include "ftk_button.h"
#include "ftk_message_box.h"

static Ret message_button_clicked(void* ctx, void* obj)
{
	*(int*)ctx = ftk_widget_id(obj);

	return RET_QUIT;
}

#define FTK_BUTTON_DEFAULT_HEIGHT 48

int ftk_message_box_height(const char* text)
{
	int start = 0;
	int width = 0;
	int height = 0;
	const char* end = text;
	FtkFont* font = ftk_default_font();

	height = 5 * FTK_DIALOG_MARGIN + FTK_DIALOG_TITLE_HEIGHT + FTK_BUTTON_DEFAULT_HEIGHT;
	width = ftk_display_width(ftk_default_display());

	width -= FTK_LABEL_LEFT_MARGIN * 2;
	while(*end != '\0')
	{
		height += ftk_font_height(font) + FTK_LABEL_TOP_MARGIN;
		end = ftk_font_calc_str_visible_range(font, text, start, -1, width);
		start = end - text;
	}

	return height;
}

int ftk_message_box(FtkBitmap* icon, const char* title, const char* text, const char* buttons[FTK_MSGBOX_MAX_BUTTONS])
{
	int i = 0;
	int w = 0;
	int h = 0;
	int id = 0;
	int ret = 0;
	int width = 0;
	int xoffset = 0;
	int yoffset = 0;
	int buttons_nr = 0;
	int height = ftk_message_box_height(text);

	FtkWidget* label = NULL;
	FtkWidget* button = NULL;
	FtkWidget* dialog = NULL;

	return_val_if_fail(text != NULL, -1);

	dialog = ftk_dialog_create(0, 40, 320, height);
	ftk_dialog_set_icon(dialog, icon);
	ftk_widget_set_text(dialog, title);

	width = ftk_widget_width(dialog);
	height = ftk_widget_height(dialog);

	w = width - 2 * FTK_DIALOG_MARGIN;
	h = height - FTK_DIALOG_TITLE_HEIGHT - FTK_DIALOG_MARGIN - FTK_BUTTON_DEFAULT_HEIGHT - 4;

	label = ftk_label_create(dialog, xoffset, yoffset, w, h);
	ftk_widget_set_text(label, "Are you sure to quit?");

	for(i = 0; buttons[i] != NULL && i < FTK_MSGBOX_MAX_BUTTONS; i++)
	{
		buttons_nr++;
	}

	h = 50;
	w = 80;
	xoffset = 0;
	yoffset = height - FTK_DIALOG_TITLE_HEIGHT - h - FTK_DIALOG_MARGIN;

	switch(buttons_nr)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			w = (3 * w) < width ? w : width / 3;
			break;
		}
		case 3:
		default:
		{
			buttons_nr = 3;
			w = (4 * w) < width ? w : width / 4;
			break;
		}
	}

	for(i = 0; i < buttons_nr; i++)
	{
		xoffset += (width - buttons_nr * w) / (buttons_nr + 1);
		button = ftk_button_create(dialog, xoffset, yoffset, w, h);
		ftk_widget_set_id(button, i + 1);
		ftk_widget_set_text(button, buttons[i]);
		ftk_button_set_clicked_listener(button, message_button_clicked, &id);
		xoffset += w;
	}
	ftk_widget_show_all(dialog, 1);

	ret = ftk_dialog_run(dialog);

	ftk_widget_unref(dialog);

	return ret;
}

int ftk_warning(const char* title, const char* text, const char* buttons[FTK_MSGBOX_MAX_BUTTONS])
{
	FtkBitmap* icon = ftk_theme_load_image(ftk_default_theme(), "warning"FTK_STOCK_IMG_SUFFIX); 

	return ftk_message_box(icon, title, text, buttons);
}

int ftk_question(const char* title, const char* text, const char* buttons[FTK_MSGBOX_MAX_BUTTONS])
{
	FtkBitmap* icon = ftk_theme_load_image(ftk_default_theme(), "question"FTK_STOCK_IMG_SUFFIX); 

	return ftk_message_box(icon, title, text, buttons);
}

int ftk_infomation(const char* title, const char* text, const char* buttons[FTK_MSGBOX_MAX_BUTTONS])
{
	FtkBitmap* icon = ftk_theme_load_image(ftk_default_theme(), "info"FTK_STOCK_IMG_SUFFIX); 

	return ftk_message_box(icon, title, text, buttons);
}


