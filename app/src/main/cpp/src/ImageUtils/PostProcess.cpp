/*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and
* confidential information.
*
* The information and code contained in this file is only for authorized ArcSoft employees
* to design, create, modify, or review.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy, distribute, modify,
* or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/
/*
* PostProcess.cpp
*
* Reference:
*
* Description:
*
*/

#include "ammem.h"
#include "merror.h"
#include "PostProcess.h"
#include "LogUtil.h"

#define ARRAY_SIZE(Array) (sizeof(Array)/sizeof(Array[0]))
#define LINE_BYTES(Width, BitCt)    (((long)(Width) * (BitCt) + 31) / 32 * 4)
#define ALIGN_SIZE(Width, Byte)  (((Width) + (Byte) -1) & ~((Byte) -1))
#define GET_BUFFER_SIZE() \
(bitmap.lPitch[0] * lBufferH + bitmap.lPitch[1] * (lBufferH/lSubSampleV) + bitmap.lPitch[2] * (lBufferH/lSubSampleV))

#define AGET_ALIGN_SIZE_16(size) (((size) + 15) & ~(15))



MRESULT MBitmapAlloc(MBITMAP& bitmap)
{
    MLong   lSize = 0;
    MLong   lBufferH = 0;
    MLong   lSubSampleV = 1;
    MByte*  pData = MNull;
    MMemSet(bitmap.lPitch, 0, MPAF_MAX_PLANES * sizeof(MInt32));
    MMemSet(bitmap.pPlane, MNull, MPAF_MAX_PLANES * sizeof(MByte*));

    switch(bitmap.dwPixelArrayFormat)
    {
    case MPAF_RGB32_B8G8R8A8:
    case MPAF_RGB32_A8R8G8B8:
    case MPAF_RGB32_R8G8B8:
    case MPAF_RGB32_B8G8R8:
    case MPAF_RGB32_A8B8G8R8:
    case MPAF_RGB32_R8G8B8A8:
        bitmap.lPitch[0] = bitmap.lWidth * 4;//LINE_BYTES(bitmap.lWidth, 32);
        lBufferH = bitmap.lHeight;
        lSize = MBitmapSize(bitmap);
        LOGI("MBitmapSize 32bit lSize = %ld" , lSize);
        break;

    case MPAF_RGB24_R8G8B8:
    case MPAF_RGB24_B8G8R8:
        bitmap.lPitch[0] = bitmap.lWidth * 3;//LINE_BYTES(bitmap.lWidth, 24);
        lBufferH = bitmap.lHeight;
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_RGB16_B5G6R5:
    case MPAF_RGB16_R5G6B5:
        bitmap.lPitch[0] = bitmap.lWidth * 2;//LINE_BYTES(bitmap.lWidth, 16);
        lBufferH = bitmap.lHeight;
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_YUYV:
    case MPAF_YVYU:
    case MPAF_UYVY:
    case MPAF_VYUY:
        bitmap.lPitch[0] = bitmap.lWidth * 2;//LINE_BYTES(bitmap.lWidth, 16);
        lBufferH = bitmap.lHeight;
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_I420:
    case MPAF_YV12:
        bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
        bitmap.lPitch[1] = bitmap.lPitch[0]>>1;
        bitmap.lPitch[2] = bitmap.lPitch[0]>>1;
        lBufferH = bitmap.lHeight;
        lSubSampleV = 2;
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_OTHERS_NV21:
    case MPAF_OTHERS_NV12:
    case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
    	{
    		bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
			bitmap.lPitch[1] = bitmap.lPitch[0];
			bitmap.lPitch[2] = bitmap.lPitch[0];
			lBufferH = (bitmap.lHeight + 1) >> 1 <<1;
			lSubSampleV = 2;
			lSize = bitmap.lPitch[0] * lBufferH + bitmap.lPitch[1] * (lBufferH/lSubSampleV);
			LOGI("MBitmapSize lSize = %ld" , lSize);
        }
        break;

    case MPAF_I422V:
        bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
        bitmap.lPitch[1] = bitmap.lPitch[0];
        bitmap.lPitch[2] = bitmap.lPitch[0];
        lBufferH = bitmap.lHeight;
        lSubSampleV = 2;
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_I422H:
        bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
        bitmap.lPitch[1] = bitmap.lPitch[0]>>1;
        bitmap.lPitch[2] = bitmap.lPitch[0]>>1;
        lBufferH = (bitmap.lHeight);
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_I444:
        bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
        bitmap.lPitch[1] = bitmap.lPitch[0];
        bitmap.lPitch[2] = bitmap.lPitch[0];
        lBufferH = (bitmap.lHeight);
        lSize = MBitmapSize(bitmap);
        break;

    case MPAF_YUV:
        bitmap.lPitch[0] =  bitmap.lWidth * 3;//LINE_BYTES(bitmap.lWidth, 24);
        lBufferH = (bitmap.lHeight);
        lSize = MBitmapSize(bitmap);
        break;
    case MPAF_GRAY8:
        bitmap.lPitch[0] =  bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 24);
        lBufferH = (bitmap.lHeight);
        lSize = MBitmapSize(bitmap);
        break;

    default:
        return MERR_UNSUPPORTED;
    }

    pData = (MByte*)MMemAlloc(MNull, lSize );
    if( MNull == pData )
        return MERR_NO_MEMORY;

    if(bitmap.pPlane[0])
        MMemFree( MNull, bitmap.pPlane[0]);
    bitmap.pPlane[0] = pData;
    MMemSet(bitmap.pPlane[0], 0, lSize);

    switch (bitmap.dwPixelArrayFormat)
    {
    case MPAF_OTHERS_NV21:
    case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
        if (bitmap.lPitch[2])
            bitmap.pPlane[2] = bitmap.pPlane[0] + bitmap.lPitch[0] * lBufferH;
        if (bitmap.lPitch[1])
            bitmap.pPlane[1] = bitmap.pPlane[2] + 1 ;
        break;

    case MPAF_OTHERS_NV12:
        if (bitmap.lPitch[1])
            bitmap.pPlane[1] = bitmap.pPlane[0] + bitmap.lPitch[0] * lBufferH;
        if (bitmap.lPitch[2])
            bitmap.pPlane[2] = bitmap.pPlane[1] + 1 ;
        break;

    default:
        if (bitmap.lPitch[1])
            bitmap.pPlane[1] = bitmap.pPlane[0] + bitmap.lPitch[0] * lBufferH;
        if (bitmap.lPitch[2])
            bitmap.pPlane[2] = bitmap.pPlane[1] + bitmap.lPitch[1] * (lBufferH/lSubSampleV);
        break;
    }
    return MOK;
}

MRESULT MBitmapFree(MBITMAP& bitmap)
{
    if (bitmap.pPlane[0])
    {
        MMemFree(MNull, bitmap.pPlane[0]);
        bitmap.pPlane[0] = MNull;
    }
    return MOK;
}

MLong MBitmapSize(MBITMAP& bitmap)
{
    MLong   lSize = 0;
    MLong   lBufferH = 0;
    MLong   lSubSampleV = 1;
    switch(bitmap.dwPixelArrayFormat)
        {
        case MPAF_RGB32_B8G8R8A8:
        case MPAF_RGB32_A8R8G8B8:
        case MPAF_RGB32_R8G8B8:
        case MPAF_RGB32_B8G8R8:
        case MPAF_RGB32_A8B8G8R8:
        case MPAF_RGB32_R8G8B8A8:
            bitmap.lPitch[0] = bitmap.lWidth * 4;//LINE_BYTES(bitmap.lWidth, 32);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_RGB24_R8G8B8:
        case MPAF_RGB24_B8G8R8:
            bitmap.lPitch[0] = bitmap.lWidth * 3;//LINE_BYTES(bitmap.lWidth, 24);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_RGB16_B5G6R5:
        case MPAF_RGB16_R5G6B5:
            bitmap.lPitch[0] = bitmap.lWidth * 2;//LINE_BYTES(bitmap.lWidth, 16);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_YUYV:
        case MPAF_YVYU:
        case MPAF_UYVY:
        case MPAF_VYUY:
            bitmap.lPitch[0] = bitmap.lWidth * 2;//LINE_BYTES(bitmap.lWidth, 16);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_I420:
        case MPAF_YV12:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0]>>1;
            bitmap.lPitch[2] = bitmap.lPitch[0]>>1;
            lBufferH = bitmap.lHeight;
            lSubSampleV = 2;
            break;

        case MPAF_OTHERS_NV21:
        case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
        case MPAF_OTHERS_NV12:
            bitmap.lPitch[0] = (bitmap.lWidth + 1) >> 1 <<1;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0];
            bitmap.lPitch[2] = bitmap.lPitch[0];
            lBufferH = (bitmap.lHeight + 1) >> 1 <<1;// NV21 format width & height must be 2 rat
            lSubSampleV = 4;
            break;

        case MPAF_I422V:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0];
            bitmap.lPitch[2] = bitmap.lPitch[0];
            lBufferH = bitmap.lHeight;
            lSubSampleV = 2;
            break;

        case MPAF_I422H:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0]>>1;
            bitmap.lPitch[2] = bitmap.lPitch[0]>>1;
            lBufferH = (bitmap.lHeight);
            break;

        case MPAF_I444:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0];
            bitmap.lPitch[2] = bitmap.lPitch[0];
            lBufferH = (bitmap.lHeight);
            break;

        case MPAF_YUV:
            bitmap.lPitch[0] = bitmap.lWidth * 3;//LINE_BYTES(bitmap.lWidth, 24);
            lBufferH = (bitmap.lHeight);
            break;
        case MPAF_GRAY8:
            bitmap.lPitch[0] = bitmap.lWidth;
            lBufferH = (bitmap.lHeight);
            break;

        default:
            return 0;
        }

    lSize = (bitmap.lPitch[0] * lBufferH + bitmap.lPitch[1] * lBufferH/lSubSampleV + bitmap.lPitch[2] * lBufferH/lSubSampleV);
    return lSize ;
}

MDWord g_dwSpaceID[] =
	{
		MPAF_RGB16_R5G6B5,
		MPAF_RGB16_B5G6R5,
		MPAF_RGB24_R8G8B8,
		MPAF_RGB24_B8G8R8,
		MPAF_RGB32_R8G8B8,
		MPAF_RGB32_A8R8G8B8,
		MPAF_RGB32_B8G8R8,
		MPAF_RGB32_B8G8R8A8,
		MPAF_RGB32_R8G8B8A8,
		MPAF_RGB32_A8B8G8R8,
		MPAF_YUV,
		MPAF_I420,
		MPAF_I422V,
		MPAF_I422H,
		MPAF_I444,
		MPAF_OTHERS_NV21,
		MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR,
		MPAF_OTHERS_NV12,
		MPAF_YUYV,
		MPAF_YVYU,
		MPAF_UYVY,
		MPAF_GRAY8
	};

CPostProcess::CPostProcess()
{
	m_dwReSizeAlg		 = MPP_RESIZE_BILINEAR;
	m_dwAspectRatio		 = MPP_ASPECT_RATIO_FIT_OUT;
	m_dwProcessorType	 = MPP_PROCESSOR_CORTEX_NEON;
	m_dwPerformanceMode	 = MPP_PERFORMANCE_AUTO;//MPP_PERFORMANCE_4X4; //| MPP_PERFORMANCE_8X8
	m_dwDither			 = 0;
	m_dwAlpha			 = 255; // 0 - transparency, 255 - Opaque
	m_dwDirection		 = 0;
}

CPostProcess::~CPostProcess()
{
}


MLong CPostProcess::BuildASVLOFFSCREEN(MBITMAP* lpBitmap, ASVLOFFSCREEN* lpOffscreen)
{
    if(MNull == lpOffscreen || MNull == lpBitmap)
    {
        return MERR_INVALID_PARAM;
    }
    return BuildASVLOFFSCREEN(*lpBitmap,*lpOffscreen);
}

MLong CPostProcess::BuildASVLOFFSCREEN(MBITMAP& bitmap, ASVLOFFSCREEN &offscreen)
{
	offscreen.i32Width = bitmap.lWidth;
	offscreen.i32Height = bitmap.lHeight;
	offscreen.pi32Pitch[0] = bitmap.lPitch[0];
	offscreen.pi32Pitch[1] = bitmap.lPitch[1];
	offscreen.pi32Pitch[2] = bitmap.lPitch[2];
	
	switch(bitmap.dwPixelArrayFormat)
	{
	case MPAF_RGB32_B8G8R8A8:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB32_B8G8R8A8;
		break;

	case MPAF_RGB32_A8R8G8B8:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB32_A8R8G8B8;
		break;

	case MPAF_RGB32_R8G8B8:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB32_R8G8B8;
		break;

	case MPAF_RGB32_B8G8R8:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB32_B8G8R8;
		break;

	case MPAF_RGB16_B5G6R5:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB16_B5G6R5;
		break;

	case MPAF_RGB16_R5G6B5:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB16_R5G6B5;
		break;

	case MPAF_RGB24_R8G8B8:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB24_R8G8B8;
		break;

	case MPAF_RGB24_B8G8R8:
		offscreen.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
		break;

	case MPAF_YUYV:
		offscreen.u32PixelArrayFormat = ASVL_PAF_YUYV;
		break;

	case MPAF_YVYU:
		offscreen.u32PixelArrayFormat = ASVL_PAF_YVYU;
		break;

	case MPAF_UYVY:
		offscreen.u32PixelArrayFormat = ASVL_PAF_UYVY;
		break;

	case MPAF_VYUY:
		offscreen.u32PixelArrayFormat = ASVL_PAF_VYUY;
		break;

	case MPAF_I420:
		offscreen.u32PixelArrayFormat = ASVL_PAF_I420;
		break;

	case MPAF_YV12:
		offscreen.u32PixelArrayFormat = ASVL_PAF_YV12;
		break;

	case MPAF_OTHERS_NV21:
	case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
		offscreen.u32PixelArrayFormat = ASVL_PAF_NV21;
		break;

	case MPAF_OTHERS_NV12:
		offscreen.u32PixelArrayFormat = ASVL_PAF_NV12;
		break;

	case MPAF_I422V:
		offscreen.u32PixelArrayFormat = ASVL_PAF_I422V;
		break;

	case MPAF_I422H:
		offscreen.u32PixelArrayFormat = ASVL_PAF_I422H;
		break;

	case MPAF_I444:
		offscreen.u32PixelArrayFormat = ASVL_PAF_I444;
		break;

	case MPAF_YUV:
		offscreen.u32PixelArrayFormat = ASVL_PAF_YUV;
		break;

	case MPAF_YVU:
		offscreen.u32PixelArrayFormat = ASVL_PAF_YVU;
		break;

	case MPAF_UVY:
		offscreen.u32PixelArrayFormat = ASVL_PAF_UVY;
		break;

	case MPAF_VUY:
		offscreen.u32PixelArrayFormat = ASVL_PAF_VUY;
		break;

	default:
		return MERR_UNSUPPORTED;
	}

	switch (bitmap.dwPixelArrayFormat)
	{
	case MPAF_OTHERS_NV21:
	case MPAF_OTHERS_NV21  | MPAF_BT601_YCBCR:
		offscreen.ppu8Plane[0] = bitmap.pPlane[0];
		offscreen.ppu8Plane[1] = bitmap.pPlane[2];
		offscreen.ppu8Plane[2] = MNull;
		break;

	case MPAF_OTHERS_NV12:
		offscreen.ppu8Plane[0] = bitmap.pPlane[0];
		offscreen.ppu8Plane[1] = bitmap.pPlane[1];
		offscreen.ppu8Plane[2] = bitmap.pPlane[1]+1;
		break;

	default:
		offscreen.ppu8Plane[0] = bitmap.pPlane[0];
		offscreen.ppu8Plane[1] = bitmap.pPlane[1];
		offscreen.ppu8Plane[2] = bitmap.pPlane[2];
		break;
	}

	return MOK;
}

MLong CPostProcess::BuildMBITMAP(LPASVLOFFSCREEN lpOffscreen,LPMBITMAP lpBitmap)
{
    if(MNull == lpOffscreen || MNull == lpBitmap)
    {
        return MERR_INVALID_PARAM;
    }
    return BuildMBITMAP(*lpOffscreen, *lpBitmap);
}

MLong CPostProcess::BuildMBITMAP(ASVLOFFSCREEN &offscreen, MBITMAP& bitmap)
{
	bitmap.lHeight = offscreen.i32Height;
	bitmap.lWidth  = offscreen.i32Width;
	bitmap.lPitch[0] = offscreen.pi32Pitch[0];
	bitmap.lPitch[1] = offscreen.pi32Pitch[1];
	bitmap.lPitch[2] = offscreen.pi32Pitch[1];
	
	switch (offscreen.u32PixelArrayFormat)
	{
	case ASVL_PAF_RGB32_B8G8R8A8:
		bitmap.dwPixelArrayFormat = MPAF_RGB32_B8G8R8A8;
		break;

	case ASVL_PAF_RGB32_A8R8G8B8:
		bitmap.dwPixelArrayFormat = MPAF_RGB32_A8R8G8B8;
		break;
		
	case ASVL_PAF_RGB32_R8G8B8:
		bitmap.dwPixelArrayFormat = MPAF_RGB32_R8G8B8;
		break;
		
	case ASVL_PAF_RGB32_B8G8R8:
		bitmap.dwPixelArrayFormat = MPAF_RGB32_B8G8R8;
		break;

	case ASVL_PAF_RGB24_R8G8B8:
		bitmap.dwPixelArrayFormat = MPAF_RGB24_R8G8B8;
		break;
		
	case ASVL_PAF_RGB24_B8G8R8:
		bitmap.dwPixelArrayFormat = MPAF_RGB24_B8G8R8;
		break;
		
	case ASVL_PAF_RGB16_B5G6R5:
		bitmap.dwPixelArrayFormat = MPAF_RGB16_B5G6R5;
		break;
		
	case ASVL_PAF_RGB16_R5G6B5:
		bitmap.dwPixelArrayFormat = MPAF_RGB16_R5G6B5;
		break;
		
	case ASVL_PAF_YUYV:
		bitmap.dwPixelArrayFormat = MPAF_YUYV;
		break;

	case ASVL_PAF_YVYU:
		bitmap.dwPixelArrayFormat = MPAF_YVYU;
		break;
		
	case ASVL_PAF_UYVY:
		bitmap.dwPixelArrayFormat = MPAF_UYVY;
		break;
		
	case ASVL_PAF_VYUY:
		bitmap.dwPixelArrayFormat = MPAF_VYUY;
		break;
		
	case ASVL_PAF_I420:
		bitmap.dwPixelArrayFormat = MPAF_I420;
		break;

	case ASVL_PAF_YV12:
		bitmap.dwPixelArrayFormat = MPAF_YV12;
		break;
		
	case ASVL_PAF_NV21:
	case ASVL_PAF_NV21  | MPAF_BT601_YCBCR:
		bitmap.dwPixelArrayFormat = MPAF_OTHERS_NV21;
		break;

	case ASVL_PAF_NV12:
		bitmap.dwPixelArrayFormat = MPAF_OTHERS_NV12;
		break;

	case ASVL_PAF_I422V:
		bitmap.dwPixelArrayFormat = MPAF_I422V;
		break;
		
	case ASVL_PAF_I422H:
		bitmap.dwPixelArrayFormat = MPAF_I422H;
		break;
		
	case ASVL_PAF_I444:
		bitmap.dwPixelArrayFormat = MPAF_I444;
		break;
				
	case ASVL_PAF_YUV:
		bitmap.dwPixelArrayFormat = MPAF_YUV;
		break;
		
	case ASVL_PAF_YVU:
		bitmap.dwPixelArrayFormat = MPAF_YVU;
		break;
		
	case ASVL_PAF_UVY:
		bitmap.dwPixelArrayFormat = MPAF_UVY;
		break;
		
	case ASVL_PAF_VUY:
		bitmap.dwPixelArrayFormat = MPAF_VUY;
		break;

	default:
		return MERR_UNSUPPORTED;
	}

	switch (offscreen.u32PixelArrayFormat)
	{
	case ASVL_PAF_NV21:
		bitmap.pPlane[0]	= (MByte*)offscreen.ppu8Plane[0];
		bitmap.pPlane[2]    = (MByte*)offscreen.ppu8Plane[1];
		bitmap.pPlane[1]	= bitmap.pPlane[2] + 1;
		break;

	case ASVL_PAF_NV12:
		bitmap.pPlane[0] = (MByte*)offscreen.ppu8Plane[0];
		bitmap.pPlane[1] = (MByte*)offscreen.ppu8Plane[1];
		bitmap.pPlane[2] = (MByte*)bitmap.pPlane[1] + 1;
		break;

	default:
		bitmap.pPlane[0] = (MByte*)offscreen.ppu8Plane[0];
		bitmap.pPlane[1] = (MByte*)offscreen.ppu8Plane[1];
		bitmap.pPlane[2] = (MByte*)offscreen.ppu8Plane[2];
		break;
	}

	return MOK;
}

MRESULT CPostProcess::checkFormat(MDWord dwPixelFormat)
{
	int i = 0;
	for (i = 0; i < ARRAY_SIZE(g_dwSpaceID); i ++)
	{
		if (g_dwSpaceID[i] == dwPixelFormat)
			break;		
	}

	if (i == ARRAY_SIZE(g_dwSpaceID))
		return MERR_UNSUPPORTED;
	else
		return MOK;
}


MRESULT CPostProcess::DoPostProcess(LPMBITMAP pSrcBitmap, LPMBITMAP pDstBitmap)
{
	LOGI("DoPostProcess in!");
	int res = MOK;
	if ((res = checkFormat(pSrcBitmap->dwPixelArrayFormat)) != MOK)
	{
		LOGI("DoPostProcess (A) res = %ld ,pSrcBitmap->dwPixelArrayFormat = %ld" , res ,pSrcBitmap->dwPixelArrayFormat);
		return res;
	}

	if ((res = checkFormat(pDstBitmap->dwPixelArrayFormat)) != MOK)
	{
		LOGI("DoPostProcess (B) res = %ld , pDstBitmap->dwPixelArrayFormat = %ld" , res, pDstBitmap->dwPixelArrayFormat);
		return res;
	}

	MPAFPIXEL PixelSrc = {0}, PixelDst = {0};
	MByte *ppSrc[MPP_MAX_PLANES];
	MInt32 lSrcPitch[MPP_MAX_PLANES];
	MByte *ppDst[MPP_MAX_PLANES];
	MInt32 lDstPitch[MPP_MAX_PLANES];
	MBLITFX mblitfx = {0};
	MHandle	pphandle;

	PixelSrc.dwHeight  = pSrcBitmap->lHeight;
	PixelSrc.dwWidth   = pSrcBitmap->lWidth;

	if(MPAF_GRAY8 == pSrcBitmap->dwPixelArrayFormat)
	{
		PixelSrc.dwSpaceID = pSrcBitmap->dwPixelArrayFormat;
	}
	else
	{
		PixelSrc.dwSpaceID = pSrcBitmap->dwPixelArrayFormat  | MPAF_BT601_YCBCR;
	}


	PixelDst.dwHeight  = pDstBitmap->lHeight;
	PixelDst.dwWidth   = pDstBitmap->lWidth;
	PixelDst.dwSpaceID = pDstBitmap->dwPixelArrayFormat;

	mblitfx.dwDirection       = m_dwDirection;
	mblitfx.dwReSizeAlg       = m_dwReSizeAlg;
	if(MPAF_GRAY8 == pSrcBitmap->dwPixelArrayFormat)
	{
		mblitfx.dwReSizeAlg       = MPP_RESIZE_NEAREST_NEIGHBOUR;
	}
	mblitfx.dwAspectRatio     = m_dwAspectRatio;
	mblitfx.dwProcessorType   = m_dwProcessorType;
	mblitfx.dwPerformanceMode = m_dwPerformanceMode;
	mblitfx.dwDither          = m_dwDither;
	mblitfx.dwAlpha           = m_dwAlpha;

 	res = MPPCreate(&PixelDst, &PixelSrc, &mblitfx, &pphandle);
 	LOGI("DoPostProcess (C) res = %ld" , res);
	if (res != MOK)
	{
		return res;
	}
	
	lSrcPitch[0] = pSrcBitmap->lPitch[0];
	lSrcPitch[1] = pSrcBitmap->lPitch[1];
	lSrcPitch[2] = pSrcBitmap->lPitch[2];
	
	lDstPitch[0] = pDstBitmap->lPitch[0];
	lDstPitch[1] = pDstBitmap->lPitch[1];
	lDstPitch[2] = pDstBitmap->lPitch[2];
	
	ppSrc[0] = pSrcBitmap->pPlane[0];
	ppSrc[1] = pSrcBitmap->pPlane[1];
	ppSrc[2] = pSrcBitmap->pPlane[2];
	
	ppDst[0] = pDstBitmap->pPlane[0];
	ppDst[1] = pDstBitmap->pPlane[1];
	ppDst[2] = pDstBitmap->pPlane[2];

	res = MPProcess(pphandle, ppDst, MNull, lDstPitch, ppSrc, lSrcPitch);
	LOGI("DoPostProcess (D) res = %ld" , res);
	if (pphandle)
	{
		MPPDestroy(pphandle);
	}
	LOGI("DoPostProcess out!");
	return res;
}


MRESULT CPostProcess::MdUtilsFormatColorIDEx(MDWord dwColorFormat, MDWord *dwSX,
		MDWord *dwSY, MDWord* dwBitCount, MDWord *dwPlanes, MDWord *dwXPicth)
{
	MDWord dwSX0, dwSY0;
	MDWord dwBitCount0;
	MDWord dwPlanes0;
	MDWord dwXPicth0;

	dwSX0 = 1;
	dwSY0 = 1;
	dwPlanes0 = 1;
	dwBitCount0 = 8;
	dwXPicth0 = 1;

	if ((dwColorFormat & 0xF0000000) == MPAF_RGBA_BASE)
	{
		dwPlanes0 = 1;
		switch (dwColorFormat & 0xF000000)
		{
			case MPAF_8BITS:
				dwBitCount0 = 8;
				break;
			case MPAF_16BITS:
				dwBitCount0 = 16;
				break;
			case MPAF_24BITS:
				dwBitCount0 = 24;
				break;
			case MPAF_32BITS:
				dwBitCount0 = 32;
				break;
			default:
				return MERR_UNSUPPORTED;
		}
	}
	else if ((dwColorFormat & 0xF0000000) == MPAF_RGB_BASE)
	{
		dwPlanes0 = 1;
		switch (dwColorFormat & 0xF000000)
		{
		case MPAF_8BITS:
			dwBitCount0 = 8;
			break;
		case MPAF_16BITS:
			dwBitCount0 = 16;
			break;
		case MPAF_24BITS:
			dwBitCount0 = 24;
			break;
		case MPAF_32BITS:
			dwBitCount0 = 32;
			break;
		default:
			return MERR_UNSUPPORTED;
		}
	}
	else if ((dwColorFormat & 0xF0000000) == MPAF_YUV_BASE)
	{
		MDWord dwYuvYCbCr = 0X00003000;

		dwColorFormat &= ~dwYuvYCbCr;

		switch (dwColorFormat)
		{
		case MPAF_YUYV:
		case MPAF_YVYU:
		case MPAF_UYVY:
		case MPAF_VYUY:
		case MPAF_YUYV2:
		case MPAF_YVYU2:
		case MPAF_UYVY2:
		case MPAF_VYUY2:
			dwBitCount0 = 16;
			break;
		case MPAF_YUV:
		case MPAF_YVU:
		case MPAF_UVY:
		case MPAF_VUY:
			dwBitCount0 = 24;
			break;
		case MPAF_I420:
		case MPAF_YV12:
			dwBitCount0 = 8;
			dwPlanes0 = 3;
			dwSX0 = 2;
			dwSY0 = 2;
			break;
		case MPAF_I422V:
		case MPAF_YV16V:
			dwBitCount0 = 8;
			dwPlanes0 = 3;
			dwSX0 = 1;
			dwSY0 = 2;
			break;
		case MPAF_I422H:
		case MPAF_YV16H:
			dwBitCount0 = 8;
			dwPlanes0 = 3;
			dwSX0 = 2;
			dwSY0 = 1;
			break;
		case MPAF_I444:
		case MPAF_YV24:
			dwBitCount0 = 8;
			dwPlanes0 = 3;
			dwSX0 = 1;
			dwSY0 = 1;
			break;

		default:
			return MERR_UNSUPPORTED;
		}
	}
	else if ((dwColorFormat & 0xF0000000) == MPAF_GRAY_BASE)
	{
		dwBitCount0 = 8;
		dwPlanes0 = 1;
		dwSX0 = 1;
		dwSY0 = 1;
	}
	else if (dwColorFormat == MPAF_OTHERS_NV21
			|| dwColorFormat == (MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR)
			|| dwColorFormat == MPAF_OTHERS_NV12
			|| dwColorFormat == (MPAF_OTHERS_NV12 | MPAF_BT601_YCBCR))
	{
		dwSX0 = 2;
		dwSY0 = 2;
		dwPlanes0 = 3;
		dwXPicth0 = 2;
	}
	else
	{
		return MERR_UNSUPPORTED;
	}

	if (dwSX)
		*dwSX = dwSX0;
	if (dwSY)
		*dwSY = dwSY0;
	if (dwBitCount)
		*dwBitCount = dwBitCount0;
	if (dwPlanes)
		*dwPlanes = dwPlanes0;
	if (dwXPicth)
		*dwXPicth = dwXPicth0;

	return MOK;
}


MRESULT CPostProcess::DoPostProcess(LPASVLOFFSCREEN pSrcOffScreen, LPASVLOFFSCREEN pDstOffScreen)
{
	MRESULT res = MOK;
	MBITMAP SrcBitmap = {0},DstBitmap = {0};
	res = BuildMBITMAP(pSrcOffScreen, &SrcBitmap);
	if (res != MOK)
		return res;
	
	res = 	BuildMBITMAP(pDstOffScreen, &DstBitmap);
 	if (res != MOK)
		return res;

	return DoPostProcess(&SrcBitmap, &DstBitmap);
}

MRESULT CPostProcess::DoPostProcess(LPMBITMAP pSrcBitmap, LPASVLOFFSCREEN pDstOffScreen)
{
	MRESULT res = MOK;
	MBITMAP DstBitmap = {0};
	res = BuildMBITMAP(pDstOffScreen, &DstBitmap);
	if (res != MOK)
		return res;

	return DoPostProcess(pSrcBitmap, &DstBitmap);
}

MRESULT CPostProcess::DoPostProcess(LPASVLOFFSCREEN pSrcOffScreen, LPMBITMAP pDstBitmap)
{
	MRESULT res = MOK;
	MBITMAP SrcBitmap = {0};
	res = BuildMBITMAP(pSrcOffScreen, &SrcBitmap);
	if (res != MOK)
		return res;

	return DoPostProcess(&SrcBitmap, pDstBitmap);
}

