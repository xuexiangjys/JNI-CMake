package com.xuexiang.jnidemo;

/**
 *
 * @author xuexiang
 * @since 2018/10/16 下午4:54
 */
public class JNIApi {

    /**
     * 非静态成员变量
     */
    public int noStaticField = 0;
    /**
     * 静态成员变量
     */
    public static String staticField = "薛翔";

    public String method = "薛翔";

    private MainActivity.OnShowLogListener mOnShowLogListener;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public JNIApi(MainActivity.OnShowLogListener logListener) {
        mOnShowLogListener = logListener;
    }

    public void noParamMethod() {
        mOnShowLogListener.onShowLog("无参的Java方法被调用了");
    }

    public void paramMethod(int number) {
        mOnShowLogListener.onShowLog("有参的Java方法被调用了" + number + "次");
    }

    public void clear() {
        mOnShowLogListener = null;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * jni访问java非静态成员变量
     */
    public native void testCallNoStaticField();

    /**
     * jni访问java静态成员变量
     */
    public native void testCallStaticField();

    /**
     * jni调用无参方法
     */
    public native void testCallNoParamMethod();

    /**
     * jni调用有参方法
     */
    public native void testCallParamMethod();

    /**
     * jni调用静态方法
     */
    public native void testCallStaticMethod();

    /**
     * jni调用自定义类的构造方法
     */
    public native int testCallConstructorMethod();

    /**
     * jni调用有返回值
     */
    public native String getDisplayName(String name, int age);

    /**
     * 获取随机数组
     * @param length
     * @return
     */
    public native int[] getRandomIntArray(int length);

    /**
     * 对数组进行排序
     * @param array
     */
    public native void sort(int[] array);

    /**
     * jni引用
     */
    public native void setJNIReference(String value);

    /**
     * 获取jni引用
     * @return
     */
    public native String getJNIReference();

    /**
     * 释放jni引用
     * @return
     */
    public native void releaseJNIReference();

    /**
     * jni上抛错误
     */
    public native void throwException() throws Exception;

    /**
     * jni层进行异常捕获
     */
    public native void jniTryCatchException() throws Exception;

}
