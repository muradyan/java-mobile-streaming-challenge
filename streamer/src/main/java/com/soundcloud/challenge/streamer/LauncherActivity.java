package com.soundcloud.challenge.streamer;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;

/**
  Another solution would be to setup a Timer (or Handler) instead.  From JNI
  the messages would be accumulated in a queue and timer would read and display
  on the UI.  But this would bring the overhead of the timer, synchronization
  when reading/writing form the queue.

  Depending on the frequency, size of the message the importance how fast they
  should be displayed to the user, multiple messages could be grouped to reduce
  the number of calls from/to JNI.
 */
public class LauncherActivity extends Activity implements Observer {

    private NotificationPresenter notificationPresenter;
    private Streamer streamer;
    private TextView textView;

    @Override
    public void notify(int eventType, String msg)
    {
        notificationPresenter.setEventType(eventType);
        notificationPresenter.setMessage(msg);
        runOnUiThread(notificationPresenter);
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
        notificationPresenter = new NotificationPresenter(textView);
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
