#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <ImageUtils/PostProcess.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>

#include "amcomdef.h"
#include "amdisplay.h"
#include "ammem.h"
#include "merror.h"
#include "LogUtil.h"

#include <ImageUtils/Utils_MBitmapAndroidBitmap.h>

#include "LogUtil.h"
#include "merror.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "PostProcess.h"

#define NV21_DIR_PATH    "/sdcard/nv21result/"

#define CHK_RES(mResult) if(mResult) goto EXIT


/*Define the resolution format space*/
typedef struct __tag_FILE_OFFSCREEN
{
    MInt32	i32Width;
    MInt32	i32Height;
    MInt32	pi32Pitch;
}OFFSCREENBYNAME, *LPOFFSCREENBYNAME;

/**
 * convert to nv21
 * */
LPMBITMAP GetArcSoftBitmap(JNIEnv *env, jobject jbitmap) {
    if (MNull == jbitmap || MNull == env) {
        LOGI("GetAndroidBitmap in  env / jbitmap is null");
        return MNull;
    }
    LOGI("GetArcSoftBitmap in:%p", jbitmap);
    AndroidBitmapInfo bitmapInfo = {0};
    MVoid *lpBitmapAddr = MNull;

    LPMBITMAP pMBitmap = MNull;
    MRESULT res = 0;
    MBool lockPixelsSucc = 0;
    LOGD("AndroidBitmap_getInfo");
    res = AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo);
    if (res != 0) {
        LOGI("GetArcSoftBitmap AndroidBitmap_getInfo res:%ld", res);
        return MNull;
    }
    LOGI("AndroidBitmap_getInfo res:%d", res);
    MUInt32 dwFormat = 0;
    switch (bitmapInfo.format) {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            dwFormat = MPAF_RGB32_B8G8R8A8;
            break;
        case ANDROID_BITMAP_FORMAT_RGB_565:
            dwFormat = MPAF_RGB16_R5G6B5;
            break;

    }

    if (dwFormat <= 0) {
        return MNull;
    }

    res = AndroidBitmap_lockPixels(env, jbitmap, &lpBitmapAddr);
    lockPixelsSucc = (res == 0);
    LOGI("GetArcSoftBitmap AndroidBitmap_lockPixels res:%ld", res);
    CHK_RES(res);


    pMBitmap = (LPMBITMAP) MMemAlloc(MNull, sizeof(MBITMAP));
    if (MNull == pMBitmap) {
        LOGI("GetArcSoftBitmap MMemAlloc no memery");
        res = MERR_NO_MEMORY;
        CHK_RES(res);
    }

    pMBitmap->dwPixelArrayFormat = dwFormat;
    pMBitmap->lWidth = bitmapInfo.width;
    pMBitmap->lHeight = bitmapInfo.height;

    res = MBitmapAlloc(*pMBitmap);
    CHK_RES(res);

    LOGI("GetArcSoftBitmap MMemCpy to pAddress");
    MMemCpy(pMBitmap->pPlane[0], lpBitmapAddr,
            pMBitmap->lPitch[0] * pMBitmap->lHeight);


    EXIT:
    if (lockPixelsSucc) {
        AndroidBitmap_unlockPixels(env, jbitmap);
    }

    if (res) {
        if (pMBitmap) {
            MBitmapFree(*pMBitmap);
            MMemFree(MNull, pMBitmap);
            pMBitmap = MNull;
        }
    }

    LOGI("GetArcSoftBitmap out");
    return pMBitmap;
}


void dumpYUVtoFile(ASVLOFFSCREEN *pAsvl, const char *name) {
    FILE *file_fd = fopen(name, "w");
    if (file_fd) {
        LOGD("dumpYUVtoFile: open success.");
        //fchmod(file_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        ssize_t writen_bytes = 0;
        writen_bytes = fwrite(pAsvl->ppu8Plane[0], pAsvl->pi32Pitch[0] * pAsvl->i32Height, 1,
                              file_fd);//only for NV21 or NV12
        writen_bytes = fwrite(pAsvl->ppu8Plane[1], pAsvl->pi32Pitch[1] * (pAsvl->i32Height >> 1), 1,
                             file_fd);
//        writen_bytes = write(file_fd, pAsvl->ppu8Plane[1], pAsvl->pi32Pitch[1] * (pAsvl->i32Height >> 1));//only for NV21 or NV12
        fclose(file_fd);
    } else {
        LOGD("dumpYUVtoFile: open failed.");
    }
}


extern "C" JNIEXPORT jint

JNICALL
Java_com_arcsoft_bitmaptonv21_MainActivity_generateNv21(
        JNIEnv *env, jobject  obj,
        jstring name,
        jobject bitmap) {

    LPMBITMAP pBitmap;//;
    pBitmap = GetArcSoftBitmap(env, bitmap);

    CPostProcess pp;

    ASVLOFFSCREEN offscreenResult = {0};
    offscreenResult.i32Width = pBitmap->lWidth;
    offscreenResult.i32Height = pBitmap->lHeight;
    offscreenResult.u32PixelArrayFormat = ASVL_PAF_NV21;
    offscreenResult.pi32Pitch[0] = pBitmap->lWidth;
    offscreenResult.pi32Pitch[1] = pBitmap->lWidth;
    offscreenResult.ppu8Plane[0] = (unsigned char *) malloc(
            offscreenResult.pi32Pitch[0] * offscreenResult.i32Height * 3 / 2);
    offscreenResult.ppu8Plane[1] = offscreenResult.ppu8Plane[0] + offscreenResult.pi32Pitch[0] * offscreenResult.i32Height;

    int ret = pp.DoPostProcess(pBitmap, &offscreenResult);
    if(ret){
        LOGE("DoPostProcess failed, ret = %d", ret);
        return ret;
    }

    // 从 instring 字符串取得指向字符串 UTF 编码的指针
    jboolean isCopy;
    const char* fileName = (char* )env->GetStringUTFChars(name, &isCopy);
    if(fileName == NULL) {
        return -1; /* OutOfMemoryError already thrown */
    }


    //转换成JNI格式字符串返回
    /*
    const char* tmpstr = "return string success!";
    jstring rtstr = env->NewStringUTF(tmpstr);
    return reinterpret_cast<jint>(rtstr);
    */

    char nv21Filename [80] = {0};
    snprintf(nv21Filename, sizeof(nv21Filename), "%s%s_%d_%dx%d.nv21", NV21_DIR_PATH, fileName,
             offscreenResult.i32Width,
             offscreenResult.pi32Pitch[0], offscreenResult.i32Height);
    mkdir(NV21_DIR_PATH, 0777);

    dumpYUVtoFile(&offscreenResult, nv21Filename);
    LOGD("dumpYUVtoFile finished: file: %s", nv21Filename);

    // 通知虚拟机本地代码不再需要通过 str 访问 Java 字符串。
    env->ReleaseStringUTFChars(name, fileName);

    free(offscreenResult.ppu8Plane[0]);
    if (pBitmap) {
        MBitmapFree(*pBitmap);
        MMemFree(MNull, pBitmap);
        pBitmap = MNull;
    }

    return 0;
}

/**
 * read nv21
 * */

void dumpRGBtoFile(ASVLOFFSCREEN *pAsvl, const char *name) {
    FILE *file_fd = fopen(name, "w");
    if (file_fd) {
        LOGD("dumpRGBtoFile: open success.");
        //fchmod(file_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        ssize_t writen_bytes = 0;
        writen_bytes = fwrite(pAsvl->ppu8Plane[0], pAsvl->pi32Pitch[0] * pAsvl->i32Height, 1,
                              file_fd);//only for NV21 or NV12
//        writen_bytes = write(file_fd, pAsvl->ppu8Plane[1], pAsvl->pi32Pitch[1] * (pAsvl->i32Height >> 1));//only for NV21 or NV12
        fclose(file_fd);
    } else {
        LOGD("dumpRGBtoFile: open failed.");
    }
}

/*
void dumpYUVtoFile(ASVLOFFSCREEN *pAsvl, const char *name) {
    int file_fd = open(name, O_RDWR | O_CREAT, 0777);
    if (file_fd > 0) {
        //fchmod(file_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        ssize_t writen_bytes = 0;
        writen_bytes = write(file_fd, pAsvl->ppu8Plane[0],
                             pAsvl->pi32Pitch[0] * pAsvl->i32Height);//only for NV21 or NV12
        writen_bytes = write(file_fd, pAsvl->ppu8Plane[1],
                             pAsvl->pi32Pitch[1] * (pAsvl->i32Height >> 1));//only for NV21 or NV12
        close(file_fd);
    }
}*/

// 内部实现函数：从特定文件名格式提取分辨率
void extractOffscreenByName(OFFSCREENBYNAME *pOffscreen, const char *name) {
    //name : "test_1440_1472x1080"
    std::string str(name);
    uint64_t _Pos, _Pos2, xPos;

    xPos = str.rfind('x', strlen(str.c_str())); //从末尾反向查找"x"的位置
    std::string strHeight(str, xPos+1, strlen(str.c_str())-xPos); //取出'x'之后的Height
    pOffscreen->i32Height = atoi(strHeight.c_str()); //将字符串转换成整型
    LOGD("Get xPos = %d, strHeight = %s", xPos, strHeight.c_str());

    _Pos = str.rfind('_', xPos-1); //从"x"前一位开始查找'_'的位置
    std::string strPitch(str, _Pos+1, xPos-_Pos-1); //取出'_'和'x'之间的 pitch
    pOffscreen->pi32Pitch = atoi(strPitch.c_str()); //将字符串转换成整型
    LOGD("Get _Pos = %d, strPitch = %s", _Pos, strPitch.c_str());

    _Pos2 = str.rfind('_', _Pos-1); //从之前找到的"_"查找下一个'_'的位置
    std::string strWidth(str, _Pos2+1, _Pos2==-1 ? _Pos : _Pos-_Pos2-1); //取出倒数第二个'_'和倒数第一个'_'之间的 width
    pOffscreen->i32Width = atoi(strWidth.c_str()); //将字符串转换成整型
    LOGD("Get _Pos2 = %d, strWidth = %s", _Pos2, strWidth.c_str());
}

extern "C" JNIEXPORT jobject

JNICALL
Java_com_arcsoft_bitmaptonv21_MainActivity_objectFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring name) {
    std::string hello = "Hello from C++";

    // 从 instring 字符串取得指向字符串 UTF 编码的指针
    jboolean isCopy;
    const char* fileName = (char* )env->GetStringUTFChars(name, &isCopy);
    if(fileName == NULL) {
        LOGE("OutOfMemoryError already thrown!");
    }

    // 从 nv21 文件名中提取出分辨率大小
    OFFSCREENBYNAME offScreenByName = {0};
    extractOffscreenByName(&offScreenByName, fileName);

    ASVLOFFSCREEN offscreen = {0};
    offscreen.i32Width = offScreenByName.i32Width;
    offscreen.i32Height = offScreenByName.i32Height;
    offscreen.u32PixelArrayFormat = ASVL_PAF_NV21;
    offscreen.pi32Pitch[0] = offScreenByName.pi32Pitch;
    offscreen.pi32Pitch[1] = offScreenByName.pi32Pitch;
    offscreen.ppu8Plane[0] = (unsigned char *) malloc(
            offscreen.pi32Pitch[0] * offscreen.i32Height * 3 / 2);
    offscreen.ppu8Plane[1] = offscreen.ppu8Plane[0] + offscreen.pi32Pitch[0] * offscreen.i32Height;

    char nv21FilePath [80] = {0};
    snprintf(nv21FilePath, sizeof(nv21FilePath), "%s%s.nv21", NV21_DIR_PATH, fileName);
    LOGI("%s nv21FilePath = %s", __func__, nv21FilePath);

    FILE *fp = MNull;
    fp = fopen(nv21FilePath, "r");
    LOGD("%s fp = %p", __func__, fp);
    long count = 1;
    int seekRet = fseek(fp, 0, SEEK_SET);
    long readCount = fread(offscreen.ppu8Plane[0],
                           (offscreen.pi32Pitch[0] * offscreen.i32Height * 3 / 2), count, fp);
    if (readCount != count) {
        LOGD("fread error, readCount != count.");
        ferror(fp);
        fclose(fp);
        return NULL;
    } else {
        LOGD("fread success");
        fclose(fp);
    }

    ASVLOFFSCREEN offscreenResult = {0};
    offscreenResult.i32Width = offscreen.i32Width;
    offscreenResult.i32Height = offscreen.i32Height;
    offscreenResult.u32PixelArrayFormat = ASVL_PAF_RGB32_B8G8R8A8;
    offscreenResult.pi32Pitch[0] = offscreen.i32Width * 4;
    offscreenResult.ppu8Plane[0] = (unsigned char *) malloc(
            offscreenResult.pi32Pitch[0] * offscreenResult.i32Height);
    LOGD("Ready to DoPostProcess ");
    CPostProcess pp;
    int ret = pp.DoPostProcess(&offscreen, &offscreenResult);
    LOGD("DoPostProcess result = %d", ret);

    char rgbFilePath [80] = {0};
    snprintf(rgbFilePath, sizeof(rgbFilePath), "%s%s.rgb", NV21_DIR_PATH, fileName);

    dumpRGBtoFile(&offscreenResult, rgbFilePath);
    LOGD("ArcSoft_JNI dumpRGBtoFile : %s",rgbFilePath);

    MBITMAP rgbBitmap = {0};
    ret = pp.BuildMBITMAP(&offscreenResult, &rgbBitmap);
    LOGD("pp.BuildMBITMAP result = %d", ret);

//    free(offscreenResult.ppu8Plane[0]);
    free(offscreen.ppu8Plane[0]);

    // 通知虚拟机本地代码不再需要通过 str 访问 Java 字符串。
    env->ReleaseStringUTFChars(name, fileName);

    return _GetBitmapFromMBitmap(env, (MBITMAP *) &rgbBitmap);
}


