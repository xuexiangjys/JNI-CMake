# 构建系统提供的宏函数 my-dir 将返回当前目录（包含 Android.mk 文件本身的目录）的路径，基本上是固定的，不需要去动
LOCAL_PATH := $(call my-dir)

# 会清除很多 LOCAL_XXX 变量，不会清除 LOCAL_PATH，基本上是固定的，不需要去动
include $(CLEAR_VARS)

# 需要构建模块的名称，会自动生成相应的 libNDKSample.so 文件，每个模块名称必须唯一，且不含任何空格
LOCAL_MODULE := native-lib

# 包含要构建到模块中的 C 或 C++ 源文件列表
LOCAL_SRC_FILES := native-lib.cpp

# 指定这个模块里会用到哪些原生 API，详见：https://developer.android.google.cn/ndk/guides/stable_apis.html
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

# 帮助系统将所有内容连接到一起，固定的，不需要去动
include $(BUILD_SHARED_LIBRARY)

