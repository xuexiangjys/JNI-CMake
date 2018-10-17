#include <jni.h>
#include <string>
#include <android/log.h>

#define TAG "CMake-JNI" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

void throwException(JNIEnv *env, const char *message) {
    jclass newExcCls = env->FindClass("java/lang/Exception");
    env->ThrowNew(newExcCls, message);
}

extern "C" JNIEXPORT jstring
JNICALL
Java_com_xuexiang_jnidemo_JNIApi_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallNoStaticField(JNIEnv *env, jobject instance) {

    //*********************************知识点：访问java成员变量，并且修改非静态变量值 *****************************


    /*      方法签名解释：（参数）返回值
     *           public void test(){}                   ()V
     *           public void test1(int i){}              (I)V
     *           public int test2(){ return 1;}         ()I
     *           public int test3(int i) { return i;}          (I)I
     *
     *      基本数据类型对应关系( Boolean 比较特殊, 对应的是 Z ， Long 对应 J)
     *          Z           boolean
     *          B           byte
     *          C           char
     *          S           short
     *          I           int
     *          J           long
     *          F           float
     *          D           double
     *
     *      引用数据类型(以“L”开头，以“；”结束，中间对应的是该类型的完整路径)
     *          String ： Ljava/lang/String;
     *          Ljava/lang/Object;
     *
     *          案例
     *              自定义类 MyClass对应完整类名com.example.administrator.ndk.Area
     *              Area ： Lcom/example/administrator/ndk/Area：
     *
     *      数组表示
     *          int [] ：[I
     *          Long[][]  ： [[J
     *          Object[][][] ： [[[Ljava/lang/Object
     *
     *
     *
     *
     *
     * */

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

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallStaticField(JNIEnv *env, jobject instance) {

    //*********************************知识点：访问java成员变量，并且修改静态变量值 *****************************
    //获取jclass
    jclass j_class = env->GetObjectClass(instance);
    //获取jfieldID
    jfieldID j_fid = env->GetStaticFieldID(j_class, "staticField", "Ljava/lang/String;");
    //获取java成员变量string值
    jstring j_str = static_cast<jstring>(env->GetStaticObjectField(j_class, j_fid));
    //jstring--string
    const char *chs = env->GetStringUTFChars(j_str, NULL);
    LOGI("staticField==%s", chs);//staticField==薛翔

    const char *tmp = "正走向人生巅峰！";
    jstring j_tmp = env->NewStringUTF(tmp);

    int length = env->GetStringUTFLength(j_str) + env->GetStringUTFLength(j_tmp);

    //修改值
    char buf[length];
    sprintf(buf, "%s%s", chs, tmp);

    jstring j_newString = env->NewStringUTF(buf);
    //Set<Type>Field    修改staticField的值
    env->SetStaticObjectField(j_class, j_fid, j_newString);

    env->DeleteLocalRef(j_tmp);
    env->ReleaseStringUTFChars(j_str, chs);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallNoParamMethod(JNIEnv *env, jobject instance) {
    //回调JNIApi中的noParamMethods
    jclass clazz = env->FindClass("com/xuexiang/jnidemo/JNIApi");
    if (clazz == NULL) {
        printf("find class Error");
        return;
    }
    jmethodID id = env->GetMethodID(clazz, "noParamMethod", "()V");
    if (id == NULL) {
        printf("find method Error");
    }
    env->CallVoidMethod(instance, id);
}

int number = 0;

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


extern "C"
JNIEXPORT jint JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallConstructorMethod(JNIEnv *env, jobject instance) {

    //*********************************知识点：c访问java自定义类(计算面积) com.example.administrator.ndk.MyClass *****************************
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

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_testCallStaticMethod(JNIEnv *env, jobject instance) {

    jclass clazz = env->FindClass("com/xuexiang/jnidemo/MainActivity");
    if (clazz == NULL) {
        printf("find class Error");
        return;
    }
    jmethodID id = env->GetStaticMethodID(clazz, "staticMethod", "()V");
    if (id == NULL) {
        printf("find method Error");
        return;
    }

    env->CallStaticVoidMethod(clazz, id);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xuexiang_jnidemo_JNIApi_getDisplayName(JNIEnv *env, jobject instance, jstring name_,
                                                      jint age) {

//    jclass String_clazz = env->FindClass("java/lang/String");
//
//    jmethodID concat_methodID = env->GetMethodID(String_clazz, "concat",
//                                                 "(Ljava/lang/String;)Ljava/lang/String;");
//    ///====================================
//    //需要在后面拼接的字符串...
//    char buf[64];
//    sprintf(buf, "今年%d岁", age);
//    jstring tmp = env->NewStringUTF(buf);
//
//    jobject str = env->CallObjectMethod(name_, concat_methodID, tmp);
//    const char *chars = env->GetStringUTFChars((jstring) str, 0);
//
//    //释放内存
//    env->DeleteLocalRef(tmp);
//    env->ReleaseStringUTFChars((jstring) str, chars);


    //*********************************知识点：jni层处理java层传入的String *****************************

    /*
     *          资源释放
     *                  JNI 基本数据类型是不需要释放的 ，
     *                  如 jint , jlong , jchar 等等 。
     *                  我们需要释放是引用数据类型，
     *                  当然也包括数组家族。如：jstring ，
     *                  jobject ，jobjectArray，jintArray 等等。
     *
     * */
    //需要被拼接的字符串
    char *name = const_cast<char *>(env->GetStringUTFChars(name_, NULL));
    //处理需要在后面拼接的字符串...
    char buf[100];
    sprintf(buf, "我的名字叫%s,今年%d岁", name, age);

    const char *c_text = ",来自江苏南京,现从事Android开发！";

    strcat(buf, c_text); //字符串进行拼接

    //释放
    env->ReleaseStringUTFChars(name_, name);

    //如果会有中文乱码问题，自行处理（可以调用java的String(byte[] bytes, String charsetName)）
    return env->NewStringUTF(buf);
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_xuexiang_jnidemo_JNIApi_getRandomIntArray(JNIEnv *env, jobject instance,
                                                         jint j_length) {
    int length = j_length;

    jintArray j_intArray = env->NewIntArray(j_length);

    jint *p_jint = env->GetIntArrayElements(j_intArray, NULL);

    for (int i = 0; i < length; ++i) {
        p_jint[i] = rand() % 100;  //随机生成0～100的数
    }
    env->ReleaseIntArrayElements(j_intArray, p_jint, JNI_OK);
    return j_intArray;
}

//比较函数
int compare(const void *a, const void *b) {
    return *(int *) a - *(int *) b;  //升序排序
//return *(int *)b - *(int *)a; //降序排序
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_sort(JNIEnv *env, jobject instance, jintArray array_) {
    jint *array = env->GetIntArrayElements(array_, NULL);

    int len = env->GetArrayLength(array_);

    //qsort最后一个参数是函数指针，int (*__comparator)(const void* __lhs, const void* __rhs))
    qsort(array, static_cast<size_t>(len), sizeof(int), compare);

    /**
    * 最后一个参数 mode
    *      JNI_OK no error
    *      JNI_ABORT, Java数组不进行更新，但是释放C/C++数组
    *      JNI_COMMIT，Java数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）
    */
    env->ReleaseIntArrayElements(array_, array, JNI_OK);
}

//创建全局引用(共享)
jstring global_jstring;
jboolean isRelease;

void thowException(JNIEnv *env);

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_setJNIReference(JNIEnv *env, jobject instance, jstring value_) {
    //*********************************知识点：jni层创建引用（全局、局部） *****************************
    /*      引用类型：局部引用和全局引用
     *              局部引用，通过DeleteLocalRef手动释放对象   env->DeleteLocalRef(j_str);
     *              全局引用，通过DeleteGlobalRef手动释放对象  env->DeleteGlobalRef(global_jstring);
     *
     *
     * */
    const char *value = env->GetStringUTFChars(value_, 0);

    global_jstring = static_cast<jstring>(env->NewGlobalRef(value_));

    env->ReleaseStringUTFChars(value_, value);

    isRelease = static_cast<jboolean>(false);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xuexiang_jnidemo_JNIApi_getJNIReference(JNIEnv *env, jobject instance) {
    if (isRelease) {
        return env->NewStringUTF("引用已被回收！");
    }
    return global_jstring;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_releaseJNIReference(JNIEnv *env, jobject instance) {
    if (!isRelease) {
        env->DeleteGlobalRef(global_jstring);
        isRelease = static_cast<jboolean>(true);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_JNIApi_throwException(JNIEnv *env, jobject instance) {

    //*********************************知识点：Java层捕捉native异常 *****************************
    //获取jclass
    jclass j_class = env->GetObjectClass(instance);
    //获取jfieldID
    jfieldID j_fid = env->GetFieldID(j_class, "method", "Ljava/lang/String;");
    //获取java成员变量int值
    jstring j_stirng = static_cast<jstring>(env->GetObjectField(instance, j_fid));
    //jstring--string
    const char *chs = env->GetStringUTFChars(j_stirng, NULL);
    LOGI("method==%s", chs);//method==暂时没被访问

    //根据获取到的 method来判断是否合法

    /**
     * strcmp
            C/C++函数，比较两个字符串
            设这两个字符串为str1，str2，
            若str1==str2，则返回零；
            若str1 > str2，则返回正数；
            若str1< str2，则返回负数。
     *
     */

    if (strcmp(chs, "我是薛翔，我选Android") != 0) {
        //认为抛出异常，给Java层处理
        throwException(env, "参数不合法！");
    }

}extern "C"
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