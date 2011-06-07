/*
 * File: ftk_source_input.c
 * Author:  Han.hui <sylixos@gmail.com>
 * Brief:   source to handle /dev/input/xx
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
 * 2011-05-15 Han.hui <sylixos@gmail.com> created
 *
 */

#include "ftk_log.h"
#include "ftk_key.h"
#include "ftk_display.h"
#include "ftk_globals.h"
#include <string.h>
#include <keyboard.h>
#include <mouse.h>
#include <input_device.h>
#include "ftk_source_sylixos.h"

#define MAX_INPUT_DEVICE    5

static FtkKey   f_key_map[512];

static FtkEvent eventMouse;
static FtkEvent eventKeyborad;

static char     cKeyboradFiles[PATH_MAX + 1];
static char     cMouseFiles[PATH_MAX + 1];

static void ftk_source_sylixos_keyboard_initmap (void)
{
    int  i;
    static int  init = 0;

    if (init == 0) {
        init =  1;
    } else {
        return;
    }

    for (i = 0; i <= 0x7f; i++) {
        f_key_map[i] = i;
    }

    f_key_map[K_F1]          =  FTK_KEY_F1;
    f_key_map[K_F2]          =  FTK_KEY_F2;
    f_key_map[K_F3]          =  FTK_KEY_F3;
    f_key_map[K_F4]          =  FTK_KEY_F4;
    f_key_map[K_F5]          =  FTK_KEY_F5;
    f_key_map[K_F6]          =  FTK_KEY_F6;
    f_key_map[K_F7]          =  FTK_KEY_F7;
    f_key_map[K_F8]          =  FTK_KEY_F8;
    f_key_map[K_F9]          =  FTK_KEY_F9;
    f_key_map[K_F10]         =  FTK_KEY_F10;

    f_key_map[K_ESC]         =  FTK_KEY_ESC;
    f_key_map[K_TAB]         =  FTK_KEY_TAB;
    f_key_map[K_CAPSLOCK]    =  FTK_KEY_CAPSLOCK;
    f_key_map[K_SHIFT]       =  FTK_KEY_LEFTSHIFT;
    f_key_map[K_CTRL]        =  FTK_KEY_LEFTCTRL;
    f_key_map[K_ALT]         =  FTK_KEY_LEFTALT;
    f_key_map[K_SHIFTR]      =  FTK_KEY_RIGHTSHIFT;
    f_key_map[K_CTRLR]       =  FTK_KEY_RIGHTCTRL;
    f_key_map[K_ALTR]        =  FTK_KEY_RIGHTALT;
    f_key_map[K_ENTER]       =  FTK_KEY_ENTER;
    f_key_map[K_BACKSPACE]   =  FTK_KEY_BACKSPACE;

    f_key_map[K_PRINTSCREEN] = 0;
    f_key_map[K_SCROLLLOCK]  = 0;
    f_key_map[K_PAUSEBREAK]  = 0;

    f_key_map[K_INSERT]      = FTK_KEY_INSERT;
    f_key_map[K_HOME]        = FTK_KEY_HOME;
    f_key_map[K_DELETE]      = FTK_KEY_DELETE;
    f_key_map[K_END]         = FTK_KEY_END;
    f_key_map[K_PAGEUP]      = FTK_KEY_PAGEUP;
    f_key_map[K_PAGEDOWN]    = FTK_KEY_PAGEDOWN;

    f_key_map[K_UP]          = FTK_KEY_UP;
    f_key_map[K_DOWN]        = FTK_KEY_DOWN;
    f_key_map[K_LEFT]        = FTK_KEY_LEFT;
    f_key_map[K_RIGHT]       = FTK_KEY_RIGHT;
}

static int ftk_source_sylixos_key_map (int key)
{
    return f_key_map[key] != 0 ? f_key_map[key] : key;
}

static void  ftk_source_sylixos_on_pointer_event (mouse_event_notify  *pmnotify)
{
    if (pmnotify) {

        eventMouse.type = FTK_EVT_NOP;

        if (pmnotify->ctype == MOUSE_CTYPE_REL) { /* relative coordinate */
            eventMouse.u.mouse.x += pmnotify->xmovement;
            eventMouse.u.mouse.y += pmnotify->ymovement;

        } else { /* absolutely coordinate */
            eventMouse.u.mouse.x = 10;  /* temp */
            eventMouse.u.mouse.y = 10;
        }

        if (pmnotify->kstat & MOUSE_LEFT) {
            if (eventMouse.u.mouse.press) {
                eventMouse.type = FTK_EVT_MOUSE_MOVE;
            } else {
                eventMouse.type = FTK_EVT_MOUSE_DOWN;
                eventMouse.u.mouse.press = 1;
            }
        } else {
            if (eventMouse.u.mouse.press) {
                eventMouse.type = FTK_EVT_MOUSE_UP;
                eventMouse.u.mouse.press = 0;
            }
        }

        if (eventMouse.type != FTK_EVT_NOP) {
            ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &eventMouse);
        }
    }
}

static void  ftk_source_sylixos_on_keyboard_event (keyboard_event_notify  *pknotify)
{
    if (pknotify) {

        if (pknotify->type == KE_PRESS) {
            eventKeyborad.type = FTK_EVT_KEY_DOWN;
        } else {
            eventKeyborad.type = FTK_EVT_KEY_UP;
        }
        eventKeyborad.u.key.code = ftk_source_sylixos_key_map(pknotify->keymsg[0]);

        ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &eventKeyborad);
    }
}

static int ftk_source_sylixos_input_init (char  *file, const char  *file_list[])
{
    int      i = 0;
    char    *divptr = file;
    char    *start;

    do {
        start  = divptr;
        divptr = strchr(start, ':');
        if (divptr) {
            *divptr = '\0';
            divptr++;
        }

        if (i < MAX_INPUT_DEVICE) {
            file_list[i] = start;
        } else {
            break;
        }
    } while (divptr);

    return  (i);
}

void ftk_source_sylixos_input_create (void)
{
    LW_CLASS_THREADATTR   attr = Lw_ThreadAttr_GetDefault();
    int                   max_keyboard_num;
    int                   max_mouse_num;

    const char           *pcKeyboradArray[MAX_INPUT_DEVICE];
    const char           *pcMouseArray[MAX_INPUT_DEVICE];

    ftk_source_sylixos_keyboard_initmap();

    if (getenv_r("KEYBOARD", cKeyboradFiles, PATH_MAX + 1) >= 0) {
        max_keyboard_num = ftk_source_sylixos_input_init(cKeyboradFiles, pcKeyboradArray);
    } else {
        pcKeyboradArray[0] = "/dev/input/keyboard0";
        max_keyboard_num = 1;
    }

    if (getenv_r("MOUSE", cMouseFiles, PATH_MAX + 1) >= 0) {
        max_mouse_num = ftk_source_sylixos_input_init(cMouseFiles, pcMouseArray);
    } else {
        pcMouseArray[0] = "/dev/input/mouse0";
        max_mouse_num = 1;
    }

    guiInputDevReg(pcKeyboradArray, max_keyboard_num, pcMouseArray, max_mouse_num);
    guiInputDevKeyboardHookSet(ftk_source_sylixos_on_keyboard_event);
    guiInputDevMouseHookSet(ftk_source_sylixos_on_pointer_event);
    guiInputDevProcStart(&attr);
}
