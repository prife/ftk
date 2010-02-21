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


#ifndef FTK_INPUT_METHOD_PREEDITOR_H
#define FTK_INPUT_METHOD_PREEDITOR_H

#include "ftk_widget.h"

FTK_BEGIN_DECLS

FtkWidget* ftk_input_method_preeditor_create(void);
Ret ftk_input_method_preeditor_reset(FtkWidget* thiz);
Ret ftk_input_method_preeditor_disconnect(FtkWidget* thiz);
Ret ftk_input_method_preeditor_connect(FtkWidget* thiz, FtkWidget* editor);
Ret ftk_input_method_preeditor_set_raw_text(FtkWidget* thiz, const char* text);
Ret ftk_input_method_preeditor_add_candidate(FtkWidget* thiz, const char* text);

Ret ftk_input_method_preeditor_show(FtkWidget* editor, FtkPoint* caret_pos, FtkCommitInfo* info);

FTK_END_DECLS

#endif/*FTK_INPUT_METHOD_PREEDITOR_H*/

