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
*---------------------------------------------------------------------------------------------*/
/*  
 * amimgcodec.h
 *
 * 
 *
 */

#ifndef __AMIMG_CODEC_H__
#define __AMIMG_CODEC_H__

#include "platform_export.h"

#ifdef __cplusplus
extern "C" {
#endif

//BMP type
#define AM_BMP_TYPE_WINDOWS	0L
#define AM_BMP_TYPE_OS2		1L

//BMP compression type
#define AM_BMP_RGB			0L
#define AM_BMP_RLE8			1L
#define AM_BMP_RLE4			2L

//CODEC PRIORITY DEFINITIONS
#define AM_CODEC_PRIORITY_BASE		3
#define AM_CODEC_PRIORITY_LOWEST	AM_CODEC_PRIORITY_BASE-2
#define AM_CODEC_PRIORITY_LOW		AM_CODEC_PRIORITY_BASE-1
#define AM_CODEC_PRIORITY_NORMAL	AM_CODEC_PRIORITY_BASE
#define AM_CODEC_PRIORITY_HIGH		AM_CODEC_PRIORITY_BASE+1
#define AM_CODEC_PRIORITY_HIGHEST	AM_CODEC_PRIORITY_BASE+2

//CODEC CAPABILITY:
#define AM_CODEC_CAPABILITY_FRAME		0x00000001
#define AM_CODEC_CAPABILITY_SCANLINE	0x00000002
#define AM_CODEC_CAPABILITY_RANDOMBLOCK	0x00000004
#define AM_CODEC_CAPABILITY_RESIZE      0x00000008


//CODEC MAIN TYPE
#define AM_MAINTYPEID_MASK				0xff000000
#define AM_MAINTYPEID_DECODER			0x01000000
#define AM_MAINTYPEID_ENCODER			0x02000000
#define AM_MAINTYPEID_EDITOR			0x04000000
#define AM_MAINTYPEID_IMGDRM			0x08000000

//CODEC SUB TYPE
#define AM_SUBTYPEID_MASK				0x0000ffff
#define AM_SUBTYPEID_BMP				0x00000001
#define AM_SUBTYPEID_JPG				0x00000002
#define AM_SUBTYPEID_GIF				0x00000003
#define AM_SUBTYPEID_PNG				0x00000004
#define AM_SUBTYPEID_WBMP				0x00000005
#define AM_SUBTYPEID_TIF				0x00000006
#define AM_SUBTYPEID_SWF				0x00000007
#define AM_SUBTYPEID_SVG				0x00000008
#define AM_SUBTYPEID_ICO				0x00000009
#define AM_SUBTYPEID_MBITMAP			0x0000000a

//ARCSOFT CODEC DEFINITIONS:
#define AMCM_UID_BMPDEC					0x81001001
#define AMCM_UID_JPGDEC					0x81001002
#define AMCM_UID_AJLDEC					0x81001202
#define AMCM_UID_AJLDEC2				0x81001202
#define AMCM_UID_GIFDEC					0x81001003
#define AMCM_UID_PNGDEC					0x81001004
#define AMCM_UID_PNGDEC2				0x81001204
#define AMCM_UID_WBMPDEC				0x81001005
#define AMCM_UID_TIFDEC					0x81001006
#define AMCM_UID_EXTIMGDEC				0x81001FFF  //3th decoder
#define AMCM_UID_USER_DECBASE			0x81001800
#define AMCM_UID_ICODEC					0x81001007
#define AMCM_UID_MBITMAPDEC				0x81001008
#define AMCM_UID_SVGDEC					0x81001009
#define AMCM_UID_SWFDEC					0x8100100a

#define AMCM_UID_BMPENC					0x81002001
#define AMCM_UID_JPGENC					0x81002002
#define AMCM_UID_AJLENC					0x81002202
#define AMCM_UID_AJLENC2				0x81002202
#define AMCM_UID_GIFENC					0x81002003
#define AMCM_UID_PNGENC					0x81002004
#define AMCM_UID_WBMPENC				0x81002005
#define AMCM_UID_TIFENC					0x81002006
#define AMCM_UID_EXTIMGENC				0x81002FFF  //3th encoder
#define AMCM_UID_USERENCBASE			0x81002800

#define AMCM_UID_AJL_EDITOR				0x81001802
#define AMCM_UID_AJL_EDITOR2			0x81001802

#define AMCM_UID_AJLINNERDEC			0x81001803
#define AMCM_UID_AJLINNERENC			0x81001804

#define AMCM_UID_IMGDRM				    0x82000001
#define AMGDT_GENERAL_DRM_CONTEXT		0X9200000A

MRESULT MPlugin_GetVersionInfo(MDWord* pdwRelease, MDWord* pdwMajor, MDWord* pdwMinor,MTChar* pszPatch, MDWord dwLen);

PLATFORM_EXPORT
MRESULT MImgGetFileInfo(MHandle hCMgr, HMSTREAM hStream, MDWord* pdwFileFormat, MSIZE* psizeFile, MDWord* pdwFileSubformat, MInt32* plBitCount);

PLATFORM_EXPORT
MRESULT MImgGetFileInfoWithFileName(MHandle hCMgr, HMSTREAM hStream, MTChar *pszFileName, MDWord* pdwFileFormat, 
									MSIZE* psizeFile, MDWord* pdwFileSubformat, MInt32* plBitCount);

PLATFORM_EXPORT
MRESULT MDecoder_BMPCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_JPGCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_AJLCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_PNGCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_GIFCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_WBMPCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_AJL2Create(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_TIFCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT	MDecoder_ExtImgCreate(MHandle hCMgr, MHandle* phCodecComponent);
PLATFORM_EXPORT
MRESULT	MDecoder_ICOCreate(MHandle hCMgr, MHandle* phCodecComponent);
PLATFORM_EXPORT
MRESULT	MDecoder_PNGCreate2(MHandle hCMgr, MHandle* phCodecComponent);
PLATFORM_EXPORT
MRESULT MDecoder_MBitmapCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_SVGCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MDecoder_SWFCreate(MHandle hCMgr, MHandle* phEncComponent);

PLATFORM_EXPORT
MRESULT MEncoder_BMPCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_JPGCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_AJLCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_GIFCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_PNGCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_WBMPCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_AJL2Create(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEncoder_TIFCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEditor_AJLCreate(MHandle hCMgr, MHandle* phEncComponent);
PLATFORM_EXPORT
MRESULT MEditor_AJL2Create(MHandle hCMgr, MHandle* phEncComponent);

MRESULT MIMGDrm_Create(MHandle hCMgr, MHandle* phDrmComponent);

PLATFORM_EXPORT
MRESULT MDecoderInner_AJL2Create(MHandle hCMgr, MHandle* phDecComponent);
PLATFORM_EXPORT
MRESULT MEncoderInner_AJL2Create(MHandle hCMgr, MHandle* phEncComponent);

//Register Decoder:
#define REGISTER_AMCM_BMPDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_BMPDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_BMP, dwPriority, MDecoder_BMPCreate)

#define REGISTER_AMCM_JPGDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_JPGDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_JPG, dwPriority, MDecoder_JPGCreate)

//#define REGISTER_AMCM_AJLDECODER    REGISTER_AMCM_AJLDECODER2

#define REGISTER_AMCM_AJLDECODER(dwPriority)	\
	AMCM_RegisterEx(__hCMgr, AMCM_UID_AJLDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_JPG, dwPriority, MDecoder_AJLCreate)

#define REGISTER_AMCM_AJLDECODER2(dwPriority)	\
	AMCM_RegisterEx(__hCMgr, AMCM_UID_AJLDEC2,	\
	AM_MAINTYPEID_DECODER, AM_SUBTYPEID_JPG, dwPriority, MDecoder_AJL2Create)

#define REGISTER_AMCM_PNGDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_PNGDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_PNG, dwPriority, MDecoder_PNGCreate)

#define REGISTER_AMCM_PNGDECODER2(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_PNGDEC2,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_PNG, dwPriority, MDecoder_PNGCreate2)

#define REGISTER_AMCM_GIFDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_GIFDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_GIF, dwPriority, MDecoder_GIFCreate)

#define REGISTER_AMCM_WBMPDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_WBMPDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_WBMP, dwPriority, MDecoder_WBMPCreate)

#define REGISTER_AMCM_TIFDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_TIFDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_TIF, dwPriority, MDecoder_TIFCreate)

#define REGISTER_AMCM_ICODECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_ICODEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_ICO, dwPriority, MDecoder_ICOCreate)

#define REGISTER_AMCM_SVGDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_SVGDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_SVG, dwPriority, MDecoder_SVGCreate)

#define REGISTER_AMCM_SWFDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_SWFDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_SWF, dwPriority, MDecoder_SWFCreate)

//Register Encoder:
#define REGISTER_AMCM_BMPENCODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_BMPENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_BMP, dwPriority, MEncoder_BMPCreate)

#define REGISTER_AMCM_JPGENCODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_JPGENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_JPG, dwPriority, MEncoder_JPGCreate)

//#define REGISTER_AMCM_AJLENCODER    REGISTER_AMCM_AJLENCODER2

#define REGISTER_AMCM_AJLENCODER(dwPriority)	\
	AMCM_RegisterEx(__hCMgr, AMCM_UID_AJLENC,	\
	AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_JPG, dwPriority, MEncoder_AJLCreate)

#define REGISTER_AMCM_AJLENCODER2(dwPriority)	\
	AMCM_RegisterEx(__hCMgr, AMCM_UID_AJLENC2,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_JPG, dwPriority, MEncoder_AJL2Create)

#define REGISTER_AMCM_GIFENCODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_GIFENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_GIF, dwPriority, MEncoder_GIFCreate)

#define REGISTER_AMCM_PNGENCODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_PNGENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_PNG, dwPriority, MEncoder_PNGCreate)

#define REGISTER_AMCM_WBMPENCODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_WBMPENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_WBMP, dwPriority, MEncoder_WBMPCreate)

#define REGISTER_AMCM_TIFENCODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_TIFENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_TIF, dwPriority, MEncoder_TIFCreate)

#define REGISTER_AMCM_EXTIMGDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_EXTIMGDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_MASK, dwPriority, MDecoder_ExtImgCreate)

#define REGISTER_AMCM_MBITMAPDECODER(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_MBITMAPDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_MBITMAP, dwPriority, MDecoder_MBitmapCreate)
//Register ajleditor:
//#define REGISTER_AMCM_AJLEDITOR   REGISTER_AMCM_AJLEDITOR2

#define REGISTER_AMCM_AJLEDITOR(dwPriority)		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_AJL_EDITOR,	\
				AM_MAINTYPEID_EDITOR, AM_SUBTYPEID_JPG, dwPriority, MEditor_AJLCreate)

#define REGISTER_AMCM_AJLEDITOR2(dwPriority)		\
	AMCM_RegisterEx(__hCMgr, AMCM_UID_AJL_EDITOR2,	\
	AM_MAINTYPEID_EDITOR, AM_SUBTYPEID_JPG, dwPriority, MEditor_AJL2Create)


//Register DRM component
#define REGISTER_AMCM_IMGDRM()		\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_IMGDRM,			\
					AM_MAINTYPEID_IMGDRM, AM_SUBTYPEID_MASK, 3, MIMGDrm_Create)

#define  AMCM_SETCONTEXT_IMGDRM(hAMCM, pDrm_Context) \
		AMCM_SetGlobalData(hAMCM, AMGDT_GENERAL_DRM_CONTEXT, pDrm_Context, sizeof(AM_IMGDRM_CONTEXT));

//Register AJL codec
#define REGISTER_AMCM_AJLINNERDECODER(dwPriority)	\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_AJLINNERDEC,	\
				AM_MAINTYPEID_DECODER, AM_SUBTYPEID_JPG, dwPriority, MDecoderInner_AJL2Create)

#define REGISTER_AMCM_AJLINNERENCODER(dwPriority)	\
		AMCM_RegisterEx(__hCMgr, AMCM_UID_AJLINNERENC,	\
				AM_MAINTYPEID_ENCODER, AM_SUBTYPEID_JPG, dwPriority, MEncoderInner_AJL2Create)
		


//Register 3th(ext) codec component
//    dwUID of REGISTER_EXT_DECODEREX should be:  AMGDT_EXT_DECODER_UID_START<= dwUID <= AMGDT_EXT_DECODER_UID_END
#define AMGDT_EXT_DECODER_UID_START		0X92000F00
#define AMGDT_EXT_DECODER_UID_END		0X92000F1F

//    dwUID of REGISTER_EXT_ENCODEREX should be:  AMGDT_EXT_ENCODER_UID_START<= dwUID <= AMGDT_EXT_ENCODER_UID_END
#define AMGDT_EXT_ENCODER_UID_START		0X92000F20
#define AMGDT_EXT_ENCODER_UID_END		0X92000F3F

//MRESULT REGISTER_EXT_DECODEREX(MDWord dwUID, LPARC_EXT_IMGDECODER pExtImgDecoder);
#define REGISTER_EXT_DECODEREX(dwUID, pExtImgDecoder, lSize)		\
		AMCM_SetGlobalData(__hCMgr, dwUID, (MVoid*)(pExtImgDecoder), lSize)

//MRESULT REGISTER_EXT_ENCODEREX(MDWord dwUID, LPARC_EXT_IMGENCODER pExtImgEncoder);
#define REGISTER_EXT_ENCODEREX(dwUID, pExtImgEncoder, lSize)		\
		AMCM_SetGlobalData(__hCMgr, dwUID, (MVoid*)(pExtImgEncoder), lSize)

#define AM_CODEC_PROP_CAPABILITY		0x0001
#define AM_CODEC_PROP_IMG_SIZE			0x0002
#define AM_CODEC_PROP_DEST_PIXEL_FORMAT	0x0003
#define AM_CODEC_PROP_TRANS_COLOR		0x0004
#define AM_CODEC_PROP_YUV_BT_TYPE		0x0005
#define AM_CODEC_PROP_BACKGROUND_COLOR	        0x0006

typedef struct tagAM_BMPINFOHEADER
{
    MUInt32      biSize;
    MInt32       biWidth;
    MInt32       biHeight;
    MWord       biPlanes;
    MWord       biBitCount;
    MUInt32      biCompression;
    MUInt32      biSizeImage;
    MInt32       biXPelsPerMeter;
    MInt32       biYPelsPerMeter;
    MUInt32      biClrUsed;
    MUInt32      biClrImportant;
} AM_BMPINFOHEADER, *LPAM_BMPINFOHEADER, *PAM_BMPINFOHEADER;

typedef struct tagAM_GIF_FRAMEINFO { 	
	MWord		wLeft;
	MWord		wTop;
	MWord		wWidth;
	MWord		wHeight;
	MWord		wLocalColorBits;
	MWord		wDelayTime;
	MInt32		lDisposalMethod;
	MBool		bLocalColorTable;
	MBool		bInterlace;
	MBool		bTransColor;
	MByte		byTransColor;
} AM_GIF_FRAMEINFO, *LPAM_GIF_FRAMEINFO; 

typedef struct tagAM_GIF_GLOBALINFO
{
	MWord 		wSrcWidth;
	MWord 		wSrcHeight;
	MInt32       lGlobColBits;
	MBool		bGlobalPal;
	MBool		bSortFlag;
	MByte       byColorResolution;
	MByte       byBackColor;
}AM_GIF_GLOBALINFO, * LPAM_GIF_GLOBALINFO;


typedef struct tagAM_PROGRESSCALLBACK
{
    AM_FNPROGRESS		fnProgress;
    MPVoid				pUsrParam;    
} AM_PROGRESSCALLBACK, *LPAM_PROGRESSCALLBACK;


//////////////////////////////////////////////////////////////////////////
//Encode Structure Definition
typedef struct tagAM_ENCODE_INITPARAM
{
	HMSTREAM	hStream;
	MInt32		lWidth;
	MInt32		lHeight;
	MDWord		dwPixelFormat;
	MInt32		lXPelsPerMeter;
	MInt32		lYPelsPerMeter;
}AM_ENCODE_INITPARAM,*LPAM_ENCODE_INITPARAM;

#if defined( __cplusplus )
}
#endif  //


#endif //__AMIMG_CODEC_H__

// End of file
