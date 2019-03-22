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
 * mddef.h
 *
 * Reference:
 *
 * Description: Define some common structures, constants, callback prototypes, and so on for ArcSoft's low-level APIs.
 * 
 */

#ifndef _MDDEF_H_
#define _MDDEF_H_


#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////

#define AMGDT_GENERAL_IMEM_CONTEXT			0X92000001

/************************************************************************/
/*    Image File Format definition...                                   */
/************************************************************************/
#define AM_UNKNOWN					0x00000000
#define AM_FILE_SUPPORTED			0xFFFFFFFF

#define	AM_IMAGE_FF_MASK			0x0000FFFF  /* image file format mask */
#define	AM_BMP						0x00000001
#define	AM_JPEG						0x00000002
#define AM_GIF						0x00000004
#define AM_PNG						0x00000008
#define AM_WBMP						0x00000020
#define AM_SWF						0x00000080
#define AM_SVG						0x00000100
#define AM_TTF						0x00000200
#define AM_TIF						0x00000400
#define AM_ICO                      0x00000800
#define AM_MBITMAP					0x00001000

#define	AM_VIDEO_FF_MASK			0x00FF0000  /* video file format mask */

/************************************************************************/
/*    Image File Subformat definition...                                */
/************************************************************************/
#define AM_SUBFMT_UNUSED			0X00000000
#define AM_SUBFMT_UNKNOWN			0X00000001
#define AM_SUBFMT_GIF_STILL			0X00000002
#define AM_SUBFMT_GIF_ANIMATED		0X00000003
#define AM_SUBFMT_JPEG_BASELINE		0X00000004
#define AM_SUBFMT_JPEG_PROGRESSIVE  0X00000005
#define AM_SUBFMT_SVG_UNCOMPRESSED	0X00000006
#define AM_SUBFMT_SVG_COMPRESSED	0X00000007


/************************************************************************/
/*   Data Source Type definition                                        */
/************************************************************************/
#define AM_DATASOURCE_NULL			0xFFFFFFFF
#define AM_DATASOURCE_FILE			0x00000001
#define AM_DATASOURCE_STREAM		0x00000002
#define AM_DATASOURCE_BITMAP		0x00000003

/************************************************************************/
/*   Flip Mode definition												*/
/************************************************************************/
#define AM_FLIPMODE_NONE	0x0
#define AM_FLIPMODE_HORIZ	0x1
#define AM_FLIPMODE_VERT	0x2

#define AM_DIMENSION_ORIGINAL   0x1
#define AM_DIMENSION_TRIM       0x2
#define AM_DIMENSION_PAD        0x3

/**************************************************************************/
#define	AM_RESIZE_NEAREST_NEIGHBOUR     0x001	//Nearest Neighbour Interpolation
#define	AM_RESIZE_BILINEAR			    0x002	//Bilinear Interpolation
#define AM_RESIZE_AUTO				    0x008   //Auto choice one of arithmetic based on high quality
/**************************************************************************/

typedef MRESULT (*AM_FNPROGRESS)
(	
	MDWord      dwProgress,
	MVoid*      pUserData
);

typedef struct _tag_MDIMGFILEINFO{
	MDWord	 dwFileFormat;
	MDWord	 dwFileSubformat;
	MInt32	 lWidth;
	MInt32	 lHeight;
	MInt32    lBitCounts;
} MDIMGFILEINFO, *LPMDIMGFILEINFO;

/*******************************************************************************/
/* Define the DRM callback functions										   */
/*******************************************************************************/
typedef MBool  (*AM_FNDRMISDRM) (
	const MVoid *pFullPath, 
	MVoid*		 pUserData
	);

typedef MVoid* (*AM_FNDRMGETINFO) (
	const MVoid *pFullPath, 
	MVoid*		 pUserData
	);

typedef MVoid (*AM_FNDRMRELEASEINFO) (
	const MVoid *pInfo, 
	MVoid*		 pUserData
	);

typedef struct _tagAM_DRM_CONTEXT {
	AM_FNDRMISDRM 			 fnDrmIsDrm;
	AM_FNDRMGETINFO			 fnDrmGetInfo;
	AM_FNDRMRELEASEINFO		 fnDrmReleaseInfo;
	MVoid*					 pUserData;
} AM_IMGDRM_CONTEXT, *LPAM_IMGDRM_CONTEXT;

/************************************************************************/
/* EXIF Field Item ID Definition...                                     */
/************************************************************************/
#define AME_EXIF_THUMBNAIL					0xFFFF00F1
#define AME_EXIF_EXISTENTFLAG				0xFFFF00F2
#define AME_EXIF_THUMBNAIL_SIZE				0xFFFF00F3
#define AME_EXIF_COMPRESSED_THUMBNAIL		0xFFFF00F4

#define AME_EXIF_LARGETHUMBNAIL				0xFFFF00F5
#define AME_EXIF_COMPRESSED_LARGETHUMBNAIL	0xFFFF00F6
#define	AME_EXIF_USERDEFINED				0xFFFF00F7

#define AME_TAGID_IMAGEWIDTH				0x0100
#define AME_TAGID_IMAGEHEIGHT				0x0101
#define AME_TAGID_MAKE						0x010F
#define AME_TAGID_MODEL						0x0110
#define AME_TAGID_ORIENTATION				0x0112
#define AME_TAGID_SOFTWARE					0x0131
#define AME_TAGID_IMAGECREATETIME			0x0132
#define AME_TAGID_DATETIMEORIGINAL			0x9003
#define AME_TAGID_DATETIMEDIGITIZED			0x9004
#define AME_TAGID_EXIFVERSION				0x9000
#define AME_TAGID_XRESOLUTION				0x011A
#define AME_TAGID_YRESOLUTION				0x011B
#define AME_TAGID_RESOLUTIONUNIT			0x0128
#define AME_TAGID_USERCOMMENT				0x9286
#define AME_TAGID_RELATEDSOUNDFILE			0xA004
#define AME_TAGID_OECF						0x8828
#define AME_TAGID_SPATIALFREQUENCYRESPONSE	0xA20C
#define AME_TAGID_FILESOURCE				0xA300
#define AME_TAGID_SCENETYPE					0xA301
#define AME_TAGID_FLASH						0x9209
#define AME_TAGID_SUBJECTLOCATION			0xA214
#define AME_TAGID_ISOSPEEDRATINGS			0x8827
#define AME_TAGID_LIGHTSOURCE				0x9208
#define AME_TAGID_EXPOSUREPROGRAM			0x8822
#define AME_TAGID_COLORSPACE				0xA001
#define AME_TAGID_SENSINGMETHOD				0xA217
#define AME_TAGID_SHUTTERSPEEDVALUE			0x9201
#define AME_TAGID_FNUMBER					0x829D
#define AME_TAGID_APERTUREVALUE				0x9202
#define AME_TAGID_COMPRESSEDBITSPERPIXEL	0x9102
#define AME_TAGID_EXPOSURETIME				0x829A
#define AME_TAGID_FOCALLENGTH				0x920A
#define AME_TAGID_BRIGHTNESSVALUE			0x9203
#define AME_TAGID_EXPOSUREBIASVALUE			0x9204
#define AME_TAGID_MAXAPERTUREVALUE			0x9205
#define AME_TAGID_SUBJECTDISTANCE			0x9206
#define AME_TAGID_METERINGMODE				0x9207
#define AME_TAGID_FLASHENERGY				0xA20B
#define AME_TAGID_EXPOSUREINDEX				0xA215
#define AME_TAGID_IMAGEDESCRIPTION			0x010E
#define AME_TAGID_YCBCRPOSITION				0x0213			
#define AME_TAGID_COPYRIGHT					0x8298
#define AME_TAGID_COMPONENTCONFIGURATION	0x9101
#define AME_TAGID_FLASHPIXVERSION			0xA000
#define AME_TAGID_PIXELXDIMENSION			0xA002
#define AME_TAGID_PIXELYDIMENSION			0xA003
#define AME_TAGID_MAKERNOTE					0x927C

#define AME_TAGID_GPS_VERSION				0x0000
#define AME_TAGID_GPS_LATITUDEREF			0x0001
#define AME_TAGID_GPS_LATITUDE				0x0002
#define AME_TAGID_GPS_LONGITUDEREF			0x0003
#define AME_TAGID_GPS_LONGITUDE				0x0004
#define AME_TAGID_GPS_ALTITUDEREF			0x0005
#define AME_TAGID_GPS_ALTITUDE				0x0006
#define AME_TAGID_GPS_AREAINFORMATION 		0x001C

#define AME_TAGID_WHITEBALANCE				0xA403
#define AME_TAGID_DIGITALZOOMRATIO			0xA404
#define AME_TAGID_CONTRAST					0xA408
#define AME_TAGID_SATURATION				0xA409
#define AME_TAGID_SHARPNESS					0xA40A
#define	AME_TAGID_SCENECAPTURETYPE			0xA406
#define	AME_TAGID_EXPOSUREMODE				0xA402
#define AME_TAGID_IMAGEUNIQUEID				0xA420

#define AME_TAGID_INTEROPERABILITYINDEX 	AME_EXIF_USERDEFINED + 0x0001
#define AME_TAGID_EXTRATHUMBNAIL            AME_EXIF_USERDEFINED + 0x0002
#define AME_TAGID_EXTRATHUMBNAIL_SIZE       AME_EXIF_USERDEFINED + 0x0003

#define AME_TAGID_CUSTOM_DATA_VOICE         AME_EXIF_USERDEFINED + 0x0004
#define AME_TAGID_INTEROPERABILITYVERSION 	AME_EXIF_USERDEFINED + 0x0005
#define AME_TAGID_THUMBNAIL_ORIENTATION 	AME_EXIF_USERDEFINED + 0x0006

#define AME_TAGID_EXIFPOINTER				0x8769
#define AME_TAGID_INTEROPERABILITYPOINTER	0xA005
#define	AME_TAGID_INTERCHANGEFORMAT			0x0201
#define	AME_TAGID_INTERCHANGEFORMATLENFTH	0x0202

#define AME_TAGID_ARCM_EXTRATHUMBNAIL       0x1002
#define AME_TAGID_ARCM_INDEX                0x1001

#ifdef __cplusplus
}
#endif

#endif	/*#ifndef _MDDEF_H_*/

/* End of file */


