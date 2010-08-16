/*
 * File: ftk_input_method_py.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   pinyin wubi input method.
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
 * 2010-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_event.h"
#include "ftk_globals.h"
#include "ftk_lookup_table.h"
#include "ftk_input_method_py.h"
#include "ftk_input_method_util.h"

typedef struct _PrivInfo
{
	int raw_text_size;
	int candidate_size;

	FtkWidget* editor;
	FtkCompare compare;
	const char* filename;
	FtkInputType input_type;
	FtkCommitInfo commit_info;
	FtkLookupTable* lookup_table;
}PrivInfo;

static Ret  ftk_input_method_py_handle_event(FtkInputMethod* thiz, FtkEvent* event);

static Ret  ftk_input_method_py_init(FtkInputMethod* thiz)
{
	DECL_PRIV(thiz, priv);
	char filename[FTK_MAX_PATH+1] = {0};
	if(priv->lookup_table != NULL) return RET_OK;
	return_val_if_fail(priv->filename != NULL, RET_FAIL);
	
	ftk_snprintf(filename, sizeof(filename), "%s/data/%s", DATA_DIR, priv->filename);
	if((priv->lookup_table = ftk_lookup_table_create(filename, 1, priv->compare)) == NULL)
	{
		ftk_snprintf(filename, sizeof(filename), "%s/data/%s", LOCAL_DATA_DIR, priv->filename);
		priv->lookup_table = ftk_lookup_table_create(filename, 1, priv->compare);
	}

	return RET_OK;
}

static Ret  ftk_input_method_py_reset(FtkInputMethod* thiz)
{

	return RET_OK;
}

static Ret  ftk_input_method_py_activate(FtkInputMethod* thiz)
{
	ftk_input_method_py_init(thiz);

	ftk_wnd_manager_add_global_listener(ftk_default_wnd_manager(),
		(FtkListener)ftk_input_method_py_handle_event, thiz);

	ftk_logd("%s\n", __func__);

	return RET_OK;
}

static Ret  ftk_input_method_py_deactivate(FtkInputMethod* thiz)
{
	ftk_input_method_py_reset(thiz);
	
	ftk_wnd_manager_remove_global_listener(ftk_default_wnd_manager(), 
		(FtkListener)ftk_input_method_py_handle_event, thiz);

	ftk_logd("%s\n", __func__);

	return RET_OK;
}

static Ret  ftk_input_method_py_focus_in(FtkInputMethod* thiz, FtkWidget* editor)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL && editor != NULL, RET_FAIL);

	priv->editor = editor;

	return ftk_input_method_py_activate(thiz);
}

static Ret  ftk_input_method_py_focus_out(FtkInputMethod* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->editor = NULL;
	
	return ftk_input_method_py_deactivate(thiz);
}

static Ret  ftk_input_method_py_set_type(FtkInputMethod* thiz, FtkInputType input_type)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL, RET_FAIL);

	priv->input_type = input_type;

	return RET_OK;
}

#define IS_IME_CHAR(c) ((c < 0xff) && (((c) >= 'a' && (c) <= 'z') || (c) == '\''))

static Ret  ftk_input_method_py_handle_event(FtkInputMethod* thiz, FtkEvent* event)
{
	Ret ret = RET_OK;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(priv != NULL && event != NULL, RET_FAIL);
	
	switch(event->type)
	{
		case FTK_EVT_KEY_DOWN:
		{
			FtkEvent evt = {0};
			const char* matched = NULL;
			int code = event->u.key.code;
		
			if((code != FTK_KEY_BACKSPACE) && !IS_IME_CHAR(code))
			{
				break;
			}

			if(code == FTK_KEY_BACKSPACE)
			{
				if(priv->raw_text_size == 0)
				{
					break;
				}
				else
				{
					priv->raw_text_size--;
					priv->commit_info.raw_text[priv->raw_text_size] = '\0';
				}
			}
			
			if(priv->raw_text_size >= FTK_IM_RAW_TEXT_LENGTH)
			{
				ret = RET_REMOVE;
				break;
			}

			if(IS_IME_CHAR(code))
			{
				priv->commit_info.raw_text[priv->raw_text_size++] = 0xff & (event->u.key.code);
				priv->commit_info.raw_text[priv->raw_text_size] = '\0';
			}

			priv->candidate_size = 0;
			priv->commit_info.candidate_nr = 0;
			if(priv->raw_text_size > 0)
			{
				matched = ftk_lookup_table_lookup(priv->lookup_table, priv->commit_info.raw_text);
			}

			if(matched != NULL)
			{
				ftk_im_candidate_info_parse(&(priv->commit_info), matched, priv->compare);
			}
			else
			{
				priv->commit_info.candidate_nr = (priv->raw_text_size) > 0 ? 1 : 0;
				strcpy(priv->commit_info.candidates, priv->commit_info.raw_text);
			}
			evt.type = FTK_EVT_IM_PREEDIT;
			evt.u.extra = &(priv->commit_info);
			evt.widget = priv->editor;

			ftk_widget_event(priv->editor, &evt);
			
			ret = RET_REMOVE;

			break;
		}
		case FTK_EVT_IM_ACT_COMMIT:
		{
			priv->raw_text_size = 0;
			priv->commit_info.raw_text[priv->raw_text_size] = '\0';

			break;
		}
		default:break;
	}

	return ret;
}

static void ftk_input_method_py_destroy(FtkInputMethod* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		ftk_lookup_table_destroy(priv->lookup_table);
		FTK_ZFREE(thiz, sizeof(FtkInputMethod) + sizeof(PrivInfo));
	}

	return;
}

FtkInputMethod* ftk_input_method_py_wb_create(const char* name, const char* filename, FtkCompare compare)
{
	FtkInputMethod* thiz = FTK_ZALLOC(sizeof(FtkInputMethod) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		thiz->ref  = 1;
		thiz->name = name;	
		priv->compare      = compare;
		priv->filename     = filename;
		thiz->set_type     = ftk_input_method_py_set_type;
		thiz->focus_in     = ftk_input_method_py_focus_in;
		thiz->focus_out    = ftk_input_method_py_focus_out;
		thiz->handle_event = ftk_input_method_py_handle_event;
		thiz->destroy      = ftk_input_method_py_destroy;
	}

	return thiz;
}

static int py_compare(const void* obj1, const void* obj2)
{
	return strncmp(obj1, obj2, strlen(obj2));
}

static int wb_compare(const void* obj1, const void* obj2)
{
	return strncmp(obj1, obj2, strlen(obj2));
}

FtkInputMethod* ftk_input_method_py_create(void)
{
	return ftk_input_method_py_wb_create(_("Pin Yin"), "py.bin", py_compare);
}

FtkInputMethod* ftk_input_method_wb_create(void)
{
	return ftk_input_method_py_wb_create(_("Wu Bi"), "wbx.bin", wb_compare);
}

