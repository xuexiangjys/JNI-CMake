# 使用NDK和Android.mk进行JNI开发

## 1.编写native接口和C/C++代码

### 定义native接口

```
package com.xuexiang.jnidemo;

public class JNIApi {

    public native String stringFromJNI();
}
```

### 编写C/C++代码
```
extern "C" JNIEXPORT jstring
JNICALL
Java_com_xuexiang_jnidemo_JNIApi_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
```

## 2.编写Android.mk

模版如下：

```
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := native-lib

LOCAL_SRC_FILES := native-lib.cpp

## 导入logcat日志库
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
```

说明：

* `LOCAL_PATH := $(call my-dir)` :指向当前目录的地址，包含该.mk

* `include $(CLEAR_VARS)`:清理掉所有以LOCAL_开头的内容,这句话是必须的，因为如果所有的变量都是全局的，所有的可控的编译文件都需要在一个单独的GNU中被解析并执行。

* `LOCAL_MODULE`:调用的库名，用来区分android.mk中的每一个模块。文件名必须是唯一的，不能有空格。注意，这里编译器会为你自动加上一些前缀lib和后缀.so，来保证文件是一致的。

* `LOCAL_SRC_FILES`:变量必须包含一个C、C++或者java源文件的列表，这些会被编译并聚合到一个模块中,文件之间可以用空格或Tab键进行分割,换行请用"\\"

* `LOCAL_LDLIBS`:定义需要链接的库。一般用于链接那些存在于系统目录下本模块需要链接的库（比如这里的logcat库）。

* `include $(BUILD_SHARED_LIBRARY)`:来生成一个动态库libnative-lib.so

## 3.编写Application.mk

```
# APP_ABI := armeabi armeabi-v7a arm64-v8a x86
APP_ABI := all
APP_OPTIM := release

## 引用静态库
APP_STL := stlport_static
#NDK_TOOLCHAIN_VERSION=4.8
#APP_PLATFORM := android-14
```

说明：

* `APP_ABI`：定义编译so文件的CPU型号，all为所有类型。也可以指定特定类型的CPU型号，直接使用空格隔开。

* `APP_OPTIM`:优化选项，非必填。其值可以为'release'或'debug'.此变量用来修改优先等级.默认情况下为release.在release模式下，将编译生成被优化了的二进制的机器码，而debug模块用来生成便于调试的未被优化的二进制机器码。

* `APP_STL`:选择支持的C++标准库。在默认情况下，NDK通过Androoid自带的最小化的C++运行库(system/lib/libstdc++.so)来提供标准C++头文件.然而，NDK提供了可供选择的C++实现，你可以通过此变量来选择使用哪个或链接到你的程序。

```
APP_STL := stlport_static    --> static STLport library
APP_STL := stlport_shared    --> shared STLport library
APP_STL := system            --> default C++ runtime library
```
比如，这里我们使用到了`#include <string>`，就需要设置`stlport_static`

## 4.编译C/C++代码生成so文件

cd 到jni（存放Android.mk的目录)下，执行`ndk-build`即可。

执行成功后，将会在jni的同级目录下生成`libs`和`obj`文件夹，存放的是编译好的so文件。

## 5.在模块的build.gradle中设置so文件路径

```
sourceSets {
    main {
        jni.srcDirs = []
        jniLibs.srcDirs = ['src/main/libs']
    }
}
```

## 6.设置项目根目录的local.properties文件

因为Android Studio 2.2以后推荐使用CMake进行JNI开发，因此需要修改一下参数进行兼容。

```
android.useDeprecatedNdk=true
```


至此大功告成，可以愉快地进行jni开发了！