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
 * amcmerrdef.h
 *
 * Description:
 *
 *	The error code defined for ArcSoft Mobile Component Manager Library.
 *
 */



#ifndef _AMCMERRDEF_H_
#define _AMCMERRDEF_H_

#include "merror.h"

#define AMCM_IS_AMCM_ERROR(rt) (MERR_AMCM_BASE <= (rt) && MERR_AMCM_MAX >= (rt))

#define MERR_AMCM_BASE					0X40000
#define MERR_AMCM_GENERAL						MERR_AMCM_BASE
#define MERR_AMCM_GLOBALDATA_NOTEXISTS			(MERR_AMCM_BASE + 1)
#define MERR_AMCM_COMPONENT_ID_ALREADYEXISTS	(MERR_AMCM_BASE + 2)
#define MERR_AMCM_COMPONENT_ID_NOTEXISTS		(MERR_AMCM_BASE + 3)
#define MERR_AMCM_COMPONENT_ENUMID_REACHED_TAIL	(MERR_AMCM_BASE + 4)
#define MERR_AMCM_COMPONENT_ENUMID_OVER_RANGE	(MERR_AMCM_BASE + 5)
#define MERR_AMCM_COMPONENT_ENUMID_OLD_HANDLE	(MERR_AMCM_BASE + 6)
#define MERR_AMCM_MAX					0X40FFF


#endif	/*_AMCMERRDEF_H_ */
 
