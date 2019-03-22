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
* mdexif.h
* 
* 
* Copyright:  (C) 2008, ArcSoft Inc. All rights reserved
*
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MD_EXIF_H_
#define _MD_EXIF_H_

#include "platform_export.h"

typedef struct _tag_exif_photofix_para
{
	MInt32 	lColorBalanceRed;               //-100 ~ 100
	MInt32 	lColorBalanceGreen; 			//-100 ~ 100
	MInt32 	lColorBalanceBlue; 				//-100 ~ 100	
	MInt32 	lSaturation; 					//-100 ~ 100	
	MInt32 	lBrightness; 					//-100 ~ 100	
	MInt32 	lContrastMax;					//0 ~ 511
	MInt32 	lContrastMin; 					//-256 ~ 255	
	MInt32 	lSharpness; 					//0 ~ 100	
	MInt32 	lShadows; 						//0 ~ 100
	MInt32 	lHighlights; 					//-100 ~ 0	
	MInt32	lDenoiseParam;					//0 ~ 100
	MDWord  dwZoom;	 
	MDWord  dwRotDegree;
	MRECT   rcCrop;
		
}AMEXIFPHOTOFIXPARA, *LPAMEXIFPHOTOFIXPARA;
//exif adapter layer 
//
//

/*
 *  MExif_Create
 *      Create an EXIF handle
 *
 *	Parameter:
 *				hMemContext     (in)		Handle to the memory context
 *				phExif		    (in)		Address to receive the EXIF handle
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *	Remark:
 *
 */
PLATFORM_EXPORT MRESULT MExif_Create( MHandle hMemContext, MHandle *phExif );

/*
 *  MExif_Destroy
 *      Destroy the EXIF handle
 *
 *	Parameter:
 *				hExif		    (in)		The EXIF handle
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *	Remark:
 *
 */
PLATFORM_EXPORT MRESULT MExif_Destroy( MHandle hExif );

/*
 *  MExif_InitFromStream
 *      Initialize the EXIF handle from an input JPEG stream.
 *
 *	Parameter:
 *				hExif		    (in)		The EXIF handle
 *				hStream		    (in)		The JPEG file stream.
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *	Remark:
 *
 */
PLATFORM_EXPORT MRESULT MExif_InitFromStream( MHandle hExif, HMSTREAM hStream );

/*
 *  MExif_InitBlank
 *      Initialize the EXIF handle with blank EXIF information.
 *
 *	Parameter:
 *				hExif		    (in)		The EXIF handle
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *	Remark:
 *
 */
PLATFORM_EXPORT MRESULT MExif_InitBlank( MHandle hExif );


/*
 *  MExif_GetFieldData
 *      Get EXIF field data from the EXIF handle.
 *
 *	Parameter:
 *				hExif		    (in)		The EXIF handle
 *				dwFieldID		(in)		EXIF tag field ID
 *				pDataBuf		(in)		Address of the data buffer
 *				plBufSize       (in,out)	Pointer to a MDWord value indicating size of pDataBuf, in BYTES
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *	Remark:
 *      1. For EXIF field IDs, please refer to mddef.h, TMM_Advanced_Photo_Editor_API_Reference_V3.5.2_20080417.doc
 *      2. EXIF field IDs that need not support: AME_EXIF_THUMBNAIL, AME_EXIF_EXISTENTFLAG
 *
 */
PLATFORM_EXPORT MRESULT MExif_GetFieldData( MHandle hExif,
                            MDWord dwFieldID,
                            MPVoid pDataBuf,
                            MInt32 *plBufSize);
                            
/*
 *  MExif_SetFieldData
 *      Set EXIF field data to the EXIF handle.
 *
 *	Parameter:
 *				hExif		    (in)		The EXIF handle
 *				dwFieldID		(in)		EXIF tag field ID
 *				pDataBuf		(in)		Address of the data buffer
 *				lBufSize        (in)	    size of pDataBuf, in BYTES
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *	Remark:
 *      1. Please refer to "Remark" of MExif_GetFieldData
 *
 */
PLATFORM_EXPORT MRESULT MExif_SetFieldData( MHandle hExif,
                            MDWord dwFieldID,
                            MPVoid pDataBuf,
                            MInt32 lBufSize);

/*
 *  MExif_DeleteFieldData
 *      delete EXIF field data .
 *
 *	Parameter:
 *				hExif		    (in)		The EXIF handle
 *				dwFieldID		(in)		EXIF tag field ID
 *              
 *
 *	Return:
 *				Returns MOK if successful, or an error value otherwise.
 *
 *
 */
PLATFORM_EXPORT MRESULT MExif_DeleteFieldData( MHandle hExif,MDWord dwFieldID);

PLATFORM_EXPORT MRESULT MExif_LockExifData( MHandle hExif, MVoid **pExifData, MDWord* pdwExifDataLen);
PLATFORM_EXPORT MRESULT MExif_UnLockExifData( MHandle hExif );

/**
 *	MExif_AddExifToJpegFile
 *				Add EXIF data to JPEG file that haven't EXIF data
 *	
 *	Parameter:
 *				hStream			(in)		Handle to the stream of JPEG file, must create with STREAM_W_PLUS
 *											must open with STREAM_R_PLUS mode
 *				pExifData		(in)		Pointer to the EXIF data
 *				dwExifDataLen	(in)		Length of EXIF data
 *
 *	Return:
 *				Returns MD_OK if successful, or an error value otherwise.
 *
 *	Remark:
 *				
 */
PLATFORM_EXPORT MRESULT MExif_AddExifToJpegFile(HMSTREAM hStream, MVoid *pExifData, MDWord dwExifDataLen);

/**
 *	MExif_AddExifToReservedSpaceInJpegFile
 *				Add EXIF data to reserved EXIF space in JPEG file
 *	
 *	Parameter:
 *				hStream			(in)		Handle to the stream of JPEG file, must create with STREAM_W_PLUS
 *											must open with STREAM_R_PLUS mode
 *				pExifData		(in)		Pointer to the EXIF data
 *				dwExifDataLen	(in)		Length of EXIF data
 *
 *	Return:
 *				Returns MD_OK if successful, or an error value otherwise.
 *
 *	Remark:
 *				
 */
PLATFORM_EXPORT MRESULT MExif_AddExifToReservedSpaceInJpegFile(HMSTREAM hStream, MVoid *pExifData, MDWord dwExifDataLen);

/**
 *	MExif_AddPhotoFixInfoToJpegFile
 *				Add PhotoFix information to JPEG file,stored in APP1 segment
 *	
 *	Parameter:
 *				hStream			(in)		Handle to the stream of JPEG file, must create with STREAM_W_PLUS
 *											must open with STREAM_R_PLUS mode
 *				pPhotoFix		(in)		Pointer to the PhotoFix data				
 *
 *	Return:
 *				Returns MD_OK if successful, or an error value otherwise.
 *
 *	Remark:
 *				
 */
PLATFORM_EXPORT MRESULT MExif_AddPhotoFixInfoToJpegFile(HMSTREAM hStream, AMEXIFPHOTOFIXPARA *pPhotoFix);

/**
 *	MExif_GetPhotoFixInfoFromJpegFile
 *				Get PhotoFix information Fro, JPEG file,stored in APP1 segment
 *	
 *	Parameter:
 *				hStream			(in)		Handle to the stream of JPEG file
 *				pPhotoFix		(in)		Pointer to the PhotoFix data				
 *
 *	Return:
 *				Returns MD_OK if successful, or an error value otherwise.
 *
 *	Remark:
 *				
 */
PLATFORM_EXPORT MRESULT MExif_GetPhotoFixInfoFromJpegFile(HMSTREAM hStream, AMEXIFPHOTOFIXPARA *pPhotoFix);


/*
*  Following APIs are used for manage external exif info(app10) stored in JPEG file ending.
*/
PLATFORM_EXPORT MRESULT MExif_InitExtExifFromStream(MHandle hExif, HMSTREAM hStream);
PLATFORM_EXPORT MRESULT MExif_LockExtExifData(MHandle hExif, MVoid **pExifData, MDWord *pdwExifDataLen);
PLATFORM_EXPORT MRESULT MExif_UnLockExtExifData(MHandle hExtExifData );
PLATFORM_EXPORT MRESULT MExif_AddExtExifToJpegFile(HMSTREAM hStream, MVoid *pExtExifData, MDWord dwExtExifDataLen);


#ifdef __cplusplus
}
#endif

#endif // _MD_EXIF_H_