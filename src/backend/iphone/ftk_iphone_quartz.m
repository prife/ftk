/*
 * File: ftk_iphone_quartz.m
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: 
 *
 * Copyright (c) 2009 - 2010  ngwsx2008 <ngwsx2008@126.com>
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
 * 2010-09-28 ngwsx2008 <ngwsx2008@126.com> created.
 *
 */
#import "ftk_display_iphone.h"

@implementation FtkView

@synthesize display;

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.userInteractionEnabled = YES;

    cg_image = nil;
    redraw_cg_img = nil;
    image = nil;

    return self;
}

-(void)layoutSubviews
{
}

-(void)dealloc
{
    CGColorSpaceRelease(color_space);
    CGContextRelease(bitmap_context);

    [super dealloc];
}

-(BOOL)setupView
{
    DECL_PRIV(display, priv);

    color_space = CGColorSpaceCreateDeviceRGB();
    bitmap_context = CGBitmapContextCreate(priv->bits, priv->width, priv->height,
        8, 4 * priv->width, color_space, kCGImageAlphaPremultipliedLast);

    return YES;
}

-(void)drawView
{
    DECL_PRIV(display, priv);

    if (cg_image) {
        CGImageRelease(cg_image);
        cg_image = nil;
    }
    if (redraw_cg_img) {
        CGImageRelease(redraw_cg_img);
        redraw_cg_img = nil;
    }
    if (image) {
        //[image release];
        image = nil;
    }
    cg_image = CGBitmapContextCreateImage(bitmap_context);
}

-(void)drawRect:(CGRect)rect
{
    DECL_PRIV(display, priv);

    //CGContextRef ctx = UIGraphicsGetCurrentContext();
    if (!cg_image) {
        return;
    }
    redraw_cg_img = CGImageCreateWithImageInRect(cg_image, rect);
    if (!redraw_cg_img) {
        return;
    }
    image = [UIImage imageWithCGImage:redraw_cg_img];
    if (!image) {
        return;
    }
    [image drawInRect:rect];
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    DECL_PRIV(display, priv);
	NSEnumerator *enumerator = [touches objectEnumerator];
	UITouch *touch = (UITouch*)[enumerator nextObject];
	
	if (touch) {
        CGPoint locationInView = [touch locationInView: self];
        priv->event.type = FTK_EVT_MOUSE_DOWN;
	priv->event.u.mouse.x = locationInView.x;
	priv->event.u.mouse.y = locationInView.y;
	ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &priv->event);
	}
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    DECL_PRIV(display, priv);
	NSEnumerator *enumerator = [touches objectEnumerator];
	UITouch *touch = (UITouch*)[enumerator nextObject];
	
	if (touch) {
        CGPoint locationInView = [touch locationInView: self];
        priv->event.type = FTK_EVT_MOUSE_UP;
	priv->event.u.mouse.x = locationInView.x;
	priv->event.u.mouse.y = locationInView.y;
	ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &priv->event);
	}
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	/*
		this can happen if the user puts more than 5 touches on the screen
		at once, or perhaps in other circumstances.  Usually (it seems)
		all active touches are canceled.
	*/
	[self touchesEnded: touches withEvent: event];
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    DECL_PRIV(display, priv);
	NSEnumerator *enumerator = [touches objectEnumerator];
	UITouch *touch = (UITouch*)[enumerator nextObject];
	
	if (touch) {
		CGPoint locationInView = [touch locationInView: self];
                priv->event.u.mouse.x = locationInView.x;
		priv->event.u.mouse.y = locationInView.y;
		priv->event.type = FTK_EVT_MOUSE_MOVE;
		ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &priv->event);
	}
}

@end
