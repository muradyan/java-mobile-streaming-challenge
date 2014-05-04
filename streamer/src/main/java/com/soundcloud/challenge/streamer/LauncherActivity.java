package com.soundcloud.challenge.streamer;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;

public class LauncherActivity extends Activity implements Observer {

    private Streamer streamer;
    private TextView textView;
    private int currentNotificationLineNumber = 0;

    @Override
    public void notify(int eventType, String msg)
    {
        assert(textView != null);
        String line = Integer.toString(currentNotificationLineNumber);
        switch (eventType) {
            case 0: { 
                textView.append(line + ": UserEvent: " + msg + "\n");
                break;
            } case 1: {
                textView.append(line + ": SystemEvent: " + msg + "\n");
                break;
            } case 2: {
                textView.append(line + ": LoggingEvent: " + msg + "\n");
                break;
            } default: {
                Log.d("LauncherActivity", "not supported event type");
            }
        }
        ++currentNotificationLineNumber;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.launcher_activity_layout);
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.i("LauncherActivity", "Starting the Activity");
        streamer = new Streamer(this);
        textView = ((TextView) findViewById(R.id.test));
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i("LauncherActivity", "Pausing the Activity");
        assert(null != streamer);
        streamer.stopEvents();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i("LauncherActivity", "Resuming the Activity");
        assert(null != textView);
        assert(null != streamer);
        textView.setText(streamer.getInfo() + "\n\n");
        streamer.startEvents();
    }

}
