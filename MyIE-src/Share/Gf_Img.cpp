#include "stdafx.h"
#include "Gf_Img.h"

//TransparentBlt不能用，98下内存泄漏的厉害

/*
以拼接方式画背景
ratio://从3倍高度处截取背景图作为rTemp的背景

  picMask目前无用
*/
void CGf_Img::Bkground(CDC* pDC, const HBITMAP& hmp, const CRect& rTemp, int ratio, UINT picMask )
{
	if (NULL == hmp)
	{
		return;
	}
	
	BITMAP bmInfo;
	VERIFY(::GetObject(hmp, sizeof(BITMAP), &bmInfo));
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	HGDIOBJ pOld = dcMem.SelectObject(hmp);
	
	
	int bmHeight = bmInfo.bmHeight;
	
	int ybmp = bmInfo.bmHeight - ratio * rTemp.Height(); //从3倍处截取背景图作为tab的背景
	if (ybmp > 0)
	{			
		bmHeight = rTemp.Height();
		
	}
	else
	{
		ybmp = 0;
		
	}
	
	
	for (int y=0; y<rTemp.Height(); y+=bmInfo.bmHeight)
	{
		for (int x=0; x<rTemp.Width(); x+= bmInfo.bmWidth)
		{				
			
			//				TransparentBlt(pDC->GetSafeHdc(), rTemp.left + x, rTemp.top+y, bmInfo.bmWidth, bmHeight,  \
			// 					dcMem.GetSafeHdc(), 0, ybmp, bmInfo.bmWidth, bmHeight, picMask);
			
			BitBlt(pDC->GetSafeHdc(), rTemp.left + x, rTemp.top+y, bmInfo.bmWidth, bmHeight,  \
				dcMem.GetSafeHdc(), 0, ybmp, SRCCOPY);
		}
	}
	
	
	dcMem.SelectObject(pOld);
	dcMem.DeleteDC();
	
}

//扩展图片长度. 用于tab背景
void CGf_Img::LongWidth(CDC* pDC, const HBITMAP& hmp, const CRect& rOri, BOOL bTop, int yOff, int FIRST_DRAW, int MID_DRAW, COLORREF picMask )
{
	if (!hmp)
	{
		return;
	}
	
	
	BITMAP bmInfo;
	VERIFY(::GetObject(hmp, sizeof(BITMAP), &bmInfo));
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	//CBitmap* pOld = (CBitmap*)dcMem.SelectObject(hmp);
	HGDIOBJ pOld = dcMem.SelectObject(hmp);
	
	int yStart = yOff;
	if (bTop)
	{
		yStart = rOri.bottom - bmInfo.bmHeight + yOff;
	}
	
	if ( (rOri.Width() == bmInfo.bmWidth) || (rOri.Width() < 2* FIRST_DRAW )) //扩大会使点击tab反映慢
	{
		MyTransparentBlt(pDC, rOri.left, yStart, rOri.Width(), bmInfo.bmHeight,  \
			dcMem, 0, 0,  picMask);
		
	}
	else
	{
		MyTransparentBlt(pDC, rOri.left, yStart, FIRST_DRAW, bmInfo.bmHeight,  \
			dcMem, 0, 0, picMask);
		
		int paintedWidth = FIRST_DRAW;
		
		for (int inc = MID_DRAW; paintedWidth<(rOri.Width()- FIRST_DRAW); paintedWidth+=inc)
		{
			int leftNotPaint= rOri.Width()-paintedWidth- FIRST_DRAW; //剩余未画背景的长度
			
			if (leftNotPaint< MID_DRAW)
			{
				inc=leftNotPaint;
			}
			else
			{
				inc = MID_DRAW;
			}
			
			MyTransparentBlt(pDC, rOri.left + paintedWidth, yStart, inc, bmInfo.bmHeight,  \
				dcMem, FIRST_DRAW, 0,  picMask);
			
		}
		
		
		MyTransparentBlt(pDC, rOri.left + paintedWidth, yStart, FIRST_DRAW, bmInfo.bmHeight,  \
			dcMem, bmInfo.bmWidth- FIRST_DRAW, 0, picMask);
		
	}
	
//gf?第二次会异常	
	dcMem.SelectObject(pOld);
	dcMem.DeleteDC();
}

/************************************************************************/
/* dcTemp:源图
pDC：目标
/************************************************************************/
void CGf_Img::MyTransparentBlt(CDC*& pDC, int   xStart,int   yStart, int w, int h,
							   CDC& dcTemp, int sx, int sy, 
							   COLORREF   cTransparentColor)   
{   
	//gf BITMAP           bm;   
	COLORREF       cColor;   
	CBitmap         bmAndBack,   bmAndObject,   bmAndMem,   bmSave;   
	//CBitmap         *pbmBackOld,   *pbmObjectOld,   *pbmMemOld,   *pbmSaveOld;   
	HGDIOBJ  pbmBackOld,  pbmObjectOld, pbmMemOld, pbmSaveOld;
	CDC                 dcMem,   dcBack,   dcObject,     dcSave;   
	CPoint           ptSize;   
    
	//dcTemp.CreateCompatibleDC(pDC);   
	//dcTemp.SelectObject(pBitmap);       //   Select   the   bitmap   
    
	//gf pBitmap->GetObject(sizeof(BITMAP),(LPSTR)&bm);   
    
	ptSize.x   =   w;             //   Get   width   of   bitmap   
	ptSize.y   =   h;           //   Get   height   of   bitmap   
	//gf dcTemp.DPtoLP(&ptSize,1);       //   Convert   from   device   to   logical   points   
    
	//   Create   some   DCs   to   hold   temporary   data.   
	dcBack.CreateCompatibleDC(pDC);   
	dcObject.CreateCompatibleDC(pDC);   
	dcMem.CreateCompatibleDC(pDC);   
	dcSave.CreateCompatibleDC(pDC);   
    
	//   Create   a   bitmap   for   each   DC.   DCs   are   required   for   a   number   of   
	//   GDI   functions.   
    
	//   Monochrome   DC   
	bmAndBack.CreateBitmap(ptSize.x,ptSize.y,1,1,NULL);   
    
	//   Monochrome   DC   
	bmAndObject.CreateBitmap(ptSize.x,ptSize.y,1,1,NULL);   
    
	bmAndMem.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y);   
	bmSave.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y);   
    
	//   Each   DC   must   select   a   bitmap   object   to   store   pixel   data.   
	pbmBackOld   =   dcBack.SelectObject(&bmAndBack);   
	pbmObjectOld   =   dcObject.SelectObject(&bmAndObject);   
	pbmMemOld   =   dcMem.SelectObject(&bmAndMem);   
	pbmSaveOld   =   dcSave.SelectObject(&bmSave);   
    
	//   Set   proper   mapping   mode.   
	dcTemp.SetMapMode(pDC->GetMapMode());   
    
	//   Save   the   bitmap   sent   here,   because   it   will   be   overwritten.   
	dcSave.BitBlt(0,0,ptSize.x,ptSize.y,&dcTemp,sx,sy,SRCCOPY);   
    
	//   Set   the   background   color   of   the   source   DC   to   the   color.   
	//   contained   in   the   parts   of   the   bitmap   that   should   be   transparent   
	cColor   =   dcTemp.SetBkColor(cTransparentColor);   
    
	//   Create   the   object   mask   for   the   bitmap   by   performing   a   BitBlt()   
	//   from   the   source   bitmap   to   a   monochrome   bitmap.   
	dcObject.BitBlt(0,0,ptSize.x,ptSize.y,&dcTemp,sx,sy,SRCCOPY);   
    
	//   Set   the   background   color   of   the   source   DC   back   to   the   original   color.   
	dcTemp.SetBkColor(cColor);   
    
	//   Create   the   inverse   of   the   object   mask.   
	dcBack.BitBlt(0,0,ptSize.x,ptSize.y,&dcObject,0,0,NOTSRCCOPY);   
    
	//   Copy   the   background   of   the   main   DC   to   the   destination.   
	dcMem.BitBlt(0,0,ptSize.x,ptSize.y,pDC,xStart,yStart,SRCCOPY);   
    
	//   Mask   out   the   places   where   the   bitmap   will   be   placed.   
	dcMem.BitBlt(0,0,ptSize.x,ptSize.y,&dcObject,0,0,SRCAND);   
    
	//   Mask   out   the   transparent   colored   pixels   on   the   bitmap.   
	dcTemp.BitBlt(sx,sy,ptSize.x,ptSize.y,&dcBack,0,0,SRCAND);   
    
	//   XOR   the   bitmap   with   the   background   on   the   destination   DC.   
	dcMem.BitBlt(0,0,ptSize.x,ptSize.y,&dcTemp,sx,sy,SRCPAINT);   
    
	//   Copy   the   destination   to   the   screen.   
	pDC->BitBlt(xStart,yStart,ptSize.x,ptSize.y,&dcMem,0,0,SRCCOPY);   
    
	//   Place   the   original   bitmap   back   into   the   bitmap   sent   here.   
	dcTemp.BitBlt(sx,sy,ptSize.x,ptSize.y,&dcSave,0,0,SRCCOPY);   
    
	//   Delete   the   memory   bitmaps.   
// 	(dcBack.SelectObject(pbmBackOld))->DeleteObject();   
// 	(dcObject.SelectObject(pbmObjectOld))->DeleteObject();   
// 	(dcMem.SelectObject(pbmMemOld))->DeleteObject();   
// 	(dcSave.SelectObject(pbmSaveOld))->DeleteObject();   
	dcBack.SelectObject(pbmBackOld);
	 
	 	dcObject.SelectObject(pbmObjectOld);   
		dcMem.SelectObject(pbmMemOld);   
		dcSave.SelectObject(pbmSaveOld);   

		bmAndBack.DeleteObject();   
		   bmAndObject.DeleteObject();   
		   bmAndMem.DeleteObject();   
		   bmSave.DeleteObject();      

    
	//   Delete   the   memory   DCs.   
	dcMem.DeleteDC();   
	dcBack.DeleteDC();   
    
	dcObject.DeleteDC();   
	dcSave.DeleteDC();   
	//gf dcTemp.DeleteDC();   
}