/*
 * ftk_source_touch.c
 *
 *  Created on: 2010-9-13
 *      Author: jiaojinxing
 */

#include "ftk_log.h"
#include "ftk_source_touch.h"
#include "ts.h"

typedef struct _PrivInfo
{
    int        pressed;
    int        fd;
    int        x;
    int        y;
    FtkEvent   event;
    FtkOnEvent on_event;
    void*      user_data;
	struct rt_touch_event touchevent;
	rt_device_t	device;
} PrivInfo;

static int ftk_source_touch_get_fd(FtkSource* thiz)
{
    DECL_PRIV(thiz, priv);

    return priv->fd;
}

static int ftk_source_touch_check(FtkSource* thiz)
{
    return -1;
}

static Ret ftk_source_touch_dispatch(FtkSource* thiz)
{
    DECL_PRIV(thiz, priv);

    priv->event.type = FTK_EVT_NOP;
    priv->event.u.mouse.x = priv->touchevent.x;
    priv->event.u.mouse.y = priv->touchevent.y;

    ftk_logd("%s: sample.pressure=%d x=%d y=%d\n", __func__,
            priv->touchevent.pressed, priv->touchevent.x, priv->touchevent.y);

    if (priv->touchevent.pressed)
    {
        if (priv->pressed)
        {
            priv->event.type =  FTK_EVT_MOUSE_MOVE;
        }
        else
        {
            priv->event.type =  FTK_EVT_MOUSE_DOWN;
            priv->pressed = 1;
        }
    }
    else
    {
        if (priv->pressed)
        {
            priv->event.type =  FTK_EVT_MOUSE_UP;
        }
        priv->pressed = 0;
    }

    if (priv->on_event != NULL && priv->event.type != FTK_EVT_NOP)
    {
        priv->on_event(priv->user_data, &priv->event);

        ftk_logd("%s: type=%d x=%d y=%d\n", __func__,
                priv->event.type, priv->event.u.mouse.x, priv->event.u.mouse.y);

        priv->event.type = FTK_EVT_NOP;
    }

    return RET_OK;
}

static Ret ftk_source_touch_eventpost(FtkSource* thiz, struct rt_touch_event *event)
{
    DECL_PRIV(thiz, priv);

	priv->touchevent = *event;

	ftk_logd("ftk_source_touch_eventpost(), %d\n", priv->fd);

	ftk_rtthread_set_file_readble(priv->fd);

    return RET_OK;
}

static void ftk_source_touch_destroy(FtkSource* thiz)
{
    if (thiz != NULL)
    {
        DECL_PRIV(thiz, priv);

        ftk_rtthread_select_fd_free(priv->fd);

		rt_device_close(priv->device);

        FTK_ZFREE(thiz, sizeof(thiz) + sizeof(PrivInfo));
    }
}

FtkSource* ftk_source_touch_create(const char* filename, FtkOnEvent on_event, void* user_data)
{
    FtkSource* thiz = (FtkSource*)FTK_ZALLOC(sizeof(FtkSource) + sizeof(PrivInfo));
	rt_device_t device = NULL;

    if (thiz != NULL)
    {
        DECL_PRIV(thiz, priv);

        thiz->get_fd   = ftk_source_touch_get_fd;
        thiz->check    = ftk_source_touch_check;
        thiz->dispatch = ftk_source_touch_dispatch;
        thiz->destroy  = ftk_source_touch_destroy;

        thiz->ref = 1;

		priv->device = rt_device_find(filename); 
        if (priv->device == NULL)
        {
            FTK_ZFREE(thiz, sizeof(thiz) + sizeof(PrivInfo));
            return NULL;
        }
		rt_device_open(priv->device, 0);

		rt_device_control(priv->device, RT_TOUCH_EVENTPOST, ftk_source_touch_eventpost); 

		rt_device_control(priv->device, RT_TOUCH_EVENTPOST_PARAM, thiz); 

		priv->fd = ftk_rtthread_select_fd_alloc();

        priv->on_event = on_event;
        priv->user_data = user_data;
        ftk_logd("%s: %d=%s priv->user_data=%p\n", __func__, priv->fd, filename, priv->user_data);
    }
    return thiz;
}
