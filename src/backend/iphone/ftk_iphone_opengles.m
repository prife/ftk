/*
 * File: ftk_iphone_opengles.m
 * Author:  ngwsx2008 <ngwsx2008@126.com>
 * Brief: opengles 
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

void appInit()
{
 glEnable(GL_BLEND);
 glEnable(GL_TEXTURE_2D);

 glClearColorx(0,0,0,0);
 glShadeModel(GL_SMOOTH);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

typedef struct {
	GLfloat	x;
	GLfloat y;
	GLfloat z;
} Vertex3D;

typedef Vertex3D Vector3D;

void testTexture(FtkDisplay *display)
{
 DECL_PRIV(display, priv);

 Vertex3D rect[] = {
  {-1.0,  1.0, -0.0},
  { 1.0,  1.0, -0.0},
  {-1.0, -1.0, -0.0},
  { 1.0, -1.0, -0.0}
 };
 Vector3D rnormal[] = {
  {0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0},
  {0.0, 0.0, 1.0}
 };
 GLfloat rectuv[] = {
  0.25, 0.75,
  0.75, 0.75,
  0.25, 0.25,
  0.75, 0.25
 };

 GLushort *buffer = (GLushort *) priv->bits;
#if 0
 unsigned short color[4]={ 0xF800, 0x7E0, 0x1F, 0xffff };
 int i;
 int j;
 for (  i = 0; i < priv->width; ++i )
 {
  for (  j = 0; j < priv->height/8; ++j )
  {
   buffer[i*priv->width+j] = color[0];
  }
 }
 for (  i = 0; i < priv->width; ++i )
 {
  for (  j = priv->height/8; j < priv->height; ++j )
  {
   buffer[i*priv->width+j] = color[2];
  }
 }
#endif
 glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

 glEnableClientState( GL_VERTEX_ARRAY );
 glEnableClientState( GL_NORMAL_ARRAY );
 glEnableClientState( GL_TEXTURE_COORD_ARRAY );

 glVertexPointer( 3, GL_FIXED, 0, rect );
 glNormalPointer( GL_FIXED, 0, rnormal);
 glTexCoordPointer( 2, GL_FIXED, 0, rectuv );
 glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

 glDisableClientState( GL_VERTEX_ARRAY );
 glDisableClientState( GL_NORMAL_ARRAY );
 glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void appRender(FtkDisplay *display)
{
 DECL_PRIV(display, priv);

 glViewport(0, 0, priv->width, priv->height);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();

 testTexture(display);
}

@interface FtkView (private)
#if 0
-(BOOL)initGLES;
#endif
-(BOOL)createFramebuffer;
-(void)destroyFramebuffer;
-(void)drawView;
@end

@implementation FtkView

@synthesize display;

+(Class)layerClass
{
    return [CAEAGLLayer class];
}

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    return self;
}

-(void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
}

-(void)dealloc
{
    [self destroyFramebuffer];

    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }

    [context release];
    [super dealloc];
}

-(BOOL)initGLES
{
    CAEAGLLayer *layer = (CAEAGLLayer *) self.layer;

    layer.opaque = YES;
    layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking,
        kEAGLColorFormatRGBA8/*kEAGLColorFormatRGB565*/, kEAGLDrawablePropertyColorFormat, nil];

    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

    if (!context
        || ![EAGLContext setCurrentContext:context]
        || ![self createFramebuffer])
    {
        return YES;
    }

    return NO;
}

-(BOOL)createFramebuffer
{
    // Generate IDs for a framebuffer object and a color renderbuffer
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);

	// This call associates the storage for the current render buffer with the EAGLDrawable (our CAEAGLLayer)
	// allowing us to draw into a buffer that will later be rendered to screen whereever the layer is (which corresponds with our view).
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	// For this sample, we also need a depth buffer, so we'll create and attach one via another renderbuffer.
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES/*GL_DEPTH_COMPONENT16_OES*/, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);

	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}

    appInit(display);

    return YES;
}

-(void)destroyFramebuffer
{
#if 0
    glDisableClientState(GL_VERTEX_ARRAY);
#endif

    glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
	
	if(depthRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
}

-(void)drawView
{
    DECL_PRIV(display, priv);

    [EAGLContext setCurrentContext:context];

    appRender(display);

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];

    GLenum err = glGetError();
    if(err)
        ftk_logd("%x error", err);
}

@end
