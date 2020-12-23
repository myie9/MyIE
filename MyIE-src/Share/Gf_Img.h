#pragma once

class CGf_Img
{
public:
	/************************************************************************/
	/* 不做拉伸，可设透明cTransparentColor                                                                     */
	/************************************************************************/
	static void MyTransparentBlt(CDC*& pDC, int   xStart,int   yStart, int w, int h,
							   CDC& dcTemp, int sx, int sy, 
							   COLORREF   cTransparentColor);
	static void LongWidth(CDC* pDC, const HBITMAP& hmp, const CRect& rOri, BOOL bTop, int yOff, int FIRST_DRAW, int MID_DRAW, COLORREF picMask);
	static void Bkground(CDC* pDC, const HBITMAP& hmp, const CRect& rTemp, int ratio, UINT picMask );


};