#ifndef __UTILS_MBITMAPANDROIDBITMAP___H__
#define __UTILS_MBITMAPANDROIDBITMAP___H__
#include <jni.h>
#include "amdisplay.h"
jobject _GetBitmapFromMBitmap(JNIEnv *env, MBITMAP *pMBitmap);
jint ReturnBitmap(JNIEnv* env, jobject returnBitmap, MBITMAP& resBitmap);

#endif //__UTILS_MBITMAPANDROIDBITMAP___H__
