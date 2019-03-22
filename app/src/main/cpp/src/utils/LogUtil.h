#ifndef _____LOG_UTILS___H___
#define _____LOG_UTILS___H___

#include <stdio.h>
#include <time.h>

#include <android/log.h>

#define LOG_TAG "ArcSoft_bitmapToNV21"

#define DEBUG

#ifdef DEBUG
#define ARCSOFT_BENCH_MARK

#define  LOGI(...)                  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)                  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)                  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGV(...)                  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)                  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#define FUNCTION_ENTER              LOGI("%s (in)", __FUNCTION__);
#define FUNCTION_QUIT               LOGI("%s (out)", __FUNCTION__);
#else
#define LOGD
#define LOGI
#define LOGW
#define LOGE
#define LOGF

#define FUNCTION_ENTER
#define FUNCTION_QUIT
#endif

#define LOG_VERSION(...)           __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// used for performance
inline long long gettime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}


// get the performance
#ifdef DEBUG
#define BEGIN_TIME {\
        long t0 = gettime();
#define END_TIME(FUN) \
        LOGD("%s cost time = %d ms", FUN, gettime()-t0);}
#else
#define BEGIN_TIME
#define END_TIME(FUN)
#endif    //end of JNI_TIME

#endif//_____LOG_UTILS___H___

