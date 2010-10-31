LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libftkapp

LOCAL_CFLAGS :=	\
	-I$(LOCAL_PATH)/../../../../../src \
	-I$(LOCAL_PATH)/../../../../../src/im \
	-I$(LOCAL_PATH)/../../../../../src/backend/android \
	-I$(LOCAL_PATH)/../../../../../src/os/android

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../libzip \
	$(LOCAL_PATH)/../libpng

LOCAL_SRC_FILES := \
	ftk_jni.cpp \
	../../../../../src/demos/demo_entry.c

LOCAL_STATIC_LIBRARIES := libftk libft2 libpng libzip

LOCAL_LDLIBS := -lGLESv1_CM -llog -lgcc -lz

include $(BUILD_SHARED_LIBRARY)
