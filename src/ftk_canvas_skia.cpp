/*
 * File: ftk_canvas_skia.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   skia canvas implemntation.
 *
 * Copyright (c) 2009 - 2011  Li XianJing <xianjimli@hotmail.com>
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
 * 2011-04-18 Li XianJing <xianjimli@hotmail.com> created
 */

#include "ftk_log.h"
#include "ftk_util.h"
#include "ftk_bitmap.h"
#include "ftk_canvas.h"
#include "SkCanvas.h"
#include "effects/SkPorterDuff.h"

using namespace android;

#define COLOR_TO_SKCOLOR(c) SkColorSetARGB(c.a, c.r, c.g, c.b)

typedef struct _PrivInfo
{
	size_t w;
	size_t h;
	FtkColor* bits;
	SkPaint* paint;
	SkCanvas* canvas;
	FtkBitmap* bitmap;
}PrivInfo;

static Ret ftk_canvas_skia_sync_gc(FtkCanvas* thiz)
{
	int size = 0;
	DECL_PRIV(thiz, priv);
	FtkColor c = thiz->gc.fg;
	
	priv->paint->setARGB(c.a, c.b, c.g, c.r);
//	ftk_logd("%s: c=%02x %02x %02x %02x\n", __func__, c.a, c.r, c.g, c.b);
	if(thiz->gc.font != NULL)
	{
		char desc[64];
		size = ftk_font_desc_get_size(thiz->gc.font->font_desc);
		priv->paint->setTextSize(SkIntToScalar(size));
		ftk_font_desc_get_string(thiz->gc.font->font_desc, desc, sizeof(desc));
//		ftk_logd("%s: font_size=%d %s\n", __func__, size, desc);
	}

	return RET_OK;
}

static Ret ftk_canvas_skia_set_clip(FtkCanvas* thiz, FtkRegion* clip)
{
	SkRegion clipReg;
	DECL_PRIV(thiz, priv);

	if(clip != NULL)
	{
		SkIRect rect;
		FtkRegion* iter = clip;
		while(iter != NULL)
		{
			rect.set(iter->rect.x, iter->rect.y, iter->rect.x + iter->rect.width, iter->rect.y + iter->rect.height);
			clipReg.op(rect, SkRegion::kUnion_Op);
			ftk_logd("set_clip: %d %d %d %d\n", iter->rect.x, iter->rect.y, iter->rect.width, iter->rect.height);
			iter = iter->next;
		}
	}
	else
	{
		clipReg.setRect(0, 0, priv->w, priv->h);
		ftk_logd("set_clip: %d %d %d %d\n", 0, 0, priv->w, priv->h);
	}

	priv->canvas->clipRegion(clipReg, SkRegion::kReplace_Op);

	return RET_OK;
}

static Ret ftk_canvas_skia_draw_pixels(FtkCanvas* thiz, FtkPoint* points, size_t nr)
{
	DECL_PRIV(thiz, priv);
	/*TODO:*/

	return RET_OK;
}

static Ret ftk_canvas_skia_draw_line(FtkCanvas* thiz, size_t x1, size_t y1, size_t x2, size_t y2)
{
	DECL_PRIV(thiz, priv);

//	ftk_logd("%s: %p\n", __func__, priv->paint->getColor());
	priv->canvas->drawLine(SkIntToScalar(x1), SkIntToScalar(y1), SkIntToScalar(x2), SkIntToScalar(y2), *priv->paint);

	return RET_OK;
}

static Ret ftk_canvas_skia_clear_rect(FtkCanvas* thiz, size_t x, size_t y, size_t w, size_t h)
{
	SkRect rect;
	DECL_PRIV(thiz, priv);
	
	rect.iset(x, y, x + w, y + h);
	priv->paint->setStyle(SkPaint::kStrokeAndFill_Style);
	priv->canvas->drawRect(rect, *priv->paint);
	ftk_logd("%s: %p\n", __func__, priv->paint->getColor());

	return RET_OK;
}

static Ret ftk_canvas_skia_draw_rect(FtkCanvas* thiz, size_t x, size_t y, size_t w, size_t h, 
	int round, int fill)
{
	SkRect rect;
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);

	rect.iset(x, y, x + w, y + h);

	if(fill)
	{
		priv->paint->setStyle(SkPaint::kStrokeAndFill_Style);
	}
	else
	{
		priv->paint->setStyle(SkPaint::kStroke_Style);
	}

	if(round)
	{
		priv->canvas->drawRoundRect(rect, SkIntToScalar(5), SkIntToScalar(5), *priv->paint);
	}
	else
	{
		priv->canvas->drawRect(rect, *priv->paint);
	}
	priv->paint->setStyle(SkPaint::kStroke_Style);

	ftk_logd("ftk_canvas_skia_draw_rect: %d %d %d %d round=%d fill=%d c=%p\n", x, y, w, h, round, fill,
		priv->paint->getColor());

	return ret;
}

static Ret ftk_canvas_skia_draw_bitmap(FtkCanvas* thiz, FtkBitmap* bitmap, 
	FtkRect* src_r, FtkRect* dst_r, int alpha)
{
	SkRect dst;
	SkIRect src;
	SkBitmap bmp;
	Ret ret = RET_FAIL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && bitmap != NULL && dst_r != NULL && src_r != NULL, RET_FAIL);

	src.set(src_r->x, src_r->y, src_r->x + src_r->width, src_r->y + src_r->height);
	dst.iset(dst_r->x, dst_r->y, dst_r->x + dst_r->width, dst_r->y + dst_r->height);
	
	bmp.setConfig(SkBitmap::kARGB_8888_Config, ftk_bitmap_width(bitmap), ftk_bitmap_height(bitmap), ftk_bitmap_width(bitmap) * 4);
	bmp.setPixels(ftk_bitmap_bits(bitmap));
//	bmp.setIsOpaque(false);

	priv->canvas->drawBitmapRect(bmp, &src, dst, priv->paint);
//
//	ftk_logd("%s: w=%d h=%d %d %d %d %d\n", __func__, priv->w, priv->h, dst_r->x, dst_r->y, dst_r->width, dst_r->height);

	return ret;
}

static Ret ftk_canvas_skia_draw_string(FtkCanvas* thiz, size_t x, size_t y, 
	const char* str, int len, int vcenter)
{
	DECL_PRIV(thiz, priv);

	if(vcenter)
	{
		y += ftk_font_desc_get_size(thiz->gc.font->font_desc)/3;
	}

	priv->canvas->drawText(str, len, SkIntToScalar(x), SkIntToScalar(y), *priv->paint);

	ftk_logd("%s: %s len=%d text_size=%d %p\n", __func__, str, len, 
		SkFixedFloor(SkScalarToFixed(priv->paint->getTextSize())), priv->paint->getColor());

	return RET_OK;
}

static Ret fk_canvas_skia_lock_buffer(FtkCanvas* thiz, FtkBitmap** bitmap)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(bitmap != NULL, RET_FAIL);

	*bitmap = priv->bitmap;

	return RET_OK;
}

static Ret ftk_canvas_skia_unlock_buffer(FtkCanvas* thiz)
{
	return RET_OK;
}

static void ftk_canvas_skia_destroy(FtkCanvas* thiz)
{
	DECL_PRIV(thiz, priv);
	priv->canvas->setBitmapDevice(SkBitmap());
	ftk_bitmap_unref(priv->bitmap);
	delete priv->canvas;
	delete priv->paint;
	FTK_FREE(thiz);

	return;
}

FtkCanvas* ftk_canvas_create(size_t w, size_t h, FtkColor* clear_color)
{
	FtkCanvas* thiz = NULL;
	return_val_if_fail(w > 0 && h > 0 && clear_color != NULL, NULL);

	thiz = (FtkCanvas*)FTK_ZALLOC(sizeof(FtkCanvas) + sizeof(PrivInfo));

	ftk_logd("ftk_canvas_create: %d %d\n", w, h);
	if(thiz != NULL)
	{
		SkBitmap bitmap;
		DECL_PRIV(thiz, priv);

		thiz->sync_gc = ftk_canvas_skia_sync_gc;
		thiz->set_clip = ftk_canvas_skia_set_clip;
		thiz->draw_pixels = ftk_canvas_skia_draw_pixels;
		thiz->draw_line = ftk_canvas_skia_draw_line;
		thiz->draw_rect = ftk_canvas_skia_draw_rect;
		thiz->clear_rect = ftk_canvas_skia_clear_rect;
		thiz->draw_bitmap = ftk_canvas_skia_draw_bitmap;
		thiz->draw_string = ftk_canvas_skia_draw_string;
		thiz->lock_buffer = fk_canvas_skia_lock_buffer;
		thiz->unlock_buffer = ftk_canvas_skia_unlock_buffer;
		thiz->destroy = ftk_canvas_skia_destroy;

		thiz->gc.bg = *clear_color;
		thiz->gc.fg.a = 0xff;
		thiz->gc.fg.r = 0xff - clear_color->r;
		thiz->gc.fg.g = 0xff - clear_color->g;
		thiz->gc.fg.b = 0xff - clear_color->b;
		thiz->gc.mask = FTK_GC_FG | FTK_GC_BG;

		priv->bitmap = ftk_bitmap_create(w, h, *clear_color);
		priv->bits = ftk_bitmap_bits(priv->bitmap);
		priv->w = ftk_bitmap_width(priv->bitmap);
		priv->h = ftk_bitmap_height(priv->bitmap);
	
		bitmap.setConfig(SkBitmap::kARGB_8888_Config, priv->w, priv->h, priv->w * 4);
		bitmap.setPixels(priv->bits);
//		bitmap.setIsOpaque(true);

		priv->paint = new SkPaint();
		priv->canvas = new SkCanvas();
//		priv->paint->setXfermodeMode(SkXfermode::kDstOut_Mode);
//		priv->paint->setXfermode(SkPorterDuff::CreateXfermode(SkPorterDuff::kSrc_Mode));
		priv->canvas->setBitmapDevice(bitmap);
	}

	return thiz;
}

