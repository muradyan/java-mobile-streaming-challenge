package com.soundcloud.challenge.streamer;

public interface Observer {

    public abstract void notify(int eventType, String msg);

}
