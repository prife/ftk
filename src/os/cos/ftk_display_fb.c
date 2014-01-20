/*
 * File: ftk_display_fb.c
 * Author:	Li XianJing <xianjimli@hotmail.com>
 * Brief:	framebuffer implemented display.
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
 * 2014-01-20 prifezhu <prifezhu@china-liantong.com> created
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

typedef struct hw_lcd_screen_info {
   int           is_on_screen;     /*  in  */
   void         *cpu_virt_addr;    /* out */
   unsigned int  alignment;        /* out */
   unsigned int  width;
   unsigned int  height;
   unsigned int  BPP;
} hw_lcd_screen_info_t;

#ifndef FTK_GDI_LDD
extern int hw_lcd_get_screen_info(hw_lcd_screen_info_t *screen_info);
extern int hw_lcd_init();
extern int hw_lcd_deinit();
extern int hw_lcd_sync();
extern void hw_lcd_get_cpuaddr(void **);
#else
int (*hw_lcd_init)();
int (*hw_lcd_sync)();
int (*hw_lcd_deinit)();
int (*hw_lcd_get_screen_info)(hw_lcd_screen_info_t *screen_info);
void (*hw_lcd_get_cpuaddr)(void **);
#endif

static hw_lcd_screen_info_t info;

#ifdef FTK_GDI_LDD
#include <dlfcn.h>
#define GDI_LIBNAME "/minidvb/lib/libminilcd.so"
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

    hw_lcd_init = dlsym(handle, "hw_lcd_init");
    if ((error = dlerror()) != NULL)
        goto __err;

    hw_lcd_get_cpuaddr = dlsym(handle, "hw_lcd_get_cpuaddr");
    if ((error = dlerror()) != NULL)
        goto __err;

    hw_lcd_sync = dlsym(handle, "hw_lcd_sync");
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

    hw_lcd_init();
    return 0;
}

static void lcd_sync(void* ctx, FtkRect* rect)
{
	hw_lcd_sync();
    return;
}

static void lcd_deinit(void)
{
    // hw_lcd_deinit()
}

FtkDisplay* ftk_display_fb_create(const char * name)
{
    void * buf = NULL;
	FtkDisplay* thiz = NULL;
    FtkPixelFormat format = 0;

    if (lcd_init() != 0)
        return NULL;

    hw_lcd_get_cpuaddr(&buf);
    if (buf == NULL)
    {
        printf("get NULL framebuf address\n");
        exit(-1);
    }

    switch (32)//FIXME
    {
    case 16: format = FTK_PIXEL_RGB565; break;
    case 24: format = FTK_PIXEL_BGR24; break;
    case 32: format = FTK_PIXEL_BGRA32; break;
    default:
        assert(!"not supported framebuffer format.");
        break;
    }

    thiz = ftk_display_mem_create(format, 1280, 720,
        buf, lcd_deinit, NULL);
    ftk_display_mem_set_sync_func(thiz, lcd_sync, NULL);

	return thiz;
}

