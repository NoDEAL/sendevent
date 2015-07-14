LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDFLAGS   += -static
LOCAL_CFLAGS    += -std=c99
LOCAL_MODULE    := sendevent_i9300
LOCAL_SRC_FILES := sendevent_i9300.c

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_LDFLAGS   += -static
LOCAL_CFLAGS    += -std=c99
LOCAL_MODULE    := sendevent_ef63l
LOCAL_SRC_FILES := sendevent_ef63l.c

include $(BUILD_EXECUTABLE)
