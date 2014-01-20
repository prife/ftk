#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <GLES/egl.h>
#include "EGLUtils.h"

#define assert(EX)                     \
    if (!(EX)) {                       \
        volatile char dummy=0;         \
        printf("(%s) assert failed at %s:%d \n", #EX, __FUNCTION__, __LINE__); \
        exit(-1);\
    }

typedef struct
{
    GLsizei version;
    GLuint width;
    GLuint height;
    GLint stride;
    GLubyte* data;
    GLubyte format;
    GLubyte rfu[3];
    GLuint  texid;  // now we only support 16 pics.

    union {
        GLint compressedFormat;
        GLint vstride;
    };
    void * reserved;
} GLSurface;

#define TEXTURES_MAX    64

static     EGLSurface gsurface = EGL_NO_SURFACE;          // May be used when drawing or swapping
static     EGLDisplay dpy = EGL_NO_DISPLAY;              // May be used when drawing or swapping
static     EGLContext context = EGL_NO_CONTEXT;
static     EGLint majorVersion;
static     EGLint minorVersion;
static     EGLConfig config;
static     GLuint textures[TEXTURES_MAX];                           // texture to store the pic.
static     GLuint tex_cur_id = 0;
/////////////////////////////////////////////////////////////////////////////
//  start
/////////////////////////////////////////////////////////////////////////////
static GLSurface gGLSurface;

static void hw_gl_init(void)
{
    EGLint configAttribs[] = {
        EGL_BUFFER_SIZE, EGL_DONT_CARE,
        EGL_DEPTH_SIZE, 16,
        EGL_RED_SIZE,   5,
        EGL_BLUE_SIZE,  6,
        EGL_BLUE_SIZE,  5,
        EGL_NONE
    };

    EGLint numConfigs;

    EGLNativeWindowType window = android_createDisplaySurface();

    dpy = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    eglInitialize( dpy, &majorVersion, &minorVersion );

    int err = eglutils_selectConfigForNativeWindow(
        dpy, configAttribs, window, &config );

    if ( err ) {
        fprintf( stderr, "[minegl]couldn't find an EGLConfig matching the screen format \n");
        return;
    }

    gsurface = eglCreateWindowSurface( dpy, config, window, NULL );
    context = eglCreateContext( dpy, config, NULL, NULL );
    eglMakeCurrent( dpy, gsurface, gsurface, context );

    EGLint w, h;
    eglQuerySurface( dpy, gsurface, EGL_WIDTH, &w );
    eglQuerySurface( dpy, gsurface, EGL_HEIGHT, &h );

    glGenTextures( TEXTURES_MAX, &textures[0]); // gen TEXTURES_MAX textures
    int i = 0;
    for ( i = 0; i < TEXTURES_MAX; i++ ){
        glBindTexture( GL_TEXTURE_2D, textures[i] );
        glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterx( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterx( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    }

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_DITHER );
    glEnable( GL_BLEND );

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, w, 0, h, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    eglSwapInterval( dpy, 1 );
}

static int hw_gl_get_screen_width(void)
{
    EGLint w;
    eglQuerySurface(dpy, gsurface, EGL_WIDTH, &w);
    return w;
}

static int hw_gl_get_screen_height(void)
{
    EGLint h;
    eglQuerySurface(dpy, gsurface, EGL_HEIGHT, &h);
    return h;
}

static void hw_gl_blit(GLSurface * surface, int sx, int sy, int w, int h, int dx, int dy )
{
    int screen_width, screen_height;
    int pic_width, pic_height;

    assert(gGLSurface.data != NULL);
    assert(surface != NULL && surface->data != NULL);

    screen_width = gGLSurface.width;
    screen_height = gGLSurface.height;
    pic_width = surface->width;
    pic_height = surface->height;
    //printf("[%d]:%s width:%d, height:%d\n", __LINE__, __func__, pic_width, pic_height);
    fprintf( stdout, "screen_width %d screen_height %d pic_width %d pic_height %d texture id %d\n",
             screen_width, screen_height, pic_width, pic_height, surface->texid);

    const GLfloat vertices[4][2] =
    {
        { dx , screen_height - dy - h },
        { dx , screen_height - dy },
        { (dx + w) , screen_height - dy },
        { (dx +w) , screen_height - dy - h }
    };

    const GLfloat texCoords[4][2] =
    {
        { (sx / (float)pic_width) ,         ( ( sy + h ) / (float)pic_height ) },
        { (sx / (float)pic_width) ,         ( sy / (float)pic_height ) },
        { ( (w + sx) / (float)pic_width ),  ( sy / (float)pic_height ) },
        { ( (w + sx) / (float)pic_width ),  ( ( sy + h ) / (float)pic_height ) }
    };

    glBindTexture( GL_TEXTURE_2D, textures[surface->texid] );

    glEnable( GL_TEXTURE_2D );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

static int hw_gl_create_surface(GLSurface * surface, GLuint texid)
{
    void * buf;
    int w, h;
    int result;

    assert(surface->data == NULL);//should only be create once!

    w = hw_gl_get_screen_width();
    h = hw_gl_get_screen_height();

    buf = malloc(sizeof(GLSurface) + 4*w*h);
    //buf = malloc(4* w * h);
    assert(buf != NULL);
    surface->data = buf + sizeof(GLSurface);
    surface->width = w;
    surface->height = h;
    surface->texid = texid;

    glBindTexture(GL_TEXTURE_2D, textures[texid]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->data);
    return 0;
}

static void hw_gl_flip (void)
{
    eglSwapBuffers(dpy, gsurface);
}

/////////////////////////////////////////////////////////////////////////////
//  middle the next 5 functions is for ftk
//  NOTE: depth is 32bit
/////////////////////////////////////////////////////////////////////////////

void hw_lcd_init(void)
{
    hw_gl_init();
    hw_gl_create_surface(&gGLSurface, 0);
}

int hw_lcd_get_screen_width(void)
{
    assert(gGLSurface.data != NULL);
    return gGLSurface.width;
}

int hw_lcd_get_screen_height(void)
{
    assert(gGLSurface.data != NULL);
    return gGLSurface.height;
}

void hw_lcd_sync(void)
{
    assert(gGLSurface.data != NULL);

    glBindTexture(GL_TEXTURE_2D, textures[gGLSurface.texid]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gGLSurface.width, gGLSurface.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, gGLSurface.data);

    hw_gl_blit(&gGLSurface, 0, 0, gGLSurface.width, gGLSurface.height, 0, 0);
    hw_gl_flip();
}

int hw_lcd_get_cpuaddr(void **addr)
{
    assert(gGLSurface.data != NULL);
    *addr = gGLSurface.data;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
//  end
/////////////////////////////////////////////////////////////////////////////
