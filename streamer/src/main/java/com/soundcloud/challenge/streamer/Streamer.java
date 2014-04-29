package com.soundcloud.challenge.streamer;

import android.util.Log;

public class Streamer {
    static {
        System.loadLibrary("streamer");
    }


    public native void startEvents();
    public native void stopEvents();
    public native String getInfo();

    public void onEvent(int eventType, String msg){
        Log.i("Streamer", "Type:" + eventType + " | " + msg);
    }

}
