/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_UI_EGLUTILS_H
#define ANDROID_UI_EGLUTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#include <system/window.h>
#include <EGL/egl.h>

typedef int32_t     status_t;
enum {
    MINEGL_OK                = 0,    // Everything's swell.
    MINEGL_NO_ERROR          = 0,    // No errors.

    MINEGL_UNKNOWN_ERROR       = 0x80000000,

    MINEGL_NO_MEMORY           = -ENOMEM,
    MINEGL_INVALID_OPERATION   = -ENOSYS,
    MINEGL_BAD_VALUE           = -EINVAL,
    MINEGL_BAD_TYPE            = 0x80000001,
    MINEGL_NAME_NOT_FOUND      = -ENOENT,
    MINEGL_PERMISSION_DENIED   = -EPERM,
    MINEGL_NO_INIT             = -ENODEV,
    MINEGL_ALREADY_EXISTS      = -EEXIST,
    MINEGL_DEAD_OBJECT         = -EPIPE,
    MINEGL_FAILED_TRANSACTION  = 0x80000002,
    MINEGL_JPARKS_BROKE_IT     = -EPIPE,
    MINEGL_BAD_INDEX           = -EOVERFLOW,
    MINEGL_NOT_ENOUGH_DATA     = -ENODATA,
    MINEGL_WOULD_BLOCK         = -EWOULDBLOCK, 
    MINEGL_TIMED_OUT           = -ETIMEDOUT,
    MINEGL_UNKNOWN_TRANSACTION = -EBADMSG,
    MINEGL_FDS_NOT_ALLOWED     = 0x80000007,
};

//inline const char *eglutils_strerror(EGLint err);

inline status_t eglutils_selectConfigForPixelFormat(
    EGLDisplay dpy,
    EGLint const* attrs,
    int32_t format,
    EGLConfig* outConfig);

inline status_t eglutils_selectConfigForNativeWindow(
    EGLDisplay dpy,
    EGLint const* attrs,
    EGLNativeWindowType window,
    EGLConfig* outConfig);

// ----------------------------------------------------------------------------

const char *eglutils_strerror(EGLint err)
{
    switch (err){
        case EGL_SUCCESS:           return "EGL_SUCCESS";
        case EGL_NOT_INITIALIZED:   return "EGL_NOT_INITIALIZED";
        case EGL_BAD_ACCESS:        return "EGL_BAD_ACCESS";
        case EGL_BAD_ALLOC:         return "EGL_BAD_ALLOC";
        case EGL_BAD_ATTRIBUTE:     return "EGL_BAD_ATTRIBUTE";
        case EGL_BAD_CONFIG:        return "EGL_BAD_CONFIG";
        case EGL_BAD_CONTEXT:       return "EGL_BAD_CONTEXT";
        case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";
        case EGL_BAD_DISPLAY:       return "EGL_BAD_DISPLAY";
        case EGL_BAD_MATCH:         return "EGL_BAD_MATCH";
        case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";
        case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";
        case EGL_BAD_PARAMETER:     return "EGL_BAD_PARAMETER";
        case EGL_BAD_SURFACE:       return "EGL_BAD_SURFACE";
        case EGL_CONTEXT_LOST:      return "EGL_CONTEXT_LOST";
        default: return "UNKNOWN";
    }
}

status_t eglutils_selectConfigForPixelFormat(
        EGLDisplay dpy,
        EGLint const* attrs,
        int32_t format,
        EGLConfig* outConfig)
{
    EGLint numConfigs = -1, n=0;

    if (!attrs)
        return MINEGL_BAD_VALUE;

    if (outConfig == NULL)
        return MINEGL_BAD_VALUE;

    // Get all the "potential match" configs...
    if (eglGetConfigs(dpy, NULL, 0, &numConfigs) == EGL_FALSE)
        return MINEGL_BAD_VALUE;

    EGLConfig* const configs = (EGLConfig*)malloc(sizeof(EGLConfig)*numConfigs);
    if (eglChooseConfig(dpy, attrs, configs, numConfigs, &n) == EGL_FALSE) {
        free(configs);
        return MINEGL_BAD_VALUE;
    }

    int i;
    EGLConfig config = NULL;
    for (i=0 ; i<n ; i++) {
        EGLint nativeVisualId = 0;
        eglGetConfigAttrib(dpy, configs[i], EGL_NATIVE_VISUAL_ID, &nativeVisualId);
        if (nativeVisualId>0 && format == nativeVisualId) {
            config = configs[i];
            break;
        }
    }

    free(configs);

    if (i<n) {
        *outConfig = config;
        return MINEGL_NO_ERROR;
    }

    return MINEGL_NAME_NOT_FOUND;
}

status_t eglutils_selectConfigForNativeWindow(
        EGLDisplay dpy,
        EGLint const* attrs,
        EGLNativeWindowType window,
        EGLConfig* outConfig)
{
    int err;
    int format;

    if (!window)
        return MINEGL_BAD_VALUE;

    if ((err = window->query(window, NATIVE_WINDOW_FORMAT, &format)) < 0) {
        return err;
    }

    return eglutils_selectConfigForPixelFormat(dpy, attrs, format, outConfig);
}

#endif /* ANDROID_UI_EGLUTILS_H */
