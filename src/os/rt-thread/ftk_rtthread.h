
#ifndef __FTK_RTTHREAD_H__
#define __FTK_RTTHREAD_H__

#include <rtthread.h>
#include <sys/time.h>
#include <sys/types.h>

#define FTK_ROOT_DIR                    "/ftk"
#define DATA_DIR                        FTK_ROOT_DIR
#define LOCAL_DATA_DIR                  FTK_ROOT_DIR
#define FTK_DATA_ROOT                   FTK_ROOT_DIR"/data"
#define TESTDATA_DIR                    FTK_ROOT_DIR"/testdata"
#define APP_DATA_DIR                    FTK_ROOT_DIR"/apps"
#define APP_LOCAL_DATA_DIR              FTK_ROOT_DIR"/apps"

#define FTK_FONT                        "unicode.fnt"

#define ftk_strncpy                     strncpy
#define ftk_snprintf                    snprintf
#define ftk_vsnprintf                   vsnprintf
#define ftk_getcwd                      getcwd
#define ftk_sscanf                      sscanf

#define FTK_PATH_DELIM                  '/'

#define USE_FTK_MAIN

#define usleep(us)			           rt_thread_sleep(us*1000000/RT_TICK_PER_SECOND)

#define HAS_BMP
#define HAS_PNG

#endif // __FTK_RTTHREAD_H__
