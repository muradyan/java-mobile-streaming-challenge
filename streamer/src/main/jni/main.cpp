
#include "event_generator.h"

#include <jni.h>
#include <android/log.h>

#include <cstdlib>
#include <cassert>

#include <thread>
#include <functional>

#define LOG_TAG "EventGeneratorJNI"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/// the callback from Java to call for triggering events
static jmethodID _event_callback;

/// Java VM 
static JavaVM* _java_vm = 0;

static event_generator engine;

/**
 for local vs. global references 
 @see http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/design.html#wp1242
 for more info about attaching & detaching to the thread
 @see http://developer.android.com/training/articles/perf-jni.html
*/
extern "C" JNIEXPORT void native_start(JNIEnv* env, jobject thiz) 
{
    LOGI("native_start - begin");
    engine.enable();
    // need to get the global reference to share between threads
    jobject obj = env->NewGlobalRef(thiz);
    std::thread([obj]() {
        LOGI("native_start - beginning of the thread");
        JNIEnv* e = nullptr;
        assert(nullptr != _java_vm);
        // getting the env since it's not shareable between threads
        int s1 = _java_vm->GetEnv((void**) &e, JNI_VERSION_1_4);
        if (s1 == JNI_EDETACHED) {
            int s2 = _java_vm->AttachCurrentThread((JNIEnv**) &e, nullptr);
            LOGI(s2 != 0 ? "thread failed to attach" : 
                           "thread successfully attached");
        } 
        engine.run([e, obj] () {
            jstring msg = e->NewStringUTF("Event yeah");
            e->CallVoidMethod(obj, _event_callback, rand() % 3, msg);
        });
        e->DeleteGlobalRef(obj);
        _java_vm->DetachCurrentThread();
        LOGI("native_start - returning from the thread");
    }).detach();
    LOGI("native_start - end");
}

extern "C" JNIEXPORT void native_stop(JNIEnv* env, jobject thiz) 
{
    engine.disable();
}

extern "C" JNIEXPORT jstring native_get_info(JNIEnv* env, jobject thiz) 
{
    return env->NewStringUTF("My Info");
}

/** List of implemented native methods */
static JNINativeMethod native_methods[] = {
    {"stopEvents", "()V", (void*) native_stop},
    {"startEvents", "()V", (void*) native_start},
    {"getInfo", "()Ljava/lang/String;", (void*) native_get_info},
};

/** Library initializer */
jint JNI_OnLoad (JavaVM* vm, void* reserved)
{
    JNIEnv* env = 0;
    _java_vm = vm;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("Failed to get JNIEnv for JNI 1.4+");
        return 0;
    }
    jclass klass = env->FindClass("com/soundcloud/challenge/streamer/Streamer");
    _event_callback = env->GetMethodID(klass, "onEvent", 
        "(ILjava/lang/String;)V");
    env->RegisterNatives(klass, native_methods, 3);
    srand(time(nullptr));
    engine.set_period_milli(500);
    return JNI_VERSION_1_4;
}
