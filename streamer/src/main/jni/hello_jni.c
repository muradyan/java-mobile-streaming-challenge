#include <android/log.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL 0

static JavaVM *java_vm;
static jmethodID event_callback;

static void native_start(JNIEnv* env, jobject thiz) 
{
    jstring msg = (*env)->NewStringUTF(env, "Event yeah");
    (*env)->CallVoidMethod (env, thiz, event_callback, 0, msg);
    (*env)->CallVoidMethod (env, thiz, event_callback, 1, msg);
    (*env)->CallVoidMethod (env, thiz, event_callback, 2, msg);
}

static void native_stop(JNIEnv* env, jobject thiz) 
{

}

static jstring native_get_info(JNIEnv * env, jobject thiz) 
{
    return (*env)->NewStringUTF(env, "My Info");
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
  JNIEnv* env = NULL;
  java_vm = vm;
  if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    __android_log_print (ANDROID_LOG_ERROR, "Streamer",
        "Failed to get JNIEnv for JNI 1.4+");
    return 0;
  }
  jclass klass = (*env)->FindClass (env,
            "com/soundcloud/challenge/streamer/Streamer");
  event_callback = (*env)->GetMethodID (env, klass, "onEvent", 
            "(ILjava/lang/String;)V");
  (*env)->RegisterNatives (env, klass, native_methods, 3);
  return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
}

#ifdef __cplusplus
}
#endif

