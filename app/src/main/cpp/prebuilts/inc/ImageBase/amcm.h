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
 * amcm.h
 *
 * Component Manager definition uses C language syntax for easy-to-porting proposes. 
 * But it is object based and fully inheritable and extensible.
 * 
 * 
 */

#ifndef _AMCM_H_
#define _AMCM_H_

#include "merror.h"	

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
	
#define AMCM_BEGIN_REGISTER(hCMgr) \
{ \
	MHandle __hCMgr = (hCMgr)

#define AMCM_END_REGISTER() }

////////////////////////////////////////////////////////////////////////

typedef MRESULT(*AM_FNCREATECOMPONENT) (MHandle hCMgr, MHandle* phComponent);

// APIs definition

MRESULT	AMCM_Create(MHandle hMemContext, MHandle* phCMgr); 

MRESULT AMCM_Destroy(MHandle hCMgr); 

MRESULT AMCM_RegisterEx(MHandle hCMgr, MDWord uid, 
						MDWord dwMainType, 
						MDWord dwSubType, 
						MDWord dwPriority, 
						AM_FNCREATECOMPONENT fnCreate);	

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif	  

