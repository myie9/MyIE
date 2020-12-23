#include "stdafx.h"
#include "ToolMenu.h"
#include "afxpriv.h"
#include "MenuData.h"
//
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//##############################################################
#define		MENU_SPACETEXT			1//10
#define		MENU_TOPFACTORS			0//2
#define		MENU_LEFTFACTORS		0//4
#define		MENU_RIGHTFACTORS		0//4
#define		MENU_BOTTOMFACTORS		0//2

#define GAP		1

//##############################################################
int m_selectcheck = -1;
int m_iconX=16;
int m_iconY=16;
CImageList *checkmaps=NULL;
int m_unselectcheck = -1;
int disable_old_style=FALSE;
int original_select_disabled=TRUE;
int m_nMenuColorMode=1;

//##############################################################

struct CMenuItemInfo : public 
#ifndef UNICODE   //SK: this fixes warning C4097: typedef-name 'MENUITEMINFO' used as synonym for class-name 'tagMENUITEMINFOA'
	tagMENUITEMINFOA
#else
	tagMENUITEMINFOW
#endif
	{
		CMenuItemInfo()
		{
			memset(this, 0, sizeof(MENUITEMINFO));
			cbSize = sizeof(MENUITEMINFO);
		}
	};

CToolMenu::CToolMenu()
{
}

CToolMenu::~CToolMenu()
{
	try{
	DestroyMenuData(m_hMenu,0);
	}catch(...){}
}

enum Win32Type{
	Win32s,
	Windoze95,
	WinNT3,
	WinNT4orHigher
};


Win32Type IsShellType()
{
	Win32Type  ShellType;
	DWORD winVer;
	OSVERSIONINFO *osvi;
	
	winVer=GetVersion();
	if(winVer<0x80000000){/*NT */
		ShellType=WinNT3;
		osvi= (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi!=NULL){
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);
			if (osvi->dwMajorVersion>=4L)
				ShellType=WinNT4orHigher;//yup, it is NT 4 or higher!
			free(osvi);
		}
	}
	else if  (LOBYTE(LOWORD(winVer))<4)
		ShellType=Win32s;/*Win32s*/
	else
		ShellType=Windoze95;/*Windoze95*/
	return ShellType;
}

static Win32Type g_Shell=IsShellType();

BOOL IsNewShell ()
{
	return (Windoze95==g_Shell || WinNT4orHigher==g_Shell);
}

COLORREF LightenColor(COLORREF col,double factor)
{
	if(factor>0.0&&factor<=1.0){
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(col);
		green = GetGValue(col);
		blue = GetBValue(col);
		lightred = (BYTE)((factor*(255-red)) + red);
		lightgreen = (BYTE)((factor*(255-green)) + green);
		lightblue = (BYTE)((factor*(255-blue)) + blue);
		col = RGB(lightred,lightgreen,lightblue);
	}
	return(col);
}

void DitherBlt (HDC hdcDest, int nXDest, int nYDest, int nWidth, 
                        int nHeight, HBITMAP hbm, int nXSrc, int nYSrc,
						COLORREF bgcolor)
{
	ASSERT(hdcDest && hbm);
	ASSERT(nWidth > 0 && nHeight > 0);
	
	// Create a generic DC for all BitBlts
	HDC hDC = CreateCompatibleDC(hdcDest);
	ASSERT(hDC);
	
	if (hDC)
	{
		// Create a DC for the monochrome DIB section
		HDC bwDC = CreateCompatibleDC(hDC);
		ASSERT(bwDC);
		
		if (bwDC)
		{
			// Create the monochrome DIB section with a black and white palette
			struct {
				BITMAPINFOHEADER bmiHeader; 
				RGBQUAD      bmiColors[2]; 
			} RGBBWBITMAPINFO = {
				
				{    // a BITMAPINFOHEADER
					sizeof(BITMAPINFOHEADER),  // biSize 
						nWidth,         // biWidth; 
						nHeight,        // biHeight; 
						1,            // biPlanes; 
						1,            // biBitCount 
						BI_RGB,         // biCompression; 
						0,            // biSizeImage; 
						0,            // biXPelsPerMeter; 
						0,            // biYPelsPerMeter; 
						0,            // biClrUsed; 
						0            // biClrImportant; 
				},    
				{
					{ 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }
					} 
			};
			VOID *pbitsBW;
			HBITMAP hbmBW = CreateDIBSection(bwDC,
				(LPBITMAPINFO)&RGBBWBITMAPINFO, DIB_RGB_COLORS, &pbitsBW, NULL, 0);
			ASSERT(hbmBW);
			
			if (hbmBW)
			{
				// Attach the monochrome DIB section and the bitmap to the DCs
				//gf HBITMAP olddib = (HBITMAP)SelectObject(bwDC, hbmBW);
				HGDIOBJ olddib = SelectObject(bwDC, hbmBW);
				HGDIOBJ hdcolddib = SelectObject(hDC, hbm);
				
				// BitBlt the bitmap into the monochrome DIB section
				BitBlt(bwDC, 0, 0, nWidth, nHeight, hDC, nXSrc, nYSrc, SRCCOPY);
				
				// Paint the destination rectangle in gray
				FillRect(hdcDest, CRect(nXDest, nYDest, nXDest + nWidth, nYDest +
					nHeight), GetSysColorBrush(bgcolor));
				//SK: looks better on the old shell
				// BitBlt the black bits in the monochrome bitmap into COLOR_3DHILIGHT
				// bits in the destination DC
				// The magic ROP comes from the Charles Petzold's book
				HBRUSH hb = CreateSolidBrush(GetSysColor(COLOR_3DHILIGHT));
				//HBRUSH oldBrush = (HBRUSH)SelectObject(hdcDest, hb);
				HGDIOBJ oldBrush = SelectObject(hdcDest, hb);
				BitBlt(hdcDest,nXDest+1,nYDest+1,nWidth,nHeight,bwDC,0,0,0xB8074A);
				
				// BitBlt the black bits in the monochrome bitmap into COLOR_3DSHADOW
				// bits in the destination DC
				hb = CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
				DeleteObject(SelectObject(hdcDest, hb));
				BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight,bwDC,0,0,0xB8074A);
				DeleteObject(SelectObject(hdcDest, oldBrush));
				VERIFY(DeleteObject(SelectObject(bwDC, olddib)));
				SelectObject(hDC, hdcolddib);
			}
			VERIFY(DeleteDC(bwDC));
		}
		VERIFY(DeleteDC(hDC));
	}
}

void DitherBlt2(CDC *drawdc, int nXDest, int nYDest, int nWidth, 
                        int nHeight, CBitmap &bmp, int nXSrc, int nYSrc,
						COLORREF bgcolor)
{
	// create a monochrome memory DC
	CDC ddc;
	ddc.CreateCompatibleDC(0);
	CBitmap bwbmp;
	bwbmp.CreateCompatibleBitmap(&ddc, nWidth, nHeight);
//	CBitmap * pddcOldBmp = ddc.SelectObject(&bwbmp);
		HGDIOBJ pddcOldBmp = ddc.SelectObject(&bwbmp);
	
	CDC dc;
	dc.CreateCompatibleDC(0);
	HGDIOBJ pdcOldBmp = dc.SelectObject(&bmp);
	
	// build a mask
	ddc.PatBlt(0, 0, nWidth, nHeight, WHITENESS);
	dc.SetBkColor(GetSysColor(COLOR_BTNFACE));
	ddc.BitBlt(0, 0, nWidth, nHeight, &dc, nXSrc,nYSrc, SRCCOPY);
	dc.SetBkColor(GetSysColor(COLOR_BTNHILIGHT));
	ddc.BitBlt(0, 0, nWidth, nHeight, &dc, nXSrc,nYSrc, SRCPAINT);
	
	// Copy the image from the toolbar into the memory DC
	// and draw it (grayed) back into the toolbar.
	dc.FillSolidRect(0,0, nWidth, nHeight, bgcolor);
	//SK: Looks better on the old shell
	dc.SetBkColor(RGB(0, 0, 0));
	dc.SetTextColor(RGB(255, 255, 255));
	CBrush brShadow, brHilight;
	brHilight.CreateSolidBrush(GetSysColor(COLOR_BTNHILIGHT));
	brShadow.CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
HGDIOBJ pOldBrush = dc.SelectObject(&brHilight);
	dc.BitBlt(0,0, nWidth, nHeight, &ddc, 0, 0, 0x00E20746L);
	drawdc->BitBlt(nXDest+1,nYDest+1,nWidth, nHeight, &dc,0,0,SRCCOPY);
	dc.BitBlt(1,1, nWidth, nHeight, &ddc, 0, 0, 0x00E20746L);
	dc.SelectObject(&brShadow);
	dc.BitBlt(0,0, nWidth, nHeight, &ddc, 0, 0, 0x00E20746L);
	drawdc->BitBlt(nXDest,nYDest,nWidth, nHeight, &dc,0,0,SRCCOPY);
	// reset DCs
	ddc.SelectObject(pddcOldBmp);
	ddc.DeleteDC();
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pdcOldBmp);
	dc.DeleteDC();
	
	brShadow.DeleteObject();
	brHilight.DeleteObject();
	bwbmp.DeleteObject();
}

BOOL GetBitmapFromImageList(CDC* pDC,CImageList *imglist,int nIndex,CBitmap &bmp)
{
	HICON hIcon = imglist->ExtractIcon(nIndex);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,m_iconX,m_iconY);
//	CBitmap* pOldBmp = dc.SelectObject(&bmp);
	HGDIOBJ pOldBmp = dc.SelectObject(&bmp);
	CBrush brush ;
	brush.CreateSolidBrush(GetSysColor(COLOR_MENU));
	::DrawIconEx(
		dc.GetSafeHdc(),
		0,
		0,
		hIcon,
		m_iconX,
		m_iconY,
		0,
		(HBRUSH)brush,
		DI_NORMAL
		);
	dc.SelectObject( pOldBmp );
	dc.DeleteDC();
	// the icon is not longer needed
	DestroyIcon(hIcon);
	return(TRUE);
}

void DrawCheckMark(CDC* pDC,int x,int y,COLORREF color)
{
	CPen m_penBack;
	m_penBack.CreatePen (PS_SOLID,0,color);
//	CPen *pOldPen = pDC->SelectObject (&m_penBack);
HGDIOBJ pOldPen = pDC->SelectObject (&m_penBack);
	pDC->MoveTo(x,y+2);
	pDC->LineTo(x,y+5);
	
	pDC->MoveTo(x+1,y+3);
	pDC->LineTo(x+1,y+6);
	
	pDC->MoveTo(x+2,y+4);
	pDC->LineTo(x+2,y+7);
	
	pDC->MoveTo(x+3,y+3);
	pDC->LineTo(x+3,y+6);
	
	pDC->MoveTo(x+4,y+2);
	pDC->LineTo(x+4,y+5);
	
	pDC->MoveTo(x+5,y+1);
	pDC->LineTo(x+5,y+4);
	
	pDC->MoveTo(x+6,y);
	pDC->LineTo(x+6,y+3);
	
	pDC->SelectObject (pOldPen);
	m_penBack.DeleteObject();
}

void DrawRadioDot(CDC *pDC,int x,int y,COLORREF color)
{
	CRect rcDot(x,y,x+6,y+6);
	CBrush brush;
	CPen pen;
	brush.CreateSolidBrush(color);
	pen.CreatePen(PS_SOLID,0,color);
//	CBrush *pOldBrush=pDC->SelectObject(&brush);
		HGDIOBJ pOldBrush=pDC->SelectObject(&brush);
	//CPen *pOldPen=pDC->SelectObject(&pen);
		HGDIOBJ pOldPen=pDC->SelectObject(&pen);
	pDC->Ellipse(&rcDot);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();
}

BOOL Draw3DCheckmark(CDC *dc, const CRect& rc,
                             BOOL bSelected, HBITMAP hbmCheck)
{
	CRect rcDest = rc;
	CBrush brush;
	//COLORREF col=GetSysColor((bSelected||!IsNewShell())?COLOR_MENU:COLOR_3DLIGHT);//SK: Looks better on the old shell
	COLORREF col;//
	if (bSelected||!IsNewShell())
		col=GetSysColor(COLOR_MENU);
	else
		col=RGB(233,233,233);
	brush.CreateSolidBrush(col);
	dc->FillRect(rcDest,&brush);
	brush.DeleteObject();
	if (IsNewShell()) //SK: looks better on the old shell
		dc->DrawEdge(&rcDest, BDR_SUNKENOUTER, BF_RECT);
	if (!hbmCheck)DrawCheckMark(dc,rc.left+7,rc.top+7,GetSysColor(COLOR_MENUTEXT));
	else DrawRadioDot(dc,rc.left+7,rc.top+7,GetSysColor(COLOR_MENUTEXT));
	return TRUE;
}

BOOL DrawXPCheckmark(CDC *dc, const CRect& rc, HBITMAP hbmCheck,COLORREF &colorout)
{
	CBrush brushin;
	brushin.CreateSolidBrush(LightenColor(colorout,0.85));
	dc->FillRect(rc,&brushin);
	brushin.DeleteObject();
	dc->Draw3dRect (rc,colorout,colorout);
	if (!hbmCheck)DrawCheckMark(dc,rc.left+5,rc.top+4,GetSysColor(COLOR_MENUTEXT));
	else DrawRadioDot(dc,rc.left+5,rc.top+4,GetSysColor(COLOR_MENUTEXT));
	return TRUE;
}

void DrawItem_Win9xNT2000 (LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS != NULL);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	//?
	CMenuData* pmd = (CMenuData*)(lpDIS->itemData);
	if(pmd->m_pImgList != NULL)
		::ImageList_GetIconSize(*(pmd->m_pImgList), &m_iconX, &m_iconY);
	else if (lpDIS->itemID!=0)//for fav bar not have image
	{
		m_iconX=16;
		m_iconY=16;
	}
	//
	if (m_iconX==11)//for tab icon 11*16
	{
		m_iconX=16;
		m_iconY=16;
	}
	//special,when SEPARATOR,itemState is 6
	if (lpDIS->itemID==0)//if (pmd->m_pImgList==NULL)
		lpDIS->itemState = MF_SEPARATOR;
	CRect rect;
	UINT state = lpDIS->itemState;//?(((BCMenuData*)(lpDIS->itemData))->nFlags);

	CBrush m_brBackground;
	m_brBackground.CreateSolidBrush(GetSysColor((IsNewShell())?COLOR_3DFACE:COLOR_MENU));

	// remove the selected bit if it's grayed out
	if(lpDIS->itemState & ODS_GRAYED&&!original_select_disabled)
	{
		if(lpDIS->itemState & ODS_SELECTED)
			lpDIS->itemState=lpDIS->itemState & ~ODS_SELECTED;
	}
	
	if(state & MF_SEPARATOR)
	{
		rect.CopyRect(&lpDIS->rcItem);
		pDC->FillRect (rect,&m_brBackground);
		rect.top += (rect.Height()>>1);
		pDC->DrawEdge(&rect,EDGE_ETCHED,BF_TOP);
	}
	else
	{
		CRect rect2;
		BOOL standardflag=FALSE,selectedflag=FALSE,disableflag=FALSE;
		BOOL checkflag=FALSE;
		COLORREF crText = GetSysColor(COLOR_MENUTEXT);
		COLORREF m_clrBack=GetSysColor((IsNewShell())?COLOR_3DFACE:COLOR_MENU);
		CBrush m_brSelect;
		CPen m_penBack;
		int x0,y0,dy;
		int nIconNormal=-1,xoffset=-1;
		CImageList *bitmap=NULL;
		CFont m_fontMenu;
		LOGFONT m_lf;
		
		// set some colors and the font
		m_penBack.CreatePen (PS_SOLID,0,GetSysColor((IsNewShell())?COLOR_3DFACE:COLOR_MENU));
		m_brSelect.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));
		NONCLIENTMETRICS nm;
		nm.cbSize = sizeof (NONCLIENTMETRICS);
		VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
		m_lf =  nm.lfMenuFont;
		m_fontMenu.CreateFontIndirect (&m_lf);
		
		// draw the colored rectangle portion
		rect.CopyRect(&lpDIS->rcItem);
		rect2=rect;
		
		// draw the up/down/focused/disabled state
		UINT state = lpDIS->itemState;
		CString strText;
		LOGFONT lf;
		lf = m_lf;
		
		CFont dispFont;
		//CFont *pFont=NULL;
		HGDIOBJ pFont=NULL;
		
		if(lpDIS->itemData != NULL)
		{
			nIconNormal = pmd->m_nBitmap;
			xoffset = pmd->m_nBitmap;
			bitmap = pmd->m_pImgList;
			strText = pmd->m_szMenuText;
			
			if(state&ODS_CHECKED && nIconNormal<0)
			{
				if(state&ODS_SELECTED && m_selectcheck>0)checkflag=TRUE;
				else if(m_unselectcheck>0) checkflag=TRUE;
			}
			else if(nIconNormal != -1)
			{
				standardflag=TRUE;
				if(state&ODS_SELECTED && !(state&ODS_GRAYED))selectedflag=TRUE;
				else if(state&ODS_GRAYED) disableflag=TRUE;
			}
		}
		else
		{
			strText.Empty();
		}
		
		if(state&ODS_SELECTED)
		{
			// draw the down edges			
//			CPen *pOldPen = pDC->SelectObject (&m_penBack);
HGDIOBJ pOldPen = pDC->SelectObject (&m_penBack);
			
			// You need only Text highlight and thats what you get
			if(checkflag||standardflag||selectedflag||disableflag||state&ODS_CHECKED)
				rect2.SetRect(rect.left+m_iconX+4+GAP,rect.top,rect.right,rect.bottom);
			pDC->FillRect (rect2,&m_brSelect);
			
			pDC->SelectObject (pOldPen);
			if((HFONT)dispFont != NULL)dispFont.DeleteObject ();
			dispFont.CreateFontIndirect (&lf);
			crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
		}
		else 
		{
//			CPen *pOldPen = pDC->SelectObject (&m_penBack);
HGDIOBJ pOldPen = pDC->SelectObject (&m_penBack);
			pDC->FillRect (rect,&m_brBackground);
			pDC->SelectObject (pOldPen);
			
			// draw the up edges			
			pDC->Draw3dRect (rect,m_clrBack,m_clrBack);
			if ((HFONT)dispFont != NULL) dispFont.DeleteObject ();
			dispFont.CreateFontIndirect (&lf); //Normal
		}
		
		// draw the text if there is any
		//We have to paint the text only if the image is nonexistant		
		dy = (rect.Height()-4-m_iconY)/2;
		dy = dy<0 ? 0 : dy;
		
		if(checkflag||standardflag||selectedflag||disableflag)
		{
			rect2.SetRect(rect.left+1,rect.top+1+dy,rect.left+m_iconX+3,
				rect.top+m_iconY+3+dy);
			pDC->Draw3dRect (rect2,m_clrBack,m_clrBack);
			if(checkflag && checkmaps)
			{
				pDC->FillRect (rect2,&m_brBackground);
				rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
					rect.top+m_iconY+4+dy);
				
				pDC->Draw3dRect (rect2,m_clrBack,m_clrBack);
				CPoint ptImage(rect.left+2,rect.top+2+dy);
				
				if(state&ODS_SELECTED)checkmaps->Draw(pDC,1,ptImage,ILD_TRANSPARENT);
				else checkmaps->Draw(pDC,0,ptImage,ILD_TRANSPARENT);
			}
			else if(disableflag)
			{
				if(!selectedflag)
				{
					CBitmap bitmapstandard;
					GetBitmapFromImageList(pDC,bitmap,xoffset,bitmapstandard);
					rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
                        rect.top+m_iconY+4+dy);
					pDC->Draw3dRect (rect2,m_clrBack,m_clrBack);
					if(disable_old_style)
						DitherBlt(lpDIS->hDC,rect.left+2,rect.top+2+dy,m_iconX,m_iconY,
						(HBITMAP)(bitmapstandard),0,0,m_clrBack);
					else
						DitherBlt2(pDC,rect.left+2,rect.top+2+dy,m_iconX,m_iconY,
						bitmapstandard,0,0,m_clrBack);
					bitmapstandard.DeleteObject();
				}
			}
			else if(selectedflag)
			{
				pDC->FillRect (rect2,&m_brBackground);
				rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
					rect.top+m_iconY+4+dy);
				if (IsNewShell())
				{
					if(state&ODS_CHECKED)
						pDC->Draw3dRect(rect2,GetSysColor(COLOR_3DSHADOW),
						GetSysColor(COLOR_3DHILIGHT));
					else
						pDC->Draw3dRect(rect2,GetSysColor(COLOR_3DHILIGHT),
						GetSysColor(COLOR_3DSHADOW));
				}
				//?
				int cx,cy;
				::ImageList_GetIconSize(*bitmap, &cx, &cy);
				CPoint ptImage(rect.left+2+(m_iconX-cx)/2,rect.top+2+dy);
				if(bitmap)bitmap->Draw(pDC,xoffset,ptImage,ILD_TRANSPARENT);
			}
			else
			{
				if(state&ODS_CHECKED)
				{
					CBrush brush;
					COLORREF col =RGB(233,233,233);//GetSysColor(COLOR_3DLIGHT);
					brush.CreateSolidBrush(col);
					pDC->FillRect(rect2,&brush);
					brush.DeleteObject();
					rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
                        rect.top+m_iconY+4+dy);
					if (IsNewShell())
						pDC->Draw3dRect(rect2,GetSysColor(COLOR_3DSHADOW),
						GetSysColor(COLOR_3DHILIGHT));
				}
				else{
					pDC->FillRect (rect2,&m_brBackground);
					rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
                        rect.top+m_iconY+4+dy);
					pDC->Draw3dRect (rect2,m_clrBack,m_clrBack);
				}
				//?
				int cx,cy;
				::ImageList_GetIconSize(*bitmap, &cx, &cy);
				CPoint ptImage(rect.left+2+(m_iconX-cx)/2,rect.top+2+dy);
				if(bitmap)bitmap->Draw(pDC,xoffset,ptImage,ILD_TRANSPARENT);
			}
		}
		if(nIconNormal<0 && state&ODS_CHECKED && !checkflag)
		{
			rect2.SetRect(rect.left,rect.top,rect.left+20,rect.top+20);
			CMenuItemInfo info;
			info.fMask = MIIM_CHECKMARKS;
			::GetMenuItemInfo((HMENU)lpDIS->hwndItem,lpDIS->itemID,
				MF_BYCOMMAND, &info);
			if(state&ODS_CHECKED || info.hbmpUnchecked) 
			{
				Draw3DCheckmark(pDC, rect2, state&ODS_SELECTED,
					state&ODS_CHECKED ? info.hbmpChecked :
				info.hbmpUnchecked);
			}
		}
		
		//This is needed always so that we can have the space for check marks
		x0=rect.left;y0=rect.top;
		rect.left = rect.left + m_iconX + 8 + GAP; 
		
		if(!strText.IsEmpty())
		{
			CRect rectt(rect.left,rect.top-1,rect.right,rect.bottom-1);
			//   Find tabs
			CString leftStr,rightStr;
			leftStr.Empty();rightStr.Empty();
			int tablocr=strText.ReverseFind(_T('\t'));
			if(tablocr!=-1){
				rightStr=strText.Mid(tablocr+1);
				leftStr=strText.Left(strText.Find(_T('\t')));
				rectt.right-=m_iconX;
			}
			else leftStr=strText;
			
			int iOldMode = pDC->GetBkMode();
			pDC->SetBkMode( TRANSPARENT);
			
			// Draw the text in the correct colour:
			UINT nFormat  = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
			UINT nFormatr = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;
			if(!(lpDIS->itemState & ODS_GRAYED))
			{
				pDC->SetTextColor(crText);
				pDC->DrawText (leftStr,rectt,nFormat);
				if(tablocr!=-1) pDC->DrawText (rightStr,rectt,nFormatr);
			}
			else
			{	
				// Draw the disabled text
				if(!(state & ODS_SELECTED))
				{
					RECT offset = *rectt;
					offset.left+=1;
					offset.right+=1;
					offset.top+=1;
					offset.bottom+=1;
					pDC->SetTextColor(GetSysColor(COLOR_BTNHILIGHT));
					pDC->DrawText(leftStr,&offset, nFormat);
					if(tablocr!=-1) pDC->DrawText (rightStr,&offset,nFormatr);
					pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
					pDC->DrawText(leftStr,rectt, nFormat);
					if(tablocr!=-1) pDC->DrawText (rightStr,rectt,nFormatr);
				}
				else
				{
					// And the standard Grey text:
					pDC->SetTextColor(m_clrBack);
					pDC->DrawText(leftStr,rectt, nFormat);
					if(tablocr!=-1) pDC->DrawText (rightStr,rectt,nFormatr);
				}
			}
			pFont = pDC->SelectObject (&dispFont);
			pDC->SetBkMode( iOldMode );
			pDC->SelectObject (pFont); //set it to the old font
		}
		
		m_penBack.DeleteObject();
		m_fontMenu.DeleteObject();
		m_brSelect.DeleteObject();
		dispFont.DeleteObject ();
	}
	m_brBackground.DeleteObject();
}

void DrawItem_WinXP (LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS != NULL);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
#ifdef BCMENU_USE_MEMDC
	BCMenuMemDC *pMemDC=NULL;
#endif

	CMenuData* pmd = (CMenuData*)(lpDIS->itemData);
	if(pmd->m_pImgList != NULL)
		::ImageList_GetIconSize(*(pmd->m_pImgList), &m_iconX, &m_iconY);
	else if (lpDIS->itemID!=0)//for fav bar not have image
	{
		m_iconX=16;
		m_iconY=16;
	}
	//
	if (m_iconX==11)//for tab icon 11*16
	{
		m_iconX=16;
		m_iconY=16;
	}
	//
	CRect rect,rect2;
	//special,when SEPARATOR,itemState is 6
	if (lpDIS->itemID==0)//if (pmd->m_pImgList==NULL)
		lpDIS->itemState = MF_SEPARATOR;
	UINT state = lpDIS->itemState;//?(((BCMenuData*)(lpDIS->itemData))->nFlags);
	COLORREF m_newclrBack=GetSysColor((IsNewShell())?COLOR_3DFACE:COLOR_MENU);
	COLORREF m_clrBack=GetSysColor(COLOR_MENU);
	BOOL XPflag = m_newclrBack==m_clrBack ? FALSE : TRUE;
	if(!XPflag)
	{
		if (m_nMenuColorMode==1)
		{
			m_newclrBack=LightenColor(m_newclrBack,0.35);
			m_clrBack=RGB(255,255,255);//RGB(212,208,200)
		}
		else if (m_nMenuColorMode==2)
		{
			m_newclrBack=LightenColor(m_newclrBack,0.1);
			m_clrBack=RGB(255,128,255);//RGB(212,208,200)
		}
		else if (m_nMenuColorMode==3)
		{
			m_newclrBack=LightenColor(m_newclrBack,0.9);
			m_clrBack=RGB(255,255,128);//RGB(212,208,200)
		}
	}
	CBrush m_newbrBackground,m_brBackground;
	m_brBackground.CreateSolidBrush(m_clrBack);
	m_newbrBackground.CreateSolidBrush(m_newclrBack);
	
	// remove the selected bit if it's grayed out
	//if(lpDIS->itemState & ODS_GRAYED&&!xp_select_disabled){
	if(lpDIS->itemState & ODS_GRAYED&&!pmf->m_nMenuStyle){
		if(lpDIS->itemState & ODS_SELECTED)lpDIS->itemState=lpDIS->itemState & ~ODS_SELECTED;
#ifdef BCMENU_USE_MEMDC
		pMemDC=new BCMenuMemDC(pDC,&lpDIS->rcItem);
		pDC = pMemDC;
#endif
	}
	
	if(state & MF_SEPARATOR){
		rect.CopyRect(&lpDIS->rcItem);
		pDC->FillRect (rect,&m_brBackground);
		rect2.SetRect(rect.left,rect.top,rect.left+m_iconX+4+GAP,rect.bottom);
		rect.top+=rect.Height()>>1;
		rect.left = rect2.right+4;
		pDC->DrawEdge(&rect,EDGE_ETCHED,BF_TOP);
		pDC->FillRect (rect2,&m_newbrBackground);
		pDC->Draw3dRect (rect2,m_newclrBack,m_newclrBack);
	}
	else{
		BOOL standardflag=FALSE,selectedflag=FALSE,disableflag=FALSE;
		BOOL checkflag=FALSE;
		COLORREF crText = GetSysColor(COLOR_MENUTEXT);
		COLORREF crSelect = GetSysColor(COLOR_HIGHLIGHT);
		COLORREF crSelectFill;
		if(!XPflag)crSelectFill=LightenColor(crSelect,0.85);
		else crSelectFill=LightenColor(crSelect,0.7);
		CBrush m_brSelect;
		CPen m_penBack;
		int x0,y0,dy;
		int nIconNormal=-1,xoffset=-1;
		CImageList *bitmap=NULL;
		CFont m_fontMenu;
		LOGFONT m_lf;
		
		// set some colors and the font
		m_penBack.CreatePen (PS_SOLID,0,m_clrBack);
		m_brSelect.CreateSolidBrush(crSelectFill);
		ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));
		NONCLIENTMETRICS nm;
		nm.cbSize = sizeof (NONCLIENTMETRICS);
		VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
		m_lf =  nm.lfMenuFont;
		m_fontMenu.CreateFontIndirect (&m_lf);
		
		// draw the colored rectangle portion
		rect.CopyRect(&lpDIS->rcItem);
		rect2=rect;
		
		// draw the up/down/focused/disabled state
		UINT state = lpDIS->itemState;
		CString strText;
		LOGFONT lf;
		lf = m_lf;
		
		CFont dispFont;
		//CFont *pFont=NULL;
		HGDIOBJ pFont=NULL;
		
		if(lpDIS->itemData != NULL){
			nIconNormal = pmd->m_nBitmap;
			xoffset = pmd->m_nBitmap;
			bitmap = pmd->m_pImgList;
			strText = pmd->m_szMenuText;
			
			if(state&ODS_CHECKED && nIconNormal<0){
				if(state&ODS_SELECTED && m_selectcheck>0)checkflag=TRUE;
				else if(m_unselectcheck>0) checkflag=TRUE;
			}
			else if(nIconNormal != -1){
				standardflag=TRUE;
				if(state&ODS_SELECTED && !(state&ODS_GRAYED))selectedflag=TRUE;
				else if(state&ODS_GRAYED) disableflag=TRUE;
			}
		}
		else{
			strText.Empty();
		}
		
		if(state&ODS_SELECTED){ // draw the down edges
			
//			CPen *pOldPen = pDC->SelectObject (&m_penBack);
						HGDIOBJ pOldPen = pDC->SelectObject (&m_penBack);
			
			pDC->FillRect (rect,&m_brSelect);
			pDC->Draw3dRect (rect,crSelect,crSelect);
			
			pDC->SelectObject (pOldPen);
			if((HFONT)dispFont != NULL)dispFont.DeleteObject ();
			dispFont.CreateFontIndirect (&lf);
		}
		else {
			rect2.SetRect(rect.left,rect.top,rect.left+m_iconX+4+GAP,rect.bottom);
//			CPen *pOldPen = pDC->SelectObject (&m_penBack);
					HGDIOBJ pOldPen = pDC->SelectObject (&m_penBack);
			pDC->FillRect (rect,&m_brBackground);
			pDC->FillRect (rect2,&m_newbrBackground);
			pDC->SelectObject (pOldPen);
			
			// draw the up edges
			
			pDC->Draw3dRect (rect,m_clrBack,m_clrBack);
			pDC->Draw3dRect (rect2,m_newclrBack,m_newclrBack);
			if ((HFONT)dispFont != NULL) dispFont.DeleteObject ();
			dispFont.CreateFontIndirect (&lf); //Normal
		}
		
		// draw the text if there is any
		//We have to paint the text only if the image is nonexistant
		
		dy = (rect.Height()-4-m_iconY)/2;
		dy = dy<0 ? 0 : dy;
		
		if(checkflag||standardflag||selectedflag||disableflag){
			rect2.SetRect(rect.left+1,rect.top+1+dy,rect.left+m_iconX+3,
				rect.top+m_iconY+3+dy);
			pDC->Draw3dRect (rect2,m_newclrBack,m_newclrBack);
			if(checkflag && checkmaps){
				pDC->FillRect (rect2,&m_newbrBackground);
				rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
					rect.top+m_iconY+4+dy);
				
				pDC->Draw3dRect (rect2,m_newclrBack,m_newclrBack);
				CPoint ptImage(rect.left+3,rect.top+2+dy);
				
				if(state&ODS_SELECTED)checkmaps->Draw(pDC,1,ptImage,ILD_TRANSPARENT);
				else checkmaps->Draw(pDC,0,ptImage,ILD_TRANSPARENT);
			}
			else if(disableflag){
				if(!selectedflag){
					CBitmap bitmapstandard;
					GetBitmapFromImageList(pDC,bitmap,xoffset,bitmapstandard);
					rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
                        rect.top+m_iconY+4+dy);
					pDC->Draw3dRect (rect2,m_newclrBack,m_newclrBack);
					COLORREF transparentcol=m_newclrBack;
					if(state&ODS_SELECTED)transparentcol=crSelectFill;
					if(disable_old_style)
						DitherBlt(lpDIS->hDC,rect.left+2,rect.top+2+dy,m_iconX,m_iconY,
						(HBITMAP)(bitmapstandard),0,0,transparentcol);
					else
						DitherBlt2(pDC,rect.left+2,rect.top+2+dy,m_iconX,m_iconY,
						bitmapstandard,0,0,transparentcol);
					if(state&ODS_SELECTED)pDC->Draw3dRect (rect,crSelect,crSelect);
					bitmapstandard.DeleteObject();
				}
			}
			else if(selectedflag){
				pDC->FillRect (rect2,&m_brSelect);
				if (IsNewShell() && state&ODS_CHECKED){
					rect2.SetRect(rect.left,rect.top,rect.left+m_iconX+GAP+3,rect.bottom);
					pDC->Draw3dRect(rect2,crSelect,crSelect);
				}
				CPoint ptImage(rect.left+3,rect.top+2+dy);
				if(bitmap)bitmap->Draw(pDC,xoffset,ptImage,ILD_TRANSPARENT);
			}
			else{
				if(state&ODS_CHECKED){
					if(!XPflag)rect2.SetRect(rect.left+1,rect.top,rect.left+m_iconX+GAP+3,rect.bottom);
					else rect2.SetRect(rect.left+1,rect.top,rect.left+m_iconX+GAP+3,rect.bottom-1);
					CBrush brushin;
					brushin.CreateSolidBrush(LightenColor(crSelect,0.85));
					pDC->FillRect(rect2,&brushin);
					brushin.DeleteObject();
					pDC->Draw3dRect(rect2,crSelect,crSelect);
				}
				else{
					pDC->FillRect (rect2,&m_newbrBackground);
					rect2.SetRect(rect.left,rect.top+dy,rect.left+m_iconX+4,
                        rect.top+m_iconY+4+dy);
					pDC->Draw3dRect (rect2,m_newclrBack,m_newclrBack);
				}
				CPoint ptImage(rect.left+3,rect.top+2+dy);
				if(bitmap)bitmap->Draw(pDC,xoffset,ptImage,ILD_TRANSPARENT);
			}
		}
		if(nIconNormal<0 && state&ODS_CHECKED && !checkflag){
			CMenuItemInfo info;
			info.fMask = MIIM_CHECKMARKS;
			::GetMenuItemInfo((HMENU)lpDIS->hwndItem,lpDIS->itemID,
				MF_BYCOMMAND, &info);
			if(state&ODS_CHECKED || info.hbmpUnchecked) {
				if(XPflag)rect2.SetRect(rect.left+2,rect.top+2+dy,rect.left+m_iconX+2,rect.top+m_iconY+3+dy);
				else rect2.SetRect(rect.left+2,rect.top+2+dy,rect.left+m_iconX+2,rect.top+m_iconY+2+dy);
				DrawXPCheckmark(pDC, rect2,state&ODS_CHECKED ? info.hbmpChecked :
				info.hbmpUnchecked,crSelect);
			}
		}
		
		//This is needed always so that we can have the space for check marks
		
		x0=rect.left;y0=rect.top;
		rect.left = rect.left + m_iconX + 8 + GAP; 
		
		if(!strText.IsEmpty()){
			
			CRect rectt(rect.left,rect.top-1,rect.right,rect.bottom-1);
			
			//   Find tabs
			
			CString leftStr,rightStr;
			leftStr.Empty();rightStr.Empty();
			int tablocr=strText.ReverseFind(_T('\t'));
			if(tablocr!=-1){
				rightStr=strText.Mid(tablocr+1);
				leftStr=strText.Left(strText.Find(_T('\t')));
				rectt.right-=m_iconX;
			}
			else leftStr=strText;
			
			int iOldMode = pDC->GetBkMode();
			pDC->SetBkMode( TRANSPARENT);
			
			// Draw the text in the correct colour:
			
			pFont = pDC->SelectObject (&dispFont);
			UINT nFormat  = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
			UINT nFormatr = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;
			if(!(lpDIS->itemState & ODS_GRAYED)){
				pDC->SetTextColor(crText);
				pDC->DrawText (leftStr,rectt,nFormat);
				if(tablocr!=-1) pDC->DrawText (rightStr,rectt,nFormatr);
			}
			else{
				RECT offset = *rectt;
				offset.left+=1;
				offset.right+=1;
				offset.top+=1;
				offset.bottom+=1;
				if(!XPflag){
					COLORREF graycol=GetSysColor(COLOR_GRAYTEXT);
					graycol = LightenColor(graycol,0.55);
					pDC->SetTextColor(graycol);
				}
				else pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
				pDC->DrawText(leftStr,rectt, nFormat);
				if(tablocr!=-1) pDC->DrawText (rightStr,rectt,nFormatr);
			}
			pDC->SetBkMode( iOldMode );
			pDC->SelectObject (pFont); //set it to the old font
		}
		
		m_penBack.DeleteObject();
		m_fontMenu.DeleteObject();
		m_brSelect.DeleteObject();
		dispFont.DeleteObject ();
	}
	m_brBackground.DeleteObject();
	m_newbrBackground.DeleteObject();
#ifdef BCMENU_USE_MEMDC
	if(pMemDC)delete pMemDC;
#endif
}

void CToolMenu::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
	if (!pmf->m_nMenuStyle)
		DrawItem_Win9xNT2000(lpDIS);
	else
		DrawItem_WinXP(lpDIS);
}
#define GAP 1

void CToolMenu::MeasureItem( LPMEASUREITEMSTRUCT lpMIS ) 
{
	if(lpMIS == NULL)
		return;

	try
	{
	// get image size
	int cx=16, cy=16;//0;
	CMenuData* pmd = (CMenuData*)(lpMIS->itemData);
	if(pmd==NULL)
		return;

	if(pmd->m_pImgList != NULL)
		::ImageList_GetIconSize(*(pmd->m_pImgList), &cx, &cy);
	
    CFont m_fontMenu;
    LOGFONT m_lf;
    ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));
    NONCLIENTMETRICS nm;
    nm.cbSize = sizeof (NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
           nm.cbSize,&nm,0)); 
    m_lf =  nm.lfMenuFont;
    m_fontMenu.CreateFontIndirect (&m_lf);

    // Obtain the width of the text:
    CWnd *pWnd = AfxGetMainWnd();            // Get main window
    CDC *pDC = pWnd->GetDC();              // Get device context
    //CFont* pFont=NULL;    // Select menu font in...
    
    HGDIOBJ pFont = pDC->SelectObject (&m_fontMenu);// Select menu font in...
        
    //Get pointer to text SK
    const char *lpstrText = pmd->m_szMenuText;//SK: we use const to prevent misuse
    
        
    SIZE size={0,0};
	if(lpstrText!=NULL)
		::GetTextExtentPoint32(pDC->m_hDC,lpstrText,
             strlen(lpstrText),&size);

    CSize t = CSize(size);
    pDC->SelectObject (pFont);  // Select old font in
    AfxGetMainWnd()->ReleaseDC(pDC);  // Release the DC

    // Set width and height:
    lpMIS->itemWidth = cx + t.cx + cx + GAP;
	int temp = t.cy+4;
	if (lpMIS->itemID)//if(pmd->m_pImgList) 
		lpMIS->itemHeight = temp>cy+4 ? temp : cy+4;
	else
		lpMIS->itemHeight = 5;
    m_fontMenu.DeleteObject();
	
	}catch(...){}
}

BOOL DestroyMenuItemData(HMENU hMenu,int nPos, BOOL bPos)
{
	if(hMenu==NULL)
		return FALSE;
	if(!IsMenu(hMenu))
		return FALSE;
	try{
	MENUITEMINFO mii; 
	mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
	mii.dwTypeData = NULL;
	mii.cch = 0;
	mii.dwItemData = NULL;
	mii.cbSize = sizeof(mii);
	::GetMenuItemInfo(hMenu, nPos, bPos, &mii); 
	CMenuData* pmd = (CMenuData*)(mii.dwItemData);
	if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
	{
		if(pmd->m_szMenuText != NULL)
		{
			delete[] pmd->m_szMenuText;
			pmd->m_szMenuText = NULL;
		}
		if(pmd->m_szURL != NULL)
		{
			delete[] pmd->m_szURL;
			pmd->m_szURL = NULL;
		}
		delete pmd;
		mii.dwItemData = NULL;
		mii.fMask = MIIM_DATA;
		mii.cbSize = sizeof(mii);
		::SetMenuItemInfo(hMenu, nPos, bPos, &mii); 
	}
	}catch(...){}

	return true;
}

BOOL DestroyMenuData(HMENU hMenu,int nStart)
{
	if(hMenu==NULL)
		return FALSE;
	if(!IsMenu(hMenu))
		return FALSE;
	try{
	int iCount;
	MENUITEMINFO mii;
	CMenuData* pmd;
	for(iCount=0; iCount<::GetMenuItemCount(hMenu);iCount++)
	{
		mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
	    mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		::GetMenuItemInfo(hMenu, iCount, TRUE, &mii); 
		pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType & MFT_OWNERDRAW && pmd!=NULL && iCount>=nStart)
		{
			if(pmd->m_szMenuText != NULL)
			{
				delete[] pmd->m_szMenuText;
				pmd->m_szMenuText = NULL;
			}
			if(pmd->m_szURL != NULL)
			{
				delete[] pmd->m_szURL;
				pmd->m_szURL = NULL;
			}
			delete pmd;
			mii.dwItemData = NULL;
			mii.fMask = MIIM_DATA;
			mii.cbSize = sizeof(mii);
			::SetMenuItemInfo(hMenu, iCount, TRUE, &mii); 
			//
			HMENU hSubMenu = ::GetSubMenu(hMenu, iCount);
			if(IsMenu(hSubMenu))
			{
				DestroyMenuData(hSubMenu,0);
			}
		}
	}
	}catch(...){}

	return true;
}

BOOL DestroySubMenu(HMENU hMenu,HMENU hSubMenu)
{
	if(hMenu==NULL)
		return FALSE;
	if(!IsMenu(hMenu))
		return FALSE;
	try{
	int iCount,nItemCount;
	MENUITEMINFO mii;
	CMenuData* pmd;
	nItemCount = ::GetMenuItemCount(hMenu);
	for(iCount=0; iCount<nItemCount;iCount++)
	{
		mii.fMask = MIIM_DATA|MIIM_TYPE|MIIM_SUBMENU;    // get the state of the menu item
	    mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		::GetMenuItemInfo(hMenu, iCount, TRUE, &mii);
		pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType&MFT_OWNERDRAW && pmd && mii.hSubMenu)
		{
			if (mii.hSubMenu==hSubMenu)
			{
				//delete sub menu
				DestroyMenuData(hSubMenu,0);
				while(::DeleteMenu(hSubMenu,0, MF_BYPOSITION));
				//
				if(pmd->m_szMenuText != NULL)
				{
					delete[] pmd->m_szMenuText;
					pmd->m_szMenuText = NULL;
				}
				if(pmd->m_szURL != NULL)
				{
					delete[] pmd->m_szURL;
					pmd->m_szURL = NULL;
				}
				delete pmd;
				mii.dwItemData = NULL;
				mii.fMask = MIIM_DATA;
				mii.cbSize = sizeof(mii);
				::SetMenuItemInfo(hMenu, iCount, TRUE, &mii);
				//!! delete menu item
				DeleteMenu(hMenu, iCount, MF_BYPOSITION);
			}
			else
			{
				DestroySubMenu(mii.hSubMenu,hSubMenu);
			}
		}
	}
	}catch(...){}

	return true;
}
BOOL CToolMenu::DestroyMenu()
{
	DestroyMenuData(m_hMenu,0);
	return CMenu::DestroyMenu();
}


