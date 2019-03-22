/*----------------------------------------------------------------------------------------------
*
 *This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and		
 *confidential information. 
 *
 *The information and code contained in this file is only for authorized ArcSoft employees 
 *to design, create, modify, or review.
 *
 *DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
 *
 *If you are not an intended recipient of this file, you must not copy, distribute, modify, 
 *or take any action in reliance on it. 
 *
 *If you have received this file in error, please immediately notify ArcSoft and 
 *permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/
/*
  *mdutils.h
 *
  *Reference:
 *
  *Description: Define some imaging utility APIs.
  *
 */

#ifndef _MDUTILS_H_
#define _MDUTILS_H_

#include "amplat.h"
#include "amdisplay.h"
#include "mddef.h"
#include "amimgcodec.h"
#include "amimgerror.h"

#include "platform_export.h"

#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////

#define  AM_MSG_PROGRESS 0x01
#define  AM_MSG_ERROR    0x02

typedef MRESULT (*AM_IMAGE_FNNOTIFY) (
							MDWord		dwMsg,
							MDWord		dwParam,
							MVoid* 		pUserData,
							MBool* 		pbCancel
);
PLATFORM_EXPORT MRESULT MdExifCreate( MHandle hAMCM,
                     HMSTREAM hFileStream,
                     MHandle *phExif );
PLATFORM_EXPORT MRESULT MdExifGetFieldData( MHandle hExif,
                            MDWord dwFieldID,
                            MPVoid pDataBuf,
                            MInt32 *plBufSize);

PLATFORM_EXPORT MRESULT MdExifSetFieldData( MHandle hExif,
                            MDWord dwFieldID,
                            MPVoid pDataBuf,
                            MInt32 lBufSize);

PLATFORM_EXPORT MRESULT MdGetExifFieldData(MHandle hAMCM,
                           HMSTREAM  hFileStream,
                           MDWord *pdwFieldID,
                           MPVoid *ppDataBuf,
                           MInt32 *plBufSize,
                           MInt32 lFieldCnt);

PLATFORM_EXPORT MRESULT MdExifDestroy( MHandle hExif );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdGetImgFileInfo( MHandle hAMCM, HMSTREAM hImgStream, LPMDIMGFILEINFO pImgInfo );
PLATFORM_EXPORT MRESULT MdGetImgFileFormat( MHandle hAMCM, HMSTREAM hImgStream, MDWord *pdwFormat );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdBitmapAlloc( MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapFree( MBITMAP *pBitmap );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdBitmapMerge( MHandle hAMCM, MBITMAP *pBitmap, 
                      MBITMAP *pBitmapFore, PMPOINT pPtForePos, 
                      MBITMAP *pBitmapMask, PMPOINT pPtMaskPos, MInt32 lOpacity );
PLATFORM_EXPORT MRESULT MdBitmapFillColor( MHandle hAMCM, MBITMAP *pBitmap,
                          MCOLORREF clrFill, PMRECT prtFillRect,
                          MBITMAP *pMask, MInt32 lOpacity); 
PLATFORM_EXPORT MRESULT MdBitmapResample( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap );
PLATFORM_EXPORT MRESULT MdBitmapCrop( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MRECT *prtCrop );
PLATFORM_EXPORT MRESULT MdBitmapRotate( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MInt32 lDegree );
PLATFORM_EXPORT MRESULT MdBitmapFreeRotate( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MInt32 lDegree );
PLATFORM_EXPORT MRESULT MdBitmapFlip( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MDWord dwFlipMode );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdBitmapLoad( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapLoad2( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapLoadWithAlphaTrans( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap,MDWord dwAlphaTrans );
PLATFORM_EXPORT MRESULT MdBitmapLoad2WithAlphaTrans( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap,MDWord dwAlphaTrans );
PLATFORM_EXPORT MRESULT MdBitmapSave( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapSave2( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapLoadRaw( HMSTREAM hFileStream, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapLoadRaw2( MVoid *pFile, MBITMAP *pBitmap );

PLATFORM_EXPORT MRESULT MdFileResize( MHandle hAMCM, 
                      MVoid *pInputFile, 
                      MInt32 lWidth, 
                      MInt32 lHeight, 
                      MDWord dwMaxFileSize, 
                      MVoid *pOutputFile);

PLATFORM_EXPORT MRESULT MdBitmapColorConvert( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap );

PLATFORM_EXPORT MRESULT MdBitmapCropRotFlipResample( MHandle hAMCM, 
                            MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, 
                            MRECT *prtSrc, MRECT *prtDst,
                            MInt32 lRotateDegree, MDWord dwFlipMode );

PLATFORM_EXPORT MRESULT MdBitmapLoadFast(MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP* pBitmap);
PLATFORM_EXPORT MRESULT MdBitmapLoadFast2(MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP* pBitmap);

//////////////////////////////////////////////////////////////////////////

#define MD_VTEXT_DIRECTION_LEFT_TO_RIGHT			0x00000000
#define MD_VTEXT_DIRECTION_RIGHT_TO_LEFT			0x00000001

#define MD_VTEXT_ALIGN_LEFT							0x00000000
#define MD_VTEXT_ALIGN_RIGHT						0x00000002
#define MD_VTEXT_ALIGN_CENTER_HORZ					0x00000004

#define MD_VTEXT_ALIGN_TOP							0x00000000
#define MD_VTEXT_ALIGN_BOTTOM						0x00000008
#define MD_VTEXT_ALIGN_CENTER_VERT					0x00000010

#define MD_VTEXT_SHAPE_CONVERT						0x00000000
#define MD_VTEXT_SHAPE_ORIGINAL						0x00000020

#define MD_VTEXT_ORDER_REORDER						0x00000000
#define MD_VTEXT_ORDER_ORIGINAL						0x00000040

#define MD_VTEXT_FONT_STYLE_NORMAL					0x00000000
#define MD_VTEXT_FONT_STYLE_ITALIC					0x00000080

#define MD_VTEXT_FONT_WEIGHT_100					0x00000100
#define MD_VTEXT_FONT_WEIGHT_200					0x00000200
#define MD_VTEXT_FONT_WEIGHT_300					0x00000300
#define MD_VTEXT_FONT_WEIGHT_400					0x00000400
#define MD_VTEXT_FONT_WEIGHT_500					0x00000500
#define MD_VTEXT_FONT_WEIGHT_600					0x00000600
#define MD_VTEXT_FONT_WEIGHT_700					0x00000700
#define MD_VTEXT_FONT_WEIGHT_800					0x00000800
#define MD_VTEXT_FONT_WEIGHT_900					0x00000900

#define MD_VTEXT_DECORATION_NONE					0x00000000
#define MD_VTEXT_DECORATION_UNDERLINE				0x00001000
#define MD_VTEXT_DECORATION_OVERLINE				0x00002000
#define MD_VTEXT_DECORATION_LINETHROUGH				0x00004000

#define MD_VTEXT_FONT_WEIGHT_NORMAL					MD_VTEXT_FONT_WEIGHT_400
#define MD_VTEXT_FONT_WEIGHT_BOLD					MD_VTEXT_FONT_WEIGHT_700
//////////////////////////////////////////////////////////////////////////
typedef struct  _tagMDVTEXTINFO
{
	MWChar*		pszText;
	MCOLORREF	clrTxt;
	MInt32		lFontSize;
	MPVoid		pDataTTF;
    MDWord      dwDSTypeTTF;
	MDWord		dwFormatTTF;
	MPVoid		pDataBubble;
    MDWord      dwDSTypeBubble;
	MDWord		dwFormatBubble;
	MCOLORREF	clrBubble;
	MBool       bAutoLine;
	MInt32       lLineWidth;
	MDWord		dwTextFlag;
}MDVTEXTINFO, *LPMDVTEXTINFO;

PLATFORM_EXPORT MRESULT MdPoint2Mask(MHandle hAMCM, MSIZE sizeSrc, MPOINT *pptPoint, MDWord dwPtNum, MBITMAP *pbmpDstMask, MBool bReverse); 
PLATFORM_EXPORT MRESULT MdEllipse2Mask(MHandle hAMCM, MSIZE sizeSrc, MRECT *prtEllipse, MBITMAP *pbmpDstMask, MBool bReverse); 
PLATFORM_EXPORT MRESULT MdEllipse2Mask2(MHandle hAMCM, MSIZE sizeSrc, MRECT *prtEllipse, 
					   MBITMAP *pbmpDstMask, MRECT *prtMask, MBool bReverse);

PLATFORM_EXPORT MRESULT MdCreateMagicWand(MHandle hAMCM, MBITMAP *pbmpData, MHandle *phMagicWand);
PLATFORM_EXPORT MRESULT MdGetMagicMask(MHandle hMagicWand,  MPOINT *pPoint, MInt32 lTolerance, MBool bReverse, 
					   MBITMAP **ppbmpMask, MPOINT **ppptPoint, MInt32 **ppPolyPtNum, MInt32 *plPolyCount);
PLATFORM_EXPORT MRESULT MdDestroyMagicWand(MHandle hMagicWand);

PLATFORM_EXPORT MRESULT MdDrawShadowWithRect(MBITMAP *pbmpData, MRECT *prtRect, MBITMAP *pbmpShadow, MInt32 lOpacity, MBool bReverse,
							 AM_FNPROGRESS fnProgress, MVoid *pUserData);
PLATFORM_EXPORT MRESULT MdDrawShadowWithMask(MBITMAP *pbmpData, MBITMAP *pbmpMask, MBITMAP *pbmpShadow, MInt32 lOpacity,
							 AM_FNPROGRESS fnProgress, MVoid *pUserData);
PLATFORM_EXPORT MRESULT MdUtilsJpgAttachStream(MByte* pHead, MInt32 *plHeadLen, MBool bHeadAttach, MByte* pTail, MInt32 lTailLen);

MRESULT MdUtilsAGIFEncode_Create(MHandle hAMCM, HMSTREAM hStream, MSIZE sizeDst, MHandle *hEncode);
MRESULT MdUtilsAGIFEncode_CreateAsynPalette(MBITMAP *pbmpFrame, MHandle *hPalette, MDWord dwAlgoithmType);
MRESULT MdUtilsAGIFEncode_CreateAsynPaletteWithMask(MBITMAP *pbmpFrame, MBITMAP *pbmpMask,MHandle *hPalette, MDWord dwAlgoithmType);
MRESULT MdUtilsAGIFEncode_AsynPaletteProcess( MHandle hPalette);
MRESULT MdUtilsAGIFEncode_AsynPaletteProcessWithMask( MHandle hPalette);
MRESULT MdUtilsAGIFEncode_SetLoopNum(MHandle hEncode, MInt32 lLoopNum);
MRESULT MdUtilsAGIFEncode_UsePalette(MHandle hEncode, MBool bSame);

MRESULT MdUtilsAGIFEncode_SetAlgorithmType(MHandle hEncode, MDWord dwAlgorithmType);//0 is old algorithm,1 is new algorithm
MRESULT MdUtilsAGIFEncode_SetDurationTime(MHandle hEncode, MInt32 lDurationTime);//1/100 s

MRESULT MdUtilsAGIFEncode_AddFrame(MHandle hEncode, MBITMAP *pbmpFrame,MPOINT ptOffset);

MRESULT MdUtilsAGIFEncode_CreatePalette(MBITMAP *pbmpFrame, MHandle *hPalette,MDWord dwAlgoithmType);

//lSize =  LINE_BYTES(pbmpFrame->lWidth, 8) * pbmpFrame->lHeight); LINE_BYTES is defined in adkutils.h
MRESULT MdUtilsAGIFEncode_GetFrameDataByPalette(MHandle hPalette,MBITMAP *pbmpFrame,MByte *pDest, MInt32 lDestLineBytes);

MRESULT MdUtilsAGIFEncode_CreatePaletteWithMask(MBITMAP *pbmpFrame, MBITMAP *pbmpMask,MHandle *hPalette,MDWord dwAlgoithmType);

MRESULT MdUtilsAGIFEncode_GetFrameDataByPaletteWithMask(MHandle hPalette,MBITMAP *pbmpFrame,MBITMAP *pbmpMask,MByte *pDest, MInt32 lDestLineBytes); 


MRESULT MdUtilsAGIFEncode_AddFrameWithData(MHandle hEncode,MHandle hPalette,MByte *pDest, MInt32 lDestLineBytes,MInt32 lW, MInt32 lH,MPOINT ptOffset);

MRESULT MdUtilsAGIFEncode_DestoryPalette( MHandle hPalette);
 
//Modify Mask Based on Difference 
MRESULT MdUtilsMASK_MMBD(MBITMAP *pPreFrame, MBITMAP *pCurFrame ,MBITMAP *pMask);

MRESULT MdUtilsAGIFEncode_LoopDataDostep(MHandle hEncode,MInt32 *lpRemainStep );

MRESULT MdUtilsAGIFEncode_Destroy(MHandle hEncode);

MRESULT MdUtilsAGIFDecode_Create(MHandle hAMCM, HMSTREAM hStream, MDWord dwDstPixFormat, MSIZE *psizeDst, MInt32 *plFrameNum,
								 MHandle *hDecode);
MRESULT MdUtilsAGIFDecode_DecodeFrame(MHandle hDecode, MBITMAP *pbmpFrame);
MRESULT MdUtilsAGIFDecode_GetDelayTime(MHandle hDecode,  MDWord* pdwDelayTime);
MRESULT MdUtilsAGIFDecode_Destroy(MHandle hDecode);


PLATFORM_EXPORT MRESULT MdUtilsLoad( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdUtilsLoad2( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap );

PLATFORM_EXPORT MRESULT MdUtilsLoad2WithCallBack( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap,AM_IMAGE_FNNOTIFY fnNotify, MVoid* pUserData );
PLATFORM_EXPORT MRESULT MdUtilsLoadWithCallBack( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap,AM_IMAGE_FNNOTIFY fnNotify, MVoid* pUserData );

PLATFORM_EXPORT MRESULT MdMask_Create( MHandle *hMask );
PLATFORM_EXPORT MRESULT MdMask_SetPoint( MHandle hMask,MPOINT *pptPoint );

PLATFORM_EXPORT MRESULT MdMask_Apply( MHandle hMask, MBITMAP *pBitmapMask );
PLATFORM_EXPORT MRESULT MdMask_Destroy( MHandle hMask );

PLATFORM_EXPORT MRESULT MdUtils_FeatherMask(  MBITMAP *pBitmapMask,MInt32 lHalfRadius );

PLATFORM_EXPORT MRESULT MdMultiThreadDeocde(MByte *pSrcFileBuf, MInt32 lSrcFileBufSize, MBITMAP *pBitmap, MDWord dwImgFormat, MInt32 lThreadNum);

PLATFORM_EXPORT MRESULT MdMultiThreadEncode(HMSTREAM hStreamDes, MBITMAP *pBitmap, MDWord dwImgFormat, MInt32 lQuality, MInt32 lThreadNum);
PLATFORM_EXPORT MRESULT MdMultiThread_CreateEncoder( MInt32 lSrcW, MInt32 lSrcH,MDWord dwImgFormat,MInt32 lQuality,MHandle *phMultiEncoder );
PLATFORM_EXPORT MRESULT MdMultiThread_Encode( MHandle hAMCM,MHandle hMultiEncoder, HMSTREAM hStream,MBITMAP *pBitmap,MInt32 *plStartLine, MInt32 *plLines);
PLATFORM_EXPORT MRESULT MdMultiThread_AttachStream(MHandle hMultiEncoder, HMSTREAM hDstStream,HMSTREAM hHeadStream,HMSTREAM hTailStream );
PLATFORM_EXPORT MRESULT MdMultiThread_DestoryEncoder( MHandle hMultiEncoder );

#define AM_GIF_PROP_DURATION_TIME 	0x00F0
/**
*	This property is used to set/get the duration time of each frame gif
*  
*	
*	Parameter:
*		pData		pointer to duration time
*		lDataSize	sizeof(MDWord)
*/

#define AM_GIF_PROP_ENCODE_ALGORITHM_TYPE 	0x00F1
/**
*	This property is used to set the Algorithm Type of encode each frame gif
*  
*	
*	Parameter:
*		pData		pointer to Algorithm Type 0,1,2
*		lDataSize	sizeof(MDWord)
*/

#define AM_GIF_PROP_ENCODE_USE_GOBAL_PALETTE  0x00F2
/**
*	This property is used to set use the same palette or not
*  
*	
*	Parameter:
*		pData		pointer to value of palette
*		lDataSize	sizeof(MBool)
*/

#define AM_GIF_PROP_ENCODE_LOOP_NUMBER  0x00F3
/**
*	This property is used to set loop number of gif
*  
*	
*	Parameter:
*		pData		pointer to value of loop number
*		lDataSize	sizeof(MInt32)
*/

#define AM_GIF_PROP_ENCODE_LOOP_MODE  0x00F4
/**
*	This property is used to loop mode
*  
*	
*	Parameter:
*		pData		pointer to value of loop number
*		lDataSize	sizeof(MBool)
*/

PLATFORM_EXPORT MRESULT MdMultiThread_CreateGifEncoder(MHandle hAMCM,HMSTREAM hDstStream,MSIZE sizeDst, MHandle *phMultiGifEncoder);
PLATFORM_EXPORT MRESULT MdMultiThread_SetGifProp(MHandle hMultiGifEncoder, MDWord dwPropID, MPVoid pData, MInt32 lDataSize);
PLATFORM_EXPORT MRESULT MdMultiThread_SetGoalPaltteInfo(MHandle hMultiGifEncoder,MBITMAP *pBitmap,MBITMAP *pBitmapMsak);
PLATFORM_EXPORT MRESULT MdMultiThread_EncodeGifFrame( MHandle hMultiGifEncoder, MBITMAP *pBitmap,MBITMAP *pBitmapMsak,MPOINT ptOffSet, MInt32 lFrameIndex,MByte* pBuf,MBool bLastFrame);
PLATFORM_EXPORT MRESULT MdMultiThread_DoStep( MHandle hMultiGifEncoder,MInt32 *lpStepHasDone);
PLATFORM_EXPORT MRESULT MdMultiThread_DestoryGifEncoder( MHandle hMultiGifEncoder );


#ifdef __cplusplus
}
#endif

#endif	/*#ifndef _MDUTILS_H_*/

/*End of file */



