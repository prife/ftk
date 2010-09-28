/*
 * File: ftk_display_iphone.h
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: display implememtation for iphone.
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
#ifndef FTK_DISPLAY_IPHONE_H
#define FTK_DISPLAY_IPHONE_H

#import "ftk_display.h"
#import "ftk_event.h"
#import "ftk_wnd_manager.h"
#import "ftk_wnd_manager_default.h"
#import "ftk_globals.h"
#import "ftk_log.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface FtkView : UIView
{
@private
    CGColorSpaceRef    color_space;
    CGContextRef       bitmap_context;
    CGImageRef         cg_image;
    CGImageRef         redraw_cg_img;
    UIImage           *image;

    GLint              backingWidth;
    GLint              backingHeight;

    EAGLContext       *context;

    GLuint             viewRenderbuffer;
    GLuint             viewFramebuffer;
    GLuint             depthRenderbuffer;

    GLuint             texture;

    FtkDisplay        *display;
}

@property FtkDisplay *display;

-(BOOL)setupView;
-(void)drawView;

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;

@end

typedef struct _PrivInfo
{
    FtkEvent event;
    UIWindow *window;
    FtkView *view;
    int width;
    int height;
    int depth;
    int pixel_size;
    unsigned char* bits;
    FtkBitmapCopyFromData copy_from_data;
    FtkBitmapCopyToData   copy_to_data;
} PrivInfo;

FtkDisplay* ftk_display_iphone_create(void);

#endif/*FTK_DISPLAY_IPHONE_H*/
