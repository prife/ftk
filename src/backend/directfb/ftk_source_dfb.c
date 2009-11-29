/*
 * File: ftk_source_dfb.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   source to handle directfb event.
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
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
 * 2009-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_key.h"
#include "ftk_log.h"
#include "ftk_display.h"
#include "ftk_event.h"
#include "ftk_globals.h"
#include "ftk_source_dfb.h"
#include "ftk_wnd_manager.h"

typedef struct _PrivInfo
{
	int fd;
	int x;
	int y;
	FtkEvent event;
	IDirectFB* dfb;
	IDirectFBEventBuffer   *event_buffer;
}PrivInfo;

static int  ftk_source_dfb_get_fd(FtkSource* thiz)
{
	DECL_PRIV(thiz, priv);
	
	return priv->fd;
}

static int  ftk_source_dfb_check(FtkSource* thiz)
{
	return -1;
}
#if 0
static const int s_key_map[] = 
{
	[0x0a] = FTK_KEY_1,
	[0x0b] = FTK_KEY_2,
	[0x0c] = FTK_KEY_3,
	[0x0d] = FTK_KEY_4,
	[0x0e] = FTK_KEY_5,
	[0x0f] = FTK_KEY_6,
	[0x10] = FTK_KEY_7,
	[0x11] = FTK_KEY_8,
	[0x12] = FTK_KEY_9,
	[0x13] = FTK_KEY_0,
	[0x14] = FTK_KEY_MINUS,
	[0x15] = FTK_KEY_EQUAL,
	[0x16] = FTK_KEY_BACKSPACE,
	[0x17] = FTK_KEY_TAB,
	[0x18] = FTK_KEY_q,
	[0x19] = FTK_KEY_w,
	[0x1a] = FTK_KEY_e,
	[0x1b] = FTK_KEY_r,
	[0x1c] = FTK_KEY_t,
	[0x1d] = FTK_KEY_y,
	[0x1e] = FTK_KEY_u,
	[0x1f] = FTK_KEY_i,
	[0x20] = FTK_KEY_o,
	[0x21] = FTK_KEY_p,
	[0x22] = FTK_KEY_BRACKETLEFT,
	[0x23] = FTK_KEY_BRACKETRIGHT,
	[0x24] = FTK_KEY_ENTER,
	[0x26] = FTK_KEY_a,
	[0x27] = FTK_KEY_s,
	[0x28] = FTK_KEY_d,
	[0x29] = FTK_KEY_f,
	[0x2a] = FTK_KEY_g,
	[0x2b] = FTK_KEY_h,
	[0x2c] = FTK_KEY_j,
	[0x2d] = FTK_KEY_k,
	[0x2e] = FTK_KEY_l,
	[0x2f] = FTK_KEY_SEMICOLON,
	[0x30] = FTK_KEY_QUOTERIGHT,
	[0x31] = FTK_KEY_GRAVE, 
	[0x32] = FTK_KEY_LEFTSHIFT,
	[0x33] = FTK_KEY_BACKSLASH,
	[0x34] = FTK_KEY_z,
	[0x35] = FTK_KEY_x,
	[0x36] = FTK_KEY_c,
	[0x37] = FTK_KEY_v,
	[0x38] = FTK_KEY_b,
	[0x39] = FTK_KEY_n,
	[0x3a] = FTK_KEY_m,
	[0x3b] = FTK_KEY_COMMA,
	[0x3c] = FTK_KEY_DOT,
	[0x3d] = FTK_KEY_SLASH,
	[0x3e] = FTK_KEY_RIGHTSHIFT,
	[0x40] = FTK_KEY_LEFTALT,
	[0x41] = FTK_KEY_SPACE,
	[0x42] = FTK_KEY_CAPSLOCK,
	[0x43] = FTK_KEY_F1,
	[0x44] = FTK_KEY_F2,
	[0x45] = FTK_KEY_F3,
	[0x46] = FTK_KEY_F4,
	[0x47] = FTK_KEY_F5,
	[0x48] = FTK_KEY_F6,
	[0x49] = FTK_KEY_F7,
	[0x4a] = FTK_KEY_F8,
	[0x61] = FTK_KEY_HOME,
	[0x62] = FTK_KEY_UP,
	[0x63] = FTK_KEY_PAGEUP,
	[0x64] = FTK_KEY_LEFT,
	[0x66] = FTK_KEY_RIGHT,
	[0x67] = FTK_KEY_END,
	[0x68] = FTK_KEY_DOWN,
	[0x69] = FTK_KEY_PAGEDOWN,
	[0x6a] = FTK_KEY_INSERT,
	[0x6b] = FTK_KEY_DELETE,
	[0x71] = FTK_KEY_RIGHTALT,
};
#else
static const int s_key_map[] = 
{
	[DIKI_A-DIKI_UNKNOWN]              =  FTK_KEY_a,
	[DIKI_B-DIKI_UNKNOWN]              =  FTK_KEY_b,
	[DIKI_C-DIKI_UNKNOWN]              =  FTK_KEY_c,
	[DIKI_D-DIKI_UNKNOWN]              =  FTK_KEY_d,
	[DIKI_E-DIKI_UNKNOWN]              =  FTK_KEY_e,
	[DIKI_F-DIKI_UNKNOWN]              =  FTK_KEY_f,
	[DIKI_G-DIKI_UNKNOWN]              =  FTK_KEY_g,
	[DIKI_H-DIKI_UNKNOWN]              =  FTK_KEY_h,
	[DIKI_I-DIKI_UNKNOWN]              =  FTK_KEY_i,
	[DIKI_J-DIKI_UNKNOWN]              =  FTK_KEY_j,
	[DIKI_K-DIKI_UNKNOWN]              =  FTK_KEY_k,
	[DIKI_L-DIKI_UNKNOWN]              =  FTK_KEY_l,
	[DIKI_M-DIKI_UNKNOWN]              =  FTK_KEY_m,
	[DIKI_N-DIKI_UNKNOWN]              =  FTK_KEY_n,
	[DIKI_O-DIKI_UNKNOWN]              =  FTK_KEY_o,
	[DIKI_P-DIKI_UNKNOWN]              =  FTK_KEY_p,
	[DIKI_Q-DIKI_UNKNOWN]              =  FTK_KEY_q,
	[DIKI_R-DIKI_UNKNOWN]              =  FTK_KEY_r,
	[DIKI_S-DIKI_UNKNOWN]              =  FTK_KEY_s,
	[DIKI_T-DIKI_UNKNOWN]              =  FTK_KEY_t,
	[DIKI_U-DIKI_UNKNOWN]              =  FTK_KEY_u,
	[DIKI_V-DIKI_UNKNOWN]              =  FTK_KEY_v,
	[DIKI_W-DIKI_UNKNOWN]              =  FTK_KEY_w,
	[DIKI_X-DIKI_UNKNOWN]              =  FTK_KEY_x,
	[DIKI_Y-DIKI_UNKNOWN]              =  FTK_KEY_y,
	[DIKI_Z-DIKI_UNKNOWN]              =  FTK_KEY_z,
	[DIKI_0-DIKI_UNKNOWN]              =  FTK_KEY_o,
	[DIKI_1-DIKI_UNKNOWN]              =  FTK_KEY_1,
	[DIKI_2-DIKI_UNKNOWN]              =  FTK_KEY_2,
	[DIKI_3-DIKI_UNKNOWN]              =  FTK_KEY_3,
	[DIKI_4-DIKI_UNKNOWN]              =  FTK_KEY_4,
	[DIKI_5-DIKI_UNKNOWN]              =  FTK_KEY_5,
	[DIKI_6-DIKI_UNKNOWN]              =  FTK_KEY_6,
	[DIKI_7-DIKI_UNKNOWN]              =  FTK_KEY_7,
	[DIKI_8-DIKI_UNKNOWN]              =  FTK_KEY_8,
	[DIKI_9-DIKI_UNKNOWN]              =  FTK_KEY_9,
	[DIKI_F1-DIKI_UNKNOWN]             =  FTK_KEY_F1,
	[DIKI_F2-DIKI_UNKNOWN]             =  FTK_KEY_F2,
	[DIKI_F3-DIKI_UNKNOWN]             =  FTK_KEY_F3,
	[DIKI_F4-DIKI_UNKNOWN]             =  FTK_KEY_F4,
	[DIKI_F5-DIKI_UNKNOWN]             =  FTK_KEY_F5,
	[DIKI_F6-DIKI_UNKNOWN]             =  FTK_KEY_F6,
	[DIKI_F7-DIKI_UNKNOWN]             =  FTK_KEY_F7,
	[DIKI_F8-DIKI_UNKNOWN]             =  FTK_KEY_F8,
	[DIKI_F9-DIKI_UNKNOWN]             =  FTK_KEY_F9,
	[DIKI_SHIFT_L-DIKI_UNKNOWN]        =  FTK_KEY_LEFTSHIFT,
	[DIKI_SHIFT_R-DIKI_UNKNOWN]        =  FTK_KEY_RIGHTSHIFT,
	[DIKI_CONTROL_L-DIKI_UNKNOWN]      =  FTK_KEY_LEFTCTRL,
	[DIKI_CONTROL_R-DIKI_UNKNOWN]      =  FTK_KEY_RIGHTCTRL,
	[DIKI_ALT_L-DIKI_UNKNOWN]          =  FTK_KEY_LEFTALT,
	[DIKI_ALT_R-DIKI_UNKNOWN]          =  FTK_KEY_RIGHTALT,
	[DIKI_CAPS_LOCK-DIKI_UNKNOWN]      =  FTK_KEY_CAPSLOCK,
	[DIKI_NUM_LOCK-DIKI_UNKNOWN]       =  FTK_KEY_NUMBERSIGN,
	[DIKI_ESCAPE-DIKI_UNKNOWN]         =  FTK_KEY_ESC,
	[DIKI_LEFT-DIKI_UNKNOWN]           =  FTK_KEY_LEFT,
	[DIKI_RIGHT-DIKI_UNKNOWN]          =  FTK_KEY_RIGHT,
	[DIKI_UP-DIKI_UNKNOWN]             =  FTK_KEY_UP,
	[DIKI_DOWN-DIKI_UNKNOWN]           =  FTK_KEY_DOWN,
	[DIKI_TAB-DIKI_UNKNOWN]            =  FTK_KEY_TAB,
	[DIKI_ENTER-DIKI_UNKNOWN]          =  FTK_KEY_ENTER,
	[DIKI_SPACE-DIKI_UNKNOWN]          =  FTK_KEY_SPACE,
	[DIKI_BACKSPACE-DIKI_UNKNOWN]      =  FTK_KEY_BACKSPACE,
	[DIKI_INSERT-DIKI_UNKNOWN]         =  FTK_KEY_INSERT,
	[DIKI_DELETE-DIKI_UNKNOWN]         =  FTK_KEY_DELETE,
	[DIKI_HOME-DIKI_UNKNOWN]           =  FTK_KEY_HOME,
	[DIKI_END-DIKI_UNKNOWN]            =  FTK_KEY_END,
	[DIKI_PAGE_UP-DIKI_UNKNOWN]        =  FTK_KEY_PAGEUP,
	[DIKI_PAGE_DOWN-DIKI_UNKNOWN]      =  FTK_KEY_PAGEDOWN,
	[DIKI_QUOTE_LEFT-DIKI_UNKNOWN]     =  FTK_KEY_QUOTELEFT,
	[DIKI_MINUS_SIGN-DIKI_UNKNOWN]     =  FTK_KEY_MINUS,
	[DIKI_EQUALS_SIGN-DIKI_UNKNOWN]    =  FTK_KEY_EQUAL,
	[DIKI_BRACKET_LEFT-DIKI_UNKNOWN]   =  FTK_KEY_BRACKETLEFT,
	[DIKI_BRACKET_RIGHT-DIKI_UNKNOWN]  =  FTK_KEY_BRACKETRIGHT,
	[DIKI_BACKSLASH-DIKI_UNKNOWN]      =  FTK_KEY_BACKSLASH,
	[DIKI_SEMICOLON-DIKI_UNKNOWN]      =  FTK_KEY_SEMICOLON,
	[DIKI_QUOTE_RIGHT-DIKI_UNKNOWN]    =  FTK_KEY_QUOTERIGHT,
	[DIKI_COMMA-DIKI_UNKNOWN]          =  FTK_KEY_COMMA,
	[DIKI_PERIOD-DIKI_UNKNOWN]         =  FTK_KEY_PERIOD,
	[DIKI_SLASH-DIKI_UNKNOWN]          =  FTK_KEY_SLASH,
	[DIKI_LESS_SIGN-DIKI_UNKNOWN]      =  FTK_KEY_LESS
};
#endif
static int ftk_source_dfb_keymap(FtkSource* thiz, int keycode)
{
	ftk_logd("0x%03x --> 0x%03x %c\n", keycode, s_key_map[0xff&keycode], (char)s_key_map[0xff&keycode]);

	return s_key_map[0xff&keycode] ? s_key_map[0xff&keycode] : keycode;
}

static Ret ftk_source_dfb_dispatch_input_event(FtkSource* thiz, DFBInputEvent* event)
{
	DECL_PRIV(thiz, priv);
	switch(event->type)
	{
		case DIET_KEYPRESS:
		case DIET_KEYRELEASE:
		{
			//priv->event.u.key.code = ftk_source_dfb_keymap(thiz, event->key_code);
			priv->event.u.key.code = ftk_source_dfb_keymap(thiz, event->key_id);
			priv->event.type = event->type == DIET_KEYPRESS ? FTK_EVT_KEY_DOWN : FTK_EVT_KEY_UP;
			break;
		}
		case DIET_BUTTONPRESS:
		case DIET_BUTTONRELEASE:
		{
			priv->event.type = event->type == DIET_BUTTONPRESS ? FTK_EVT_MOUSE_DOWN : FTK_EVT_MOUSE_UP;
			priv->event.u.mouse.x = priv->x;
			priv->event.u.mouse.y = priv->y;
			break;
		}
		case DIET_AXISMOTION:
		{
			switch(event->axis)
			{
				case DIAI_Y:
				{
					priv->y = event->axisabs;
					break;
				}
				case DIAI_X:
				{
					priv->x = event->axisabs;
					break;
				}
				default:
				{
					break;
				}
			}
			priv->event.type = FTK_EVT_MOUSE_MOVE;
			priv->event.u.mouse.x = priv->x;
			priv->event.u.mouse.y = priv->y;
			break;
		}
		default:break;
	}

	if(priv->event.type != FTK_EVT_NOP)
	{
		ftk_wnd_manager_queue_event(ftk_default_wnd_manager(), &priv->event);
		priv->event.type = FTK_EVT_NOP;
	}

	return RET_OK;
}
static Ret  ftk_source_dfb_dispatch(FtkSource* thiz)
{
	int i = 0;
	int nr = 0;
	DFBEvent   buff[10];
	DECL_PRIV(thiz, priv);
	DFBEvent* event = NULL;
	int size =  read(priv->fd, buff, sizeof(buff));
	return_val_if_fail(size > 0, RET_FAIL);

	nr = size/sizeof(DFBEvent);

	for(i = 0; i < nr; i++)
	{
		event = buff+i;
		switch(event->clazz)
		{
			case DFEC_INPUT:
			{
				ftk_source_dfb_dispatch_input_event(thiz, &(event->input));
				break;
			}
			case DFEC_USER:
			{
				break;
			}
			case DFEC_WINDOW:
			{
				break;
			}
			default:break;
		}
	}

	return RET_OK;
}

static void ftk_source_dfb_destroy(FtkSource* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		close(priv->fd);
		if(priv->event_buffer != NULL)
		{
			priv->event_buffer->Release(priv->event_buffer);
		}
		FTK_ZFREE(thiz, sizeof(FtkSource) + sizeof(PrivInfo));
	}

	return;
}

FtkSource* ftk_source_dfb_create(IDirectFB* dfb)
{
	FtkSource* thiz = FTK_ZALLOC(sizeof(FtkSource)+sizeof(PrivInfo));

	if(thiz != NULL)
	{
		int fd = 0;
		DECL_PRIV(thiz, priv);
		IDirectFBEventBuffer* event_buffer = NULL;

		thiz->get_fd   =  ftk_source_dfb_get_fd;
		thiz->check    = ftk_source_dfb_check;
		thiz->dispatch = ftk_source_dfb_dispatch;
		thiz->destroy  = ftk_source_dfb_destroy;

		dfb->CreateInputEventBuffer( dfb, DICAPS_ALL, DFB_FALSE, &event_buffer );
		if(event_buffer != NULL)
		{
			event_buffer->CreateFileDescriptor(event_buffer, &fd);
		}
		priv->fd = fd;
		priv->event_buffer = event_buffer;
	}

	return thiz;
}


