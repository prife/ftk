LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := minilcd.c
LOCAL_MODULE_TAGS := eng

LOCAL_C_INCLUDES += \
    $(COMMON_EXTERNAL_DIR)/zlib \
	$(COMMON_EXTERNAL_DIRI)/safe-iop/include \
	pdk/frameworks/base/opengl/include

LOCAL_CFALGS := \
	-g \
	-O0

	#libui \
	#libpng \

LOCAL_SHARED_LIBRARIES := \
	libui \
	libEGL \
	libGLESv1_CM \
	libcutils

#LOCAL_MODULE_TAGS := user

LOCAL_MODULE := libminilcd

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
