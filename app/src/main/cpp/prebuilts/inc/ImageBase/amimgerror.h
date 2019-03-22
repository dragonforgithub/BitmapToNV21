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
 * amimgerror.h
 *
 * This file has defined error codes for imaging API. 
 * 
 * 
 *
 */

#ifndef _AMIMGERROR_H_
#define _AMIMGERROR_H_

#define MERR_IMAGE_BASE					0X080000


#define MERR_COMPONENT_NOTFOUND			(MERR_IMAGE_BASE)
#define MERR_NOT_IMPLEMENTED			(MERR_IMAGE_BASE+1) 
#define MERR_NO_MORE					(MERR_IMAGE_BASE+2) 
#define MERR_NOT_READY					(MERR_IMAGE_BASE+3)
#define MERR_SAME_STATE					(MERR_IMAGE_BASE+4)
#define MERR_OUT_OF_RANGE					(MERR_IMAGE_BASE+5)
#define MERR_VERSION_MISMATCH			(MERR_IMAGE_BASE+6)
#define MERR_REACH_LIMIT				(MERR_IMAGE_BASE+7)
#define MERR_ITEM_NO_EXIST			(MERR_IMAGE_BASE+8)
#define MERR_DECODE_FAILED			(MERR_IMAGE_BASE+9)


#endif	//#ifndef _AMIMGERROR_H_

