/*
 * File: ftk_display_fb.c    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   framebuffer implemented display.
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
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

struct FbInfo 
{
	int fd;
	void* bits;
	struct fb_fix_screeninfo fi;
	struct fb_var_screeninfo vi;
};

#define fb_width(fb) ((fb)->vi.xres)
#define fb_height(fb) ((fb)->vi.yres)
#define fb_size(fb) ((fb)->vi.xres * (fb)->vi.yres * fb->vi.bits_per_pixel/8)

static int fb_open(struct FbInfo *fb, const char* fbfilename)
{
	fb->fd = open(fbfilename, O_RDWR);
	if (fb->fd < 0)
	{
		return -1;
	}
	
	if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fi) < 0)
		goto fail;
	if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vi) < 0)
		goto fail;

	ftk_logd("FbInfo: %s\n", fbfilename);
	ftk_logd("FbInfo: xres=%d yres=%d bits_per_pixel=%d\n", 
		fb->vi.xres, fb->vi.yres, fb->vi.bits_per_pixel);
	ftk_logd("FbInfo: red(%d %d) green(%d %d) blue(%d %d)\n", 
		fb->vi.red.offset, fb->vi.red.length,
		fb->vi.green.offset, fb->vi.green.length,
		fb->vi.blue.offset, fb->vi.blue.length);

#ifdef FTK_FB_NOMMAP
	//uclinux doesn't support MAP_SHARED or MAP_PRIVATE with PROT_WRITE, so no mmap at all is simpler
	fb->bits = fb->fi.smem_start;
#else
	fb->bits = mmap(0, fb_size(fb), PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
#endif

	if (fb->bits == MAP_FAILED)
	{
		ftk_logd("map framebuffer failed.\n");
		goto fail;
	}

	memset(fb->bits, 0xff, fb_size(fb));

	return 0;
fail:
	ftk_logd("%s is not a framebuffer.\n", fbfilename);
	close(fb->fd);

	return -1;
}

static void fb_close(struct FbInfo *fb)
{
	if(fb != NULL)
	{
		munmap(fb->bits, fb_size(fb));
		close(fb->fd);
		FTK_FREE(fb);
	}

	return;
}

FtkDisplay* ftk_display_fb_create(const char* filename)
{
	FtkDisplay* thiz = NULL;
	struct FbInfo* fb = NULL;
	return_val_if_fail(filename != NULL, NULL);

	fb = FTK_ZALLOC(sizeof(struct FbInfo));
	return_val_if_fail(fb != NULL, NULL);

	if(fb_open(fb, filename) == 0)
	{
		FtkPixelFormat format = 0;
		int bits_per_pixel = fb->vi.bits_per_pixel;
		
		if(bits_per_pixel == 16)
		{
			format = FTK_PIXEL_RGB565;
		}
		else if(bits_per_pixel == 24)
		{
			format = FTK_PIXEL_BGR24;
		}
		else if(bits_per_pixel == 32)
		{
			format = FTK_PIXEL_BGRA32;
		}
		else
		{
			assert(!"not supported framebuffer format.");
		}
	
		thiz = ftk_display_men_create(format, fb->vi.xres, fb->vi.yres, 
			fb->bits, fb_close, fb);
	}
		
	return thiz;
}

