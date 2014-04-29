package com.soundcloud.challenge.streamer;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;


public class LauncherActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.launcher_activity_layout);
    }

    @Override
    protected void onStart() {
        super.onStart();
        Streamer streamer = new Streamer();
        ((TextView) findViewById(R.id.test)).setText(streamer.getInfo());
    }
}
