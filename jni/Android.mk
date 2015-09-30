LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_MODULE := EncryptMsg

LOCAL_SRC_FILES := \
EncryptMsg.c \
aes256.c \
base64.c \
encode.c

LOCAL_LDLIBS :=  -llog

include $(BUILD_SHARED_LIBRARY)