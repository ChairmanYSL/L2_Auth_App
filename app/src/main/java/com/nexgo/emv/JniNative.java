package com.nexgo.emv;

/**
 * Created by mai on 2016/5/19.
 */
public class JniNative {

    static{
        System.loadLibrary("sdkemvtest");
    }
    public static native int appmain();
    public static native int appoutprocess();

}
