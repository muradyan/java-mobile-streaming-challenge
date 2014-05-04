package com.soundcloud.challenge.streamer;

import android.widget.TextView;
import android.util.Log;

public class NotificationPresenter implements Runnable {

    private TextView textView;
    private int currentNotificationLineNumber = 0;
    private String message;
    private int eventType;

    public void setMessage(String m) {
        message = m;
    }

    public void setEventType(int e) {
        eventType = e;
    }

    NotificationPresenter(TextView t) {
        textView = t;
    }

    @Override
    public void run() {
        assert(textView != null);
        String line = Integer.toString(currentNotificationLineNumber);
        switch (eventType) {
            case 0: { 
                textView.append(line + ": UserEvent: " + message + "\n");
                break;
            } case 1: {
                textView.append(line + ": SystemEvent: " + message + "\n");
                break;
            } case 2: {
                textView.append(line + ": LoggingEvent: " + message + "\n");
                break;
            } default: {
                Log.d("LauncherActivity", "not supported event type");
            }
        }
        ++currentNotificationLineNumber;
    }
}
