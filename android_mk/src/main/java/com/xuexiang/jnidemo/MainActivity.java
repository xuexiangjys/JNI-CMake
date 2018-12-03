package com.xuexiang.jnidemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;

import com.xuexiang.mkdemo.R;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity implements OnClickListener {

    private static final String TAG = "xuexiang";

    private TextView tv;

    private int[] array;

    private JNIApi mJNIApi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mJNIApi = new JNIApi(new OnShowLogListener() {
            @Override
            public void onShowLog(String msg) {
                showLog(msg);
            }
        });
        // Example of a call to a native method
        tv = findViewById(R.id.sample_text);
        showLog(mJNIApi.stringFromJNI());
    }

    public static void staticMethod() {
        Log.e(TAG, "静态的Java方法被调用了");
    }

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.btn_call_no_static_field:
                mJNIApi.testCallNoStaticField();
                showLog("noStaticField修改前=0\n修改后=" + mJNIApi.getNoStaticField());
                break;
            case R.id.btn_call_static_field:
                mJNIApi.testCallStaticField();
                showLog("staticField修改前=薛翔\n修改后=" + JNIApi.staticField);
                break;
            case R.id.btn_no_params:
                mJNIApi.testCallNoParamMethod();
                break;
            case R.id.btn_params:
                mJNIApi.testCallParamMethod();
                break;
            case R.id.btn_static_method:
                mJNIApi.testCallStaticMethod();
                break;
            case R.id.btn_custom_class:
                showLog("执行结果:" + mJNIApi.testCallConstructorMethod());
                break;
            case R.id.btn_return:
                String display = mJNIApi.getDisplayName("xuexiang", 24);
                showLog(display);
                break;

            case R.id.btn_get_random_array:
                array = mJNIApi.getRandomIntArray(10);
                showLog("随机生成的数组:" + Arrays.toString(array));
                break;
            case R.id.btn_sort_array:
                if (array == null) {
                    array = mJNIApi.getRandomIntArray(10);
                }
                mJNIApi.sort(array);
                showLog("排序后的数组:" + Arrays.toString(array));
                break;
            case R.id.btn_jni_reference:
                mJNIApi.setJNIReference("我是薛翔，我选Android!");
                break;
            case R.id.btn_get_jni_reference:
                showLog("获取全局变量："+ mJNIApi.getJNIReference());
                break;
            case R.id.btn_release_jni_reference:
                mJNIApi.releaseJNIReference();
                break;
            case R.id.btn_throw_exception:
                try {
                    mJNIApi.throwException();
                } catch (Exception e) {
                    e.printStackTrace();
                    showLog(e.getMessage());
                }
                break;
            case R.id.btn_jni_try_catch_exception:
                try {
                    mJNIApi.jniTryCatchException();
                } catch (Exception e) {
                    e.printStackTrace();
                    showLog(e.getMessage());
                }
                break;
            default:
                break;
        }
    }

    private void showLog(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText(message);
            }
        });
    }


    public interface OnShowLogListener {

        void onShowLog(String msg);
    }

    @Override
    protected void onDestroy() {
        mJNIApi.clear();
        super.onDestroy();
    }
}
