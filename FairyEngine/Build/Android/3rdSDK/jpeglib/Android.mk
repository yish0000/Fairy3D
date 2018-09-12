LOCAL_MODULE := jpeglib_static

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VAR)

ifeq ($(APP_OPTIM), release)
	LOCAL_MODULE_FILENAME := libjpeg
else
	LOCAL_MODULE_FILENAME := libjpeg_d
endif

LOCAL_CFLAGS := \
	-DANDROID_NDK

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../Source/3rdSDK/jpeglib

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../Source/3rdSDK/jpeglib

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/autofit/autofit.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/bdf/bdg.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/ccf/ccf.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftbase.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftbitmap.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/cache/ftcache.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/ftdebug.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftfstype.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftgasp.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftglyph.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/gzip/ftgzip.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftinit.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/lzw/ftlzw.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftstroke.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftsystem.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/smooth/smooth.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftbbox.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftgxval.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftlcdfil.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftmm.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftotval.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftpatent.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftpfr.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftsynth.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/fttype1.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftwinfnt.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/base/ftxf86.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/pcf/pcf.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/pfr/pfr.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/psaux/psaux.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/pshinter/pshinter.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/psnames/psmodule.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/raster/raster.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/sfnt/sfnt.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/truetype/truetype.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/type1/type1.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/cid/type1cid.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/type42/type42.c \
	$(LOCAL_PATH)/../../../../Source/3rdSDK/freetype/src/winfonts/winfnt.c

include $(BUILD_STATIC_LIBRARY)