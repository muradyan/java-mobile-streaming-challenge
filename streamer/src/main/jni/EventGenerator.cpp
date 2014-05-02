
//#include "Timer.h"

#include <android/log.h>
#include <jni.h>

static JavaVM *java_vm;
static jmethodID event_callback;

extern "C" JNIEXPORT void native_start(JNIEnv* env, jobject thiz) 
{
    jstring msg = env->NewStringUTF("Event yeah");
    env->CallVoidMethod(thiz, event_callback, 0, msg);
    env->CallVoidMethod(thiz, event_callback, 1, msg);
    env->CallVoidMethod(thiz, event_callback, 2, msg);
}

extern "C" JNIEXPORT void native_stop(JNIEnv* env, jobject thiz) 
{

}

extern "C" JNIEXPORT jstring native_get_info(JNIEnv * env, jobject thiz) 
{
    return env->NewStringUTF("My Info");
}

/* List of implemented native methods */
static JNINativeMethod native_methods[] = {
  {"stopEvents", "()V", (void *) native_stop},
  {"startEvents", "()V", (void *) native_start},
  {"getInfo", "()Ljava/lang/String;", (void *) native_get_info},
};

/* Library initializer */
jint JNI_OnLoad (JavaVM* vm, void* reserved)
{
  JNIEnv* env = 0;
  java_vm = vm;
  if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    __android_log_print (ANDROID_LOG_ERROR, "Streamer",
        "Failed to get JNIEnv for JNI 1.4+");
    return 0;
  }
  jclass klass = env->FindClass("com/soundcloud/challenge/streamer/Streamer");
  event_callback = env->GetMethodID(klass, "onEvent", "(ILjava/lang/String;)V");
  env->RegisterNatives(klass, native_methods, 3);
  return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
}
