#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <utils/LogUtil.h>

#include "Utils_MBitmapAndroidBitmap.h"
#include "amdisplay.h"
#include "ammem.h"
//#include "ArcJNILog.h"

jobject _GetBitmapFromMBitmap(JNIEnv *env, MBITMAP *pMBitmap)
{
	LOGI("_GetBitmapFromMBitmap in ");
	if(MNull == pMBitmap || MNull == pMBitmap->pPlane[0]){
		LOGI("_GetBitmapFromMBitmap in  pMBitmap is null or empty");
		return MNull;
	}

	int nRet = 0;
	AndroidBitmapInfo bitmapInfo;
	MVoid* pAddress = MNull;

	jobject jbitmap = MNull;
	jobject obj_config = MNull;


	const char* pCreateBitmapSig = "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;";
	const char* pConfigSig = "Landroid/graphics/Bitmap$Config;";

	jclass clazzBitmap = env->FindClass("android/graphics/Bitmap");
//	jclass clazzBitmap = env->FindClass("java/com.arcsoft.readnv21/MainActivity");
	jclass clazzConfig = env->FindClass("android/graphics/Bitmap$Config");

	jmethodID method_createBitmap = MNull;
	jfieldID  fieldID_config = MNull;

	MLong lWidth = pMBitmap->lWidth;
	MLong lHeight = pMBitmap->lHeight;

	int color_format = pMBitmap->dwPixelArrayFormat;

	LOGI(" _GetBitmapFromMBitmap MPAF_GRAY8: %d ",MPAF_GRAY8);
	LOGI(" _GetBitmapFromMBitmap MPAF_RGB16_R5G6B5: %d ",MPAF_RGB16_R5G6B5);
	LOGI(" _GetBitmapFromMBitmap MPAF_RGB32_A8R8G8B8: %d ",MPAF_RGB32_A8R8G8B8);
	LOGI(" _GetBitmapFromMBitmap color_format: %d,lWidth: %d,lHeight: %d ",color_format,lWidth,lHeight);
	switch (color_format)
	{
	case MPAF_GRAY8:
		fieldID_config = env->GetStaticFieldID(clazzConfig, "ALPHA_8", pConfigSig);
		break;
	case MPAF_RGB16_R5G6B5:
		fieldID_config = env->GetStaticFieldID(clazzConfig, "RGB_565", pConfigSig);
		break;
	case MPAF_RGB32_A8R8G8B8:
	case MPAF_RGB32_B8G8R8A8:
		fieldID_config = env->GetStaticFieldID(clazzConfig, "ARGB_8888", pConfigSig);
		break;
	default:
		goto EXIT;
		break;
	}

	obj_config = env->GetStaticObjectField(clazzConfig, fieldID_config);

	LOGI("_GetBitmapFromMBitmap before createBitmap ");
	method_createBitmap = env->GetStaticMethodID(clazzBitmap, "createBitmap",pCreateBitmapSig);
	jbitmap = env->CallStaticObjectMethod(clazzBitmap, method_createBitmap,lWidth, lHeight, obj_config);
	LOGI("_GetBitmapFromMBitmap after createBitmap");

	if(MNull == jbitmap){
		LOGI("createBitmap faild...");
		goto EXIT;
	}

	nRet = AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo);
	if (nRet != 0)
	{
		LOGI("AndroidBitmap_getInfo nRet:%d",nRet);
		goto EXIT;
	}

	nRet = AndroidBitmap_lockPixels(env, jbitmap, &pAddress);
	if (nRet != 0)
	{
		LOGI("AndroidBitmap_lockPixels nRet:%d",nRet);
		goto EXIT;
	}


	if(color_format == MPAF_RGB32_A8R8G8B8)
	{
		MLong lRowBytes = pMBitmap->lPitch[0];
	    MByte* pData = pMBitmap->pPlane[0];
	    for (MDWord i = 0; i < lRowBytes * lHeight; i += 4)
	    {
	    	((MByte *)pAddress)[i] = ((MByte *)pData)[i+2];
	        ((MByte *)pAddress)[i+1] = ((MByte *)pData)[i+1];
	        ((MByte *)pAddress)[i+2] = ((MByte *)pData)[i];
	        ((MByte *)pAddress)[i+3] = ((MByte *)pData)[i+3];
	    }
	}
	else
	{
		LOGI("MMemCpy to pAddress");
		MMemCpy(pAddress, pMBitmap->pPlane[0],
				pMBitmap->lPitch[0] * pMBitmap->lHeight);
	}

	AndroidBitmap_unlockPixels(env, jbitmap);

EXIT:
	if (MNull != clazzBitmap)
	{
		env->DeleteLocalRef(clazzBitmap);
	}

	if (MNull != clazzConfig)
	{
		env->DeleteLocalRef(clazzConfig);
	}
	if (MNull != obj_config)
	{
		env->DeleteLocalRef(obj_config);
	}


	LOGI("return jbitmap: %p",jbitmap);
	return jbitmap;
}

jint ReturnBitmap(JNIEnv* env, jobject returnBitmap, MBITMAP& resBitmap)
{
    LOGI("ReturnBitmap in");
    const char* bitmapSigture = "Landroid/graphics/Bitmap;";
    jclass retClazz = env->GetObjectClass(returnBitmap);
    jfieldID mBitmap = env->GetFieldID(retClazz, "mBitmap", bitmapSigture);
    jobject bitmap = _GetBitmapFromMBitmap(env, &resBitmap);

    env->SetObjectField(returnBitmap, mBitmap, bitmap);
    env->DeleteLocalRef(retClazz);
    LOGI("ReturnBitmap out");
    return 0;
}