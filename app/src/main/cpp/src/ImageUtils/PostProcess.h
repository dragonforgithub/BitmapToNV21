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
* PostProcess.h
*
* Reference:
*
* Description:
*
*/

#ifndef POSTPROCESS_H_
#define POSTPROCESS_H_

#include "amdisplay.h"
#include "asvloffscreen.h"
#include "ampostprocess.h"

#define  DECTECT_SCALE  22

MRESULT MBitmapAlloc(MBITMAP& bitmap);
MRESULT MBitmapFree(MBITMAP& bitmap);
MRESULT MBitmapSize(MBITMAP& bitmap);

class CPostProcess
{
public:
	CPostProcess();
	~CPostProcess();

public:
	MVoid SetReSizeAlg(MDWord dwResizeAlg)				{ m_dwReSizeAlg = dwResizeAlg; }
	MVoid SetAspectRatio(MDWord dwAspectRatio)			{ m_dwAspectRatio = dwAspectRatio; }
	MVoid SetProcessorType(MDWord dwProcessorType)		{ m_dwProcessorType = dwProcessorType; }
	MVoid SetPerformanceMode(MDWord dwPerformanceMode)	{ m_dwPerformanceMode = dwPerformanceMode; }
	MVoid SetDither(MDWord dwDither)					{ m_dwDither = dwDither; }
	MVoid SetAlpha(MDWord dwAlpha)						{ m_dwAlpha = dwAlpha; }
	MVoid SetDirection(MDWord dwDirection)				{ m_dwDirection = dwDirection; }

	MRESULT DoPostProcess(LPMBITMAP pSrcBitmap, LPMBITMAP pDstBitmap);
	MRESULT DoPostProcess(LPASVLOFFSCREEN pSrcOffScreen, LPASVLOFFSCREEN pDstOffScreen);
	MRESULT DoPostProcess(LPMBITMAP pSrcBitmap, LPASVLOFFSCREEN pDstOffScreen);
	MRESULT DoPostProcess(LPASVLOFFSCREEN pSrcOffScreen, LPMBITMAP pDstBitmap);
	MRESULT MdUtilsFormatColorIDEx(MDWord dwColorID,MDWord *dwSX, MDWord *dwSY, MDWord* dwBitCount,MDWord *dwPlanes, MDWord *dwXPicth);

	MLong BuildASVLOFFSCREEN(MBITMAP& bitmap, ASVLOFFSCREEN &offscreen);
	MLong BuildASVLOFFSCREEN(LPMBITMAP lpBitmap, LPASVLOFFSCREEN lpOffscreen);
	MLong BuildMBITMAP(ASVLOFFSCREEN &offscreen, MBITMAP& bitmap);
    MLong BuildMBITMAP(LPASVLOFFSCREEN lpOffscreen,LPMBITMAP lpBitmap);

protected:
	MRESULT checkFormat(MDWord dwPixelFormat);

private:
	MDWord		m_dwReSizeAlg;
	MDWord		m_dwAspectRatio;
	MDWord		m_dwProcessorType;
	MDWord		m_dwPerformanceMode;
	MDWord		m_dwDither;
	MDWord		m_dwAlpha;
	MDWord		m_dwDirection;
};

#endif /* POSTPROCESS_H_ */
