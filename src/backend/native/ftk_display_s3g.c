/*
 * File: ftk_display_s3g.c
 * Author:  prife <prifezhu@china-liantong.com>
 * Brief:   implemented display.
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
 * 2013-12-27 prifezhu <prifezhu@china-liantong.com> created
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ftk_log.h"
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "ftk_display_mem.h"
#include "ftk_display_fb.h"

#define FTK_GDI_LDD

typedef struct s3glib_screen_info {
   int           is_on_screen;     /*  in  */
   void         *cpu_virt_addr;    /* out */
   unsigned int  alignment;        /* out */
   unsigned int  width;
   unsigned int  height;
   unsigned int  BPP;
} s3glib_screen_info_t;

#ifndef FTK_GDI_LDD
extern int s3glib_get_screen_info(s3glib_screen_info_t *screen_info);
extern int s3glib_init();
extern int s3glib_deinit();
extern int s3glib_update_screen();
#else
int (*s3glib_init)();
int (*s3glib_deinit)();
int (*s3glib_get_screen_info)(s3glib_screen_info_t *screen_info);
int (*s3glib_update_screen)();
#endif

static s3glib_screen_info_t info;

#ifdef FTK_GDI_LDD
#include <dlfcn.h>
#define GDI_LIBNAME "/usr/lib/libs3g_al.so"
int lib_gdi_init()
{
    void *handle;
    char *error;
    //handle = dlopen(CWD LIBNAME, RTLD_LAZY);
    handle = dlopen(GDI_LIBNAME, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror();    /* Clear any existing error */

    s3glib_get_screen_info = dlsym(handle, "s3glib_get_screen_info");
    if ((error = dlerror()) != NULL)
        goto __err;

    s3glib_update_screen = dlsym(handle, "s3glib_update_screen");
    if ((error = dlerror()) != NULL)
        goto __err;


    s3glib_init = dlsym(handle, "s3glib_init");
    if ((error = dlerror()) != NULL)
        goto __err;

    s3glib_deinit = dlsym(handle, "s3glib_deinit");
    if ((error = dlerror()) != NULL)
        goto __err;

    return 0;
__err:
    fprintf(stderr, "%s:%d: %s\n", __func__, __LINE__, error);
    exit(EXIT_FAILURE);
    return -1;
}
#endif

static int lcd_init()
{

#ifdef FTK_GDI_LDD
    lib_gdi_init();
#endif

    if (s3glib_init() != 0)
    {
        printf("s3glib_init failed!\n");
        return -1;
    }

    info.is_on_screen = 1; //FIXME
    s3glib_get_screen_info(&info);
	if (info.cpu_virt_addr == NULL)
        return -1;

    printf("info.width = %d\n", info.width);
    printf("info.height = %d\n", info.height);
    printf("info.alignment = %d\n", info.alignment);
    printf("info.bits_per_pixel = %d\n", info.BPP);

    return 0;
}

static void lcd_sync(void* ctx, FtkRect* rect)
{
//	return s3glib_update_screen();
}

FtkDisplay* ftk_display_fb_create(const char * name)
{
	FtkDisplay* thiz = NULL;
    FtkPixelFormat format = 0;

    if (lcd_init() != 0)
        return NULL;

    switch (info.BPP)
    {
    case 16: format = FTK_PIXEL_RGB565; break;
    case 24: format = FTK_PIXEL_BGR24; break;
    case 32: format = FTK_PIXEL_BGRA32; break;
    default:
        assert(!"not supported framebuffer format.");
        break;
    }

    thiz = ftk_display_mem_create(format, info.width, info.height,
        info.cpu_virt_addr, s3glib_deinit, NULL);
    ftk_display_mem_set_sync_func(thiz, lcd_sync, NULL);

	return thiz;
}

