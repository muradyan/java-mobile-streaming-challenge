
#include <jni.h>
#include <android/log.h>

#include <cstdlib>
#include <cassert>
#include <ctime>

#include <thread>
#include <mutex>
#include <condition_variable>

#define LOG_TAG "EventGeneratorJNI"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/// the callback from Java to call for triggering events
static jmethodID _event_callback;

/// Java VM 
static JavaVM* _java_vm = 0;

/// indicates whether event generator is stopped or not
static bool _stopped;

/// synchronization primitives to block the thread 
static std::mutex _mutex;
static std::condition_variable _condition_variable;

/// period for triggering event in milliseconds 
static int _period = 500;

/**
 blocks the thread until either _period elapses or _stopped becomes true
 returns true if _stopped became true
 */
static bool wait_until()
{
    std::unique_lock<std::mutex> lck(_mutex);
    auto a = std::chrono::duration<int,std::milli>(_period);
    return _condition_variable.wait_for(lck, a, []{return _stopped;});
}

static void generate_random_events(JNIEnv* env, jobject thiz)
{
    assert(nullptr != env);
    do {
        jstring msg = env->NewStringUTF("Event yeah");
        env->CallVoidMethod(thiz, _event_callback, rand() % 3, msg);
    } while (!wait_until());
}

/**
 for local vs. global references 
 @see http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/design.html#wp1242
 for more info about attaching & detaching to the thread
 @see http://developer.android.com/training/articles/perf-jni.html
*/
extern "C" JNIEXPORT void native_start(JNIEnv* env, jobject thiz) 
{
    LOGI("native_start - begin");
    _stopped = false;
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
        generate_random_events(e, obj);
        e->DeleteGlobalRef(obj);
        _java_vm->DetachCurrentThread();
        LOGI("native_start - returning from the thread");
    }).detach();
    LOGI("native_start - end");
}

extern "C" JNIEXPORT void native_stop(JNIEnv* env, jobject thiz) 
{
    std::unique_lock<std::mutex> lck(_mutex);
    _stopped = true;
    _condition_variable.notify_one();
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
    return JNI_VERSION_1_4;
}
