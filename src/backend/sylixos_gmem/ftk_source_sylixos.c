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

/*
 *  tslib/plugins/linear.c
 *
 *  Copyright (C) 2001 Russell King.
 *
 * This file is placed under the LGPL.  Please see the file
 * COPYING for more details.
 *
 * $Id: linear.c,v 1.10 2005/02/26 01:47:23 kergoth Exp $
 *
 * Linearly scale touchscreen values
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

struct tslib_linear {
    int swap_xy;

    // Linear scaling and offset parameters for pressure
    int p_offset;
    int p_mult;
    int p_div;

    // Linear scaling and offset parameters for x,y (can include rotation)
    int a[7];
};

static struct tslib_linear f_linear_info;

static int linear_init(void)
{
    struct stat sbuf;
    int         cal_fd;
    char        namebuffer[PATH_MAX + 1];
    char*       name;
    int         index;
    char*       tokptr;

    // Use default values that leave ts numbers unchanged after transform
    f_linear_info.a[0] = 1;
    f_linear_info.a[1] = 0;
    f_linear_info.a[2] = 0;
    f_linear_info.a[3] = 0;
    f_linear_info.a[4] = 1;
    f_linear_info.a[5] = 0;
    f_linear_info.a[6] = 1;
    f_linear_info.p_offset = 0;
    f_linear_info.p_mult   = 1;
    f_linear_info.p_div    = 1;
    f_linear_info.swap_xy  = 0;

    /*
     * Check calibration file
     */
    if (getenv_r("TSLIB_CALIBFILE", namebuffer, PATH_MAX + 1) >= 0) {
        name = namebuffer;
    } else {
        name = FTK_ROOT_DIR"/pointercal";
    }

    if (stat(name, &sbuf) == 0) {

        cal_fd = open(name, O_RDONLY);

        read(cal_fd, namebuffer, 200);

        f_linear_info.a[0] = atoi(strtok(namebuffer, " "));

        index = 1;

        while (index < 7) {
            tokptr = strtok(NULL, " ");
            if (*tokptr != '\0') {
                f_linear_info.a[index] = atoi(tokptr);
                index++;
            }
        }

        printf("Linear calibration constants: ");
        for (index = 0; index < 7; index++) {
            printf("%d ", f_linear_info.a[index]);
        }
        printf("\n");

        close(cal_fd);
    }

    return 0;
}

static void ftk_source_sylixos_ad_to_abs(mouse_event_notify  *pmnotify)
{
    int xtemp, ytemp;

    xtemp = pmnotify->xanalog;
    ytemp = pmnotify->yanalog;

    eventMouse.u.mouse.x =   (f_linear_info.a[2] +
                              f_linear_info.a[0] * xtemp +
                              f_linear_info.a[1] * ytemp) / f_linear_info.a[6];
    eventMouse.u.mouse.y =   (f_linear_info.a[5] +
                              f_linear_info.a[3] * xtemp +
                              f_linear_info.a[4] * ytemp) / f_linear_info.a[6];

    if (f_linear_info.swap_xy) {
        int tmp = eventMouse.u.mouse.x;
        eventMouse.u.mouse.x = eventMouse.u.mouse.y;
        eventMouse.u.mouse.y = tmp;
    }
}

static void  ftk_source_sylixos_on_pointer_event (mouse_event_notify  *pmnotify)
{
    if (pmnotify) {

        eventMouse.type = FTK_EVT_NOP;

        if (pmnotify->ctype == MOUSE_CTYPE_REL) { /* relative coordinate */
            eventMouse.u.mouse.x += pmnotify->xmovement;
            eventMouse.u.mouse.y += pmnotify->ymovement;

        } else { /* absolutely coordinate */
            ftk_source_sylixos_ad_to_abs(pmnotify);
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

        ftk_logd("%s: sample.pressure=%d x=%d y=%d\n",
            __func__, eventMouse.u.mouse.press, eventMouse.u.mouse.x, eventMouse.u.mouse.y);

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

        i++;
    } while (divptr);

    return  (i);
}

static int ftk_source_sylixos_get_fd(FtkSource* thiz)
{
    return -1;
}

static int ftk_source_sylixos_check(FtkSource* thiz)
{
    return -1;
}

static Ret ftk_source_sylixos_dispatch(FtkSource* thiz)
{
    return RET_OK;
}

static void ftk_source_sylixos_destroy(FtkSource* thiz)
{
    guiInputDevKeyboardHookSet(NULL);
    guiInputDevMouseHookSet(NULL);
}

FtkSource* ftk_source_sylixos_input_create (void)
{
    LW_CLASS_THREADATTR   attr = Lw_ThreadAttr_GetDefault();
    int                   max_keyboard_num;
    int                   max_mouse_num;

    const char           *pcKeyboradArray[MAX_INPUT_DEVICE];
    const char           *pcMouseArray[MAX_INPUT_DEVICE];

    FtkSource            *thiz = (FtkSource*)FTK_ZALLOC(sizeof(FtkSource));

    if (thiz != NULL)
    {
        thiz->get_fd   = ftk_source_sylixos_get_fd;
        thiz->check    = ftk_source_sylixos_check;
        thiz->dispatch = ftk_source_sylixos_dispatch;
        thiz->destroy  = ftk_source_sylixos_destroy;
        thiz->ref      = 1;

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

        linear_init();

        guiInputDevReg(pcKeyboradArray, max_keyboard_num, pcMouseArray, max_mouse_num);
        guiInputDevKeyboardHookSet(ftk_source_sylixos_on_keyboard_event);
        guiInputDevMouseHookSet(ftk_source_sylixos_on_pointer_event);
        guiInputDevProcStart(&attr);
    }

    return thiz;
}
