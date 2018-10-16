# JNI-CMake

Android使用CMake进行JNI开发演示

## 开发环境

> JNI：Java Native Interface（Java 本地编程接口），一套编程规范，它提供了若干的 API 实现了 Java 和其他语言的通信（主要是 C/C++）。Java 可以通过 JNI 调用本地的 C/C++ 代码，本地的 C/C++ 代码也可以调用 java 代码。Java 通过 C/C++ 使用本地的代码的一个关键性原因在于 C/C++ 代码的高效性。

在 Android Studio 下，进行JNI的开发，需要准备以下内容：

* Android Studio 2.2以上。

* NDK：这套工具集允许为 Android 使用 C 和 C++ 代码。

* CMake：一款外部构建工具，可与 Gradle 搭配使用来构建原生库。如果只计划使用 ndk-build，则不需要此组件。

* LLDB：一种调试程序，Android Studio 使用它来调试原生代码。

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/1.png)

## 创建支持C++的项目

### 新建支持C++的项目

在新建项目时，勾上`Include C++ support`就行了：

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/2.png)

在向导的 Customize C++ Support 部分，有下列自定义项目可供选择：

* C++ Standard：使用下拉列表选择使用哪种 C++ 标准。选择 Toolchain Default 会使用默认的 CMake 设置。
* Exceptions Support：如果希望启用对 C++ 异常处理的支持，请选中此复选框。如果启用此复选框，Android Studio 会将 -fexceptions 标志添加到模块级 build.gradle文件的 cppFlags中，Gradle 会将其传递到 CMake。
* Runtime Type Information Support：如果希望支持 RTTI，请选中此复选框。如果启用此复选框，Android Studio 会将 -frtti 标志添加到模块级 build.gradle文件的 cppFlags中，Gradle 会将其传递到 CMake。

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/3.png)