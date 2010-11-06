
#define PNG_SKIP_SETJMP_CHECK

#include <jni.h>
#include <android/log.h>
#include "ftk_typedef.h"
#include "ftk_event.h"
#include "ftk_log.h"
#include "ftk_key.h"
#include "ftk_globals.h"
#include "ftk_display.h"
#include "ftk_display_rotate.h"
#include "ftk_bitmap.h"
#include <png.h>
#include <zip.h>

/******************************************************************************/

#define TAG "FTK"

JNIEnv* mEnv = NULL;

jclass mActivity;

jmethodID midInitEGL;
jmethodID midCreateEGLSurface;
jmethodID midDestroyEGLSurface;
jmethodID midFlipEGL;
jmethodID midShowKeyboard;
jmethodID midHideKeyboard;
jmethodID midShowInputMethodPicker;
jmethodID midEnableFeature;
jmethodID midUpdateAudio;

extern "C" int FTK_MAIN(int argc, char* argv[]);
extern "C" void FTK_QUIT();
extern "C" void Android_SetScreenResolution(int width, int height);

static int render_enabled = 0;
static int surface_valid = 0;

static FtkEvent event;
static FtkEvent event1;

#if 0
	FTK_KEY_EXCLAM        =  0x021,
	FTK_KEY_QUOTEDBL      =  0x022,
	FTK_KEY_NUMBERSIGN    =  0x023,
	FTK_KEY_DOLLAR        =  0x024,
	FTK_KEY_PERCENT       =  0x025,
	FTK_KEY_AMPERSAND     =  0x026,
	FTK_KEY_QUOTERIGHT    =  0x027,
	FTK_KEY_PARENLEFT     =  0x028,
	FTK_KEY_PARENRIGHT    =  0x029,
	FTK_KEY_ASTERISK      =  0x02a,
	FTK_KEY_COLON         =  0x03a,
	FTK_KEY_LESS          =  0x03c,
	FTK_KEY_GREATER       =  0x03e,
	FTK_KEY_QUESTION      =  0x03f,
	FTK_KEY_ASCIICIRCUM   =  0x05e,
	FTK_KEY_UNDERSCORE    =  0x05f,
	FTK_KEY_QUOTELEFT     =  0x060,
	FTK_KEY_LEFTBRACE     =  0x07B,
	FTK_KEY_OR            =  0x07C,
	FTK_KEY_RIGHTBRACE    =  0x07D,
	FTK_KEY_NOT           =  0x07E,
	FTK_KEY_DOT           =  '.',
	FTK_KEY_HOME          =  0xFF50,
	FTK_KEY_PRIOR         =  0xFF55,
	FTK_KEY_NEXT          =  0xFF56,
	FTK_KEY_END           =  0xFF57,
	FTK_KEY_BEGIN         =  0xFF58,
	FTK_KEY_INSERT        =  0xFF63,
	FTK_KEY_ESC           =  0xFF1B,
	FTK_KEY_RIGHTCTRL   =  0xFFE4,
	FTK_KEY_LEFTCTRL    =  0xFFE3,
	FTK_KEY_RETURN      = FTK_KEY_F3,
	FTK_KEY_CAPSLOCK    =  0xFFE5,
	FTK_KEY_SEND        =  0xFFA1,
	FTK_KEY_REPLY       =  0xFFA2,
	FTK_KEY_SAVE        =  0xFFA3,
	FTK_KEY_BATTERY     =  0xFFA4,
	FTK_KEY_BLUETOOTH   =  0xFFA5,
	FTK_KEY_WLAN        =  0xFFA6,
#endif

#if 0
static const int s_key_map[] =
{
    /*KeyCodeUnknown*/[0] = 0,

    /*KeyCodeSoftLeft*/[1] = 0,
    /*KeyCodeSoftRight*/[2] = 0,
    /*KeyCodeHome*/[3] = 0,
    /*KeyCodeBack*/[4] = FTK_KEY_BACKSPACE,
    /*KeyCodeCall*/[5] = 0,
    /*KeyCodeEndCall*/[6] = 0,
    /*KeyCode0*/[7] = FTK_KEY_0,
    /*KeyCode1*/[8] = FTK_KEY_1,
    /*KeyCode2*/[9] = FTK_KEY_2,
    /*KeyCode3*/[10] = FTK_KEY_3,
    /*KeyCode4*/[11] = FTK_KEY_4,
    /*KeyCode5*/[12] = FTK_KEY_5,
    /*KeyCode6*/[13] = FTK_KEY_6,
    /*KeyCode7*/[14] = FTK_KEY_7,
    /*KeyCode8*/[15] = FTK_KEY_8,
    /*KeyCode9*/[16] = FTK_KEY_9,
    /*KeyCodeStar*/[17] = 0,
    /*KeyCodePound*/[18] = 0,
    /*KeyCodeDpadUp*/[19] = FTK_KEY_UP,
    /*KeyCodeDpadDown*/[20] = FTK_KEY_DOWN,
    /*KeyCodeDpadLeft*/[21] = FTK_KEY_LEFT,
    /*KeyCodeDpadRight*/[22] = FTK_KEY_RIGHT,
    /*KeyCodeDpadCenter*/[23] = 0,
    /*KeyCodeVolumeUp*/[24] = 0,
    /*KeyCodeVolumeDown*/[25] = 0,
    /*KeyCodePower*/[26] = FTK_KEY_POWER,
    /*KeyCodeCamera*/[27] = 0,
    /*KeyCodeClear*/[28] = 0,
    /*KeyCodeA*/[29] = FTK_KEY_A,
    /*KeyCodeB*/[30] = FTK_KEY_B,
    /*KeyCodeC*/[31] = FTK_KEY_C,
    /*KeyCodeD*/[32] = FTK_KEY_D,
    /*KeyCodeE*/[33] = FTK_KEY_E,
    /*KeyCodeF*/[34] = FTK_KEY_F,
    /*KeyCodeG*/[35] = FTK_KEY_G,
    /*KeyCodeH*/[36] = FTK_KEY_H,
    /*KeyCodeI*/[37] = FTK_KEY_I,
    /*KeyCodeJ*/[38] = FTK_KEY_J,
    /*KeyCodeK*/[39] = FTK_KEY_K,
    /*KeyCodeL*/[40] = FTK_KEY_L,
    /*KeyCodeM*/[41] = FTK_KEY_M,
    /*KeyCodeN*/[42] = FTK_KEY_N,
    /*KeyCodeO*/[43] = FTK_KEY_O,
    /*KeyCodeP*/[44] = FTK_KEY_P,
    /*KeyCodeQ*/[45] = FTK_KEY_Q,
    /*KeyCodeR*/[46] = FTK_KEY_R,
    /*KeyCodeS*/[47] = FTK_KEY_S,
    /*KeyCodeT*/[48] = FTK_KEY_T,
    /*KeyCodeU*/[49] = FTK_KEY_U,
    /*KeyCodeV*/[50] = FTK_KEY_V,
    /*KeyCodeW*/[51] = FTK_KEY_W,
    /*KeyCodeX*/[52] = FTK_KEY_X,
    /*KeyCodeY*/[53] = FTK_KEY_Y,
    /*KeyCodeZ*/[54] = FTK_KEY_Z,
    /*KeyCodeComma*/[55] = FTK_KEY_COMMA,
    /*KeyCodePeriod*/[56] = FTK_KEY_PERIOD,
    /*KeyCodeAltLeft*/[57] = FTK_KEY_LEFTALT,
    /*KeyCodeAltRight*/[58] = FTK_KEY_RIGHTALT,
    /*KeyCodeShiftLeft*/[59] = FTK_KEY_LEFTSHIFT,
    /*KeyCodeShiftRight*/[60] = FTK_KEY_RIGHTSHIFT,
    /*KeyCodeTab*/[61] = FTK_KEY_TAB,
    /*KeyCodeSpace*/[62] = FTK_KEY_SPACE,
    /*KeyCodeSym*/[63] = 0,
    /*KeyCodeExplorer*/[64] = 0,
    /*KeyCodeEnvelope*/[65] = 0,
    /*KeyCodeNewline*/[66] = 0,
    /*KeyCodeDel*/[67] = FTK_KEY_DELETE,
    /*KeyCodeGrave*/[68] = FTK_KEY_GRAVE,
    /*KeyCodeMinus*/[69] = FTK_KEY_MINUS,
    /*KeyCodeEquals*/[70] = FTK_KEY_EQUAL,
    /*KeyCodeLeftBracket*/[71] = FTK_KEY_BRACKETLEFT,
    /*KeyCodeRightBracket*/[72] = FTK_KEY_BRACKETRIGHT,
    /*KeyCodeBackslash*/[73] = FTK_KEY_BACKSLASH,
    /*KeyCodeSemicolon*/[74] = FTK_KEY_SEMICOLON,
    /*KeyCodeApostrophe*/[75] = FTK_KEY_APOSTROPHE,
    /*KeyCodeSlash*/[76] = FTK_KEY_SLASH,
    /*KeyCodeAt*/[77] = FTK_KEY_AT,
    /*KeyCodeNum*/[78] = 0,
    /*KeyCodeHeadSetHook*/[79] = 0,
    /*KeyCodeFocus*/[80] = 0,
    /*KeyCodePlus*/[81] = FTK_KEY_PLUS,
    /*KeyCodeMenu*/[82] = FTK_KEY_MENU,
    /*KeyCodeNotification*/[83] = 0,
    /*KeyCodeSearch*/[84] = 0,
    /*KeyCodePlayPause*/[85] = 0,
    /*KeyCodeStop*/[86] = 0,
    /*KeyCodeNextSong*/[87] = 0,
    /*KeyCodePreviousSong*/[88] = 0,
    /*KeyCodeRewind*/[89] = 0,
    /*KeyCodeForward*/[90] = 0,
    /*KeyCodeMute*/[91] = 0,
    /*KeyCodePageUp*/[92] = FTK_KEY_PAGEUP,
    /*KeyCodePageDown*/[93] = FTK_KEY_PAGEDOWN,
    /*KeyCodePictSymbols*/[94] = 0,
    /*KeyCodeSwitchCharset*/[95] = 0
};
#endif

extern "C" void Android_Log(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	__android_log_vprint(ANDROID_LOG_INFO, TAG, fmt, ap);
	va_end(ap);
}


zip* apk;
zip_file* file;

static void png_read(png_structp png_ptr, png_bytep data, png_size_t length)
{
	zip_fread(file, data, length);
}

extern "C" FtkBitmap* Android_LoadPng(const char* filename)
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int passes_nr = 0;
	FtkColor* dst = NULL;
	unsigned char* src = NULL;
	FtkBitmap* bitmap = NULL;
	FtkColor  bg = {0};
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep * row_pointers = NULL;

	bg.a = 0xff;

	filename++;
	ftk_logd("android_load_png: %s\n", filename);

	if ((file = zip_fopen(apk, filename, 0)) == NULL)
	{
		ftk_logd("%s: zip_fopen %s failed.\n", __func__, filename);
		return NULL;
	}

	if((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL)
	{
		zip_fclose(file);
		return NULL;
	}
	
	if((info_ptr = png_create_info_struct(png_ptr)) == NULL)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		zip_fclose(file);
		return NULL;
	}

#ifdef PNG_SETJMP_SUPPORTED
	setjmp(png_jmpbuf(png_ptr));
#endif
 
	// png_init_io(png_ptr, fp);
	png_set_read_fn(png_ptr, NULL, png_read);

	memset(info_ptr, 0x00, sizeof(*info_ptr));
	png_read_info(png_ptr, info_ptr);

	int bit_depth, color_type;
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) &w, (png_uint_32 *) &h, &bit_depth, &color_type,
		NULL, NULL, NULL);

	passes_nr = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

#ifdef PNG_SETJMP_SUPPORTED
	setjmp(png_jmpbuf(png_ptr));
#endif

	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	row_pointers = (png_bytep*) FTK_ZALLOC(h * sizeof(png_bytep));
	for (y=0; y< h; y++)
	{
		row_pointers[y] = (png_byte*) FTK_ZALLOC(rowbytes);
	}
	png_read_image(png_ptr, row_pointers);

	int num_trans;
	png_color_16p trans_color;
	png_get_tRNS(png_ptr, info_ptr, NULL, &num_trans, &trans_color);

	bitmap = ftk_bitmap_create(w, h, bg);
	dst = ftk_bitmap_bits(bitmap);
	if (color_type == PNG_COLOR_TYPE_RGBA)
	{
		ftk_logd("PNG_COLOR_TYPE_RGBA\n");

		for(y = 0; y < h; y++)
		{
			src = row_pointers[y];
			for(x = 0; x < w; x++)
			{
				if(src[3])
				{
					dst->r = src[2];//src[0];
					dst->g = src[1];
					dst->b = src[0];//src[2];
				}
				dst->a = src[3];
				src +=4;
				dst++;
			}
		}
	}
	else if(color_type == PNG_COLOR_TYPE_RGB)
	{
		ftk_logd("PNG_COLOR_TYPE_RGB\n");

		if(0 == num_trans)
		{
			for(y = 0; y < h; y++)
			{
				src = row_pointers[y];
				for(x = 0; x < w; x++)
				{
					dst->r = src[2];//src[0];
					dst->g = src[1];
					dst->b = src[0];//src[2];
					dst->a = 0xff;
					src += 3;
					dst++;
				}
			}
		}
		else 
		{
			png_byte red = trans_color->red & 0xff;
			png_byte green = trans_color->green & 0xff;
			png_byte blue = trans_color->blue & 0xff;

			for(y = 0; y < h; y++)
			{
				src = row_pointers[y];
				for(x = 0; x < w; x++)
				{
					if(src[0] == red && src[1] == green && src[2] == blue)
					{
						dst->a = 0;
					}
					else
					{
						dst->a = 0xff;
					}
					dst->r = src[0];
					dst->g = src[1];
					dst->b = src[2];
					src += 3;
					dst++;
				}
			}
		}
	}
	else
	{
		assert(!"not supported.");
	}

	for(y = 0; y < h; y++)
	{
		FTK_FREE(row_pointers[y]);
	}
	FTK_FREE(row_pointers);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL); 

	zip_fclose(file);

	return bitmap;
}

/******************************************************************************/

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;

	if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		return -1;
	}

	ftk_logd("JNI: OnLoad()");

	mEnv = env;
	jclass cls = mEnv->FindClass("org/libftk/app/FtkActivity");
	mActivity = cls;

	midInitEGL = mEnv->GetStaticMethodID(cls, "initEGL", "()V");
	midCreateEGLSurface = mEnv->GetStaticMethodID(cls, "createEGLSurface", "()V");
	midDestroyEGLSurface = mEnv->GetStaticMethodID(cls, "destroyEGLSurface", "()V");
	midFlipEGL = mEnv->GetStaticMethodID(cls, "flipEGL", "()V");
	midShowKeyboard = mEnv->GetStaticMethodID(cls, "showKeyboard", "()V");
	midHideKeyboard = mEnv->GetStaticMethodID(cls, "hideKeyboard", "()V");
	midShowInputMethodPicker = mEnv->GetStaticMethodID(cls, "showInputMethodPicker", "()V");

	if(!midInitEGL || !midCreateEGLSurface || !midDestroyEGLSurface || !midFlipEGL
		|| !midShowKeyboard || !midHideKeyboard || !midShowInputMethodPicker)
	{
		ftk_logd("Bad mids");
	}
	else
	{
		ftk_logd("Good mids");
	}

	return JNI_VERSION_1_4;
}

extern "C" void Java_org_libftk_app_FtkActivity_nativeInit(JNIEnv* env, jobject obj)
{
	int argc, rv;
	char* argv[2];

	ftk_logd("native init");

	mEnv = env;

	argv[0] = (char *) "ftk";
	argv[1] = NULL;
	argc = 1;
	rv = FTK_MAIN(argc, argv);
}

extern "C" void Java_org_libftk_app_FtkActivity_nativeQuit(JNIEnv* env, jobject obj)
{
	FTK_QUIT();
	ftk_logd("native quit");
}

extern "C" void Java_org_libftk_app_FtkActivity_nativeEnableRender(JNIEnv* env, jobject obj)
{
	render_enabled = 1;
}

extern "C" void Java_org_libftk_app_FtkActivity_nativeDisableRender(JNIEnv* env, jobject obj)
{
	render_enabled = 0;
	surface_valid = 0;
}

extern "C" void Java_org_libftk_app_FtkActivity_onNativeKey(JNIEnv* env, jobject obj, jint action, jint keyCode)
{
	ftk_logd("native key action:%d keyCode:%d", action, keyCode);

	if(action == 2)
	{
		return;
	}
	//event.type = action == 0 ? FTK_EVT_KEY_DOWN : FTK_EVT_KEY_UP;
	//event.u.key.code = s_key_map[keyCode];
	//ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &event);
}

extern "C" void Java_org_libftk_app_FtkActivity_onNativeCommitText(JNIEnv* env, jobject obj, jstring text, jint newCursorPosition)
{
	char* buf;
	jsize len;
	const char *str;

	len = env->GetStringUTFLength(text);
	str = env->GetStringUTFChars(text, NULL);
	buf = (char*)FTK_ALLOC(len + 1);
	memcpy(buf, str, len);
	buf[len] = '\0';
	env->ReleaseStringUTFChars(text, str);

	memset(&event, 0, sizeof(event));
	event.u.extra = buf;
	event.type = FTK_EVT_OS_IM_COMMIT;
	ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &event);
}

extern "C" void Java_org_libftk_app_FtkActivity_onNativeTouch(JNIEnv* env, jobject obj, jint action, jfloat x, jfloat y, jfloat p)
{
	ftk_logd("native touch event %d @ %f/%f, pressure %f", action, x, y, p);
	memset(&event, 0, sizeof(event));
	if(action==0)
	{
		event.type = FTK_EVT_MOUSE_DOWN;
	}
	else if(action == 1 || action == 3)
	{
		event.type = FTK_EVT_MOUSE_UP;
	}
	else if(action == 2)
	{
		event.type = FTK_EVT_MOUSE_MOVE;
	}
	else
	{
		return;
	}
	event.u.mouse.x = x;
	event.u.mouse.y = y;
	ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &event);
}

extern "C" void Java_org_libftk_app_FtkActivity_nativeSetApkFilePath(JNIEnv*  env, jobject obj, jstring apkFilePath)
{
	const char* str = env->GetStringUTFChars(apkFilePath, NULL);

	ftk_logd("loading apk: %s", str);

	apk = zip_open(str, 0, NULL);
	if(apk == NULL)
	{
		ftk_loge("loading apk failed");
		env->ReleaseStringUTFChars(apkFilePath, str);
		return;
	}

	env->ReleaseStringUTFChars(apkFilePath, str);
}

extern "C" void Java_org_libftk_app_FtkActivity_nativeSetScreenSize(JNIEnv* env, jobject obj, jint width, jint height)
{
	ftk_logd("nativeSetScreenSize() %dX%d", width, height);
	Android_SetScreenResolution(width, height);
}

extern "C" void Java_org_libftk_app_FtkActivity_onNativeResize(JNIEnv* env, jobject obj, jint width, jint height, jint format)
{
	int w, h;
	FtkDisplay* display = NULL;
	FtkRotate r = FTK_ROTATE_0;

	display = ftk_default_display();
	r = ftk_display_get_rotate(display);
	w = ftk_display_width(display);
	h = ftk_display_height(display);

	ftk_logd("onNativeResize() old size:%dX%d new size:%dX%d format:%d", w, h, width, height, format);

	if(width != w && height != h)
	{
		memset(&event, 0, sizeof(event));
		if(r == FTK_ROTATE_0)
		{
			event.u.extra = (void*)FTK_ROTATE_90;
		}
		else
		{
			event.u.extra = (void*)FTK_ROTATE_0;
		}
		event.type = FTK_EVT_OS_SCREEN_ROTATED;
		//ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &event);
	}

	memset(&event1, 0, sizeof(event1));
	event1.type = FTK_EVT_RELAYOUT_WND;
	ftk_wnd_manager_queue_event_auto_rotate(ftk_default_wnd_manager(), &event1);
}

/******************************************************************************/

extern "C" void Android_InitEGL()
{
	ftk_logd("Android_InitEGL()");
	mEnv->CallStaticVoidMethod(mActivity, midInitEGL);
}

extern "C" Ret Android_PreRender()
{
	if(!render_enabled)
	{
		if(surface_valid)
		{
			mEnv->CallStaticVoidMethod(mActivity, midDestroyEGLSurface);
		}
		return RET_FAIL;
	}
	if(!surface_valid)
	{
		mEnv->CallStaticVoidMethod(mActivity, midCreateEGLSurface);
		surface_valid = 1;
	}
	return RET_OK;
}

extern "C" void Android_Render()
{
	if(!render_enabled || !surface_valid)
	{
		return;
	}
	mEnv->CallStaticVoidMethod(mActivity, midFlipEGL);
}

extern "C" void Android_ShowKeyboard()
{
	ftk_logd("Android_ShowKeyboard()");
	mEnv->CallStaticVoidMethod(mActivity, midShowKeyboard);
}

extern "C" void Android_HideKeyboard()
{
	ftk_logd("Android_HideKeyboard()");
	mEnv->CallStaticVoidMethod(mActivity, midHideKeyboard);
}

extern "C" void Android_ShowInputMethodChooser()
{
	ftk_logd("Android_ShowInputMethodChooser()");
	mEnv->CallStaticVoidMethod(mActivity, midShowInputMethodPicker);
}
