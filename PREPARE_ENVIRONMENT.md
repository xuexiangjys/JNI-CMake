# JNI开发环境和基础配置

## 开发环境

> JNI：Java Native Interface（Java 本地编程接口），一套编程规范，它提供了若干的 API 实现了 Java 和其他语言的通信（主要是 C/C++）。Java 可以通过 JNI 调用本地的 C/C++ 代码，本地的 C/C++ 代码也可以调用 java 代码。Java 通过 C/C++ 使用本地的代码的一个关键性原因在于 C/C++ 代码的高效性。

在 Android Studio 下，进行JNI的开发，需要准备以下内容：

* Android Studio 2.2以上。

* NDK：这套工具集允许为 Android 使用 C 和 C++ 代码。

* CMake：一款外部构建工具，可与 Gradle 搭配使用来构建原生库。如果只计划使用 ndk-build，则不需要此组件。

* LLDB：一种调试程序，Android Studio 使用它来调试原生代码。

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/1.png)

---------

## 创建支持C++的项目

### 新建支持C++的项目

在新建项目时，勾上`Include C++ support`就行了：

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/2.png)

在向导的 Customize C++ Support 部分，有下列自定义项目可供选择：

* C++ Standard：使用下拉列表选择使用哪种 C++ 标准。选择 Toolchain Default 会使用默认的 CMake 设置。
* Exceptions Support：如果希望启用对 C++ 异常处理的支持，请选中此复选框。如果启用此复选框，Android Studio 会将 -fexceptions 标志添加到模块级 build.gradle文件的 cppFlags中，Gradle 会将其传递到 CMake。
* Runtime Type Information Support：如果希望支持 RTTI，请选中此复选框。如果启用此复选框，Android Studio 会将 -frtti 标志添加到模块级 build.gradle文件的 cppFlags中，Gradle 会将其传递到 CMake。

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/3.png)

### 支持C++的项目目录

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/4.png)

* `src/main/cpp`下存放的我们编写供JNI调用的C++源码。

* `CMakeLists.txt`文件是CMake的配置文件,通常他包含的内容如下：

```
# TODO 设置构建本机库文件所需的 CMake的最小版本
cmake_minimum_required(VERSION 3.4.1)

# TODO 添加自己写的 C/C++源文件
add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp )

# TODO 依赖 NDK中的库
find_library( log-lib
              log )

# TODO 将目标库与 NDK中的库进行连接
target_link_libraries( native-lib
                       ${log-lib} )
```

### build.gradle的配置

```
android {
    ...
    defaultConfig {
        ...
        externalNativeBuild {
            cmake {
                // 默认是 “ cppFlags "" ”
                // 如果要修改 Customize C++ Support 部分，可在这里加入
                cppFlags "-frtti -fexceptions"
            }
        }

        ndk {
            // abiFiliter： ABI 过滤器（application binary interface，应用二进制接口）
            // Android 支持的 CPU 架构
            abiFilters 'armeabi-v7a','arm64-v8a','x86','x86_64'//, 'armeabi' 不支持了
        }
    }
    buildTypes {
        ...
    }
    externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }
}
```

## 注意事项

* 1.在使用JNI前，需要加载so库

```
static {
    System.loadLibrary("native-lib");
}
```

* 2.快速生成C++代码：先在java中定义native方法，然后使用`Alt + Enter`快捷键自动生成C++方法体。

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/5.png)

* 3.CPP 资源文件夹下面的文件和文件夹不能重名，不然 System.loadLibrary() 时找不到，会报错：java.lang.UnsatisfiedLinkError: Native method not found.

* 4.在定义库的名字时，不要加前缀 lib 和后缀 .so，不然会报错：java.lang.UnsatisfiedLinkError: Couldn’t load xxx : findLibrary【findLibrary returned null错误.

* 5.新建 C/C++ 源代码文件，要添加到 CMakeLists.txt 文件中。

```
# 增加c++源代码
add_library( # library的名称.
             native-lib

             # 标志库共享.
             SHARED

             # C++源码文件的相对路径.
             src/main/cpp/native-lib.cpp )

# 将目标库与 NDK中的库进行连接
target_link_libraries( # 目标library的名称.
                    native-lib
                    ${log-lib} )
```

* 6.引入第三方 .so文件，要添加到 CMakeLists.txt 文件中。

```
# TODO 添加第三方库
# TODO add_library(libavcodec-57
# TODO 原先生成的.so文件在编译后会自动添加上前缀lib和后缀.so，
# TODO       在定义库的名字时，不要加前缀lib和后缀 .so，
# TODO       不然会报错:java.lang.UnsatisfiedLinkError: Couldn't load xxx : findLibrary returned null
add_library(avcodec-57
            # TODO STATIC表示静态的.a的库，SHARED表示.so的库
            SHARED
            IMPORTED)
set_target_properties(avcodec-57
                      PROPERTIES IMPORTED_LOCATION
                      # TODO ${CMAKE_SOURCE_DIR}：表示 CMakeLists.txt的当前文件夹路径
                      # TODO ${ANDROID_ABI}：编译时会自动根据 CPU架构去选择相应的库
                      # TODO ABI文件夹上面不要再分层，直接就 jniLibs/${ANDROID_ABI}/
                      # TODO ${CMAKE_SOURCE_DIR}/src/main/jniLibs/ffmpeg/${ANDROID_ABI}/libavcodec-57.so
                      ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavcodec-57.so)
```

* 引入第三方 .h 文件夹，也要添加到 CMakeLists.txt 文件中

```
# TODO include_directories( src/main/jniLibs/${ANDROID_ABI}/include )
# TODO 路径指向上面会编译出错(无法在jniLibs中引入)，指向下面的路径就没问题
include_directories( src/main/cpp/ffmpeg/include )
```

* C++library编译生成的so文件，在 `build/intermediates/cmake` 下

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/6.png)