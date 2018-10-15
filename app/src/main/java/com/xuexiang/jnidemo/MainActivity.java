package com.xuexiang.jnidemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    private static final String TAG = "xuexiang";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    public void noParamMethod() {
        Log.e(TAG, "无参的Java方法被调用了");
    }

    public void paramMethod(int number) {
        Log.e(TAG, "有参的Java方法被调用了" + number + "次");
    }

    public static void staticMethod() {
        Log.e(TAG, "静态的Java方法被调用了");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

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

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.btn_no_params:
                testCallNoParamMethod();
                break;
            case R.id.btn_params:
                testCallParamMethod();
                break;
            case R.id.btn_static_method:
                testCallStaticMethod();
                break;
            default:
                break;
        }
    }
}
