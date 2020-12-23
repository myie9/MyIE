//_ **********************************************************
//_ 
//_ Name: EnTabCtrl.cpp 
//_ Purpose: 
//_ Created: 15 September 1998 
//_ Author: D.R.Godson
//_ Modified By: 
//_ 
//_ Copyright (c) 1998 Brilliant Digital Entertainment Inc. 
//_ 
//_ **********************************************************

// EnTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EnTabCtrl.h"
#include "Consts_gf.h"
#include "Gf_AppStatic.h"
#include "MainFrm.h"
#include "Gf_Img.h"
#include "MyIE9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnTabCtrl

static const COLORREF BOTTOM_SKIN_LINE = 0xACACAC;

static const int FIRST_DRAW = 25; //靠拼接方式来显示任何长度的tab背景图
static const int MID_DRAW = 40;

DWORD CEnTabCtrl::s_dwCustomLook = 0;

enum { PADDING = 3, EDGE = 20};



extern BOOL g_bShowDot;
//////////////////////////////////////////////////////////////////////////////
// helpers
CEnTabCtrl::CEnTabCtrl()
{
	m_bMoveToCloseIcon = FALSE;
}

CEnTabCtrl::~CEnTabCtrl()
{
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CEnTabCtrl message handlers

void CEnTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());
	
	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);
	
	CRect rItem(lpdis->rcItem);
	
	CRect rOri = rItem; //不变的原始值
	
	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	
	BOOL bExistTabImg = pmf->hbTabBkBottomActive && pmf->hbTabBkBottom && pmf->hbTabBkTopActive && pmf->hbTabBkTop;
	
	if (IsSkin() )
	{
		
		try
		{
			
			if (bBottom)
			{
				if (bSelected)
				{
					CGf_Img::LongWidth(pDC, pmf->hbTabBkBottomActive, rOri, FALSE, rOri.top-1, FIRST_DRAW, MID_DRAW, PIC_MASK);				
				}
				else
				{
					CGf_Img::LongWidth(pDC, pmf->hbTabBkBottom, rOri, FALSE, rOri.top-1, FIRST_DRAW, MID_DRAW, PIC_MASK);
				}
			}
			else
			{
				if (bSelected)
				{
					CGf_Img::LongWidth(pDC, pmf->hbTabBkTopActive, rOri, TRUE, 2, FIRST_DRAW, MID_DRAW, PIC_MASK);				
				}
				else
				{
					CGf_Img::LongWidth(pDC, pmf->hbTabBkTop, rOri, TRUE, 2, FIRST_DRAW, MID_DRAW, PIC_MASK);
				}
			}

			
		}
		catch (...) 
		{
			ASSERT(false);
			pmf->m_nTabStyle = FLAT_TAB;
			m_nTabStyle = pmf->m_nTabStyle;
			
			//CMyIE9App* app = (CMyIE9App*)AfxGetApp();
			//app->WriteProfileInt("TabStyle","TabStyle2",m_nTabStyle);
			
			pmf->PostMessage(WM_UPDATE_TAB, TRUE);
			
			return;
 		}
	}
	else if (m_nTabStyle==0 && !bSelected)
		//gf if (m_nTabStyle==0 && !bSelected)
	{
		if (bBottom)
		{
			rItem.left -= 1;
			rItem.top -= 2;
		}
		else
		{
			rItem.left -= 1;
			rItem.bottom += 2;
		}
	}
	else if (m_nTabStyle==1)
		//gf else if( (m_nTabStyle==1) || !bExistTabImg )
	{
		if (bBottom)
		{
			rItem.bottom += 2;
			if (bSelected)
				rItem.top -= 1;
		}
		else
		{
			rItem.top -= 2;
			if (bSelected)
			{
				rItem.bottom += 1;
				rItem.bottom += 2; //gfdelborder
			}
			
		}
	}
	
	// blend from back color to COLOR_3DFACE if 16 bit mode or better
	COLORREF crFrom = GetTabColor(bSelected);
	
	// simple solid fill
	if (!IsSkin() )
	{
		CRect rTemp = rOri;
		rTemp.top-=2;
		rTemp.bottom+=3;
		
		pDC->FillSolidRect(rTemp, crFrom);
	}
	
	
	int topPad = PADDING+1;
	
	if (IsSkin() )
	{
		if (bBottom)
		{
			rItem.top -= PADDING;
		}
		else
		{
			rItem.top+= 1;
		}
		
		
	}
	
	// text & icon
	rItem.left += PADDING;
	rItem.top += topPad;
	//
	if (m_nTabStyle==0 && !bSelected)
	{
		if (bBottom)
			rItem.top-=2;
		else
			rItem.top--;
	}
	
	pDC->SetBkMode(TRANSPARENT);
	
	char cTemp[100];
	
	
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = cTemp; //gf sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	//gf sTemp.ReleaseBuffer();
	CString sTemp;
	sTemp.Format("%s", cTemp);
	
	// icon
	if (hilTabs && tci.iImage>=0)
	{
		ImageList_Draw(hilTabs, tci.iImage, *pDC, rItem.left, rItem.top-1, ILD_TRANSPARENT);
		rItem.left += 16 + PADDING;//16+
	}
	
	//gfx
	if(CGf_AppStatic::_instance._bShowXIconInTab && hilTabs && bSelected)
	{
		rItem.right -= GF_X_ICON_SIZE + PADDING;
		
		int iCloseIcon=GF_TI_X;
		
		if (m_bMoveToCloseIcon)
		{
			iCloseIcon = GF_TI_X_ACTIVE;
		}
		
		ImageList_Draw(hilTabs, iCloseIcon, *pDC, rItem.right, rItem.top-1, ILD_TRANSPARENT);
		
	}
	
	
	// text
	rItem.right -= PADDING;
	
	//gf
	if (IsSkin())
	{
		rItem.top+=1;
	}
	
	
	//FormatText(sTemp, pDC, rItem.Width());
	
	if (bSelected)
		pDC->SetTextColor(m_crSelFontColor);
	else
		pDC->SetTextColor(m_crUnSelFontColor);
	pDC->SelectObject(&m_UnselFont);
	if (g_bShowDot)
		pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_END_ELLIPSIS );
	else
		pDC->DrawText(sTemp, rItem, DT_NOPREFIX  );
}

void CEnTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	//gfsd ASSERT (s_dwCustomLook & ETC_FLAT);
	
	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);
	
	CRect rItem(lpdis->rcItem);
	CRect rOri = rItem;
	
	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	if (bBottom)
	{
		rItem.bottom += 2;
	}
	else
	{
		rItem.top -= 2;
		rItem.bottom += 3; //gfdelborder
	}
	
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 
	
	//gfb
	if (!bSelected && bBottom)
	{
		//下黑
		UINT clr = 0;
		
		if (IsSkin() )
		{
			clr= BOTTOM_SKIN_LINE;
		}
		
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top - 1), clr);
		
	}
	
	if (bSelected && IsSkin() )
	{
		return;
	}
	
	COLORREF crTab = GetTabColor(bSelected);
	
	if (bSelected)
	{
		BOOL bBottom = GetStyle() & TCS_BOTTOM;
		if (bBottom)
			rItem.top -= 1;
		else
			rItem.bottom += 1;
		
		// edges
		//left
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), 0xFFFFFF); 
		
		if (bBottom)
			pDC->FillSolidRect(CRect(rItem.left, rItem.bottom, rItem.right, rItem.bottom - 1), BOTTOM_SKIN_LINE);
		//else
		//	pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), 0xFFFFFF);
		
		//pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), 0x433F3E);
		pDC->FillSolidRect(CRect(rItem.right - 1, rOri.top-1, rItem.right, rOri.bottom+2), 0x433F3E);
	}
	else if(!IsSkin() ) // draw simple dividers
	{
		
		pDC->FillSolidRect(CRect(rItem.right, rItem.top+4, rItem.right+1, rItem.bottom-4), m_crSepColor);
	}
}

void CEnTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 
	
	BOOL bBottom = GetStyle() & TCS_BOTTOM;
	if (bBottom)
	{
		//rItem.top = rItem.bottom-1;
		//pDC->FillSolidRect(rItem, 0);
		
		rItem.top=rItem.bottom-1;
		
		if (IsSkin())
		{			
			pDC->FillSolidRect(rItem, BOTTOM_SKIN_LINE);
		}
		else
		{
			pDC->FillSolidRect(rItem, 0);
		}
	}
	
	//gfdelborder del below
	//	else 
	//	{
	//		rItem.bottom = rItem.top+1 ;
	//		pDC->FillSolidRect(rItem, 0xFFFFFF);
	//	}
}

COLORREF CEnTabCtrl::GetTabColor(BOOL bSelected)
{
	if (!bSelected)
		return m_crUnSelColor;
	else
		return m_crSelColor;// RGB(215,231,251);//gf ::GetSysColor(COLOR_3DFACE);
}

void CEnTabCtrl::EnableCustomLook(BOOL bEnable, DWORD dwStyle)
{
	if (!bEnable)
		dwStyle = 0;
	
	s_dwCustomLook = dwStyle;
}

void CEnTabCtrl::PreSubclassWindow() 
{
	CBaseTabCtrl::PreSubclassWindow();
	
	if (s_dwCustomLook)
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

void CEnTabCtrl::SetFonts(int nSelWeight,   BOOL bSelItalic,   BOOL bSelUnderline,
						  int nUnselWeight, BOOL bUnselItalic, BOOL bUnselUnderline)
{
	// Free any memory currently used by the fonts.
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
	
	LOGFONT lFont;
	ZeroMemory ((PVOID) &lFont,sizeof (LOGFONT));
	NONCLIENTMETRICS nm;
	nm.cbSize = sizeof (NONCLIENTMETRICS);
	VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
	lFont =  nm.lfMenuFont;
	
	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bSelItalic;
	lFont.lfUnderline = bSelUnderline;
	m_SelFont.CreateFontIndirect(&lFont);
	
	// Create the "Unselected" font
	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bUnselItalic;
	lFont.lfUnderline = bUnselUnderline;
	m_UnselFont.CreateFontIndirect(&lFont);
	
	SetFont( (nSelWeight > nUnselWeight)? &m_SelFont : &m_UnselFont);
}

LONG CEnTabCtrl::GetCloseIconLeft(const RECT& rect)
{
	return rect.right - GF_X_ICON_SIZE - PADDING;
}

BOOL CEnTabCtrl::IsInCloseIconArea(int id, const CPoint& point)
{
	RECT rect;
	GetItemRect(id, &rect);
	
	LONG iconLeft = GetCloseIconLeft(rect);
	
	BOOL isInCloseIcon = (point.x>iconLeft) && (point.x< (iconLeft + GF_X_ICON_SIZE) ) && (point.y > (rect.top+ GF_X_ICON_TOP_PADDING ) ) && (point.y < (rect.bottom - GF_X_ICON_TOP_PADDING) );
	
	if ((GetCurSel() == id ) && isInCloseIcon)
	{
		return TRUE;
	}
	
	return FALSE;
}