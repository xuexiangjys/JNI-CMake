#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_xuexiang_jnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_MainActivity_testCallNoParamMethod(JNIEnv *env, jobject instance) {
    //回调MainActivity中的noParamMethod
    jclass clazz = env->FindClass("com/xuexiang/jnidemo/MainActivity");
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

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_MainActivity_testCallParamMethod(JNIEnv *env, jobject instance) {
    //回调MainActivity中的noParamMethod
    jclass clazz = env->FindClass("com/xuexiang/jnidemo/MainActivity");
    if (clazz == NULL) {
        printf("find class Error");
        return;
    }
    jmethodID id = env->GetMethodID(clazz, "paramMethod", "(I)V");
    if (id == NULL) {
        printf("find method Error");
    }
    env->CallVoidMethod(instance, id, 10);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xuexiang_jnidemo_MainActivity_testCallStaticMethod(JNIEnv *env, jobject instance) {

    jclass clazz = env->FindClass("com/xuexiang/jnidemo/MainActivity");
    if (clazz == NULL) {
        printf("find class Error");
        return;
    }
    jmethodID id = env->GetStaticMethodID(clazz, "staticMethod", "()V");
    if (id == NULL) {
        printf("find method Error");
    }

    env->CallStaticVoidMethod(clazz, id);
}