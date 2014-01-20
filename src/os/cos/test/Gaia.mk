LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ftktest

LOCAL_MODULE_TAGS := eng

FTK_ROOT       = $(LOCAL_PATH)/../../../../

LOCAL_CFLAGS :=	\
	-g


LOCAL_C_INCLUDES := \
	$(FTK_ROOT)/src

#ifeq ($(DISPLAY_ENGINE), EGL)
#	CFLAGS += -DUSE_LINUX_FB -DUSE_LINUX_NATIVE
#endif

FTK_SOURCE := ftk_test.c

LOCAL_SHARED_LIBRARIES := \
	libftk \
	libjpeg \
	libpng \
	libEGL \
	libGLESv1_CM \
	libcutils

LOCAL_SRC_FILES := $(FTK_SOURCE:$(LOCAL_PATH)/%=%)

#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_EXECUTABLE)
