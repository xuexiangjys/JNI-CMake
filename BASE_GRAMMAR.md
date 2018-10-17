# 基础JNI语法

## 基础类型

Java类型 | native类型 | 描述
:-|:-:|:-
boolean | jboolean | unsigned 8 bits
byte | jbyte | signed 8 bits
char | jchar | unsigned 16 bits
short | jshort | signed 16 bits
int | jint | signed 32 bits
long | jlong | signed 64 bits
float | jfloat | 32 bits
double | jdouble | 64 bits
void | void | N/A

## 引用类型

JNI为不同的java对象提供了不同的引用类型，JNI引用类型如下：

![](http://static.zybuluo.com/shensky711/xdjuj8ypjwh6v9858yy36okl/image_1aurdi9ud1jtgtngelh1fm6st7g.png)

在c里面，所有JNI引用类型其实都是jobject。

## Native方法参数

* JNI接口指针是native方法的第一个参数，JNI接口指针的类型是JNIEnv。
* 第二个参数取决于native method是否静态方法，如果是非静态方法，那么第二个参数是对对象的引用，如果是静态方法，则第二个参数是对它的class类的引用
* 剩下的参数跟Java方法参数一一对应

```
extern "C" /* specify the C calling convention */
jdouble Java_pkg_Cls_f__ILjava_lang_String_2 (

     JNIEnv *env,        /* interface pointer */

     jobject obj,        /* "this" pointer */

     jint i,             /* argument #1 */

     jstring s)          /* argument #2 */
{

     const char *str = env->GetStringUTFChars(s, 0);

     ...

     env->ReleaseStringUTFChars(s, str);

     return ...

}

```

[点击查看JNI接口](https://github.com/xuexiangjys/JNI-CMake/blob/master/JNI_INTERFACE.md)


## 签名描述

### 基础数据类型

Java类型 | 签名描述
:-|:-
boolean | Z
byte | B
char | C
short | S
int | I
long | J
float | F
double | D
void |

### 引用数据类型

(以`L`开头，以`;`结束，中间对应的是该类型的完整路径)

```
String ： Ljava/lang/String;
Object ： Ljava/lang/Object;
自定义类型 Area ： Lcom/xuexiang/jnidemo/Area;
```

### 数组

(在类型前面添加`[`,几维数组就在前面添加几个`[`)

```
int [] ：[I
Long[][]  ： [[J
Object[][][] ： [[[Ljava/lang/Object
```

### 使用命令查看

```
javap -s <java类的class文件路径>

```

class文件存在于 `build->intermediates->classes`下。

![](https://github.com/xuexiangjys/JNI-CMake/blob/master/img/7.png)


## JNI常见用法

### 1、jni访问java非静态成员变量

* 1.使用`GetObjectClass`、`FindClass`获取调用对象的类

* 2.使用`GetFieldID`获取字段的ID。这里需要传入字段类型的签名描述。

* 3.使用`GetIntField`、`GetObjectField`等方法，获取字段的值。使用`SetIntField`、`SetObjectField`等方法，设置字段的值。

注意：即使字段是`private`也照样可以正常访问。

```
extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallNoStaticField(JNIEnv *env, jobject instance) {
    //获取jclass
    jclass j_class = env->GetObjectClass(instance);
    //获取jfieldID
    jfieldID j_fid = env->GetFieldID(j_class, "noStaticField", "I");
    //获取java成员变量int值
    jint j_int = env->GetIntField(instance, j_fid);
    LOGI("noStaticField==%d", j_int);//noStaticField==0

    //Set<Type>Field    修改noStaticKeyValue的值改为666
    env->SetIntField(instance, j_fid, 666);
}
```

### 2、jni访问java静态成员变量

* 1.使用`GetObjectClass`、`FindClass`获取调用对象的类

* 2.使用`GetStaticFieldID`获取字段的ID。这里需要传入字段类型的签名描述。

* 3.使用`GetStaticIntField`、`GetStaticObjectField`等方法，获取字段的值。使用`SetStaticIntField`、`SetStaticObjectField`等方法，设置字段的值。

### 3、jni调用java非静态成员方法

* 1.使用`GetObjectClass`、`FindClass`获取调用对象的类

* 2.使用`GetMethodID`获取方法的ID。这里需要传入方法的签名描述。

* 3.使用`CallVoidMethod`执行无返回值的方法，使用`CallIntMethod`、`CallBooleanMethod`等执行有返回值的方法。

```
extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallParamMethod(JNIEnv *env, jobject instance) {
    //回调JNIApi中的noParamMethod
    jclass clazz = env->FindClass("com/xuexiang/jnidemo/JNIApi");
    if (clazz == NULL) {
        printf("find class Error");
        return;
    }
    jmethodID id = env->GetMethodID(clazz, "paramMethod", "(I)V");
    if (id == NULL) {
        printf("find method Error");
        return;
    }
    env->CallVoidMethod(instance, id, ++number);
}
```

### 4、jni调用java静态成员方法

* 1.使用`GetObjectClass`、`FindClass`获取调用对象的类

* 2.使用`GetStaticMethodID`获取方法的ID。这里需要传入方法的签名描述。

* 3.使用`CallStaticVoidMethod`执行无返回值的方法，使用`CallStaticIntMethod`、`CallStaticBooleanMethod`等执行有返回值的方法。

### 5、jni调用java构造方法

* 1.使用`FindClass`获取需要构造的类

* 2.使用`GetMethodID`获取构造方法的ID。方法名为`<init>`, 这里需要传入方法的签名描述。

* 3.使用`NewObject`执行创建对象。

```
extern "C"
JNIEXPORT jint JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallConstructorMethod(JNIEnv *env, jobject instance) {
    //获取jclass
    jclass j_class = env->FindClass("com/xuexiang/jnidemo/Area");
    //找到构造方法jmethodID   public Area(int width, int height)
    jmethodID j_constructor_methoid = env->GetMethodID(j_class, "<init>", "(II)V");
    //初始化java类构造方法  public Area(int width, int height)
    jobject j_Area_obj = env->NewObject(j_class, j_constructor_methoid, 2, 10);

    //找到getArea()  jmethodID
    jmethodID j_getArea_methoid = env->GetMethodID(j_class, "getArea", "()I");
    //调用java中的   public int getArea() 获取面积
    jint j_area = env->CallIntMethod(j_Area_obj, j_getArea_methoid);
    LOGI("面积==%d", j_area);//面积==20
    return j_area;
}
```

### 6、jni引用全局变量

* 使用`NewGlobalRef`创建全局引用,使用`NewLocalRef`创建局部引用。

* 局部引用，通过DeleteLocalRef手动释放对象;全局引用，通过DeleteGlobalRef手动释放对象。

* 引用不主动释放会导致内存泄漏。

### 7、jni异常处理

* 使用`ExceptionOccurred`进行异常的检测。注意，这里只能检测java异常。

* 使用`ExceptionClear`进行异常的清除。

* 使用`ThrowNew`来上抛异常。

注意，`ExceptionOccurred`和`ExceptionClear`一般是成对出现的，类似于java的try-catch。

```
//上抛java异常
void throwException(JNIEnv *env, const char *message) {
    jclass newExcCls = env->FindClass("java/lang/Exception");
    env->ThrowNew(newExcCls, message);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_jniTryCatchException(JNIEnv *env, jobject instance) {

    //获取jclass
    jclass j_class = env->GetObjectClass(instance);
    //获取jfieldID
    jfieldID j_fid = env->GetFieldID(j_class, "method", "Ljava/lang/String666;");

    //检测是否发生Java异常
    jthrowable exception = env->ExceptionOccurred();
    if (exception != NULL) {
        LOGE("jni发生异常");
        //jni清空异常信息
        env->ExceptionClear(); //需要和ExceptionOccurred方法成对出现
        throwException(env, "native出错！");
    }
}
```

### 8、日志打印

```
#include <android/log.h> //引用android log

//定义日志打印的方法
#define TAG "CMake-JNI" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

LOGE("jni发生异常"); //日志打印
```

### 相关连接

* [Android JNI基础篇](https://blog.csdn.net/kgdwbb/article/details/72810251)
